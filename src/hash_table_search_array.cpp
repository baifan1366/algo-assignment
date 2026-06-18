// *********************************************************
// Program: hash_table_search_array.cpp
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

#include "../include/common.hpp"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    const int kNoNode = -1;

    // One node inside the array-based AVL tree.
    // Child links are array indexes instead of pointers.
    struct AvlNode
    {
        Record record;
        int left = kNoNode;
        int right = kNoNode;
        int height = 1;
    };

    // One hash table bucket. The first record is stored directly.
    // Collision records are stored in an array-based AVL tree.
    struct Bucket
    {
        bool occupied = false;
        Record direct;
        int root = kNoNode;
    };

    struct SearchResult
    {
        bool found = false;
        const Record *record = nullptr;
        int comparisons = 0;
    };

    int get_max(int first, int second)
    {
        if (first > second)
        {
            return first;
        }

        return second;
    }

    int get_height(const std::vector<AvlNode> &nodes, int index)
    {
        if (index == kNoNode)
        {
            return 0;
        }

        return nodes[index].height;
    }

    int get_balance(const std::vector<AvlNode> &nodes, int index)
    {
        if (index == kNoNode)
        {
            return 0;
        }

        return get_height(nodes, nodes[index].left) - get_height(nodes, nodes[index].right);
    }

    void update_height(std::vector<AvlNode> &nodes, int index)
    {
        nodes[index].height = 1 + get_max(get_height(nodes, nodes[index].left), get_height(nodes, nodes[index].right));
    }

    int create_node(std::vector<AvlNode> &nodes, const Record &record)
    {
        AvlNode node;
        node.record = record;
        nodes.push_back(node);
        return static_cast<int>(nodes.size()) - 1;
    }

    int right_rotate(std::vector<AvlNode> &nodes, int root)
    {
        int new_root = nodes[root].left;
        int moved_subtree = nodes[new_root].right;

        nodes[new_root].right = root;
        nodes[root].left = moved_subtree;

        update_height(nodes, root);
        update_height(nodes, new_root);

        return new_root;
    }

    int left_rotate(std::vector<AvlNode> &nodes, int root)
    {
        int new_root = nodes[root].right;
        int moved_subtree = nodes[new_root].left;

        nodes[new_root].left = root;
        nodes[root].right = moved_subtree;

        update_height(nodes, root);
        update_height(nodes, new_root);

        return new_root;
    }

    int insert_avl(std::vector<AvlNode> &nodes, int root, const Record &record)
    {
        if (root == kNoNode)
        {
            return create_node(nodes, record);
        }

        if (record.key < nodes[root].record.key)
        {
            nodes[root].left = insert_avl(nodes, nodes[root].left, record);
        }
        else if (record.key > nodes[root].record.key)
        {
            nodes[root].right = insert_avl(nodes, nodes[root].right, record);
        }
        else
        {
            nodes[root].record = record;
            return root;
        }

        update_height(nodes, root);

        int balance = get_balance(nodes, root);

        if (balance > 1 && record.key < nodes[nodes[root].left].record.key)
        {
            return right_rotate(nodes, root);
        }

        if (balance < -1 && record.key > nodes[nodes[root].right].record.key)
        {
            return left_rotate(nodes, root);
        }

        if (balance > 1 && record.key > nodes[nodes[root].left].record.key)
        {
            nodes[root].left = left_rotate(nodes, nodes[root].left);
            return right_rotate(nodes, root);
        }

        if (balance < -1 && record.key < nodes[nodes[root].right].record.key)
        {
            nodes[root].right = right_rotate(nodes, nodes[root].right);
            return left_rotate(nodes, root);
        }

        return root;
    }

    SearchResult search_avl(const std::vector<AvlNode> &nodes, int root, std::uint64_t target, int comparisons_so_far)
    {
        int current = root;
        int comparisons = comparisons_so_far;

        while (current != kNoNode)
        {
            comparisons++;

            if (target == nodes[current].record.key)
            {
                SearchResult result;
                result.found = true;
                result.record = &nodes[current].record;
                result.comparisons = comparisons;
                return result;
            }

            if (target < nodes[current].record.key)
            {
                current = nodes[current].left;
            }
            else
            {
                current = nodes[current].right;
            }
        }

        SearchResult result;
        result.found = false;
        result.record = nullptr;
        result.comparisons = comparisons;
        return result;
    }

    int get_hash_index(std::uint64_t key, std::size_t table_size)
    {
        return key % table_size;
    }

    int insert_into_hash_table(std::vector<Bucket> &hash_table, std::vector<AvlNode> &nodes, const Record &record)
    {
        int index = get_hash_index(record.key, hash_table.size());

        if (hash_table[index].occupied == false)
        {
            hash_table[index].occupied = true;
            hash_table[index].direct = record;
        }
        else
        {
            hash_table[index].root = insert_avl(nodes, hash_table[index].root, record);
        }

        return index;
    }

    SearchResult search_hash_table(const std::vector<Bucket> &hash_table, const std::vector<AvlNode> &nodes, std::uint64_t target)
    {
        int index = get_hash_index(target, hash_table.size());

        SearchResult result;

        if (hash_table[index].occupied == false)
        {
            result.found = false;
            result.record = nullptr;
            result.comparisons = 0;
            return result;
        }

        result.comparisons = 1;

        if (hash_table[index].direct.key == target)
        {
            result.found = true;
            result.record = &hash_table[index].direct;
            return result;
        }

        return search_avl(nodes, hash_table[index].root, target, result.comparisons);
    }

    std::vector<Bucket> build_hash_table(const std::vector<Record> &records, std::vector<AvlNode> &nodes)
    {
        int table_size = static_cast<int>(records.size());

        if (table_size < 1)
        {
            table_size = 1;
        }

        nodes.reserve(table_size);

        std::vector<Bucket> table(table_size);

        for (int i = 0; i < static_cast<int>(records.size()); i++)
        {
            insert_into_hash_table(table, nodes, records[i]);
        }

        return table;
    }

    std::string make_output_filename(const std::string &input_filename)
    {
        return "hash_table_search_array_dataset_" + dataset_size_from_filename(input_filename) + ".txt";
    }

    void write_output_file(const std::string &output_path, double best_time, double average_time, double worst_time)
    {
        std::ofstream output(output_path);

        if (!output)
        {
            throw std::runtime_error("Cannot open output file: " + output_path);
        }

        output << std::fixed << std::setprecision(9);
        output << "Best case time: " << best_time << " seconds\n";
        output << "Average case time: " << average_time << " seconds\n";
        output << "Worst case time: " << worst_time << " seconds\n";
    }

    double measure_repeated_key_search_time(const std::vector<Bucket> &table, const std::vector<AvlNode> &nodes, std::uint64_t target, int repeat_count)
    {
        std::uint64_t checksum = 0;

        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < repeat_count; i++)
        {
            SearchResult result = search_hash_table(table, nodes, target);

            if (result.found == true)
            {
                checksum = checksum + result.record->key;
            }
            else
            {
                checksum = checksum + result.comparisons;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end_time - start_time;

        if (checksum == 0)
        {
            std::cout << "";
        }

        return elapsed.count();
    }
}

