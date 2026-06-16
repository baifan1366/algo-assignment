// *********************************************************
// Program: hash_table_search_array_step.cpp
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
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
    const int kNoNode = -1;

    struct AvlNode
    {
        Record record;
        int left = kNoNode;
        int right = kNoNode;
        int height = 1;
    };

    struct Bucket
    {
        bool occupied = false;
        Record direct;
        int root = kNoNode;
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

    std::uint64_t parse_target(const std::string &text)
    {
        return std::stoull(text);
    }

    std::string make_step_output_filename(const std::string &input_path, std::uint64_t target)
    {
        return "dataset_" + dataset_size_from_filename(input_path) + "_hash_table_search_array_step_" + std::to_string(target) + ".txt";
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

    std::vector<std::string> get_search_path(const std::vector<Bucket> &table, const std::vector<AvlNode> &nodes, std::uint64_t target)
    {
        std::vector<std::string> path;

        int index = get_hash_index(target, table.size());

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

        int current = table[index].root;

        while (current != kNoNode)
        {
            if (nodes[current].record.key == target)
            {
                path.push_back(std::to_string(target) + " = " + record_for_step_output(nodes[current].record));
                return path;
            }

            path.push_back(std::to_string(nodes[current].record.key) + " != " + std::to_string(target));

            if (target < nodes[current].record.key)
            {
                current = nodes[current].left;
            }
            else
            {
                current = nodes[current].right;
            }
        }

        path.push_back("-1 != " + std::to_string(target));
        return path;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_usage(argv[0], "<target_integer>");
        return 1;
    }

    try
    {
        // std::string input_path = "../dataset_1000.csv";
        // std::string input_path = "../dataset_5000.csv";
        // std::string input_path = "../dataset_10000.csv";
        // std::string input_path = "../dataset_50000.csv";
        // std::string input_path = "../dataset_100000.csv";
        // std::string input_path = "../dataset_250000.csv";
        // std::string input_path = "../dataset_500000.csv";
        // std::string input_path = "../dataset_1000000.csv";
        // std::string input_path = "../dataset_2500000.csv";
        std::string input_path = "../dataset_5000000.csv";

        std::uint64_t target = parse_target(argv[1]);
        std::vector<Record> records = read_dataset_csv(input_path);

        if (records.empty())
        {
            throw std::runtime_error("Dataset is empty");
        }

        std::vector<AvlNode> nodes;
        std::vector<Bucket> table = build_hash_table(records, nodes);
        std::vector<std::string> path = get_search_path(table, nodes, target);
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
        std::cerr << "hash_table_search_array_step error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
