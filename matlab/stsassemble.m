% ������װ��ơ�����ŷ�������
% Origined by ������
%             �����϶�һ���������ӳ�������ŷ���
% Created by ����
%            ժȡ�����ٳ����л�ͼ�Ĳ��֣�ֻ���������ӳ�����
%            ��ͼ������չ��GUI��ƣ��������ܿ���
% 2014.9.23  �����������������ı����ػ�ͼ��ʱ������������
% 2015.9     �޸�������װ�Ľ������ܣ����������װ������ʵʱ��ʾ���������ļн�
% 2015.11    ���ӽ����޸ĵ����̫��Ӱ�췶Χ
% 2015.12    ���ӽ������������������ڹ��
% 2016.10    ��ѡ��������װ�Զ����½���
%            ���������ļ�����Ͷ�ȡ����
%            ����һ������D
%            ���Ӽ����ܸ��Ż��ΰٷֱ�
% 2016.11    ������̬��ת��ѡ��
%            ����beta�ǵ���������
%            

% TODO:
%    ��̬��������֮��������---��ɶ���ͼ���д��������ͼ
%    ���ƽ���ʽ��������װ��ƣ����ݲ�ͬ����������,�������³�ʼ������(�������)
%    ����ά��ͼ�У����Ʊ���ϵ���������ᡢ���ϵ����������
%    ��ά��ͼ�У������������幹��
% BUG:
%    1.patch������ͼ���лᱻ�ضϣ��γɴ��������
%    2.ͶӰͼʹ������ͶӰ��ͼ�С��ϱ��������ֱ��δ�ͶӰͼ�е�������뱻���������ÿ�ѡ����ͶӰ��ʽ

function stsassemble
rad = pi/180;
Re = 6378.137;% �������뾶
default_orbit_height = 600;
default_beta = 0;
default_phi = 0;
default_theta = 0;
default_psi = 0;

% za = cosd([46.4137;   77.1297;   133.5698]); % ����A����
% zb = cosd([133.5449;    77.1047;  133.6003]); % ����B����
% zc = [0.20424014  0.73179743 -0.65006076]'; % ����C����
% FH-2������װ
% za = cosd([60+12/60+58/3600	35 	106+39/60+57/3600]');
% zb = cosd([119+47/60+2*3600	35 	106+39/60+57/3600]');
% zc = cosd([90     	145 	125]');
% za = [  0.52992     -0.82353   -0.20244]';
% zb = [ -0.52992     -0.82353   -0.20244]';
% zc = [  0.0          0.82353   -0.56727]';

% za = [ 0.49481     -0.82413      0.27564]';
% zb = [ -0.49481     -0.82413     0.27564]';
% zc = [ 0.17365      0.82413      0.53913]';
za = cosd([59.747        145.5           75]');
zb = cosd([120.25        145.5           75]');
zc = cosd([67            34.5       65.7917]');
zd = [ 0.17365      0.82413      0.53913]';
% �����ڹ��
az = 30;
bz = 30;
cz = 30;
dz = 30;

%% ��������
hf = figure('Position',[20 40 1140 820],'MenuBar','figure','Toolbar','figure',...
    'Name','�������������Է���','NumberTitle','off','Color',[0.9 0.9 0.8],...
    'Resize','off','HandleVisibility','off','Visible','on');
uicontrol('Parent',hf,'style','text','String','�������������Է���V1.00','Units','normalized',...
        'fontsize',22,'Position',[0.50,0.95,0.3,0.05]);
% ����ͼ���ܰ���
uicontrol('Parent',hf,  'Callback',@MeasureDist, ...
    'String','�����н�','TooltipString','(��������ʼ���Ҽ�����)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[650 35 250 25]);
uicontrol('Parent',hf,  'Style','text',...
    'String','(��������ʼ���Ҽ�����)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[650 10 250 25]);
uicontrol('Parent',hf,  'Callback',@manualInstallSS, ...
    'String','���������������λ��','TooltipString','(���˳��:�����������Ҽ�)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[910 35 200 25]);
uicontrol('Parent',hf,  'Style','text',...
    'String','(�����˳��:�����������Ҽ�)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[910 10 200 25]);
% ������ť
uicontrol('Parent',hf,'style','pushbutton','String','��ϸ���','Units','normalized','fontsize',12,...
        'callback','web(''.\help\���������Է��������ϸ���(�ڲ�).htm'',''-browser'')','Position',[0.90,0.95,0.1,0.05]);
uicontrol('Parent',hf,'style','pushbutton','String','ʹ��˵��','Units','normalized','fontsize',12,...
        'callback','web(''.\help\���������Է������ʹ��˵��(�ڲ�).htm'',''-browser'')','Position',[0.80,0.95,0.1,0.05]);

% ��̬������UI
xb = 80;yb = 740;
uicontrol('Parent',hf,  'Style','text', 'String','��̬ת��','Tag','tzhuanxu',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[10 yb+30 50 20]);
zhuanxu = {'ZYX';'ZXY';'YXZ';'YZX';'XYZ';'XZY';'ZYX'};
hzhuanxu = uicontrol('Parent',hf,  'Style','popupmenu', 'String',zhuanxu,'Value',2,'Tag','zhuanxu',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[10 yb+5 65 20]);
hphi = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_phi),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb yb 50 20]);
htheta = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_theta),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+55 yb 50 20]);
hpsi = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_psi),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+110 yb 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','����(��)','Tag','tphi',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb yb+20 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','����(��)','Tag','ttheta',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+55 yb+20 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','ƫ��(��)','Tag','tpsi',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+110 yb+20 50 20]);
uicontrol('Parent',hf,  'Style','text', 'String','��̬������',...
    'FontUnits','pixels', 'FontSize',14,    'Position',[10 yb+50 230 20]);
% �������
hheight = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_orbit_height),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+165 yb 75 20]);
hbeta = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(default_beta),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+165+80 yb 85 20]);
uicontrol('Parent',hf,  'Style','text', 'String','����߶�(km)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+165 yb+20 75 20]);
uicontrol('Parent',hf,  'Style','text', 'String','̫���߶Ƚ�(��)',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+165+80 yb+20 85 20]);
uicontrol('Parent',hf,  'Style','text', 'String','�������',...
    'FontUnits','pixels', 'FontSize',14,    'Position',[xb+165 yb+50 160 20]);
