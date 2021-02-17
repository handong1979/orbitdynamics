% 轨道坐标系相对地心惯性坐标系的旋转矩阵
function inertia = symcoi()
syms u I RAAN
sinu = sin(u);
cosu = cos(u);
sini = sin(I);
cosi = cos(I);
sino = sin(RAAN);
coso = cos(RAAN);
	
R = [0,1,0;0,0,-1;-1,0,0];
Rzu = [cosu,sinu,0;-sinu,cosu,0;0,0,1];
Rxi = [1,0,0;0,cosi,sini;0,-sini,cosi];
Rzo = [coso,sino,0;-sino,coso,0;0,0,1];
	
inertia = R*Rzu*Rxi*Rzo;