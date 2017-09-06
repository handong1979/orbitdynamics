% 绘制3×2的六个subplot曲线
% plot a 3*2 subplot in current figure
% plot6(x)
% subplot is column order with x
function plot6(x,y,varargin)
if nargin == 0, error('Not enough input arguments.'); end

labeled = false;

if nargin==1
    hasoption = false;
else
    if nargin > 2 && strcmpi(varargin{1},'LABEL')
        labeled = true;
        str1 = varargin{2};
        str2 = varargin{3};
        str3 = varargin{4};
        str4 = varargin{5};
        str5 = varargin{6};
        str6 = varargin{7};
        if nargin > 8
            option = varargin(8:end);
            hasoption = true;
        else
            hasoption = false;
        end
    else
        option = varargin;
        hasoption = true;
    end
end

cm = colormap('lines');
cidx = get(gcf,'UserData');
if isempty(cidx)
    cidx = 1;
else
    cidx = cidx + 1;
    if cidx > 64
        cidx = cidx - 64;
    end
end
set(gcf,'UserData',cidx);

subplot(3,2,1);
plot(x,y(:,1),'Color',cm(cidx,:));grid on,hold on;
if hasoption
    np = length(option)/2;
    for i=1:2:np*2
        set(gco,option{i},option{i+1});
    end
end

subplot(3,2,3);
plot(x,y(:,2),'Color',cm(cidx,:));grid on,hold on;
if hasoption
    np = length(option)/2;
    for i=1:2:np*2
        set(gco,option{i},option{i+1});
    end
end

subplot(3,2,5);
plot(x,y(:,3),'Color',cm(cidx,:));grid on,hold on;
if hasoption
    np = length(option)/2;
    for i=1:2:np*2
        set(gco,option{i},option{i+1});
    end
end

subplot(3,2,2);
plot(x,y(:,4),'Color',cm(cidx,:));grid on,hold on;
if hasoption
    np = length(option)/2;
    for i=1:2:np*2
        set(gco,option{i},option{i+1});
    end
end

subplot(3,2,4);
plot(x,y(:,5),'Color',cm(cidx,:));grid on,hold on;
if hasoption
    np = length(option)/2;
    for i=1:2:np*2
        set(gco,option{i},option{i+1});
    end
end

subplot(3,2,6);
plot(x,y(:,6),'Color',cm(cidx,:));grid on,hold on;
if hasoption
    np = length(option)/2;
    for i=1:2:np*2
        set(gco,option{i},option{i+1});
    end
end

if labeled == true
  subplot(3,2,1),ylabel(str1);
  subplot(3,2,3),ylabel(str2);
  subplot(3,2,5),ylabel(str3);
  subplot(3,2,2),ylabel(str4);
  subplot(3,2,4),ylabel(str5);
  subplot(3,2,6),ylabel(str6);
end

