import time

from com.wordcount.Source import Source


class Reducer:
    __result = dict[str: int]()
    __part = dict()

    def reduce(self, order: int):
        """
        对各个mapper产生的对应编号分区数据进行汇总
        :param order: 对应进程序号
        :return: 汇总结果送回主进程
        """
        start = time.time()
        total = 0
        for i in range(1, Source.map_task_num + 1):
            try:
                f = open(Source.base_file_path + str(i) + '_' + str(order), mode='r')
            except OSError:
                print("文件打开失败！")
            else:
                lines = f.readlines()
                for line in lines:
                    word_with_count = line[:-1].split(' ')
                    if word_with_count[0] not in self.__result:
                        self.__result.setdefault(word_with_count[0], 0)
                    for element in word_with_count[1:]:
                        # 汇总相同的单词
                        self.__result[word_with_count[0]] += int(element)
                        # 统计单词总数，判断是否遗漏单词
                        total += int(element)
                f.close()
        end = time.time()
        return [end - start, total, self.__result]

    def save(self):
        """
        保存结果文件
        :return:
        """
        f = None
        try:
            f = open(Source.base_file_path + 'result', mode='w')
        except OSError:
            print("Reducer.save：文件打开失败！")
        else:
            total = 0
            new_list = sorted(self.__result.items(), key=lambda x: x[0])
            for key, value in new_list:
                f.write(key + ' ' + str(value) + '\n')
                total += value
        finally:
            if f is not None:
                f.close()
        print("finish!")
