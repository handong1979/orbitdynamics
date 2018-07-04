% 绕X轴旋转矩阵
function R = rotx(angle)
if isscalar(angle)
    R = [ 1          0         0;
          0    cos(angle)  sin(angle);
          0   -sin(angle)  cos(angle)];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [ ones(1,1,n)     zeros(1,1,n)    zeros(1,1,n);
          zeros(1,1,n)    cos(angle)      sin(angle);
          zeros(1,1,n)   -sin(angle)      cos(angle)];
else
    error('输入必须为标量或矢量');
end
