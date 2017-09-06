function M=fm(e,f)
E = mod(atan2(sin(f)/sqrt(1-e*e),cos(f)+e)+2*pi,2*pi);
M = E - e*sin(E);