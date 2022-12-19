#include "crow/path.hpp"
#include "crow/format.hpp"
#include "crow/unicode.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iterator>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

using namespace Crow;
using namespace std::chrono;
using namespace std::literals;

void test_crow_path_resolution() {

    Path cwd, f1, f2;
    std::string cwdstr;

    TRY(cwd = Path::current_directory());
    TRY(cwdstr = cwd.name());
    std::replace(cwdstr.begin(), cwdstr.end(), '\\', '/');
    TEST_MATCH(cwdstr, "/crow/");

    TRY(f1 = Path());                     TRY(f2 = f1.resolve());  TEST_EQUAL(f2, cwd);
    TRY(f1 = Path("C MakeCache.txt"));    TRY(f2 = f1.resolve());  TEST_EQUAL(f2, cwd / "C MakeCache.txt");
    TRY(f1 = Path("."));                  TRY(f2 = f1.resolve());  TEST_EQUAL(f2, cwd);
    TRY(f1 = Path("./C MakeCache.txt"));  TRY(f2 = f1.resolve());  TEST_EQUAL(f2, cwd / "C MakeCache.txt");

    #ifdef _XOPEN_SOURCE

        #ifdef _XOPEN_SOURCE
            auto cptr = std::getenv("HOME");
            REQUIRE(cptr != nullptr);
            Path home = cptr;
        #endif

        TRY(f1 = Path("~"));            TRY(f2 = f1.resolve());  TEST_EQUAL(f2, home);
        TRY(f1 = Path("~/Documents"));  TRY(f2 = f1.resolve());  TEST_EQUAL(f2, home / "Documents");
        TRY(f1 = Path("/"));            TRY(f2 = f1.resolve());  TEST_EQUAL(f2, Path("/"));
        TRY(f1 = Path("/usr"));         TRY(f2 = f1.resolve());  TEST_EQUAL(f2, Path("/usr"));
        TRY(f1 = Path("/usr/local"));   TRY(f2 = f1.resolve());  TEST_EQUAL(f2, Path("/usr/local"));

    #else

        TRY(f1 = Path("C:/"));         TRY(f2 = f1.resolve());  TEST_EQUAL(f2, Path("C:/"));
        TRY(f1 = Path("C:/Windows"));  TRY(f2 = f1.resolve());  TEST_EQUAL(f2, Path("C:/Windows"));

    #endif

}

