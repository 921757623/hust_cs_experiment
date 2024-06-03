import os
import time
from multiprocessing import Pool

from com.wordcount.Map.Mapper import Mapper
from com.wordcount.Reduce.Reducer import Reducer
from com.wordcount.Source import Source


def callback(data):
    """
    统计最大用时，供分析
    :param data:
    :return:
    """
    for index in range(4):
        ave[index] = max(ave[index], data[index])


result = {}
div = []


def callback2(data):
    """
    统计最大用时，以及对每个reducer返回的数据进行合并
    :param data:
    :return:
    """
    # 运行时长
    ave[0] = max(ave[0], data[0])
    # 分区内单词总数
    div.append(data[1])
    # 分区统计字典合并
    result.update(data[2])


if __name__ == '__main__':
    # 进程池创建9个进程
    process_pool = Pool(Source.map_task_num)

    print("开始执行Map...")
    start_time = time.time()
    ave = [0.0, 0.0, 0.0, 0.0]
    # 异步执行map
    for i in range(1, Source.map_task_num + 1):
        process_pool.apply_async(Mapper.map, [i], callback=callback)

    process_pool.close()
    process_pool.join()

    print("Map执行完毕！")
    # 统计平均时间
    print("生成<k1, v1>最大用时：{:.4f} s".format(
        ave[0]
    ))
    print("生成<k2, v2>最大用时：{:.4f} s".format(
        ave[1]
    ))
    print("Shuffle阶段最大用时：{:.4f} s".format(
        ave[2]
    ))
    print("Shuffle结果存储最大用时：{:.4f} s".format(
        ave[3]
    ))

    process_pool = Pool(Source.reduce_task_num)

    ave[0] = 0.0
    print("开始执行Reduce...")
    for i in range(Source.reduce_task_num):
        process_pool.apply_async(Reducer().reduce, (i,), callback=callback2)

    process_pool.close()
    process_pool.join()

    print("Reduce执行完毕！")
    print("生成<k3, v3>最大用时：{:.4f} s".format(
        ave[0]
    ))
    print(div)

    # 保存合并后的最终结果文件
    f = open(Source.base_file_path + 'result', mode='w')
    new_list = sorted(result.items(), key=lambda x: x[1], reverse=True)
    total = 0
    for element in new_list:
        f.write(element[0] + ' ' + str(element[1]) + '\n')
        total += element[1]
    f.close()
    print(total)
    end_time = time.time()
    print(f'总计用时：{end_time - start_time} S')

    # 删除中间文件
    for i in range(1, Source.map_task_num + 1):
        for j in range(Source.reduce_task_num):
            os.remove(Source.base_file_path + str(i) + '_' + str(j))
