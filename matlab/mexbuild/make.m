% !copy ..\..\lib\OrbitDyn.lib
% !copy ..\mexDE421\mexDE421.cpp
% !copy ..\OrbitDynMatlab\OrbitDynMatlab.cpp OrbitDyn.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 mexde405.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 mexde421.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 OrbitDyn.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 -lsofa sofa.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexde405.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexde421.cpp
% mex -v -Iinclude -Llib -lorbitdyn -lblas_win64_MT -llapack_win64_MT MexOrbitDyn.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lsofa sofa.cpp

arch = computer('arch');
switch(arch)
    case 'win32'
        mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32_MT -llapack_win32_MT mexDE405.cpp
        mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32_MT -llapack_win32_MT mexDE421.cpp
        mex -v -Iinclude -Llib -lorbitdyn -lblas_win32_MT -llapack_win32_MT mexOrbitDyn.cpp
    case 'win64'
        mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexDE405.cpp
        mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexDE421.cpp
        mex -v -Iinclude -Llib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexOrbitDyn.cpp
    case 'maci64'
         mex -v -I../../include -L../../lib libOrbitDyn.dylib mexDE405.cpp
         mex -v -I../../include -L../../lib libOrbitDyn.dylib mexDE421.cpp
         mex -v -I../../include -L../../lib libOrbitDyn.dylib LINKLIBS='$LINKLIBS -framework Accelerate' ...
             RUNPATHSEARTHPATHS='@loader_path' mexOrbitDyn.cpp
%         mex -v -I../../include -L../../lib libOrbitDyn.dylib LINKLIBS='$LINKLIBS -framework Accelerate' ...
%              mexOrbitDyn.cpp
        !otool -L mexOrbitDyn.mexmaci64
    case 'glnxa64'
        error('TODO');
end
