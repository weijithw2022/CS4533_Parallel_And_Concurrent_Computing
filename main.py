import matplotlib.pyplot as plt
import csv
import subprocess
import math
import os

INITIAL_ITERATIONS = 30

SERIAL_CSV_COLUMNS = ['n', 'm', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']
PARALLEL_MUTEX_CSV_COLUMNS = ['n', 'm', 'num_threads', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']
PARALLEL_RW_CSV_COLUMNS = ['n', 'm', 'num_threads', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']
serial_linked_list_csv_file_name:str = "serial_execution_time.csv"
parallel_mutex_linked_list_csv_file_name:str = "execution_time_parallel_single_mutex.csv"
parallel_rw_linked_list_csv_file_name:str = "execution_time_parallel_rwlock.csv"
serial_linked_list_executable:str = "./serial_linked_list"
parallel_single_linked_list_executable:str = "./parallel_single_linked_list"
parallel_single_rw_linked_list_executable:str = "./parallel_single_rw_linked_list"

Threads = [1, 2, 4, 8]

Test_cases = [
    (1000,10000,0.99,0.005,0.005),
    (1000,10000,0.90,0.05,0.05),
    (1000,10000,0.50,0.25,0.25),
]


def find_needed_samples(mean:float,std:float,z_confidence:float,accuracy:float)->int:
    n = (100*z_confidence*std/(accuracy*mean))**2
    return math.ceil(n)

for test_case in Test_cases:
    n, m, mMember, mInsert, mDelete = test_case
    command = [serial_linked_list_executable, str(n), str(m), str(mMember), str(mInsert), str(mDelete)]
    print("-" * 50)
    print(f"Test case: n={n}, m={m}, mMember={mMember}, mInsert={mInsert}, mDelete={mDelete}")
    for _ in range(INITIAL_ITERATIONS):
        subprocess.run(command)

    with open(serial_linked_list_csv_file_name, mode='r') as file:
        csv_reader = csv.DictReader(file, fieldnames=SERIAL_CSV_COLUMNS)
        execution_times = []
        for row in csv_reader:
            execution_times.append(float(row['ExecutionTime']))

        mean_execution_time = sum(execution_times) / len(execution_times)
        std_dev_execution_time = math.sqrt(sum((x - mean_execution_time) ** 2 for x in execution_times) / len(execution_times))
        needed_samples = find_needed_samples(mean_execution_time, std_dev_execution_time, 1.96, 5)

        print(f"Mean Execution Time: {mean_execution_time} ms and Standard Deviation: {std_dev_execution_time} ms")
        print(f"Needed Samples for 95% confidence and 5% accuracy: {needed_samples}")

    os.remove(serial_linked_list_csv_file_name)
    for _ in range(needed_samples):
        subprocess.run(command)
    
    with open(serial_linked_list_csv_file_name, mode='r') as file:
        csv_reader = csv.DictReader(file, fieldnames=SERIAL_CSV_COLUMNS)
        for row in csv_reader:
            execution_times.append(float(row['ExecutionTime']))

        mean_execution_time = sum(execution_times) / len(execution_times)
        std_dev_execution_time = math.sqrt(sum((x - mean_execution_time) ** 2 for x in execution_times) / len(execution_times))

        print(f"Experiment Outputs")
        print(f"Mean Execution Time: {mean_execution_time} ms")
        print(f"Standard Deviation: {std_dev_execution_time} ms")
        print()
    os.remove(serial_linked_list_csv_file_name)

    execution_times_final = []
    for thread in Threads:
        command = [parallel_single_linked_list_executable, str(n), str(m), str(thread), str(mMember), str(mInsert), str(mDelete)]
        print("-" * 50)
        print(f"Test case: n={n}, m={m}, num_threads={thread}, mMember={mMember}, mInsert={mInsert}, mDelete={mDelete}")
        for _ in range(INITIAL_ITERATIONS):
            subprocess.run(command)

        with open(parallel_mutex_linked_list_csv_file_name, mode='r') as file:
            csv_reader = csv.DictReader(file, fieldnames=PARALLEL_MUTEX_CSV_COLUMNS)
            execution_times = []
            for row in csv_reader:
                execution_times.append(float(row['ExecutionTime']))

            mean_execution_time = sum(execution_times) / len(execution_times)
            std_dev_execution_time = math.sqrt(sum((x - mean_execution_time) ** 2 for x in execution_times) / len(execution_times))
            needed_samples = find_needed_samples(mean_execution_time, std_dev_execution_time, 1.96, 5)

            print(f"Mean Execution Time: {mean_execution_time} ms and Standard Deviation: {std_dev_execution_time} ms")
            print(f"Needed Samples for 95% confidence and 5% accuracy: {needed_samples}")

        os.remove(parallel_mutex_linked_list_csv_file_name)

        for _ in range(needed_samples):
            subprocess.run(command)

        with open(parallel_mutex_linked_list_csv_file_name, mode='r') as file:
            csv_reader = csv.DictReader(file, fieldnames=PARALLEL_MUTEX_CSV_COLUMNS)
            for row in csv_reader:
                execution_times.append(float(row['ExecutionTime']))

            mean_execution_time = sum(execution_times) / len(execution_times)
            std_dev_execution_time = math.sqrt(sum((x - mean_execution_time) ** 2 for x in execution_times) / len(execution_times))

        print(f"Experiment Outputs")
        print(f"Mean Execution Time: {mean_execution_time} ms")
        print(f"Standard Deviation: {std_dev_execution_time} ms")
        print()
        execution_times_final.append((thread, mean_execution_time,std_dev_execution_time))
    os.remove(parallel_mutex_linked_list_csv_file_name)

    plt.figure()
    plt.errorbar([x[0] for x in execution_times_final], [x[1] for x in execution_times_final],
                 yerr=[x[2] for x in execution_times_final], fmt='o')
    plt.plot([x[0] for x in execution_times_final], [x[1] for x in execution_times_final], marker='o')
    plt.xticks(Threads)
    plt.title(f"Parallel Linked List Execution Times with Mutex - Test Case {Test_cases.index(test_case)}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time (ms)")
    plt.grid()
    plt.savefig(f"parallel_linked_list_execution_times_with_mutex_test_case_{Test_cases.index(test_case)}.png")

    execution_times_final = []
    for thread in Threads:
        command = [parallel_single_rw_linked_list_executable, str(n), str(m), str(thread), str(mMember), str(mInsert), str(mDelete)]
        print("-" * 50)
        print(f"Test case: n={n}, m={m}, num_threads={thread}, mMember={mMember}, mInsert={mInsert}, mDelete={mDelete}")
        for _ in range(INITIAL_ITERATIONS):
            subprocess.run(command)

        with open(parallel_rw_linked_list_csv_file_name, mode='r') as file:
            csv_reader = csv.DictReader(file, fieldnames=PARALLEL_RW_CSV_COLUMNS)
            execution_times = []
            for row in csv_reader:
                execution_times.append(float(row['ExecutionTime']))

            mean_execution_time = sum(execution_times) / len(execution_times)
            std_dev_execution_time = math.sqrt(sum((x - mean_execution_time) ** 2 for x in execution_times) / len(execution_times))
            needed_samples = find_needed_samples(mean_execution_time, std_dev_execution_time, 1.96, 5)

            print(f"Mean Execution Time: {mean_execution_time} ms and Standard Deviation: {std_dev_execution_time} ms")
            print(f"Needed Samples for 95% confidence and 5% accuracy: {needed_samples}")

        os.remove(parallel_rw_linked_list_csv_file_name)

        for _ in range(needed_samples):
            subprocess.run(command)

        with open(parallel_rw_linked_list_csv_file_name, mode='r') as file:
            csv_reader = csv.DictReader(file, fieldnames=PARALLEL_RW_CSV_COLUMNS)
            for row in csv_reader:
                execution_times.append(float(row['ExecutionTime']))

            mean_execution_time = sum(execution_times) / len(execution_times)
            std_dev_execution_time = math.sqrt(sum((x - mean_execution_time) ** 2 for x in execution_times) / len(execution_times))

        print(f"Experiment Outputs")
        print(f"Mean Execution Time: {mean_execution_time} ms")
        print(f"Standard Deviation: {std_dev_execution_time} ms")
        print()
        execution_times_final.append((thread, mean_execution_time,std_dev_execution_time))
    os.remove(parallel_rw_linked_list_csv_file_name)

    plt.figure()
    plt.errorbar([x[0] for x in execution_times_final], [x[1] for x in execution_times_final],
                 yerr=[x[2] for x in execution_times_final], fmt='o')
    plt.plot([x[0] for x in execution_times_final], [x[1] for x in execution_times_final], marker='o')
    plt.xticks(Threads)
    plt.title(f"Parallel Linked List Execution Times with Read-Write Locks - Test Case {Test_cases.index(test_case)}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Execution Time (ms)")
    plt.grid()
    plt.savefig(f"parallel_linked_list_execution_times_with_rwlock_test_case_{Test_cases.index(test_case)}.png")

 