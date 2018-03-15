% 太阳轨道的纬度幅角
function u = sunu(Y,M,D,h,m,s)
if nargin == 6
    mjd = date2mjd(Y,M,D,h,m,s);
elseif nargin == 1
    mjd = Y;
else
    error('输入参数数目错误');
end
w = sunw(mjd);
e = sune(mjd);
m = sunm(mjd);
u = mod(w + m + 2*e*sin(m) + 1.25*e^2*sin(2*m),2*pi);
