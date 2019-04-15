function [ cla ] = eqt2cla( eqt )
%EQT2CLA 将转小偏心率根数化为经典轨道根数
%   经典轨道根数为[a;e;i;Omega;w;M]
%   小偏心率根数为[a;ex;ey;i;Omega;lamda]
cla(1) = eqt(1);
cla(2) = norm(eqt(2:3));
cla(3) = eqt(4);
cla(4) = eqt(5);
cla(5) = atan2(eqt(3),eqt(2));
cla(6) = eqt(6) - cla(5);

end

