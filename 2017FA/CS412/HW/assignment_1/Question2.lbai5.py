import pandas as pd
import numpy as np
import sys
import utils

from scipy.stats import zscore

if __name__ == "__main__":
    ids, midterms, finals = utils.read_score('data.online.scores.txt')
    df = pd.DataFrame(np.array([midterms, finals]).T, columns=['Midterm', 'Final'], index=ids)
    normalized_df = df.apply(zscore)
    sys.stdout.write('Original empirical variance: ')
    print(df['Midterm'].var())
    sys.stdout.write('Normalized empirical variance: ')
    print(normalized_df['Midterm'].var())
    print('-------------------------------------------')
    sys.stdout.write('Normalized 90:')
    print((90 - df['Midterm'].mean()) / df['Midterm'].std())
    print('-------------------------------------------')
    sys.stdout.write('Pearson\'s Correlation Coefficient:')
    print(df.corr(method='pearson'))
    print('-------------------------------------------')
    sys.stdout.write('Covariance:')
    print(df.cov())