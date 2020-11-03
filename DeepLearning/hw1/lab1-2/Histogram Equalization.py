# -*- coding: utf-8 -*-
"""
Created on Fri Oct  2 01:31:16 2020

@author: cheng
"""
import cv2  
import numpy as np  

name = "../hist.png"
image = cv2.imread(name, 0)
#image = cv2.imread("D:/test/unequ.jpg", 0)  

lut = np.zeros(256, dtype = image.dtype )#创建空的查找表  

hist,bins = np.histogram(image.flatten(),256,[0,256])   
cdf = hist.cumsum() #计算累积直方图  
cdf_m = np.ma.masked_equal(cdf,0) #除去直方图中的0值  
cdf_m = (cdf_m - cdf_m.min())*255/(cdf_m.max()-cdf_m.min())#等同于前面介绍的lut[i] = int(255.0 *p[i])公式  
cdf = np.ma.filled(cdf_m,0).astype('uint8') #将掩模处理掉的元素补为0  

#计算  
result2 = cdf[image]

cv2.rectangle(result2, (1, 5), (100, 35), (0, 0, 0), -1)
text = "OpenCVLUT"
fontFace = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText( result2, text, ( 1, 25 ), fontFace, 0.5, ( 255, 0, 0 ) )
#--------------------------------------------
result = cv2.LUT(image, cdf)

cv2.rectangle(result, (1, 5), (100, 35), (0, 0, 0), -1)
text1 = "NumPyLUT"
fontFace = cv2.FONT_HERSHEY_SIMPLEX
cv2.putText( result, text1, ( 1, 25 ), fontFace, 0.5, ( 255, 0, 0 ) )


#cv2.imshow("OpenCVLUT", result)  
#cv2.imshow("lookup table_NumPyLUT", result2)
res = np.hstack((result,result2)) #stacking images side-by-side
cv2.imwrite('CV_Hint.png',res)

cv2.waitKey(0)  
cv2.destroyAllWindows()