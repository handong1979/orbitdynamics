% 画相对运动轨迹图
function plotreltraj(traj,t,type)
if nargin == 1
    type = 'xzxy';
elseif nargin == 2
    type = 'txyz';
end
switch type
    case 'xzxy'
        subplot 211,plot(traj(:,1),traj(:,3));
        grid on,axis equal,xlabel('x'),ylabel('z');
        hold on,plot(traj(1,1),traj(1,3),'ro');
        set(gca,'Ydir','reverse');
        subplot 212,plot(traj(:,1),traj(:,2));
        grid on,axis equal,xlabel('x'),ylabel('y');
        hold on,plot(traj(1,1),traj(1,2),'ro');
    case 'xz'
        plot(traj(:,1),traj(:,3));
        grid on,axis equal,xlabel('x(km)'),ylabel('z(km)');
        hold on,plot(traj(1,1),traj(1,3),'ro');
        set(gca,'Ydir','reverse');
    case 'xy'
        plot(traj(:,1),traj(:,2)),grid on,axis equal;
        grid on,axis equal,xlabel('x(km)'),ylabel('y(km)');
        hold on,plot(traj(1,1),traj(1,2),'ro');
    case 'xyz'
        plot3(traj(:,1),traj(:,2),traj(:,3)),grid on,axis equal;
        hold on,plot3(traj(1,1),traj(1,2),traj(1,3),'ro');
    case 'txyz'
        subplot 311,plot(t,traj(:,1)),grid on,xlabel('t'),ylabel('x');
        subplot 312,plot(t,traj(:,2)),grid on,xlabel('t'),ylabel('y');
        subplot 313,plot(t,traj(:,3)),grid on,xlabel('t'),ylabel('z');
end