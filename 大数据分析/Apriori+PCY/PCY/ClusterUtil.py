import math
from itertools import combinations
from bitmap import BitMap


class ClusterUtil:
    """
    工具类
    """
    support = 0.005
    confidence = 0.5

    @staticmethod
    def construct(data: list, bitmap: BitMap, num: int, bucket_num: int):
        """
        生成候选集
        :param data: 原始数据集
        :param bitmap: 上一轮生成的BitMap
        :param num: 当前轮次，也用于排序组合确定组合大小
        :param bucket_num: 生成上一轮BitMap的哈希桶的数量
        :return: 候选集
        """
        selectable = bitmap.nonzero()
        _c = list()
        for items in data:
            if len(items) < num:
                continue
            # 对原始数据集每行排列组合相应大小的元组
            for item in combinations(items, num):
                pos = sum(hash(i) for i in item) % bucket_num
                # 如果哈希值的取余值在bitMap对应位置值为1，则加入候选集
                if pos in selectable:
                    _c.append(item)
        return _c

    @staticmethod
    def hash_with_bitmap(data: list, num: int):
        """
        生成下一轮要用的BitMap
        :param data: 原始数据集
        :param num: 当前轮次
        :return: BitMap，哈希桶大小
        """
        _list = []
        for item in data:
            if len(item) < num:
                continue
            # 对原始数据集排列组合
            for i in combinations(item, num):
                _list.append(i)
        # 哈希桶大小为原始候选集大小的十分之一
        bucket_num = math.ceil(len(_list) / 10)
        _bitmap = BitMap(bucket_num)
        bucket = [0] * bucket_num
        # 对对应哈希桶的值加一计数
        for item in _list:
            pos = sum(hash(i) for i in item) % bucket_num
            bucket[pos] += 1
        for i in range(bucket_num):
            if (bucket[i] / len(data)) >= ClusterUtil.support:
                _bitmap.set(i)
        return _bitmap, bucket_num

    @staticmethod
    def count(data: list, __len: int):
        """
        对初步过滤后的候选集再次细分计数过滤，得到频繁项集
        :param data: 候选集
        :param __len: 原始数据集大小，用于计算支持度
        :return: 频繁项集
        """
        _l = dict()
        tmp = dict()
        # 计算每个候选项出现的次数
        for item in data:
            key = frozenset(item)
            if key not in tmp:
                tmp[key] = 1
            else:
                tmp[key] += 1
        # 计算支持度
        for key, value in tmp.items():
            if (value / __len) >= ClusterUtil.support:
                _l[key] = value
        return _l
