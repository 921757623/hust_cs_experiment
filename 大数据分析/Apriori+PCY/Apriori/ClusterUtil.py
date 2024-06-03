class ClusterUtil:
    """
    工具类
    """
    support = 0.005
    confidence = 0.5

    @staticmethod
    def count(_c: dict, base: list):
        """
        统计元组在原始数据集中出现的次数
        :param _c: 候选集
        :param base: 原始数据集
        :return:
        """
        for item in _c:
            for base_item in base:
                # 转化成集合，并运用集合运算判断是否在原始数据集中出现
                a = set(item)
                if a <= base_item:
                    _c[item] += 1

    @staticmethod
    def filter(data: dict, last: dict, base_size: int, cycle_cnt: int):
        """
        过滤支持度不满足阈值的候选项
        :param data: 需要过滤的候选集
        :param last: 上一轮的频繁项集
        :param base_size: 原始数据集大小
        :param cycle_cnt: 当前轮次
        :return: 这一轮的频繁项集
        """
        _l = dict()
        if cycle_cnt == 1:
            for item in data:
                if data[item] / base_size >= ClusterUtil.support:
                    _l[item] = data[item]
        else:
            for key, value in data.items():
                # 计算支持度和置信度
                if (value / base_size) >= ClusterUtil.support or \
                        (value / last[key[:-1]] >= ClusterUtil.confidence and
                         value / last[key[-1:]] >= ClusterUtil.confidence):
                    _l[key] = value
        return _l

    @staticmethod
    def construct(ll: dict, cycle_cnt: int):
        """
        生成候选集
        :param ll: 上一轮的频繁项集
        :param cycle_cnt: 当前轮次
        :return: 候选集
        """
        _c = dict()
        _l = list(ll.keys())
        if cycle_cnt == 1:
            # 生成二元组只需要两两组合就可以了
            for i in range(len(_l)):
                for j in range(i + 1, len(_l)):
                    _c[_l[i] + _l[j]] = 0
        else:
            # 三元组及以上元组的生成需要特殊判断
            for i in range(len(_l)):
                for j in range(i + 1, len(_l)):
                    # 拆分前cycle_cnt - 1项，并判断是否相等
                    _l1 = _l[i][:cycle_cnt - 1]
                    _l2 = _l[j][:cycle_cnt - 1]
                    # 相等则将它们俩组合
                    if _l1 == _l2:
                        _tmp = _l[i] + _l[j][cycle_cnt - 1:]
                        _c[_tmp] = 0
        return _c
