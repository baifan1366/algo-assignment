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

#include "common.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_usage(argv[0], "<dataset_n.csv> <target_integer>");
        return 1;
    }

    std::cout << "TODO: write hash table search path for target " << argv[2]
              << " in " << argv[1] << '\n';
    return 0;
}
