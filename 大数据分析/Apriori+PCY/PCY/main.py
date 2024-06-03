import time

from FileInput import FileInput
from FileOutput import FileOutput
from PCY.ClusterUtil import ClusterUtil


# 字典的值为列表，第一项为键所对应的数字，第二项为键出现的次数
def init_candidate():
    _c: list = []
    for items in data_base:
        for item in items:
            _c.append((item,))
    return _c


if __name__ == "__main__":
    cycle_num = 4
    print("开始执行……")
    is_save = input("是否将数据保存？[y/n]")
    if is_save == 'y':
        is_save = True
    elif is_save == 'n':
        is_save = False

    start_time = time.time()
    data_base = FileInput.data_init()
    start = time.time()
    C = init_candidate()
    L = ClusterUtil.count(C, len(data_base))
    bitmap, bucket_num = ClusterUtil.hash_with_bitmap(data_base, 2)
    end = time.time()
    print("步骤1执行完毕！")
    print(len(L))
    print("用时：{:.4f} s".format(
        end - start
    ))
    if is_save:
        FileOutput.result_output(1, L, len(data_base))

    for i in range(2, cycle_num + 1):
        start = time.time()
        C = ClusterUtil.construct(data_base, bitmap, i, bucket_num)
        L = ClusterUtil.count(C, len(data_base))
        bitmap, bucket_num = ClusterUtil.hash_with_bitmap(data_base, i + 1)

        end = time.time()
        print("步骤" + str(i) + "执行完毕！")
        print(len(L))
        print("用时：{:.4f} s".format(
            end - start
        ))
        if is_save:
            FileOutput.result_output(i, L, len(data_base))

    end_time = time.time()
    print("总计用时：{:.4f} s".format(
        end_time - start_time
    ))
