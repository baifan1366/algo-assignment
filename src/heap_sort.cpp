// *********************************************************
// Program: heap_sort.cpp
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

#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
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

void heap_sort_by_key(std::vector<Record>& records) {
    if (records.size() < 2) {
        return;
    }

    for (std::size_t root = records.size() / 2; root > 0; --root) {
        sift_down(records, root - 1, records.size());
    }

    for (std::size_t heap_size = records.size(); heap_size > 1; --heap_size) {
        std::swap(records[0], records[heap_size - 1]);
        sift_down(records, 0, heap_size - 1);
    }
}

std::string make_output_filename(const std::string& input_path) {
    return "heap_sorted_dataset_" + dataset_size_from_filename(input_path) + ".csv";
}

void write_heap_output(
    const std::string& output_path,
    const std::string& input_path,
    const std::vector<Record>& records,
    double elapsed_seconds
) {
    std::ofstream output(output_path);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + output_path);
    }

    output << "Algorithm: Heap Sort (maxheap)\n";
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
        //const std::string input_path = "dataset_1000.csv";
        // const std::string input_path = "dataset_5000.csv";
        const std::string input_path = "dataset_10000.csv";
        // const std::string input_path = "dataset_50000.csv";
        // const std::string input_path = "dataset_100000.csv";
        // const std::string input_path = "dataset_250000.csv";
        // const std::string input_path = "dataset_500000.csv";
        // const std::string input_path = "dataset_1000000.csv";
        // const std::string input_path = "dataset_2500000.csv";
        // const std::string input_path = "dataset_5000000.csv";

        auto records = read_dataset_csv(input_path);

        const auto start_time = std::chrono::high_resolution_clock::now();
        heap_sort_by_key(records);
        const auto end_time = std::chrono::high_resolution_clock::now();

        const std::chrono::duration<double> elapsed = end_time - start_time;
        const auto output_path = make_output_filename(input_path);
        write_heap_output(output_path, input_path, records, elapsed.count());

        std::cout << "Heap sort completed for " << input_path << '\n';
        std::cout << "Output file: " << output_path << '\n';
        std::cout << "Running time: " << std::fixed << std::setprecision(9)
                  << elapsed.count() << " seconds\n";
    } catch (const std::exception& error) {
        std::cerr << "heap_sort error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
