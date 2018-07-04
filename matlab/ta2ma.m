%  真近点角f转换为平近点角M
function M = ta2ma(e,f)
cosE = (cos(f)+e)./(1+e*cos(f));
sinE = sqrt(1-e^2)*sin(f)./(1+e*cos(f));
E = atan2(sinE,cosE);
E = mod(E+2*pi,2*pi);
M = E-e*sin(E);
M = mod(M+2*pi,2*pi);