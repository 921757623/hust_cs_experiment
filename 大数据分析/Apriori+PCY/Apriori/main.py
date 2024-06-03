import time

from FileInput import FileInput
from Apriori.ClusterUtil import ClusterUtil


def init_candidate():
    """
    初始化候选集
    :return:
    """
    _c1 = dict()
    for items in data_source:
        for item in items:
            # 采用元组作为键
            _tmp = (item,)
            # 同时进行计数
            if _tmp not in _c1:
                _c1[_tmp] = 1
            else:
                _c1[_tmp] += 1
    return _c1


if __name__ == "__main__":
    cycle_num = 4
    print("开始执行……")
    start_time = time.time()
    data_source = FileInput.data_init()

    start = time.time()
    C = init_candidate()
    L = ClusterUtil.filter(C, dict(), len(data_source), 1)
    end = time.time()
    print("步骤1执行完毕！")
    print(len(L))
    print("用时：{:.4f} s".format(
        end - start
    ))

    pre_L = L
    for i in range(2, cycle_num + 1):
        start = time.time()
        C = ClusterUtil.construct(L, i - 1)
        e = time.time()
        print("construct用时：{:.4f} s".format(
            e - start
        ))
        ClusterUtil.count(C, data_source)
        print("count用时：{:.4f} s".format(
            time.time() - e
        ))

        # 频繁项集合并，便于计算置信度
        if i > 2:
            L.update(pre_L)
            pre_L = L
        e = time.time()

        L = ClusterUtil.filter(C, L, len(data_source), i)
        print("filter用时：{:.4f} s".format(
            time.time() - e
        ))
        end = time.time()
        print("步骤" + str(i) + "执行完毕！")
        print(len(L))
        print("用时：{:.4f} s".format(
            end - start
        ))

    end_time = time.time()
    print("总计用时：{:.4f} s".format(
        end_time - start_time
    ))
