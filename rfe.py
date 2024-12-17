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

data = df_combine.drop(columns=['date', 'x_var'])

data['label'] = data['label'].astype(int)

x = data.drop(columns=['label'])
y = data['label']

scaler=MinMaxScaler()
x_scaled = scaler.fit_transform(x)

x_train, x_test, y_train, y_test = train_test_split(x_scaled, y, test_size=0.2, random_state= 42, stratify=y)

rfe = RandomForestClassifier(
    n_estimators=1000,      # Number of trees
    max_depth=10,           # Depth of the trees
    max_features=6,         # Features per split
    random_state=42
)

# Train the model
rfe.fit(x_train, y_train)

# Predictions
y_pred = rfe.predict(x_test)

print("Random Forest Ensemble:")
print("Accuracy:", accuracy_score(y_test, y_pred))
print("Classification Report:\n", classification_report(y_test, y_pred))
