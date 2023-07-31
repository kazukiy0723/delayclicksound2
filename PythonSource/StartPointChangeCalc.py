'''開始列の違いによる評価指標の推移の可視化'''
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

#####################
## 設定
filepath = "test731.csv"     # 読み込むファイルパス
true_values = 60 / 90 * 1000 # rmseの真値
StartPoint = 5               # 開始列
EndPoint = 30                # 終了列
##########################

# 初期化 平均値を保存する配列
mean_stdevs = []
mean_rmse = []

# csvファイルの読み込み
df = pd.read_csv(filepath)

# 開始列を1から31に変化させ、10列のデータを選択
for i in range(StartPoint, EndPoint):
    stdevs = []
    rmse = []
    
    for _, row in df.iterrows(): # 1行ずつ取り出す
        data = row[i:i+10]
        stdev = data.std()
        stdevs.append(stdev)
        rmse.append(np.sqrt(((data - true_values) ** 2).mean()))
    mean_stdevs.append(np.mean(stdevs))
    mean_rmse.append(np.mean(rmse))

# Create a figure with two subplots (1 row, 2 columns)
fig, ax = plt.subplots(1, 2, figsize=(15, 5))

# Plot mean standard deviations on the first subplot
ax[0].plot(range(1, EndPoint-StartPoint+1), mean_stdevs)
ax[0].set_xlabel("First index of data")
ax[0].set_ylabel("Standard deviation [ms]")

# Plot mean RMSE on the second subplot
ax[1].plot(range(1, EndPoint-StartPoint+1), mean_rmse)
ax[1].set_xlabel("First index of data")
ax[1].set_ylabel("RMSE [ms]")

# Show the figure with two subplots
plt.tight_layout()
plt.show()






