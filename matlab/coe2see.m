% 经典轨道根数==>小偏心率根数
% a e i O w M ==> a ksi eta i O lambda
function ke = coe2see(k)
ke(:,1) = k(:,1);
ke(:,2) = k(:,2).*cos(k(:,5));
ke(:,3) = k(:,2).*sin(k(:,5));
ke(:,4) = k(:,3);
ke(:,5) = k(:,4);
ke(:,6) = mod(k(:,5) + k(:,6) + 4*pi,2*pi);