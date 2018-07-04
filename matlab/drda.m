% 半长轴的小偏差造成每个轨道周期的位置漂移量(km)
function dr = drda(a,da)
n = sqrt(GEarth/a^3);
dn = -1.5*da/a*n;
du = dn*2*pi/n;
dr = du*a;