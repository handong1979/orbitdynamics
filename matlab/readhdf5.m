% 读取hdf5文件,将文件中的Dataset按照其名称命名，读取到‘caller'的空间内
% readhdf5(file)
% TODO:  做一个viewhdf5工具

function DatasetNames = readhdf5(file)
if nargin == 0 %self test
    file = 'D:\Project\hdf5test\pt.h5';
end
info = h5info(file);
np = length(info.Datasets);
DatasetNames = cell(np,1);
for k=1:np
    varname = info.Datasets(k).Name;
    DatasetNames{k} = varname;
    exp = [varname,' = h5read(''',file,''',''/',varname,''');'];
    evalin('caller',exp);
    evalin('caller',[varname ' = squeeze(' varname ');']);
end
% clear file info np exp k varname