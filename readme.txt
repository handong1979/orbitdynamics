OrbitDyn V2.0.2(2015年1月7日)
* 将hdf5升级到1.8.14静态库,含Debug版本

OrbitDyn V2.0.1(2015年1月7日)
* 将hdf5升级到1.8.13静态库

OrbitDyn V2.0.0(2014年10月30日)
* 去掉Vector和Matrix，矩阵矢量运算全面升级到armadillo
* 添加hdf5,采用1.8.10

================================================================================

OrbitDyn V1.7.0(2014年2月20日)
* 增加sofa库
* 增加LibrationSat类
* Facility中修改地固位置计算

OrbitDyn V1.6.1(2013年6月4日)
* 修正nrlmsise-00.c中meso_tn3数组定义小的bug
* 优化外推性能，减少章动角的计算
* 矩阵乘法中增加并行处理，经过测试，性能提高不多，仍保持原状态。

OrbitDyn V1.6.0(2012年9月29日)
* 增加DE421，并默认使用该星历
* 增加Delay,PosVel两个类
* 增加矩阵和四元数的成员函数，扩充功能

OrbitDyn V1.5.4(2012年7月2日)
* 增加DE405目录
* Vector计算叉乘矩阵错误修正

OrbitDyn V1.5.3(2012年1月17日)
* 修改恒星时计算，提高精度；
* 地影模型修改为圆锥地影和圆柱地影两个函数，由光压函数调用圆锥地影模型
* matlab的mf函数改名为ma2ta

OrbitDyn V1.5.2(2011年9月2日)
* 增加无数据文件模式，运行时不需要读取任何数据文件

OrbitDyn V1.5.1(2011年3月3日)
* 扩充摄动项定义

OrbitDyn V1.5.0(2011年2月16日)
* 修改CDateTime内部的变量为整天数和当天的秒数
* 修改CRapidSatellite内部计时变量为毫秒数(整数)，避免截断误差的累计

OrbitDyn V1.4.5(2010年7月15日)
* 增加月球解析星历，将日月的DE405星历和解析星历统一由中心天体管理
* 增加不使用任何数据文件的模式，日月星历用解析星历、引力场参数使用前20阶、地球自转参数取为0
* 改变中心天体类的名称，不再加前缀C

OrbitDyn V1.4.4(2010年6月21日)
* 增加月球轨道的解析解计算；
* 修正CSatellite::GetECF()函数中地球自转角速度的错误；
* 更新m中的mexDE405.mexw32修正了天体对应整数的错误；更新orbitdyn.mexw32增加向前推和加推力的功能；增加ewmu.m函数；增加VC6下生成mex文件的工程；
* 更新压缩用的list.lst文件

OrbitDyn V1.4.3(2010年5月24日)
* 修改了VC6下环境变量的读取错误，暂时屏蔽了使用配置文件的功能

OrbitDyn V1.4.2(2010年5月19日)
* 更新VC6.0的支持

OrbitDyn V1.4.1(2010年5月12日)
* 梳理类的DLL导出，将所有的类均导出；修改与VC60兼容

OrbitDyn V1.4.0(2010年5月4日)
* 更新DE405星历，使用新的星历计算程序

OrbitDyn V1.3.5(2010年3月29日)
* Vector中增加Zero()函数

OrbitDyn V1.3.4(2010年3月24日)
* 更新mex文件及工程
* Vector中增加[]操作符重载，增加Zero()函数；
* matrix中恢复模板定义

OrbitDyn V1.3.3(2010年3月15日)
* Dll库中去掉HDFCL.H

OrbitDyn V1.3.2(2009年12月3日~2010年3月8日)
* 修改格林威治恒星时的计算；修改mex文件，增加平根数输出
* 修改极移的函数名称；
* 修正查找data目录中的错误；增加GetSGM函数；修正Facility.cpp中的错误；
* 更改CentralBody为Singlton；matlabmex中增加SetForce功能
* 清理异常；更改CDateTime定义，增加了UTCG、BJT等定义
* 更新说明文件；CDateTime中增加对秒数的取整处理；cart2Kepler函数中增加对e=0和输入值的保护；Satellite中增加了说明需要修改，但是还未更改；删除了VC6的lib工程文件

OrbitDyn V1.3.1(2009年11月27日)
* 增加MATLAB中的mex文件，增加了生成mex文件的工程matlabmex和mexde405,其中matlabmex用于生成OrbitDyn.mexw32轨
  道动力学外推程序，mexde405用于生成mexde405.mexw32行星历表程序，并增加了相应的.m辅助文件
