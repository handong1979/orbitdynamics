
%
% Files
%   a2t                      - 由轨道半长轴计算轨道周期
%   AU                       - 天文单位距离(km)
%   Avv                      - 双矢量定姿
%   Ayrp                     - 姿态角==>方向余弦阵
%   c2q                      - 方向余弦阵＝＝》四元数
%   cart2kepler              - 惯性坐标转化为轨道根数 cart2kepler(cart,miu)
%   Constant                 - 设置常用的常数值，全部放在Const结构中
%   date2mjd                 - 年月日时分秒(世界时)转化为简约儒略日
%   deg                      - 常数，弧度->度
%   dfm2deg                  - 将度分秒表示的角度转换为度
%   dnda                     - 半长轴的小偏差造成轨道角速度的变化(rad)
%   drda                     - 半长轴的小偏差造成每个轨道周期的位置漂移量(km)
%   duda                     - 半长轴的小偏差造成每个轨道周期的相位漂移量(rad)
%   Earth                    - 绘制地图
%   eci2ecf                  - 惯性系到地固系转换矩阵
%   ecliptic_equator_angle   - 黄赤交角
%   GEarth                   - 地球引力常数
%   getcoi                   - 轨道坐标系相对地心惯性坐标系的旋转矩阵
%   gha                      - 格林威治真恒星时(Greenwich hour angle)
%   gham                     - 格林威治平恒星时(Greenwich hour angle)
%   ghamj2000                - 相对J2000平春分点的格林威治平恒星时(Greenwich hour angle)
%   GMoon                    - 月球引力常数
%   GSun                     - 太阳引力常数
%   h2n                      - 由轨道高度计算轨道平均角速度
%   hms2deg                  - 将时分秒表示的角度转换为度
%   hohmanntransfer          - 霍曼转移
%   hohmanntransfer_m        - 环月轨道的霍曼转移
%   J2                       - 地球引力J2常数
%   kdeg                     - 将轨道根数中的弧度转化为度
%   kepler2cart              - 由轨道根数计算位置速度
%   keplerfunc               - 解开普勒方程 M = E - e*sin(E)
%   kepleru                  - 由轨道六根数计算轨道幅角u
%   krad                     - 将轨道根数中的弧度转化为度
%   limit360                 - 将角度误差加减360变成0附近的值
%   limitpi                  - 将角度误差加减pi*2变成0附近的值
%   mjd2000                  - J2000.0的简约儒略日
%   mjd2date                 - 由简约儒略日计算日期
%   nutation                 - 章动矩阵计算
%   nutation_angle           - 章动角计算
%   oblate                   - 常数:地球赤道平均半径
%   plot2                    - 画两个subplot曲线
%   plot6                    - 绘制3×2的六个subplot曲线
%   plotorbit                - 以轨道根数elem为初值，按照二体运动绘制一个轨道周期的三维诡计
%   polewander               - 地球极移矩阵
%   pr                       - 岁差矩阵计算
%   precession               - 岁差矩阵计算
%   precession_angle         - 岁差角计算
%   q2c                      - 四元数＝＝》方向余弦阵
%   qim                      - 四元数除法 q = q1^-1 * q2
%   qm                       - 四元数乘法 q = q1*q2
%   rad                      - 常数 度->弧度
%   Re                       - 地球赤道半径
%   rkf78                    - RK78外推算法
%   Rm                       - 月球赤道半径
%   rotx                     - 绕X轴旋转矩阵
%   roty                     - 绕Y轴旋转矩阵
%   rotz                     - 绕Z轴旋转矩阵
%   sune                     - 太阳平均偏心率es
%   suni                     - 黄赤交角
%   sunm                     - 太阳平近点角Ms
%   sunsyn                   - 太阳同步轨道倾角、高度的计算
%   sunu                     - 太阳轨道的纬度幅角
%   sunw                     - 黄道近地点幅角ws
%   symc2q                   - 方向余弦阵-->四元数
%   t2a                      - 由轨道周期计算轨道半长轴
%   tcr                      - 由两颗星的轨道根数计算相对位置和相对速度   tcr(telem,celem)
%   trc                      - 已知目标根数及相对位置速度，求追踪星根数 trc(targetelem,rel)
%   utc2tdt                  - UTC时转化为TDT时
%   utc2ut1                  - UTC时转化为UT1时
%   VelPeAp                  - 由近地点半径、远地点半径计算近地点速度和远地点速度
%   We                       - 地球自转角速度常数
%   YRPa                     - 方向余弦阵==>姿态角
%   J3                       - 地球引力J3常数
%   J4                       - 地球引力J4常数
%   ewmu                     - 由e,w和M计算轨道纬度幅角u
%   getcdni                  - 东南坐标系相对惯性系的旋转矩阵
%   orbitdyn                 - 轨道动力学外推mex程序,用作卫星的轨道动力学外推,并可以同时外推多颗卫星的轨道
%   phaseplane               - 绘制直线型相平面分区图
%   phaseplane_parabola      - 绘制抛物线线型相平面分区图
%   phaseplanelogic          - 直线型相平面喷气控制逻辑
%   phaseplanelogic_parabola - 抛物线型相平面喷气控制逻辑
%   ppdesign                 - 直线型喷气相平面设计
%   ppdesign_parabola        - 抛物线型相平面设计
%   SunSyncRepeating         - 太阳同步回归轨道方程
%   cw2p                     - dv = cw2p(x0,xf,T,w0)
%   cwstm4                   - CW方程平面内状态转移矩阵
%   cwstm6                   - CW方程状态转移矩阵
%   cwtraj                   - 根据初始状态计算CW轨迹
%   de421                    - DE421星历表
%   find_stairs              - index = find_stairs(x)
%   find_time                - index = find_time(t,st)
%   lineargument             - 将限幅到0～2pi的幅角（存在从2pi跳变到0的点）调整成一条连续变化的幅角
%   lla2ecf                  - 
%   ma2ta                    - 平近点角M转换为真近点角f
%   pidparam                 - 计算二阶系统的PID控制参数
%   plot3axis                - 3*1的三条曲线
%   plotatt                  - 画姿态曲线
%   plotdatt                 - 画姿态角速度曲线
%   plotf                    - 
%   plotms                   - 
%   ppparam                  - 抛物线型相平面参数    
%   removebias               - remove an entire row of data containing the outlier
%   removeinvalid            - Remove any rows containing NaN or Inf from a matrix X.

