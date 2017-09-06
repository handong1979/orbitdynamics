% 星敏安装、光干扰设计工具
% Origined by 李铁寿
%             用于嫦娥一号敏感器视场、光干扰分析
% Created by 韩冬
%            摘取李铁寿程序中绘图的部分，只保留星敏视场分析
%            绘图功能扩展，GUI设计，交互功能开发
% 2014.9.23  增加星敏光轴设置文本框，重绘图形时读入星敏光轴

% TODO:
%    1. 在界面上设置星敏遮蔽角
%    2. 动态测量两点之间的球面角---完成顶视图，有待补充侧视图
%    3. 完善交互式的星敏安装设计，兼容不同的星敏个数,增加重新初始化功能(迭代设计)
%    4. 使用MATLAB Mapping Toolbox中的各种功能绘制投影图
%    5. 将地球边缘、地球干扰边缘用不同的颜色区分---完成，地球边缘使用蓝色，干扰边缘依次变浅
%    6. 将太阳边缘、太阳干扰边缘用不同的颜色区分---完成，地球边缘使用红色，干扰边缘依次变浅
%    7. 在三维视图中，绘制本体系三个坐标轴、轨道系三个坐标轴
% BUG:
%    1.patch区域在图形中会被截断，形成错误的区域
%    2.投影图使用麦卡锡投影，图中“南北极”部分变形大，投影图中的两点距离被拉长

function stsassemble
rad = pi/180;
Re = 6378.137;% 地球赤道半径
default_orbit_height = 673;
default_beta = -58;
default_phi = 0;
default_theta = 0;
default_psi = 0;

% za = cosd([46.4137;   77.1297;   133.5698]); % 星敏A光轴
% zb = cosd([133.5449;    77.1047;  133.6003]); % 星敏B光轴
% zc = [0.20424014  0.73179743 -0.65006076]'; % 星敏C光轴
% FH-2星敏安装
za = cosd([60+12/60+58/3600	35 	106+39/60+57/3600]');
zb = cosd([119+47/60+2*3600	35 	106+39/60+57/3600]');
zc = cosd([90     	145 	125]');

%% 创建界面
hf = figure('Position',[50 110 1140 820],'MenuBar','figure','Toolbar','figure',...
    'Name','星敏安装设计分析工具','NumberTitle','off','Color',[0.9 0.9 0.8],...
    'Resize','off','HandleVisibility','on','Visible','on');
% 顶视图功能按键
uicontrol('Parent',hf,  'Callback',@setButtonDown, ...
    'String','测量两点球面距离(鼠标左键开始右键结束)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[700 10 250 50]);
uicontrol('Parent',hf,  'Callback',@manualInstallSS, ...
    'String','用鼠标点击星敏光轴位置',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[960 10 150 50]);
% 姿态角设置UI
xb = 30;yb = 740;
hphi = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_phi),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb yb 50 20]);
htheta = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_theta),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+55 yb 50 20]);
hpsi = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_psi),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+110 yb 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','滚动(度)','Tag','tphi',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb yb+20 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','俯仰(度)','Tag','ttheta',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+55 yb+20 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','偏航(度)','Tag','tpsi',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+110 yb+20 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','姿态角设置',...
    'FontUnits','pixels', 'FontSize',14,    'Position',[xb yb+50 160 20]);
