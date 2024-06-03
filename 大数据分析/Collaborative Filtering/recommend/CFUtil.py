import math
import random

import numpy as np


class CFUtil:
    """
    协同过滤算法主要功能实现的工具类
    """
    __mode = 0
    __is_hash = False

    USER_TO_USER = 1
    ITEM_TO_ITEM = 2

    @staticmethod
    def init(mode: int = USER_TO_USER, is_hash: bool = False):
        """
        设置推荐模式，默认为基于用户推荐
        设置是否采用minHash进行数据降维，默认为不使用
        :param mode: CFUtil.USER_TO_USER or CFUtil.ITEM_TO_ITEM
        :param is_hash: True or False
        :return: None
        """
        CFUtil.__mode = mode
        CFUtil.__is_hash = is_hash

    @staticmethod
    def __min_hash(rows: list):
        """
        运用随机函数生成哈希签名
        :param rows: 需要进行哈希签名的矩阵（需要转化成列表）
        :return: 签名行
        """
        random.shuffle(rows)
        sign_row = []
        # 按列遍历
        for column in range(len(rows[0])):
            for row in range(len(rows)):
                # 记录第一个不为0的行序号作为签名
                if rows[row][column] == 1:
                    sign_row.append(row)
                    break

        return sign_row

    @staticmethod
    def signature(matrix: np.ndarray, sign_num: int):
        """
        生成签名矩阵，对数据降维
        :param matrix: 要降维压缩的矩阵
        :param sign_num: 生成签名的数量
        :return: 签名矩阵
        """
        re_mat = None
        # 01化处理
        if CFUtil.__mode == 1:
            re_mat = list((matrix.T > 2.5) * 1.0)
        elif CFUtil.__mode == 2:
            re_mat = list((matrix.T > 0) * 1.0)
        # 签名矩阵
        sign_mat = []
        for i in range(sign_num):
            sign_mat.append(CFUtil.__min_hash(re_mat))

        return np.asarray(sign_mat).T

    @staticmethod
    def similarity(matrix: np.ndarray, target: tuple[int, np.ndarray]):
        """
        计算不同项之间的相似度，使用minHash后均计算Jaccard相似度，不使用则分别计算Pearson相似度和Cosine相似度
        :param matrix: 用户数据矩阵
        :param target: 传入被计算相似度的用户
        :return: 关系字典
        """
        order = target[0]
        target = target[1]
        relate_dict: dict[int | frozenset, float] = dict()
        if not CFUtil.__is_hash:
            # 计算Pearson相似度
            if CFUtil.__mode == CFUtil.USER_TO_USER:
                for index, user in enumerate(matrix):
                    # np.corrcoef返回的是2X2的对角矩阵，取右上角的值即为Pearson相似度
                    relate_dict[index] = np.corrcoef(user, target)[0][1]
                relate_dict[order] = 0
            # 计算Cosine相似度
            elif CFUtil.__mode == CFUtil.ITEM_TO_ITEM:
                # 已评分的电影
                rated = list(np.nonzero(target)[0])
                # 未评分的电影
                unrate = list(np.nonzero(target == 0)[0])
                # 计算未评分电影与所有已评分电影的相似度
                # for movie in range(len(target)):  # 测试时需要解除注解
                for movie in unrate:  # 测试时需要注解
                    for rated_movie in rated:
                        relate_dict[frozenset({movie, rated_movie})] = \
                            matrix[movie].dot(matrix[rated_movie]) \
                            / (np.linalg.norm(matrix[movie]) * np.linalg.norm(matrix[rated_movie]))
                relate_dict[frozenset({order, order})] = 0
        # 利用签名矩阵计算Jaccard相似度
        else:
            if CFUtil.__mode == CFUtil.USER_TO_USER:
                for index, user in enumerate(matrix):
                    # 两个集合的交集除以其并集得出Jaccard相似度
                    relate_dict[index] = len(set(user) & set(target)) / len(
                        set(user) | set(target))
                relate_dict[order] = 0
            elif CFUtil.__mode == CFUtil.ITEM_TO_ITEM:
                # 已评分的电影
                rated = list(np.nonzero(target)[0])
                # 未评分的电影
                unrate = list(np.nonzero(target == 0)[0])
                # 计算电影与所有已评分电影的相似度
                # for movie in range(len(target)):  # 测试时需要解除注解
                for movie in unrate:  # 测试时需要注解
                    for rated_movie in rated:
                        relate_dict[frozenset({movie, rated_movie})] = len(
                            set(matrix[movie]) & set(matrix[rated_movie])) / len(
                            set(matrix[movie]) | set(matrix[rated_movie]))
                relate_dict[frozenset({order, order})] = 0
        return relate_dict

    @staticmethod
    def recommend(user_movie: np.ndarray, movie_genre: np.ndarray, target: int):
        """
        对目标用户进行推荐
        :param user_movie: 用户-电影矩阵
        :param movie_genre: 电影-种类tf-idf特征矩阵
        :param target: 目标用户
        :return: 返回用户的电影推荐列表
        """
        # 获取要被推荐的用户
        predict_user = user_movie[target].copy()
        # 获取已评分的电影
        rated = np.nonzero(predict_user)[0]
        # 获取未评分的电影
        unrate = np.nonzero(predict_user == 0)[0]
        # 获取所有预测的评分项
        predict_rates: list[tuple] = []
        # 基于用户推荐
        if CFUtil.__mode == CFUtil.USER_TO_USER:
            if CFUtil.__is_hash:
                # 设置签名矩阵的大小，这里选用列数的算术平方根的两倍作为其大小
                sign_num = math.ceil(math.sqrt(user_movie.shape[1])) * 2
                # sign_num = 1024
                # 生成签名矩阵
                sign_mat = CFUtil.signature(user_movie, sign_num)
                # 计算其他用户与该用户的相似度
                sim = CFUtil.similarity(sign_mat, (target, predict_user))
            else:
                # 计算其他用户与该用户的相似度
                sim = CFUtil.similarity(user_movie, (target, predict_user))
            # 选取前K个最相近用户进行推荐
            k = math.ceil(math.sqrt(user_movie.shape[0]))
            sim = sorted(sim.items(), key=lambda x: x[1], reverse=True)[:k]
            # 组合出新的预测矩阵
            predict_mat = np.zeros((k, user_movie.shape[1]))
            order = 0
            for index, _ in sim:
                # 计算相似用户已评分的平均值
                ave = np.average(user_movie[index][np.nonzero(user_movie[index])])
                for movie, rate in enumerate(user_movie[index]):
                    # 为零则用平均值替代
                    if rate == 0:
                        predict_mat[order][movie] = ave
                    else:
                        predict_mat[order][movie] = rate
                order += 1

            # 根据公式predict_rate = sum(sim * rating) / sum(sim) 推算用户的评分
            for movie in range(len(predict_user)):
                predict_rate = 0
                sim_sum = 0
                # 这里不需要考虑相似度为0的情况
                for i, rating in enumerate(predict_mat[:, movie]):
                    simi = sim[i][1]
                    predict_rate += simi * rating
                    sim_sum += simi
                predict_rates.append((movie, predict_rate / sim_sum))
        # 基于内容推荐
        elif CFUtil.__mode == CFUtil.ITEM_TO_ITEM:
            if CFUtil.__is_hash:
                # 设置签名矩阵的大小，这里设置为12
                sign_num = 12
                # 生成签名矩阵
                sign_mat = CFUtil.signature(movie_genre, sign_num)
                # 计算电影间的相似度,注意这里传入的参数是ndarray ↓
                sim = CFUtil.similarity(sign_mat, (target, predict_user))
            else:
                # 计算电影间的相似度,注意这里传入的参数是ndarray ↓
                sim = CFUtil.similarity(movie_genre, (target, predict_user))
            # 计算评分的平均值
            ave = np.average(predict_user[np.nonzero(predict_user)])
            # 预测未评分电影的评分
            # for movie in range(len(predict_user)):  # 此代码在测试时需要解除注释
            for movie in unrate:  # 此代码在测试时需要注释
                predict_rate = 0
                sim_sum = 0
                for rated_movie in rated:
                    simi = sim[frozenset({movie, rated_movie})]
                    if simi > 0:
                        predict_rate += simi * predict_user[rated_movie]
                        sim_sum += simi
                # 如果该电影与所有已评分电影相似度均为0，则用平均值代替
                if predict_rate == 0:
                    predict_rates.append((movie, ave))
                else:
                    predict_rates.append((movie, predict_rate / sim_sum))
        # 排序获得推荐列表
        predict_rates.sort(key=lambda x: x[1], reverse=True)
        return predict_rates
