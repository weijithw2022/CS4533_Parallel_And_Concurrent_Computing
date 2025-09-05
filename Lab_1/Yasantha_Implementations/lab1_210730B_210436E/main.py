import matplotlib.pyplot as plt
import csv
import subprocess
import math
import os

INITIAL_ITERATIONS = 30

SERIAL_CSV_COLUMNS = ['n', 'm', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']
PARALLEL_MUTEX_CSV_COLUMNS = ['n', 'm', 'num_threads', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']
PARALLEL_RW_CSV_COLUMNS = ['n', 'm', 'num_threads', 'mMember', 'mInsert', 'mDelete', 'ExecutionTime']

serial_linked_list_csv_file_name: str = "serial_execution_time.csv"
parallel_mutex_linked_list_csv_file_name: str = "execution_time_parallel_single_mutex.csv"
parallel_rw_linked_list_csv_file_name: str = "execution_time_parallel_rwlock.csv"

serial_linked_list_executable: str = "./serial_linked_list"
parallel_single_linked_list_executable: str = "./parallel_single_linked_list"
parallel_single_rw_linked_list_executable: str = "./parallel_single_rw_linked_list"

Threads = [1, 2, 4, 8]

Test_cases = [
    (1000, 10000, 0.99, 0.005, 0.005),
    (1000, 10000, 0.90, 0.05, 0.05),
    (1000, 10000, 0.50, 0.25, 0.25),
]


def find_needed_samples(mean: float, std: float, z_confidence: float, accuracy: float) -> int:
    n = (100 * z_confidence * std / (accuracy * mean)) ** 2
    return math.ceil(n)


# Store results for plotting later
results = []

for test_index, test_case in enumerate(Test_cases):
    n, m, mMember, mInsert, mDelete = test_case
    print("-" * 80)
    print(f"Running Test Case {test_index+1}: n={n}, m={m}, mMember={mMember}, mInsert={mInsert}, mDelete={mDelete}")

    # ---------------- SERIAL ----------------
    command = [serial_linked_list_executable, str(n), str(m), str(mMember), str(mInsert), str(mDelete)]
    for _ in range(INITIAL_ITERATIONS):
        subprocess.run(command)

    with open(serial_linked_list_csv_file_name, mode="r") as file:
        csv_reader = csv.DictReader(file, fieldnames=SERIAL_CSV_COLUMNS)
        serial_times = [float(row["ExecutionTime"]) for row in csv_reader]

    mean_serial = sum(serial_times) / len(serial_times)
    std_serial = (sum((x - mean_serial) ** 2 for x in serial_times) / len(serial_times)) ** 0.5
    os.remove(serial_linked_list_csv_file_name)

    print(f"Serial => Mean: {mean_serial:.2f}, Std: {std_serial:.2f}")

    # ---------------- MUTEX ----------------
    mutex_means, mutex_stds = [], []
    for thread in Threads:
        command = [parallel_single_linked_list_executable, str(n), str(m), str(thread), str(mMember), str(mInsert), str(mDelete)]
        for _ in range(INITIAL_ITERATIONS):
            subprocess.run(command)

        with open(parallel_mutex_linked_list_csv_file_name, mode="r") as file:
            csv_reader = csv.DictReader(file, fieldnames=PARALLEL_MUTEX_CSV_COLUMNS)
            times = [float(row["ExecutionTime"]) for row in csv_reader]

        mean_val = sum(times) / len(times)
        std_val = (sum((x - mean_val) ** 2 for x in times) / len(times)) ** 0.5
        os.remove(parallel_mutex_linked_list_csv_file_name)

        mutex_means.append(mean_val)
        mutex_stds.append(std_val)
        print(f"Mutex (Threads={thread}) => Mean: {mean_val:.2f}, Std: {std_val:.2f}")

    # ---------------- RWLOCK ----------------
    rw_means, rw_stds = [], []
    for thread in Threads:
        command = [parallel_single_rw_linked_list_executable, str(n), str(m), str(thread), str(mMember), str(mInsert), str(mDelete)]
        for _ in range(INITIAL_ITERATIONS):
            subprocess.run(command)

        with open(parallel_rw_linked_list_csv_file_name, mode="r") as file:
            csv_reader = csv.DictReader(file, fieldnames=PARALLEL_RW_CSV_COLUMNS)
            times = [float(row["ExecutionTime"]) for row in csv_reader]

        mean_val = sum(times) / len(times)
        std_val = (sum((x - mean_val) ** 2 for x in times) / len(times)) ** 0.5
        os.remove(parallel_rw_linked_list_csv_file_name)

        rw_means.append(mean_val)
        rw_stds.append(std_val)
        print(f"RWLock (Threads={thread}) => Mean: {mean_val:.2f}, Std: {std_val:.2f}")

    # Save results
    results.append({
        "serial": (mean_serial, std_serial),
        "mutex": (mutex_means, mutex_stds),
        "rw": (rw_means, rw_stds),
        "test_case": f"Test Case {test_index+1}"
    })

    # ---------------- PLOT FOR THIS TEST CASE ----------------
    plt.figure(figsize=(8, 6))
    plt.hlines(mean_serial, min(Threads), max(Threads), colors="red", linestyles="--", label=f"Serial ({mean_serial:.2f} ms)")
    plt.errorbar(Threads, mutex_means, yerr=mutex_stds, marker="o", label="Mutex")
    plt.errorbar(Threads, rw_means, yerr=rw_stds, marker="s", label="RWLock")

    plt.title(
    f"Execution Time vs Number of Threads\n"
    f"(n={n}, m={m}, mMember={mMember}, mInsert={mInsert}, mDelete={mDelete})"
)
    plt.ylabel("Execution Time (ms)")
    plt.grid(True)
    plt.legend()

    plt.figtext(0.1, 0.95, "\nClass: CS4533\nGroup: Yasantha 210436E & Weijith 210730B", ha="left", fontsize=10)

    plt.savefig(f"execution_times_test_case_{test_index+1}.png")


# ---------------- COMBINED PLOT FOR ALL TEST CASES ----------------
fig, axs = plt.subplots(1, len(results), figsize=(18, 6), sharey=True)

for i, res in enumerate(results):
    axs[i].hlines(res["serial"][0], min(Threads), max(Threads), colors="red", linestyles="--", label="Serial")
    axs[i].errorbar(Threads, res["mutex"][0], yerr=res["mutex"][1], marker="o", label="Mutex")
    axs[i].errorbar(Threads, res["rw"][0], yerr=res["rw"][1], marker="s", label="RWLock")
    axs[i].set_title(res["test_case"])
    axs[i].set_xlabel("Threads")
    axs[i].grid(True)
    if i == 0:
        axs[i].set_ylabel("Execution Time (ms)")
    axs[i].legend()

plt.suptitle("Execution Times Across Test Cases")
plt.savefig("execution_times_all_test_cases.png")
plt.show()

import matplotlib.pyplot as plt

# === One merged plot for all test cases ===
plt.figure(figsize=(10, 7))

colors = ["blue", "green", "orange"]  # one color per test case
markers = {"serial": "x", "mutex": "o", "rw": "s"}
linestyles = {"serial": "--", "mutex": "-", "rw": "-"}

for i, res in enumerate(results):
    color = colors[i % len(colors)]
    label_suffix = f"(Test Case {i+1})"

    # Serial → horizontal line across threads
    plt.hlines(
        res["serial"][0], min(Threads), max(Threads),
        colors=color, linestyles=linestyles["serial"],
        label=f"Serial {label_suffix}"
    )

    # Mutex curve
    plt.errorbar(
        Threads, res["mutex"][0], yerr=res["mutex"][1],
        marker=markers["mutex"], color=color, linestyle=linestyles["mutex"],
        label=f"Mutex {label_suffix}"
    )

    # RWLock curve
    plt.errorbar(
        Threads, res["rw"][0], yerr=res["rw"][1],
        marker=markers["rw"], color=color, linestyle=linestyles["rw"],
        label=f"RWLock {label_suffix}"
    )

plt.title("Execution Time vs Number of Threads\n(All Test Cases)")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (ms)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("execution_times_all_algorithms_one_plot.png")
plt.show()