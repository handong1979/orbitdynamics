%% 创建数据－误差figure
function h = figurede(varargin)
h = figure;
set(h,'Position',[460   320   540*1.5   410*1.5]);
set(zoom,'Motion','vertical','Enable','on');
set(h,'Toolbar','figure');
for i=1:2:length(varargin)
    set(h,varargin{i},varargin{i+1});
end
% 曲线切换的按钮
p = get(h,'Position');
x = (p(3)-300)/2;
y = 0;%(p(4)-30)/2;
uicontrol('Style','CheckBox','String','真实值曲线',...
    'pos',[x y 100 30],'parent',h,'HandleVisibility','off',...
    'FontSize',12,'FontName','宋体','Value',1,...
    'Callback',@checktrue,'Tag','CheckBoxTrueValue');
uicontrol('Style','CheckBox','String','估计值曲线',...
    'pos',[x+100 y 100 30],'parent',h,'HandleVisibility','off',...
    'FontSize',12,'FontName','宋体','Value',1,...
    'Callback',@checkest,'Tag','CheckBoxEstValue');
uicontrol('Style','CheckBox','String','误差曲线',...
    'pos',[x+200 y 100 30],'parent',h,'HandleVisibility','off',...
    'FontSize',12,'FontName','宋体','Value',0,...
    'Callback',@checkerror,'Tag','CheckBoxErrorValue');

handles = guidata(gcf);
handles.figurede = 1;
guidata(gcf,handles);

    function checktrue(hObject,~)
        ud = guidata(hObject);
        if (get(hObject,'Value') == get(hObject,'Max'))
            for kk=1:ud.nlines
                set(ud.tru(kk),'Visible','On');
            end
        else
            for kk=1:ud.nlines
                set(ud.tru(kk),'Visible','Off');
            end
        end
    end
    function checkest(hObject,~)
        ud = guidata(hObject);
        if (get(hObject,'Value') == get(hObject,'Max'))
            for kk=1:ud.nlines
                set(ud.est(kk),'Visible','On');
            end
        else
            for kk=1:ud.nlines
                set(ud.est(kk),'Visible','Off');
            end
        end
    end
    function checkerror(hObject,~)
        ud = guidata(hObject);
        if (get(hObject,'Value') == get(hObject,'Max'))
            for kk=1:ud.nlines
                set(ud.err(kk),'Visible','On');
            end
        else
            for kk=1:ud.nlines
                set(ud.err(kk),'Visible','Off');
            end
        end
    end
end