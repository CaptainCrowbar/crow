#include "crow/path.hpp"
#include "crow/format.hpp"
#include "crow/guard.hpp"
#include "crow/unicode.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

using namespace Crow;

namespace {

    template <typename T, typename Range>
    size_t index_of(const T& t, const Range& range) {
        using std::begin;
        using std::end;
        auto it = std::find(begin(range), end(range), t);
        return it == end(range) ? std::string::npos : it - begin(range);
    }

}

void test_crow_path_directory_iterators() {

    Path dir;
    Path::directory_range range;
    std::vector<Path> files;

    TRY(range = dir.directory());
    REQUIRE(! range.empty());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TRY(std::sort(files.begin(), files.end()));

    #ifdef _MSC_VER
        TEST_EQUAL(files[0], Path("ALL_BUILD.vcxproj"));
    #else
        TEST_EQUAL(files[0], Path("CMakeCache.txt"));
    #endif

    TRY(dir = "../source/crow");
    TRY(range = dir.directory());
    REQUIRE(! range.empty());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TRY(std::sort(files.begin(), files.end()));
    TEST_EQUAL(files[0], Path("../source/crow/algorithm.hpp"));

}

void test_crow_path_current_directory() {

    Path dot = ".", parent = "..", test = "__test_cwd__";
    Path abs_dot, abs_test, cwd;

    TRY(abs_dot = dot.resolve());
    TRY(abs_test = test.resolve());
    TRY(cwd = Path::current_directory());
    TEST_EQUAL(cwd, abs_dot);
    TRY(test.make_directory());
    TEST(test.exists());
    TEST(test.is_directory());
    TRY(test.change_directory());
    TRY(cwd = Path::current_directory());
    TEST_EQUAL(cwd, abs_test);
    TRY(parent.change_directory());
    TRY(cwd = Path::current_directory());
    TEST_EQUAL(cwd, abs_dot);
    TRY(test.remove());

}

void test_crow_path_deep_search() {

    Path root = "__test_root__";
    Path::search_range range;
    std::vector<Path> files;
    std::string s;
    auto guard = on_scope_exit([=] { root.remove(Path::flag::recurse); });

    TEST(! root.exists());
    TRY(range = root.deep_search());
    TEST(range.empty());

    TRY(root.make_directory());
    TEST(root.exists());
    TEST(root.is_directory());
    TRY(range = root.deep_search());
    TEST(range.empty());

    TRY((root / "foo").make_directory());
    TRY((root / "bar").make_directory());
    TRY((root / "zap").make_directory());
    TRY((root / "foo/alpha").create());
    TRY((root / "foo/bravo").create());
    TRY((root / "foo/charlie").create());
    TRY((root / "bar/foobar").make_directory());
    TRY((root / "bar/foobar/xray").create());
    TRY((root / "bar/foobar/yankee").create());
    TRY((root / "bar/foobar/zulu").create());

    TRY(range = root.deep_search());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TEST_EQUAL(files.size(), 10u);
    TEST(index_of(Path("__test_root__/foo"), files) < index_of(Path("__test_root__/foo/alpha"), files));
    TEST(index_of(Path("__test_root__/bar"), files) < index_of(Path("__test_root__/bar/foobar"), files));
    TEST(index_of(Path("__test_root__/bar/foobar"), files) < index_of(Path("__test_root__/bar/foobar/xray"), files));
    std::sort(files.begin(), files.end());
    TRY(s = format_range(files));
    std::replace(s.begin(), s.end(), '\\', '/');
    TEST_EQUAL(s, "["
        "__test_root__/bar,"
        "__test_root__/bar/foobar,"
        "__test_root__/bar/foobar/xray,"
        "__test_root__/bar/foobar/yankee,"
        "__test_root__/bar/foobar/zulu,"
        "__test_root__/foo,"
        "__test_root__/foo/alpha,"
        "__test_root__/foo/bravo,"
        "__test_root__/foo/charlie,"
        "__test_root__/zap"
    "]");

    TRY(range = root.deep_search(Path::flag::bottom_up));
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TEST_EQUAL(files.size(), 10u);
    TEST(index_of(Path("__test_root__/foo"), files) > index_of(Path("__test_root__/foo/alpha"), files));
    TEST(index_of(Path("__test_root__/bar"), files) > index_of(Path("__test_root__/bar/foobar"), files));
    TEST(index_of(Path("__test_root__/bar/foobar"), files) > index_of(Path("__test_root__/bar/foobar/xray"), files));
    std::sort(files.begin(), files.end());
    TRY(s = format_range(files));
    std::replace(s.begin(), s.end(), '\\', '/');
    TEST_EQUAL(s, "["
        "__test_root__/bar,"
        "__test_root__/bar/foobar,"
        "__test_root__/bar/foobar/xray,"
        "__test_root__/bar/foobar/yankee,"
        "__test_root__/bar/foobar/zulu,"
        "__test_root__/foo,"
        "__test_root__/foo/alpha,"
        "__test_root__/foo/bravo,"
        "__test_root__/foo/charlie,"
        "__test_root__/zap"
    "]");

}
