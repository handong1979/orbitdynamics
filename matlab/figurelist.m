
function figurelist
fontsize = 12;
hf = figure('Position',[1   1   550   620],'MenuBar','none',...
    'Name','FigureList','NumberTitle','off','Color',[0.9 0.9 0.8],...
    'Resize','off','HandleVisibility','off','Visible','on');

hdir = uicontrol('Parent',hf,  'Style','popupmenu',  'String',{'.';'run1';'run2';'run3';'run4'},...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',fontsize,    'Position',[20 400 180 50]);

hcmd = uicontrol('Parent',hf,  'Style','edit',  'String','draw',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',fontsize,    'Position',[20 330 180 50]);

uicontrol('Parent',hf,  'Callback',@execute,  'String','ִ��...',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',fontsize,    'Position',[20 260 180 50]);

uicontrol('Parent',hf,  'Callback',@export,  'String','���Ƶ�������',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',fontsize,    'Position',[20 190 180 50]);

uicontrol('Parent',hf,  'Callback',@listf,  'String','ˢ���б�',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',fontsize,    'Position',[20 120 180 50]);

uicontrol('Parent',hf,  'Callback',@closeallfigure,  'String','�ر���������',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',fontsize,    'Position',[20 50 180 50]);

flist = uicontrol('Parent',hf,'FontUnits','pixels',...
    'BackgroundColor',[1 1 1],'FontSize',fontsize,'Position',[230 50 300 550],...
    'String',{  },'Style','listbox','Value',1,...
    'Callback',@OnListBtnDown_Callback,'Tag','figurelist');

listf();

%% ��ȡ�������ߣ������б�
    function h = listf(hObject,~)
       set(flist,'String',{});
       fl = get(0,'Children');
%        fl = sort(fl);
       fn = cell(length(fl),1);
       num = nan(length(fl),1);
       for i=1:length(fl)
           % ���figure����
            nm = get(fl(i),'Name');
            if isempty(nm)
                % ���axes��title
                ax = get(fl(i),'Children');
                for j=1:length(ax)
                    tt = get(get(ax(j),'Title'),'String');
                    if ~isempty(tt)
                        nm = tt;
                        break; % ȡ��һ��axes��title��Ϊ����
                    end
                end
            end
            num(i) = fl(i).Number;
            fn{i} = [num2str(fl(i).Number),':',nm];
       end
       [~,idx] = sort(num);
       set(flist,'String',fn(idx));
    end

%% �ر���������
    function closeallfigure(hObject,~)
        button = questdlg('�ر��������ߣ�','ע��');
        switch button
            case 'Yes'
                close all;
                set(flist,'Value',1);
                set(flist,'String',{});
        end
    end

%% �б�ѡ��
    function OnListBtnDown_Callback(hObject,~)
        list = get(flist,'String');
        val = get(flist,'Value');
        if ~isempty(val)
            figure(sscanf(list{val},'%d'));
        end
    end

%% ����ͼ��ͼ��������
    function export(hObject,~)
        export_fig('-transparent','-clipboard');
    end

%% ִ�л�ͼָ��
    function execute(hObject,~)
        fd = get(hdir,'String');
        fdn = get(hdir,'Value')
        evalin('base',['global folder']);
        evalin('base',['folder = ''',fd{fdn},'''']);
        cmd = get(hcmd,'String');
        evalin('base',cmd);
        listf();
    end
end