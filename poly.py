def lagrange_coefficients(xs, ys, p):
    assert len(xs) == len(ys), "xs and ys should have the same length"
    n = len(xs)

    # 定义在有限域上的基本运算
    def add(a, b):
        return (a + b) % p

    def sub(a, b):
        return (a - b) % p

    def mul(a, b):
        return (a * b) % p

    def inv(a):
        return pow(a, p - 2, p)

    def div(a, b):
        return mul(a, inv(b))

    # 获取拉格朗日基函数的系数
    def lagrange_base_coeffs(i):
        coeffs = [1]
        for j in range(n):
            if i != j:
                # 多项式乘法
                new_coeffs = [0] * (len(coeffs) + 1)
                for k in range(len(coeffs)):
                    new_coeffs[k] = sub(0, mul(xs[j], coeffs[k]))
                    new_coeffs[k + 1] = add(new_coeffs[k + 1], coeffs[k])
                coeffs = new_coeffs
                # 除以常数
                factor = div(1, sub(xs[i], xs[j]))
                coeffs = [mul(coef, factor) for coef in coeffs]
        return coeffs

    # 初始化为0的系数列表
    result_coeffs = [0] * n

    # 根据每个拉格朗日基函数计算最终的系数
    for i in range(n):
        base_coeffs = lagrange_base_coeffs(i)
        for j in range(len(base_coeffs)):
            result_coeffs[j] = add(result_coeffs[j], mul(ys[i], base_coeffs[j]))

    return result_coeffs


# 示例
xs = [1, 2, 3]
ys = [1, 4, 9]
p = 65537

print(lagrange_coefficients(xs, ys, p))
