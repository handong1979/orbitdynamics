% 地心引力加速度对位置矢量的雅克比矩阵
%   引力加速度仅考虑中心引力
%   j = jac_dadr(r)
%   输入r为三维位置矢量，得到3*3的雅克比矩阵
%   输入r为六维位置速度矢量，得到6*6的雅克比矩阵，矩阵的值与速度无关
%      (   Miu  [ x ] )
%    d (- ----- [ y ] )
%      (   r^3  [ z ] )
%J = ------------------
%          [ x ]
%        d [ y ]
%          [ z ]
function j = jac_dadr_2body(r)
r = r(:);
m = size(r,1);
if m == 3
    dim = 3;
elseif m == 6;
    dim = 6;
    r = r(1:3);
else
    error('输入矢量维数不对，应为3维位置或者6维位置速度');
end
radius = sqrt(r(1)^2+r(2)^2+r(3)^2);
j = GEarth/radius^3*(3*r*r'/radius^2 - eye(3));
if dim == 6
    j = [zeros(3,3) eye(3);j  zeros(3,3)];
end