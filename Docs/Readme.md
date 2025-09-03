# Lab 01: Parallel and Concurrent Programming

## Environment Setup

This lab uses both Python and C. To set up the environment:

1. **Python Environment**
   - A virtual environment can be created using `venv`:
     ```sh
     python -m venv .
     ```
   - Required Python packages are listed in `requirements.txt`.
   - To activate the environment:
     ```sh
     source bin/activate
     ```
   - To install packages (if needed):
     ```sh
     pip install -r requirements.txt
     ```

2. **C Compiler**
   - The lab uses `clang` for compiling C files. Ensure `clang` is installed:
     ```sh
     clang --version
     ```
   - Alternatively, you can use `gcc` by editing the `Makefile`.

## Compiling the C Files

A `Makefile` is provided for easy compilation.

To compile both serial and parallel executables, run:
```sh
make all 
```
This will generate:
- `serial_linked_list`
- `parallel_single_linked_list`

To clean up compiled files:
```sh
make clean
```

## Running the Scripts

### 1. Serial Linked List (C)

Run the serial linked list executable with:
```sh
./serial_linked_list <n> <m> <mMember> <mInsert> <mDelete>
```
- `n`: Initial number of elements in the list
- `m`: Total number of operations
- `mMember`: Fraction of member operations (e.g., 0.99)
- `mInsert`: Fraction of insert operations (e.g., 0.005)
- `mDelete`: Fraction of delete operations (e.g., 0.005)

Example:
```sh
./serial_linked_list 1000 10000 0.99 0.005 0.005
```

### 2. Parallel Linked List (C)

Run the parallel linked list executable with:
```sh
./parallel_single_linked_list <n> <m> <num_threads> <mMember> <mInsert> <mDelete>
```
- `n`: Initial number of elements
- `m`: Total operations
- `num_threads`: Number of threads
- `mMember`, `mInsert`, `mDelete`: Fractions as above

Example:
```sh
./parallel_single_linked_list 1000 10000 4 0.99 0.005 0.005
```

### 3. Python Script

The Python script `main.py` automates running the serial C executable, collects timing data, and analyzes results.

To run:
```sh
python main.py
```
- Ensure the serial executable (`serial_linked_list`) is compiled and present in the same directory.
- The script will print experiment results and required sample sizes for statistical confidence.

## Output Files
- `serial_execution_time.csv`: Created by the serial C program and used by `main.py`.
- `execution_time_parallel_single_mutex.csv`: Created by the parallel C program.

## Notes
- Make sure to activate the Python environment before running Python scripts.
- If you encounter permission issues, you may need to make the executables runnable:
  ```sh
  chmod +x serial_linked_list parallel_single_linked_list
  ```

---
For any issues, refer to the comments in the source files or contact your instructor. 
