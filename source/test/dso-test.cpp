#include "crow/dso.hpp"
#include "crow/path.hpp"
#include "crow/unit-test.hpp"
#include <functional>
#include <system_error>
#include <utility>

using namespace Crow;

void test_crow_dynamic_library() {

    Path dir, file;
    Dso lib1, lib2, lib3, lib4;

    TRY(lib1 = Dso::self());
    TEST_THROW(Dso("no-such-thing"), std::system_error);

    #ifdef _XOPEN_SOURCE

        #ifdef __APPLE__
            dir = "/usr/local/lib";
            file = "libpng.dylib";
        #else
            dir = "/usr/lib/x86_64-linux-gnu";
            file = "libpng.so";
        #endif

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
