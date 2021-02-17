% 根据速度增量计算开机时间和燃料消耗
% [T,dm] = fireduration(dv,F,mass0,Isp)
%  dv: 速度增量,单位m/s
%   F: 发动机推力，单位N
%  mass0:初始质量，单位kg
%  Isp：发动机比冲，单位s
%  输出：
%     T: 开机时长，单位s
%     dm: 燃料消耗，单位kg
function [T,dm] = fireduration(dv,F,mass0,Isp)
mdot = F/Isp/9.8;
dm = mass0*(1-exp(-dv/Isp/9.8));
T = dm/mdot;