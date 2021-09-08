import numpy as np

PI = 3.1415926535

print(np.sin(np.arange(0,PI*2,2*PI/101))) # 正弦波

print(np.arange(-1,1+1/99,2/99)) # 锯齿波的上升阶段