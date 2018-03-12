#!/usr/bin/env python
import argparse
import math
import json
import sys

from util import *

def get_features(data):
    features = set([key for key, val in data[0].items()])
    return features
    
class DecisionTree:
    def __init__(self, maxDepth = 10, tol = 0.0):
        self.maxDepth = maxDepth
        self.tolerance = tol
    
    def train(self, data, label, evaluation, depth = 0):
        root = dict()
        if depth == self.maxDepth or len(data) == 1:
            occur = get_occurrence(label)
            majority = max(occur.keys(), key=lambda k: occur[k])
            root['prediction'] = majority
            root['leaf'] = 1
            return root
        features = get_features(data)
        feature, gain = pick_feature(features, data, label, evaluation)
        subtrees = split_by_feature(feature, data, label, evaluation)
        
        occur = get_occurrence(label)
        majority = max(occur.keys(), key=lambda k: occur[k])
        root['prediction'] = majority
        if gain <= self.tolerance:
            root['leaf'] = 1
            return root
        root['feature'] = feature
        root['subtrees'] = {key : self.train(subtrees[key][0], subtrees[key][1], evaluation, depth + 1) for key in subtrees.keys()}
        return root
        
    def evaluate_single(self, data, root):
        curr = root
        if 'leaf' in curr:
            return curr['prediction']
        else:
            feature = curr['feature']
            val = data[feature]
            subtrees = curr['subtrees']
            return self.evaluate_single(data, subtrees[val]) if val in subtrees else curr['prediction']
        
        

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('train_file', type=str, help="the training data file")
    parser.add_argument('test_file', type=str, help="the testing data file")
    parser.add_argument("-d", "--depth", type=int, help="the maximum depth allowed")
    parser.add_argument("-t", "--tol", type=float, help="the minimum gini gain required")
    args = parser.parse_args()
    #print('Train file: %s, test file: %s' % (args.train_file, args.test_file))
    #print('Hyperparameters:')
    #if args.depth:
    #    print('\t %s: %d' % ('Max Depth', args.depth))
    #else:
    #    print('\t %s: %d' % ('Max Depth', 10))
    #if args.tol:
    #    print('\t %s: %f' % ('Min Gini Gain', args.tol))
    #else:
    #    print('\t %s: %f' % ('Min Gini Gain', 0.0))
    train_file, test_file = args.train_file, args.test_file
    depth, tol = args.depth, args.tol
    train_label, train_data = readfile(train_file)
    test_label, test_data = readfile(test_file)
    
    tree = None
    if depth and not tol:
        tree = DecisionTree(maxDepth=depth)
    elif tol and not depth:
        tree = DecisionTree(tol=tol)
    elif depth and tol:
        tree = DecisionTree(maxDepth=depth, tol=tol)
    else:
        tree = DecisionTree()
    trainedTree = tree.train(train_data, train_label, gini_gain)
    train_preds = [tree.evaluate_single(data, trainedTree) for data in train_data]
    test_preds = [tree.evaluate_single(data, trainedTree) for data in test_data]
    #print('Training Accuracy: %.3f' % acc(train_preds, train_label))
    #print('Testing Accuracy: %.3f' % acc(test_preds, test_label))
    
    k = len(set(test_label).union(set(train_label)))
    confusion = [[0 for i in range(k)] for j in range(k)]
    for pred, label in zip(test_preds, test_label):
        confusion[pred - 1][label - 1] += 1
    for row in confusion:
        sys.stdout.write(str(row[0]))
        for num in row[1:]:
            sys.stdout.write(' ')
            sys.stdout.write(str(num))
        sys.stdout.write('\n')
    #for i in range(len(confusion)):
    #    evaluate(confusion, i)
        
    
    
    