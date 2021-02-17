% J2000.0�¹������ת����TOD�¹������
function ktod = claJ2ktod(epoch,kj2k)
if nargin == 0
    epoch = date2mjd([2019 1 1 21 0 0]);
    kj2k = krad([42166.15 0.0001 0.133 90.0 0.0 339.651]);
end
cfi = nutation(epoch)*precession(epoch);
[r,v] = kepler2cart(kj2k);
r = cfi*r';
v = cfi*v';
ktod = cart2kepler([r;v]);
if nargin == 0
    ktod = kdeg(ktod');
end
