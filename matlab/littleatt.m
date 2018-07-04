% 小角度姿态矩阵--符号运算
function c = littleatt()
phi = sym('phi');
theta = sym('theta');
psi = sym('psi');
c = [1,psi,-theta;...
    -psi,1,phi;...
    theta,-phi,1];