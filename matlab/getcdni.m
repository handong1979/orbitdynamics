% 东南坐标系相对惯性系的旋转矩阵
function cdni = getcdni(k0)
% [r v] = kepler2cart(k0);
% alpha = atan2(r(2),r(1));
% beta = asin(r(3)/norm(r));
f = ma2ta(k0(2),k0(6));
u = k0(5) + f;
if cos(u)>0
    L1 = atan(tan(u)*cos(k0(3)));
else
    L1 = atan(tan(u)*cos(k0(3))) + pi;
end
alpha = k0(4) + L1;
beta = atan(sin(L1)*tan(k0(3)));
cdni = [0 1 0; 0 0 -1; -1 0 0]*roty(-beta)*rotz(alpha);