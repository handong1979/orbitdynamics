% 姿态动力学程序,用作卫星的姿态动力学外推,并可以同时外推多颗卫星的轨道
% out = mexAttitudedyn(satllite_name,operator,parameter,...)
% satllite_name: 卫星名称
% operator: 操作,具体为(不区分大小小):
%     'init':初始化一颗卫星
%     'step':外推一步(支持向前推)
%     'Propagate':外推一段时间
% parameter: 对应operator的参数
% 输出:out为一个数组,包括J2000下的轨道根数和位置速度,数组中的数据依次为:
%      惯性四元数qbi,惯性角速度wbi
%
% 各种操作对应的参数为:
%     'init':参数parameter为一个数组,依次为:[惯性四元数qbi,惯性角速度wbi]
%     'step':积分一步
%            [步长  Hwb  Tw  Tex]
%            Hwb为卫星轮系的合成角动量
%            Tw为轮系的合成力矩
%            Tex为推力器合成三轴力矩
%     'remove':无参数
%     'SetIs':设置惯量,参数为3*3惯量矩阵
% 
% 一旦初始化一颗卫星,则它一直被保留在内存中,直到执行了remove操作或者执行了命令clear all或命令clear orbitdyn
%
% 允许初始化一颗已经存在的卫星,便于重复使用,程序会给出警告提示
%
% Example:
%   初始化一颗名为sat的卫星:
%   att = [0 0 0 1 0.0 0.0 0.0];
%   mexAttitudedyn('a','init',att)
%   将'sat'卫星的轨道外推1秒:
%   Hwb = [0 10 0];
%   Tw = [0 0.1 0];
%   Tew = [2 0.0 2];
%   mexAttitudedyn('a','step',[1,Hwb,Tw,Tew])

% 示例：
% att = [0 0 0 1 0.0 0.0 0.0];
% mexAttitudedyn('a','init',att);
% clear all
% att = [0 0 0 1 0.0 0.0 0.0];
% mexAttitudedyn('a','init',att);
% mexAttitudedyn('a','getis')
% Is = magic(3)
% mexAttitudedyn('a','setis',Is)
% mexAttitudedyn('a','getis')
% Hwb = [0 10 0];
% Tw = [0 0.1 0];
% Tew = [2 0.0 2];
% mexAttitudedyn('a','step',[1,Hwb,Tw,Tew])
% Hwb = [0 10 0];
% Tw = [0 0.1 0];
% Tew = [2 0.0 2];
% mexAttitudedyn('a','step',[1,Hwb,Tw,Tew])