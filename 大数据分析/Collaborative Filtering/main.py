import random

from recommend.CFUtil import CFUtil
from recommend.FileFormat import FileFormat

global N


def init():
    # 选择模式
    while True:
        try:
            mode = int(input('请输入要执行的模式:\n1.UserToUser 2.ItemToItem -1.exit\n'))
        except ValueError:
            print('输入不合法！')
            continue
        else:
            if mode == -1:
                exit(0)
            elif mode in (1, 2):
                is_hash = input('是否使用minHash？[y/n]:')
                if is_hash == 'y':
                    CFUtil.init(mode, True)
                elif is_hash == 'n':
                    CFUtil.init(mode, False)
                break
            else:
                print('输入不合法！')
                continue


def test():
    sse = 0
    test_set = FileFormat.read_test()
    for t_user, movie_rating in test_set.items():
        print('开始计算：' + str(t_user) + '...')
        _rec = CFUtil.recommend(user_movie, movie_genre, users.index(t_user))
        _rec_movie = [movie for movie, rating in _rec]
        _rec_rating = [rating for movie, rating in _rec]
        for movie, rating in movie_rating:
            sse += (_rec_rating[_rec_movie.index(movie)] - rating) ** 2
    return sse


if __name__ == "__main__":
    init()
    N = 10
    users, movies, user_movie, movie_genre = FileFormat.read("train_set.csv", "movies.csv")
    target = int(input('请输入要推荐的用户Id，-1代表进行测试：'))
    if target == -1:
        print(test())
    else:
        # random.seed(2233)
        rec = CFUtil.recommend(user_movie, movie_genre, users.index(target))[:N]
        for r_movie, r_rating in rec:
            print("{:<8d}  {:.6f}".format(r_movie, r_rating))
