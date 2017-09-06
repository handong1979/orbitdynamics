%  由轨道周期计算轨道半长轴
%  See also a2t, h2n.
function a = t2a(t)
a = (GEarth./(2*pi./t).^2).^(1/3);