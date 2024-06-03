import math

import numpy as np
import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer


class FileFormat:
    """
    用于读取和格式化数据
    """
    __BASE_FILE_PATH = "./datasets/"

    @staticmethod
    def read(file1: str, file2: str):
        """
        读取并格式化文件
        :param file1: 要读取的user-movie文件
        :param file2: 要读取的movie-genre文件
        :return: 用户集、电影集、用户-电影矩阵、电影-类别矩阵
        """
        try:
            data_frame1 = pd.read_csv(FileFormat.__BASE_FILE_PATH + file1, sep=',')
            data_frame2 = pd.read_csv(FileFormat.__BASE_FILE_PATH + file2, sep=',')
        except OSError as e:
            print(e)
            exit(-1)
        else:
            # 获取所有的用户Id
            users = list(data_frame1.iloc[:, 0].unique())
            # 获取所有的电影Id
            movies = list(data_frame1.iloc[:, 1].unique())
            # 创建用户-电影矩阵，大小为用户数×电影数，本实验中为671×9066
            user_movie = data_frame1.pivot_table(index='userId', columns='movieId', values='rating').values
            # 将矩阵中的NAN替换成0
            user_movie = np.apply_along_axis(lambda rates: [(0.0 if math.isnan(rate) else rate) for rate in rates]
                                             , axis=1, arr=user_movie)
            # 获取所有电影的Id，将train_set中不存在的电影添加到movies中，保证列序不变
            for movie in list(data_frame2.iloc[:, 0]):
                if movie not in movies:
                    movies.append(movie)
            # 创建电影-类别tf-idf矩阵，大小为电影数×类别数，本实验中为9125×24
            vectorizer = TfidfVectorizer()
            genres = list(data_frame2.iloc[:, 2])
            vectorizer.fit(genres)
            movie_genre = vectorizer.transform(genres).toarray()

            return users, movies, user_movie, movie_genre

    @staticmethod
    def read_test():
        try:
            data_frame = pd.read_csv(FileFormat.__BASE_FILE_PATH + 'test_set.csv', sep=',')
        except OSError as e:
            print(e)
            exit(-1)
        else:
            test_set = dict()
            for row in data_frame.values:
                user = int(row[0])
                movie = int(row[1])
                rating = row[2]
                if user not in test_set:
                    test_set[user] = []
                test_set[user].append((movie, rating))

            return test_set

