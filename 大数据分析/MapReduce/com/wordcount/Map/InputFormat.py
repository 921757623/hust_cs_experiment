class InputFormat:

    @staticmethod
    def read_file(file_name: str):
        """
        对原始数据的读取以及格式化,生成k1, v1键值对
        :param file_name:
        :return: <k1, v1>键值对字典
        """
        format_data = dict()
        f = None
        try:
            f = open(file_name, 'r')
        except OSError:
            print("InputFormat.read_file：文件打开失败！")
        else:
            lines = f.readlines()
            # k1为行偏移量，v1为该行存储的单词数据
            for i, line in enumerate(lines):
                format_data.setdefault(i, line[:-1])
        finally:
            if f is not None:
                f.close()
            return format_data
