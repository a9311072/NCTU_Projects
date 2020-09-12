clc
clear all
close all
%% Main
p3_4a = imread('problem3_4a.bmp');
p3_4b = imread('problem3_4b.bmp');
p3_4c = imread('problem3_4c.bmp');
p3_4d = imread('problem3_4d.bmp');
p3_4e = imread('problem3_4e.bmp');

% Q = 0, equals arithmetic_mean_filter
% figure,imshow(p3_4a);
pdf_plot(p3_4a);
result = harmonic_mean_filter(p3_4a, 3);
figure,imshow(result,[]);
% pdf_plot(result)

% adptive_median
% figure,imshow(p3_4b);
pdf_plot(p3_4b);
result = adaptive_median(p3_4b,5)
figure,imshow(result,[]);
% pdf_plot(result)

% adptive_median + arithmetic_mean_filter
% figure,imshow(p3_4c);
pdf_plot(p3_4c);
result = adaptive_median(p3_4c,5)
result = arithmetic_mean_filter(result,3)
figure,imshow(result,[]);
% pdf_plot(result)

% adptive_median*2 
% figure,imshow(p3_4d);
pdf_plot(p3_4d);
result = adaptive_median(p3_4d,5)
result = adaptive_median(result,3)
figure,imshow(result,[]);
% pdf_plot(result)

% arithmetic_mean_filter
% figure,imshow(p3_4e);
pdf_plot(p3_4e);
result = arithmetic_mean_filter(p3_4e,3)
figure,imshow(result,[]);
% pdf_plot(result)


%% PDF plot function
function x = pdf_plot(image)
    % Get histogram:
    [counts, grayLevels] = imhist(image);
    % Computer probability density function:
    pdf = counts / numel(image);
    figure;
%     plot(grayLevels, pdf, 'b-', 'LineWidth', 2);
    x = plot(grayLevels, pdf, 'b-', 'LineWidth', 2);
end 

%% arithmetic_mean_filter
function [result_imag]= arithmetic_mean_filter(image,filter_size)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);     
            result_imag(x,y)=sum(slid(:))/numel(slid);
        end
    end
end

%% geo_mean_filter
function [result_imag]= geo_mean_filter(image,filter_size)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);     
            result_imag(x,y)=prod(slid(:))^( 1 /numel(slid));
        end
    end
end

%% harmonic_mean_filter
function [result_imag]= harmonic_mean_filter(image,filter_size)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);
            slid = 1./slid;
            result_imag(x,y) = numel(slid(:))/sum(slid(:));
        end
    end
end

%% contra-harmonic_mean_filter
% Q= 0, equals arithmetic_mean_filter
% Q= -1, equals harmonic_mean_filter
function [result_imag]= contra_harmonic_mean_filter(image,filter_size,Q)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    %Q= 1.5 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);
            result_imag(x,y) = sum(slid(:).^(Q+ 1 ))/sum(slid(:).^(Q));
        end
    end
end

%% max_value_filter
function [result_imag]= max_value_filter(image,filter_size)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);
            temp = slid(:);
            result_imag(x,y)= max(temp);
        end
    end
end

%% min_value_filter
function [result_imag]= min_value_filter(image,filter_size)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);
            temp = slid(:);
            result_imag(x,y)= min(temp);
        end
    end
end

%% half_value_filter
function [result_imag]= half_value_filter(image,filter_size)
    Ex_image = extend_imag(image);
    result_imag = image;
    [M,N]=size(Ex_image);
    helf_size = (filter_size -1 )/ 2 ;
    for x= 1 +helf_size: 1 :M-helf_size
        for y= 1 +helf_size: 1 :N-helf_size
            slid=Ex_image(x-helf_size: 1 :x+helf_size,y-helf_size: 1 :y+helf_size);
            temp = slid(:);
            result_imag(x,y)= (max(temp)+min(temp))/ 2 ;
        end
    end
end

%% 
function [result_imag] = extend_imag(image)
    [M,N] = size(image);
    temp = zeros(M+ 2 ,N+ 2 );
    for x = 1 :M
        temp(x+ 1 ,:) = [image(x, 1 ) image(x,:) image(x,N)];
    end
    temp( 1 ,:) = temp( 2 ,:);
    temp(M+ 2 ,:) = temp(M+ 1 ,:);
    result_imag = temp;
end

%%
function f = adaptive_median(img, filter_size)
if (filter_size <= 1) | (filter_size/2 == round(filter_size/2)) | (filter_size ~= round(filter_size))
   error('SMAX must be an odd integer > 1.')
end
[M, N] = size(img);

% Initial setup.
f = img;
f(:) = 0;
alreadyProcessed = false(size(img));

% Begin filtering.
for k = 3:2:filter_size
   zmin = ordfilt2(img, 1, ones(k, k), 'symmetric');
   zmax = ordfilt2(img, k * k, ones(k, k), 'symmetric');
   zmed = medfilt2(img, [k k], 'symmetric');
   
   processUsingLevelB = (zmed > zmin) & (zmax > zmed) & ...
       ~alreadyProcessed; 
   zB = (img > zmin) & (zmax > img);
   outputZxy  = processUsingLevelB & zB;
   outputZmed = processUsingLevelB & ~zB;
   f(outputZxy) = img(outputZxy);
   f(outputZmed) = zmed(outputZmed);
   
   alreadyProcessed = alreadyProcessed | processUsingLevelB;
   if all(alreadyProcessed(:))
      break;
   end
end
f(~alreadyProcessed) = zmed(~alreadyProcessed);
end
