def f(base):
    l1 = []

    for i in base:
        for j in base:
            if i + j not in base:
                l1.append(i + j)
    for i in l1:
        base.append(i)

    s = []

    for i in base:
        if i not in s:
            s.append(i)

    base = s

    l1 = []

    for i in base:
        for j in base:
            if i + j not in base:
                l1.append(i + j)
    for i in l1:
        base.append(i)

    base = sorted(base)

    s = []

    for i in base:
        if i not in s:
            s.append(i)

    s = sorted(s)
    print(s)
    return len(s)


import random


def select_five_numbers():
    return random.sample(range(1, 16), 5)


# for i in range(20000):
#     print(i)
#     selected_numbers = select_five_numbers()
#     selected_numbers.append(16)
#     print(selected_numbers)
#     if f(selected_numbers) == 64:
#         print(selected_numbers)
#         exit()

print(f([2, 1, 15, 12, 8, 16]))
