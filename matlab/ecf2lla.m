function [ lla ] = ecf2lla( ecf )
%ecf2lla 地固系位置转化为地心经纬度
%   Detailed explanation goes here
[m,n] = size(ecf);
if n==3 || n==6
    seq = 2;
elseif m==3 || m==6
    seq = 1;
else
    error('input must be m*3 3*n m*6 6*n matrix');
end

if seq == 1
    ecf = ecf.';
    ecf = ecf(:,1:3);
else
    ecf = ecf(:,1:3);
end
r = sqrt(sum(ecf.^2,2));
lla = [atan2(ecf(:,2),ecf(:,1)) asin(ecf(:,3)./r) r-Re];

if seq == 1
    lla = lla.';
end

end

