% 绘制CW相对运动轨迹
function hf = plotcwtraj(rv,w,T,type)
if nargin == 3
    type = 'xz';
end
h = 60;
[traj,t] = cwtraj(rv,w,T,h);
plotreltraj(traj,t,type);
  