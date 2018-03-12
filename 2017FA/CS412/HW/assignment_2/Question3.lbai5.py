import numpy as np
import pandas as pd
import itertools
from collections import OrderedDict

def havePattern(l, pattern):
    return all([i in l for i in pattern])
    
def title(t):
    print('============%s============' % t)
    
def maxPatterns(support):
    max_patterns = set()
    freq_pattern = list(filter(lambda x : freq[x] >= support, freq))
    for pattern in freq_pattern:
        should_remove = []
        should_add =  1
        for max_pattern in max_patterns:
            #print(pattern, max_pattern)
            if havePattern(pattern, max_pattern) and len(max_pattern) < len(pattern):
                should_remove.append(max_pattern)
            if havePattern(max_pattern, pattern) and len(max_pattern) < len(pattern):
                should_add == 0
                break
        if should_add:
            max_patterns.add(pattern)
        for r in should_remove:
            max_patterns.remove(r)
        #print(max_patterns)
        #print('=============')
    return max_patterns
    
if __name__ == '__main__':
    filename = 'Q3data'
    items_list = []
    with open(filename, 'r') as f:
        data = f.readlines()
        print('Num of transactions : %d' % len(data))
        for line in data:
            items = line.strip().split(' ')
            items_list.append(items)
    #print(items_list)
    letters = list('ABCDEFG')
    patterns = []
    for i in range(1, len(letters) + 1):
        for j in itertools.combinations(letters, i):
            patterns.append(j)
    freq = OrderedDict()
    for pattern in patterns:
        #print(pattern)
        for items in items_list:
            if havePattern(items, pattern):
                if pattern in freq:
                    freq[pattern] += 1
                else:
                    freq[pattern] = 1
    title('Qa')
    print('1. Number of frequent pattern: %d' % len(list(filter(lambda x : freq[x] >= 20, freq))))
    print('2. Number of frequent pattern: %d' % len(list(filter(lambda x : freq[x] >= 20 and len(x) == 3, freq))))
    
    #max_patterns = set()
    max_patterns = maxPatterns(20)
    print(max_patterns)
    print('3. Number of max pattern: %d' % len(max_patterns))
    title('Qb')
    print('1. Number of frequent pattern: %d' % len(list(filter(lambda x : freq[x] >= 10, freq))))
    print('2. Number of frequent pattern: %d' % len(list(filter(lambda x : freq[x] >= 10 and len(x) == 3, freq))))
    max_patterns = maxPatterns(10)
    print(max_patterns)
    print('3. Number of max pattern: %d' % len(max_patterns))
    ACE = len(list(filter(lambda x : havePattern(x, ('A', 'C', 'E')), map(tuple, items_list))))
    CE = len(list(filter(lambda x : havePattern(x, ('C', 'E')), map(tuple, items_list))))
    print('ACE = %d, CE = %d' % (ACE, CE))
    print('4. Confidence of (C, E)->A: %f' % (float(ACE) / float(CE)))
    ABCE = len(list(filter(lambda x : havePattern(x, ('A', 'B', 'C', 'E')), map(tuple, items_list))))
    ABC = len(list(filter(lambda x : havePattern(x, ('A', 'B', 'C')), map(tuple, items_list))))
    print('ABCE = %d, ABC = %d' % (ABCE, ABC))
    print('5. Confidence of (A, B, C)->E: %f' % (float(ABCE) / float(ABC)))    
    
    