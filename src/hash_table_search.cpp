// *********************************************************
// Program: hash_table_search.cpp
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
#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <chrono>

namespace
{
    // one item inside the AVL tree.
    struct AvlNode
    {
        Record record;
        AvlNode *left = nullptr;
        AvlNode *right = nullptr;
        int height = 1;
    };

    // one slot in the hash table, which may contain multiple records due to collisions.
    struct Bucket
    {
        bool occupied = false;
        Record direct;
        AvlNode *collision_root = nullptr;
    };

    // the result of searching for a target integer in the hash table.
    struct SearchResult
    {
        bool found = false;
        const Record *record = nullptr;
        int comparisons = 0;
    };

    int get_height(AvlNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return node->height;
    }

    int get_balance(AvlNode *node)
    {
        if (node == nullptr)
        {
            return 0;
        }

        return get_height(node->left) - get_height(node->right);
    }

    int get_max(int a, int b)
    {
        if (a > b)
        {
            return a;
        }
        else
        {
            return b;
        }
    }

    AvlNode *right_rotate(AvlNode *root)
    {
        AvlNode *new_root = root->left;
        AvlNode *moved_subtree = new_root->right;

        // Perform rotation
        new_root->right = root;
        root->left = moved_subtree;

        // Update heights
        root->height = 1 + get_max(get_height(root->left), get_height(root->right));
        new_root->height = 1 + get_max(get_height(new_root->left), get_height(new_root->right));

        // Return new root
        return new_root;
    }

    AvlNode *left_rotate(AvlNode *root)
    {
        AvlNode *new_root = root->right;
        AvlNode *moved_subtree = new_root->left;

        // Perform rotation
        new_root->left = root;
        root->right = moved_subtree;

        // Update heights
        root->height = 1 + get_max(get_height(root->left), get_height(root->right));
        new_root->height = 1 + get_max(get_height(new_root->left), get_height(new_root->right));

        // Return new root
        return new_root;
    }

    AvlNode *insert_avl(AvlNode *node, const Record &record)
    {
        if (node == nullptr)
        {
            AvlNode *new_node = new AvlNode;
            new_node->record = record;
            new_node->left = nullptr;
            new_node->right = nullptr;
            new_node->height = 1;
            return new_node;
        }

        if (record.key < node->record.key)
        {
            node->left = insert_avl(node->left, record);
        }
        else if (record.key > node->record.key)
        {
            node->right = insert_avl(node->right, record);
        }
        else
        {
            node->record = record;
            return node;
        }

        node->height = 1 + get_max(get_height(node->left), get_height(node->right));

        int balance = get_balance(node);

        if (balance > 1 && record.key < node->left->record.key)
        {
            return right_rotate(node);
        }

        if (balance < -1 && record.key > node->right->record.key)
        {
            return left_rotate(node);
        }

        if (balance > 1 && record.key > node->left->record.key)
        {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }

        if (balance < -1 && record.key < node->right->record.key)
        {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }

        return node;
    }

    SearchResult search_avl(AvlNode *node, std::uint64_t target, int comparisons_so_far)
    {
        AvlNode *current = node;
        int comparisons = comparisons_so_far;

        while (current != nullptr)
        {
            comparisons++;

            if (target == current->record.key)
            {
                SearchResult result;
                result.found = true;
                result.record = &current->record;
                result.comparisons = comparisons;
                return result;
            }

            if (target < current->record.key)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
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

    int insert_into_hash_table(std::vector<Bucket> &hash_table, const Record &record)
    {
        int index = get_hash_index(record.key, static_cast<int>(hash_table.size()));

        if (hash_table[index].occupied == false)
        {
            hash_table[index].occupied = true;
            hash_table[index].direct = record;
        }
        else
        {
            hash_table[index].collision_root = insert_avl(hash_table[index].collision_root, record);
        }
        return index;
    }

    SearchResult search_hash_table(const std::vector<Bucket> &hash_table, std::uint64_t target)
    {
        int index = get_hash_index(target, static_cast<int>(hash_table.size()));

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

        return search_avl(hash_table[index].collision_root, target, result.comparisons);
    }

    std::vector<Bucket> build_hash_table(const std::vector<Record> &records)
    {
        int table_size = static_cast<int>(records.size());

        if (table_size < 1)
        {
            table_size = 1;
        }

        std::vector<Bucket> table(table_size);

        for (int i = 0; i < static_cast<int>(records.size()); i++)
        {
            insert_into_hash_table(table, records[i]);
        }

        return table;
    }

    std::string make_output_filename(const std::string &input_filename)
    {
        return "hash_table_search_dataset_" + dataset_size_from_filename(input_filename) + ".txt";
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

    double measure_search_time(const std::vector<Bucket> &table, const std::vector<std::uint64_t> &targets)
    {
        std::uint64_t checksum = 0;

        auto start_time = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < static_cast<int>(targets.size()); i++)
        {
            SearchResult result = search_hash_table(table, targets[i]);

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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_usage(argv[0], "<dataset_n.csv>");
        return 1;
    }

    try
    {
        std::string input_path = argv[1];

        std::vector<Record> records = read_dataset_csv(input_path);

        if (records.empty())
        {
            throw std::runtime_error("Dataset is empty");
        }

        std::vector<Bucket> table = build_hash_table(records);

        std::cout << "Dataset loaded: " << records.size() << " records\n";
        std::cout << "Hash table built successfully\n";
        std::vector<std::uint64_t> best_targets;
        std::vector<std::uint64_t> average_targets;
        std::vector<std::uint64_t> worst_targets;

        best_targets.reserve(records.size());
        average_targets.reserve(records.size());
        worst_targets.reserve(records.size());

        std::uint64_t best_key = records[0].key;
        std::uint64_t worst_key = records[0].key;

        int best_comparisons = search_hash_table(table, best_key).comparisons;
        int worst_comparisons = best_comparisons;

        for (int i = 0; i < static_cast<int>(records.size()); i++)
        {
            SearchResult result = search_hash_table(table, records[i].key);

            if (result.comparisons < best_comparisons)
            {
                best_comparisons = result.comparisons;
                best_key = records[i].key;
            }

            if (result.comparisons > worst_comparisons)
            {
                worst_comparisons = result.comparisons;
                worst_key = records[i].key;
            }

            average_targets.push_back(records[i].key);
        }

        for (int i = 0; i < static_cast<int>(records.size()); i++)
        {
            best_targets.push_back(best_key);
            worst_targets.push_back(worst_key);
        }

        double best_time = measure_search_time(table, best_targets);
        double average_time = measure_search_time(table, average_targets);
        double worst_time = measure_search_time(table, worst_targets);

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
        std::cerr << "hash_table_search error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
