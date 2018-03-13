% »­×ËÌ¬ÇúÏß
% See also: plotdatt
function plotatt(t,dyn,est)
plot3axis(t,dyn);
subplot 311,ylabel('\phi(deg)');
set(zoom,'Motion','vertical','Enable','on');
subplot 312,ylabel('\theta(deg)');
set(zoom,'Motion','vertical','Enable','on');
subplot 313,ylabel('\psi(deg)');
set(zoom,'Motion','vertical','Enable','on');
if nargin == 3
    plot3axis(t,est,'r');
    legend('dyn','est','Location','best')
end