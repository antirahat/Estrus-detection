import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split, StratifiedKFold
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import accuracy_score, classification_report


#label filter all the dataset

file_path1 = 'resampled_16_5s.csv'
file_path2= 'resampled_16_3s.csv'
file_path3= 'resampled_16_1s.csv'
gyro5s = pd.read_csv(file_path1)
gyro3s = pd.read_csv(file_path2)
gyro1s = pd.read_csv(file_path3)

filtered_gyro5s = gyro5s[~gyro5s['label'].isin([2.0, 4.0])]
output_file = 'filtered_gyro5s.csv'  
filtered_gyro5s.to_csv(output_file, index=False)

filtered_gyro3s = gyro3s[~gyro3s['label'].isin([2.0, 4.0])]
output_file = 'filtered_gyro3s.csv'  
filtered_gyro3s.to_csv(output_file, index=False)

filtered_gyro1s = gyro3s[~gyro3s['label'].isin([2.0, 4.0])]
output_file = 'filtered_gyro1s.csv'  
filtered_gyro1s.to_csv(output_file, index=False)

#combine the whole dataset into one 

df_combine = pd.concat([filtered_gyro1s,filtered_gyro3s, filtered_gyro5s], ignore_index= True)
output_file = 'combined.csv'
df_combine.to_csv(output_file, index = False)

