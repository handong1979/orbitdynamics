% TH-Yamanaka÷∆µº
function [dv0,dvf,T] = TH2p(tm,x0,xf,tf)
if nargin == 0
    warning('self test');
    tm = krad([24671,0.71,28,0,180,180]);
    x0 = [0;0;0;0;0;0];
    xf = [50;0;0;0;0;0];
    tf = 5*3600;
    [dv0,dvf,T] = TH2p(tm,x0,xf,tf)
    dvall = sum(abs([dv0;dvf]))*1000
    return
end

theta0 = ma2ta(tm(2),tm(6));
thetaf = ma2ta(tm(2),tm(6) + sqrt(GEarth/tm(1)^3)*tf);
e = tm(2);
T = THstm(tf,0,theta0,thetaf,tm(1),e);
v0 = T(1:3,4:6)\(xf(1:3)-T(1:3,1:3)*x0(1:3));
dv0 = v0 - x0(4:6);
vf = T(4:6,:)*[x0(1:3);v0];
dvf = xf(4:6) - vf;