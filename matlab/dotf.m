% 由输入的开普勒根数(a,e,i,Omega,w,M)计算真近点角f的变化率和角加速度
function [df,ddf] = dotf(k)
e = k(2);
a = k(1);
p = a*(1-e^2);
f = ma2ta(e,k(6));
r = p/(1+e*cos(f));
df = sqrt(GEarth*p)/r^2;
rdot = sqrt(GEarth/a/(1-e^2))*e*sin(f);
if nargout > 1
    ddf = -2*rdot.*df./r;
end