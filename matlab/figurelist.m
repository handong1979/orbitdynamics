
function figurelist

hf = figure('Position',[1   1   550   620],'MenuBar','none',...
    'Name','FigureList','NumberTitle','off','Color',[0.9 0.9 0.8],...
    'Resize','off','HandleVisibility','off','Visible','on');

hdir = uicontrol('Parent',hf,  'Style','popupmenu',  'String',{'.';'run1';'run2';'run3';'run4'},...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',12,    'Position',[20 400 180 50]);

hcmd = uicontrol('Parent',hf,  'Style','edit',  'String','draw',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',12,    'Position',[20 330 180 50]);

uicontrol('Parent',hf,  'Callback',@execute,  'String','执行...',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',12,    'Position',[20 260 180 50]);

uicontrol('Parent',hf,  'Callback',@export,  'String','复制到剪贴板',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',12,    'Position',[20 190 180 50]);

uicontrol('Parent',hf,  'Callback',@listf,  'String','刷新列表',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',12,    'Position',[20 120 180 50]);

uicontrol('Parent',hf,  'Callback',@closeallfigure,  'String','关闭所有曲线',...
    'FontUnits','pixels',    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'FontSize',12,    'Position',[20 50 180 50]);

flist = uicontrol('Parent',hf,'FontUnits','pixels',...
    'BackgroundColor',[1 1 1],'FontSize',12,'Position',[230 50 300 550],...
    'String',{  },'Style','listbox','Value',1,...
    'Callback',@OnListBtnDown_Callback,'Tag','figurelist');

listf();

%% 读取所有曲线，加入列表
    function h = listf(hObject,~)
       set(flist,'String',{});
       fl = get(0,'Children');
       fl = sort(fl);
       for i=1:length(fl)
           % 获得figure名字
            nm = get(fl(i),'Name');
            if isempty(nm)
                % 获得axes的title
                ax = get(fl(i),'Children');
                for j=1:length(ax)
                    tt = get(get(ax(j),'Title'),'String');
                    if ~isempty(tt)
                        nm = tt;
                        break; % 取第一个axes的title作为名字
                    end
                end
            end
            lnow = get(flist,'String');
            fn = [num2str(fl(i)),':',nm];
            set(flist,'String',[lnow{:} {fn}]);
       end
    end

%% 关闭所有曲线
    function closeallfigure(hObject,~)
        button = questdlg('关闭所有曲线？','注意');
        switch button
            case 'Yes'
                close all;
                set(flist,'Value',1);
                set(flist,'String',{});
        end
    end

%% 列表选择
    function OnListBtnDown_Callback(hObject,~)
        list = get(flist,'String');
        val = get(flist,'Value');
        if ~isempty(val)
            figure(sscanf(list{val},'%d'));
        end
    end

%% 当期图截图到剪贴板
    function export(hObject,~)
        export_fig('-transparent','-clipboard');
    end

%% 执行绘图指令
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