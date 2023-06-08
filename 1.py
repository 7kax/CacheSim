import pandas as pd
import fileinput
import subprocess
import re


def modify_lines(file_path, line_numbers, new_contents):
    # 修改文件中的某些行
    with fileinput.FileInput(file_path, inplace=True, backup=".bak") as file:
        for line_number, line in enumerate(file, start=1):
            if line_number in line_numbers:
                line = str(new_contents[line_numbers.index(line_number)]) + "\n"
            print(line, end="")


def run_command(cmd):
    # 在命令行中执行命令, 并获取程序的标准输出
    p = subprocess.Popen(
        cmd,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )
    # 获取标准输出
    out = p.stdout.read()
    # 获取out中的第一行
    first_line = out.splitlines()[0]
    # 用正则表达式提取第一行中的小数
    result = re.findall(r"\d+\.?\d*", first_line)
    # 将字符串转换为浮点数
    result = float(result[0])
    # 返回浮点数
    return result


configPath = "config.txt"
modifyLines = [1, 2, 3]
way = 1
blockSizes = [4, 8, 16, 32, 64, 128, 256]
chacheSizes = [16, 32, 64, 128, 256, 512, 1024]
command = "cd build && CacheSim.exe -c config.txt -t ls.trace"

data = {}
for chacheSize in chacheSizes:
    dataKey = str(chacheSize) + "KB"
    data[dataKey] = []
    for blockSize in blockSizes:
        modify_lines(configPath, modifyLines, [blockSize, way, chacheSize])
        result = run_command(command)
        data[dataKey].append(result)

df = pd.DataFrame(data)
df.to_excel("data.xlsx")
