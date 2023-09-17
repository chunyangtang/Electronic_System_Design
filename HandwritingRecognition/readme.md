# 综合实验——使用FPGA的手写数字识别

## 项目介绍

综合实验中，我使用FPGA开发板实现了手写数字识别任务，该任务将一个已训练好的神经网络载入程序中，并使用UART串口与Windows计算机通信，接收计算机发送的手写数字图片，对其进行识别，并将结果显示在屏幕上。

此外，该项目还引入了按钮来实际判断模型是否准确作为反馈机制；使用Python开发了一个具有UART通信功能的人工手写数字窗口。

## 程序运行环境
- ZYNQ MINI (7010)开发板
- Vivado 2022.2
- Nuclei Studio 2022-12

## 文件说明
- `my_e203_sys.bits`：蜂鸟E203处理器的比特流文件，需要通过JTAG端口使用Vivado下载至开发板。
- `E203_ZYNQ.xdc`：比特流文件对应的引脚绑定信息（供对应参考，不重新综合无需使用）。
- `board_ddr200t.h`：Nuclei RISC-V C/C++ Project中`hbird_sdk\SoC\hbirdv2\Board\ddr200t\Include`下的引脚对应文件。
- `openocd_hbirdv2.cfg`：Nuclei RISC-V C/C++ Project中`hbird_sdk\SoC\hbirdv2\Board\ddr200t`下的配置文件。
- `application/`：在Nuclei环境（即开发板上）运行的程序代码。
    - `main.c`：主程序，包含了手写数字识别的主要流程：屏幕、网络初始化->与电脑UART通信获取手写数字->识别->显示结果->根据按键继续输出->等待新一轮输入。
    - `network_values.h`：网络的全部参数（声明为`const float`）
    - `network_tools.h`：网络中用于做预测的函数与结构体
    - `oleddriver.h`：屏幕驱动函数，其中有输出字符串、显示函数等功能和用法的详细介绍
    - `oledfont.h`：屏幕显示的字体文件（即ASCII码等对应的“图片”）
    - `nice_sum_accelerator.h`：NICE协处理器的调用函数（经测试性能不佳）



- `mnist_desktop_project`：在Windows环境中配套的代码，包含数据集处理、训练测试程序与FPGA的UART连接程序，应先运行`dataset_generator.py`生成数据集，再运行`mnist.py`训练网络并导出参数（编译命令`gcc -o main mnist.c mnist_file.c neural_network.c`），最后在通信时运行`connect_uart_mnist.py`。
    - `mnist.c`：C中训练的主程序，程序中定义了2个数值作为超参数，声明了4个字符串代表要训练的数据集位置，程序默认随机化初始网络参数，在训练结束后导出网络参数到指定文件中。导出数据已经加入大括号，只需拷贝到`application/network_values.h`中的`b`、`W`处即可使用。
    - `mnist_file.c`、`mnist_file.h`、`neural_network.c`、`neural_network.h`：`mnist.c`中使用到的功能的实现，需要在编译时一起编译。
    - `dataset_generator.py`：数据集增强工具，用于将原始数据集进行二值化、旋转、平移、缩放操作，生成新的数据集（为原数据及的7倍大小）。
    - `connect_uart_mnist.py`：用于与FPGA通信，会发送写好的数字数组，并接收FPGA返回的字符。依赖`image_handler.py`用于实现绘图窗口，注意根据电脑修改程序中的端口号。
    - `image_handler.py`：PyGame绘图窗口程序。
    - `dataset_visualizer.py`：数据集的可视化，需要调整程序末尾来决定导出的数据集和图片数量。
    - `network.txt`、`network_binarized.txt`：训练好的神经网络参数，分别为用增强数据集和二值化后的增强数据集训练的结果。
    - `data/`：原始的MNIST数据集。
    - `requirements.txt`：Python依赖库列表，使用`pip install -r requirements.txt`安装。


- `NETWORK_LARGE/`：测试的784*12*12*10的神经网络，其中`Desktop_Part/`为在电脑上训练的部分（使用`gcc -o mnist.exe *.c`编译即可运行），导出的网络参数可根据txt文件头的说明查看。`application`为Nuclei中的application部分，与上述配置相似，其中很遗憾由于操作失误导致主程序`mnist_demo.c`丢失，采用文件夹中的`mnist_demo.o`仍可以正常编译链接运行，但无法查看和修改程序。使用`mnist_desktop_project/connect_uart_mnist.py`即可查看回传结果。
