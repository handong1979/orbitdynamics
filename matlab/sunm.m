% 太阳平近点角Ms
function m = sunm(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = Y;
else
    error('输入参数数目错误');
end
dt = (TDT-mjd2000)/36525.0;
m = mod((357.5291 + 129596581.04/3600.0*dt - 0.562/3600.0*dt^2)*rad,2*pi);