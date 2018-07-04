% 由轨道六根数计算轨道幅角u
function u = kepleru(kepler)
EE = keplerfunc(kepler(2),kepler(6));
cosf = (cos(EE)-kepler(2)) / (1-kepler(2)*cos(EE));
sinf = sqrt(1-kepler(2)^2)*sin(EE)/(1-kepler(2)*cos(EE));
sinu = sinf*cos(kepler(5)) + cosf*sin(kepler(5));
cosu = cosf*cos(kepler(5)) - sinf*sin(kepler(5));
u = atan2(sinu,cosu);