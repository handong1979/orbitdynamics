% �볤���Сƫ�����ÿ��������ڵ�λ��Ư����(km)
function dr = drda(a,da)
n = sqrt(GEarth/a^3);
dn = -1.5*da/a*n;
du = dn*2*pi/n;
dr = du*a;