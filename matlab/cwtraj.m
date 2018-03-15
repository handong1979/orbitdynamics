% 根据初始状态计算CW轨迹
function traj = cwtraj(x0,w,T,h)
if ~isvector(x0)
    error('输入初始状态x0必须为矢量');
end
[m,n] = size(x0);
dim = max(m,n);
if ~(dim==4 || dim==6)
    error('输入初始状态必须为4维或6维矢量');
end
t = 0:h:T;
if t(end)~=T
    t = [t,T];
end
len = length(t);
traj = nan(dim,len);
if m==1
    x0 = x0'; %转成列矢量
end
for i=1:len
    if dim==4
        phi = cwstm4(w,t(i));
    else
        phi = cwstm6(w,t(i));
    end
    traj(:,i) = phi*x0;
end
traj = traj';
