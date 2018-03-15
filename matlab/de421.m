% DE421星历表
% rv = DE421(mjd,cent,target)
% mjd: 北京时简约儒略日
% cent: 中心天体
% target: 目标天体
% rv: 输出1*6矩阵，分别为位置速度，单位km和km/s
% 中心天体和目标天体的名称为(不区分大小写):
%  名称与编号的对应列表
%   0--Mercury
%   1--Venus
%   2--Earth
%   3--Mars
%   4--Jupiter
%   5--Saturn
%   6--Uranus
%   7--Neptune
%   8--Pluto
%   9--Moon
%   10--Sun
%   11--Solar_system_barycenter
%   12--Earth_moon_barycenter
%   13--Nutations
%   14--Librations
% Example:
%     mjd = date2mjd(2009,1,19,12,0,0)
%  计算月球位置
%     rv = de421(mjd,'Earth','moon')
%  计算章动角   
%     nt = de421(mjd,'Nutations')
%  计算月球天平动角
%     lb = de421(mjd,'Librations')

%% example code
% 月球天平动角的变化
% mjd = date2mjd(2009,1,19,12,0,0);
% for t=1:1440*28
%    lb(t,:) = de421(mjd+t/1440,'Librations');
% end
% figure,plot(1:1440*28,lb(:,1)*deg)
% figure,plot(1:1440*28,lb(:,2)*deg)
% figure,plot(1:1440*28,lb(:,3)*deg)
% figure,plot(1:1440*28-1,diff(lb(:,3))*deg)

%%
function out = de421(mjd,cent,target)
icent = name2int(cent);
if (icent == 13) || (icent == 14)
    % 章动或天平动
    out = mexDE421(mjd,icent);
else
    % 行星
    itar = name2int(target);
    out = mexDE421(mjd,icent,itar);
end

% 每个天体对应的编号
function i = name2int(name)
switch upper(name)
    case 'MERCURY'
        i = 0;
    case 'VENUS'
        i = 1;
    case 'EARTH'
        i = 2;
    case 'MARS'
        i = 3;
    case 'JUPITER'
        i = 4;
    case 'SATURN'
        i = 5;
    case 'URANUS'
        i = 6;
    case 'NEPTUNE'
        i = 7;
    case 'PLUTO'
        i = 8;
    case 'MOON'
        i = 9;
    case 'SUN'
        i = 10;
    case 'SOLAR_SYSTEM_BARYCENTER'
        i = 11;
    case 'EARTH_MOON_BARYCENTER'
        i = 12;
    case 'NUTATIONS'
        i = 13;
    case 'LIBRATIONS'
        i = 14;
    otherwise
        error('there is no this planet');
end
