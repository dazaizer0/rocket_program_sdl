#include "config.h"

namespace program {
    #pragma region initialize
    // example
    int x{};
    #pragma endregion

    void start() {
        x = 8;
    }

    void update() {
        x += 1;
    }

    void end() {
        std::cout << "final score: " << x << std::endl;
    }
}