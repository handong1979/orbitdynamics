% 读取hdf5文件,将文件中的Dataset按照其名称命名，读取到‘caller'的空间内
% readhdf5(file)
% TODO:  做一个viewhdf5工具

function DatasetNames = guiplot(file)
if nargin == 0 %self test
    file = 'D:\BD-2A\simulate\GEO_simu\OutFile\Attitude.h5';
end
info = h5info(file);
np = length(info.Datasets);
DatasetNames = cell(np,1);
for k=1:np
    varname = info.Datasets(k).Name;
    DatasetNames{k} = varname;
%     exp = [varname,' = h5read(''',file,''',''/',varname,''');'];
%     evalin('caller',exp);
%     evalin('caller',[varname ' = squeeze(' varname ''');']);
end
% clear file info np exp k varname
ix = 0;
iy = 0;

hf = figure('Position',[5   35   710   420],'MenuBar','none',...
    'Name',file,'NumberTitle','off','Color',[0.9 0.9 0.8],...
    'Resize','off','HandleVisibility','on','Visible','on');
hfile = uicontrol('Parent',hf,'Style','text',...
    'FontUnits','pixels','FontSize',12,'HorizontalAlignment','left',...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[50 385 450 25],'String',file);
hlist = uicontrol('Parent',hf,'Style','Listbox',...
    'FontUnits','pixels','FontSize',12,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[50 60 250 320],'String',DatasetNames,'callback',@OnSel);
htip = uicontrol('Parent',hf,'Style','text',...
    'FontUnits','pixels','FontSize',12,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[50 40 250 20],'String','tip');
hplotsetx = uicontrol('Parent',hf,'Style','text',...
    'FontUnits','pixels','FontSize',22,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[380 310 200 50],'String','plotsetx');
hplotsety = uicontrol('Parent',hf,'Style','text',...
    'FontUnits','pixels','FontSize',22,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[380 250 200 50],'String','plotsety');

uicontrol('Parent',hf,'FontUnits','pixels','FontSize',12,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[320 310 50 50],'String','Set X:','Callback',@OnSetX);
uicontrol('Parent',hf,'FontUnits','pixels','FontSize',12,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[320 250 50 50],'String','Set Y:','Callback',@OnSetY);
uicontrol('Parent',hf,'FontUnits','pixels','FontSize',12,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[650 280 50 50],'String','Plot...','Callback',@OnPlot);

    function OnSel(hObject,~)
        i = get(hlist,'Value');
        dim = info.Datasets(i).Datatype.Type.Dims;
        set(htip,'String',[info.Datasets(i).Name,'[',num2str(dim(1)),'x',num2str(dim(2)),']']);
    end
    function OnSetX(hObject,~)
        ix = get(hlist,'Value');
        set(hplotsetx,'String',info.Datasets(ix).Name);
    end
    function OnSetY(hObject,~)
        iy = get(hlist,'Value');
        set(hplotsety,'String',info.Datasets(iy).Name);
    end
    function OnPlot(hObject,~)
        if ix > 0 && iy > 0
            xvarname = info.Datasets(ix).Name;
            exp = [xvarname,' = h5read(''',file,''',''/',xvarname,''');'];
            evalin('caller',exp);
            evalin('caller',[xvarname ' = squeeze(' xvarname ''');']);
            
            yvarname = info.Datasets(iy).Name;
            exp = [yvarname,' = h5read(''',file,''',''/',yvarname,''');'];
            evalin('caller',exp);
            evalin('caller',[yvarname ' = squeeze(' yvarname ''');']);
            
            evalin('caller',['figure,plot(',xvarname,',',yvarname,');']);
        end
    end
end

