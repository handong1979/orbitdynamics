% 将度分秒表示的角度转换为度
% y = dfm2deg(d,f,m)
% example:
%     y = dfm2deg(12,15,19)
%     y = dfm2deg(-12,23,55)
%     y = dfm2deg([12,15,19]);
%     y = dfm2deg([-12,15,19]);
function y = dfm2deg(d,f,m)
if nargin == 3
    if f>60 || f<0 || m>60 || m<0
        error('dfm2deg:输入的分、秒应在0~50之内');
    end
    if d>=0
        y = d + f/60 + m/3600;
    else
        y = d - f/60 - m/3600;
    end
elseif nargin == 1
    if d(1)>0
        y = d(1) + d(2)/60 + d(3)/3600;
    else
        y = d(1) - d(2)/60 - d(3)/3600;
    end
else
    error('dfm2deg:输入变量个数错误');
end