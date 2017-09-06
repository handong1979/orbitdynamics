%由两颗星的轨道根数计算相对位置和相对速度   tcr(telem,celem)
%output: rel[x;y;z;vx;vy;vz]

function rel = tcr(telem,celem)
error(nargchk(2,2,nargin));

txyz = kepler2cart(telem);
cxyz = kepler2cart(celem);

Ct = getcoi(telem);

relinertia = cxyz'-txyz';

%relpos: position vector in orbit coodinate
relpos = Ct*relinertia(1:3);

% target orbit angle rate
w = sqrt(GEarth/telem(1)^3);

%relvel: velocity vector in orbit coodinate (no add-in velocity)
relvel = Ct*relinertia(4:6,1) - cross([0;-w;0],relpos);

rel = [relpos;relvel];