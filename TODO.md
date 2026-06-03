# CCP6214 Assignment TODO

Source analyzed: `C:\Users\weixu\Downloads\CCP6214_Assignment.pdf` extracted into `assignment_extracted.txt`.

## 0. Assignment Facts

- Weight: 40 marks / 40%.
- Deadline: Week 13, 22 June 2026, 11:59 PM.
- Group size: 3-4 students unless tutor permits fewer.
- Interview/demo/Q&A: 30 minutes per group.
- Must submit one ZIP, e.g. `T13L_G04.zip`.
- Submit: `.docx` report, `.cpp` code files, `.csv` dataset input files, `.txt` output files.
- If ZIP exceeds 99 MB, provide one Microsoft OneDrive folder link in the report for large input/output files.

## 1. Non-Negotiable Rules

- Implement sorting and searching in standard C++.
- Do not use built-in sorting/searching functions.
- Do not use library data structures that sort/search internally.
- Timing must exclude file reading and printing/writing output.
- Every member must run all algorithms and include their results in the document.
- Every member must understand all submitted code and report content.
- Zero mark risks: absence from interview, plagiarism, unexplainable code/text, missing `.docx`, missing runtime proof screenshots, sharing solutions, cheating.

## 2. Project Structure TODO

- [x] Create C++17 project scaffold with CMake.
- [x] Create seven executable entry files matching assignment algorithms.
- [x] Create shared `Record` type and basic CSV/output helper placeholders.
- [ ] Fill in group member source-code header in every `.cpp`.
- [ ] Decide final executable usage style and keep it consistent with tutor demo expectations.
- [x] Confirm exact group leader student ID and derive RNG seed from assignment letter-to-digit mapping: `253UC2557B` -> `2531325572`.

## 3. Dataset Generator TODO

- [x] Implement `dataset_generator size_n`.
- [x] Output file must be named `dataset_n.csv`.
- [x] Each row format must be `integer,string` in the input dataset.
- [x] Generate unique random positive 10-digit integers.
- [x] Integer range must be `1,000,000,000` to `9,999,999,999`.
- [x] Generate 5-letter lowercase alphabet strings.
- [x] Keep all elements randomized before sorting.
- [x] Seed RNG using group leader student ID mapping:
  - A=1, B=2, ..., I=9, J=0, K=1, ..., Z=6.
  - Example in PDF: `243UC247CT` -> `2431324730`.
- [x] Handle duplicate integer generation with collision handling/regeneration.
- [ ] Generate at least 10 dataset sizes for experiments.
- [ ] Choose maximum dataset size large enough that radix sort vs heap sort runtime differs by at least 60 seconds, or justify max runtime up to/near 6 hours.
- [ ] Keep generated large datasets in OneDrive folder if submission ZIP exceeds 99 MB.

## 4. Radix Sort TODO

- [x] Implement `radix_sort dataset_n.csv`.
- [x] Sort by integer field, not string field.
- [x] Process digits from rightmost character/digit.
- [x] Preserve `(integer,string)` records in sorted output.
- [x] Save sorted rows in comma CSV row format: `integer,string`.
- [x] Use slash format `integer/string` only for display/step outputs.
- [x] Output file should be `.txt` according to confirmed requirement, e.g. `radix_sorted_dataset_n.txt`.
- [x] Include all rows in sorted output.
- [x] Print running time to command prompt.
- [x] Write running time into the `.txt` output file.
- [x] Ensure timing excludes input file reading and output writing.
- [ ] Add theoretical time complexity and space complexity explanation to report.
- [ ] Capture command prompt screenshot for every input size.
- [ ] Add result table and chart for every member.

## 5. Radix Sort Step TODO

- [ ] Implement `radix_sort_step dataset_n.csv start_row end_row`.
- [ ] Extract only rows from `start_row` to `end_row` for step display.
- [ ] Output file must be named like `dataset_1000_radix_sorted_step_startrow_endrow.txt`.
- [ ] Print original selected records in bracket format.
- [ ] Show each digit pass from rightmost to leftmost, labelled like `d=10`, `d=9`, ..., `d=1`.
- [ ] Match sample style using `integer/string` display inside step output.
- [ ] Verify with sample first 7 records from PDF.

