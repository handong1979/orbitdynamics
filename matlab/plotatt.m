% »­×ËÌ¬ÇúÏß
function plotatt(t,dyn,est)
plot3axis(t,dyn);
plot3axis(t,est,'r');
subplot 311,ylabel('\phi(deg)'),legend('dyn','est','Location','best');
set(zoom,'Motion','vertical','Enable','on');
subplot 312,ylabel('\theta(deg)');
set(zoom,'Motion','vertical','Enable','on');
subplot 313,ylabel('\psi(deg)');
set(zoom,'Motion','vertical','Enable','on');