% ���»�ͼ��ť
uicontrol('Parent',hf,  'Callback',@redraw, ...
    'String','�ػ�',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+260+75 yb 150 30]);
uicontrol('Parent',hf,  'Callback',@distpecent, ...
    'String','�����԰ٷֱ�',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+260+75 yb-30 150 30]);
% ���»�ͼ��ť
uicontrol('Parent',hf,  'Callback',@savecfg, ...
    'String','��������',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+260+75 yb+40 70 30]);
uicontrol('Parent',hf,  'Callback',@loadcfg, ...
    'String','��������',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+260+145 yb+40 70 30]);
% ������������
uicontrol('Parent',hf,  'Style','text', 'String','����A����',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 120 75 20]);
hsza = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(za'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 120 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','�ڹ��',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+360 120 50 20]);
haz = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(az),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+410 120 50 20]);

uicontrol('Parent',hf,  'Style','text', 'String','����B����',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 100 75 20]);
hszb = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(zb'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 100 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','�ڹ��',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+360 100 50 20]);
hbz = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(bz),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+410 100 50 20]);

uicontrol('Parent',hf,  'Style','text', 'String','����C����',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 80 75 20]);
hszc = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(zc'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 80 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','�ڹ��',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+360 80 50 20]);
hcz = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(cz),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+410 80 50 20]);

uicontrol('Parent',hf,  'Style','text', 'String','����D����',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 60 75 20]);
hszd = uicontrol('Parent',hf,  'Style','edit', 'String',['[',num2str(zd'),']'],...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 60 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','�ڹ��',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+360 60 50 20]);
hdz = uicontrol('Parent',hf,  'Style','edit', 'String',num2str(dz),...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+410 60 50 20]);
% �����̫����չԲ����
uicontrol('Parent',hf,  'Style','text', 'String','����Ӱ�췶Χ',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 40 75 20]);
hearthc = uicontrol('Parent',hf,  'Style','edit', 'String','30',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 40 275 20]);
uicontrol('Parent',hf,  'Style','text', 'String','̫��Ӱ�췶Χ',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb 20 75 20]);
hsunc = uicontrol('Parent',hf,  'Style','edit', 'String','30',...
    'FontUnits','pixels', 'FontSize',12,    'Position',[xb+80 20 275 20]);
% ������ͼ�ľ��
f3d = [];
f2d1 = [];
f2d2 = [];
% share value
beta = default_beta;
Cbo = eye(3);
% ����Ĭ��ͼ��
coordinate(); %����������
drawes(); %���Ƶ����̫��
% ������������
drawss();

