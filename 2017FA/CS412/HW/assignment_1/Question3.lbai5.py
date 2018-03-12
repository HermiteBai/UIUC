import numpy as np
import numpy.linalg as la
import sys
import utils

from sklearn.metrics.pairwise import cosine_similarity

if __name__ == "__main__":
    CML, CBL = utils.read_library('data.libraries.inventories.txt')
    CML = np.array(CML)
    CBL = np.array(CBL)
    print('b.')
    sys.stdout.write('Norm-1:')
    print(la.norm(CML - CBL, 1))
    sys.stdout.write('Norm-2:')
    print(la.norm(CML - CBL, 2))
    sys.stdout.write('Norm-infty:')
    print(la.norm(CML - CBL, np.inf))
    print('---------------------------')
    print('c.')
    sys.stdout.write('Cosine similarity:')
    print(cosine_similarity(CML.reshape(1, -1), CBL.reshape(1, -1), dense_output=False))
    print('---------------------------')
    CML_prob = np.array([float(x) / sum(CML) for x in CML])
    CBL_prob = np.array([float(x) / sum(CBL) for x in CBL])
    print('d.')
    sys.stdout.write('K-L Divergence:')
    print(utils.kl(CML_prob, CBL_prob))