// *********************************************************
// Program: radix_sort.cpp
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

#include "common.cpp"

#include <array>
#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
constexpr int kDecimalBase = 10;
constexpr int kDigitCount = 10;

void radix_sort_by_key(std::vector<Record>& records) {
    if (records.size() < 2) {
        return;
    }

    std::vector<Record> output(records.size());
    std::uint64_t divisor = 1;

    for (int digit_position = 0; digit_position < kDigitCount; ++digit_position) {
        std::array<std::size_t, kDecimalBase> counts{};

        for (const auto& record : records) {
            const auto digit = static_cast<std::size_t>((record.key / divisor) % kDecimalBase);
            ++counts[digit];
        }

        for (std::size_t digit = 1; digit < counts.size(); ++digit) {
            counts[digit] += counts[digit - 1];
        }

        for (std::size_t index = records.size(); index > 0; --index) {
            const auto& record = records[index - 1];
            const auto digit = static_cast<std::size_t>((record.key / divisor) % kDecimalBase);
            output[--counts[digit]] = record;
        }

        records.swap(output);
        divisor *= kDecimalBase;
    }
}

std::string make_output_filename(const std::string& input_path) {
    return "radix_sorted_dataset_" + dataset_size_from_filename(input_path) + ".csv";
}

void write_radix_output(
    const std::string& output_path,
    const std::string& input_path,
    const std::vector<Record>& records,
    double elapsed_seconds
) {
    std::ofstream output(output_path);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + output_path);
    }

    output << "Algorithm: Radix Sort (rightmost digit first)\n";
    output << "Input file: " << input_path << '\n';
    output << "Number of records: " << records.size() << '\n';
    output << "Running time: " << std::fixed << std::setprecision(9)
           << elapsed_seconds << " seconds\n";

    for (const auto& record : records) {
        output << record.key << '/' << record.value << '\n';
    }
}
} // namespace

int main() {
    try {
        // const std::string input_path = "../dataset_1000.csv";
        // const std::string input_path = "../dataset_5000.csv";
        // const std::string input_path = "../dataset_10000.csv";
        // const std::string input_path = "../dataset_50000.csv";
        // const std::string input_path = "../dataset_100000.csv";
        // const std::string input_path = "../dataset_250000.csv";
        // const std::string input_path = "../dataset_500000.csv";
        // const std::string input_path = "../dataset_1000000.csv";
        // const std::string input_path = "../dataset_2500000.csv";
        const std::string input_path = "../dataset_5000000.csv";

        auto records = read_dataset_csv(input_path);

        const auto start_time = std::chrono::high_resolution_clock::now();
        radix_sort_by_key(records);
        const auto end_time = std::chrono::high_resolution_clock::now();

        const std::chrono::duration<double> elapsed = end_time - start_time;
        const auto output_path = make_output_filename(input_path);
        write_radix_output(output_path, input_path, records, elapsed.count());

        std::cout << "Radix sort completed for " << input_path << '\n';
        std::cout << "Output file: " << output_path << '\n';
        std::cout << "Running time: " << std::fixed << std::setprecision(9)
                  << elapsed.count() << " seconds\n";
    } catch (const std::exception& error) {
        std::cerr << "radix_sort error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
