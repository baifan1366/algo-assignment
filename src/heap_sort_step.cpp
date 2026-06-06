// *********************************************************
// Program: heap_sort_step.cpp
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

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
bool contains_digits_only(const std::string& text) {
    return !text.empty() && std::all_of(text.begin(), text.end(), [](unsigned char character) {
        return std::isdigit(character) != 0;
    });
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
    for (std::size_t index = 0; index < records.size(); ++index) {
        if (index > 0) {
            output << ", ";
        }
        output << record_for_step_output(records[index]);
    }
    output << ']';
}

void sift_down(std::vector<Record>& records, std::size_t root, std::size_t heap_size) {
    while (true) {
        const std::size_t left_child = root * 2 + 1;
        if (left_child >= heap_size) {
            return;
        }

        std::size_t largest = root;
        if (records[left_child].key > records[largest].key) {
            largest = left_child;
        }

        const std::size_t right_child = left_child + 1;
        if (right_child < heap_size && records[right_child].key > records[largest].key) {
            largest = right_child;
        }

        if (largest == root) {
            return;
        }

        std::swap(records[root], records[largest]);
        root = largest;
    }
}

void build_maxheap(std::vector<Record>& records) {
    for (std::size_t root = records.size() / 2; root > 0; --root) {
        sift_down(records, root - 1, records.size());
    }
}

std::string make_output_filename(
    const std::string& input_path,
    std::size_t start_row,
    std::size_t end_row
) {
    return "dataset_" + dataset_size_from_filename(input_path)
        + "_heap_sorted_step_" + std::to_string(start_row)
        + "_" + std::to_string(end_row) + ".txt";
}

void write_heap_steps(const std::string& output_path, std::vector<Record> selected_records) {
    std::ofstream output(output_path);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + output_path);
    }

    build_maxheap(selected_records);
    write_record_list(output, selected_records);
    output << " initial\n";

    for (std::size_t heap_size = selected_records.size(); heap_size > 1; --heap_size) {
        std::swap(selected_records[0], selected_records[heap_size - 1]);
        sift_down(selected_records, 0, heap_size - 1);
        write_record_list(output, selected_records);
        output << " i = " << heap_size - 1 << '\n';
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
        write_heap_steps(output_path, selected_records);

        std::cout << "Heap sort step output written to " << output_path << '\n';
    } catch (const std::exception& error) {
        std::cerr << "heap_sort_step error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
