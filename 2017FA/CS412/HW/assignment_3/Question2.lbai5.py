#!/usr/bin/env python
from collections import Counter
import itertools
import sys
def inSequence(pattern, seq, r):
    window_size = len(pattern)
    for s in range(window_size, len(seq)):
        #print('s:%d' % s)
        for i in range(len(seq) - window_size + 1):
            outlier = list(filter(lambda x : x not in pattern, seq[i : i + s]))
            #print(seq[i : i + s], pattern, outlier)
            if all([j in seq[i : i + s] for j in pattern]) and len(outlier) / (len(seq[i : i + s]) - len(outlier)) <= r:
                return True
            
    return False
        
file_name = 'Data.txt'
with open(file_name, 'r') as f:
    with open('output.txt', 'w') as output:
        lines = f.readlines()
        params = list(map(float, lines[0].strip().replace(" ", "").split(',')))
        support, resilient = params
        S1 = list(map(int, lines[1].strip().replace(" ", "").split(',')))
        S2 = list(map(int, lines[2].strip().replace(" ", "").split(',')))
        S3 = list(map(int, lines[3].strip().replace(" ", "").split(',')))
        patterns = list()
        for i in range(1, 9):
            for j in itertools.combinations(range(1, 9), i):
                patterns.append(j)
        ps = []
        for pattern in patterns:
            count = 0.
            if inSequence(pattern, S1, resilient):
                count += 1
            if inSequence(pattern, S2, resilient):
                count += 1
            if inSequence(pattern, S3, resilient):
                count += 1
            if count / 3 >= support:
                sys.stdout.write(str(pattern[0]))
                for e in pattern[1:]:
                    sys.stdout.write(', ')
                    sys.stdout.write(str(e))
                    
                #output.write(str(pattern[0]))
                p = [pattern[0]]
                for e in pattern[1:]:
                    #output.write(', ')
                    p.append(e)
                    #output.write(str(e))
                #output.write('\n')
                ps.append(p)
                
                sys.stdout.write(': ')
                if inSequence(pattern, S1, resilient):
                    sys.stdout.write('S1 ')
                if inSequence(pattern, S2, resilient):
                    sys.stdout.write('S2 ')
                if inSequence(pattern, S3, resilient):
                    sys.stdout.write('S3')
                sys.stdout.write('\n')
        for p in sorted(ps):
            print(p)
            output.write(str(p[0]))
            for e in p[1:]:
                output.write(', ')
                output.write(str(e))
            output.write('\n')