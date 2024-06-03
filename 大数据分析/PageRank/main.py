import csv
import time

import numpy as np


def create_matrix():
    mat_array = list()
    for ix, x in enumerate(vertex):
        mat_array.append(list())
        for iy, y in enumerate(vertex):
            if x in edges[y]:
                mat_array[ix].append(1 / len(edges[y]))
            # DeadEnds 在这里就对矩阵做出修正
            elif not edges[y]:
                mat_array[ix].append(1 / len(vertex))
            else:
                mat_array[ix].append(0.0)

    matrix = np.asmatrix(np.array(mat_array))
    return matrix


def read_file():
    """
    读取数据文件
    :return:
    """
    base_file_path = "D:\\bigData\\pagerank\\"
    try:
        with open(base_file_path + 'sent_receive.csv') as f:
            reader = csv.reader(f)
            # 去除表头
            next(reader)
            # row[1] => row[2]
            for _, send, receive in reader:
                # 字典中初始化边关系
                if send not in edges:
                    edges.setdefault(send, [])
                if receive not in edges:
                    edges.setdefault(receive, [])
                # 添加边关系
                if receive not in edges[send]:
                    edges[send].append(receive)
                # 统计所有的顶点
                if send not in vertex:
                    vertex.append(send)
                if receive not in vertex:
                    vertex.append(receive)
    except OSError:
        print("文件打开失败！")
    finally:
        if f is not None:
            f.close()


def pagerank(r: np.matrix):
    """
    循环执行算法，直至收敛
    :param r: 要执行的矩阵
    :return:
    """
    e = r.copy()
    while True:
        # SpiderTraps 加入修正项
        r_current = d * np.dot(M, r) + (1 - d) * e
        if np.linalg.norm((r_current - r)) < 10e-8:
            break
        r = r_current
    return r_current


if __name__ == '__main__':
    d = 0.7
    edges = dict[str: list[str]]()
    vertex = []
    start_time = time.time()

    read_file()
    M = create_matrix()

    r_array = []

    for i in range(len(vertex)):
        r_array.append(1 / len(vertex))

    r_primary = np.matrix(np.array(r_array)).T

    r_final: np.matrix = pagerank(r_primary)
    end_time = time.time()

    result = dict[str: int]()
    for index, i in enumerate(r_final.tolist()):
        result.setdefault(vertex[index], i[0])
    result = sorted(result.items(), key=lambda x: x[1], reverse=True)

    total = 0
    for i, item in enumerate(result, start=1):
        if i % 6 == 0:
            print("{:<5}  {:<6.4f}".format(item[0], item[1]))
        else:
            print('{:<5}  {:<6.4f}'.format(item[0], item[1]), end=' | ')
        # 统计所有的rank值
        total += item[1]

    print(total)
    print("用时：{:.4f} s".format(
        end_time - start_time
    ))
