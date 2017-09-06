function ax = plotshpere(rou)
if nargin == 0
    rou = 1;
end
n = 48;
theta = pi*(-n:2:n)/n;
phi = (pi/2)*(-n:4:n)'/n;
X = rou*cos(phi)*cos(theta);
Y = rou*cos(phi)*sin(theta);
Z = rou*sin(phi)*ones(size(theta));
ax = axes;
colormap(0.9*[1 1 1;1 1 1]);
mesh(X,Y,Z,'facealpha',0.5);
set(ax,'CameraViewAngle',7,'CameraViewAngleMode','manual',...
    'CameraTarget',[0 0 0],'CameraTargetMode','manual',...
    'NextPlot','add','Visible','off',...
    'Zdir','Reverse','Ydir','Reverse');