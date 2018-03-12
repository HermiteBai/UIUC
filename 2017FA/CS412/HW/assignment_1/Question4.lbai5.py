import numpy as np
import numpy.linalg as la
import sys
import utils

import scipy.stats as stat

def chisquare(data):
    n = sum(data.flatten())
    e = np.zeros(data.shape)
    for i in range(e.shape[0]):
        for j in range(e.shape[1]):
            e[i, j] = (sum(data[i,:].flatten()) * sum(data[:,j].flatten())) / n
    e = e.flatten()
    return stat.chisquare(data.flatten(), e.flatten())

if __name__ == '__main__':
    data = np.array([[150, 40], [15, 3300]])
    sys.stdout.write('chisquare: ')
    print(chisquare(data))
    