img1 = imread('001.bmp');
img1 = rgb2gray(img1)
img2 = imread('002.bmp');
img2 = rgb2gray(img2)

N=8;
R=3;
f1 = img1;
f2 = img2;
[height, width] = size(f2);
for i = 1:N:height-N
    for j = 1:N:width-N
          MAD_min = 256*N*N;
          mvx = 0;
          mvy = 0;
          for k = 1:R
              for l = 1:R
                  MAD = sum(sum(abs(f1(i:i+N-1, j:j+N-1) - f2(i+k:i+k+N-1, j+l:j+l+N-1))));
                  if (MAD < MAD_min)
                      MAD_min = MAD;
                      dy = k;
                      dx = l;
                  end
              end
          end
          fp(i:i+N-1, j:j+N-1) = f2(i+dy:i+dy+N-1, j+dx:j+dx+N-1);        
          iblk = floor((i-1)/(N+1));
          jblk = floor((j-1)/(N+1));
          mvx(iblk, jblk) = dx;
          mvy(iblk, jblk) = dy;
    end
end
