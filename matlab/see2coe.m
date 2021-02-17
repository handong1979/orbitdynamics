% 小偏心率根数==>经典轨道根数
%  a ksi eta i O lambda ==> a e i O w M
function ke = see2coe(k)
ke(:,1) = k(:,1);
ke(:,2) = sqrt(k(:,2).^2+k(:,3).^2);
ke(:,3) = k(:,4);
ke(:,4) = k(:,5);
ke(:,5) = atan2(k(:,3),k(:,2));
ke(:,6) = mod(k(:,6) - ke(:,5) + 4*pi,2*pi);