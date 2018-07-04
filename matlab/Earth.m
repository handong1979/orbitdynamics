%  绘制地图
function Earth()
persistent topo
if isempty(topo)
    load('topo.mat','topo','topomap1');
end
contour(0:359,-89:90,topo,[0 0],'b','HandleVisibility','off'),grid on;
axis equal
set(gca,'XLim',[0 360],'YLim',[-90 90]);
hold on;