* 更改OrbitDyn.h文件,仅支持VC2005以上版本的VC，不再支持VC6.0

OrbitDyn V1.3(2009年9月7日)
* 去掉全局变量，改用静态指针生成地球、月球、DE405等全局对象
* 更改异常处理，利用统一定义的异常基类，每个类定义自己的异常类型

OrbitDyn V1.2(2008年12月28日)
* 增加CRapidSatellite类。类重载了OneStep()函数，在其中每1800s更新岁差章动矩阵和日月位置，每50ms更新一次摄动加速度，
而在动力学方程DynFunction()直接使用计算好的摄动加速度。外推数值方法改为Gill4方法。从性能比较来看，原CSatellite外推
计算中最耗时的是地球非球形引力的计算，若将CRapidSatellite中的地球非球形引力计算放在动力学方程中，则计算耗时与
原CSatellite几乎一样。将地球非球形引力的计算放在OneStep()函数中，并每隔50ms才更新一次，则外推速度大大提高，可
用EarthFull中的相应测试函数测试速度提高的效果。
* CRapidSatellite只适用于外推步长很小(<50ms，如5ms)，外推时间不长(几千秒)，并对轨道外推精度要求不高的计算。

OrbitDyn V1.2(2008年7月25日)
* 更改大气密度计算中使用的高度算法，使用大地高度代替r-Re

OrbitDyn V1.1(2007年10月22日)
* 更改卫星的姿态变量，将Cbi改为qbi和wbi
* 删除CCentralBody中的StartEpoch变量
* 增加地球大气模型的选择，增加MSISE2000大气模型，并默认使用MSISE2000大气模型
* 更改PAirDrag()函数接口，增加时间和地固系位置两个变量，删除大气密度接口，由函数内部调用Earth.AtmosRuoH()计算大气密度
* CDateTime时间输出格式更改，增加BACCTIME格式

OrbitDyn V1.0(2007年8月29日)
* 为了便于发布，将代码放到cpp文件中，h文件中只保留定义
* 更改目录格式，改为bin\data\doc\include\lib\m\src\test形式
* 将代码subfunction.cpp更名为utility.cpp便于索引函数

EarthFull v5.3.1
* 修正环境变量没找到时的bug，当没有找到OrbitDyn时，getenv返回NULL，这时std::string DataDir(Name)时就会出错
* 修正轨道高度小于0时，计算大气密度时的溢出bug，当高度小于0时抛出异常
* 增加Propagate2Epoch()函数

EarthFull v5.3
* 增加了外推到远地点函数Propagate2Apogee()
* 卫星的时间不再是相对与Earth.StartEpoch的时间，而使用独立的历元Epoch,这样不同的卫星可以有不同的初始历元
* Earth.StartEpoch存在必要不大，但为了保持一定的继承性和考虑未来可能的需要，暂时保留
* 更新异常处理，采用throw exception方式
* 如果没有找到环境变量OrbitDyn,则使用当前目录下的data目录作为数据文件目录

EarthFull v5.2(2006年3月1日)
* 调整了时间的计算，在计算格林威治恒星时的时候要考虑dUT1
* 增加了外推到近地点函数Propagate2Perigee()
* 对轨道根数与直角坐标的计算中扩展到双曲线轨道
* 程序应用于环月轨道计算、中途修正等仿真，验证了程序计算法的正确性

EarthFull v5.1.1(2005年9月21日)
* 修正地影判断中的错误，增加锥形地影、月影的计算

EarthFull v5.1(2005年9月15日)
* 建立抽象基类CSatelliteBase作为所有卫星的基类，并以此建立环月卫星动力学
* 将头文件分开，以免编辑一个头文件导致所有cpp文件都需要重新编译
* 增加了CentralBody.h,Coordination.h,Facility.h,Kepler.h,Tank.h,Utility.h,Satellite.h头文件
* OrbitDyn.h只作为最后包含用
* 增加矩阵的内容，包括卫星的惯性姿态、岁差、章动等的矩阵运算，惯性系到地固系的转换改成矩阵运算
* 增加月心惯性系到月固系的转换函数，利用DE405中的月球天平动数据直接得到坐标系转换的三个欧拉角
* 增加工程OrbitDyn用作输出DLL，为此增加了头文件config.h定义dll输入输出
* 测试验证了勒让德球谐系数的计算，同Matlab和GSL相比在归一化的时候有不同之处。在此基础上整理了
* 引力场的计算公式和函数，重新推导和验算的引力场模型。通过环月轨道外推与STK比较，达到了较高的精度。

