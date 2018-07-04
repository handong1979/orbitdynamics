% 动量轮角动量包络,最大包络
% See also :
function [hx,hy,hz,h] = wheelenvelopemax(Cwh,hmax)
if nargin == 0
    warning('self test');
    hmax = 50;
    alfa = 45;
    beta = 54.74;
    sa = sind(alfa);  ca = cosd(alfa);
    sb = sind(beta);  cb = cosd(beta);
    Cwh = [sb*ca, -sb*sa, sb*ca, -sb*sa;...
        -cb,    -cb,      cb,    cb;...
        sb*sa, sb*ca,  sb*sa, sb*ca];
%     b2 = 20;
%     y2 = beta;
%     Cwh = [ Cwh, [-cosd(b2)*cosd(y2);sind(y2);sind(b2)*cosd(y2)], ...
%                   [ cosd(b2)*cosd(y2);sind(y2);sind(b2)*cosd(y2)] ];

%     Cwh = [1 0  0;
%         0 1 0;
%         0 0 1;
%         0 1/sqrt(2) 1/sqrt(2)]';
%           sqrt(3)/3  sqrt(3)/3  sqrt(3)/3]';
    
    [hx,hy,hz,h] = wheelenvelopemax(Cwh,hmax);
%     figure;
    surfc(hx,hy,hz,h,'edgealpha',0.2,'facealpha',0.5);
    grid on,hold on,xlabel('hx'),ylabel('hy'),zlabel('hz');
    axis equal;
    colormap([            0            0       0.5625
        0            0      0.57622
        0            0      0.58994
        0            0      0.60366
        0            0      0.61738
        0            0       0.6311
        0            0      0.64482
        0            0      0.65854
        0            0      0.67226
        0            0      0.68598
        0            0       0.6997
        0            0      0.71341
        0            0      0.72713
        0            0      0.74085
        0            0      0.75457
        0            0      0.76829
        0            0      0.78201
        0            0      0.79573
        0            0      0.80945
        0            0      0.82317
        0            0      0.83689
        0            0      0.85061
        0            0      0.86433
        0            0      0.87805
        0            0      0.89177
        0            0      0.90549
        0            0      0.91921
        0            0      0.93293
        0            0      0.94665
        0            0      0.96037
        0            0      0.97409
        0            0       0.9878
        0    0.0015244            1
        0     0.015244            1
        0     0.028963            1
        0     0.042683            1
        0     0.056402            1
        0     0.070122            1
        0     0.083841            1
        0     0.097561            1
        0       0.0625            1
        0      0.21196            1
        0      0.36141            1
        0      0.51087            1
        0      0.66033            1
        0      0.80978            1
        0      0.95924            1
        0.1087            1       0.8913
        0.25815            1      0.74185
        0.40761            1      0.59239
        0.55707            1      0.44293
        0.70652            1      0.29348
        0.85598            1      0.14402
        1      0.99457            0
        1      0.84511            0
        1      0.69565            0
        1       0.5462            0
        1      0.39674            0
        1      0.24728            0
        1     0.097826            0
        0.94837            0            0
        0.79891            0            0
        0.64946            0            0
        0.5            0            0]);
    
    return
end
nw = size(Cwh,2);
Dwh = Cwh'*inv(Cwh*Cwh');
fac = 60;
[x,y,z] = sphere(fac);
[hx,hy,hz] = sphere(fac);
h = nan(size(hx));
% vmax = 0;
tic
for i=1:fac+1
    for j=1:fac+1
        v = [x(i,j),y(i,j),z(i,j)]'; % 方向
        
        hp = findh(Cwh,v);

        h(i,j) = dot(v,Cwh*hp); % 模值
        hx(i,j)=h(i,j)*x(i,j); % 分量x
        hy(i,j)=h(i,j)*y(i,j); % 分量y
        hz(i,j)=h(i,j)*z(i,j); % 分量z
    end
end
toc

	% 通过fminimax函数搜索，进行动量轮角动量最大值的最小化
    function h = findh(Cwh,v)
        options = optimoptions('fminimax','MinAbsMax',nw,'Display','off');
%         x0 = Dwh*v;
        x0 = zeros(nw,1);
        h = fminimax(@(x)x,x0,[],[],Cwh,v,-1*ones(nw,1),ones(nw,1),[],options);
        sf = hmax/max(abs(h)); % 放大因子
        h = sf*h; % 放大得到每个轮子的角动量
    end
end