% ̫��ͬ���ع�������
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
        % �ع�����Ҫ�󣬷��̵�ֵΪ����
        f(1) = rev*TN*(We - dotOmega) - day*2*pi;
        % ̫��ͬ�������Ҫ�󣬹����ÿ�����0.9856�ȣ����̵�ֵΪ����
        f(2) = dotOmega*86164.09 - 0.9856*rad;
        % f = f*deg*100;
    end
end