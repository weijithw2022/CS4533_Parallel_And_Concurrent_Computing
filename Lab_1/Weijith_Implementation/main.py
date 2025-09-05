import csv

# Data structured as: (n, m, mMember, mInsert, mDelete, implementation, [ (threads, avg, std), ... ])
data = [
    (1000, 10000, 0.99, 0.005, 0.005, 'Serial', [(1, 0.006444, 0.000168)]),
    (1000, 10000, 0.99, 0.005, 0.005, 'One mutex for entire list', [
        (1, 0.006617, 0.000203), (2, 0.005783, 0.000623), (4, 0.007099, 0.002486), (8, 0.011332, 0.002038)
    ]),
    (1000, 10000, 0.99, 0.005, 0.005, 'Read Write lock', [
        (1, 0.00653, 0.000164), (2, 0.014925, 0.002818), (4, 0.00173, 0.000038), (8, 0.018994, 0.000982)
    ]),
    (1000, 10000, 0.9, 0.05, 0.05, 'Serial', [(1, 0.006895, 0.000544)]),
    (1000, 10000, 0.9, 0.05, 0.05, 'One mutex for entire list', [
        (1, 0.007004, 0.001138), (2, 0.006008, 0.000905), (4, 0.00592, 0.002817), (8, 0.009897, 0.001742)
    ]),
    (1000, 10000, 0.9, 0.05, 0.05, 'Read Write lock', [
        (1, 0.00697, 0.00034), (2, 0.016162, 0.002355), (4, 0.020183, 0.002363), (8, 0.020083, 0.001829)
    ]),
    (1000, 10000, 0.5, 0.25, 0.25, 'Serial', [(1, 0.010367, 0.0)]),
    (1000, 10000, 0.5, 0.25, 0.25, 'One mutex for entire list', [
        (1, 0.010367, 0.000237), (2, 0.006896, 0.001241), (4, 0.009716, 0.002325), (8, 0.01257, 0.006172)
    ]),
    (1000, 10000, 0.5, 0.25, 0.25, 'Read Write lock', [
        (1, 0.010397, 0.0), (2, 0.019008, 0.002562), (4, 0.023832, 0.004786), (8, 0.023436, 0.006648)
    ]),
]

# CSV filename
filename = 'linkedlist_results.csv'

# Write CSV
with open(filename, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    # Header
    writer.writerow(['n', 'm', 'mMember', 'mInsert', 'mDelete', 'Implementation', 'Threads', 'Average', 'Std'])
    
    # Write rows
    for entry in data:
        n, m, mMember, mInsert, mDelete, implementation, results = entry
        for thread, avg, std in results:
            writer.writerow([n, m, mMember, mInsert, mDelete, implementation, thread, avg, std])

print(f"Data written to {filename}")
