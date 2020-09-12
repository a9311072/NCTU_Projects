clear;
clc;

f=imread('cameraman.tif');
f=double(f);
T=dctmtx(8);
dct=@(block_struct)T*block_struct.data*T';
invdct=@(block_struct)T'*block_struct.data*T;

% Dct
f_tf=blockproc(f,[8,8],dct);

%     jpeg normalization
%     qt_mtx=[16,11,10,16,24,40,51,61
%         12,12,14,19,26,58,60,55
%         14,13,16,24,40,57,69,56
%         14,17,22,29,51,87,80,62
%         18,22,37,56,68,109,103,77
%         24,35,55,64,81,104,113,92
%         49,64,78,87,103,121,120,101
%         72,92,95,98,112,100,103,99];

qt_mtx_n10=[10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10
10 10 10 10 10 10 10 10];

qt_mtx_n20 =[20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20
20 20 20 20 20 20 20 20];

% quantization n=10
f_qt_n10=blockproc(f_tf,[8,8],@(block_struct)block_struct.data./qt_mtx_n10);
f_qt_n10=round(f_qt_n10); %四捨五入

g_n10=blockproc(f_qt_n10,[8,8],@(block_struct)block_struct.data.*qt_mtx_n10);
g_n10=blockproc(g_n10,[8,8],invdct);  % IDct


% quantization n=20
f_qt_n20=blockproc(f_tf,[8,8],@(block_struct)block_struct.data./qt_mtx_n20);
f_qt_n20=round(f_qt_n20); %四捨五入

g_n20=blockproc(f_qt_n20,[8,8],@(block_struct)block_struct.data.*qt_mtx_n20);
g_n20=blockproc(g_n20,[8,8],invdct);  % IDct


f=uint8(f);
g_n10=uint8(g_n10);
g_n20=uint8(g_n20);

MSE_n10 = immse(f, g_n10);
MSE_n20 = immse(f, g_n20);
MSE_n10 = sprintf('compressed with n=10, MSE is %i', MSE_n10);
MSE_n20 = sprintf('compressed with n=20, MSE is %i', MSE_n20);

subplot(2,3,1);imshow(f);           title('original image');
subplot(2,3,2);imshow(f_qt_n10);    title('quantization with n=10')
subplot(2,3,3);imshow(g_n10);       title(MSE_n10)
subplot(2,3,4);imshow(f);           title('original image');
subplot(2,3,5);imshow(f_qt_n20);    title('quantization with n=20')
subplot(2,3,6);imshow(g_n20);       title(MSE_n20)


