import pandas as pd
import numpy as np
import sys
import utils
import statistics as stat


if __name__ == "__main__":
    ids, midterms, finals = utils.read_score('data.online.scores.txt')
    df = pd.DataFrame(np.array([midterms, finals]).T, columns=['Midterm', 'Final'], index=ids)
    print(df)
    print(df['Midterm'].describe())
    print('-------------------------------------------')
    sys.stdout.write('Mode:')
    print(df['Midterm'].mode().values)
    print('-------------------------------------------')
    sys.stdout.write('Variance:')
    print(df['Midterm'].var())