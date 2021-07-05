# author Morty
# 2019-8-25
# python 3.6
from pyecharts.charts import Surface3D
from pyecharts import options as opts
 
class CreateMountain:
    def __init__(self):
        self.data_set = []
        self.gragh = Surface3D()
 
    def data_process(self):
        with open(r"./data_set.txt", "r") as f:
            data_set_origin = f.readlines()
 
        for line in data_set_origin:
            for index, item in enumerate(line.split("\t")[:-1], start=0):
                # index即为步长
                if index == 0:
                    read_throughput = int(item[0:-1])
                    flag = item[-1]
                    if flag == "m":
                        read_throughput *= 1000000
                    elif flag == "k":
                        read_throughput *= 1000
                else:
                    size = int(item)
                    self.data_set.append([index, read_throughput, size])
    
    def plot(self):
        self.data_process()
        self.gragh.add(
            # series name
            "",
            # 通过k*k循环展开测出来的数据集
            data=self.data_set,
            # 三维图形的着色效果配置，realistic即为真实感渲染
            shading="realistic",
            # 图元配置项
            itemstyle_opts=opts.ItemStyleOpts(),
            # 3D X坐标轴配置项
            xaxis3d_opts=opts.Axis3DOpts(
                name="步长",
                name_gap=40),
            # 3D Y坐标轴配置项
            yaxis3d_opts=opts.Axis3DOpts(
                name="大小/B",
                name_gap=40),
            # 3D Z坐标轴配置项
            zaxis3d_opts=opts.Axis3DOpts(
                name="存储器读取速度MB/s",
                name_gap=40),
            # 三维笛卡尔坐标系配置项
            grid3d_opts=opts.Grid3DOpts(
                width=100,
                height=100,
                depth=100
                ),
            ).set_global_opts(
            title_opts=opts.TitleOpts(
                title="Memory Mountain K*K Loop unrolling",
                pos_left="center",
                pos_top=90,
                ),
            visualmap_opts=opts.VisualMapOpts(
                dimension=3,
                max_=1,
                min_=-1,
                range_color=[
                "#313695",
                "#4575b4",
                "#74add1",
                "#abd9e9",
                "#e0f3f8",
                "#ffffbf",
                "#fee090",
                "#fdae61",
                "#f46d43",
                "#d73027",
                "#a50026",
                ],
            )).render()
                        
if __name__ == "__main__":
    c = CreateMountain()
    c.plot()
