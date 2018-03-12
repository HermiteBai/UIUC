#!/usr/bin/env python
import argparse
import math
import json
import sys
import random
from util import *
    
    

def get_features(data):
    features = set([key for key, val in data[0].items()])
    k = int(math.sqrt(len(features)))
    features = random.sample(features, k = k)
    return features
    
def get_random_data(data, label, N):
    num = len(label)
    candicate_id = [i for i in range(num)]
    elected_id = random.choices(candicate_id, k = N)
    sampled_data, sampled_label = [], []
    for id in elected_id:
        sampled_data.append(data[id])
        sampled_label.append(label[id])
    return sampled_data, sampled_label
    
def pred_single_in_forest(tree, forest, data):
    preds = [tree.evaluate_single(data, root) for root in forest]
    occur = get_occurrence(preds)
    return max(occur.keys(), key=lambda k: occur[k])
    
    
class DecisionTree:
    def __init__(self):
        self.maxDepth = 100000
        self.tolerance = 0.0
    
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
    parser.add_argument("-n", "--num", type=int, help="the number of trees in forest")
    args = parser.parse_args()
    #print('Train file: %s, test file: %s' % (args.train_file, args.test_file))

    train_file, test_file = args.train_file, args.test_file
    train_label, train_data = readfile(train_file)
    test_label, test_data = readfile(test_file)
    N = 50
    if args.num:
        N = args.num
    #print('Hyperparameters:')
    #print('\t %s: %d' % ('Number of trees', N))
    tree = DecisionTree()
    forest = []
    for i in range(N):
        M = int(0.8 * len(train_label))
        sampled_train_data, sampled_train_label = get_random_data(train_data, train_label, M)
        forest.append(tree.train(sampled_train_data, sampled_train_label, gini_gain))
    
    preds_train = [pred_single_in_forest(tree, forest, data) for data in train_data]
    preds_test = [pred_single_in_forest(tree, forest, data) for data in test_data]
    #print('Training Accuracy: %f' % acc(preds_train, train_label))
    #print('Testing Accuracy: %f' % acc(preds_test, test_label))
    k = len(set(test_label).union(set(train_label)))
    confusion = [[0 for i in range(k)] for j in range(k)]
    for pred, label in zip(preds_test, test_label):
        confusion[pred - 1][label - 1] += 1
    for row in confusion:
        sys.stdout.write(str(row[0]))
        for num in row[1:]:
            sys.stdout.write(' ')
            sys.stdout.write(str(num))
        sys.stdout.write('\n')
    #for i in range(len(confusion)):
    #    evaluate(confusion, i)
    
    
    