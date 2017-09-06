% 半长轴的小偏差造成每个轨道周期的相位漂移量(rad)
function du = duda(a,da)
n = sqrt(GEarth/a^3);
dn = -1.5*da/a*n;
du = dn*2*pi/n;