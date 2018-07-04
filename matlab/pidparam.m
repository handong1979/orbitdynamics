% 计算二阶系统的PID控制参数
% [kp ki kd] = pidparam(I,wn,ksi,tao)
% I：转动惯量
% wn: 系统带宽
% ksi: 系统阻尼
% tao：积分时间常数,不输入的情况下为10/(ksi*wn)
function [kp,ki,kd] = pidparam(I,wn,ksi,tao)
if nargin == 0  %test
    I = 2.33;%1.98;%1.81;%
    ksi = 0.707;%ksia(i);
    wn = 0.07;
    TT = 10/(ksi*wn);
else
    if nargin == 3
        TT = 10/(ksi*wn);
    else
        TT = tao;
    end
    kp = I*(wn^2 + 2*ksi*wn/TT);
    kd = I*(2*ksi*wn+1/TT);
    ki = I*wn^2/TT;
end