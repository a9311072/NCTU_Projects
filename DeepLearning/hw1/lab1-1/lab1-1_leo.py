# -*- coding: utf-8 -*-
"""
Created on Thu Oct  1 23:38:35 2020

@author: cheng
"""
#============Split the color image to Red/Green/Blue image=========
from PIL import Image

img =Image.open('../data.png')
data = img.getdata()
# Suppress specific bands (e.g. (255, 120, 65) -> (0, 120, 0) for g)
#– Split the color image to Red/Green/Blue image
r = [(d[0], 0, 0) for d in data]
g = [(0, d[1], 0) for d in data]
b = [(0, 0, d[2]) for d in data]

img.putdata(r)
img.save('Split_Red.png')
img.putdata(g)
img.save('Split_Green.png')
img.putdata(b)
img.save('Split_Blue.png')


#================= HSV image and show each channel================
#import numpy as np
import cv2

image = cv2.imread("../data.png")
#cv2.imshow("Original",image)
#cv2.waitKey(0)

#R、G、B分量的提取
#(B,G,R) = cv2.split(image)#提取R、G、B分量
#cv2.imshow("Red",R)
#cv2.imshow("Green",G)
#cv2.imshow("Blue",B)
#HSV空间
hsv = cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
cv2.imshow("HSV",hsv)
#cv2.imwrite("../HSV.jpg", hsv)
cv2.imwrite("./HSV.png", hsv, [int(cv2.IMWRITE_PNG_COMPRESSION), 3])

h = hsv[:,:,0]
s = hsv[:,:,1]
v = hsv[:,:,2]

cv2.imshow("Hue",h)
cv2.imwrite("./Hue.png", h, [int(cv2.IMWRITE_PNG_COMPRESSION), 3])

cv2.imshow("Saturation",s)
cv2.imwrite("./Saturation.png", s, [int(cv2.IMWRITE_PNG_COMPRESSION), 3])

cv2.imshow("Value",v)
cv2.imwrite("./Value.png", v, [int(cv2.IMWRITE_PNG_COMPRESSION), 3])


#cv2.waitKey(0)
cv2.destroyAllWindows()
