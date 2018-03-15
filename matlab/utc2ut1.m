% UTC时转化为UT1时
% utc2ut1(mjd) 将协调世界时mjd(简约儒略日表示)转化为世界时1(简约儒略日表示)
% Example:
%     utc2ut1(53736)
% See also: utc2tdt, date2mjd
function UT1 = utc2ut1(UTC)
persistent eop
% 在程序第一次运行到这里时,eop=[]，运行load eop.mat
% 由于eop是全局变量，当再次调用次函数时，eop已有值,则无需再次load
% 这里的eop与polewander中共用一个，也是只需要load一次即可
% 当运行了clear all以后，全局变量eop才被清除
if isempty(eop)
    load eop.mat;
end
if UTC > eop(length(eop),1) || UTC<eop(1,1)
    % 时间超出表中时间范围
    dut1 = 0.0;
else
    % 插值得到UTC时的dut1
    dut1 = interp1(eop(:,1),eop(:,4),UTC);
end
UT1 = UTC + dut1/86400;
