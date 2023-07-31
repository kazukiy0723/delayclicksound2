# Import required libraries
import pandas as pd
import numpy as np
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt

# Define the predicted value
d = 60 / 90 * 1000

# Load the CSV file without header
filename  = r'C:\Users\1999k\OneDrive\ドキュメント\大学\村上研\クリック実験\delayclicksound2_yamashita\RESULT\Result_unique_yamashita\BPM90_10ms_roup300.csv'
df = pd.read_csv(filename, header=None)

# Define Group A and Group B
# Define Group A and Group B
group_A = df.iloc[0:3]
group_B = df.iloc[3:6]

# Drop NaN values in Group A and Group B
group_A = group_A.dropna(axis=1)
group_B = group_B.dropna(axis=1)

# Calculate the average standard deviation for Group A and Group B
avg_std_dev_A = group_A.std(axis=1).mean()
avg_std_dev_B = group_B.std(axis=1).mean()

# Calculate the RMSE for Group A and Group B
rmse_A = np.sqrt(mean_squared_error([d]*len(group_A.values.ravel()), group_A.values.ravel()))
rmse_B = np.sqrt(mean_squared_error([d]*len(group_B.values.ravel()), group_B.values.ravel()))

# x-axis labels
x_labels = ["A", "B"]

# Data to plot
avg_std_devs = [avg_std_dev_A, avg_std_dev_B]
rmse_values = [rmse_A, rmse_B]

# Bar plot settings
x = np.arange(len(x_labels))  # label locations
width = 0.35  # width of the bars

# Create the plot
fig, ax = plt.subplots()
rects1 = ax.bar(x - width/2, avg_std_devs, width, label='Average Std Dev')
rects2 = ax.bar(x + width/2, rmse_values, width, label='RMSE')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_xlabel('Group',fontsize=22)
ax.set_ylabel('Evaluation index[ms]',fontsize=22)
ax.set_xticks(x)
# Set the limits of y-axis
ax.set_ylim([0, 40])
ax.set_xticklabels(x_labels, fontsize=20)
ax.legend(fontsize=20)

# Attach a text label above each bar in rects1 and rects2, displaying its height
for rect in rects1:
    height = rect.get_height()
    ax.annotate('{}'.format(round(height, 2)),
                xy=(rect.get_x() + rect.get_width() / 2, height),
                xytext=(0, 3),  # 3 points vertical offset
                textcoords="offset points",
                ha='center', va='bottom',fontsize=22)
for rect in rects2:
    height = rect.get_height()
    ax.annotate('{}'.format(round(height, 2)),
                xy=(rect.get_x() + rect.get_width() / 2, height),
                xytext=(0, 3),  # 3 points vertical offset
                textcoords="offset points",
                ha='center', va='bottom',fontsize=22)

# Show the plot
plt.show()
