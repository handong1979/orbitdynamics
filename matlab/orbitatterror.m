% 计算两组轨道根数的轨道坐标系的偏差
% 认为是小角度偏差，与转序无关
function err = orbitatterror(k1,k2)
if size(k1)~=size(k2)
    error('size must be same');
end
if size(k1,2)~=6 || size(k2,2)~=6
    error('k1 or k2 is not 6 column matrix');
end
m = size(k1,1);
err = nan(m,3);
for i=1:m
    coi1 = getcoi(k1(i,:));
    coi2 = getcoi(k2(i,:));
    ce = coi1*coi2';
    err(i,1) = ce(2,3)*deg;
    err(i,2) = ce(3,1)*deg;
    err(i,3) = ce(1,2)*deg;
end
