% 计算位置速度矢量对轨道六根数的偏导数矩阵
% 轨道六根数定义为: a  e  i  Omega  w  M
% jacobian = pdmcoe(kp)
% pdmcoe : 自我测试
% jacobian = pdmcoe(kp) : kp为小偏心率轨道六根数，返回6*6雅克比矩阵
function jacobian = pdmcoe(kp)
%% selftest
if nargin == 0 
    warning('pdmcoe self test');
    a = 42164;
    e = 0.0001;
    w = 2;
    i = 2.2*rad;
    Omega = 10;
    kp = [a,e,i,Omega,w,0];
    for M=0:360
        kp(6) = M*rad;
        j = pdmcoe(kp);
        jm = nan(6,6); % 数值差分计算雅克比矩阵
        eps = 1e-9;  % 差分小量
        [rvector0, vvector0] = kepler2cart(kp);
        for k=1:6;
            kpt = kp;
            kpt(k) = kpt(k)+eps;
            [rvector1, vvector1] = kepler2cart(kpt);
            jm(:,k) = [(rvector1-rvector0)/eps, (vvector1-vvector0)/eps];
        end
%         disp(norm(jm-j));
        % jm和j的比较
        if norm(jm-j) > 0.1
            kp
            disp('数值计算结果:');disp(jm);
            disp('解析计算结果:');disp(j);
            disp('两者之差:');disp(j-jm);
            error('pdmcoe:selftest 雅克比矩阵对比失败');
        end
    end
    disp('self test passed');
    return
end
%% 基本变量赋值
mu = GEarth;
a = kp(1);
e = kp(2);
i = kp(3);
% Omega = kp(4);
w = kp(5);
M = kp(6);
E = keplerfunc(e,M);
n0 = sqrt(mu/a^3);
r = a*(1-e*cos(E));
%% 按照刘林《航天器轨道理论》中的公式计算
[rvector, vvector] = kepler2cart(kp);
rvector = rvector(:);
vvector = vvector(:);
% 位置矢量对轨道根数偏导数部分
prpa = rvector/a;
p = a*(1-e^2);
H = -a/p*(cos(E)+e);
K = sin(E)/n0*(1+r/p);
prpe = H*rvector + K*vvector;
prpM = 1/n0*vvector;
R = cross(rvector,vvector)/sqrt(mu*p);
prpi = rvector(3)/sin(i)*R;
prpo = [-rvector(2);rvector(1);0];
prpw = cross(R,rvector);
% 速度矢量对轨道根数偏导数部分
pvpa = -1/2/a*vvector;
Hp = sqrt(mu*a)*sin(E)/r/p*(1-a/r*(1+p/r));
Kp = a/p*cos(E);
pvpe = Hp*rvector + Kp*vvector;
pvpM = -mu/n0/r^3*rvector;
pvpi = vvector(3)/sin(i)*R;
pvpo = [-vvector(2);vvector(1);0];
pvpw = cross(R,vvector);
jacobian = [prpa, prpe, prpi, prpo, prpw, prpM;...
    pvpa, pvpe, pvpi, pvpo, pvpw, pvpM];