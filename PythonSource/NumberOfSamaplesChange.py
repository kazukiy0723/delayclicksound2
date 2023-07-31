'''サンプル数による評価指標の値の変化を可視化する'''

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

###################
## 設定
filepath = ""
start_index = 6
true_values = 60 / 90 * 1000 # rmseの真値
###################

# csvファイルの読み込み
df = pd.read_csv('filepath')

# 初期化
mean_stdevs = []
mean_rmse = []



# 
for i in range(10, 36):
    stdevs = []
    rmse = []
   
    for index, row in df.iterrows():
        data = row[start_index:start_index+i]
        stdev = data.std()
        stdevs.append(stdev)
        rmse.append(np.sqrt(((data - true_values) ** 2).mean()))
    mean_stdevs.append(np.mean(stdevs))
    mean_rmse.append(np.mean(rmse))

# 
fig, ax = plt.subplots(1, 2, figsize=(15, 5))

# 
ax[0].plot(range(10, 36), mean_stdevs)
ax[0].set_xlabel("Number of Samples")
ax[0].set_ylabel("Evaluation index")
ax[0].set_title("STDEV")

# 
ax[1].plot(range(10, 36), mean_rmse)
ax[1].set_xlabel("Number of Samples")
ax[1].set_ylabel("Evaluation index")
ax[1].set_title("RMSE")

# 
plt.tight_layout()
plt.show()
