import numpy as np

def read_score(file_name):
    ids = []
    midterms = []
    finals = []
    with open(file_name) as f:
        for line in f.readlines():
            id, midterm, final = [int(term) for term in line.split('\t')]
            ids.append(id)
            midterms.append(midterm)
            finals.append(final)
    return ids, midterms, finals
    
def read_library(file_name):
    with open(file_name) as f:
        lines = f.readlines()
        CML = list(map(int, lines[1].split('\t')[1:]))
        CBL = list(map(int, lines[2].split('\t')[1:]))
        return CML, CBL
        
def kl(p, q):
    p = np.asarray(p, dtype=np.float)
    q = np.asarray(q, dtype=np.float)
    return np.sum(np.where(p != 0,p * np.log(p / q), 0))