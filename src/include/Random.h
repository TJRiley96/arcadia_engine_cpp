#pragma once


// Built-in includes
#include <random>

// Third-party includes

// Local includes



class Random {
public:
    static void Init();
    static float GetFloat(float min, float max);
    static int GetInt(int min, int max);

private:
    static std::mt19937 generator;
};
