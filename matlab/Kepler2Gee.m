% 经典Kepler根数转化为同步轨道根数
function [ ge ] = Kepler2Gee( k )
[m,n] = size(k);
if n==6
    seq = 2;
elseif m==6
    seq = 1;
else
    error('input must be m*6 or 6*n matrix');
end

if seq == 1
    k = k.';
end
ge = [k(:,1)-42164.2   k(:,2).*cos(k(:,4)+k(:,5))   k(:,2).*sin(k(:,4)+k(:,5)) ...
    k(:,3).*cos(k(:,4))   k(:,3).*sin(k(:,4))   mod(k(:,4)+k(:,5)+k(:,6),2*pi)];
if seq == 1
    ge = ge.';
end

end

