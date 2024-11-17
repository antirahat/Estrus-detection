import pandas as pd 

import csv

# Open and read the CSV file
with open('all_data.csv', mode='r') as file:
    csv_reader = csv.reader(file)
    
    # Convert to a list (optional)
    data = list(csv_reader)

# Print the first row
print(data[0])  # Header row

