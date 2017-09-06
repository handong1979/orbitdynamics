% 动量轮角动量包络
function [hx,hy,hz,h] = wheelenvelope(Cwh,hmax)
if nargin == 0
    warning('self test');
    hmax = 50;
    alfa = 45*rad;
    beta = 54.74*rad;
    sa = sin(alfa);  ca = cos(alfa);
    sb = sin(beta);  cb = cos(beta);
    Cwh = [sb*ca, -sb*sa, sb*ca, -sb*sa;...
          -cb,    -cb,      cb,    cb;...
           sb*sa, sb*ca,  sb*sa, sb*ca];
%     Cwh = [1 0  0;
%         0 1 0;
%         0 0 1;
%          sqrt(3)/3  sqrt(3)/3  sqrt(3)/3]';
end

Dwh = Cwh'*inv(Cwh*Cwh');
fac = 60;
[x,y,z] = sphere(fac);
[hx,hy,hz] = sphere(fac);
h = nan(size(hx));
vmax = 0;
for i=1:fac+1
    for j=1:fac+1
        v = [x(i,j),y(i,j),z(i,j)]'; % 方向
        hp = Dwh*v; % 分配
        sf = hmax/max(abs(hp)); % 放大因子
        hp = sf*hp; % 放大得到每个轮子的角动量
        h(i,j) = dot(v,Cwh*hp); % 模值
        hx(i,j)=h(i,j)*x(i,j); % 分量x
        hy(i,j)=h(i,j)*y(i,j); % 分量y
        hz(i,j)=h(i,j)*z(i,j); % 分量z
    end
end

if nargin==0
%     figure;
    surfc(hx,hy,hz,h,'edgealpha',0.2,'facealpha',0.5);
    grid on,hold on,xlabel('hx'),ylabel('hy'),zlabel('hz');
    axis equal;
end