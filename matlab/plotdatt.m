% »­×ËÌ¬½ÇËÙ¶ÈÇúÏß
function plotdatt(t,dyn,est)
plot3axis(t,dyn);
plot3axis(t,est,'r');
subplot 311,ylabel('d\phi(deg/s)'),legend('est','dyn','Location','best');
set(zoom,'Motion','vertical','Enable','on');
subplot 312,ylabel('d\theta(deg/s)');
set(zoom,'Motion','vertical','Enable','on');
subplot 313,ylabel('d\psi(deg/s)');
set(zoom,'Motion','vertical','Enable','on');