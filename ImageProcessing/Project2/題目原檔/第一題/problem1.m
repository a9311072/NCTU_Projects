clear all;

A = load('problem1.txt')
%A=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15];
Fa=fft(A); %檢查用
IFa=ifft(Fa)
L=length(A);

%先對序列做排序
inx=swap_index(L);
for i=1:L
    X(i)=A(inx(i)+1);
end

K=0;
index=1;
for d=1:log2(L) %d代表流程數 
    for e=1:L/(2^d) %每流程的對稱點數
        for g=1:2^(d-1) %上群
            W(index)=X(index)+exp((-1i*2*pi*K)/L)*X(index+(2^(d-1)));
            index=index+1;
            K=K+(L/(2^d));
        end
        for g=1:2^(d-1) %下群
            W(index)=X(index-(2^(d-1)))+exp((-1i*2*pi*K)/L)*X(index);
            index=index+1;
            K=K+(L/(2^d));
        end
        K=0;
    end
    for t=1:L %更新
        X(t)=W(t);
    end
    index=1;
end

function [ inx ] = swap_index( L )
    dep=log2(L);
    for i=0:L-1
        temp=dec2bin(i,dep)-48;
        a=0;
        for j=dep-1:-1:0
            a=a+((2^j)*temp(j+1));
        end
        inx(i+1)=a;    
    end
end