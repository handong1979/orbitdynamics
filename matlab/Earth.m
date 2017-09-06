%  ªÊ÷∆µÿÕº
load('topo.mat','topo','topomap1');
contour(0:359,-89:90,topo,[0 0],'b'),grid on;
axis equal
set(gca,'XLim',[0 360],'YLim',[-90 90]);
hold on;