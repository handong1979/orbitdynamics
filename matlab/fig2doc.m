% 自动截所有的图到word文件中
% 作者：林波
% 修改：韩冬
function fig2doc(bDoc)
if nargin == 0
    bDoc=1;
end
if(bDoc)
    strpath=pwd;
    filespec_user=sprintf('%s\\simureport.doc',strpath);
    try
        Word=actxGetRunningServer('Word.Application');
    catch
        Word=actxserver('Word.Application');
    end;
    set(Word,'Visible',1);
    documents=Word.Documents;
    if exist(filespec_user,'file')
        document=invoke(documents,'Open',filespec_user);
    else
        document=invoke(documents,'Add');
        %    document.Save;
    end
    
    content=document.Content;
    selection=Word.Selection;
    section=document.Section;
    Caption=Word.AutoCaption;
    paragraphformat=selection.ParagraphFormat;
    
    CaptionLabels=Word.CaptionLabels;
    CaptionLabel=CaptionLabels.Add('表'); %加入一个图标
    CaptionLabel=CaptionLabels.Add('图'); %加入一个图标
    
    end_of_doc=get(content,'end');
    selection.Start=content.end;   %设置光标到文档最后
    % selection.TypeParagraph;      %另取一段
end
% FigureTable = plotdata();
% nNumFig=length(FigureTable);
FigureTable = sort(get(0,'Children')); % 所有figure的句柄
nNumFig = length(FigureTable);
disp(sprintf('需要复制到Word的图形个数%d\n',nNumFig));
if(bDoc)
    for idfig=1:nNumFig
        zft=FigureTable(idfig);
        %hgexport(zft,'-clipboard');
        figure(zft);
        export_fig -transparent -clipboard
        selection.Range.Paste;
        selection.MoveRight(1,1);
        selection.TypeParagraph;
        
        disp(sprintf('Copy figure %d in Word\n',idfig));
        %        worstINDEX=WORST_PLAN(idfig);
        %        figureFootString=sprintf('图%d: %s,%s(工况%d),Bp=%2.0f,%s曲线',figureIdBase+idfig,axsString,conString,flgWorkingCondiction,Bp,comboString);
        
        %         if(zft.Userid)
        %             tmp=sprintf('图 %s-%d.%d %s',zft.strFooterDate,zft.idFooter,idfig,zft.footer);
        %             set(content,'Start',selection.Start);
        %             set(content,'Text',tmp);
        %             set(paragraphformat,'Alignment','wdAlignParagraphCenter');
        %             rr=document.Range(selection.Start,content.end);
        %             rr.Font.Size=10;
        %             selection.Start=content.end;
        %         else
        %             selection.InsertCaption('图',zft.footer);  %插入标题
        %             set(content,'Start',selection.Start);
        %         end
        set(paragraphformat,'Alignment','wdAlignParagraphLeft');
        %     selection.TypeParagraph;
        %     delete(zft);
    end
    %     document.Save;
    Word.delete;
end
end

function hfig=plotdata()
strpath=pwd;
filespec_user=sprintf('%s\\a0simcfg.txt',strpath);
fcfg=fopen(filespec_user,'r');
if fcfg<0
    disp(sprintf('读取文件%s失败\n',filespec_user));
else
    disp(sprintf('读取文件%s成功\n',filespec_user));
end
%strTmpMemTitle=fgets(fcfg);
tmp=fscanf(fcfg,'%s',1);
ChapterLevel=fscanf(fcfg,'%s',1);
strChapter=fscanf(fcfg,'%s',1);
strFooterDate=fscanf(fcfg,'%s',1);
idFooter=fscanf(fcfg,'%d',1);
Userid=fscanf(fcfg,'%d',1);
strTmpMemTitle=fgets(fcfg);
strLine=fscanf(fcfg,'%s',1);
strplotcfg=fscanf(fcfg,'%s',1);
fclose(fcfg);

filespec_user=sprintf('%s\\%s',strpath,strplotcfg);
fcfg=fopen(filespec_user,'r');

if fcfg<0
    disp(sprintf('读取文件%s失败\n',filespec_user));
else
    disp(sprintf('读取文件%s成功\n',filespec_user));
