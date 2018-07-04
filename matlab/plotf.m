% 取一个数组内的两列画曲线
function plotf(data,index1,index2)
plot(data(:,index1),data(:,index2)),grid on,hold on;