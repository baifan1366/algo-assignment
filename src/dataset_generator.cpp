// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: ID | NAME | EMAIL | PHONE
// Member_2: ID | NAME | EMAIL | PHONE
// Member_3: ID | NAME | EMAIL | PHONE
// Member_4: ID | NAME | EMAIL | PHONE
// *********************************************************
// Task Distribution
// Member_1:
// Member_2:
// Member_3:
// Member_4:
// *********************************************************

#include "common.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <exception>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
constexpr std::uint64_t kMinimumKey = 1'000'000'000ULL;
constexpr std::uint64_t kMaximumKey = 9'999'999'999ULL;
constexpr std::uint64_t kKeyRange = kMaximumKey - kMinimumKey + 1ULL;

// Group leader ID 253UC2557B mapped with U=1, C=3, B=2.
constexpr std::uint64_t kGroupLeaderSeed = 2'531'325'572ULL;

bool contains_digits_only(const std::string& text) {
    return !text.empty() && std::all_of(text.begin(), text.end(), [](unsigned char character) {
        return std::isdigit(character) != 0;
    });
}

std::uint64_t parse_size(const std::string& text) {
    if (!contains_digits_only(text)) {
        throw std::invalid_argument("size_n must contain digits only");
    }

    const auto value = std::stoull(text);
    if (value == 0) {
        throw std::invalid_argument("size_n must be greater than 0");
    }
    if (value > kKeyRange) {
        throw std::invalid_argument("size_n is larger than the available 10-digit unique key range");
    }
    if (value > static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max())) {
        throw std::invalid_argument("size_n is too large for this platform");
    }
    return value;
}

std::uint64_t parse_seed(const std::string& text) {
    if (!contains_digits_only(text)) {
        throw std::invalid_argument("seed must contain digits only");
    }

    const auto value = std::stoull(text);
    return value;
}

std::string random_lowercase_word(std::mt19937_64& generator) {
    std::uniform_int_distribution<int> letter_distribution(0, 25);
    std::string word;
    word.reserve(5);
    for (int i = 0; i < 5; ++i) {
        word.push_back(static_cast<char>('a' + letter_distribution(generator)));
    }
    return word;
}

std::vector<Record> generate_records(std::size_t size, std::uint64_t seed) {
    std::mt19937_64 generator(seed);
    std::uniform_int_distribution<std::uint64_t> offset_distribution(0, kKeyRange - 1ULL);

    // This multiplier is coprime with 9,000,000,000, so the formula below is
    // a permutation of the full key range and cannot produce duplicate keys.
    constexpr std::uint64_t permutation_multiplier = 1'000'003ULL;
    const std::uint64_t offset = offset_distribution(generator);

    std::vector<Record> records;
    records.reserve(size);
    for (std::uint64_t i = 0; i < static_cast<std::uint64_t>(size); ++i) {
        const std::uint64_t key = kMinimumKey + ((i * permutation_multiplier + offset) % kKeyRange);
        records.push_back(Record{key, random_lowercase_word(generator)});
    }

    std::shuffle(records.begin(), records.end(), generator);
    return records;
}
} // namespace

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        print_usage(argv[0], "<size_n> [seed]");
        return 1;
    }

    try {
        const auto size = parse_size(argv[1]);
        const auto seed = argc == 3 ? parse_seed(argv[2]) : kGroupLeaderSeed;
        const auto output_path = "dataset_" + std::to_string(size) + ".csv";

        const auto records = generate_records(static_cast<std::size_t>(size), seed);
        write_dataset_csv(output_path, records);

        std::cout << "Generated " << records.size() << " randomized unique records in "
                  << output_path << '\n';
        std::cout << "Seed used: " << seed << '\n';
    } catch (const std::exception& error) {
        std::cerr << "dataset_generator error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
