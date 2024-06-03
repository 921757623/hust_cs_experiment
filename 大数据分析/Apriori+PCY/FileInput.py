import csv
import re


class FileInput:
    __base_file_path = "D:\\bigData\\cluster\\data_source\\"

    @staticmethod
    def data_init():
        _data_source = list()
        try:
            f = open(FileInput.__base_file_path + 'Groceries.csv')
        except OSError as e:
            print("file_read：" + e.strerror)
            exit(-1)
        else:
            reader = csv.reader(f)
            next(reader)
            for index, row in enumerate(reader):
                _data_source.append(set())
                obj = re.compile(r'\{(?P<new_str>.*?)}')
                for i in obj.finditer(row[1]):
                    for item in i.group('new_str').split(','):
                        _data_source[index].add(item)
            f.close()
            return _data_source
