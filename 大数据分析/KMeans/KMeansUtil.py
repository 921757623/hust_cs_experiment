import random
import matplotlib.pyplot as plt
import numpy
import numpy as np
from pandas import DataFrame


class KMeansUtil:
    @staticmethod
    def select_points(data: DataFrame, k: int):
        """
        选出最初的K个中心点
        :param data: 点集
        :param k: 中心点个数
        :return: 中心点集
        """
        centroid = list()
        points = []
        for i in data.iterrows():
            points.append(numpy.asmatrix(numpy.asarray(i[1])))
        # 获取第一个点
        centroid.append(points[random.randint(0, len(points) - 1)])

        # 循环找出后面的点
        for i in range(1, k):
            points_having = points[:i]
            maxx = -1
            candidate = None
            for point in points:
                dis = min(numpy.linalg.norm(point - center) for center in points_having)
                if dis > maxx:
                    maxx = dis
                    candidate = point
            centroid.append(candidate)
        return centroid

    @staticmethod
    def KMeans(data: DataFrame, centroid: list, k: int, max_iter: int = 100):
        points = []
        for i in data.iterrows():
            points.append(numpy.asmatrix(numpy.asarray(i[1])))
        current_iter = 0
        while True:
            clusters = []
            for i in range(k):
                clusters.append([])
            current_iter += 1
            if current_iter > max_iter:
                return None, None
            # 聚类所有的点
            for point in points:
                minn = min(((numpy.linalg.norm(point - center), index) for index, center in enumerate(centroid))
                           , key=lambda x: x[0])
                clusters[minn[1]].append(point)
            # 重新计算中心点
            new_centroid = []
            for cluster in clusters:
                new_center = np.mean(np.asmatrix(np.asarray(cluster)), axis=0)
                new_centroid.append(np.asmatrix(new_center))

            if KMeansUtil.__is_equal(new_centroid, centroid):
                print(current_iter)
                return centroid, clusters
            else:
                centroid = new_centroid

    @staticmethod
    def __is_equal(m1: list[numpy.matrix], m2: list[numpy.matrix]):
        for i in range(len(m1)):
            if not np.allclose(m1[i], m2[i]):
                return False
        return True

    @staticmethod
    def draw(data: list):
        color = ['red', 'green', 'blue']
        figure = plt.figure()
        ax = figure.add_subplot(111)
        for index, cluster in enumerate(data):
            for point in cluster:
                tmp = point.tolist()[0]
                ax.scatter(tmp[11], tmp[12], marker='.', color=color[index], s=32)
        plt.show()
