// satellite time adjust

#include <OrbitDyn.h>

using namespace Constant;

CFacility fac(120,40,0.2);
double GLon,GLat,GAlt;
CDateTime epoch;
Kepler kp;

double delaysat;
double delaygrd;

typedef struct {
    CDateTime ts;
    CDateTime re;
    double dt;
} timepair;
vector<timepair> tl;

CDateTIme string2epoch(string s)
{
    int y, m, d, h, min;
    double sec;
    sscanf_s(s.c_str(), "%d-%d-%d %d:%d:%lf", &y, &m, &d, &h, &min, &sec);
    CDateTime t(y, m, d, h, min, sec);
    t = t - 1.0/3.0;
    return t;
}


void ReadOrbit(string filename)
{
    fstream file(filename,ios::in);
    if(!file.is_open())
        throw (string("Can't open file") + filename);
    std::string name,value;
    while(!file.eof()){
        //Semi_major_axis   =     1932.652806175639
        //Eccentricity      =    0.003606090476565559
        //Inclination       =     88.45177560605939
        //Ra_of_asc_node    =     265.2090045338037
        //Arg_of_pericenter =     120.0
        //Mean_anomaly      =     94.71685115019184
        if(ReadLine(&file,name,value))    {
            if(name == "Semi_major_axis")
               sscanf(value.c_str(),"%lf",&kp.a);
            else if(name == "Eccentricity")
                sscanf(value.c_str(),"%lf",&kp.e);
            else if(name == "Inclination")
                sscanf(value.c_str(),"%lf",&kp.i);
            else if(name == "Ra_of_asc_node")
                sscanf(value.c_str(),"%lf",&kp.o);
            else if(name == "Arg_of_pericenter")
                sscanf(value.c_str(),"%lf",&kp.w);
            else if(name == "Mean_anomaly")
                sscanf(value.c_str(),"%lf",&kp.M);
            else if(name == "Epoch")
                Epoch = string2epoch(Value);
        }
    }
}

double rou(CSatellite &sat,CFacility &fac)
{
    vec3 sp,sv;
    sat.GetECF(sp, sv);
    vec3 fp = fac.ECFPosition;
    return norm(sp-fp,2);
}

//! 校时量的计算
double adjusttime()
{
    CSatellite sat;
    sat.Initialize(epoch, kp);
    sat.Propagate(60, tl(0).ts);
    int n = tl.size();
    double dt,lastdt;
    double step;
    // 迭代每一个时间点的传输时延,求星时差
    for(unsigned int i=0;i<n;i++)
    {
        dt=lastdt = 0;
        sat.Propagate(60, tl(i).ts);
        do{
            lastdt = dt;
            dt = tl(i).re - tl(i).ts - delaysat - delaygrd - rou(sat,fac)/LightVel;
            step = tl(i).ts + dt + delaysat - sat.CurrentEpoch();
            if(step>0)
                sat.Propagate(1, step);
            else
                sat.PropagateBackward(-1, step);
        }while(fabs(lastdt-dt)<1e-6)
        tl.dt = dt;
    }
    
    // 求均值、初值和斜率
    double sum = 0;
    for(unsigned int i=0;i<n;i++)
    {
        sum += tl(i).dt;
    }
    double meandt = sum/n; // 集中校时
    double avgdt = 0;
    if(n>1)
        avgdt = (tl(n-1).dt - tl(0).dt)/(tl(n-1).ts - tl(0).ts)*60;
    
    FILE* ff = fopen('adjusttime.txt','w');
    fprintf(ff,"#集中校时时差值\n");
    fprintf(ff,"FocusAdjustTime = %.4f\n",meandt);
    fprintf(ff,"#均匀校时周期\n",60);
    fprintf(ff,"UniformAdjustTimePeriod = %f\n",60);
    fprintf(ff,"#均匀校时校正量\n");
    fprintf(ff,"UniformAdjustTime = %.4f",avgdt);
    fclose(ff);
}

int main(int argc, char* argv[])
{
    
    
    string of(argv[1]);
    ReadOrbit(of);
    
    fstream file(argv[2],ios::in);
    if(!file.is_open())
        throw (string("Can't open file") + filename);
    std::string name,value;
    while(!file.eof()){
//#星上时延
//Satdelay  = 0.1
//#地面时延
//Grounddelay = 0.1
//# 地面站位置信息
//GLon = 120
//GLat = 40
//GAlt = 0.2
//#遥测接收时间                                     遥测帧时间
//2018-10-01 00:30:22.000                  2018-10-01 00:30:20.000
//2018-10-01 00:30:23.000                  2018-10-01 00:30:21.000
        if(ReadLine(&file,name,value))    {
            if(name == "Satdelay")
                sscanf(value.c_str(),"%lf",&delaysat);
            else if(name == "Grounddelay")
                sscanf(value.c_str(),"%lf",&delaygrd);
            else if(name == "GLon")
                sscanf(value.c_str(),"%lf",&GLon);
            else if(name == "GLat")
                sscanf(value.c_str(),"%lf",&GLat);
            else if(name == "GAlt")
                sscanf(value.c_str(),"%lf",&GAlt);
        }
    }

	return 0;
}
