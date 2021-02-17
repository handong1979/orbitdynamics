% ��ȡsp3�ļ�
function [name,time,pos] = readsp3(filename)
if nargin == 0 % self test
    warning('selftest');
    filename = 'IGSData/1773/igs17730.sp3';
end

fp = fopen(filename,'r');

%read head
tl = fgetl(fp);
ts = str2num(tl(38:39)); % ʱ���¼����һ��15����һ����һ���ļ�96��
tl = fgetl(fp);
tl = fgetl(fp);
ns = sscanf(tl(5:6),'%d'); % ������
n = tl(10:end);
tl = fgetl(fp);
n = [n,tl(10:end)];
name = reshape(n(1:ns*3),3,ns)'; % ��������
for i=1:18
    tl = fgetl(fp);
end

%read record
time = nan(ts,6);
pos = nan(ts,3,ns);
for t=1:ts
    tl = fgetl(fp);
    time(t,:) = sscanf(tl(2:end),'%d %d %d %d %d %f',6);
    for i=1:ns
        tl = fgetl(fp);
        if strcmp(tl(2:4),name(i,:))
            pos(t,:,i) = sscanf(tl(5:46),'%f %f %f',3);
        else
            error('read sp3 error�������������ļ�ͷ����,%s~=%s',tl(2:4),name(i,:));
        end
    end
end

fclose(fp);

