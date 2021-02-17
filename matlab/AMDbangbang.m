function [AMD_Mt,M_ACC,M_Slide] = AMDbangbang(Vmax,amax,phi0,phim)
% Bang-Bang轨迹规划算法产品规范
% 输入：
% 姿态机动最大角速度：Vmax（°/s）；
% 姿态机动最大角加速度：amax（°/s2）；
% 初始姿态角：phi0（°）；
% 机动目标角度：phim（°）；
% 仿真步长：tstep（s）
% 输出：
% 运动加速时间：M_ACC（s）
% 匀速滑行时间：M_Slide（s）
% 轨迹规划时间：AMD_M（s）

%a.	计算加/减速段时间、滑行段时间
M_ACC=Vmax/amax;
M_Slide=abs(phim-phi0)/Vmax-M_ACC;
%b.	计算期望角速度轨迹的转折时间点：
if M_Slide<0
    M_Slide=0;
    M_ACC=sqrt(abs(phim-phi0)/amax);
end
AMD_Mt = M_ACC+M_Slide+M_ACC;

%% traj
% tstep=0.1;
% EndN=ceil(AMD_Mt/tstep);
% tm1=M_ACC;
% tm2=tm1+M_Slide;
% tm3=tm2+M_ACC;
% for i=1:1:EndN
%     t(i)=i*tstep;
%     tm=t(i);
%     [ar_code(i),phir_code(i),dotphir_code(i)]=Model_ManeuverBangBang(tm,tm1,tm2,tm3,phim,phi0,amax);
% end
