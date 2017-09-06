% 画轨道根数曲线
function plot6coe(x,y,unit,sixth)
if nargin < 3
    unit = '';
    sixth = 'M';
elseif nargin == 3
    unit = ['(' unit ')'];
    sixth = 'M';
elseif nargin == 4
    unit = ['(' unit ')'];
end

plot6(x,y,'label','a','e',['i',unit],...
    ['\Omega',unit],['\omega',unit],[sixth,unit]);

end
