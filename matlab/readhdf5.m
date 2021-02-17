% 读取hdf5文件,将文件中的Dataset按照其名称命名，读取到‘caller'的空间内
% readhdf5(file)
% TODO:  做一个viewhdf5工具

function [DatasetNames,var] = readhdf5(file)
if nargin == 0 %self test
    file = 'D:\Project\hdf5test\pt.h5';
    [DatasetNames,var] = readhdf5(file);
end
info = h5info(file);
np = length(info.Datasets);
DatasetNames = cell(np,1);
for k=1:np
    varname = info.Datasets(k).Name;
    DatasetNames{k} = varname;
    if nargout <= 1
        exp = [varname,' = h5read(''',file,''',''/',varname,''');'];
        evalin('caller',exp);
        exp = [varname,' = shiftdim(',varname,',ndims(',varname,')-1);'];
        evalin('caller',exp);
    else
        exp = ['var.',varname,' = h5read(''',file,''',''/',varname,''');'];
        eval(exp);
        eval(['var.',varname,' = shiftdim(var.',varname,',ndims(var.',varname,')-1);']);
    end
end
% clear file info np exp k varname