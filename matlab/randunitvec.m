% 产生一个随机单位矢量
function r = randunitvec()
a = (rand-0.5)*360;
b = (rand-0.5)*90;
r = [cosd(b)*cosd(a);cosd(b)*sind(a);sind(b)];