void test_crow_path_file_system_queries() {

    static const Path empty = "";
    static const Path dot1 = ".";
    static const Path dot2 = "..";
    static const Path cmcache = "CMakeCache.txt";
    static const Path cmfiles = "CMakeFiles";
    static const Path none = "no such file";

    #ifdef _XOPEN_SOURCE
        static const Path root = "/";
        static const Path null = "/dev/null";
    #else
        static const Path root = "C:/";
        static const Path null = "NUL";
    #endif

    Path::id_type id0 = {}, id1 = {}, id2 = {};

    TEST(! empty.exists());  TEST(! empty.is_directory());    TEST(! empty.is_file());    TEST(! empty.is_special());    TEST(! empty.is_hidden());    TEST(! empty.is_symlink());
    TEST(dot1.exists());     TEST(dot1.is_directory());       TEST(! dot1.is_file());     TEST(! dot1.is_special());     TEST(! dot1.is_hidden());     TEST(! dot1.is_symlink());
    TEST(dot2.exists());     TEST(dot2.is_directory());       TEST(! dot2.is_file());     TEST(! dot2.is_special());     TEST(! dot2.is_hidden());     TEST(! dot2.is_symlink());
    TEST(cmcache.exists());  TEST(! cmcache.is_directory());  TEST(cmcache.is_file());    TEST(! cmcache.is_special());  TEST(! cmcache.is_hidden());  TEST(! cmcache.is_symlink());
    TEST(cmfiles.exists());  TEST(cmfiles.is_directory());    TEST(! cmfiles.is_file());  TEST(! cmfiles.is_special());  TEST(! cmfiles.is_hidden());  TEST(! cmfiles.is_symlink());
    TEST(! none.exists());   TEST(! none.is_directory());     TEST(! none.is_file());     TEST(! none.is_special());     TEST(! none.is_hidden());     TEST(! none.is_symlink());
    TEST(root.exists());     TEST(root.is_directory());       TEST(! root.is_file());     TEST(! root.is_special());     TEST(! root.is_hidden());     TEST(! root.is_symlink());
    TEST(null.exists());     TEST(! null.is_directory());     TEST(! null.is_file());     TEST(null.is_special());       TEST(! null.is_hidden());     TEST(! null.is_symlink());

    Path::kind k = {};

    TRY(k = empty.file_kind());    TEST_EQUAL(k, Path::kind::none);
    TRY(k = dot1.file_kind());     TEST_EQUAL(k, Path::kind::directory);
    TRY(k = dot2.file_kind());     TEST_EQUAL(k, Path::kind::directory);
    TRY(k = cmcache.file_kind());  TEST_EQUAL(k, Path::kind::file);
    TRY(k = cmfiles.file_kind());  TEST_EQUAL(k, Path::kind::directory);
    TRY(k = none.file_kind());     TEST_EQUAL(k, Path::kind::none);
    TRY(k = root.file_kind());     TEST_EQUAL(k, Path::kind::directory);
    TRY(k = null.file_kind());     TEST_EQUAL(k, Path::kind::special);

    #ifdef _XOPEN_SOURCE
        TEST(Path("/").exists());
        TEST(! Path("C:/").exists());
        TEST(Path(".git").is_hidden());
    #else
        TEST(Path("C:/").exists());
        TEST(Path("C:").exists());
        TEST(Path("/").exists());
    #endif

    TEST(Path().id() == id0);
    TEST(none.id() == id0);
    TRY(id1 = cmcache.id());
    TRY(id2 = cmfiles.id());
    TEST(id1 > id0);
    TEST(id2 > id0);
    TEST(id2 != id1);

    TEST(cmcache.size() > 10'000);
    TEST(cmcache.size(Path::recurse) > 10'000);
    TEST_EQUAL(none.size(), 0u);
    TEST_EQUAL(none.size(Path::recurse), 0u);
    TEST(Path(".").size(Path::recurse) > 1'000'000);
    TEST(cmfiles.size(Path::recurse) > 100'000);

}

void test_crow_path_file_system_updates() {

    Path d1, d2, d3, f1, f2, f3, f4;
    std::vector<Path> files;
    Path::directory_range range;

    TRY(f1 = "__test_file_1__");
    TRY(f2 = "__test_file_2__");
    TRY(f3 = "__test_file_3__");
    TRY(f4 = "__test_FILE_3__");

    TEST(! f1.exists());
    TEST(! f1.is_directory());
    TEST(! f2.exists());
    TEST(! f2.is_directory());

    TRY(f1.create());
    TEST(f1.exists());
    TRY(f1.move_to(f2));
    TEST(! f1.exists());
    TEST(f2.exists());
    TRY(f2.copy_to(f3));
    TEST(f2.exists());
    TEST(f3.exists());
    TRY(f3.move_to(f4));
    TEST(f4.exists());

    TRY(f1.remove());
    TRY(f2.remove());
    TRY(f3.remove());
    TRY(f4.remove());

    TEST(! f1.exists());
    TEST(! f2.exists());
    TEST(! f3.exists());
    TEST(! f4.exists());

    TRY(d1 = "__test_dir_1__");
    TRY(d2 = "__test_dir_2__");
    TRY(d3 = "__test_dir_3__");

    TEST(! d1.exists());
    TEST(! d1.is_directory());
    TEST(! d2.exists());
    TEST(! d2.is_directory());

    TRY(d1.make_directory());
    TEST(d1.exists());
    TEST(d1.is_directory());
    TRY(d1.make_directory());
    TRY(d1.move_to(d2));
    TEST(! d1.exists());
    TEST(! d1.is_directory());
    TEST(d2.exists());
    TEST(d2.is_directory());
    TEST_THROW(d2.copy_to(d3), std::system_error);
    TRY(d2.copy_to(d3, Path::recurse));
    TEST(d2.exists());
    TEST(d2.is_directory());
    TEST(d3.exists());
    TEST(d3.is_directory());

    TRY(d1.remove());
    TRY(d2.remove());
    TRY(d3.remove());

    TEST(! d1.exists());
    TEST(! d1.is_directory());
    TEST(! d2.exists());
    TEST(! d2.is_directory());
    TEST(! d3.exists());
    TEST(! d3.is_directory());

    TRY(d1 = "__test_dir_1__");
    TRY(d2 = "__test_dir_1__/subdir");

    TEST(! d1.exists());
    TEST(! d2.exists());

    TEST_THROW(d2.make_directory(), std::system_error);
    TRY(d2.make_directory(Path::recurse));
    TEST(d1.exists());
    TEST(d2.exists());
    TEST(d1.is_directory());
    TEST(d2.is_directory());
    TRY(d2.make_directory(Path::recurse));
    TEST_THROW(d1.remove(), std::system_error);
    TRY(d1.remove(Path::recurse));

    std::this_thread::sleep_for(10ms);

    TEST(! d1.exists());
    TEST(! d2.exists());

    TRY(d1 = "__test_dir_1__");
    TRY(f1 = d1 / "hello");
    TRY(f2 = d1 / "world");

    TRY(range = d1.directory());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TEST_EQUAL(files.size(), 0u);
    TEST_EQUAL(format_range(files), "[]");

    TRY(d1.make_directory());
    TEST(d1.exists());
    TEST(d1.is_directory());
    TRY(range = d1.directory());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TEST_EQUAL(files.size(), 0u);
    TEST_EQUAL(format_range(files), "[]");

    TRY(f1.create());
    TEST(f1.exists());
    TRY(f2.create());
    TEST(f2.exists());

    TEST_THROW(f1.make_directory(), std::system_error);
    TRY(range = d1.directory());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TEST_EQUAL(files.size(), 2u);
    std::sort(files.begin(), files.end());

    #ifdef _XOPEN_SOURCE
        TEST_EQUAL(format_range(files), "[__test_dir_1__/hello,__test_dir_1__/world]");
    #else
        TEST_EQUAL(format_range(files), "[__test_dir_1__\\hello,__test_dir_1__\\world]");
    #endif

    TEST_THROW(d1.remove(), std::system_error);
    TEST(d1.exists());
    TEST(f1.exists());
    TEST(f2.exists());

    TRY(d1.remove(Path::recurse));
    TEST(! d1.exists());
    TEST(! f1.exists());
    TEST(! f2.exists());

    TRY(range = d1.directory());
    files.clear();
    TRY(std::copy(range.begin(), range.end(), std::back_inserter(files)));
    TEST_EQUAL(files.size(), 0u);
    TEST_EQUAL(format_range(files), "[]");

}

void test_crow_path_io() {

    Path cmcache = "CMakeCache.txt";
    Path testfile = "__test_io__";
    Path nofile = "__no_such_file__";
    std::string s;

    TRY(cmcache.load(s));
    TEST_EQUAL(s.size(), cmcache.size());
    TEST_EQUAL(s.substr(0, 30), "# This is the CMakeCache file.");
    TRY(cmcache.load(s, 10));
    TEST_EQUAL(s, "# This is ");

    TRY(testfile.save("Hello world\n"));
    TEST(testfile.exists());
    TEST_EQUAL(testfile.size(), 12u);

    TRY(testfile.load(s));
    TEST_EQUAL(s, "Hello world\n");
    TRY(testfile.save("Goodbye\n", Path::append));
    TEST_EQUAL(testfile.size(), 20u);
    TRY(testfile.load(s));
    TEST_EQUAL(s, "Hello world\nGoodbye\n");

    TEST_THROW(nofile.load(s), std::system_error);
    TRY(nofile.load(s, npos, Path::may_fail));
    TEST(s.empty());

    TRY(testfile.remove());
    TEST(! testfile.exists());

}

void test_crow_path_links() {

    Path file = "__test_sym_file__";
    Path link = "__test_sym_link__";
    size_t bytes = 1'000'000;
    std::string text(bytes, 'x');

    TEST(! file.exists());
    TEST(! link.exists());

    TRY(file.save(text));
    TEST(file.exists());
    TEST_EQUAL(file.size(), bytes);

    #ifdef _XOPEN_SOURCE

        TRY(file.make_symlink(link));
        TEST(link.exists());
        TEST(link.is_symlink());
        TEST_EQUAL(link.resolve_symlink(), file);
        TEST_EQUAL(link.size(), bytes);
        TEST_EQUAL(link.size(Path::no_follow), file.name().size());

        Path::kind k;

        TRY(k = link.file_kind());
        TEST_EQUAL(k, Path::kind::file);
        TRY(k = link.file_kind(Path::no_follow));
        TEST_EQUAL(k, Path::kind::symlink);

    #else

        TEST_THROW(file.make_symlink(link), std::system_error);
        TRY(file.make_symlink(link, Path::may_copy));
        TEST(link.exists());
        TEST(! link.is_symlink());
        TEST_EQUAL(link.size(), bytes);

    #endif

    TRY(link.remove());
    TEST(! link.exists());
    TEST(file.exists());
    TRY(file.remove());
    TEST(! file.exists());

}

void test_crow_path_metadata() {

    Path file = "__test_meta_file__";
    seconds sec;
    system_clock::time_point date, epoch;

    TEST(! file.exists());
    TRY(date = file.access_time());
    TRY(sec = duration_cast<seconds>(date - epoch));
    TEST_EQUAL(sec.count(), 0);
    TRY(date = file.modify_time());
    TRY(sec = duration_cast<seconds>(date - epoch));
    TEST_EQUAL(sec.count(), 0);

    TRY(file.create());
    TEST(file.exists());
    TRY(date = file.access_time());
    TRY(sec = duration_cast<seconds>(system_clock::now() - date));
    TEST_NEAR(sec.count(), 0, 2);
    TRY(date = file.modify_time());
    TRY(sec = duration_cast<seconds>(system_clock::now() - date));
    TEST_NEAR(sec.count(), 0, 2);

    TRY(file.set_access_time(system_clock::now() - 1min));
    TRY(date = file.access_time());
    TRY(sec = duration_cast<seconds>(system_clock::now() - date));
    TEST_NEAR(sec.count(), 60, 2);

    TRY(file.set_modify_time(system_clock::now() - 2min));
    TRY(date = file.modify_time());
    TRY(sec = duration_cast<seconds>(system_clock::now() - date));
    TEST_NEAR(sec.count(), 120, 2);

    TRY(file.remove());
    TEST(! file.exists());

}
