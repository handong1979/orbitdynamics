% 绕Y轴旋转矩阵
function R = roty(angle)
if isscalar(angle)
    R = [cos(angle)  0    -sin(angle);
              0      1         0;
         sin(angle)  0     cos(angle)];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [cos(angle)    zeros(1,1,n)    -sin(angle);
         zeros(1,1,n)  ones(1,1,n)      zeros(1,1,n);
         sin(angle)    zeros(1,1,n)     cos(angle)];
else
    error('输入必须为标量或矢量');
end