%   ta2ma                    - 真近点角f转换为平近点角M
%   vector2crossmatrix       - 
%   xlsn2num                 - 将Excel表格的列号转换为数字
%   UD                       - 正定矩阵的UD分解
%   coe2see                  - 经典轨道根数==>小偏心率根数
%   de405                    - DE405星历表
%   dragforce                - 计算大气阻力
%   dvdm                     - 由速度增量计算燃料消耗
%   ecf2lla                  - ecf2lla 地固系位置转化为地心经纬度
%   jac_dadr_2body           - 地心引力加速度对位置矢量的雅克比矩阵
%   magnetfield              - 
%   orbitdynJ2               - 位置速度动力学方程
%   phi_orbitJ2              - 考虑J2项的位置速度轨道动力方程的状态转移矩阵
%   plot6coe                 - 画轨道根数曲线
%   plot6pv                  - 画轨道根数曲线
%   plotll                   - 画经纬度图
%   randunitvec              - 产生一个随机单位矢量
%   readhdf5                 - 读取hdf5文件,将文件中的Dataset按照其名称命名，读取到‘caller'的空间内
%   readsp3                  - 读取sp3文件
%   rk4                      - 龙格库塔4阶积分
%   see2coe                  - 小偏心率根数==>经典轨道根数
%   sofa                     - IAU sofa 国际天文联合会基础天文程序库
%   stsassemble              - 星敏安装、光干扰设计工具
%   symcoi                   - 轨道坐标系相对地心惯性坐标系的旋转矩阵
%   vecnorm                  - 求矢量的模
%   wheelenvelope            - 动量轮角动量包络
%   wrldmagmgeo              - WRLDMAGM Use World Magnetic Model.
