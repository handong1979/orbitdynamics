% 太阳平均偏心率es
function e = sune(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = Y;
else
    error('输入参数数目错误');
end
dt = (TDT-mjd2000)/36525.0;
e = 0.01670862 - 4.204e-5*dt - 1.24e-6*dt^2;