% 地固系位置速度转到惯性系
function [ri,vi] = rv2k(c,rf,vf)
we = [0;0;We];
ri = c'*rf;
vi = c'*(vf + cross(we,rf));