import numpy as np


class LogisticRegression:
    """
    对数几率回归弱分类器
    """
    def __init__(self, learning_rate: float = 0.1, ite_num: int = 1000):
        # 学习率，控制步长
        self.learning_rate = learning_rate
        # 迭代次数
        self.ite_num = ite_num
        # 权重
        self.weights = None
        # 偏置量
        self.bias = 0
        # 是否翻转
        self.is_reverse = 1

    @staticmethod
    def _sigmoid(z: np.ndarray):
        """
        对数几率函数
        :param z: 预测值
        :return: 概率
        """
        return 1 / (1 + np.exp(-z))

    def fit(self, X: np.ndarray, y: np.ndarray, x_weights: np.ndarray):
        """
        训练模型
        :param X: 训练数据集
        :param y: 训练数据集标签
        :param x_weights: 训练集权重
        :return:
        """
        self.weights = np.zeros(X.shape[1])

        # 梯度下降优化
        for i in range(self.ite_num):
            # 计算当前权重下的预测值
            z = np.dot(X, self.weights) + self.bias
            y_pred = self._sigmoid(z)

            # 计算权重梯度
            grad = np.dot((y_pred - y) * x_weights, X) / X.shape[0]
            db = np.dot(y_pred - y, x_weights) / X.shape[0]

            # 更新权重
            self.weights -= self.learning_rate * grad
            self.bias -= self.learning_rate * db

    def predict(self, X: np.ndarray):
        """
        预测模型
        :param X: 测试集
        :return: 预测值
        """
        return ((np.round(self._sigmoid(np.dot(X, self.weights) + self.bias)) == 0) * -2 + 1) * self.is_reverse

    def reverse(self):
        self.is_reverse = -self.is_reverse
