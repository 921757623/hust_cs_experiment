import time

from com.wordcount.Map.InputFormat import InputFormat
from com.wordcount.Shuffle.Shuffle import Shuffle
from com.wordcount.Source import Source


class Mapper:
    """
        生成k2, v2键值对
    """
    @staticmethod
    def map(task_order: int):
        """
        生成<k2, v2>键值对，同时对数据进行shuffle，随后分区到不同的Reducer
        :param task_order:
        :return: 各个函数运行时间统计
        """
        time_record = []

        start = time.time()
        # 读取文件，同时得到<k1, v1>
        lines = InputFormat.read_file(
            Source.base_file_path + 'source0' + str(task_order)
        )
        end = time.time()
        time_record.append(end - start)

        word_list = dict()
        start = time.time()
        # 对每个v1进行单词拆分，并统计单词出现的次数，即combine过程
        for line in lines.values():
            for word in line.split(', '):
                if word in word_list:
                    word_list[word] += 1
                else:
                    word_list.setdefault(word, 1)
        end = time.time()
        time_record.append(end - start)

        start = time.time()
        # 进行Shuffle阶段，每个线程对应一个Shuffle类实例
        shuffler = Shuffle()
        shuffler.shuffle(word_list)
        end = time.time()
        time_record.append(end - start)
        # 保存分区后文件
        start = time.time()
        shuffler.save(task_order)
        end = time.time()
        time_record.append(end - start)

        return time_record




