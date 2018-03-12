import numpy as np
import pandas as pd
if __name__ == '__main__':
    filename = 'Q2data.csv'
    header = ['Id', 'State', 'City', 'Category', 'Price', 'Rating']
    data = pd.read_csv(filename, names=header)[['State', 'City', 'Category', 'Price', 'Rating']]
    data['Location'] = data[['State', 'City']].apply(lambda x : ','.join(x), axis = 1)
    Qb = data[['City', 'Category', 'Price', 'Rating']].drop_duplicates()
    #print(Q2)
    print('Qb:', len(Qb))
    Qc = data[['State', 'Category', 'Price', 'Rating']].drop_duplicates()
    #print(Qc)
    print('Qc:', len(Qc))
    Qd = data[['Category', 'Price', 'Rating']].drop_duplicates()
    #print(Qd)
    print('Qd:', len(Qd))
    Qe = data[data['State'] == 'Illinois'][data['Price'] == 'moderate'][data['Rating'] == 3]
    print('Qe:', len(Qe))
    Qf = data[data['City'] == 'Chicago'][data['Category'] == 'food']
    print('Qf:', len(Qf))