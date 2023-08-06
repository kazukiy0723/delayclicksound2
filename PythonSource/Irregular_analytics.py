'''実験条件が「変則」の場合のデータ分析'''

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import mean_squared_error

#####################
## 設定
filepath = "./delay10ms_1.csv"    # 読み込むファイルパス
d = 60 / 90 * 1000    # rmseの真値
StartPoint = 3                  # 開始列
EndPoint = 33                   # 終了列
######################

# 初期化 平均値を保存する配列
mean_stdevs = []
mean_rmse = []

# csvファイルの読み込み
df = pd.read_csv(filepath, header=None)

# 
df_selected = df.iloc[:, StartPoint:EndPoint+1]

group_A = df_selected.iloc[:, ::2]
group_B = df_selected.iloc[:, 1::2]

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
