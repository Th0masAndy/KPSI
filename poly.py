import time
import numpy as np
import random

L = []
for i in range(1024):
    line = [0] * 1024
    for e in line:
        e = random.randint(0, 65537)
    line = np.array(line)
    L.append(line)


y = [1 if i % 6 == 0 else 0 for i in range(1024)]

res = [0] * 1024
res = np.array(res)

start_time = time.time()


for i in range(1024):
    if y[i] == 0:
        continue
    else:
        res = res + L[i]


end_time = time.time()

print("cost %.6fs" % (end_time - start_time))
