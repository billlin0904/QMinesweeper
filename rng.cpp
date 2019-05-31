#include <vector>
#include "rng.h"

RandomEngine::RandomEngine() {
    std::random_device random_device;
    std::vector<std::random_device::result_type> seed_data(std::mt19937_64::state_size);
    std::generate(std::begin(seed_data),
        std::end(seed_data),
        std::ref(random_device));
    std::seed_seq seq(std::begin(seed_data),
        std::end(seed_data));
    generator.seed(seq);
}

RandomEngine& RandomEngine::getThreadLocalEngein() {
    static thread_local RandomEngine eng;
    return eng;
}
