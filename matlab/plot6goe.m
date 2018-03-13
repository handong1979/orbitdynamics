% 画轨道根数曲线
function plot6goe(x,y,unit,sixth)
if nargin < 3
    unit = '';
    sixth = '\lambda';
elseif nargin == 3
    unit = ['(' unit ')'];
    sixth = '\lambda';
elseif nargin == 4
    unit = ['(' unit ')'];
end

plot6(x,y,'label','a(km)','e_x','e_y','i_x',...
    'i_y',[sixth,unit]);

end
