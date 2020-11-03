# -*- coding: utf-8 -*-
"""
Created on Fri Oct  2 00:42:35 2020

@author: cheng
"""
import cv2
 
img = cv2.imread('../data.png', cv2.IMREAD_UNCHANGED)
 
print('Original Dimensions : ',img.shape)
 
scale_percent = 50       # percent of original size
#----------------------------------
width = int(img.shape[1] * scale_percent / 100)
height = int(img.shape[0] * scale_percent / 100)
dim = (width, height)
# resize image
resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
 
print('Resize2xSmaller : ',resized.shape)
cv2.imshow("Resize2xSmaller", resized)
cv2.imwrite("./Resize2xSmaller.png", resized, [int(cv2.IMWRITE_PNG_COMPRESSION), 3])
#----------------------------------
scale_percent = 200       # percent of original size
#----------------------------------
width = int(img.shape[1] * scale_percent / 100)
height = int(img.shape[0] * scale_percent / 100)
dim = (width, height)
# resize image
resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
 
print('Resize2xBigger : ',resized.shape)
cv2.imshow("Resize2xBigger", resized)
cv2.imwrite("./Resize2xBigger.png", resized, [int(cv2.IMWRITE_PNG_COMPRESSION), 3])
#----------------------------------

cv2.waitKey(0)
cv2.destroyAllWindows()