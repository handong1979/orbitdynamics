% 求矢量的模
function r = vecnorm(p)
[m n] = size(p);
if m==3
    i = 1;
elseif n==3
    i = 2;
else
    error('输入p的行或列需要有一个为3');
end
r = sqrt(sum(p(:,1:3).^2,i));