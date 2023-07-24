#include "crow/dso.hpp"
#include "crow/path.hpp"
#include "crow/regex.hpp"
#include "crow/unit-test.hpp"
#include <cstdlib>
#include <functional>
#include <string>
#include <stdexcept>
#include <system_error>
#include <utility>

using namespace Crow;

// #ifdef __APPLE__

//     namespace {

//         Path find_xcode_root() {
//             Path out = "__dso_test__";
//             auto cmd = "clang++ --version | grep InstalledDir >" + out.name();
//             std::system(cmd.data());
//             std::string line;
//             out.load(line);
//             out.remove();
//             Regex pattern("InstalledDir: (.+)\n", Regex::full);
//             auto match = pattern(line);
//             if (! match)
//                 throw std::runtime_error("Can't find Xcode installation root");
//             auto name = match[1];
//             return Path(name);
//         }

//     }

// #endif

void test_crow_dso_loading() {

    Path dir, file;
    Dso lib1, lib2, lib3, lib4;

    TRY(lib1 = Dso::self());
    TEST_THROW(Dso("no-such-thing"), std::system_error);

    #ifdef __APPLE__

        // TODO - Temporarily removed because of issues with Xcode 15 beta

        // TRY(dir = find_xcode_root());
        // REQUIRE(dir.is_directory());
        // dir /= "Contents/Developer/usr/lib";
        // file = "libxcrun.dylib";

        // REQUIRE(dir.is_directory());
        // Path path = dir / file;
        // REQUIRE(path.exists());

        // TRY(lib1 = Dso(path));
        // TRY(lib2 = Dso(file));
        // TRY(lib3 = Dso::search("xcrun"));
        // TEST(lib1);
        // TEST(lib2);
        // TEST(lib3);
        // REQUIRE(lib1 || lib2 || lib3);

        // if (! lib1 && ! lib2)
        //     lib1 = std::move(lib3);
        // else if (! lib1)
        //     lib1 = std::move(lib2);

        // TODO - Function tests for Apple

    #elifdef _XOPEN_SOURCE

        dir = "/usr/lib/x86_64-linux-gnu";
        file = "libpng.so";

        REQUIRE(dir.is_directory());
        Path path = dir / file;
        REQUIRE(path.exists());

        TRY(lib1 = Dso(path));
        TRY(lib2 = Dso(file));
        TRY(lib3 = Dso::search("png"));
        TEST(lib1);
        TEST(lib2);
        TEST(lib3);
        REQUIRE(lib1 || lib2 || lib3);

        if (! lib1 && ! lib2)
            lib1 = std::move(lib3);
        else if (! lib1)
            lib1 = std::move(lib2);

        using fun_ptr_type = uint32_t (*)();
        using fun_obj_type = std::function<uint32_t()>;

        fun_ptr_type fp = nullptr;
        fun_obj_type fo = nullptr;
        uint32_t rc = 0;

        TRY(fp = lib1.symbol<fun_ptr_type>("png_access_version_number"));
        TEST(fp);
        if (fp) {
            TRY(rc = fp());
            TEST(rc > 10'000u);
        }

        TRY(fo = lib1.symbol<fun_obj_type>("png_access_version_number"));
        TEST(fo);
        if (fo) {
            TRY(rc = fo());
            TEST(rc > 10'000u);
        }

        fp = nullptr;
        TEST(lib1.symbol("png_access_version_number", fp));
        TEST(fp);
        if (fp) {
            TRY(rc = fp());
            TEST(rc > 10'000u);
        }

        fo = nullptr;
        TEST(lib1.symbol("png_access_version_number", fo));
        TEST(fo);
        if (fo) {
            TRY(rc = fo());
            TEST(rc > 10'000u);
        }

    #else

        dir = "C:\\Windows\\System32";
        file = "kernel32.dll";

        Path path = dir / file;
        REQUIRE(path.exists());

        TRY(lib1 = Dso(path));
        TRY(lib2 = Dso(file));
        TRY(lib3 = Dso::search("kernel32"));
        TEST(lib1);
        TEST(lib2);
        TEST(lib3);
        REQUIRE(lib1 || lib2 || lib3);

        if (! lib1 && ! lib2)
            lib1 = std::move(lib3);
        else if (! lib1)
            lib1 = std::move(lib2);

        using fun_ptr_type = uint64_t (*)();
        using fun_obj_type = std::function<uint64_t()>;

        fun_ptr_type fp = nullptr;
        fun_obj_type fo = nullptr;
        uint64_t rc = 0;

        TRY(fp = lib1.symbol<fun_ptr_type>("GetTickCount64"));
        TEST(fp);
        if (fp) {
            TRY(rc = fp());
            TEST(rc != 0);
        }

        TRY(fo = lib1.symbol<fun_obj_type>("GetTickCount64"));
        TEST(fo);
        if (fo) {
            TRY(rc = fo());
            TEST(rc != 0);
        }

        fp = nullptr;
        TEST(lib1.symbol("GetTickCount64", fp));
        TEST(fp);
        if (fp) {
            TRY(rc = fp());
            TEST(rc != 0);
        }

        fo = nullptr;
        TEST(lib1.symbol("GetTickCount64", fo));
        TEST(fo);
        if (fo) {
            TRY(rc = fo());
            TEST(rc != 0);
        }

    #endif

}
