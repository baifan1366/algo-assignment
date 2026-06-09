#include "common.cpp"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_set>
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
    
    // For better distribution across the full key range, especially for small datasets,
    // we sample randomly from the entire range using reservoir sampling approach.
    std::uniform_int_distribution<std::uint64_t> key_distribution(kMinimumKey, kMaximumKey);

    std::vector<Record> records;
    records.reserve(size);
    
    // Use unordered_set for efficient duplicate checking
    std::unordered_set<std::uint64_t> used_keys;
    used_keys.reserve(size);
    
    while (records.size() < size) {
        std::uint64_t key = key_distribution(generator);
        
        // Check if key is unique
        if (used_keys.insert(key).second) {
            records.push_back(Record{key, random_lowercase_word(generator)});
        }
    }

    std::shuffle(records.begin(), records.end(), generator);
    return records;
}

std::string make_generation_info_filename(std::uint64_t size) {
    return "dataset_" + std::to_string(size) + "_generation_info.txt";
}

double elapsed_seconds(
    const std::chrono::high_resolution_clock::time_point& start_time,
    const std::chrono::high_resolution_clock::time_point& end_time
) {
    return std::chrono::duration<double>(end_time - start_time).count();
}

void write_generation_info(
    const std::string& output_path,
    const std::string& dataset_path,
    std::uint64_t size,
    std::uint64_t seed,
    double generation_seconds,
    double writing_seconds,
    double total_seconds
) {
    std::ofstream output(output_path);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + output_path);
    }

    output << "Program: dataset_generator\n";
    output << "Dataset file: " << dataset_path << '\n';
    output << "Number of records: " << size << '\n';
    output << "Seed used: " << seed << '\n';
    output << "Integer range: " << kMinimumKey << " to " << kMaximumKey << '\n';
    output << "String format: 5 lowercase alphabet letters\n";
    output << "Record format: integer,string\n";
    output << "Duplicate prevention: uniform random sampling with hash set tracking\n";
    output << "Randomization: std::shuffle after record generation\n";
    output << std::fixed << std::setprecision(9);
    output << "Generation and randomization time: " << generation_seconds << " seconds\n";
    output << "CSV writing time: " << writing_seconds << " seconds\n";
    output << "Total program work time: " << total_seconds << " seconds\n";
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
        const auto info_output_path = make_generation_info_filename(size);

        const auto total_start_time = std::chrono::high_resolution_clock::now();
        const auto generation_start_time = std::chrono::high_resolution_clock::now();
        const auto records = generate_records(static_cast<std::size_t>(size), seed);
        const auto generation_end_time = std::chrono::high_resolution_clock::now();

        const auto writing_start_time = std::chrono::high_resolution_clock::now();
        write_dataset_csv(output_path, records);
        const auto writing_end_time = std::chrono::high_resolution_clock::now();
        const auto total_end_time = std::chrono::high_resolution_clock::now();

        const auto generation_time = elapsed_seconds(generation_start_time, generation_end_time);
        const auto writing_time = elapsed_seconds(writing_start_time, writing_end_time);
        const auto total_time = elapsed_seconds(total_start_time, total_end_time);

        write_generation_info(
            info_output_path,
            output_path,
            size,
            seed,
            generation_time,
            writing_time,
            total_time
        );

        std::cout << std::fixed << std::setprecision(9);
        std::cout << "Generated " << records.size() << " randomized unique records in "
                  << output_path << '\n';
        std::cout << "Seed used: " << seed << '\n';
        std::cout << "Generation and randomization time: " << generation_time << " seconds\n";
        std::cout << "CSV writing time: " << writing_time << " seconds\n";
        std::cout << "Total program work time: " << total_time << " seconds\n";
        std::cout << "Generation info file: " << info_output_path << '\n';
    } catch (const std::exception& error) {
        std::cerr << "dataset_generator error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
