#include "crow/progress.hpp"
#include "crow/unit-test.hpp"
#include <chrono>
#include <thread>

using namespace Crow;
using namespace std::chrono;

void test_crow_progress_bar() {

    static constexpr double scale = 100;
    static constexpr auto runtime = 1s;
    static constexpr auto interval = runtime / scale;

    ProgressBar bar("Hello", scale);

    for (int i = 0; i <= 100; ++i) {
        bar(i);
        std::this_thread::sleep_for(interval);
    }

}
