function [ alphas ] = sunra( mjd )
%sunra ¼ÆËãÌ«Ñô³à¾­
%   Detailed explanation goes here
us = sunu(mjd);
rs = rotx(-23.4*rad)*[cos(us);sin(us);0];
alphas = atan2(rs(2),rs(1));

end

