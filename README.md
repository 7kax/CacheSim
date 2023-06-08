# 运行模拟器

1. 使用`cmake`编译源代码, `CMakeLists.txt`已经给出, 编译器为`gcc`, 至少需要支持`c++17`标准
2. 编译后, 模拟器的可执行文件是`build`目录下的`CacheSim.exe`

# 关于python代码

用于测试模拟器, 提供实验数据, 依赖于`build`目录下的`CacheSim.exe`文件(实验报告中提供了一份)

运行后, 会在`build`目录下生成`Excel`文件, 用于制作实验报告中的表格