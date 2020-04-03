% ×ËÌ¬Çò
function attsphere()
n = 48;
theta = pi*(-n:2:n)/n;
phi = (pi/2)*(-n:4:n)'/n;
X = cos(phi)*cos(theta);
Y = cos(phi)*sin(theta);
Z = sin(phi)*ones(size(theta));

f3d = gca;
colormap(f3d,0.9*[1 1 1;1 1 1]);
mesh(f3d,X,Y,Z,'facealpha',0.5,'HandleVisibility','off');
set(f3d,'CameraViewAngle',6,'CameraViewAngleMode','manual',...
    'CameraTarget',[0 0 0],'CameraTargetMode','manual',...
    'NextPlot','add','Visible','off',...
    'Zdir','Reverse','Ydir','Reverse');
xlabel(f3d,'X');ylabel(f3d,'Y');zlabel(f3d,'Z');d=0.1;
text(1+d,0,0,'+X','parent',f3d);%text(-1-d,d,d,'-X','parent',f3d);
text(0,1+d,0,'+Y','parent',f3d);%text(d,-1-d,d,'-Y','parent',f3d);
text(0,0,1+d,'+Z','parent',f3d);%text(d,d,-1-d,'-Z','parent',f3d);
% line(-1,0,0,'parent',f3d,'marker','o','markeredgecolor',[1,0,0]);
line([0 1],[0 0],[0 0],'parent',f3d,'color','k','marker','o','markersize',6,...
    'markerfacecolor',[1,0,0],'linewidth',1);
% line(0,-1,0,'parent',f3d,'marker','o','markeredgecolor',[0,1,0]);
line([0 0],[0 1],[0 0],'parent',f3d,'color','k','marker','o','markersize',6,...
    'markerfacecolor',[0,1,0],'linewidth',1);
% line(0,0,-1,'parent',f3d,'marker','o','markeredgecolor',[0,0,1]);
line([0 0],[0 0],[0 1],'parent',f3d,'color','k','marker','o','markersize',6,...
    'markerfacecolor',[0,0,1],'linewidth',1);
axis(f3d,'square');
shading(f3d,'interp');
hold on;