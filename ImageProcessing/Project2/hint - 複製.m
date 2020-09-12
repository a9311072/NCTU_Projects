clc
clear all
close all
%%
%將txt讀入後，reshape成600*800
%請勿使用ppt上的圖片處理，會產生錯誤。
I = load('periodic_noise.txt');

% periodic_noise = reshape(I,[600 800]);
imgn = reshape(I,[600 800]);
imshow(imgn,[])


% % result = medfilt2(imgn);
% % imshow(result,[]);
% [ROW,COL, DIM] = size(imgn); 
% for r = 2:ROW-1
%     for c = 2:COL-1
%         median3x3 =[imgn(r-1,c-1)    imgn(r-1,c) imgn(r-1,c+1)
%                      imgn(r,c-1)      imgn(r,c)      imgn(r,c+1)
%                      imgn(r+1,c-1)      imgn(r+1,c) imgn(r+1,c+1)];
%         sort1 = sort(median3x3, 2, 'descend');
%         sort2 = sort([sort1(1), sort1(4), sort1(7)], 'descend');
%         sort3 = sort([sort1(2), sort1(5), sort1(8)], 'descend');
%         sort4 = sort([sort1(3), sort1(6), sort1(9)], 'descend');
%         mid_num = sort([sort2(3), sort3(2), sort4(1)], 'descend');
%         Median_Img(r,c) = mid_num(2);
%     end
% end

% figure,imshow(Median_Img,[])

% [x,y]=meshgrid(-128:127,-128:127)
% z=sqrt(x.^2+y.^2)
% myc = (z<15)
% cfh=Median_Img.*myc
% g=fspecial('gaussian',255,10)
% max(g)
% g1=mat2gray(g)
% max(g1)


%Y = fftshift(fft2(double(imgn)));
Y=log10(abs(fft2(imgn))+1)
% YF = Y.*g1
%figure, fftshow(YF)
figure,imshow(Y,[])
Y = ifft2(Y);
Y = (-1)*Y
figure,imshow(Y,[])
% P2 = abs(Y/L);
% P1 = P2(1:L/2+1);
% P1(2:end-1) = 2*P1(2:end-1);
% f = Fs*(0:(L/2))/L;
% plot(f,P1) 
% imshow(f,[])