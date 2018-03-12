#!/usr/bin/env python

def readfile(filename):
    with open(filename, 'r') as f:
        lines = f.readlines()
        labels, data = [], []
        for line in lines:
            terms = line.split(' ')
            label = terms[0]
            params = {int(param.split(':')[0]) : int(param.split(':')[1]) for param in terms[1:]}
            labels.append(int(label))
            data.append(params)
        return labels, data
        
def gini_gain(pre_split, post_split):
    pre_total = sum([val for key, val in pre_split.items()])
    pre_prob = [val / pre_total for key, val in pre_split.items()]
    pre_gini = 1 - sum([prob ** 2 for prob in pre_prob])


    for post_dict in post_split:
        post_total = sum([val for key, val in post_dict.items()])
        post_prob = [val / post_total for key, val in post_dict.items()]
        post_gini = (post_total / pre_total) * (1 - sum([prob ** 2 for prob in post_prob]))
        pre_gini -= post_gini
    return pre_gini
    
def split_by_feature(feature, data, label, evaluation):
    distinct_values = set([entry[feature] for entry in data])
    splitted = {value : [[], []] for value in distinct_values}
    for i in range(len(data)):
        splitted[data[i][feature]][0].append(data[i])
        splitted[data[i][feature]][1].append(label[i])
    return splitted

def get_occurrence(labels):
    occur = dict()
    for label in labels:
        if label in occur:
            occur[label] += 1
        else:
            occur[label] = 1
    return occur
    
    
def pick_feature(features, data, label, evaluation):
    pre_split = get_occurrence(label)
    improvements = dict()
    for feature in features:
        splitteds = split_by_feature(feature, data, label, evaluation)
        post_split = [get_occurrence(splitted[1]) for splitted in splitteds.values()]
        improvements[feature] = gini_gain(pre_split, post_split)
    best_feature = max(improvements.keys(), key=lambda k: improvements[k])
    best_gain = improvements[best_feature]
    return best_feature, best_gain
    
def acc(preds, labels):
    acc = [int(pred == label) for pred, label in zip(preds, labels)]
    return sum(acc) / len(acc)
    
def evaluate(confusion, c):
    truePositive = round(confusion[c][c], 3)
    trueNegative = round(sum([confusion[i][j] for i in range(len(confusion)) for j in range(len(confusion)) if i != c and j != c]), 3)
    falseNegative = round(sum([confusion[i][c] for i in range(len(confusion)) if i != c]), 3)
    falsePositive = round(sum([confusion[c][i] for i in range(len(confusion))  if i != c]), 3)
    accuracy = round((truePositive + trueNegative) / (truePositive + trueNegative + falseNegative + falsePositive), 3)
    specificity = round(trueNegative / (trueNegative + falsePositive), 3) if trueNegative + falsePositive != 0 else 'N/A'
    precision = round(truePositive / (truePositive + falsePositive), 3) if truePositive + falsePositive != 0 else 'N/A'
    recall = round(truePositive / (truePositive + falseNegative), 3) if truePositive + falseNegative != 0 else 'N/A'
    f1 = round(2 * (precision * recall) / (precision + recall), 3) if recall != 'N/A' and precision != 'N/A' and precision + recall != 0 else 'N/A'
    f05 = round((1 + 0.5 ** 2) * (precision * recall) / ((0.5 ** 2 * precision) + recall), 3) if recall != 'N/A' and precision != 'N/A' and (0.5 ** 2 * precision) + recall != 0 else 'N/A'
    f2 = round((1 + 2 ** 2) * (precision * recall) / ((2 ** 2 * precision) + recall), 3) if recall != 'N/A' and precision != 'N/A' and (2 ** 2 * precision) + recall != 0 else 'N/A'
    print('%s & %s & %s& %s& %s& %s& %s' % (str(accuracy), str(specificity), str(precision), str(recall), str(f1), str(f05), str(f2)))