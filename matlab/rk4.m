% Áú¸ñ¿âËþ4½×»ý·Ö
function xk = rk4(dynfunc,step,x0)
k1 = step*dynfunc(x0);
k2 = step*dynfunc(x0+0.5*k1);
k3 = step*dynfunc(x0+0.5*k2);
k4 = step*dynfunc(x0+k3);
xk = x0 + 1/6*(k1+2*k2+2*k3+k4);