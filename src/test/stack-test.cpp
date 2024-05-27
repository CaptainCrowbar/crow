#include "crow/stack.hpp"
#include "crow/unit-test.hpp"
#include <ostream>
#include <string>

using namespace Crow;
using namespace Crow::UnitTest;

namespace {

    class TopTail {
    public:
        TopTail(): sp_(nullptr), ch_() {}
        TopTail(std::string& s, char c): sp_(&s), ch_(c) { s += '+'; s += c; }
        TopTail(const TopTail&) = delete;
        TopTail(TopTail&& t): sp_(t.sp_), ch_(t.ch_) { t.sp_ = nullptr; }
        ~TopTail() { destroy(); }
        TopTail& operator=(const TopTail&) = delete;
        TopTail& operator=(TopTail&& t) { if (&t != this) { destroy(); sp_ = t.sp_; ch_ = t.ch_; t.sp_ = nullptr; } return *this; }
        char get() const noexcept { return ch_; }
    private:
        std::string* sp_;
        char ch_;
        void destroy() { if (sp_) { *sp_ += '-'; *sp_ += ch_; sp_ = nullptr; } }
    };

    inline std::ostream& operator<<(std::ostream& out, const TopTail& t) {
        return out << t.get();
    }

}

void test_crow_stack() {

    Stack<TopTail> st;
    std::string s;

    TEST(st.empty());
    TEST_EQUAL(format_range(st), "[]");
    TRY(st.emplace(s, 'a'));
    TEST_EQUAL(st.size(), 1u);
    TEST_EQUAL(st.top().get(), 'a');
    TEST_EQUAL(format_range(st), "[a]");
    TEST_EQUAL(s, "+a");
    TRY(st.emplace(s, 'b'));
    TEST_EQUAL(st.size(), 2u);
    TEST_EQUAL(st.top().get(), 'b');
    TEST_EQUAL(format_range(st), "[a,b]");
    TEST_EQUAL(s, "+a+b");
    TRY(st.emplace(s, 'c'));
    TEST_EQUAL(st.size(), 3u);
    TEST_EQUAL(st.top().get(), 'c');
    TEST_EQUAL(format_range(st), "[a,b,c]");
    TEST_EQUAL(s, "+a+b+c");
    TRY(st.clear());
    TEST(st.empty());
    TEST_EQUAL(format_range(st), "[]");
    TEST_EQUAL(s, "+a+b+c-c-b-a");

}
