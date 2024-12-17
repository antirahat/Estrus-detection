import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split, StratifiedKFold, cross_val_score
from sklearn.preprocessing import MinMaxScaler
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix


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

dt_model = DecisionTreeClassifier(
    criterion='entropy',
    random_state=42
)

# Train the model
dt_model.fit(x_train, y_train)

# Predictions
y_pred = dt_model.predict(x_test)

# Accuracy and classification report
print("Decision Tree Model Performance:")
print("Accuracy:", accuracy_score(y_test, y_pred))
print("\nClassification Report:\n", classification_report(y_test, y_pred))

# Confusion matrix
cm = confusion_matrix(y_test, y_pred)
sns.heatmap(cm, annot=True, fmt="d", cmap="Blues", xticklabels=sorted(y.unique()), yticklabels=sorted(y.unique()))
plt.title("Confusion Matrix for Decision Tree")
plt.xlabel("Predicted")
plt.ylabel("True")
plt.show()

# -----------------------------
# 5. Cross-Validation
# -----------------------------
cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)
cv_scores = cross_val_score(dt_model, x_scaled, y, cv=cv, scoring='accuracy')

print(f"\nCross-Validation Accuracy: {cv_scores.mean():.4f} ± {cv_scores.std():.4f}")