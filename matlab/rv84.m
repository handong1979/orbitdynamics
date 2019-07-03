% 位置速度转到地固系
function [rf,vf] = rv84(c,ri,vi)
we = [0;0;We];
rf = c*ri;
vf = c*vi - cross(we,rf);
