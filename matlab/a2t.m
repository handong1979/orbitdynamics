% 由轨道半长轴计算轨道周期
%  See also t2a, h2n.
function T = a2t(a)
T = 2*pi/sqrt(GEarth/a^3);