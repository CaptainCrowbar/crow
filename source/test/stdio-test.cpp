#include "crow/stdio.hpp"
#include "crow/guard.hpp"
#include "crow/io-utility.hpp"
#include "crow/path.hpp"
#include "crow/unit-test.hpp"
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace Crow;

#ifdef _XOPEN_SOURCE
    #define SLASH "/"
#else
    #define SLASH "\\"
#endif

void test_crow_stdio_cstdio() {

    Cstdio io;
    Path file = "__cstdio_test__", no_file = "__no_such_file__", path;
    std::string text;
    std::vector<std::string> vec;
    ptrdiff_t offset = 0;
    size_t n = 0;
    auto guard = on_scope_exit([=] { file.remove(); });

    TRY(file.remove());
    TEST(! file.exists());
    TEST(! io.is_open());

    TRY(io = Cstdio(file, IoMode::write));
    TEST(io.is_open());
    TRY(io.writes("Hello world\n"));
    TRY(io.writes("Goodbye\n"));
    TRY(io.close());
    TEST(! io.is_open());
    TEST(file.exists());

    text.clear();
    TRY(io = Cstdio(file));
    TRY(n = io.read_some(text, 12));  TEST_EQUAL(n, 12u);  TEST_EQUAL(text, "Hello world\n");
    TRY(n = io.read_some(text, 8));   TEST_EQUAL(n, 8u);   TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(n = io.read_some(text, 1));   TEST_EQUAL(n, 0u);   TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(text = io.reads(12));  TEST_EQUAL(text, "Hello world\n");
    TRY(text = io.reads(8));   TEST_EQUAL(text, "Goodbye\n");
    TRY(text = io.reads(1));   TEST_EQUAL(text, "");
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(text = io.reads(20));  TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(text = io.reads(1));   TEST_EQUAL(text, "");
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(text = io.reads(100));  TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(text = io.read_line());  TEST_EQUAL(text, "Hello world\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "Goodbye\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "");
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(text = io.read_all());  TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(io.close());

    TRY(io = Cstdio(file));
    auto lines = io.lines();
    vec.clear();
    TRY(std::copy(lines.begin(), lines.end(), std::back_inserter(vec)));
    TEST_EQUAL(vec.size(), 2u);
    TEST_EQUAL(vec.at(0), "Hello world\n");
    TEST_EQUAL(vec.at(1), "Goodbye\n");
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 0);
    TRY(io.seek(12));
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 12);
    TRY(text = io.reads(7));
    TEST_EQUAL(text, "Goodbye");
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 19);
    TRY(io.seek(-13));
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 6);
    TRY(text = io.reads(5));
    TEST_EQUAL(text, "world");
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 11);
    TRY(io.close());

    TEST_THROW(io = Cstdio(no_file, IoMode::read), IoError);

    TRY(io = Cstdio(file, IoMode::write));
    TRY(io.format("Agent {0}\n", 86));
    TRY(io.print("Agent", 99));
    TRY(io.write_chars(10, '*'));
    TRY(io.close());

    TRY(io = Cstdio(file));
    TRY(text = io.read_line());  TEST_EQUAL(text, "Agent 86\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "Agent 99\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "**********");
    TRY(text = io.read_line());  TEST_EQUAL(text, "");
    TRY(io.close());

}

