clear;
clc;
img1 = imread('001.bmp');
img1 = rgb2gray(img1)
img2 = imread('002.bmp');
img2 = rgb2gray(img2)
pic1=img1%�e�@?��?��
pic2=img2;%?�e?��?��
pic3=zeros(size(pic1));%???
pich=size(pic1,1);
picl=size(pic1,2);

N = 16;     % Block Size
M = N-1;    
blockx=N;
blocky=N;%?�m?���j�p
blockh=pich/blockx;
blockl=picl/blocky;
blocka=zeros(blockx,blocky);%?�e?���Y?
blockb=zeros(blockx,blocky);%�b�W�@?���ǰt��?
vx=(7-1)/2;
vy=(7-1)/2;%�@???���̤j�t��
min=1000000;%�s��???�t���̤p��
px=0; py=0;
for i=1:blockh
    for j=1:blockl
        blocka=pic2( (i-1)*N+1:i*N,(j-1)*N+1:j*N );%?�e?����?
        %�bpic1���ǰt
        for ii=(i-1)*N+1-vy:i*N+vy-blockx+1
            for jj=(j-1)*N+1-vx: j*N+vx-blockx+1
                if (ii>=1) && (ii<=(pich-M)) && (jj>=1) && (jj<=picl-M)
                    blockb=pic1(ii:ii+M, jj:jj+M);
                    temp=sum(sum(abs(blocka-blockb)));
                    if min>temp
                        min=temp;
                        blockc=blockb;
                        px=ii;py=jj;
                    end
                end
            end
        end

        pic3( (i-1)*N+1:i*N,(j-1)*N+1:j*N )=blockc;
        min=1000000;
        px=0; py=0;
    end
end

pic1=uint8(pic1);%�e�@?
pic2=uint8(pic2);%?�e????��
pic3=uint8(pic3);%?�e?��??��
pic4 = abs(pic2-pic1);

subplot(2,3,1);imshow(pic1);    title('previous frame');
subplot(2,3,2);imshow(pic2);    title('current frame');
subplot(2,3,3);imshow(pic3);    title('MAD');
subplot(2,3,4);imshow(pic4); title('predict frame');
subplot(2,3,5);imshow(abs(pic2-pic3)); title('tested frame');

