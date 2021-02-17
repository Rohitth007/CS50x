# https://stackoverflow.com/questions/1374457/find-out-how-many-times-a-regex-matches-in-a-string-in-python
# https://stackoverflow.com/questions/2970520/string-count-with-overlapping-occurrences
# https://www.geeksforgeeks.org/python-maximum-consecutive-substring-occurrence/?ref=rp
from sys import argv, exit
import csv
import re

if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)   # Equivalent to return 1; in C 

with open(argv[1], 'r') as data:     # Another way to open files noe need to close later
    reader = csv.DictReader(data)    # Converts each row into a dictionary with keys as the values in the first row
    headers = reader.fieldnames      # Converts the first row into a list

    dna_profile = open(argv[2], 'r')  
    sequence = dna_profile.read()     # Reading the whole file, since file has only one line it is same as readlines()

    ''' 
    *   re.findall(substr_to_search, whole_str) finds all the strings that 
    follow the pattern in substr_to_search and puts them in a list.
    *   ?: ignores naming for a parenthesis group, + searches for 1 or more occurences. 
    *   sequence.count(substr_to_search) was tried but counts 
    all occurences even if they are not consecutive, which is not what we want. 
    '''
    # For each row_dictionary in reader I checked if value of row_dict[column] equals the lenth of STR for a given sequence.
    for row_dict in reader:
        match = 0
        for column in headers:
            pattern_list = re.findall('((?:' + column + ')+)', sequence)   
            if pattern_list != []:  # Some columns in the headers list have 0 pattern occurences which causes error for max()
                STR_len = len(max(pattern_list, key=len))//len(column)  # gives length of longest string in a list of strings
                if int(row_dict[column]) == STR_len:
                    match += 1
        if match == len(headers) - 1:  # Only if all STR types matches we get a DNA match
            print(row_dict['name'])
            dna_profile.close()
            exit(0)
    print('No Match')
    dna_profile.close()
