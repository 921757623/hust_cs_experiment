import matplotlib
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np


# 折线图
def drawHistory1(f):
    sheet = pd.read_excel(f, sheet_name='data_history')
    # 去除掉年份，缩减空间
    date = [origin_date[5:] for origin_date in sheet.iloc[:, 0]]

    plt.title('2020年新冠疫情数据', fontproperties=myfont)
    plt.xlabel('日期', fontproperties=myfont)
    plt.ylabel('人数', fontproperties=myfont)
    plt.xticks(rotation=-60)

    for i in range(1, sheet.shape[1]):
        plt.plot(date, sheet.iloc[:, i], label=sheet.columns[i], scalex=True)
    plt.legend()
    plt.show()


# 散点图
def drawHistory2(f):
    sheet = pd.read_excel(f, sheet_name='data_history')
    # 去除掉年份，缩减空间
    date = [origin_date[5:] for origin_date in sheet.iloc[:, 0]]
    markers = ['', '^', '*', 's', 'o']

    plt.title('2020年新冠疫情数据', fontproperties=myfont)
    plt.xlabel('日期', fontproperties=myfont)
    plt.ylabel('人数', fontproperties=myfont)
    plt.xticks(rotation=-60)

    for i in range(1, sheet.shape[1]):
        plt.scatter(date, sheet.iloc[:, i], label=sheet.columns[i], marker=markers[i])
    plt.legend()
    plt.show()


# 饼图
def drawWorld(f):
    # 设置中文字体
    plt.rcParams['font.sans-serif'] = 'STKAITI'
    plt.rcParams['axes.unicode_minus'] = False


    sheet = pd.read_excel(f, sheet_name='data_world')
    # 2X2的子图
    fig, axs = plt.subplots(2, 2)

    fig.suptitle('确诊人数前四的国家新冠疫情状况饼图')

    sheet_sorted = sheet.sort_values('confirm', ascending=False)
    colors = ['yellowgreen', 'gold', 'lightskyblue', 'lightcoral']

    for i in range(1, 5):
        part = axs[int(i / 2) - 1][i % 2 - 1]
        part.set_title(sheet_sorted.iloc[i - 1, 0])
        part.pie(
            sheet_sorted.iloc[i - 1, 1:],
            colors=colors
        )
        part.legend(sheet_sorted.columns[1:], loc='upper left', fontsize='small')
    plt.tight_layout()
    plt.show()


# 直方图
def drawProv1(f):
    # 设置中文字体
    plt.rcParams['font.sans-serif'] = 'STKAITI'
    plt.rcParams['axes.unicode_minus'] = False

    sheet = pd.read_excel(f, sheet_name='current_prov')
    # 2X2的子图
    fig, axs = plt.subplots(2, 2)

    for i in range(1, 5):
        part = axs[int(i / 2) - 1][i % 2 - 1]
        part.set_title(sheet.columns[i])
        index = np.arange(sheet.shape[0])
        data = []
        for index_i, j in enumerate(sheet.iloc[:, i]):
            data = data + [index[index_i]] * j
        part.hist(data, index, alpha=0.5, density=False)

        part.set_xticks(index, sheet.iloc[:, 0], rotation=90)
        part.set_xlabel('省份')
        part.set_ylabel('人数')
    plt.tight_layout()
    plt.show()


# 条形图
def drawProv2(f):
    # 设置中文字体
    plt.rcParams['font.sans-serif'] = 'STKAITI'
    plt.rcParams['axes.unicode_minus'] = False

    sheet = pd.read_excel(f, sheet_name='current_prov')
    colors = ['', 'green', 'red', 'yellow']
    index = np.arange(sheet.shape[0])
    bar_width = 0.35
    for i in range(1, 4):
        plt.bar(index + (i - 1) * bar_width, sheet.iloc[:, i], bar_width,
                    color=colors[i], alpha=0.5, label=sheet.columns[i])
    plt.xticks(index + bar_width, sheet.iloc[:, 0])
    plt.xlabel('省份', fontproperties=myfont)
    plt.ylabel('人数', fontproperties=myfont)
    plt.title('各省新冠疫情状况条形图')
    plt.legend()
    plt.show()


if __name__ == "__main__":
    file = pd.ExcelFile('covid19_data.xls')
    # 设置字体为华文楷体
    myfont = matplotlib.font_manager.FontProperties(fname=r'C:/Windows/Fonts/STKAITI.TTF')

    drawHistory1(file)
    drawHistory2(file)
    drawWorld(file)
    drawProv1(file)
    drawProv2(file)
