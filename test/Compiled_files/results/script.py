import os
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import gmean


### pip install numpy, matplotlib, scipy

# def geo_mean(iterable):
#     a = np.array(iterable)
#     return a.prod()**(1.0/len(a))

def analyzeCache(fileString):
    L1_loads = []
    L1_misses = []
    L1_miss_rate = []

    LLC_loads = []
    LLC_misses = []
    LLC_miss_rate = []

    directory = './'
    for filename in os.listdir('./'):
        f = os.path.join(directory, filename)
        if os.path.isfile(f) and filename.startswith(fileString):
            try:
                file1 = open(filename, 'r')
                Lines = file1.readlines()
                for line in Lines:
                    if "L1-dcache-loads" in line:
                        L1_loads.append(int(line.split()[0].replace(',', '', 1)))

                    if "L1-dcache-load-misses" in line:
                        L1_misses.append(int(line.split()[0].replace(',', '', 1)))
                        L1_miss_rate.append(float(line.split()[3].replace('%', '', 1)))

                    if "LLC-loads" in line:
                        LLC_loads.append(int(line.split()[0].replace(',', '', 1)))

                    if "LLC-load-misses" in line:
                        LLC_misses.append(int(line.split()[0].replace(',', '', 1)))
                        LLC_miss_rate.append(float(line.split()[3].replace('%', '', 1)))
            except:
                continue
    print("average number of L1_loads: ", int(gmean(L1_loads)))
    print("average number of L1_misses: ", int(gmean(L1_misses)))
    print("average L1_miss_rate: ", "{:.2f}".format(gmean(L1_miss_rate)) + "%")
    print("average number of LLC_loads: ", int(gmean(LLC_loads)))
    print("average number of LLC_misses: ", int(gmean(LLC_misses)))
    print("average LLC_miss_rate: ", "{:.2f}".format(gmean(LLC_miss_rate)) + "%")

    plt.xlabel("Percentage of LLC misses")
    plt.ylabel("Number of occurrence")
    n, bins, patches = plt.hist(LLC_miss_rate)
    plt.savefig(fileString + '.png')
    plt.clf()


def analyzeBranchInstructions(fileString):
    branch_instructions = []
    branch_loads = []
    branch_load_misses = []

    directory = './'
    for filename in os.listdir('./'):
        try:
            f = os.path.join(directory, filename)
            if os.path.isfile(f) and filename.startswith(fileString):
                file1 = open(filename, 'r')
                Lines = file1.readlines()
                for line in Lines:
                    if "branch-instructions" in line:
                        branch_instructions.append(int(line.split()[0].replace(',', '', 1)))

                    if "branch-loads" in line:
                        branch_loads.append(int(line.split()[0].replace(',', '', 1)))

                    if "branch-load-misses" in line:
                        branch_load_misses.append(int(line.split()[0].replace(',', '', 1)))
        except:
            continue
    print("average number of branch_instructions: ", int(gmean(branch_instructions)))
    print("average number of branch_loads: ", int(gmean(branch_loads)))
    print("average number of branch_load_misses: ", int(gmean(branch_load_misses)))
    print("average branch_load_misses rate: ", "{:.2f}".format(int(gmean(branch_load_misses))/int(gmean(branch_loads)) * 100) + "%")

    plt.xlabel("Number of Branch instructions executed")
    plt.ylabel("Number of occurrence")
    n, bins, patches = plt.hist(branch_instructions)
    plt.savefig(fileString + '.png')
    plt.clf()


print('Results for Dijkstra cache analysis if-conversion applied')
analyzeCache('dijkstra_full_opt_cache_Result')
print('-----------------------------------------------------------------------------\n')

print('Results for Dijkstra cache analysis if-conversion NOT applied')
analyzeCache('dijkstra_no_select_cache_results')
print('-----------------------------------------------------------------------------\n')


print('Results for Dijkstra branch analysis if-conversion applied')
analyzeBranchInstructions('dijkstra_full_opt_branch_Result')
print('-----------------------------------------------------------------------------\n')

print('Results for Dijkstra branch analysis if-conversion NOT applied')
analyzeBranchInstructions('dijkstra_no_select_branch_results')
print('-----------------------------------------------------------------------------\n')

