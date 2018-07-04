% 计算大气阻力
% f = dragforce(A,rou,v)
function f = dragforce(A,rou,v)
    Cd = 2.2;
    f = Cd/2*A*rou*v^2;
end