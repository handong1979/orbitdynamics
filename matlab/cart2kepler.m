%惯性坐标转化为轨道根数 cart2kepler(cart,miu)
% use:
%    cart2kepler(cart); %默认为地心
%    cart2kepler(cart,miu);  %其他中心天体
%    参数cart必须为1*6(单位km,km/s)
%    地心时miu为398600.5km^3/s^2，月心时miu为4902.8km^3/s^2
% result:
%    kepler = [a  e  i Omega  w  M](单位:km  rad)
function kepler = cart2kepler(cart,miu)
if nargin == 0 %self test
    cart =   [   6637.6      -851.42         4317     -0.76017        6.622       2.4748;...
       2659.3       2705.9       5970.5      -2.9926      -5.6122       3.9568];
%    cart = cart';
end
[m,n] = size(cart);
if m == 6
    tr = 0;
    len = n;
elseif n == 6;
    cart = cart'; % 统一作为列矢量处理
    len = m;
    tr = 1; % 计算结果需要转置成与输入一致
else
    error('input dimension error');
end
if nargin < 2
    miu = 398600.4416;
end

P = cart(1:3,:);
V = cart(4:6,:);
r = sqrt(sum(P.^2,1));
v = sqrt(sum(V.^2,1));
v2 = v.*v;
% rrd = P'*V;

kepler = nan(6,len);
% a
kepler(1,:) = 1.0 ./ (2.0./r - v2/miu);
Uz = [0;0;1]*ones(1,len);
h = cross(P,V);
h2 = sum(h.^2,1);
N = cross(Uz,h);
Nn = sqrt(sum(N.^2,1));
p = h2/miu;

% e
if any(p>kepler(1,:)+0.0000001)
    error('p>a, e is less then 0?');
%     kepler(2) = 0;
else
    kepler(2,:) = sqrt(1.0 - p./kepler(1,:));
end

% i
cosi = dot(h,Uz)./sqrt(h2);
kepler(3,:) = acos(cosi);
kepler(3,cosi >= 1.0) = 0.0;
kepler(3,cosi <= -1.0) = pi;

% Omega
Nx = N(1,:)./Nn;
id = find(N(2,:)>=0);
kepler(4,id) = acos(Nx(id));
id = find(N(2,:)<0);
kepler(4,id) = pi*2 - acos(Nx(id));
kepler(4,Nx > 1.0) = 0.0;
kepler(4,Nx < -1.0) = pi;

% w
E = cross(V,h)/miu - P./(ones(3,1)*r);
cosw = dot(N,E)./Nn./sqrt(sum(E.^2,1));
id = find(E(3,:)>0);
kepler(5,id) = acos(cosw(id));
id = find(E(3,:)<=0);
kepler(5,id) = pi*2 - acos(cosw(id));
kepler(5,cosw > 1.0) = 0.0;
kepler(5,cosw < -1.0) = pi;

% M
cosu = dot(P,N)./r./Nn;
u = nan(1,len);
id = find(P(3,:)>0);
u(id) = acos(cosu(id));
id = find(P(3,:)<=0);
u(id) = pi*2 - acos(cosu(id));
u(cosu >= 1.0) = 0.0;
u(cosu <= -1.0) = pi;

id = find(kepler(2,:)>1);
if ~isempty(id)
    x = ((kepler(2,id)-1)./(1+kepler(2,id))).^0.5 .* tan((u(id)-kepler(5,id))/2.0);
    EE = log((1+x)./(1-x));
    kepler(6,id) = kepler(2,id).*sinh(EE) - EE;
end
id = find(kepler(2,:)<=1);
if ~isempty(id)
    EE = mod(2.0 * atan( ((1-kepler(2,id))./(1+kepler(2,id))).^0.5 .* tan((u-kepler(5,id))/2.0) ) + pi*2,pi*2);
    kepler(6,id) = EE - kepler(2,id).*sin(EE);
end

if tr
    kepler = kepler';
end