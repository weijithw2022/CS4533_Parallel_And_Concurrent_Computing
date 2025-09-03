import matplotlib.pyplot as plt
import csv
import subprocess
import math
import os

INITIAL_ITERATIONS = 30

SERIAL_CSV_COLUMNS = ['n', 'm', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']
serial_linked_list_csv_file_name:str = "serial_execution_time.csv"
serial_linked_list_executable:str = "./serial_linked_list"

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
