import time

import numpy as np
from sklearn.utils import shuffle
from sklearn.model_selection import KFold
from sklearn.preprocessing import scale

from AdaBoost import AdaBoost


def read_file():
    """
    读取训练数据集和标签
    :return: 训练数据集和标签
    """
    _X = np.genfromtxt('data.csv', delimiter=',', dtype=np.float64)
    _Y = np.genfromtxt('targets.csv', delimiter=',', dtype=np.float64)
    _X, _Y = shuffle(_X, _Y)
    _X = scale(_X)
    # 将标签转换为-1和1
    _Y = (_Y == 0) * -2 + 1
    return _X, _Y


def write(res: np.ndarray, index: int):
    """
    将预测标签写入文件
    :param res: 预测结果
    :param index: 对应十折序号
    :return:
    """
    base = fold_len * (index - 1)
    try:
        f = open('experiments/base%d_fold%d.csv' % (base_num, index), mode='w')
    except OSError as e:
        print(e)
    else:
        for _i, tabel in enumerate(res, start=1):
            f.write(str(_i + base) + ',' + str(tabel) + '\n')
        f.close()


if __name__ == '__main__':
    # 读取文件
    X, y = read_file()
    fold_len = X.shape[0] // 10
    # 十折交叉验证
    kf = KFold(n_splits=10)
    # 基分类器数量
    base_list = [1, 5, 10, 100]
    # 存放效果最好的基分类器
    best_clfs = []
    clf_type = int(input('请输入要使用的基分类器：1.LR 2.DS'))

    for base_num in base_list:
        _index = 1
        error_rate = float('inf')
        best_clf = None
        print('当前训练基分类器数目：' + str(base_num))
        # 十折交叉验证
        for train_index, test_index in kf.split(X):
            # 获得训练集和训练集
            X_train, X_test = X[train_index], X[test_index]
            y_train, y_test = y[train_index], y[test_index]

            start = time.time()
            adaboost = AdaBoost(clf_type, clf_num=base_num)
            adaboost.fit(X_train, y_train)
            end = time.time()
            print('第%02d折用时：%6f S' % (_index, end - start))

            # 得到预测标签
            target = adaboost.predict(X_test)
            # 计算错误率
            er = np.sum(target != y_test) / target.size
            print('错误率：%6f' % er)
            # 存储效果最好的分类器
            if er < error_rate:
                error_rate = er
                best_clf = adaboost
            # 写入文件
            write(target, _index)
            _index += 1
        # 添加分类器
        best_clfs.append(best_clf)

    # 对预测结果进行校验
    # -------------------------------------------------------------------------------
    # 因为最初在数据初始化处理是对数据进行了shuffle打乱，因为不能通过读取target.csv文件直接进行校验
    # 这里直接使用shuffle后的y标签进行校验，这与读取target.csv文件是等效的
    # -------------------------------------------------------------------------------
    target = y

    for base_num in base_list:
        acc = []
        for i in range(1, 11):
            fold = np.genfromtxt('experiments/base%d_fold%d.csv' % (base_num, i), delimiter=',', dtype=np.int32)
            accuracy = sum(target[fold[:, 0] - 1] == fold[:, 1]) / fold.shape[0]
            acc.append(accuracy)
        print('基分类器数量：{:4}'.format(base_num) + ' 准确率：', end='')
        print(np.array(acc).mean())

    # 测试环节
    while True:
        X_name = input('请输入测试数据集文件名')
        y_name = input('请输入测试标签文件名')
        try:
            t_X = np.genfromtxt(X_name, delimiter=',', dtype=np.float64)
            t_y = np.genfromtxt(y_name, delimiter=',', dtype=np.float64)
            # 标准化数据
            t_X = scale(t_X)
            # 将标签转换为-1和1
            t_y = (t_y == 0) * -2 + 1
        except OSError as error:
            print(error)
            continue
        else:
            while True:
                num = int(input('请选择要测试的基分类器数量的AdaBoost（1、5、10、100）, -1退出'))
                if num == -1:
                    exit(0)
                try:
                    test_clf = best_clfs[base_list.index(num)]
                except ValueError as error:
                    print('数据输入错误！')
                    continue
                y_pred = test_clf.predict(t_X)
                print('准确率为：' + str(np.sum(y_pred == t_y) / y_pred.size))