% 轨道设置
hheight = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_orbit_height),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+180 yb 75 20]);
hbeta = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_beta),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+180+80 yb 75 20]);
uicontrol('Parent',hf,  'Style','text', 'String','轨道高度(km)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+180 yb+20 75 20]);
uicontrol('Parent',hf,  'Style','text', 'String','beta(度)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+180+80 yb+20 75 20]);
uicontrol('Parent',hf,  'Style','text', 'String','轨道设置',...
    'FontUnits','pixels', 'FontSize',14,    'Position',[xb+180 yb+50 160 20]);
% 更新绘图按钮
uicontrol('Parent',hf,  'Callback',@refreshcoord, ...
    'String','重绘',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+280+75 yb 150 30]);
% 星敏光轴设置
uicontrol('Parent',hf,  'Style','text', 'String','星敏A光轴',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 100 75 20]);
hsza = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(za'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 100 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','星敏B光轴',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 80 75 20]);
hszb = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(zb'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 80 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','星敏C光轴',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 60 75 20]);
hszc = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(zc'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 60 275 20]);

f3d = [];
f2d1 = [];
f2d2 = [];
% 绘制默认图形
coordinate(); %绘制坐标轴
ah = f2d2; % 用于动态测量显示
drawf();
sts(za(:),[30],'A');
sts(zb(:),[30],'B');
sts(zc(:),[30],'C');

%% 绘图
    function drawf(hObject,~)
        h = str2double(get(hheight, 'String')); % orbit height
        rou = asind(Re/(Re+h)); % earth half angle
        beta = str2double(get(hbeta, 'String')); % 太阳与轨道面夹角,+Y一侧为正
        phi = str2double(get(hphi, 'String'));
        theta = str2double(get(htheta, 'String'));
        psi = str2double(get(hpsi, 'String'));
        Cbo = roty(theta*rad)*rotx(phi*rad)*rotz(psi*rad);
        % 绘制地球
        earth([rou rou+30 rou+40],Cbo);
        % 绘制太阳
        sun([beta beta-30 beta+30 beta-40 beta+40],Cbo);
        za = str2num(get(hsza, 'String')); % 星敏A光轴
        zb = str2num(get(hszb, 'String')); % 星敏B光轴
        zc = str2num(get(hszc, 'String')); % 星敏C光轴
        sts(za(:),[30],'A');
        sts(zb(:),[30],'B');
        sts(zc(:),[30],'C');
    end

%% 投影算法
% 侧视图投影
    function [x y] = Mercator1(X,Y,Z)
        y = asin(-Z)/rad;
        x = atan2(X,-Y)/rad;
    end
% 顶视图投影
    function [x y] = Mercator2(X,Y,Z)
        y = asin(X)/rad;
        x = atan2(Y,-Z)/rad;
    end

%% 坐标图
    function coordinate()
        n = 24;
        theta = pi*(-n:2:n)/n;
        phi = (pi/2)*(-n:4:n)'/n;
        X = cos(phi)*cos(theta);
        Y = cos(phi)*sin(theta);
        Z = sin(phi)*ones(size(theta));
        % x = pi*(-n:2:n)/n;
        % y = (pi/2)*(-n:2:n)'/n;
        if isempty(f3d)
            f3d = subplot(2,2,[1 3],'Parent',hf);
        else
            set(hf,'CurrentAxes',f3d);
        end
        colormap(0.9*[1 1 1;1 1 1]);
        mesh(X,Y,Z,'facealpha',0.5);
        set(f3d,'CameraViewAngle',5,'CameraViewAngleMode','manual',...
            'CameraTarget',[0 0 0],'CameraTargetMode','manual',...
            'NextPlot','add','Visible','off',...
            'Zdir','Reverse','Ydir','Reverse');
        view([-95 10]);
        xlabel('X');ylabel('Y');zlabel('Z');d=0.1;
        text(1+d,d,d,'+X');text(-1-d,d,d,'-X');
        text(d,1+d,d,'+Y');text(d,-1-d,d,'-Y');
        text(d,d,1+d,'+Z');text(d,d,-1-d,'-Z');
        line(-1,0,0,'marker','o','markeredgecolor',[1,0,0]);
        line(1,0,0,'marker','o','markerfacecolor',[1,0,0]);
        line(0,-1,0,'marker','o','markeredgecolor',[0,1,0]);
        line(0,1,0,'marker','o','markerfacecolor',[0,1,0]);
        line(0,0,-1,'marker','o','markeredgecolor',[0,0,1]);
        line(0,0,1,'marker','o','markerfacecolor',[0,0,1]);
        axis square;hidden on;
        f2d1 = subplot(2,2,2,'Parent',hf);
        set(hf,'CurrentAxes',f2d1);
        set(f2d1,'Units','Pixel','Position',[650 457 460 300]);
        % axesm('MapProjection','mercator','MapLatLimit',[-90 90],'MapLonLimit',[-180 180]),grid on;
        d=4;
        plot(-90,0,'marker','o','markeredgecolor',[1,0,0]);grid on,hold on;
        line(90,0,'marker','o','markerfacecolor',[1,0,0]);
        line(0,-90,'marker','o','markeredgecolor',[0,1,0]);
        line(0,90,'marker','o','markerfacecolor',[0,1,0]);
        line(0,0,'marker','o','markeredgecolor',[0,0,1]);
        line(180,0,'marker','o','markerfacecolor',[0,0,1]);
        line(-180,0,'marker','o','markerfacecolor',[0,0,1]);
        text(d,d,'-Y');text(180+d,d,'+Y');text(-180-d,d,'+Y');
        text(90+d,d,'+X');        text(-90-d,d,'-X');
        text(d,90+d,'-Z');        text(d,-90-d,'+Z');
        axis([-180,180,-90,90]);        title('侧视图');
        set(f2d1,'XTick',-180:30:180);   set(f2d1,'YTick',-90:30:90);
        f2d2 = subplot(2,2,4,'Parent',hf);
        set(hf,'CurrentAxes',f2d2);
        set(f2d2,'Units','Pixel','Position',[650 100 460 310]);
        % axesm('MapProjection','mercator','MapLatLimit',[-90 90],'MapLonLimit',[-180 180]),grid on;
        d=4;
        plot(90,0,'marker','o','markerfacecolor',[1,0,0]);grid on,hold on;
        line(-90,0,'marker','o','markeredgecolor',[1,0,0]);
        line(0,-90,'marker','o','markeredgecolor',[0,1,0]);
        line(0,90,'marker','o','markerfacecolor',[0,1,0]);
        line(0,0,'marker','o','markeredgecolor',[0,0,1]);
        line(180,0,'marker','o','markerfacecolor',[0,0,1]);
        line(-180,0,'marker','o','markerfacecolor',[0,0,1]);
        text(d,d,'-Z');text(180+d,d,'+Z');text(-180-d,d,'+Z');
        text(d,90+d,'+X');text(d,-90-d,'-X');
        text(-90+d,d,'-Y');text(90+d,d,'+Y');
        axis([-180,180,-90,90]);title('顶视图');
        set(f2d2,'XTick',-180:30:180);   set(f2d2,'YTick',-90:30:90);
    end
    function refreshcoord(hObject,~)
        set(f3d,'NextPlot','replace');
        set(f2d1,'NextPlot','replace');
        set(f2d2,'NextPlot','replace');
        coordinate();
        drawf();
    end
%% 地球
    function earth(rou,Cbo)
        sth = linspace(0,360,360);
        for i = 1:length(rou)
            sph = (90-rou(i))*ones(size(sth));
            x = cos(sph*rad).*cos(sth*rad);
            y = cos(sph*rad).*sin(sth*rad);
            z = sin(sph*rad);
            c = Cbo*[x;y;z];
            x = c(1,:);
            y = c(2,:);
            z = c(3,:);
            color = [0 0.15*i 1];
            set(hf,'CurrentAxes',f3d);
            line(x,y,z,'color',color,'linewidth',1.5);
            d=1.05;
            text(x(1)*d,y(1)*d,z(1)*d,['Earth' num2str(rou(i),'%.1f')]);
            
            [a b] = Mercator1(x,y,z);
            set(hf,'CurrentAxes',f2d1);
            line(a,b,'color',color,'linewidth',1.0,'linestyle','none','marker','.');
            % linem(b,a);
            % patch(a,b,'b','FaceAlpha',0.2);
            [a b] = Mercator2(x,y,z);
            set(hf,'CurrentAxes',f2d2);
            line(a,b,'color',color,'linewidth',1.0,'linestyle','none','marker','.');
            % linem(b,a);patchm(b,a,'b','FaceAlpha',0.2);
            % patch(a,b,'b','FaceAlpha',0.2);
        end
    end

%% 太阳
    function sun(beta,Cbo)
        sth = linspace(0,360,360);
        for i = 1:length(beta)
            if abs(beta(i))>90
                continue;
            end
            sph = beta(i)*ones(size(sth));
            x = cosd(sph).*cosd(sth);
            y = cosd(sph).*sind(sth);
            z = sind(sph);
            c = Cbo*rotx(pi/2)*[x;y;z];
            x = c(1,:);
            y = c(2,:);
            z = c(3,:);
            
            set(gcf,'CurrentAxes',f3d);
            color = [1 0.5/length(beta)*i 0];
            line(x,y,z,'color',color,'linewidth',1.5);
            d=1.05;
            text(x(1)*d,y(1)*d,z(1)*d,['Sun' num2str(beta(i),'%.1f')]);
            
            [a b] = Mercator1(x,y,z);
            set(gcf,'CurrentAxes',f2d1);
            line(a,b,'color',color,'linewidth',1.0,'linestyle','none','marker','.');
            %             patch(a,b,'r','FaceAlpha',0.2);
            [a b] = Mercator2(x,y,z);
            set(gcf,'CurrentAxes',f2d2);
            line(a,b,'color',color,'linewidth',1.0,'linestyle','none','marker','.');
            %             patch(a,b,'r','FaceAlpha',0.2);
        end
    end

%% 星敏
    function sts(za,angle,name)
        sth = linspace(0,360,360);
        za = za(:);
        for i=1:length(angle)
            sph = (90-angle(i))*ones(size(sth));
            X = cosd(sph).*cosd(sth);
            Y = cosd(sph).*sind(sth);
            Z = sind(sph);
            % a
            z = [0;0;1];
            n = cross(z,za);
            n = n/norm(n);
            phi = acos(z'*za);
            q = [n*sin(phi/2); cos(phi/2)];
            ia = q2c(q);
            
            xyz = ia'*[X;Y;Z];
            x = xyz(1,:);
            y = xyz(2,:);
            z = xyz(3,:);
            set(gcf,'CurrentAxes',f3d);
            line(x,y,z,'color','k','linewidth',1.5);
            d=1.1;
            text(ia(3,1)*d,ia(3,2)*d,ia(3,3)*d,name);
            line(ia(3,1),ia(3,2),ia(3,3),'marker','o','markerfacecolor',[1,0,0])
            
            [a b] = Mercator1(x,y,z);
            set(gcf,'CurrentAxes',f2d1);
            line(a,b,'color','k','linewidth',1.0,'linestyle','none','marker','.','markersize',1);
            [a b] = Mercator1(za(1),za(2),za(3));
            line(a,b,'color','c','marker','o','markerfacecolor',[0,0,1]);text(a,b,name);
            
            [a b] = Mercator2(x,y,z);
            set(gcf,'CurrentAxes',f2d2);
            line(a,b,'color','k','linewidth',1.0,'linestyle','none','marker','.','markersize',1);
            [a b] = Mercator2(za(1),za(2),za(3));
            line(a,b,'color','c','marker','o','markerfacecolor',[0,0,1]);text(a,b,name);
        end
    end

%% 功能UI
% 开始测量距离
    function setButtonDown(hObject,~)
        set(gcf,'WindowButtonDownFcn',@wbdcb);
    end
% 动态显示两点的连线和鼠标当前点的坐标
    function wbdcb(src,evnt)
        if strcmp(get(src,'SelectionType'),'normal')
            [x,y,str] = disp_point();
            hl = line('XData',x,'YData',y,'Color','r');
            ht = text(x,y,'0.0');
            text(x,y,str,'VerticalAlignment','bottom');
            drawnow
            set(src,'WindowButtonMotionFcn',@wbmcb);
        elseif strcmp(get(src,'SelectionType'),'alt')
            set(src,'WindowButtonMotionFcn','')
            [x,y,str] = disp_point();
            text(x,y,str,'VerticalAlignment','bottom');
            drawnow
            set(gcf,'WindowButtonDownFcn','');
        end
        function wbmcb(src,evnt)
            [xn,yn,str] = disp_point();
            A = [sind(y); cosd(y)*sind(x); -cosd(y)*cosd(x) ];
            B = [sind(yn); cosd(yn)*sind(xn); -cosd(yn)*cosd(xn) ];
            N = cross(A,B);    N=N/norm(N);
            angleAB = acosd(A'*B);
            alfa = linspace(0,angleAB,20);
            xdat = nan(20,1);
            ydat = nan(20,1);
            for i=1:20
                q = [N*sind(alfa(i)/2);cosd(alfa(i)/2)];
                c = q2c(q);
                P = c'*A;
                [xdat(i) ydat(i)]=Mercator2(P(1),P(2),P(3));
            end
            set(hl,'XData',xdat,'YData',ydat);
            set(ht,'Position',[xdat(10) ydat(10)],'String',num2str(angleAB,'%0.3g'));
            drawnow
        end
    end
    function [x,y,str] = disp_point()
        cp = get(ah,'CurrentPoint');
        x = cp(1,1);y = cp(1,2);
        str = ['(',num2str(x,'%0.3g'),', ',num2str(y,'%0.3g'),')'];
    end
% 在图形上手动点出两个星敏光轴的位置，并画出两个星敏的遮光角、两星敏光轴夹角
    function manualInstallSS(hObject,~)
        %         while 1
        axes(f2d2)
        [x,y] = ginput(1);
        x = -x;
        line(x,y,'linewidth',2.0);
        xx = x;
        yy = y;
        za = [sind(y);-cosd(y).*sind(x); -cosd(y).*cosd(x)];
        sts(za,[40 90],'A');
        [x,y] = ginput(1);
        x = -x;
        line(x,y,'linewidth',2.0);
        xx = [xx;x];
        yy = [yy;y];
        zb = [sind(y); -cosd(y).*sind(x); -cosd(y).*cosd(x)];
        sts(zb,[40 90],'B');
        %     [x,y] = ginput(1);
        %     x = -x;
        %     line(x,y,'linewidth',2.0);
        %     xx = [xx;x];
        %     yy = [yy;y];
        %     zb = [sin(y*rad); -cos(y*rad).*sin(x*rad); -cos(y*rad).*cos(x*rad)];
        %     sts(zb,90,'C');
        %     sts(zb,40,'C');
        zzangle = acosd(za'*zb);
        text(mean(xx),mean(yy)+4,sprintf('ZZ=%.2f',zzangle));
        fprintf('两星敏光轴夹角=%.2f\n',zzangle)
        fprintf('星敏A光轴: %f %f %f    星敏B光轴: %f %f %f\n',acosd(za),acosd(zb));
        %end
    end
%% 其他基本函数
% 四元数＝＝》方向余弦阵
    function C = q2c(q)
        error(nargchk(1,1,nargin));
        C = [1.0 - 2.0*(q(2)^2+q(3)^2),    2.0*(q(1)*q(2)+q(3)*q(4)),    2.0*(q(1)*q(3)-q(2)*q(4));
            2.0*(q(1)*q(2)-q(3)*q(4)),    1.0 - 2.0*(q(1)^2+q(3)^2),    2.0*(q(2)*q(3)+q(1)*q(4));
            2.0*(q(1)*q(3)+q(2)*q(4)),    2.0*(q(2)*q(3)-q(1)*q(4)),    1.0 - 2.0*(q(1)^2+q(2)^2)];
    end
% 绕X轴旋转矩阵
    function R = rotx(angle)
        if isscalar(angle)
            R = [ 1          0         0;
                0    cos(angle)  sin(angle);
                0   -sin(angle)  cos(angle)];
        elseif isvector(angle)
            n = length(angle);
            angle = reshape(angle,1,1,n);
            R = [ ones(1,1,n)     zeros(1,1,n)    zeros(1,1,n);
                zeros(1,1,n)    cos(angle)      sin(angle);
                zeros(1,1,n)   -sin(angle)      cos(angle)];
        else
            error('输入必须为标量或矢量');
        end
    end
% 绕Y轴旋转矩阵
    function R = roty(angle)
        if isscalar(angle)
            R = [cos(angle)  0    -sin(angle);
                0      1         0;
                sin(angle)  0     cos(angle)];
        elseif isvector(angle)
            n = length(angle);
            angle = reshape(angle,1,1,n);
            R = [cos(angle)    zeros(1,1,n)    -sin(angle);
                zeros(1,1,n)  ones(1,1,n)      zeros(1,1,n);
                sin(angle)    zeros(1,1,n)     cos(angle)];
        else
            error('输入必须为标量或矢量');
        end
    end
% 绕Z轴旋转矩阵
    function R = rotz(angle)
        if isscalar(angle)
            R = [ cos(angle)  sin(angle)  0;
                -sin(angle)  cos(angle)  0;
                0            0      1];
        elseif isvector(angle)
            n = length(angle);
            angle = reshape(angle,1,1,n);
            R = [ cos(angle)    sin(angle)     zeros(1,1,n);
                -sin(angle)    cos(angle)     zeros(1,1,n);
                zeros(1,1,n)  zeros(1,1,n)   ones(1,1,n)];
        else
            error('输入必须为标量或矢量');
        end
    end
end