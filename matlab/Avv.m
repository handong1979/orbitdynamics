% Ë«Ê¸Á¿¶¨×Ë
function A = Avv(X1I,X2I,X1B,X2B)
error(nargchk(4,4,nargin));
V2I = cross(X1I,X2I);
V2I = V2I/norm(V2I);
V3I = cross(X1I,V2I);
V2B = cross(X1B,X2B);
V2B = V2B/norm(V2B);
V3B = cross(X1B,V2B);
A = X1B*X1I'+V2B*V2I'+V3B*V3I';