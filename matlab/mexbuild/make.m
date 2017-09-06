% !copy ..\..\lib\OrbitDyn.lib
% !copy ..\mexDE421\mexDE421.cpp
% !copy ..\OrbitDynMatlab\OrbitDynMatlab.cpp OrbitDyn.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 mexde405.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 mexde421.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 OrbitDyn.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win32 -llapack_win32 -lsofa sofa.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexde405.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lblas_win64_MT -llapack_win64_MT mexde421.cpp
mex -v -Iinclude -Llib -lorbitdyn -lblas_win64_MT -llapack_win64_MT MexOrbitDyn.cpp
% mex -v -I..\..\include -L..\..\lib -lorbitdyn -lsofa sofa.cpp