void test_crow_stdio_fdio() {

    Fdio io;
    Path file = "__fdio_test__", no_file = "__no_such_file__", path;
    std::string text;
    std::vector<std::string> vec;
    ptrdiff_t offset = 0;
    size_t n = 0;
    auto guard = on_scope_exit([=] { file.remove(); });

    TRY(file.remove());
    TEST(! file.exists());
    TEST(! io.is_open());

    TRY(io = Fdio(file, IoMode::write));
    TEST(io.is_open());
    TRY(io.writes("Hello world\n"));
    TRY(io.writes("Goodbye\n"));
    TRY(io.close());
    TEST(! io.is_open());
    TEST(file.exists());

    text.clear();
    TRY(io = Fdio(file));
    TRY(n = io.read_some(text, 12));  TEST_EQUAL(n, 12u);  TEST_EQUAL(text, "Hello world\n");
    TRY(n = io.read_some(text, 8));   TEST_EQUAL(n, 8u);   TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(n = io.read_some(text, 1));   TEST_EQUAL(n, 0u);   TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(io.close());

    TRY(io = Fdio(file));
    TRY(text = io.reads(12));  TEST_EQUAL(text, "Hello world\n");
    TRY(text = io.reads(8));   TEST_EQUAL(text, "Goodbye\n");
    TRY(text = io.reads(1));   TEST_EQUAL(text, "");
    TRY(io.close());

    TRY(io = Fdio(file));
    TRY(text = io.reads(100));  TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(io.close());

    TRY(io = Fdio(file));
    TRY(text = io.read_line());  TEST_EQUAL(text, "Hello world\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "Goodbye\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "");
    TRY(io.close());

    TRY(io = Fdio(file));
    TRY(text = io.read_all());  TEST_EQUAL(text, "Hello world\nGoodbye\n");
    TRY(io.close());

    TRY(io = Fdio(file));
    auto lines = io.lines();
    vec.clear();
    TRY(std::copy(lines.begin(), lines.end(), std::back_inserter(vec)));
    TEST_EQUAL(vec.size(), 2u);
    TEST_EQUAL(vec.at(0), "Hello world\n");
    TEST_EQUAL(vec.at(1), "Goodbye\n");
    TRY(io.close());

    TRY(io = Fdio(file));
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 0);
    TRY(io.seek(12));
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 12);
    TRY(text = io.reads(7));
    TEST_EQUAL(text, "Goodbye");
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 19);
    TRY(io.seek(-13));
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 6);
    TRY(text = io.reads(5));
    TEST_EQUAL(text, "world");
    TRY(offset = io.tell());
    TEST_EQUAL(offset, 11);
    TRY(io.close());

    TEST_THROW(io = Fdio(no_file, IoMode::read), IoError);

    TRY(io = Fdio(file, IoMode::write));
    TRY(io.format("Agent {0}\n", 86));
    TRY(io.print("Agent", 99));
    TRY(io.write_chars(10, '*'));
    TRY(io.close());

    TRY(io = Fdio(file));
    TRY(text = io.read_line());  TEST_EQUAL(text, "Agent 86\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "Agent 99\n");
    TRY(text = io.read_line());  TEST_EQUAL(text, "**********");
    TRY(text = io.read_line());  TEST_EQUAL(text, "");
    TRY(io.close());

}

void test_crow_stdio_pipe() {

    std::pair<Fdio, Fdio> pipe;
    std::string text;

    TRY(pipe = Fdio::pipe());
    TRY(pipe.second.writes("Hello world\n"));
    TRY(pipe.second.writes("Goodbye\n"));
    TRY(text = pipe.first.reads(100));
    TEST_EQUAL(text, "Hello world\nGoodbye\n");

}

