%已知目标根数及相对位置速度，求追踪星根数 trc(targetelem,rel)
% targetelem=6*1  rel=6*1

function chaseelem = trc(telem,rel)

error(nargchk(2,2,nargin));

targetxyz = kepler2cart(telem);
Ct = getcoi(telem);

w = sqrt(GEarth/telem(1)^3);

relpos = Ct'*rel(1:3,1);
relvel = rel(4:6,1) + cross( [0;-w;0] , rel(1:3,1) );
relvel = Ct'*relvel;

chasexyz = targetxyz + [relpos;relvel];

chaseelem = cart2kepler(chasexyz);