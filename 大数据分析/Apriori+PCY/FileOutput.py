class FileOutput:
    __base_file_path = "D:\\bigData\\cluster\\data_source\\"

    @staticmethod
    def result_output(id: int, data: dict, __len: int):
        f = None
        try:
            f = open(FileOutput.__base_file_path + f"result{id}.csv", "w")
        except OSError:
            print("文件打开失败")
        else:
            sorted_data = sorted(data.items(), key=lambda x: x[1], reverse=True)
            f.write("tuple, count, support\n")
            for key, value in sorted_data:
                f.write(str(set(key)) + ", " + str(value) + ", " + str(value / __len) + "\n")
        finally:
            if f is not None:
                f.close()