end
idx=1;

strLine=fgets(fcfg);
tmp=sscanf(strLine,'%s',1);
strLine=strLine(length(tmp)+1:length(strLine));
Pcomma=findstr(strLine,',');
sz=length(Pcomma);
for i=1:sz
    comma=findstr(strLine,',');
    if isempty(comma)
        LineType(i).color=strLine
        break;
    else
        tmp=strLine(1:comma(1)-1)
        strLine=strLine(1+comma(1):length(strLine));
        LineType(i).color=tmp
    end
end

strLine=fgets(fcfg);
tmp=sscanf(strLine,'%s',1);
strLine=strLine(length(tmp)+1:length(strLine));
Pcomma=findstr(strLine,',');
sz=length(Pcomma);
for i=1:sz
    comma=findstr(strLine,',');
    if isempty(comma)
        LineType(i).color=strLine
        break;
    else
        tmp=strLine(1:comma(1)-1)
        strLine=strLine(1+comma(1):length(strLine));
        LineType(i).marker=tmp
    end
end

strLine=fgets(fcfg);
tmp=sscanf(strLine,'%s',1);
strLine=strLine(length(tmp)+1:length(strLine));
MarkInfo=sscanf(strLine,'%d',2);

color=['r','g','b','c','m','y','k'];
Marker=['p','v','o','s','*','^','d'];

MarkerSize=MarkInfo(1);
MarkGap=MarkInfo(2);

while(~feof(fcfg))
    strDivLine=fgets(fcfg);  %各分线
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).PlotSize=sscanf(strLine,'%f',4);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).filename=sscanf(strLine,'%s',1);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).X=sscanf(strLine,'%s',1);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).Y=sscanf(strLine,'%s',1);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).xlabel=sscanf(strLine,'%s',1);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).ylabel=sscanf(strLine,'%s',1);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    tmp=sscanf(strLine,'%d',3);
    task(idx).gridon=tmp(1);
    task(idx).bmarkeron=tmp(2);
    task(idx).subplot=tmp(3);
    
    strLine=fgets(fcfg);
    tmp=sscanf(strLine,'%s',1);
    strLine=strLine(length(tmp)+1:length(strLine));
    task(idx).footer=sscanf(strLine,'%s',1);
    idx=idx+1;
end
fclose(fcfg);

NumTask=length(task);
disp(sprintf('画图个数%d\n',NumTask))

