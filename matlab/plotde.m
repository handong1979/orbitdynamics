function plotde(x,ytru,yest)
    handles = guidata(gcf);
    if isempty(handles) || ~isfield(handles,'figurede') 
       figurede();
    end
    a = gca;
%   set(gca,'GridLineStyle','{:}');
    tru = line('Parent',a,'Xdata',x,'Ydata',ytru,'Color','b','visible','on');
    est = line('Parent',a,'Xdata',x,'Ydata',yest,'Color','r','visible','on');
    err = line('Parent',a','Xdata',x,'Ydata',yest-ytru,'Color','k','visible','off');
    
    if ~isfield(handles,'nlines')
        handles.nlines = 1;
    else
        handles.nlines = handles.nlines + 1;
    end
    handles.tru(handles.nlines) = tru;
    handles.est(handles.nlines) = est;
    handles.err(handles.nlines) = err;
    guidata(gcf,handles);
end