% 经典轨道根数==>静止轨道根数
% a e i O w M ==> a ex ey ix iy lambda
function ke = coe2goe(k)
ke(:,1) = k(:,1);
ke(:,2) = k(:,2).*cos(k(:,4)+k(:,5));
ke(:,3) = k(:,2).*sin(k(:,4)+k(:,5));
ke(:,4) = sin(k(:,3)).*cos(k(:,4));
ke(:,5) = sin(k(:,3)).*sin(k(:,4));
ke(:,6) = mod(k(:,4) + k(:,5) + k(:,6) + 6*pi,2*pi);