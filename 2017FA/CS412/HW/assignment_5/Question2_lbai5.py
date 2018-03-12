#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import mode
from sklearn.neighbors import NearestNeighbors
from sklearn.metrics import accuracy_score
from sklearn.linear_model import SGDClassifier

test_X = [[2.7, 2.7], [2.5, 1], [1.5, 2.5], [1.2, 1]]
test_Y = [1, 1, -1, -1]
train_X = [[1, 0.5], [2, 1.2], [2.5, 2], [3, 2], [1.5, 2], [2.3, 3], [1.2, 1.9], [0.8, 1]]
train_Y = [1, 1, 1, 1, -1, -1, -1, -1]

train_positive = np.array([train_X[i] for i in range(len(train_X)) if train_Y[i] == 1], dtype=np.float)
train_negative = np.array([train_X[i] for i in range(len(train_X)) if train_Y[i] == -1], dtype=np.float)

train_positive_X = train_positive.T[0]
train_positive_Y = train_positive.T[1]

train_negative_X = train_negative.T[0]
train_negative_Y = train_negative.T[1]

plt.scatter(train_positive_X, train_positive_Y, c='r', marker='+')
plt.scatter(train_negative_X, train_negative_Y, c='b', marker='o')

test_positive = np.array([test_X[i] for i in range(len(test_X)) if test_Y[i] == 1], dtype=np.float)
test_negative = np.array([test_X[i] for i in range(len(test_X)) if test_Y[i] == -1], dtype=np.float)

test_positive_X = test_positive.T[0]
test_positive_Y = test_positive.T[1]

test_negative_X = test_negative.T[0]
test_negative_Y = test_negative.T[1]

#plt.scatter(test_positive_X, test_positive_Y, c='g', marker='+')
#plt.scatter(test_negative_X, test_negative_Y, c='g', marker='o')

nearestNeighbors1 = NearestNeighbors(n_neighbors=1, metric='euclidean')
nearestNeighbors1.fit(train_X)
dist, ind = nearestNeighbors1.kneighbors(test_X)
print(ind)
pred = list(np.array(train_Y)[ind].flatten())
print(pred)
error1 = 1 - accuracy_score(test_Y, pred)
print('Error when K = 1: %f'  % error1)

nearestNeighbors1 = NearestNeighbors(n_neighbors=3, metric='euclidean')
nearestNeighbors1.fit(train_X)
dist, ind = nearestNeighbors1.kneighbors(test_X)
print(ind)
pred = np.array(train_Y)[ind].T
mode, _ = mode(pred)
pred = mode.flatten()
print(pred)
error3 = 1 - accuracy_score(test_Y, pred)
print('Error when K = 3: %f'  % error3)

classifier = SGDClassifier(alpha=0.001)
classifier.fit(train_X, train_Y)
pred = classifier.predict(test_X)
error_linear = 1 - accuracy_score(test_Y, pred)
print('Error of linear model: %f'  % error_linear)
print(classifier.coef_)
print(classifier.intercept_)