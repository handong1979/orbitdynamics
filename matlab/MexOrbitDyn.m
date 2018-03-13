% 轨道动力学外推mex程序,用作卫星的轨道动力学外推,并可以同时外推多颗卫星的轨道
% out = orbitdyn(satllite_name,operator,parameter,...)
% satllite_name: 卫星名称
% operator: 操作,具体为(不区分大小小):
%     'init':初始化一颗卫星
%     'step':外推一步(支持向前推)
%     'Propagate':外推一段时间
%     'PropagateBackward':向前外推一段时间
%     'Propagate2Perigee':外推到近地点
%     'Propagate2Apogee':外推到远地点
%     'Propagate2AscendingNode':外推到升交点
%     'Propagate2DescendingNode':外推到降交点
%     'Propagate2Equator':外推到地球赤道升交点
%     'Impulse': 轨道系脉冲轨道机动
%     'remove':删除一颗卫星
%     'SetForce':设置摄动项
%     'GetMean':返回轨道平根数
%     'GetECF':返回地固下的位置速度
%     'SetMass':设置卫星质量
%     'SetSRP':设置光压面积
%     'SetAD': 设置大气阻力面积
% parameter: 对应operator的参数
% 输出:out为一个数组,包括J2000下的轨道根数和位置速度,数组中的数据依次为:
%      北京时t(MJD), 半长轴a(km), 偏心率e, 倾角i(rad),
%      升交点赤经Omega(rad),近地点幅角w(rad),平近点角M(rad),
%      位置x(km),位置y(km),位置z(km),速度x(km/s),速度y(km/s),速度z(km/s)
%
% 各种操作对应的参数为:
%     'init':参数parameter为一个数组,依次为:[年,月,日,时,分,秒,半长轴a(km),偏心率e,倾角i(deg)
%                                  ,升交点赤经Omega(deg),近地点幅角w(deg),平近点角M(deg),卫星质量(kg)]
%     'step':外推一步,支持向前推
%            不进行变轨时,参数parameter为步长（步长可正可负）
%            进行轨道机动时，需要提供的参数还要包含姿态、推力和质量变化率,格式如下：
%            [步长  qx  qy  qz  qs  Fbx  Fby  Fbz  dot_m]
%            qx  qy  qz  qs 为卫星本体系相对惯性系的四元数
%            一台发动机开机时质量变化率为 dot_m = - F/(Isp*9.8)
%            多台发动机同时开机时，推力Fbx  Fby  Fbz为卫星本体系的合成推力，质量变化率为各发动机的和
%     'Propagate':外推一段时间,参数parameter为时长(时长为正)
%     'PropagateBackward':向前外推一段时间,参数parameter为时长(时长为负)
%     'Propagate2Perigee':无参数
%     'Propagate2Apogee':无参数
%     'Propagate2AscendingNode':无参数
%     'Propagate2DescendingNode':无参数
%     'Propagate2Equator':无参数
%     'Impulse': 轨道系脉冲轨道机动，参数为三轴速度增量，单位km/s
%     'remove':无参数
%     'SetForce':设置摄动项，参数同C++程序中的SetForce()函数，为[p1,p2],p1为引力场阶数，
%                     p2为各种摄动项，摄动项同C++中的宏定义，对应的整数见下表：
%                     需要考虑多种摄动时，将下面对应整数求和得到p2
%                                 1:带谐项
%                                 2:田谐项
%                                 4:大气阻力
%                                 8:月球引力
%                                 16:太阳引力
%                                 32:太阳光压
%                    摄动设置的默认值为6,ODP_EARTH_ALL
%     'SetSRPCODE':设置CODE光压模型参数，参数为一个3*3的光压系数矩阵
%     'GetMean':无参数，返回轨道平根数
% 
% 一旦初始化一颗卫星,则它一直被保留在内存中,直到执行了remove操作或者执行了命令clear all或命令clear orbitdyn
%
% 允许初始化一颗已经存在的卫星,便于重复使用,程序会给出警告提示
%
% Example:
%   初始化一颗名为sat的卫星:
%   initparam = [2010,1,1,0,0,0,7044,0.001,98.5,270,90,30];
%   out = orbitdyn('sat','init',initparam);
%   设置轨道摄动模型为仅考虑J2项
%   orbitdyn('sat','SetForce',[2 1]);
%   将'sat'卫星的轨道外推60秒:
%   out = orbitdyn('sat','step',60);
%   执行脉冲轨道机动，沿X方向1m/s
%   out = orbitdyn('sat','impulse',[1,0,0]/1000)

% 示例：
% epoch = [2015,1,1,4,0,0];
% kp = [24371.137,0.73,28,0,0,0];
% F = 0.2;
% Isp = 2400;
% dm = -F/Isp/9.8;
% Mass = 2000;
% s1 = mexOrbitDyn('sat','init',[epoch,kp,Mass]);
% mexOrbitDyn('sat','SetForce',[8,1+2+4+8+16+32]);
% mexOrbitDyn('sat','SetAD',20); % 阻力面积
% mexOrbitDyn('sat','SetSRP',20); % 光压面积
% step = 60;
% for i = 1:1000
%     qbi = c2q(getcoi(s1(2:7)));
%     s1 = mexOrbitDyn('sat','step',[step, qbi', 0, 0, F, dm]);
% end