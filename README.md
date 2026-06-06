# CCP6214 Algorithm Design & Analysis Assignment

This workspace is a C++17 project scaffold for the CCP6214 assignment.

## Build

```powershell
cmake -S . -B build
cmake --build build
```

If `cmake` is not available, install CMake plus a C++ compiler such as Visual Studio Build Tools, MinGW-w64, or LLVM/Clang, then rerun the commands above.

## Required Programs

- `dataset_generator`
- `radix_sort`
- `heap_sort`
- `hash_table_search`
- `radix_sort_step`
- `heap_sort_step`
- `hash_table_search_step`

## Dataset Generator

```powershell
.\build\Debug\dataset_generator.exe 1000
.\build\Debug\dataset_generator.exe 1000 2431324730
```

The second command uses an explicit seed. If no seed is provided, the program uses the mapped group leader seed inside `src/dataset_generator.cpp`.

## Radix Sort

```powershell
.\build\Debug\radix_sort.exe dataset_1000.csv
```

This writes `radix_sorted_dataset_1000.txt`, prints the running time in the terminal, and writes the same running time above the sorted `integer,string` rows.

## Radix Sort Step

```powershell
.\build\Debug\radix_sort_step.exe dataset_1000.csv 1 7
```

This writes `dataset_1000_radix_sorted_step_1_7.txt`, showing the selected records in `integer/string` format and every digit pass from `d=10` to `d=1`.

## Heap Sort

```powershell
.\build\Debug\heap_sort.exe dataset_1000.csv
```

This builds a maxheap manually, sorts by the integer key, writes
`heap_sorted_dataset_1000.csv`, and records the algorithm-only running time in
both the terminal and output file.

## Heap Sort Step

```powershell
.\build\Debug\heap_sort_step.exe dataset_1000.csv 1 7
```

This writes `dataset_1000_heap_sorted_step_1_7.txt`, showing the initial maxheap
and each extraction step from `i = 6` through `i = 1`.

See `TODO.md` for the full requirement and rubric checklist extracted from the assignment PDF.

## Suggested Folders

- `data/`: generated input CSV datasets.
- `outputs/`: generated sorted CSV and search/step TXT output files.
- `experiments/`: experiment tables and timing notes.
- `screenshots/`: command prompt timing screenshots and hardware screenshots.
- `docs/`: report draft, references, and submission checklist.
