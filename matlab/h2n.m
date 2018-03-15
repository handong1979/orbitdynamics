% 由轨道高度计算轨道平均角速度
%  See also a2t, t2a.
function n = h2n(h)
n = sqrt(GEarth./(Re+h).^3);