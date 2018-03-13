%判断地影程序
%umbra(spacecraft,position_sun,body,r_body)判断航天器是否被天体遮挡产生阴影。
%
%输入：
%spacecraft：航天器位置，行或列向量；
%position_sun：太阳位置，行或列向量；
%body：遮挡天体位置，行或列向量；
%r_body：遮挡天体半径。
%以上参数单位必须一致。
%
%输出：
%    1为航天器在光照区
%    0为航天器在阴影区

function [y,k]= umbra(s) %spacecraft,position_sun,body,r_body

rs = de421(s(1),'Earth','Sun');
rs = rs(1:3)';
r = s(8:10)';
dr = r - rs;
rm = norm(r,2);
drm = norm(dr,2);
thetaES = acos( r'*dr/rm/drm );  % 天体-卫星-太阳夹角
as = asin(695990.0/drm);  % 卫星上看太阳的视半径
ae = asin(Re/rm);  % 卫星上看天体的视半径
if thetaES <= ae-as
    k = 0.0; % 本影区，日全食
elseif thetaES < ae+as
    if as-thetaES>=ae
        % 天体比太阳视半径小，日环食
        k = 1.0 - ae*ae/as/as;
    else
        % 太阳可视面积与太阳圆面积的比值，日偏食
        a = acos( (ae*ae+thetaES*thetaES-as*as)/(2.0*ae*thetaES) );
        b = acos( (as*as+thetaES*thetaES-ae*ae)/(2.0*as*thetaES) );
        k = 1.0 - ( as*as*(b-sin(b)*cos(b)) + ae*ae*(a-sin(a)*cos(a)) )/as/as/pi;  
    end		
else
    k = 1.0; % 阳照区
end
% 二值化处理
if k > 0.9
    y = 1;
else
    y = 0;
end


% a=reshape(position_sun(1:3),3,1);
% b=reshape(body(1:3),3,1);
% c=reshape(s(8:10),3,1);
% 
% ra=norm(a-b,2);
% rb=norm(b-c,2);
% rc=norm(c-a,2);
% 
% alpha=acos((ra^2+rb^2-rc^2)/2/ra/rb);
% h=rb*sin(alpha);
% 
% if alpha>pi/2 && h<r_body
%     y=0;%航天器在阴影区
% else
%     y=1;%航天器在光照区
% end