int main()
{
    try
    {
        // std::string input_path = "../dataset_1000.csv";
        // std::string input_path = "../dataset_5000.csv";
        // std::string input_path = "../dataset_10000.csv";
        // std::string input_path = "../dataset_50000.csv";
        // std::string input_path = "../dataset_100000.csv";
        std::string input_path = "../dataset_250000.csv";
        // std::string input_path = "../dataset_500000.csv";
        // std::string input_path = "../dataset_1000000.csv";
        // std::string input_path = "../dataset_2500000.csv";
        // std::string input_path = "../dataset_5000000.csv";

        std::vector<Record> records = read_dataset_csv(input_path);

        if (records.empty())
        {
            throw std::runtime_error("Dataset is empty");
        }

        std::vector<AvlNode> nodes;
        std::vector<Bucket> table = build_hash_table(records, nodes);

        std::cout << "Dataset loaded: " << records.size() << " records\n";
        std::cout << "Array-based hash table built successfully\n";

        int repeat_count = static_cast<int>(records.size());
        double best_time = 0.0;
        double average_time = 0.0;
        double worst_time = 0.0;
        double total_time = 0.0;

        // Each key is searched n times, where n is the dataset size.
        // Best case is the key with the shortest repeated-search time.
        // Average case is the mean repeated-search time across all keys.
        // Worst case is the key with the longest repeated-search time.
        for (int i = 0; i < static_cast<int>(records.size()); i++)
        {
            double current_time = measure_repeated_key_search_time(table, nodes, records[i].key, repeat_count);

            if (i == 0 || current_time < best_time)
            {
                best_time = current_time;
            }

            if (i == 0 || current_time > worst_time)
            {
                worst_time = current_time;
            }

            total_time = total_time + current_time;
        }

        average_time = total_time / static_cast<double>(records.size());

        std::cout << std::fixed << std::setprecision(9);
        std::cout << "Best case time: " << best_time << " seconds\n";
        std::cout << "Average case time: " << average_time << " seconds\n";
        std::cout << "Worst case time: " << worst_time << " seconds\n";

        std::string output_path = make_output_filename(input_path);
        write_output_file(output_path, best_time, average_time, worst_time);

        std::cout << "Output file: " << output_path << '\n';
    }
    catch (const std::exception &error)
    {
        std::cerr << "hash_table_search_array error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
