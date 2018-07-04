% 轨道坐标系相对地心惯性坐标系的旋转矩阵
function inertia = getcoi(elem)
e = elem(2);
i = elem(3);
omiga = elem(4);
w = elem(5);
M = elem(6);
E = keplerfunc(e,M);
cosf =(cos(E)-e) / (1-e*cos(E));
sinf = sqrt(1-e*e)*sin(E)/(1-e*cos(E));
sinu = sinf*cos(w) + cosf*sin(w);
cosu = cosf*cos(w) - sinf*sin(w);
sini = sin(i);
cosi = cos(i);
sino = sin(omiga);
coso = cos(omiga);
	
R = [0,1,0;0,0,-1;-1,0,0];
Rzu = [cosu,sinu,0;-sinu,cosu,0;0,0,1];
Rxi = [1,0,0;0,cosi,sini;0,-sini,cosi];
Rzo = [coso,sino,0;-sino,coso,0;0,0,1];
	
inertia = R*Rzu*Rxi*Rzo;