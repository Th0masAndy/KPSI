import math


def bloom_filter_false_positive_rate(n, m, k):
    """
    计算布隆过滤器的假阳性率。

    参数:
    n - 要加入布隆过滤器的元素数量
    m - 布隆过滤器的位数
    k - 使用的哈希函数数量

    返回:
    假阳性率
    """
    # 使用布隆过滤器的公式计算假阳性率
    return (1 - math.exp(-k * n / m)) ** k


# 示例
n = 199  # 元素数量
m = 1024  # 位数
k = 1  # 哈希函数数量

false_positive_rate = bloom_filter_false_positive_rate(n, m, k)

if false_positive_rate**16 < 2**-40:
    print("Yes")
else:
    print("No")
