% 绕Z轴旋转矩阵
function R = rotz(angle)
if isscalar(angle)
    R = [ cos(angle)  sin(angle)  0;
         -sin(angle)  cos(angle)  0;
              0            0      1];
elseif isvector(angle)
    n = length(angle);
    angle = reshape(angle,1,1,n);
    R = [ cos(angle)    sin(angle)     zeros(1,1,n);
         -sin(angle)    cos(angle)     zeros(1,1,n);
          zeros(1,1,n)  zeros(1,1,n)   ones(1,1,n)];
else
    error('输入必须为标量或矢量');
end