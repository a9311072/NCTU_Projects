clc
clear all
close all
%%
%將txt讀入後，reshape成600*800
%請勿使用ppt上的圖片處理，會產生錯誤。
I = load('periodic_noise.txt');

img = reshape(I,[600 800]);
imshow(img,[])

f = img;
f = mat2gray(f,[0 255]);

[M,N] = size(f);
P = 2*M;
Q = 2*N;
fc = zeros(M,N);

for x = 1:1:M
    for y = 1:1:N
        fc(x,y) = f(x,y) * (-1)^(x+y);
    end
end

F = fft2(fc,P,Q);

H_NF = ones(P,Q);

for x = (-P/2):1:(P/2)-1
     for y = (-Q/2):1:(Q/2)-1
        D = 30;
        
        v_k = 300; u_k = 300;
        D_k = ((x+u_k)^2 + (y+v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        D_k = ((x-u_k)^2 + (y-v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        
        v_k = 500; u_k = 100;
        D_k = ((x+u_k)^2 + (y+v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        D_k = ((x-u_k)^2 + (y-v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        
        v_k = -200; u_k = 200;
        D_k = ((x+u_k)^2 + (y+v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        D_k = ((x-u_k)^2 + (y-v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        
        v_k = 100; u_k = 500;
        D_k = ((x+u_k)^2 + (y+v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
        D_k = ((x-u_k)^2 + (y-v_k)^2)^(0.5);
        H_NF(x+(P/2)+1,y+(Q/2)+1) = H_NF(x+(P/2)+1,y+(Q/2)+1) * 1/(1+(D/D_k)^4);
     end
end

G_1 = H_NF .* F;

g_1 = real(ifft2(G_1));
g_1 = g_1(1:1:M,1:1:N);     

for x = 1:1:M
    for y = 1:1:N
        g_1(x,y) = g_1(x,y) * (-1)^(x+y);
    end
end

%% -----show-------

figure();

imshow(f,[0 1]);
xlabel('a).Original Image');

figure();
imshow(log(1 + abs(F)),[ ]);
xlabel('b).Fourier spectrum before Butterworth Notch filter');

figure();
imshow(H_NF,[0 1]);
xlabel('c).Butterworth Notch filter');

figure();
h = mesh(1:10:Q,1:10:P,H_NF(1:10:P,1:10:Q));
set(h,'EdgeColor','k');
axis([0 Q 0 P 0 1]);
xlabel('u');ylabel('v');
zlabel('|H(u,v)|');

figure();
imshow(log(1 + abs(G_1)),[ ]);
xlabel('e).Fourier spectrum after Butterworth Notch filter');

figure();
imshow(g_1,[0 1]);
xlabel('d).Result image with noise');

[ROW,COL, DIM] = size(g_1); 
for r = 2:ROW-1
    for c = 2:COL-1
        median3x3 =[g_1(r-1,c-1)    g_1(r-1,c) g_1(r-1,c+1)
                     g_1(r,c-1)      g_1(r,c)      g_1(r,c+1)
                     g_1(r+1,c-1)      g_1(r+1,c) g_1(r+1,c+1)];
        sort1 = sort(median3x3, 2, 'descend');
        sort2 = sort([sort1(1), sort1(4), sort1(7)], 'descend');
        sort3 = sort([sort1(2), sort1(5), sort1(8)], 'descend');
        sort4 = sort([sort1(3), sort1(6), sort1(9)], 'descend');
        mid_num = sort([sort2(3), sort3(2), sort4(1)], 'descend');
        MedianFilter_Img(r,c) = mid_num(2);
    end
end

figure();
imshow(MedianFilter_Img,[0 1]);
xlabel('f).Result image without noise');

%End


