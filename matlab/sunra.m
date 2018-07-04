function [ alphas ] = sunra( mjd )
%sunra 计算太阳赤经
%   Detailed explanation goes here
us = sunu(mjd);
rs = rotx(-23.4*rad)*[cos(us);sin(us);0];
alphas = atan2(rs(2),rs(1));

end

