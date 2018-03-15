% 由速度增量计算燃料消耗
% dm = dvdm(dv,m0,I)
% 根据火箭方程计算：dm = m0.*(1-exp(-dv/I/9.8));
% dv: 速度增量m/s
% m0: 初始总质量kg
% I: 比冲s
function dm = dvdm(dv,m0,I)
dm = m0.*(1-exp(-abs(dv)/I/9.8));