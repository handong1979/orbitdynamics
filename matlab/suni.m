% 黄赤交角
function i = suni(Y,M,D,h,m,s)
if nargin == 6
    TDT = utc2tdt(date2mjd(Y,M,D,h,m,s));
elseif nargin == 1
    TDT = Y;
else
    error('输入参数数目错误');
end
dt = (TDT-mjd2000)/36525.0;
i = (23.4392911 - 0.013004167*dt - 1.64e-7*dt^2)*rad;