filename='nofile';
idxFig=1;
for i=1:NumTask
    cfile=task(i).filename;
    if strcmp(filename,cfile)==0
        filename=task(i).filename;
        fid=fopen(filename,'r');
        
        if fid<0
            disp(sprintf('读取文件失败%s\n',filename));
        else
            disp(sprintf('读取文件成功%s\n',filename));
        end
        sHeader=fgets(fid);
        sHeader=sscanf(sHeader,'%s');
        fout=fopen('outtmp.txt','w');
        while(~feof(fid))
            s1=fgets(fid);
            c=findstr(s1,',');
            b=c(1)+1;
            e=c(2)-1;
            s1(b:e)='0';
            fprintf(fout,'%s\n\r',s1);
        end
        fclose(fout);
        fclose(fid);
        value=load('outtmp.txt');
    end
    
    cntY=1;
    indexX=[];
    indexY=[];
    X=task(i).X;
    
    %找X轴数据
    c=findstr(sHeader,',');
    c2=findstr(sHeader,X);
    if isempty(c2)
        strtmp=sprintf('数据%s不存在',X);
        disp(strtmp);
    else
        nc=length(c)+1;
        for k=1:nc
            if k==1
                b=1;
                e=c(k);
            else
                if k==nc
                    b=c(k-1);
                    e=length(sHeader);
                else
                    b=c(k-1);
                    e=c(k);
                end
            end
            c22=c2(1);
            if c22>=b && c22<e
                indexX=k;
                break;
            end
        end
    end
    
    %找Y轴数据
    Ytmp=task(i).Y;
    Ycomma=findstr(Ytmp,',');
    for j=1:length(Ycomma)+1
        if j==1
            bY=1;
            if isempty(Ycomma)
                eY=length(Ytmp);
            else
                eY=Ycomma(j)-1;
            end
        else
            if j>length(Ycomma)
                bY=Ycomma(j-1)+1;
                eY=length(Ytmp);
            else
                bY=Ycomma(j-1)+1;
                eY=Ycomma(j)-1;
            end
        end
        
        Y=Ytmp(bY:eY);
        c=findstr(sHeader,',');
        c2=findstr(sHeader,Y);
        if isempty(c2)
            strtmp=sprintf('数据%s不存在',Y);
            disp(strtmp);
        else
            nc=length(c)+1;
            for k=1:nc
                if k==1
                    b=1;
                    e=c(k);
                else
                    if k==nc
                        b=c(k-1);
                        e=length(sHeader);
                    else
                        b=c(k-1);
                        e=c(k);
                    end
                end
                c22=c2(1);
                if c22>b && c22<e
                    indexY(cntY)=k;
                    PlotStruct(cntY).legend=Y;
                    cntY=cntY+1;
                    break;
                end
            end
        end
    end
    if ~isempty(indexY) && ~isempty(indexX)
        h1=figure('Units','centimeters','Position',task(i).PlotSize);
        %h=axes('position',[0.10,0.10,0.80,0.80],'Visible','on','NextPlot','Replace');
        %h1=figure;
        if(task(i).subplot)
            subnum=length(indexY);
            for n=1:length(indexY);
                subplot(subnum,1,n);
                hLine=plot(value(:,indexX),value(:,indexY(:,n)),LineType(n).color);
                ylabel(PlotStruct(n).legend);
            end
            xlabel(task(i).xlabel);
        else
            for n=1:length(indexY);
                hLine=plot(value(:,indexX),value(:,indexY(n)),LineType(n).color);
                hold on;
                hSGroup = hggroup;
                set(hLine,'Parent',hSGroup)
                nMarker=length(value(:,indexX));
                if task(i).bmarkeron
                    incgap=ceil(nMarker/MarkGap);
                    [vss idmax]=max(value(:,indexY(n)));  %在最大值上画符号
                    hh=plot(value(idmax,indexX),value(idmax,indexY(n)),LineType(n).color,'MarkerFaceColor',LineType(n).color,'Marker',LineType(n).marker,'MarkerSize',MarkerSize);
                    set(hh,'Parent',hSGroup);
                    hold on;
                    
                    [vss idmin]=min(value(:,indexY(n))); %在最小值上画符号
                    hh=plot(value(idmin,indexX),value(idmin,indexY(n)),LineType(n).color,'MarkerFaceColor',LineType(n).color,'Marker',LineType(n).marker,'MarkerSize',MarkerSize);
                    set(hh,'Parent',hSGroup);
                    hold on;
                    b=idmin;
                    if idmax<idmin
                        b=idmax;
                    end
                    delt=(max(value(:,indexX))-min(value(:,indexX)))/MarkGap;
                    incgap=incgap+5*n;
                    tlast=-10e10;
                    for k=b:incgap:nMarker
                        if abs(value(k,indexX)-tlast)>=abs(delt)/3
                            hh=plot(value(k,indexX),value(k,indexY(n)),LineType(n).color,'MarkerFaceColor',LineType(n).color,'Marker',LineType(n).marker,'MarkerSize',MarkerSize);
                            set(hh,'Parent',hSGroup);
                            hold on;
                            tlast=value(k,indexX);
                        else
                            continue;
                        end
                    end
                end
                
                set(get(get(hSGroup,'Annotation'),'LegendInformation'),...
                    'IconDisplayStyle','on');
                set(hSGroup,'DisplayName',PlotStruct(n).legend);
                hold on;
            end
            xlabel(task(i).xlabel);
            ylabel(task(i).ylabel);
            legend('show');
        end
        
        hfig(idxFig).h=h1;
        hfig(idxFig).footer=task(i).footer;
        hfig(idxFig).ChapterLevel=ChapterLevel;
        hfig(idxFig).strFooterDate=strFooterDate;
        hfig(idxFig).idFooter=idFooter;
        hfig(idxFig).Userid=Userid;
        idxFig=idxFig+1;
        if task(i).gridon
            grid on;
        end
    end
end
end




