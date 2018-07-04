function u = ewmu(e,w,m)
% 由e,w和M计算轨道纬度幅角u
f = ma2ta(e,m);
u = mod(w+f,2*pi);