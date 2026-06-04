// *********************************************************
// Program: radix_sort_step.cpp
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
#include <cstddef>
#include <cctype>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
constexpr int kDecimalBase = 10;
constexpr int kDigitCount = 10;

bool contains_digits_only(const std::string& text) {
    if (text.empty()) {
        return false;
    }

    for (const unsigned char character : text) {
        if (std::isdigit(character) == 0) {
            return false;
        }
    }

    return true;
}

std::size_t parse_row_number(const std::string& text, const char* argument_name) {
    if (!contains_digits_only(text)) {
        throw std::invalid_argument(std::string(argument_name) + " must contain digits only");
    }

    const auto value = std::stoull(text);
    if (value == 0) {
        throw std::invalid_argument(std::string(argument_name) + " must be at least 1");
    }

    return static_cast<std::size_t>(value);
}

std::vector<Record> select_rows(
    const std::vector<Record>& records,
    std::size_t start_row,
    std::size_t end_row
) {
    if (start_row > end_row) {
        throw std::invalid_argument("start_row must be less than or equal to end_row");
    }
    if (end_row > records.size()) {
        throw std::out_of_range("end_row is larger than the number of rows in the input file");
    }

    return std::vector<Record>(records.begin() + static_cast<std::ptrdiff_t>(start_row - 1),
                               records.begin() + static_cast<std::ptrdiff_t>(end_row));
}

void write_record_list(std::ofstream& output, const std::vector<Record>& records) {
    output << '[';
    for (std::size_t i = 0; i < records.size(); ++i) {
        if (i > 0) {
            output << ", ";
        }
        output << record_for_step_output(records[i]);
    }
    output << ']';
}

void radix_pass_by_key(std::vector<Record>& records, std::uint64_t divisor) {
    std::array<std::size_t, kDecimalBase> counts{};
    std::vector<Record> sorted(records.size());

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
        sorted[--counts[digit]] = record;
    }

    records.swap(sorted);
}

std::string make_output_filename(
    const std::string& input_path,
    std::size_t start_row,
    std::size_t end_row
) {
    return "dataset_" + dataset_size_from_filename(input_path)
        + "_radix_sorted_step_" + std::to_string(start_row)
        + "_" + std::to_string(end_row) + ".txt";
}

void write_radix_steps(
    const std::string& output_path,
    std::vector<Record> selected_records
) {
    std::ofstream output(output_path);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + output_path);
    }

    write_record_list(output, selected_records);
    output << " original\n";

    std::uint64_t divisor = 1;
    for (int digit_label = kDigitCount; digit_label >= 1; --digit_label) {
        radix_pass_by_key(selected_records, divisor);
        write_record_list(output, selected_records);
        output << " d=" << digit_label << '\n';
        divisor *= kDecimalBase;
    }
}
} // namespace

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage(argv[0], "<dataset_n.csv> <start_row> <end_row>");
        return 1;
    }

    try {
        const std::string input_path = argv[1];
        const auto start_row = parse_row_number(argv[2], "start_row");
        const auto end_row = parse_row_number(argv[3], "end_row");

        const auto records = read_dataset_csv(input_path);
        const auto selected_records = select_rows(records, start_row, end_row);
        const auto output_path = make_output_filename(input_path, start_row, end_row);
        write_radix_steps(output_path, selected_records);

        std::cout << "Radix sort step output written to " << output_path << '\n';
    } catch (const std::exception& error) {
        std::cerr << "radix_sort_step error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
