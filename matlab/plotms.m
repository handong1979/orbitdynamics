% 画曲线并标注均值和RMS
function plotms(t,x)
m = mean(x);
s = std(x);
str = sprintf('Mean=%.2f,RMS=%.2f',m*1000,s*1000);
plot(t,x);
yl = get(gca,'ylim');
xl = get(gca,'xlim');
text(xl(1)+(xl(2)-xl(1))*0.1,yl(2)-(yl(2)-yl(1))*0.1,str,'BackgroundColor',[.7 .9 .7]);