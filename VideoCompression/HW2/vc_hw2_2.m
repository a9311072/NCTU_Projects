clear;
clc;
img1 = imread('001.bmp');
img1 = rgb2gray(img1)
img2 = imread('002.bmp');
img2 = rgb2gray(img2)

% Block is 16, and Window Size is 7
FullSearch = motionEstES(img1, img2, 16, 7) 
ThreeStepSearch = motionEstTSS(img1, img2, 16, 7)