import numpy as np

from DecisionStump import DecisionStump
from LogisticRegression import LogisticRegression


class AdaBoost(object):
    """
    Adaptive Boosting
    """

    def __init__(self, clf_type: int = 1, clf_num: int = 100):
        """
        :param clf_type: 基分类器的类别
        :param clf_num: 需要训练的基分类器的个数
        """
        # 基分类器个数
        self._clf_num = clf_num
        # 权重分布
        self._weights = None
        # 训练好的分类器
        self._classifiers = []
        # 对应分类器的权重
        self._alphas = []
        # 基分类器类型
        if clf_type == 1:
            self._base_clf = LogisticRegression
        elif clf_type == 2:
            self._base_clf = DecisionStump
        else:
            print('输入错误！')
            exit(-1)

    def fit(self, X, y):
        """
        模型训练
        :param X: 训练数据集
        :param y: 训练数据标签
        :return:
        """
        # 初始化权重
        self._weights = np.ones(X.shape[0]) / X.shape[0]

        for i in range(self._clf_num):
            # 使用当前权重训练一个基分类器
            classifier = self._base_clf()
            classifier.fit(X, y, self._weights)

            # 计算当前分类器的错误率和权重
            y_predict = classifier.predict(X)
            error_rate = np.sum(self._weights[y_predict != y])

            if error_rate > 0.5:
                break
            #   classifier.reverse()

            # 计算当前分类器的权重
            alpha = np.log((1 - error_rate) / error_rate) / 2

            # 更新样本权重
            for _i in range(X.shape[0]):
                if y_predict[_i] != y[_i]:
                    self._weights[_i] /= 2 * error_rate
                else:
                    self._weights[_i] /= 2 * (1 - error_rate)

            # self._weights *= np.exp(-alpha * y * y_predict)
            # self._weights /= np.sum(self.weights)

            # 保存分类器和权重
            self._classifiers.append(classifier)
            self._alphas.append(alpha)

    def predict(self, X: np.ndarray):
        """
        预测数据
        :param X: 测试数据集
        :return: 预测数据标签
        """
        predict = np.zeros(X.shape[0])
        for estimator, alpha in zip(self._classifiers, self._alphas):
            predict += alpha * estimator.predict(X)
        predict = np.sign(predict)
        return predict
