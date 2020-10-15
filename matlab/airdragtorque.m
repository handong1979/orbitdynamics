% 计算大气阻力和力矩
% [F, T] = airdragtorque(epoch,pos,vel,cbi,area,L)
% 常值面积area
% 压心相对质心的力臂L(矢量)
% [F, T] = airdragtorque(epoch,pos,vel,cbi,nbody,abody,pcbody)
% 分别计算各个表面，最后求和
% nbody:卫星各表面法线单位矢量，m*3
% abody:卫星各表面面积,m*1
% pcbody:卫星个表面中心在质心坐标系中的位置,m*3
function [F, T] = airdragtorque(epoch,pos,vel,cbi,varargin) % nbody,abody,pcbody)
if nargin == 0 % self test
    warning('self test');
    epoch = 51544;
    [pos,vel] = kepler2cart([Re+200,0,42*rad,10*rad,0,45*rad]);
    cbi = eye(3);
    
%     mode = 'complex';
%     nbody = [0,0,1;0,0,-1];
%     abody = [10;10];
%     pcbody = [0,0,0.1;0,0,0.1];
    
    mode = 'simple';
    area = 1;
    L = [0;0;0.2];
elseif nargin == 6
    area = varargin{1};
    L = varargin{2};
    mode = 'simple';
elseif nargin == 7
    nbody = varargin{1};
    abody = varargin{2};
    pcbody = varargin{3};
    mode = 'complex';
else
    error('input error');
end

% 输入处理
pos = pos(:);
vel = vel(:);

% 计算相对速度
airvel = [-We*pos(2);We*pos(1);0]; % 大气旋转速度
relvel = cbi*(vel-airvel)*1000; % 相对大气的速度，本体系下表示,转化为m/s
relv = norm(relvel); % 相对速度大小
v = relvel/relv; % 相对速度单位矢量

% 计算大气密度
sg = gha(epoch);
lla = ecef2lla((rotz(-sg)*pos)'*1000); % TODO: 由惯性转到地固系
altitude = lla(3);
latitude = lla(1);
longitude = lla(2);
date = mjd2date(epoch);
% dy = decyear(date);
year = date(1);
dayOfYear = 215; % TODO: doy
UTseconds = 43200;
f107Average = 80; % TODO: input or ?
f107Daily = 80;
ap = ones(1,7)*16;
[~, rho] = atmosnrlmsise00(altitude, latitude, longitude, year, dayOfYear, UTseconds,f107Average,f107Daily,ap);
rou = rho(:,6);
% 计算大气阻力和力矩
Cd = 2.2;
switch mode
    case 'simple' %#ok<*STCMP>
        F = -Cd/2*rou*area*relv.^2*v;
        T = cross(L,F);
    case 'complex'
        nrect = length(abody);
        F = zeros(3,1);
        T = zeros(3,1);
        for i=1:nrect
            cosalpha = nbody(i,:)*v;
            if cosalpha > 0 % 迎风面
                Fa = -Cd/2*rou*abody(i)*cosalpha*relv.^2*v;
                Ta = cross(pcbody(i,:)',Fa);
                F = F + Fa;
                T = T + Ta;
            end
        end
end