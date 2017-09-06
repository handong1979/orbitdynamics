% 3*1的三条曲线
function plot3axis(t,varargin)
subplot 311,plot(t,varargin{1}(:,1),varargin{2:end}),grid on,hold on;
subplot 312,plot(t,varargin{1}(:,2),varargin{2:end}),grid on,hold on;
subplot 313,plot(t,varargin{1}(:,3),varargin{2:end}),grid on,hold on;
