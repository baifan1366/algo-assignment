#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Record {
    std::uint64_t key{};
    std::string value;
};

std::vector<Record> read_dataset_csv(const std::string& path);
void write_dataset_csv(const std::string& path, const std::vector<Record>& records);
std::string record_for_step_output(const Record& record);
std::string dataset_size_from_filename(const std::string& path);
void print_usage(const char* program_name, const std::string& usage);
