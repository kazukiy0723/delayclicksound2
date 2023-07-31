import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# CSVファイルの読み込み
filename = r"G:\マイドライブ\temp\RESULT\Result_unique_yamashita\yamashitake.csv"
df = pd.read_csv(filename, header=None)

# 3列目から最後の列までを選択し、各行の標準偏差を計算
std_devs = df.iloc[:, 6:].std(axis=1)


# x軸のラベルを設定
x_labels = ["yuri_200ms", "yuri_unique", "kazuki_200ms", "kazuki_unique", "yuri_now", "yuri_unique2", "seiji_90", "sieji_200"] 
x = np.arange(len(x_labels))
width  = 0.5


# 棒グラフで表示
if(len(x_labels) != len(std_devs)):
    print('x軸のラベルの数が正しくありありません。')
else:
    fig, ax = plt.subplots()
    rects = ax.bar(x_labels, std_devs)
    # 棒の上に値を表示
    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(round(height, 2)),
            xy=(rect.get_x() + rect.get_width() / 2, height),
            xytext=(0, 3),  # 3 points vertical offset
            textcoords="offset points",
            ha='center', va='bottom')
        
    plt.xlabel('BPM (bits per minute)')
    plt.ylabel('Standard Deviation')
    plt.ylim([0, std_devs.max() * 1.2])
    plt.show()

