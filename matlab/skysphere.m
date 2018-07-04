function skysphere()
n = 24;
theta = pi*(-n:2:n)/n;
phi = (pi/2)*(-n:4:n)'/n;
X = cos(phi)*cos(theta);
Y = cos(phi)*sin(theta);
Z = sin(phi)*ones(size(theta));
f3d = axes;
colormap(f3d,0.9*[1 1 1;1 1 1]);
mesh(f3d,X,Y,Z,'facealpha',0.5);
set(f3d,'CameraViewAngle',7,'CameraViewAngleMode','manual',...
    'CameraTarget',[0 0 0],'CameraTargetMode','manual',...
    'NextPlot','add','Visible','off');
view(f3d,[77 28]);
xlabel(f3d,'X');ylabel(f3d,'Y');zlabel(f3d,'Z');d=0.05;
text(1+d,d,d,'+X','parent',f3d);text(-1-d,d,d,'-X','parent',f3d);
text(d,1+d,d,'+Y','parent',f3d);text(d,-1-d,d,'-Y','parent',f3d);
text(d,d,1+d,'+Z','parent',f3d);text(d,d,-1-d,'-Z','parent',f3d);
line(-1,0,0,'parent',f3d,'marker','o','markeredgecolor',[1,0,0]);
line(1,0,0,'parent',f3d,'marker','o','markerfacecolor',[1,0,0]);
line(0,-1,0,'parent',f3d,'marker','o','markeredgecolor',[0,1,0]);
line(0,1,0,'parent',f3d,'marker','o','markerfacecolor',[0,1,0]);
line(0,0,-1,'parent',f3d,'marker','o','markeredgecolor',[0,0,1]);
line(0,0,1,'parent',f3d,'marker','o','markerfacecolor',[0,0,1]);
axis(f3d,'square');
shading(f3d,'interp');