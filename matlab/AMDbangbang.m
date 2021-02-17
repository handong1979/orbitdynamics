function [AMD_Mt,M_ACC,M_Slide] = AMDbangbang(Vmax,amax,phi0,phim)
% Bang-Bang�켣�滮�㷨��Ʒ�淶
% ���룺
% ��̬���������ٶȣ�Vmax����/s����
% ��̬�������Ǽ��ٶȣ�amax����/s2����
% ��ʼ��̬�ǣ�phi0���㣩��
% ����Ŀ��Ƕȣ�phim���㣩��
% ���沽����tstep��s��
% �����
% �˶�����ʱ�䣺M_ACC��s��
% ���ٻ���ʱ�䣺M_Slide��s��
% �켣�滮ʱ�䣺AMD_M��s��

%a.	�����/���ٶ�ʱ�䡢���ж�ʱ��
M_ACC=Vmax/amax;
M_Slide=abs(phim-phi0)/Vmax-M_ACC;
%b.	�����������ٶȹ켣��ת��ʱ��㣺
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
