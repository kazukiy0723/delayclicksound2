import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# csvファイルの読み込み
df = pd.read_csv('csv_file_path')

# 初期化
mean_stdevs = []
mean_rmse = []

# 開始列を1から31に変化させ、10列のデータを選択
for i in range(5, 31):
    stdevs = []
    rmse = []
    # Loop over all rows
    for index, row in df.iterrows():
        data = row[i:i+10]
        stdev = data.std()
        stdevs.append(stdev)
        rmse.append(np.sqrt(((data - 500) ** 2).mean()))
    mean_stdevs.append(np.mean(stdevs))
    mean_rmse.append(np.mean(rmse))

# Plot mean standard deviations
plt.figure(figsize=(10, 5))
plt.plot(range(1, 31), mean_stdevs)
plt.xlabel("First index of data")
plt.ylabel("STDEV")
plt.show()

# Plot mean RMSE
plt.figure(figsize=(10, 5))
plt.plot(range(1, 31), mean_rmse)
plt.xlabel("First index of data")
plt.ylabel("RMSE")
plt.show()
