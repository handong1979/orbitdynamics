% 半长轴的小偏差造成轨道角速度的变化(rad)
function dn = dnda(a,da)
n = sqrt(GEarth/a^3);
dn = -1.5*da/a*n;