void test_crow_stdio_winio() {

    #ifdef _WIN32

        Winio io;
        Path file = "__winio_test__", no_file = "__no_such_file__", path;
        std::string text;
        std::vector<std::string> vec;
        ptrdiff_t offset = 0;
        size_t n = 0;
        auto guard = on_scope_exit([=] { file.remove(); });

        TRY(file.remove());
        TEST(! file.exists());
        TEST(! io.is_open());

        TRY(io = Winio(file, IoMode::write));
        TEST(io.is_open());
        TRY(io.writes("Hello world\n"));
        TRY(io.writes("Goodbye\n"));
        TRY(io.close());
        TEST(! io.is_open());
        TEST(file.exists());

        text.clear();
        TRY(io = Winio(file));
        TRY(n = io.read_some(text, 12));  TEST_EQUAL(n, 12u);  TEST_EQUAL(text, "Hello world\n");
        TRY(n = io.read_some(text, 8));   TEST_EQUAL(n, 8u);   TEST_EQUAL(text, "Hello world\nGoodbye\n");
        TRY(n = io.read_some(text, 1));   TEST_EQUAL(n, 0u);   TEST_EQUAL(text, "Hello world\nGoodbye\n");
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(text = io.reads(12));  TEST_EQUAL(text, "Hello world\n");
        TRY(text = io.reads(8));   TEST_EQUAL(text, "Goodbye\n");
        TRY(text = io.reads(1));   TEST_EQUAL(text, "");
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(text = io.reads(20));  TEST_EQUAL(text, "Hello world\nGoodbye\n");
        TRY(text = io.reads(1));   TEST_EQUAL(text, "");
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(text = io.reads(100));  TEST_EQUAL(text, "Hello world\nGoodbye\n");
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(text = io.read_line());  TEST_EQUAL(text, "Hello world\n");
        TRY(text = io.read_line());  TEST_EQUAL(text, "Goodbye\n");
        TRY(text = io.read_line());  TEST_EQUAL(text, "");
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(text = io.read_all());  TEST_EQUAL(text, "Hello world\nGoodbye\n");
        TRY(io.close());

        TRY(io = Winio(file));
        auto lines = io.lines();
        vec.clear();
        TRY(std::copy(lines.begin(), lines.end(), std::back_inserter(vec)));
        TEST_EQUAL(vec.size(), 2u);
        TEST_EQUAL(vec.at(0), "Hello world\n");
        TEST_EQUAL(vec.at(1), "Goodbye\n");
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(offset = io.tell());     TEST_EQUAL(offset, 0);
        TRY(io.seek(12));
        TRY(offset = io.tell());     TEST_EQUAL(offset, 12);
        TRY(text = io.reads(7));  TEST_EQUAL(text, "Goodbye");
        TRY(offset = io.tell());     TEST_EQUAL(offset, 19);
        TRY(io.seek(-13));
        TRY(offset = io.tell());     TEST_EQUAL(offset, 6);
        TRY(text = io.reads(5));  TEST_EQUAL(text, "world");
        TRY(offset = io.tell());     TEST_EQUAL(offset, 11);
        TRY(io.close());

        TEST_THROW(io = Winio(no_file, IoMode::read), IoError);

        TRY(io = Winio(file, IoMode::write));
        TRY(io.format("Agent {0}\n", 86));
        TRY(io.print("Agent", 99));
        TRY(io.write_chars(10, '*'));
        TRY(io.close());

        TRY(io = Winio(file));
        TRY(text = io.read_line());  TEST_EQUAL(text, "Agent 86\n");
        TRY(text = io.read_line());  TEST_EQUAL(text, "Agent 99\n");
        TRY(text = io.read_line());  TEST_EQUAL(text, "**********");
        TRY(text = io.read_line());  TEST_EQUAL(text, "");
        TRY(io.close());

    #endif

}

void test_crow_stdio_null_device() {

    std::string text;

    {
        Cstdio io;
        TRY(io = Cstdio::null());
        TRY(text = io.reads(20));
        TRY(text = io.reads(20));
        TEST_EQUAL(text, "");
    }

    {
        Cstdio io;
        TRY(io = Cstdio::null());
        TRY(io.writes("Hello world\n"));
        TRY(io.writes("Hello world\n"));
        TRY(text = io.reads(20));
        TRY(text = io.reads(20));
        #ifdef _XOPEN_SOURCE
            TEST_EQUAL(text, "");
        #endif
    }

    {
        Fdio io;
        TRY(io = Fdio::null());
        TRY(text = io.reads(20));
        TRY(text = io.reads(20));
        TEST_EQUAL(text, "");
    }

    {
        Fdio io;
        TRY(io = Fdio::null());
        TRY(io.writes("Hello world\n"));
        TRY(io.writes("Hello world\n"));
        TRY(text = io.reads(20));
        TRY(text = io.reads(20));
        TEST_EQUAL(text, "");
    }

    #ifdef _WIN32

        {
            Winio io;
            TRY(io = Winio::null());
            TRY(text = io.reads(20));
            TRY(text = io.reads(20));
            TEST_EQUAL(text, "");
        }

        {
            Winio io;
            TRY(io = Winio::null());
            TRY(io.writes("Hello world\n"));
            TRY(io.writes("Hello world\n"));
            TRY(text = io.reads(20));
            TRY(text = io.reads(20));
            TEST_EQUAL(text, "");
        }

    #endif

}

void test_crow_stdio_anonymous_temporary_file() {

    std::unique_ptr<TempFile> tf;
    Path path;

    TRY(tf = std::make_unique<TempFile>());
    REQUIRE(tf);
    TEST(tf->get());
    TRY(path = tf->where());
    TEST(path.is_empty());

}

void test_crow_stdio_named_temporary_file() {

    std::unique_ptr<TempFile> tf;
    Path path;

    TRY(tf = std::make_unique<TempFile>("", "__test_tempfile_"));
    REQUIRE(tf);
    TEST(tf->get());
    TRY(path = tf->where());
    TEST(! path.is_empty());
    TEST_MATCH(path.name(), ".+" SLASH "__test_tempfile_[[:xdigit:]]{16}$");
    TEST(path.exists());
    TEST(path.is_file());
    TRY(tf.reset());
    TEST(! path.exists());

}
