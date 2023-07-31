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

# 標準偏差のプロット
plt.figure(figsize=(10, 5))
plt.plot(range(1, EndPoint), mean_stdevs)
plt.xlabel("First index of data")
plt.ylabel("Standard deviation [ms]")
plt.show()

# RMSEのプロット
plt.figure(figsize=(10, 5))
plt.plot(range(1, EndPoint), mean_rmse)
plt.xlabel("First index of data")
plt.ylabel("RMSE [ms]")
plt.show()