## 6. Heap Sort TODO

- [ ] Implement `heap_sort dataset_n.csv`.
- [ ] Use maxheap.
- [ ] Do not use `std::sort`, `priority_queue`, `set`, `map`, or other internally sorting/searching containers.
- [ ] Sort by integer field, not string field.
- [ ] Preserve `(integer,string)` records in sorted output.
- [ ] Save sorted rows in comma CSV row format: `integer,string`.
- [ ] Use slash format `integer/string` only for display/step outputs.
- [ ] Output file should be `.txt` according to confirmed requirement, e.g. `heap_sorted_dataset_n.txt`.
- [ ] Include all rows in sorted output.
- [ ] Print running time to command prompt.
- [ ] Write running time into the `.txt` output file.
- [ ] Ensure timing excludes input file reading and output writing.
- [ ] Add theoretical time complexity and space complexity explanation to report.
- [ ] Capture command prompt screenshot for every input size.
- [ ] Add result table and chart for every member.

## 7. Heap Sort Step TODO

- [ ] Implement `heap_sort_step dataset_n.csv start_row end_row`.
- [ ] Extract only rows from `start_row` to `end_row` for step display.
- [ ] Output file must be named like `dataset_1000_heap_sorted_step_startrow_endrow.txt`.
- [ ] Build and display initial maxheap state.
- [ ] Show each extraction/swap/heapify step labelled like `i = 6`, `i = 5`, etc.
- [ ] Match sample style using `integer/string` display inside step output.
- [ ] Verify with sample first 7 records from PDF.

## 8. Hash Table Search TODO

- [X] Implement `hash_table_search dataset_n.csv`.
- [X] Search by integer field, not string field.
- [X] Decide hash table design that matches assignment text:
  - Direct bucket match if no collision.
  - Collision bucket represented with AVL balanced binary search tree.
- [X] Implement AVL tree manually, without library tree containers.
- [X] Implement hash function and collision strategy.
- [X] For timing, perform `n` searches where `n` is the dataset size because one search is too fast.
- [X] Measure best case, average case, and worst case.
- [X] Output file must be named like `hash_table_search_dataset_n.txt`.
- [X] Output must list running times for best, average, and worst cases.
- [X] Print running times to command prompt.
- [X] Ensure timing excludes input reading and output writing.
- [ ] Add theoretical time complexity and space complexity explanation to report.
- [ ] Compare theoretically:
  - Hash table search with array-based AVL balanced BST collision buckets.
  - Hash table search with linked-list-based AVL balanced BST collision buckets.
- [ ] Capture command prompt screenshot for every input size.
- [ ] Add result table and chart for every member.

## 9. Hash Table Search Step TODO

- [X] Implement `hash_table_search_step dataset_n.csv target`.
- [X] Tutor may specify one found target and one not-found target.
- [X] Output file must be named like `dataset_1000_hash_table_search_step_target.txt`.
- [X] Show search path:
  - Direct bucket match path when found directly.
  - AVL/tree comparison path when collision bucket is used.
  - Not-found path with compared values and final `-1 != target` style result.
- [X] Match PDF sample style:
  - Found: `2008864030 = 2008864030/rdiea`
  - Not found: `-1 != 123456789`

## 10. Experiment TODO

- [ ] Choose at least 10 input sizes for each algorithm.
- [ ] Recommended starting sizes for development: `1000`, `5000`, `10000`, `50000`, `100000`, `250000`, `500000`, `1000000`, `2500000`, `5000000`.
- [ ] After profiling, adjust largest sizes until radix vs heap differs by at least 60 seconds on same hardware.
- [ ] For every group member:
  - [ ] Run dataset generation.
  - [ ] Run radix sort on every input size.
  - [ ] Run heap sort on every input size.
  - [ ] Run hash table search on every input size.
  - [ ] Capture command prompt screenshots.
  - [ ] Capture hardware specification screenshot.
  - [ ] Record results in table.
  - [ ] Create charts for sorting and searching runtimes.
- [ ] Use same hardware for the main algorithm comparison conclusion when possible.
- [ ] Store large generated files in OneDrive and include one folder link in report.

## 11. Report / Documentation TODO

