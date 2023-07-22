import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# CSVファイルの読み込み
filename = '.\\Result_SoundAndLight\\result_yamashita_bpm90.csv'  # 適切なファイル名に置き換えてください
df = pd.read_csv(filename, header=None)

# 3列目から最後から3列目までを選択し、各行の標準偏差を計算
df_selected = df.iloc[:, 1:-1]

# 指定した真の値
true_values = 60 / 90 * 1000  # 適切な真の値に置き換えてください

# RMSEの計算
rmse_values = (df_selected.subtract(true_values, axis='columns')**2).mean(axis=1).apply(np.sqrt)

# 標準偏差の計算
std_values = df_selected.std(axis=1)

# x軸のラベルを設定
x_labels = ['10', '30', '50', '70', '90', '110', '150', '200']
# x_labels = ['90', '200', '70', '10', '30', '150', '50', '110']

x = np.arange(len(x_labels))
width  = 0.5 / 2  # 幅を2で割ることで、2つの棒が重ならないようにします

# 棒グラフで表示
if(len(x_labels) != len(rmse_values)):
    print('x軸のラベルの数が正しくありありません。')
else:
    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width/2, rmse_values, width, label='RMSE')
    rects2 = ax.bar(x + width/2, std_values, width, label='Std Dev')

    # # 棒の上に値を表示
    # for rects in [rects1, rects2]:
    #     for rect in rects:
    #         height = rect.get_height()
    #         ax.annotate('{}'.format(round(height, 2)),
    #                     xy=(rect.get_x() + rect.get_width() / 2, height),
    #                     xytext=(0, 3),  # 3 points vertical offset
    #                     textcoords="offset points",
    #                     ha='center', va='bottom', fontsize=14)
        
    
     # x軸のラベルを設定
    ax.set_xticks(x)
    ax.set_xticklabels(x_labels)
    
    plt.xlabel('Length of delay [ms]', fontsize=22)
    plt.ylabel('Values [ms]', fontsize=22)
    plt.title('BPM90 Yamashita Sound and Light')
    plt.ylim([0, max(rmse_values.max(), std_values.max()) * 1.2])
    plt.legend()
    plt.show()

# import pandas as pd
# import numpy as np
# import matplotlib.pyplot as plt

# # CSVファイルの読み込み
# filename = '.\\Result_onlySound_delay_yamashita\\summary.csv'#.\\result_0703_200ms.csv'  # 適切なファイル名に置き換えてください
# df = pd.read_csv(filename, header=None)

# # 3列目から最後から3列目までを選択し、各行の標準偏差を計算
# df_selected = df.iloc[:, 1:-1]


# # 指定した真の値
# true_values = 60 / 90 * 1000 # 適切な真の値に置き換えてください

# # RMSEの計算
# rmse_values = (df_selected.subtract(true_values, axis='columns')**2).mean(axis=1).apply(np.sqrt)

# # x軸のラベルを設定
# x_labels = ['10', '30', '50', '70', '90', '110', '150', '200']
             
# x = np.arange(len(x_labels))
# width  = 0.5

# # 棒グラフで表示
# if(len(x_labels) != len(rmse_values)):
#     print('x軸のラベルの数が正しくありありません。')
# else:
#     fig, ax = plt.subplots()
#     rects = ax.bar(x_labels, rmse_values)
#     # 棒の上に値を表示
#     for rect in rects:
#         height = rect.get_height()
#         ax.annotate('{}'.format(round(height, 2)),
#             xy=(rect.get_x() + rect.get_width() / 2, height),
#             xytext=(0, 3),  # 3 points vertical offset
#             textcoords="offset points",
#             ha='center', va='bottom', fontsize=14)
        
#     plt.xlabel('Length of delay [ms]', fontsize=14)
#     plt.ylabel('RMSE [ms]', fontsize=14)
#     plt.title('BPM90 Yamashita')
#     plt.ylim([0, rmse_values.max() * 1.2])
#     plt.show()
