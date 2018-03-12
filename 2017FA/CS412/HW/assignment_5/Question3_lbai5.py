#!/usr/bin/env python
import numpy as np
import numpy.linalg as la
import matplotlib.pyplot as plt
from random import choice

def dist(p1, p2):
    return la.norm(p1 - p2)

train_X = np.array([[1,3], [1,2], [2,1], [2,2], [2,3], [3,2], [5,3], [4,3], [4,5], [5,4], [5,5], [6,4], [6,5]])
centers = np.array([[0, 4], [6, 5]])

#plt.scatter(train_X.T[0],train_X.T[1])
num_iter = 2
cluster1, cluster2 = [], []
for i in range(num_iter):
    print('Iteration %d:\n' % (i + 1))
    cluster1, cluster2 = [], []
    for X in train_X:
        d1 = dist(X, centers[0])
        d2 = dist(X, centers[1])
        if d1 <= d2:
            print('\t$d1 = %f \leq d2 = %f$, $%r$ in cluster 1\n' % (d1, d2, tuple(X)))
            cluster1.append(X)
        else:
            print('\t$d1 = %f > d2 = %f$, $%r$ in cluster 2\n' % (d1, d2, tuple(X)))
            cluster2.append(X)
    centers[0] = np.average(cluster1, axis=0)
    centers[1] = np.average(cluster2, axis=0)
    print('\tCenter1 : %r, Center2 : $%r$\n' % (tuple(centers[0]), tuple(centers[1])))
print('Cluster 1: $%r$\n' % list(map(tuple, cluster1)))
print('Cluster 2: $%r$\n' % list(map(tuple, cluster2)))
cluster1 = np.array(cluster1)
cluster2 = np.array(cluster2)
#plt.scatter(cluster1.T[0],cluster1.T[1], c='r')
#plt.scatter(cluster2.T[0],cluster2.T[1], c='b')

#plt.show()
MinPts, Eps = 2, 1.5
unvisited, visited = set(list(map(tuple, list(train_X)))), set()
in_cluster = set()
clusters = []
outliers = set()
while len(unvisited) != 0:
    p = choice(list(unvisited))
    print(p)
    unvisited.remove(p)
    visited.add(p)
    ##########################################
    counter = 0
    N = set()
    for point in list(map(tuple, list(train_X))):
        if tuple(point) == tuple(p):
            continue
        if dist(np.array(p), np.array(point)) <= Eps:
            counter += 1
            N.add(point)
            print(point)
    ##########################################
    cluster = set()
    print('counter = %d' % counter)
    if counter >= MinPts:
        cluster.add(p)
        while len(N) != 0:
            p_n = N.pop()
            if p_n in unvisited:
                unvisited.remove(p_n)
                visited.add(p_n)
                ##########################################
                counter_n = 0
                N_n = set()
                for point in list(map(tuple, list(train_X))):
                    if tuple(point) == tuple(p_n):
                        continue
                    if dist(np.array(p_n), np.array(point)) <= Eps:
                        counter_n += 1
                        N_n.add(point)
                if counter_n >= MinPts:
                    N.update(N_n)
                ##########################################
            if p_n not in in_cluster:
                cluster.add(p_n)
                in_cluster.add(p_n)
        clusters.append(cluster)
    else:
        outliers.add(p)
print(clusters)
cs = ['r', 'g', 'b', 'w']
#cluster1 = np.array(list(map(list, clusters[0])))
#cluster2 = np.array(list(map(list, clusters[1])))
i = 0
for cluster in clusters:
    cluster = np.array(list(map(list, cluster)))
    plt.scatter(cluster.T[0], cluster.T[1], c=cs[i % 4])
    i += 1

plt.show()
            
            
        



