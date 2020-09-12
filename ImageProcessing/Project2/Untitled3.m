clc
clear all
close all

I = imread('problem3_4e.bmp');

% Get histogram:
[Counts, grayLevels] = imhist(I);
% Computer probability density function:
pdf = Counts / numel(I);
plot(grayLevels, pdf, 'b-', 'LineWidth', 2);
% grid on;