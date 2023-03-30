import seekfree, pyb
import sensor, image, time, math
import os, tf
from pyb import LED
from machine import UART

uart = UART(1, baudrate=115200)     # 初始化串口 波特率设置为115200 TX是B12 RX是B13
#
# x = [27, 23, 14, 32, 21, 16, 29, 22, 27, 13, 19, 32]
# y = [23, 22, 21, 20, 18, 16, 16, 14, 12, 10, 9, 9]

# uart.write("A")
# for i in range(len(x)):
#     uart.write("%c" % x[i])
#     utime.sleep_ms(5)
#
# for i in range(len(y)):
#     uart.write("%c" % y[i])
#     utime.sleep_ms(5)
#
# uart.write("Y")

dis_X = 30
dis_Y = 15

uart.write("C")
uart.write("%c" % dis_X)
uart.write("%c" % dis_Y)
uart.write("D")




