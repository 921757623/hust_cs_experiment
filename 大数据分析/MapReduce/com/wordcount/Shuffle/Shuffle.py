from com.wordcount.Source import Source


class Shuffle:
    """
        单词宏观统计出现频次排序：s p c a m t b u d r h e n f o g i l w v k j q z y x（由大到小）
    """

    # 分区依据，单词频次排序中首尾字母相邻，两两组合
    __frequency = ['s', 'x', 't', 'k', 'b', 'v', 'e', 'o',
                   'p', 'y', 'm', 'j', 'u', 'w', 'h', 'g',
                   'c', 'z', 'a', 'q', 'd', 'l', 'r', 'i',
                   'n', 'f']

    def __init__(self):
        self.__partition = list()
        self.__part = dict()
        self.data_init()

    def data_init(self):
        """
        初始化分区依据以及存储数据结构
        :return:
        """
        for i in range(Source.reduce_task_num):
            self.__partition.append(dict())
        gap = int(26 / Source.reduce_task_num)
        # 依据Reducer数量对分区依据进行分割
        for i in range(Source.reduce_task_num):
            for j in range(gap):
                self.__part.setdefault(self.__frequency[j + i * gap], i)
        # 对不满足整除的余数组合按顺序加入分区
        for i, j in enumerate(range(Source.reduce_task_num * gap, 26)):
            self.__part.setdefault(self.__frequency[j], i)

    def shuffle(self, word_list: dict[str | int]):
        """
        依据单词的首字母，判断其处于哪个分区，并加入
        :param word_list: <k2, v2>键值对字典
        :return:
        """
        for word in word_list.items():
            # 大小写去敏感化，同时获取分区信息
            index = self.__part.get(word[0][0].lower())
            # 有些不是单词，不以字母开头，则直接加入最后一个分区
            if index is None:
                index = Source.reduce_task_num - 1
            # 加入对应分区
            if word[0] not in self.__partition[index]:
                self.__partition[index].setdefault(word[0], [])
            self.__partition[index][word[0]].append(str(word[1]))

    def save(self, task_order: int):
        """
        将每个mapper对应shuffler进行shuffle之后的数据单独存储在文件中
        :param task_order: 对应的线程编号
        :return:
        """
        for i in range(Source.reduce_task_num):
            f = None
            try:
                f = open(Source.base_file_path + str(task_order) + '_' + str(i), mode='w')
            except OSError:
                print("文件打开失败！")
            else:
                new_list = sorted(self.__partition[i].items(), key=lambda x: x[0])
                for word in new_list:
                    f.write(word[0] + ' ' + ' '.join(word[1]) + '\n')
            finally:
                if f is not None:
                    f.close()
