% 画轨道根数曲线
function plot6pv(x,y,varargin)
if nargin == 3
    unit = varargin{1};
else
    unit = 'km';
end
punit = ['(' unit ')'];
vunit = ['(' unit '/s)'];
plot6(x, y,'label',['x',punit],['y',punit],['z',punit],...
        ['vx',vunit],['vy',vunit],['vz',vunit]);
