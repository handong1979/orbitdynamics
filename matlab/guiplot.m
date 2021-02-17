% 读取hdf5文件,将文件中的Dataset按照其名称命名，读取到‘caller'的空间内
% readhdf5(file)
% TODO:  做一个viewhdf5工具

function DatasetNames = guiplot(file)
if nargin == 0 %self test
    file = [];
end
info = struct();
ix = 0;
iy = 0;
DatasetNames = {};
if ~isempty(file)
    openfile(file);
end

hf = figure('Position',[5   35   710   620],'MenuBar','none',...
    'Name','Read HDF5 Tool','NumberTitle','off','Color',[0.9 0.9 0.8],...
    'Resize','off','HandleVisibility','off','Visible','on');
uicontrol('Parent',hf,'FontUnits','pixels','FontSize',12,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[10 580 38 25],'String','Open..','Callback',@SelFile);
hfile = uicontrol('Parent',hf,'Style','text',...
    'FontUnits','pixels','FontSize',16,'HorizontalAlignment','left',...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[50 580 450 25],'String',file);
hlist = uicontrol('Parent',hf,'Style','Listbox',...
    'FontUnits','pixels','FontSize',16,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[50 60 250 500],'String',DatasetNames,'callback',@OnSel);
htip = uicontrol('Parent',hf,'Style','text',...
    'FontUnits','pixels','FontSize',16,...
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

uicontrol('Parent',hf,'FontUnits','pixels','FontSize',16,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[320 310 50 50],'String','Set X:','Callback',@OnSetX);
uicontrol('Parent',hf,'FontUnits','pixels','FontSize',16,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[320 250 50 50],'String','Set Y:','Callback',@OnSetY);
uicontrol('Parent',hf,'FontUnits','pixels','FontSize',16,...
    'BackgroundColor',get(0,'defaultuicontrolBackgroundColor'),...
    'Position',[650 280 50 50],'String','Plot...','Callback',@OnPlot);


    function openfile(file)
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
    end
    function SelFile(hObject,~)
        [filename, pathname] = uigetfile('*.h5','选择要打开的hdf数据文件','.');
        if isequal(filename,0)
           return
        end
        file = fullfile(pathname,filename);
        openfile(file);
        set(hlist,'String',DatasetNames);
        set(hfile,'String',file);
        ix = 0;
        iy = 0;
        set(hplotsetx,'String','plotsetx');
        set(hplotsety,'String','plotsety');
    end
    function OnSel(hObject,~)
        i = get(hlist,'Value');
        if ~isempty(i)
            dim = info.Datasets(i).Datatype.Type.Dims;
            set(htip,'String',[info.Datasets(i).Name,'[',num2str(dim(1)),'x',num2str(dim(2)),']']);
        end
        action = get(hf,'SelectionType');
        if strcmpi(action,'open')
            OnSetY(hObject);
            OnPlot(hObject);
        end
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
            evalin('base',exp);
            evalin('base',[xvarname ' = squeeze(' xvarname ''');']);
            
            yvarname = info.Datasets(iy).Name;
            exp = [yvarname,' = h5read(''',file,''',''/',yvarname,''');'];
            evalin('base',exp);
            evalin('base',[yvarname ' = squeeze(' yvarname ''');']);
            
            evalin('base',['figure,plot(',xvarname,',',yvarname,');']);
            xlabel(xvarname),ylabel(yvarname);
        end
    end
end

