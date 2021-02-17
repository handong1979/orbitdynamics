% TH方程的Yamanaka状态转移矩阵
function T = THstm(tf,t0,theta0,theta,a,e)
if nargin == 0
    warning('self test');
    a = 7000;
    e = 0.0;
    t0 = 0;
    tf = 2000;
    theta0 = 0;
    theta = ma2ta(e,sqrt(GEarth/a^3)*tf+theta0);
    disp('TH方程的Yamanaka状态转移矩阵:');
    disp(cwstm6(sqrt(GEarth/a^3),tf-t0));
end
h = sqrt(GEarth*a*(1-e^2));
k = GEarth/h^1.5;
rou = 1 + e*cos(theta);
rou0 = 1 + e*cos(theta0);
s = rou*sin(theta);
s0 = rou0*sin(theta0);
c = rou*cos(theta);
c0 = rou0*cos(theta0);
sp = cos(theta)+e*cos(2*theta);
cp = -(sin(theta)+e*sin(2*theta));
J = k^2*(tf-t0);

PHI0 = phi(rou0,k,e,theta0);
PHIi = phiinv(rou, k,e,theta);
M = zeros(6,6);

% Yamanaka
C = zeros(6,6);
C(1,1) = 1;
C(1,3) = 3*e*s0*(1/rou0 + 1/rou0^2)/(1-e^2);
C(1,4) = -e*s0*(1 + 1/rou0)/(1-e^2);
C(1,6) = (-e*c0 + 2)/(1-e^2);
C(2,2) = cos(theta - theta0);
C(2,5) = sin(theta - theta0);
C(3,3) = -3*s0*(1/rou0 + e^2/rou0^2)/(1-e^2);
C(3,4) = s0*(1+1/rou0)/(1-e^2);
C(3,6) = (c0 - 2*e)/(1-e^2);
C(4,3) = -3*(c0/rou0+e)/(1-e^2);
C(4,4) = (c0*(1+1/rou0) + e)/(1-e^2);
C(4,6) = -s0/(1-e^2);
C(5,2) = -sin(theta - theta0);
C(5,5) =  cos(theta-theta0);
C(6,3) = (3*rou0 + e^2 - 1)/(1-e^2);
C(6,4) = -rou0^2/(1-e^2);
C(6,6) = e*s0/(1-e^2);
C = C;

A = zeros(6,6);
A(1,1) = 1;
A(1,3) = -c*(1+1/rou);
A(1,4) = s*(1+1/rou);
A(1,6) = 3*rou^2*J;
A(2,2) = 1;
A(3,3) = s;
A(3,4) = c;
A(3,6) = 2 - 3*e*s*J;
A(4,3) = 2*s;
A(4,4) = 2*c-e;
A(4,6) = 3*(1-2*e*s*J);
A(5,5) = 1;
A(6,3) = sp;
A(6,4) = cp;
A(6,6) = -3*e*(sp*J+s/rou^2);
M = A*C;

% 尤政 硕士论文 P17~P18
% M(1,1) = 1;
% M(1,3) = ( 3*e*s0*(1/rou0+1/rou0^2) + 3*s0*c*(1+1/rou)*(1/rou0+e^2/rou0^2) ...
%     - 3*s*(c0/rou0+e)*(1+1/rou) + 3*rou^2*J*(3*rou0+e^2-1) ) / (1-e^2);
% M(1,4) = ( -e*s0*(1+1/rou0) - s0*c*(1+1/rou0)*(1+1/rou) ...
%     + s*(1+1/rou)*(c*(1+1/rou0)+e) - 3*rou0^2*rou^2*J ) / (1-e^2);
% M(1,6) = ( 2 - e*c0 - c*(c0-2*e)*(1+1/rou) - s0*s*(1+1/rou) + 3*e*s0*rou^2*J ) / (1-e^2);
% M(2,2) = cos(theta - theta0);
% M(2,5) = sin(theta - theta0);
% M(3,3) = ( -3*s0*s*(1/rou0 + e^2/rou0^2) - 3*c*(c0/rou0 + e) + ...
%     (3*rou0 + e^2 -1)*(2 - 3*e*s*J) ) / (1-e^2);
% M(3,4) = ( s0*s*(1+1/rou0) + c*(c0*(1+1/rou0) + e) - rou0^2*(2-3*e*s*J) ) / (1-e^2);
% M(3,6) = ( s*(c0-2*e) - s0*c + e*s0*(2-3*e*s*J) ) / (1-e^2);
% M(4,3) = ( -6*s0*s*(1/rou0+e^2/rou0^2) - 3*(c0/rou0+e)*(2*c-e) ...
%     + 3*(3*rou0 + e^2 -1)*(1-2*e*s*J) ) / (1-e^2);
% M(4,4) = ( 2*s0*s*(1+1/rou0) + (c0*(1+1/rou0)+e)*(2*c-e) - 3*rou0^2*(1-2*e*s*J) ) / (1-e^2);
% M(4,6) = ( 2*s*(c0-2*e) - s0*(2*c-e) + 3*e*s0*(1-2*e*s*J) ) / (1-e^2);
% M(5,2) = -sin(theta-theta0);
% M(5,5) =  cos(theta-theta0);
% M(6,3) = ( -3*s0*sp*(1/rou0+e^2/rou0^2) - 3*cp*(c0/rou0+e) ...
%     - 3*e*(sp*J+s/rou^2)*(3*rou0+e^2-1) ) / (1-e^2);
% M(6,4) = ( s0*sp*(1+1/rou0) + cp*(c0*(1+1/rou0)+e) + 3*rou0^2*e*(sp*J + s/rou^2) ) / (1-e^2);
% M(6,6) = ( sp*(c0-2*e) - s0*cp - 3*e^2*s0*(sp*J + s/rou^2) ) / (1-e^2);

T = PHIi*M*PHI0;

% 相对位置速度 到 TH变量 的映射矩阵
% r_TH = (1+e*cos(theta)) * r  = rou * r
% v_TH = -e*sin(theta) * r + 1/k^2/rou * v
function PH = phi(rou,k,e,theta)
PH = zeros(6,6);
PH(1,1) = rou;
PH(2,2) = rou;
PH(3,3) = rou;
k2 = k^2;
PH(4,4) = 1/(k2*rou);
PH(5,5) = 1/(k2*rou);
PH(6,6) = 1/(k2*rou);
PH(4,1) = -e*sin(theta);
PH(5,2) = -e*sin(theta);
PH(6,3) = -e*sin(theta);

% TH变量 到 相对位置速度 的映射矩阵
% r = 1/(1+e*cos(theta)) * r_TH = 1/rou * r_TH
% v = k^2*(e*sin(theta) * r_TH + rou * v_TH)
function PH = phiinv(rou,k,e,theta)
PH = zeros(6,6);
PH(1,1) = 1/rou;
PH(2,2) = 1/rou;
PH(3,3) = 1/rou;
k2 = k^2;
PH(4,4) = k2*rou;
PH(5,5) = k2*rou;
PH(6,6) = k2*rou;
PH(4,1) = k2*e*sin(theta);
PH(5,2) = k2*e*sin(theta);
PH(6,3) = k2*e*sin(theta);
