from decimal import Decimal

one = Decimal("1.0")
m = 1024


def prob(b, D):
    global d, m, one
    B = Decimal("%d" % b)
    p = d / B
    for i in range(1, b):
        p *= (d - Decimal("%d" % i)) / (B - Decimal("%d" % i))

    res = (one - one / m) ** (d - b) * (one / m) ** b

    r = Decimal("0.0")
    for i in range(b, D + 1):
        r += m * res * p
        res = res / m * (m - one) / m
        p = p * (d - Decimal("%d" % (i))) / Decimal("%d" % (i + 1))

    return r


def B(size):
    global d, m, one

    D = 3 * size

    m = Decimal("%d" % m)
    d = Decimal("%d" % D)
    s = 2**40
    threshold = one / Decimal("%d" % s)

    flag = 1
    lower = 1
    upper = 2
    while flag:
        if not (prob(upper, D) < threshold):
            upper = upper * 2
        if prob(upper, D) < threshold:
            flag = 0

    while upper - lower > 1:
        mid = (upper + lower) // 2
        if prob(mid, D) < threshold:
            upper = mid
        else:
            lower = mid

    return upper - 1


# for i in range(35000, 100000, 5000):
#     b = B(i)
#     print(i, b, i / b)
# print(B(88750))

print(5000000 / 88750)
