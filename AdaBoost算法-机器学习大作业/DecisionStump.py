import numpy as np


class DecisionStump:
    """
    决策树桩基分类器
    """
    def __init__(self):
        # 选定的最优特征的下标
        self.j = None
        # 特征划分的阈值
        self.theta = None
        # 特征取值小于等于阈值时的分类标签
        self.s = None
        # 是否翻转
        self.is_reverse = 1

    def fit(self, X: np.ndarray, y: np.ndarray, x_weights: np.ndarray):
        """
        训练分类器，计算在当前样本权重下的最优特征、阈值和分类标签
        :param X: 训练数据集
        :param y: 训练数据集标签
        :param x_weights: 训练集权重
        :return: 分类器模型
        """
        # 最小的错误率
        min_error = np.inf

        # 遍历所有特征
        for j in range(X.shape[1]):
            theta_values = np.unique(X[:, j])
            for theta in theta_values.tolist():
                # 分别选取大于阈值和小于阈值的标记为-1，计算误差
                for s in (-1, 1):
                    y_pred = np.ones(X.shape[0])
                    # 分类标准是特征取值是否小于等于阈值
                    y_pred[X[:, j] * s <= theta * s] = -1
                    error = np.sum(x_weights[y_pred != y])
                    # 记录最小错误率
                    if error < min_error:
                        min_error = error
                        # 更新相应特征数据
                        self.j = j
                        self.theta = theta
                        self.s = s

    def predict(self, x: np.ndarray):
        """
        预测模型
        :param x: 测试集
        :return: 预测值
        """
        y_pred = np.ones(x.shape[0])
        y_pred[x[:, self.j] * self.s <= self.theta * self.s] = -1
        return y_pred * self.is_reverse

    def reverse(self):
        self.is_reverse = -self.is_reverse