%% ��ͼ
% �������ã��������̫��
    function drawes(hObject, eventdata, handles)
        h = str2double(get(hheight, 'String')); % orbit height
        rou = asind(Re/(Re+h)); % earth half angle
        beta = str2double(get(hbeta, 'String')); % ̫��������н�,-Yһ��Ϊ��
        getCbo();
        % ���Ƶ���
        ec = str2num(get(hearthc, 'String')); % ����Ӱ�췶Χ
        if ~isempty(ec)
            ec = ec(:)';
            ec = rou + [0,ec];
            ec = unique(sort(abs(ec)));
        else
            ec = rou;
        end
        earth(ec,Cbo);
        % ����̫��
        sc = str2num(get(hsunc, 'String')); % ̫��Ӱ�췶Χ
        if ~isempty(sc)
        	sc = sc(:)';
            sc = unique(sort(abs(sc)));
            sun(beta + [0,sc],Cbo);
            sun(beta - [0,sc],Cbo);
        else
            sun(beta,Cbo);
        end
    end
% �����������ã�������
    function drawss(hObject, eventdata, handles)
        za = str2num(get(hsza, 'String')); % ����A����
        zb = str2num(get(hszb, 'String')); % ����B����
        zc = str2num(get(hszc, 'String')); % ����C����
        zd = str2num(get(hszd, 'String')); % ����D����
        za = za(:);
        zb = zb(:);
        zc = zc(:);
        zd = zd(:);
        az = str2num(get(haz,'String')); % ����A�ڹ��
        bz = str2num(get(hbz,'String')); % ����B�ڹ��
        cz = str2num(get(hcz,'String')); % ����C�ڹ��
        dz = str2num(get(hdz,'String')); % ����D�ڹ��
        if ~isempty(za)
            sts(za,az,'A');
            fprintf('����A����: %f %f %f\n',acosd(za'));
        end
        if ~isempty(zd)
            sts(zb,bz,'B');
            sts(zc,cz,'C');
            sts(zd,cz,'D');
            abangle = acosd(za'*zb);
            acangle = acosd(zc'*za);
            bcangle = acosd(zc'*zb);
            adangle = acosd(za'*zd);
            bdangle = acosd(zb'*zd);
            cdangle = acosd(zc'*zd);
            fprintf('����B����: %f %f %f\n',acosd(zb'));
            fprintf('����C����: %f %f %f\n',acosd(zc'));
            fprintf('����D����: %f %f %f\n',acosd(zd'));
            fprintf('AB��������н�=%.2f\nBC��������н�=%.2f\nAC��������н�=%.2f\nAD��������н�=%.2f\nBD��������н�=%.2f\nCD��������н�=%.2f\n\n',...
                abangle,bcangle,acangle,adangle,bdangle,cdangle);
        elseif ~isempty(zc)
            sts(zb,bz,'B');
            sts(zc,cz,'C');
            abangle = acosd(za'*zb);
            acangle = acosd(zc'*za);
            bcangle = acosd(zc'*zb);
            fprintf('����B����: %f %f %f\n',acosd(zb'));
            fprintf('����C����: %f %f %f\n',acosd(zc'));
            fprintf('AB��������н�=%.2f\nBC��������н�=%.2f\nAC��������н�=%.2f\n\n',abangle,bcangle,acangle);	
        elseif ~isempty(zb)
            sts(zb,bz,'B');
            abangle = acosd(za'*zb);
            fprintf('����B����: %f %f %f\n',acosd(zb'));
            fprintf('AB��������н�=%.2f\n',abangle);	
        end
    end
% �ػ水ťcallback
    function redraw(hObject, eventdata, handles)
        set(f3d,'NextPlot','replace');
        set(f2d1,'NextPlot','replace');
        set(f2d2,'NextPlot','replace');
        coordinate();
        drawes();
        drawss();        
    end
% �����԰ٷֱ�
    function distpecent(hObject, eventdata, handles)
        h = str2double(get(hheight, 'String')); % orbit height
        rou = asind(Re/(Re+h)); % earth half angle
        beta = str2double(get(hbeta, 'String')); % ̫��������н�,-Yһ��Ϊ��
        getCbo();
        eb = Cbo*[0;0;1];
        sth = linspace(0,360,1800);
        nsth = length(sth);
        sph = beta*ones(size(sth));
        x = cosd(sph).*cosd(sth);
        y = cosd(sph).*sind(sth);
        z = sind(-sph);
        
        c = Cbo*rotx(pi/2)*[x;y;z];
        figure('Name','����������̫��������н�');
        grid on,hold on;        
        aga = nan;
        agb = nan;
        agc = nan;
        agd = nan;
        if ~isempty(za)
            aga = acosd(za'*c);
            pa = (1-sum(aga<=az(1))/nsth)*100;
            plot(sth,aga,'Color','k','linestyle','-');
        end
        if ~isempty(zb)
            agb = acosd(zb'*c);
            pb = (1-sum(agb<=bz(1))/nsth)*100;
            plot(sth,agb,'Color','k','linestyle','--');
        end
        if ~isempty(zc)
            agc = acosd(zc'*c);
            pc = (1-sum(agc<=cz(1))/nsth)*100;
            plot(sth,agc,'Color','k','linestyle',':');
        end
        if ~isempty(zd)
            agd = acosd(zd'*c);
            pd = (1-sum(agd<=dz(1))/nsth)*100;
            plot(sth,agd,'Color','k','linestyle','-.');
        end
        % ������Ե�н�
        if ~isempty(za)
            ase = acosd(za'*eb) - rou;
            plot(0,ase,'marker','o','markersize',4);
            text(3,ase,'����A������Ե�н�');
        end
        if ~isempty(zb)
            ase = acosd(zb'*eb) - rou;
            plot(0,ase,'marker','o','markersize',4);
            text(3,ase,'����B������Ե�н�');
        end
        if ~isempty(zc)
            ase = acosd(zc'*eb) - rou;
            plot(0,ase,'marker','o','markersize',4);
            text(3,ase,'����C������Ե�н�');
        end
        if ~isempty(zd)
            ase = acosd(zd'*eb) - rou;
            plot(0,ase,'marker','o','markersize',4);
            text(3,ase,'����D������Ե�н�');
        end
        xlim([0,360]),set(gca,'XTick',0:30:360);
        xlabel('̫����λ��(deg)'),ylabel('����������̫���н�(deg)');
        if isnan(agc)
            legend(['����A-',num2str(pa,'%.2f'),'%'],...
                   ['����B-',num2str(pb,'%.2f'),'%']);
        elseif isnan(agd)
            legend(['����A-',num2str(pa,'%.2f'),'%'],...
                   ['����B-',num2str(pb,'%.2f'),'%'],...
                   ['����C-',num2str(pc,'%.2f'),'%']);
        else
            legend(['����A-',num2str(pa,'%.2f'),'%'],...
                   ['����B-',num2str(pb,'%.2f'),'%'],...
                   ['����C-',num2str(pc,'%.2f'),'%'],...
                   ['����D-',num2str(pd,'%.2f'),'%']);
        end

        plot([0,360],[az(1) az(1)],'r');
        plot([0,360],[bz(1) bz(1)],'r');
        plot([0,360],[cz(1) cz(1)],'r');
        plot([0,360],[dz(1) dz(1)],'r');
    end
    function getCbo()
        phi = str2double(get(hphi, 'String'));
        theta = str2double(get(htheta, 'String'));
        psi = str2double(get(hpsi, 'String'));
        zv = get(hzhuanxu,'Value');
        for i=1:3
            switch zhuanxu{zv}(i)
                case 'Z'
                    angle(i) = psi;
                case 'Y'
                    angle(i) = theta;
                case 'X'
                    angle(i) = phi;
            end
        end
        Cbo = angle2dcm(angle(1)*rad,angle(2)*rad,angle(3)*rad,zhuanxu{zv});
    end
%% ͶӰ�㷨
% ����ͼͶӰ
    function [a,b] = MercatorSide(X,Y,Z)
        b = asin(-Z)/rad;
        a = atan2(X,-Y)/rad;
    end
% ����ͼͶӰ
    function [a,b] = MercatorTop(X,Y,Z)
        b = asin(X)/rad;
        a = atan2(Y,-Z)/rad;
%         jp = find(abs(diff(a))>180);
%         if ~isempty(jp)
%             a = [a(1:jp),nan,a(jp+1:end)];
%             b = [b(1:jp),nan,b(jp+1:end)];
%         end
    end

%% ����ͼ
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
        [AZ,EL] = view(f3d);
        colormap(f3d,0.9*[1 1 1;1 1 1]);
        mesh(f3d,X,Y,Z,'facealpha',0.5);
        set(f3d,'CameraViewAngle',5,'CameraViewAngleMode','manual',...
            'CameraTarget',[0 0 0],'CameraTargetMode','manual',...
            'NextPlot','add','Visible','off',...
            'Zdir','Reverse','Ydir','Reverse');
        view(f3d,[AZ EL]);
        xlabel(f3d,'X');ylabel(f3d,'Y');zlabel(f3d,'Z');d=0.1;
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
        f2d1 = subplot(2,2,2,'Parent',hf);
        set(f2d1,'Units','Pixel','Position',[650 457 460 300]);
        % axesm('MapProjection','mercator','MapLatLimit',[-90 90],'MapLonLimit',[-180 180]),grid on;
        d=4;
        plot(f2d1,-90,0,'marker','o','markeredgecolor',[1,0,0]);
        grid(f2d1,'on');hold(f2d1,'on');
        line(90,0,'Parent',f2d1,'marker','o','markerfacecolor',[1,0,0]);
        line(0,-90,'Parent',f2d1,'marker','o','markeredgecolor',[0,1,0]);
        line(0,90,'Parent',f2d1,'marker','o','markerfacecolor',[0,1,0]);
        line(0,0,'Parent',f2d1,'marker','o','markeredgecolor',[0,0,1]);
        line(180,0,'Parent',f2d1,'marker','o','markerfacecolor',[0,0,1]);
        line(-180,0,'Parent',f2d1,'marker','o','markerfacecolor',[0,0,1]);
        text(d,d,'-Y','Parent',f2d1);
        text(180+d,d,'+Y','Parent',f2d1);
        text(-180-d,d,'+Y','Parent',f2d1);
        text(90+d,d,'+X','Parent',f2d1);        text(-90-d,d,'-X','Parent',f2d1);
        text(d,90+d,'-Z','Parent',f2d1);        text(d,-90-d,'+Z','Parent',f2d1);
        axis(f2d1,[-180,180,-90,90]);        title(f2d1,'����ͼ');
        set(f2d1,'XTick',-180:30:180);   set(f2d1,'YTick',-90:30:90);
        
        f2d2 = subplot(2,2,4,'Parent',hf);
        set(f2d2,'Units','Pixel','Position',[650 100 460 310]);
        % axesm('MapProjection','mercator','MapLatLimit',[-90 90],'MapLonLimit',[-180 180]),grid on;
        d=4;
        plot(f2d2,90,0,'marker','o','markerfacecolor',[1,0,0]);
        grid(f2d2,'on');hold(f2d2,'on');
        line(-90,0,'Parent',f2d2,'marker','o','markeredgecolor',[1,0,0]);
        line(0,-90,'Parent',f2d2,'marker','o','markeredgecolor',[0,1,0]);
        line(0,90,'Parent',f2d2,'marker','o','markerfacecolor',[0,1,0]);
        line(0,0,'Parent',f2d2,'marker','o','markeredgecolor',[0,0,1]);
        line(180,0,'Parent',f2d2,'marker','o','markerfacecolor',[0,0,1]);
        line(-180,0,'Parent',f2d2,'marker','o','markerfacecolor',[0,0,1]);
        text(d,d,'-Z','Parent',f2d2);text(180+d,d,'+Z','Parent',f2d2);text(-180-d,d,'+Z','Parent',f2d2);
        text(d,90+d,'+X','Parent',f2d2);text(d,-90-d,'-X','Parent',f2d2);
        text(-90+d,d,'-Y','Parent',f2d2);text(90+d,d,'+Y','Parent',f2d2);
        axis(f2d2,[-180,180,-90,90]);title(f2d2,'����ͼ');
        set(f2d2,'XTick',-180:30:180);   set(f2d2,'YTick',-90:30:90);
    end

%% ����
    function earth(rou,Cbo)
        sth = linspace(0,360,720);
        nth = length(rou);
        for i = 1:nth
            if rou(i)>180
                continue;
            end
            sph = (90-rou(i))*ones(size(sth));
            x = cos(sph*rad).*cos(sth*rad);
            y = cos(sph*rad).*sin(sth*rad);
            z = sin(sph*rad);
            c = Cbo*[x;y;z];
            x = c(1,:);
            y = c(2,:);
            z = c(3,:);
            color = [0.95/nth*(i-1),0.95/nth*(i-1),1];
            line(x,y,z,'parent',f3d,'color',color,'linewidth',1.5);
            d=1.05;
            text(x(1)*d,y(1)*d,z(1)*d,['Earth' num2str(rou(i),'%.1f')],'parent',f3d);            
            [a,b] = MercatorSide(x,y,z);
            line(a,b,'Parent',f2d1,'color',color,'markersize',5.0,'linestyle','none','marker','.');
            % linem(b,a);
            % patch(a,b,'b','FaceAlpha',0.2);
            [a,b] = MercatorTop(x,y,z);
            line(a,b,'Parent',f2d2,'color',color,'markersize',5.0,'linestyle','none','marker','.');
%             linem(b,a);patchm(b,a,'b','FaceAlpha',0.2);
%             patch(a,b,'b','FaceAlpha',0.2);
        end
    end

%% ̫��
    function sun(beta,Cbo)
        sth = linspace(0,360,720);
        for i = 1:length(beta)
            if abs(beta(i))>90
                continue;
            end
            sph = beta(i)*ones(size(sth));
            x = cosd(sph).*cosd(sth);
            y = cosd(sph).*sind(sth);
            z = sind(-sph);
            c = Cbo*rotx(pi/2)*[x;y;z];
            x = c(1,:);
            y = c(2,:);
            z = c(3,:);
            
            color = [1,0.95/length(beta)*(i-1),0.95/length(beta)*(i-1)];
            line(x,y,z,'parent',f3d,'color',color,'linewidth',1.5);
            d=1.05;
            text(x(1)*d,y(1)*d,z(1)*d,['Sun' num2str(beta(i),'%.1f')],'parent',f3d);
            
            [a,b] = MercatorSide(x,y,z);
            line(a,b,'Parent',f2d1,'color',color,'markersize',5.0,'linestyle','none','marker','.');
%             patch(a,b,'r','FaceAlpha',0.2);
            [a,b] = MercatorTop(x,y,z);
            line(a,b,'Parent',f2d2,'color',color,'markersize',5.0,'linestyle','none','marker','.');
%             patch(a,b,'r','FaceAlpha',0.2);
        end
    end

%% ����
    function sts(za,angle,name)
        sth = linspace(0,360,720);
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
            line(x,y,z,'parent',f3d,'color','k','linewidth',1.5);
            d=1.1;
            text(ia(3,1)*d,ia(3,2)*d,ia(3,3)*d,name,'parent',f3d);
            line(ia(3,1),ia(3,2),ia(3,3),'parent',f3d,'marker','o','markerfacecolor',[1,0,0])
            
            [a b] = MercatorSide(x,y,z);
            line(a,b,'Parent',f2d1,'color','k','linewidth',1.0,'linestyle','none','marker','.','markersize',1);
            [a b] = MercatorSide(za(1),za(2),za(3));
            line(a,b,'Parent',f2d1,'color','c','marker','o','markerfacecolor',[0,0,1]);
            text(a,b,name,'Parent',f2d1);
            
            [a b] = MercatorTop(x,y,z);
            line(a,b,'Parent',f2d2,'color','k','linewidth',1.0,'linestyle','none','marker','.','markersize',1);
            [a b] = MercatorTop(za(1),za(2),za(3));
            line(a,b,'Parent',f2d2,'color','c','marker','o','markerfacecolor',[0,0,1]);
            text(a,b,name,'Parent',f2d2);
        end
    end

%% ����UI
% ��������
    function savecfg(hObject, eventdata, handles)
        [FileName,PathName] = uiputfile('*.txt','��������');
        if FileName~=0
            fp = fopen(fullfile(PathName,FileName),'w');
            fprintf(fp,'��̬ת��: %s\n',zhuanxu{get(hzhuanxu,'Value')});
            fprintf(fp,'������̬��: %s %s %s\n',get(hphi, 'String'),get(htheta, 'String'),get(hpsi, 'String'));
            fprintf(fp,'����߶�: %s\n',get(hheight, 'String'));
            fprintf(fp,'̫���߶Ƚ�: %s\n',get(hbeta, 'String'));
            fprintf(fp,'����A����: %s\n',get(hsza, 'String'));
            fprintf(fp,'����A�ڹ��: %s\n',get(haz, 'String'));
            fprintf(fp,'����B����: %s\n',get(hszb, 'String'));
            fprintf(fp,'����B�ڹ��: %s\n',get(hbz, 'String'));
            fprintf(fp,'����C����: %s\n',get(hszc, 'String'));
            fprintf(fp,'����C�ڹ��: %s\n',get(hcz, 'String'));
            fprintf(fp,'����D����: %s\n',get(hszd, 'String'));
            fprintf(fp,'����D�ڹ��: %s\n',get(hdz, 'String'));
            fprintf(fp,'����Ӱ�췶Χ: %s\n',get(hearthc, 'String'));
            fprintf(fp,'̫��Ӱ�췶Χ: %s\n',get(hsunc, 'String'));
            fclose(fp);
        end
    end
% ��������
    function loadcfg(hObject, eventdata, handles)
        [FileName,PathName] = uigetfile('*.txt','ѡ�����������ļ�');
        if FileName==0
            return
        end
        fp = fopen(fullfile(PathName,FileName),'r');
        while(~feof(fp))
            nl = fgetl(fp);
            if numel(nl)==0 || strcmp(nl(1),'#') 
                continue
            end
            ss = strsplit(nl,':');
            switch ss{1}
                case '��̬ת��'
                    token = strtok(ss{2});
                    for i=1:length(zhuanxu)
                        if strcmpi(zhuanxu{i},token)
                            break;
                        end
                    end
                    set(hzhuanxu,'Value',i);                    
                case '������̬��'
                    satt = sscanf(ss{2},'%f %f %f');
                    set(hphi,'String',    num2str(satt(1)));
                    set(htheta,'String',  num2str(satt(2)));
                    set(hpsi,'String',    num2str(satt(3)));
                case '����߶�'
                    set(hheight,'String', ss{2} );
                case '̫���߶Ƚ�'
                    set(hbeta,'String', ss{2} );
                case '����A����'
                    set(hsza,'String', ss{2} );
                case '����A�ڹ��'
                    set(haz,'String', ss{2} );
                case '����B����'
                    set(hszb,'String', ss{2} );
                case '����B�ڹ��'
                    set(hbz,'String', ss{2} );
                case '����C����'
                    set(hszc,'String', ss{2} );
                case '����C�ڹ��'
                    set(hcz,'String', ss{2} );
                case '����D����'
                    set(hszd,'String', ss{2} );
                case '����D�ڹ��'
                    set(hdz,'String', ss{2} );
                case '����Ӱ�췶Χ'
                    set(hearthc,'String', ss{2} );
                case '̫��Ӱ�췶Χ'
                    set(hsunc,'String', ss{2} );
            end
        end
        fclose(fp);
    end
    function MeasureDist(hObject, eventdata, handles)
        set(hf,'WindowButtonDownFcn',@wbdcb);
    end
% ��̬��ʾ��������ߺ���굱ǰ�������
    % ������ص�����
    function wbdcb(src,evnt)
        if strcmp(get(src,'SelectionType'),'normal')
            [x,y,str] = disp_point();
            hl = line('XData',x,'YData',y,'parent',f2d2,'Color','r'); % ���������Բ����
            ht = text(x,y,'0.0','parent',f2d2); % �������������Բ�н�����
            text(x,y,str,'parent',f2d2,'VerticalAlignment','bottom'); % �������
            drawnow
            set(src,'WindowButtonMotionFcn',@wbmcb);
        elseif strcmp(get(src,'SelectionType'),'alt')
            set(src,'WindowButtonMotionFcn','')
            [x,y,str] = disp_point();
            text(x,y,str,'parent',f2d2,'VerticalAlignment','bottom');
            drawnow
            set(hf,'WindowButtonDownFcn','');
        end
        % Ƕ�� ����ƶ��ص�����
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
                [xdat(i),ydat(i)]=MercatorTop(P(1),P(2),P(3));
            end
            set(hl,'XData',xdat,'YData',ydat);
            set(ht,'Position',[xdat(10) ydat(10)],'String',num2str(angleAB,'%0.3g'));
            drawnow
        end
    end
    % ȡ��ǰ�����λ��
    function [x,y,str] = disp_point()
        cp = get(f2d2,'CurrentPoint');
        x = cp(1,1);y = cp(1,2);
        str = ['(',num2str(x,'%0.3g'),', ',num2str(y,'%0.3g'),')'];
    end
% ��ͼ�����ֶ�����������������λ�ã������������������ڹ�ǡ�����������н�
    function manualInstallSS(hObject, eventdata, handles)
        ss = 1; % �����������
        set(hf,'WindowButtonDownFcn',@wbdcbx);        
        % ������ص�����
        function wbdcbx(src,evnt)
            if strcmp(get(src,'SelectionType'),'normal')
                [x,y,str] = disp_point();
                text(x,y,str,'parent',f2d2,'VerticalAlignment','bottom'); % �������
                if ss == 1
                    za = [sind(y); cosd(y)*sind(x); -cosd(y)*cosd(x) ];
                    sts(za,[30,90],'A');
                    hl = line('XData',x,'YData',y,'parent',f2d2,'Color','r'); % ���������Բ����
                    ht = text(x,y,'0.0','parent',f2d2); % �������������Բ�н�����
                    drawnow
                    set(src,'WindowButtonMotionFcn',@wbmcb1);
                    ss = ss+1;
                elseif ss == 2
                    zb = [sind(y); cosd(y)*sind(x); -cosd(y)*cosd(x) ];
                    sts(zb,[30,90],'B');
                    h21 = line('XData',x,'YData',y,'parent',f2d2,'Color','r'); % ���������Բ����
                    h22 = line('XData',x,'YData',y,'parent',f2d2,'Color','r'); % ���������Բ����
                    ht1 = text(x,y,'0.0','parent',f2d2); % �������������Բ�н�����
                    ht2 = text(x,y,'0.0','parent',f2d2); % �������������Բ�н�����
                    set(src,'WindowButtonMotionFcn',@wbmcb2);
                    ss = ss+1;
                elseif ss == 3
                    ss = ss+1;
                end
            elseif strcmp(get(src,'SelectionType'),'alt')
                set(src,'WindowButtonMotionFcn','')
                [x,y,str] = disp_point();
                text(x,y,str,'parent',f2d2,'VerticalAlignment','bottom');
                zc = [sind(y); cosd(y)*sind(x); -cosd(y)*cosd(x) ];
                sts(zc,[30,90],'C');
                abangle = acosd(za'*zb);
                acangle = acosd(zc'*za);
                bcangle = acosd(zc'*zb);
                fprintf('AB��������н�=%.2f,BC��������н�=%.2f,AC��������н�=%.2f\n',...
                    abangle,bcangle,acangle);
                fprintf('����A����: %f %f %f\n����B����: %f %f %f\n����C����: %f %f %f\n\n',...
                    acosd(za),acosd(zb),acosd(zc));
                set(hsza,'String',['[',num2str(za'),']']);
                set(hszb,'String',['[',num2str(zb'),']']);
                set(hszc,'String',['[',num2str(zc'),']']);
                drawnow
                set(hf,'WindowButtonDownFcn','');
            end
            % Ƕ�� ����ƶ��ص�����
            function wbmcb1(src,evnt)
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
                    [xdat(i),ydat(i)]=MercatorTop(P(1),P(2),P(3));
                end
                set(hl,'XData',xdat,'YData',ydat);
                set(ht,'Position',[xdat(10) ydat(10)],'String',num2str(angleAB,'%0.3g'));
                drawnow
            end
            function wbmcb2(src,evnt)
                [xn,yn,str] = disp_point();
                zc = [sind(yn); cosd(yn)*sind(xn); -cosd(yn)*cosd(xn) ];
                N = cross(zb,zc);    N=N/norm(N);
                angleBC = acosd(zb'*zc);
                alfa = linspace(0,angleBC,20);
                xdat = nan(20,1);
                ydat = nan(20,1);
                for i=1:20
                    q = [N*sind(alfa(i)/2);cosd(alfa(i)/2)];
                    c = q2c(q);
                    P = c'*zb;
                    [xdat(i),ydat(i)]=MercatorTop(P(1),P(2),P(3));
                end
                set(h22,'XData',xdat,'YData',ydat);
                set(ht2,'Position',[xdat(10),ydat(10)],'String',num2str(angleBC,'%0.3g'));
                drawnow

                N = cross(za,zc);    N=N/norm(N);
                angleAC = acosd(za'*zc);
                alfa = linspace(0,angleAC,20);
                xdat = nan(20,1);
                ydat = nan(20,1);
                for i=1:20
                    q = [N*sind(alfa(i)/2);cosd(alfa(i)/2)];
                    c = q2c(q);
                    P = c'*za;
                    [xdat(i),ydat(i)]=MercatorTop(P(1),P(2),P(3));
                end
                set(h21,'XData',xdat,'YData',ydat);
                set(ht1,'Position',[xdat(10),ydat(10)],'String',num2str(angleAC,'%0.3g'));
                drawnow
            end            
        end
    end
%% ������������
% ��Ԫ������������������
    function C = q2c(q)
        error(nargchk(1,1,nargin));
        C = [1.0 - 2.0*(q(2)^2+q(3)^2),    2.0*(q(1)*q(2)+q(3)*q(4)),    2.0*(q(1)*q(3)-q(2)*q(4));
            2.0*(q(1)*q(2)-q(3)*q(4)),    1.0 - 2.0*(q(1)^2+q(3)^2),    2.0*(q(2)*q(3)+q(1)*q(4));
            2.0*(q(1)*q(3)+q(2)*q(4)),    2.0*(q(2)*q(3)-q(1)*q(4)),    1.0 - 2.0*(q(1)^2+q(2)^2)];
    end
% ��X����ת����
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
            error('�������Ϊ������ʸ��');
        end
    end
% ��Y����ת����
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
            error('�������Ϊ������ʸ��');
        end
    end
% ��Z����ת����
    function R = rotz(angle)
        if isscalar(angle)
            R = [ cos(angle)  sin(angle)  0;
                -sin(angle)  cos(angle)  0;
                0            0      1];
        elseif isvector(angle)
            n = length(angle);
            angle = reshape(angle,1,1,n);
            R = [ cos(angle)    sin(angle)   zeros(1,1,n);
                -sin(angle)    cos(angle)    zeros(1,1,n);
                zeros(1,1,n)  zeros(1,1,n)   ones(1,1,n)];
        else
            error('�������Ϊ������ʸ��');
        end
    end
end