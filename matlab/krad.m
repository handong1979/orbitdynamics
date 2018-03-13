% 将轨道根数中的弧度转化为度
function k = krad(ki)
[m n] = size(ki);
if n~=6
   error('输入必须为n*6的矩阵'); 
end
k = ki;
k(:,3:6) = k(:,3:6)*rad;