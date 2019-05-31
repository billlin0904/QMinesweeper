#pragma once

#include <random>
#include <array>
#include <algorithm>
#include <functional>
#include <cmath>

class RandomEngine {    
    std::mt19937_64 generator;

public:
    RandomEngine();

    RandomEngine(const RandomEngine &) = delete;
    RandomEngine& operator=(const RandomEngine &) = delete;

    std::mt19937_64& gen() {
        return generator;
    }    

    static RandomEngine& getThreadLocalEngein();
};

class RNG {
public:
    RNG() = default;

    template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    T operator()(const T begin, const T end) {
        return std::uniform_int_distribution<T>{begin, end}(RandomEngine::getThreadLocalEngein().gen());
    }

    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    T operator()(const T begin, const T end) {
        return std::uniform_real_distribution<T>{begin, end}(RandomEngine::getThreadLocalEngein().gen());
    }     

    template <typename RandomAccessIterator>
    void shuffle(RandomAccessIterator first, RandomAccessIterator last) {
        std::shuffle(first, last, RandomEngine::getThreadLocalEngein().gen());
    }
};
