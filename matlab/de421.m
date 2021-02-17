% DE421������
% rv = DE421(mjd,cent,target)
% mjd: ����ʱ��Լ������
% cent: ��������
% target: Ŀ������
% rv: ���1*6���󣬷ֱ�Ϊλ���ٶȣ���λkm��km/s
% ���������Ŀ�����������Ϊ(�����ִ�Сд):
%  �������ŵĶ�Ӧ�б�
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
%  ��������λ��
%     rv = de421(mjd,'Earth','moon')
%  �����¶���   
%     nt = de421(mjd,'Nutations')
%  ����������ƽ����
%     lb = de421(mjd,'Librations')

%% example code
% ������ƽ���ǵı仯
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
    % �¶�����ƽ��
    out = mexDE421(mjd,icent);
else
    % ����
    itar = name2int(target);
    out = mexDE421(mjd,icent,itar);
end

% ÿ�������Ӧ�ı��
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
