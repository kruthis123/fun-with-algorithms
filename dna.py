# dna

import sys                                                      # module for argv and exit functions
import csv                                                      # module for csv file functions

if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")         # program accepts only three command line arguments
    exit(1)

dict = {}

with open(sys.argv[1], 'r') as csv_file:
    csv_reader = csv.reader(csv_file)                           # csv.reader reads the lines in the file as a list
    for line in csv_reader:
        key = line[0]                                           # name is saved as key in the dictionary
        dict.setdefault(key, [])
        for i in range(1, len(line)):
            
            # store the number of times corresponding STRs repeat as a list of values for a particular key
            dict[key].append(line[i])                           
            

file = open(sys.argv[2], 'r')
s = file.read()                                                 # the sequence is read into a list in the memory

res = next(iter(dict))                                          # generates the first key in the dictionary 
count = 0
max = 0
i = 0
repeats = []

for values in dict[res]:                                        # the sequence is checked for how many times each STR repeats
    while i < len(s):
        j = i + len(values)
        t = s[i:j]
        if t != values:
            count = 0
            i += 1
        else:
            count += 1
            i = j
            if count > max:
                max = count
    repeats.append(max)                                         # maximum repeat of each STR is stored in a list
    i = 0
    count = 0
    max = 0

for key in dict:
    if key != res:
        
        # convert the values of dictionaries into int except for the first key
        
        dict[key] = [int(i) for i in dict[key]]                 
        
        if dict[key] == repeats:
            print(f"{key}")
            exit(0)

print("No match")
exit(1)