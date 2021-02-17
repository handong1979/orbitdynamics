% J2000.0下轨道参数转换到TOD下轨道参数
function kj2k = clatod2j2k(epoch,ktod)
if nargin == 0
    epoch = date2mjd([2019 1 1 21 0 0]);
    ktod = krad([42166.15 0.0001 0.133 90.0 0.0 339.651]);
end
cpn = nutation(epoch)*precession(epoch);
[r,v] = kepler2cart(ktod);
r = cpn'*r';
v = cpn'*v';
kj2k = cart2kepler([r',v']);
if nargin == 0
    kj2k = kdeg(kj2k');
end
