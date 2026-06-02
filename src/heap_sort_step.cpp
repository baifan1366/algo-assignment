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

#include "common.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage(argv[0], "<dataset_n.csv> <start_row> <end_row>");
        return 1;
    }

    std::cout << "TODO: write heap sort steps for " << argv[1]
              << " rows " << argv[2] << " to " << argv[3] << '\n';
    return 0;
}