- [ ] Create final `.docx` report.
- [ ] Cover page must include:
  - Lecture section ID.
  - Tutorial section ID.
  - Group number.
  - Group ID.
  - Group leader name.
  - Group member student IDs.
  - Group member names in alphabetical order.
  - Phone numbers if required by template.
  - Task percentage as 100% for every member.
  - Task descriptions for every member.
- [ ] Include marksheet checklist table.
- [ ] Include table of contents with page numbers and links.
- [ ] Add Q1 Dataset Generation section:
  - Requirements.
  - Important code explanation.
  - Input/output examples.
  - Screenshots.
- [ ] Add Q2 Radix Sort section:
  - Algorithm explanation.
  - Time/space complexity.
  - Step output.
  - Experiment table/chart.
  - Screenshots.
- [ ] Add Q3 Heap Sort section:
  - Algorithm explanation.
  - Time/space complexity.
  - Step output.
  - Experiment table/chart.
  - Screenshots.
- [ ] Add Q4 Hash Table Search section:
  - Hash table + AVL collision design.
  - Theoretical analysis.
  - Time/space complexity.
  - Best/average/worst experiment results.
  - Step output for found and not found targets.
  - Screenshots.
- [ ] Add Q5 Conclusion:
  - Findings on same hardware.
  - Best sorting algorithm for array-based implementation.
  - AVL array-based vs linked-list-based theoretical comparison.
- [ ] Add Q6 Document clarity/completeness evidence:
  - Code files.
  - Input files.
  - Output files.
  - OneDrive folder link.
  - Runtime proof screenshots.
  - Hardware screenshots.
- [ ] Add APA7 citations and references.
- [ ] Ensure all code snippets in report are explainable by every member.

## 12. Demo Preparation TODO

- [ ] Prepare demo command for `dataset_generator` with tutor-selected size.
- [ ] Prepare demo command for `radix_sort_step` with tutor-selected start/end rows.
- [ ] Prepare demo command for `heap_sort_step` with tutor-selected start/end rows.
- [ ] Prepare demo command for `hash_table_search_step` with found target.
- [ ] Prepare demo command for `hash_table_search_step` with not-found target.
- [ ] Prepare demo command for `radix_sort dataset_1000000.csv`.
- [ ] Prepare demo command for `heap_sort dataset_1000000.csv`.
- [ ] Prepare demo command for `hash_table_search dataset_1000000.csv`.
- [ ] Assign at least one algorithm presentation to each member.
- [ ] Practice explaining every important code part without reading notes.

## 13. Rubric Targets for Full Marks

- [ ] Dataset generation: randomized `(integer,string)` rows, unique integers, suitable integer range/max runtime, all instructions followed.
- [ ] Radix sort: complete complexity analysis and demo, integer sorting, full `(integer,string)` sorted output, correct filename, no issue.
- [ ] Heap sort: complete complexity analysis and demo, integer sorting, full `(integer,string)` sorted output, correct filename, no issue.
- [ ] Hash table search: complete complexity analysis and demo, integer search, `(integer,string)` evidence/path plus best/average/worst runtimes, no issue.
- [ ] Conclusion: strongly supported by theoretical analysis and experiments, excellent AVL comparison.
- [ ] Document: complete `.docx`, citations/references, OneDrive folder link, runtime proofs, hardware specs.
- [ ] Experiments: at least 10 input files per algorithm, largest sorting runtimes differ by at least 60 seconds, all algorithms covered, full proofs/files/links.
- [ ] Interview: every member can explain code, complexity, results, and answer Q&A.

## 14. Open Questions to Confirm With Tutor / Group

- [x] Row format decision: stored dataset/sorted rows use comma format `integer,string`; display and step traces may use slash format `integer/string`.
- [x] Output extension decision: algorithm output files should be `.txt`, especially when runtime must be included.
- [ ] Exact expected filename for radix sorted output: PDF has inconsistent text around `dataset_n.radix_sorted_dataset_n.csv`; demo says `radix_sorted_dataset_1000000.csv`.
- [ ] Exact expected filename for heap sorted output: algorithm table says `heap_sort_dataset_n.csv`; demo says `heap_sorted_dataset_1000000.csv`.
- [ ] Exact hash table collision bucket requirement: confirm if AVL is required inside hash table implementation or only for theoretical comparison.
