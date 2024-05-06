#pragma once

#include <chrono>
#include <random>

using std::chrono::year_month_day;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;

uint64_t tsNowMillis() {
    return static_cast<uint64_t>(duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()).count());
}

uint64_t randomId() {
    using engine = std::default_random_engine;
    using dist = std::uniform_int_distribution<>;

    std::random_device r;
    engine e{r()};
    dist d{1000, 9999};
    return d(e);
}
