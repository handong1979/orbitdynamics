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

builddir = '~/Program/GitHub/orbitdynamics/bin';
dyfile = fullfile(builddir,'libOrbitDyn.dylib');
eval(['!cp ',dyfile,' libOrbitDyn.dylib']);

arch = computer('arch');
switch(arch)
    case 'win32'
        mex -v -I..\..\orbitdyn -L..\..\lib -lorbitdyn -lblas_win32_MT -llapack_win32_MT mexDE405.cpp
        mex -v -I..\..\orbitdyn -L..\..\lib -lorbitdyn -lblas_win32_MT -llapack_win32_MT mexDE421.cpp
        mex -v -Iinclude -Llib -lorbitdyn -lblas_win32_MT -llapack_win32_MT mexOrbitDyn.cpp
    case 'win64'
        mex -v -I..\..\orbitdyn -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexDE405.cpp
        mex -v -I..\..\orbitdyn -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexDE421.cpp
        mex -v -Iinclude -Llib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexOrbitDyn.cpp
    case 'maci64'
         mex -v -I../../dyn libOrbitDyn.dylib mexDE405.cpp
         mex -v -I../../dyn libOrbitDyn.dylib mexDE421.cpp
         mex -v -I../../dyn libOrbitDyn.dylib LINKLIBS='$LINKLIBS -framework Accelerate' mexOrbitDyn.cpp
        !otool -L mexOrbitDyn.mexmaci64
        !cp mexOrbitDyn.mexmaci64 ../
        !cp mexDE421.mexmaci64 ../
        !cp mexDE405.mexmaci64 ../
        !rm mexDE421.mexmaci64
        !rm mexDE405.mexmaci64
        !rm mexOrbitDyn.mexmaci64
    case 'glnxa64'
        error('TODO');
end
