#include "../include/common.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

std::vector<Record> read_dataset_csv(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("Cannot open input file: " + path);
    }

    std::vector<Record> records;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::string key_text;
        std::string value;
        if (!std::getline(stream, key_text, ',') || !std::getline(stream, value)) {
            throw std::runtime_error("Invalid CSV row: " + line);
        }

        records.push_back(Record{std::stoull(key_text), value});
    }

    return records;
}

void write_dataset_csv(const std::string& path, const std::vector<Record>& records) {
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Cannot open output file: " + path);
    }

    for (const auto& record : records) {
        output << record.key << ',' << record.value << '\n';
    }
}

std::string record_for_step_output(const Record& record) {
    return std::to_string(record.key) + "/" + record.value;
}

std::string dataset_size_from_filename(const std::string& path) {
    const auto dataset_pos = path.find("dataset_");
    if (dataset_pos == std::string::npos) {
        return "n";
    }

    const auto start = dataset_pos + std::string("dataset_").size();
    const auto end = path.find('.', start);
    if (end == std::string::npos || end <= start) {
        return "n";
    }

    return path.substr(start, end - start);
}

void print_usage(const char* program_name, const std::string& usage) {
    std::cerr << "Usage: " << program_name << ' ' << usage << '\n';
}
