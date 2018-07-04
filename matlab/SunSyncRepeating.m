% 太阳同步回归轨道方程
function f = SunSyncRepeating(x)
a = x(1);
i = x(2);
R = 44;
N = 3;
n = sqrt(GEarth/a^3);
TN = 2*pi/n*(1-1.5*J2*(Re/a)^2*(3-2.5*sin(i)*sin(i)));
dotOmega = -1.5*n*J2*(Re/a)^2*cos(i);
% 回归轨道的要求，方程的值为弧度
f(1) = R*TN*(We + dotOmega) - N*2*pi;
% 太阳同步轨道的要求，轨道面每天进动0.9856度，方程的值为弧度
f(2) = dotOmega*86164.09 - 0.9856*rad; 
% f = f*deg*100;