EarthFull v5.0
* 将轨道状态量（积分变量）改为惯性坐标
* 减少CSatellite中成员的个数
* 整理太阳、月球星历计算
* Keplerian更名为Kepler
* Kepler()更名为KeplerFunc()
* Kepler2()更名为KeplerFunc2()
* 将轨道摄动加速度的计算改为带量纲的，输出加速度为km/s^2
     其中地球引力加速度的计算使用原来的程序，只是在输入输出时进行量纲转化

EarthFull v4.2(2005年8月2日)
增加轨道机动，使用STL
在VC6.0中编译时有不同之处

EarthFull v4.1
将所有头文件合并成OrbitDyn.h，便于其他程序中引用

EarthFull v4.0(2004年11月22日)
大量更改...
存在问题：大气阻力的计算

EarthFull v3.0(2004年10月20日)
更改极移的数据读取方式
将日期时间、矢量运算分离成通用库文件
使程序结构更加合理

EarthFull v2.6(2004年5月26日)
增加Keplerian类
重载Satellite的operator =
增加是否自动保存轨道根数的选项
增加TVector和数组的相互转化
!更正RIC函数中的错误算法
!更正Cartesian_OrbitElements函数中rv的表达式错误

EarthFull v2.5(2004年5月14日)
经过54所项目的修改后返回的文件

EarthFull v2.3.4(2004年2月23日)
远程导引和伴飞使用单步调用

EarthFull v2.3.3(2004年1月15日)
远程导引基本成型了
伴飞还有部分问题
加入了cat写的C-W制导率和（视线制导率），完成逼近阶段的子程序

	使用单步调用，认为整个过程为控制过程，自由运动相当于控制量为0，
记录当前控制量为maneuver1，下次控制量为maneuver2，输出步长为StepOut,积
分步长由计算决定（为加快计算速度，可以采用变步长积分）。需要设置远
程导引阶段标志，变轨时，根据阶段标志确定下两次得方法及控制量。

EarthFull v2.3.2(2004年1月6日)
伴飞算是基本OK了，还有些小bug，对大偏心率问题仍然不适用，回头再说了
伴飞中用到了C++的默认拷贝处理，这部分还需进一步探讨
远程导引也完成一大部分了，中间还需优化

EarthFull v2.3(2003年12月27日)
结构任然在调整中，累~~~ :(
	改成了一个卫星类,应用轨道根数结构定义
两颗卫星的相对位置速度的计算,相对导航的计算
时间变量的变动

需要做的：“用函数来实现过程，用类来封装数据”

EarthFull v2.2(2003年12月10日)
结构化改造:	定义了Vector
派生新类Facility
结构大调整，累死我了......

EarthFull v2.1(2003年11月13日)
错误提示改成弹出消息窗口
变轨时，对于过小的dv，就不再执行
修正摄动计算过程中时间计算的错误，完成精度分析
输出初始参数到InitConditions.txt

EarthFull v2.0(2003年11月3日)
轨控策略的实现
查表求时间转换常数
改了摄动加速度函数名
改正了地球自转旋转矩阵种的计算错误
地球引力模型有：WGS-84、JGM-2、JGM-3、EGM-96
WGS-84模型较老，其地球半径、地球引力常数有不同之处

EarthFull v1.4(2003年10月7日)
实现有限推力变轨！包括：
	1.FiniteManeuver按照速度变化量关机
	2.tank的值在外推函数内更新，maneuver的值由实际外推的结果确定
	3.改写Tank::dv_dt()函数
DateTime的实现分离出DateTime.cpp
Tank和Maneuver的定义整合到Satellite中
改进DateTime、Satellite、XeSatellite的默认构造函数

EarthFull v1.3(2003年9月23日)
修正大气密度多项式插值中的错误
加入推力和贮箱参数计算，但没有调试完成，暂无变轨模块

EarthFull v1.2(2003年9月8日)
把结构体Time改成用类实现
完善封装，把RKF78集成到Satellite类中实现
把Satellite和XeSatellite的实现分成两个文件
大气密度由线性插值改为多项式插值
调用Matlab画图功能

EarthFull v1.1(2003年8月25日)
部分小修补

EarthFull v1.0(2003年7月23日)
这可是第一版啊:)好的开始是成功的一半
功能：外推计算
