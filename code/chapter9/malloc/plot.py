import os
import matplotlib.pyplot as plt
import numpy

print("It begins...")

os.system('make')
os.system('./implicit 1 0 >> time.txt')
os.system('./implicit 2 0 >> time.txt')
os.system('./explicit 1 0 >> time.txt')
os.system('./explicit 2 0 >> time.txt')
os.system('./segregated 1 0 >> time.txt')
os.system('./segregated 2 0 >> time.txt')

method = numpy.loadtxt('./time.txt')

name_list = ['implicit', 'explicit', 'segregated']

sample_1 = []
sample_2 = []
sample_3 = []

sample_1.append(method[0])
sample_1.append(method[1])
sample_2.append(method[2])
sample_2.append(method[3])
sample_3.append(method[4])
sample_3.append(method[5])

x = list(range(len(sample_1)))

total_width, n = 0.9, 3
width = total_width/n

plt.bar(x, sample_1, width=width, label='implicit', fc = 'y')
for i in range(len(x)):
    x[i] = x[i] + width
plt.bar(x, sample_2, width=width, label='explicit', fc = 'b')
for i in range(len(x)):
    x[i] = x[i] + width

plt.bar(x, sample_3, width=width, label='segregated', fc = 'r')
plt.legend()
plt.savefig('./time.png')
plt.close()

os.system("rm time.txt")

os.system('./implicit 1 1 >> fragment.txt')
os.system('./implicit 2 1 >> fragment.txt')
os.system('./explicit 1 1 >> fragment.txt')
os.system('./explicit 2 1 >> fragment.txt')
os.system('./segregated 1 1 >> fragment.txt')
os.system('./segregated 2 1 >> fragment.txt')

with open("./fragment.txt", 'r') as f:
    data = f.readlines()  # 将txt中所有字符串读入data
    container = []
    for line in data:
        numbers = line.split()        # 将数据分隔
        numbers_float = map(float, numbers) #转化为浮点数
        container.append(list(numbers_float))

xstick = []
for i in range(1,102):
    xstick.append(i)

plt.grid(True, linestyle='--', alpha=0.5)
plt.xlabel("execution flow", fontdict={'size': 16})
plt.ylabel("number of fragments", fontdict={'size': 16})
plt.title("The number of fragments during execution", fontdict={'size': 20})
plt.plot(xstick, container[0], c='yellow', label="implicit for sample 1")
plt.plot(xstick, container[1], c='yellow', linestyle='--', label="implicit for sample 2")
plt.plot(xstick, container[2], c='blue', label="explicit for sample 1")
plt.plot(xstick, container[3], c='blue', linestyle='--', label="explicit for sample 2")
plt.plot(xstick, container[4], c='red', label="segregated for sample 1")
plt.plot(xstick, container[5], c='red', linestyle='--', label="segregated for sample 2")

plt.legend()
plt.savefig("./fragments.png")
plt.close()

os.system('rm fragment.txt')
os.system('make clean')

