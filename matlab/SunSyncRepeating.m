% 太阳同步回归轨道方程
function [a,i] = SunSyncRepeating(rev,day)
if nargin==0
    rev = 44;
    day = 3;
end
n0 = 2*pi/(day*86164.09/rev);
a0 = (GEarth/n0^2)^(1/3);
i0 = acos(0.9856*rad/86164.09/(-1.5*n0*J2*(Re/a0)^2));
x = fsolve(@(x)ssr_constrain(x),[a0,i0]);
a = x(1);
i = x(2);

    function f = ssr_constrain(x)
        a = x(1);
        i = x(2);
        n = sqrt(GEarth/a^3);
        TN = 2*pi/n*(1-1.5*J2*(Re/a)^2*(3-2.5*sin(i)*sin(i)));
        dotOmega = -1.5*n*J2*(Re/a)^2*cos(i);
        % 回归轨道的要求，方程的值为弧度
        f(1) = rev*TN*(We - dotOmega) - day*2*pi;
        % 太阳同步轨道的要求，轨道面每天进动0.9856度，方程的值为弧度
        f(2) = dotOmega*86164.09 - 0.9856*rad;
        % f = f*deg*100;
    end
end