% 有轮刚体动力学
function [q,w,hw] = rigidwheel(t,q,w,hw,Tw,Tj,Is,iIs,Cbwh,dt)
%% self test
if nargin == 0 
    Is = [3360.72   -26.19	10.31;...
        -26.19	3222.31     35.32;...
        10.31    35.32   2420.90];
    iIs = inv(Is);
    
    alfa = 45*rad;
    beta = 54.74*rad;
    sa = sin(alfa);  ca = cos(alfa);
    sb = sin(beta);  cb = cos(beta);
    Cbwh = [sb*ca, sb*sa, -sb*ca, -sb*sa;...
        cb,    cb,      cb,    cb;...
      -sb*sa, sb*ca,  sb*sa, -sb*ca];
  
    q = [0;0;0;1];
    w = [1;0.0;0.0]*rad;
    hw = [0;0;0;0];
  
    Tw = [0.1;0;0.1;0];
    Tj = [20;20;20];
    t = 0;
    [q,w,hw] = rigidwheel(t,q,w,hw,Tw,Tj,Is,iIs,Cbwh,1)
    return
end
%%
y = [q;w;hw];
Y = rk4(@(y)odeRigidWheel(t,y),dt,y); % Solve ODE
q = Y(1:4);
w = Y(5:7);
hw = Y(8:11);

%% 有轮刚体动力学方程
% y包括四元数、角速度、各动量轮角动量
% 使用MATLAB ODE函数时，调用格式为：
% [T, Y] = ode45(@(t,y)RigidWheel(t,y,Tw,Tj,Is,iIs,Cbwh),Tspan,y,odeopt);
% 使用rk4函数时，没有时间量输入，调用格式为：
% rk4(@(y)RigidWheel(t,y,Tw,Tj,Is,iIs,Cbwh),dt,y); % Solve ODE
    function dy = odeRigidWheel(t,y)      
        Omega = Ew(y(5:7));
        dq = 0.5*Omega*y(1:4);
        
        % dw = Is\(- Cbwh*Tw + Omega(1:3,1:3)*(Is*w+Cbwh*hw));
        dw = iIs*(- Cbwh*Tw + Tj + Omega(1:3,1:3)*(Is*y(5:7)+Cbwh*y(8:11)));
        
        dy = [dq;dw;Tw];
    end
%% 龙格库塔4阶积分
    function xk = rk4(dynfunc,step,x0)
        k1 = step*dynfunc(x0);
        k2 = step*dynfunc(x0+0.5*k1);
        k3 = step*dynfunc(x0+0.5*k2);
        k4 = step*dynfunc(x0+k3);
        xk = x0 + 1/6*(k1+2*k2+2*k3+k4);
    end
%% Ew
    function Omega = Ew(w)
        Omega = [ 0     w(3)   -w(2)   w(1);...
            -w(3)   0      w(1)   w(2);...
            w(2)   -w(1)    0     w(3);...
            -w(1)  -w(2)  -w(3)    0];
    end
end
