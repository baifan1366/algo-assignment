// *********************************************************
// Program: hash_table_search_step.cpp
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

    // SearchResult search_hash_table(const std::vector<Bucket> &hash_table, std::uint64_t target)
    // {
    //     int index = get_hash_index(target, static_cast<int>(hash_table.size()));

    //     SearchResult result;

    //     if (hash_table[index].occupied == false)
    //     {
    //         result.found = false;
    //         result.record = nullptr;
    //         result.comparisons = 0;
    //         return result;
    //     }

    //     result.comparisons = 1;

    //     if (hash_table[index].direct.key == target)
    //     {
    //         result.found = true;
    //         result.record = &hash_table[index].direct;
    //         return result;
    //     }

    //     return search_avl(hash_table[index].collision_root, target, result.comparisons);
    // }

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

    std::uint64_t parse_target(const std::string &text)
    {
        return std::stoull(text);
    }

    std::string make_step_output_filename(const std::string &input_path, std::uint64_t target)
    {
        return "dataset_" + dataset_size_from_filename(input_path) + "_hash_table_search_step_" + std::to_string(target) + ".txt";
    }

    void write_step_output_file(const std::string &output_path, const std::vector<std::string> &path)
    {
        std::ofstream output(output_path);

        if (!output)
        {
            throw std::runtime_error("Cannot open output file: " + output_path);
        }

        for (int i = 0; i < static_cast<int>(path.size()); i++)
        {
            output << path[i] << '\n';
        }
    }

    std::vector<std::string> get_search_path(const std::vector<Bucket> &table, std::uint64_t target)
    {
        std::vector<std::string> path;

        int index = get_hash_index(target, static_cast<int>(table.size()));

        if (table[index].occupied == false)
        {
            path.push_back("-1 != " + std::to_string(target));
            return path;
        }

        if (table[index].direct.key == target)
        {
            path.push_back(std::to_string(target) + " = " + record_for_step_output(table[index].direct));
            return path;
        }

        path.push_back(std::to_string(table[index].direct.key) + " != " + std::to_string(target));

        AvlNode *current = table[index].collision_root;

        while (current != nullptr)
        {
            if (current->record.key == target)
            {
                path.push_back(std::to_string(target) + " = " + record_for_step_output(current->record));
                return path;
            }

            path.push_back(std::to_string(current->record.key) + " != " + std::to_string(target));

            if (target < current->record.key)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        path.push_back("-1 != " + std::to_string(target));
        return path;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        print_usage(argv[0], "<dataset_n.csv> <target_integer>");
        return 1;
    }

    try
    {
        std::string input_path = argv[1];
        std::uint64_t target = parse_target(argv[2]);

        std::vector<Record> records = read_dataset_csv(input_path);

        if (records.empty())
        {
            throw std::runtime_error("Dataset is empty");
        }

        std::vector<Bucket> table = build_hash_table(records);

        std::vector<std::string> path = get_search_path(table, target);

        std::string output_path = make_step_output_filename(input_path, target);

        for (int i = 0; i < static_cast<int>(path.size()); i++)
        {
            std::cout << path[i] << '\n';
        }

        std::cout << "Output file: " << output_path << '\n';

        write_step_output_file(output_path, path);
    }
    catch (const std::exception &error)
    {
        std::cerr << "hash_table_search_step error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}