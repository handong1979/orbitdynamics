% IAU sofa �����������ϻ�������ĳ����
% sofa(function,param,...)
% functionΪһ���ַ�����Ϊsofa�еĺ������������ִ�Сд
% param,... Ϊsofa������Ҫ�Ĳ�����������µĺ���˵��
% Ŀǰ��ʵ�������µĺ�����
% 1. �������κ���ʱ���� 
%  GMST06
%  GST06
%  GST06A
%  GST00A
%  GST00B
%  GMST82
%  GST94
% 2. ������
%  
% Example:
% mjd = 53736;
% gst94 = sofa('gst94',mjd)
% gmst82 = sofa('gmst82',mjd)
% gst00a = sofa('gst00a',mjd)
% gst00b = sofa('gst00b',mjd)
% gmst06 = sofa('gmst06',mjd)
% gst06 = sofa('gst06',mjd)
% gst06a = sofa('gst06a',mjd)
% (gst94 - gst06)*deg*3600
% (gmst82 - gst06)*deg*3600
% (gst00a - gst06)*deg*3600
% (gmst06 - gst06)*deg*3600
% (gst06a - gst06)*deg*3600

%if ~libisloaded('sofa')
%  loadlibrary('sofa');
%end

% utc = 1:0.7:365*20;
% dlg = zeros(size(utc));
% for k = 1:length(utc)
%     dlg(k) = (sofa('gst94',utc(k)) - gha(utc(k)))*deg*3600*1000;
% end
% figure,plot(utc/365.25,dlg),grid on;
% xlabel('t(year)'),ylabel('d\lambda_G(mas)');
% for k = 1:length(utc)
%     dlg(k) = (sofa('gst00a',utc(k)) - gha(utc(k)))*deg*3600*1000;
% end
% figure,plot(utc/365.25,dlg),grid on;
% xlabel('t(year)'),ylabel('d\lambda_G(mas)');
% for k = 1:length(utc)
%     dlg(k) = (sofa('gst06',utc(k)) - gha(utc(k)))*deg*3600*1000;
% end
% figure,plot(utc/365.25,dlg),grid on;
% xlabel('t(year)'),ylabel('d\lambda_G(mas)');