import numpy
import numpy as np
import pandas as pd
from pandas import DataFrame

from kmeans.KMeansUtil import KMeansUtil

BASE_FILE_PATH = "D:\\bigData\\kmeans\\kmeans\\"


def read_file(file_name: str):
    try:
        data_frame = pd.read_csv(
            filepath_or_buffer=BASE_FILE_PATH + file_name,
            delimiter=',',
            header=None
        )
    except OSError:
        print("文件打开失败！")
        exit(-1)
    else:
        return data_frame


def calc_accuracy(result: list):
    test_set = [[], [], []]
    for point in a.values:
        test_set[int(point[0]) - 1].append(point[1:])
    order = [[0, 1, 2], [0, 2, 1], [1, 0, 2], [1, 2, 0], [2, 0, 1], [2, 1, 0]]
    accuracy = [0, 0, 0, 0, 0, 0]
    for index, current_order in enumerate(order):
        have = 0
        for point in result[current_order[0]]:
            for test_point in test_set[0]:
                if np.allclose(point, test_point):
                    have += 1
        for point in result[current_order[1]]:
            for test_point in test_set[1]:
                if np.allclose(point, test_point):
                    have += 1
        for point in result[current_order[2]]:
            for test_point in test_set[2]:
                if np.allclose(point, test_point):
                    have += 1
        accuracy[index] = have / a.shape[0]
    return max(accuracy)


if __name__ == "__main__":
    origin = [[], [], []]
    a: DataFrame = read_file("归一化数据.csv")
    for i in a.values:
        origin[int(i[0]) - 1].append(i)
    data = a.iloc[:, 1:]
    points = KMeansUtil.select_points(data, 3)
    res1, res2 = KMeansUtil.KMeans(data, points, 3)
    for i in range(3):
        print(sum(numpy.linalg.norm(point - res1[i]) ** 2 for point in res2[i]))
    print(calc_accuracy(res2))
    KMeansUtil.draw(res2)
    print("")
    # print(a.iloc[0, :].values.tolist())
