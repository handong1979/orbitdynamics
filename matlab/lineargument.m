% 将限幅到0～2pi的幅角（存在从2pi跳变到0的点）调整成一条连续变化的幅角
function u = lineargument(u)
if ~isvector(u)
    error('input must be a vector');
end
% 跳变点的位置
jumppoint = find(abs(diff(u))>0.1)+1;
% 跳变点的个数
njpoint = length(jumppoint);
% 从后到前，依次加上2pi
for i=njpoint:-1:1
    u(jumppoint(i):end) = u(jumppoint(i):end) + 2*pi;
end