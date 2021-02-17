% 黄道近地点幅角ws
function w = sunw(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = Y;
else
    error('输入参数数目错误');
end
dt = (TDT-mjd2000)/36525.0;
w = mod((282.9373472 + 0.32256206*dt + 1.51e-4*dt^2)*rad,2*pi);