import pandas as pd
import matplotlib.pyplot as plt

# Read CSV
filename = 'linkedlist_results.csv'
df = pd.read_csv(filename)

# Replace this with your MLA heading for your report
mla_heading = "210730B, 210436E, CS4533, 4/10/2025"

# Unique test cases based on workload parameters (excluding Implementation)
workloads = df[['n', 'm', 'mMember', 'mInsert', 'mDelete']].drop_duplicates()

# Plot for each workload, comparing implementations
for _, workload in workloads.iterrows():
    n, m, mMember, mInsert, mDelete = workload
    subset = df[
        (df['n'] == n) &
        (df['m'] == m) &
        (df['mMember'] == mMember) &
        (df['mInsert'] == mInsert) &
        (df['mDelete'] == mDelete)
    ]
    
    plt.figure(figsize=(10, 6))  # Fill the space nicely
    
    # Plot each implementation
    for implementation in subset['Implementation'].unique():
        impl_data = subset[subset['Implementation'] == implementation]
        threads = impl_data['Threads']
        avg = impl_data['Average']
        std = impl_data['Std']
        plt.errorbar(
            threads, avg, yerr=std, fmt='-o', capsize=5,
            label=implementation
        )
    
    # Title and labels
    plt.title(f"Execution Time vs Threads\nn={n}, m={m}, mMember={mMember}, mInsert={mInsert}, mDelete={mDelete}", fontsize=14)
    plt.xlabel("Number of Threads", fontsize=12)
    plt.ylabel("Average Execution Time (seconds)", fontsize=12)
    plt.xticks(threads)  # Show all thread counts
    plt.grid(True)
    plt.legend(title="Implementation")
    
    # Optional: add MLA heading as text in the figure
    plt.text(0, max(subset['Average']) * 1.05, mla_heading, fontsize=10, ha='left', va='bottom')
    
    plt.tight_layout()
    plt.show()
