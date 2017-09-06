#include "CentralBody.h"
#include "CDatetime.h"
#include "Utility.h"

CDateTimeFormat UTCG::format = YMD;

UTCG::UTCG()
	:nday(0),sec(0)
{ 
}

UTCG::UTCG(const UTCG& t)
{
	if(this!=&t){
		nday = t.nday;
		sec = t.sec;
	}
}

UTCG::UTCG(int y,int mon,int d,int h,int min,double s)
{
	SetTime(y,mon,d,h,min,s);
}

UTCG::UTCG(double ar[6])
{  
	SetTime((int)ar[0],(int)ar[1],(int)ar[2],(int)ar[3],(int)ar[4],ar[5]);
}

double UTCG::GetMJD() const
{
	return nday + sec/86400.0;
}

double UTCG::GetUTC() const
{
	return nday + sec/86400.0;
}

double UTCG::GetTDT() const
{
	return nday + sec/86400.0 + (GetdTAI()+32.184)/SolarDay;
}

double UTCG::GetUT1() const
{
	return nday + sec/86400.0 + GetdUT1()/SolarDay;
}

double UTCG::GetTDTCentNum() const
{
	return (GetTDT() - MJD2000)/36525.0;
}

double UTCG::GetUT1CentNum() const
{ 
	return (GetUT1() - MJD2000)/36525.0;
}

void UTCG::SetMJD(double mjd)
{
	nday = floor(mjd);
	sec = (mjd - nday)*86400.0;
}

void UTCG::SetTime(double utc)
{
	nday = floor(utc);
	sec = (utc - nday)*86400.0;
}

int UTCG::day_of_year() const 
{
	int y,m,d,h,min;
	double s;
	GetCalendar(nday + sec/86400.0,y,m,d,h,min,s);
	UTCG t0(y,1,1,0,0,0);
	return (int)(nday + sec/86400.0 - t0.GetMJD());
}

//! seconds in day
double UTCG::seconds_in_day() const 
{
	int y,m,d,h,min;
	double s;
	GetCalendar(nday + sec/86400.0,y,m,d,h,min,s);
	UTCG t0(y,m,d,0,0,0);
	return (nday + sec/86400.0 - t0.GetMJD()) * 86400;
}

void UTCG::ToChar(char* a) const
{
	int year,month,day,hour,minute;
	double second;
	GetCalendar(GetUTC(),year,month,day,hour,minute,second);
	char mm[4];
	switch(month)
	{
	case 1:
		strcpy_s(mm,"Jan");	break;
	case 2:
		strcpy_s(mm,"Feb");	break;
	case 3:
		strcpy_s(mm,"Mar");	break;
	case 4:
		strcpy_s(mm,"Apr");	break;
	case 5:
		strcpy_s(mm,"May");	break;
	case 6:
		strcpy_s(mm,"Jun");	break;
	case 7:
		strcpy_s(mm,"Jul");	break;
	case 8:
		strcpy_s(mm,"Aug");	break;
	case 9:
		strcpy_s(mm,"Sep");	break;
	case 10:
		strcpy_s(mm,"Oct");	break;
	case 11:
		strcpy_s(mm,"Nov");	break;
	case 12:
		strcpy_s(mm,"Dec");	break;
	default:
		CDateTimeException ecpt;
		ecpt.SetDetails("operator << : month number %d don't exit",mm);
		throw ecpt;
	}
#if _MSC_VER < 1400
	sprintf(a,"%d %s %4d %d:%d:%.3f",day,mm,year,hour,minute,second);
#else
	sprintf_s(a,25,"%d %s %4d %d:%d:%.3f",day,mm,year,hour,minute,second);
#endif // _MSC_VER < 1400
} 

void UTCG::GetTime(double time[])
{
	int y,m,d,h,min;
	double s;
	GetCalendar(nday + sec/86400.0,y,m,d,h,min,s);
	time[0] = y;
	time[1] = m;
	time[2] = d;
	time[3] = h;
	time[4] = min;
	time[5] = s;
}

void UTCG::SetTime(int year,int month,int day,int hour,int minute,double second)
{
	if(year<1972 || month>12||month<1 || day>31||day<1 || 
		hour>24||hour<0 || minute>60||minute<0 || second>60||second<0)
	{
		//if( (y<2010 && mon<10) || (y>2011 && mon>8) )
		{
			CDateTimeException excpt;
			excpt.SetDetails("date%d-%d-%d or time%d:%d:%.1f ranger error!",
				year,month,day,hour,minute,second);
			throw excpt;
		}
	}	

	nday = day - 32075 + 1461*( year + 4800 + (month-14)/12 )/4  //注意这里的除法为整除
		+367 * (month - 2 - (month-14)/12*12 ) /12
		-3*( ( year + 4900 + (month-14)/12 )/100 )/4
		-2400001;
	sec = double(hour)*3600.0 + double(minute)*60.0 + double(second);  //儒勒日数
}

/*! 查表计算dTAI=TAI-UTC
Dates (UTCG)                  TAI minus UTC (sec)
-----------------------      -------------------
1 Jan 1972 00:00:00.000                 10
1 Jul 1972 00:00:00.000                 11
1 Jan 1973 00:00:00.000                 12
1 Jan 1974 00:00:00.000                 13
1 Jan 1975 00:00:00.000                 14
1 Jan 1976 00:00:00.000                 15
1 Jan 1977 00:00:00.000                 16
1 Jan 1978 00:00:00.000                 17
1 Jan 1979 00:00:00.000                 18
1 Jan 1980 00:00:00.000                 19
1 Jul 1981 00:00:00.000                 20
1 Jul 1982 00:00:00.000                 21
1 Jul 1983 00:00:00.000                 22
1 Jul 1985 00:00:00.000                 23
1 Jan 1988 00:00:00.000                 24
1 Jan 1990 00:00:00.000                 25
1 Jan 1991 00:00:00.000                 26
1 Jul 1992 00:00:00.000                 27
1 Jul 1993 00:00:00.000                 28
1 Jul 1994 00:00:00.000                 29
1 Jan 1996 00:00:00.000                 30
1 Jul 1997 00:00:00.000                 31
1 Jan 1999 00:00:00.000                 32
1 Jan 2006 00:00:00.000                 33
1 Jan 2009 00:00:00.000                 34
1 Jul 2012 00:00:00.000                 35
*/
int UTCG::GetdTAI() const
{
	if(nday>=56109)      return 35; // 2012-7-1
	else if(nday>=54832) return 34; // 2009-1-1
	else if(nday>=53736) return 33; // 2006-1-1
	else if(nday>=51179) return 32; // 1999-1-1
	else if(nday>=50630) return 31; // 1997-7-1 
	else if(nday>=50083) return 30; // 1996-1-1
	else if(nday>=49534) return 29; // 1994-7-1
	else if(nday>=49169) return 28; // 1993-7-1
	else if(nday>=48804) return 27; // 1992-7-1
	else if(nday>=48257) return 26; // 1991-1-1
	else if(nday>=47892) return 25; // 1990-1-1
	else if(nday>=47161) return 24; // 1988-1-1
	else if(nday>=46066) return 23; // 1985-1-1
	else if(nday>=45516) return 22; // 1983-7-1
	else if(nday>=45151) return 21; // 1982-7-1
	else if(nday>=44786) return 20; // 1981-7-1
	else if(nday>=44239) return 19; // 1980-1-1
	else if(nday>=43874) return 18; // 1979-1-1
	else if(nday>=43509) return 17; // 1978-1-1
	else if(nday>=43144) return 16; // 1977-1-1
	else if(nday>=42778) return 15; // 1976-1-1
	else if(nday>=42413) return 14; // 1975-1-1
	else if(nday>=42048) return 13; // 1974-1-1
	else if(nday>=41683) return 12; // 1973-1-1
	else if(nday>=41499) return 11; // 1972-7-1
	else return 10; // 1972-1-1
}

/*! 查表得到dUT1=UT1-UTC
\return UT1-UTC
*/
double UTCG::GetdUT1() const
{	
	return Earth::Instance()->GetdUT1(*this);
}

//! 归一化为整天＋当天秒数
void UTCG::Normalize()
{
	while(sec>=86400.0)
	{
		nday++;
		sec -= 86400.0;
	}
	while(sec<0)
	{
		nday--;
		sec += 86400.0;
	}
}

///////////////////////
//成员运算符重载
///////////////////////
UTCG& UTCG::operator+=(double dt)//增加dt的时间(秒数)
{
	sec += dt;
	Normalize();
	return *this;
}

UTCG& UTCG::operator-=(double dt)//减少dt的时间(秒数)
{
	sec -= dt;
	Normalize();
	return *this;
}

std::ostream& operator<<(std::ostream& ostrm,const UTCG& t)
{
	int year,month,day,hour,minute;
	double second;
	double mjd = t.GetUTC();
	if(UTCG::format == BJT || UTCG::format == BACCTIME || UTCG::format == NUMTIME)
		mjd = mjd + 1.0/3.0;
	
	GetCalendar(mjd,year,month,day,hour,minute,second);

	char a[25];
	if( UTCG::format == YMD || UTCG::format == BJT )
	{
#if _MSC_VER < 1400
		sprintf(a,"%04d-%02d-%02d %02d:%02d:%06.3f",year,month,day,hour,minute,second);
#else
		sprintf_s(a,25,"%04d-%02d-%02d %02d:%02d:%06.3f",year,month,day,hour,minute,second);
#endif // _MSC_VER < 1400
		ostrm<<a;
	}
	else if( UTCG::format == STK )
	{
		t.ToChar(a);
		ostrm<<a;
	}
	else if( UTCG::format == BACCTIME )
	{
#if _MSC_VER < 1400
		sprintf(a,"%04d-%02d-%02dT%02d:%02d:%08.5f",year,month,day,hour,minute,second);
#else
		sprintf_s(a,25,"%04d-%02d-%02dT%02d:%02d:%08.5f",year,month,day,hour,minute,second);
#endif // _MSC_VER < 1400
		ostrm<<a;
	}
	else if( UTCG::format == NUMTIME )
	{
#if _MSC_VER < 1400
		sprintf(a,"%04d%02d%02d%02d%02d%f",year,month,day,hour,minute,second);
#else
		sprintf_s(a,25,"%04d%02d%02d%02d%02d%f",year,month,day,hour,minute,second);
#endif // _MSC_VER < 1400
		ostrm<<a;
	}
	return ostrm;
}


std::istream& operator>>(std::istream& istrm,UTCG& t)
{
	double utc;
	istrm>>utc;
	t.SetTime(utc);
	return istrm;
}

///////////////////////
//非成员运算符重载
///////////////////////
ORBITDYN_API UTCG operator+(const UTCG & t,double sec)//增加dt的时间(秒数)
{
	UTCG t2 = t;
	t2 += sec;
	return t2;
}

ORBITDYN_API UTCG operator-(const UTCG& t,double sec)//减少dt的时间(秒数)
{
	UTCG t2 = t;
	t2 -= sec;
	return t2;
}

//! 返回两个时间的差的秒数
ORBITDYN_API double operator-(const UTCG& t1,const UTCG& t0)
{
	return (t1.GetUTC()-t0.GetUTC())*SolarDay;
}
// 关系运算
//! 比较两个时间的大小
ORBITDYN_API bool operator>(const UTCG& t1,const UTCG& t0)
{
	return t1.GetUTC() > t0.GetUTC();
}
//! 比较两个时间的大小
ORBITDYN_API bool operator<(const UTCG& t1,const UTCG& t0)
{
	return t1.GetUTC() < t0.GetUTC();
}
//! 比较两个时间的大小
ORBITDYN_API bool operator>=(const UTCG& t1,const UTCG& t0)
{
	return t1.GetUTC() >= t0.GetUTC();
}
//! 比较两个时间的大小
ORBITDYN_API bool operator<=(const UTCG& t1,const UTCG& t0)
{
	return t1.GetUTC() <= t0.GetUTC();
}

//! 日期时间转化为MJD
ORBITDYN_API double MJD(int year,int month,int day,int hour,int minute,double second)
{
	return day - 32075 + 1461*( year + 4800 + (month-14)/12 )/4  //注意这里的除法为整除
		+367 * (month - 2 - (month-14)/12*12 ) /12
		-3*( ( year + 4900 + (month-14)/12 )/100 )/4
		-0.5-2400000.5+double(hour)/24.0
		+double(minute)/1440.0+double(second)/86400.0;  //儒勒日数
}

//! 四舍五入取整
int round(double d)
{
	if(d>INT_MAX || d<INT_MIN)
		throw std::overflow_error("round(): input overflow to make a integer"); 
	int n = (int)floor(d);
	if( d - n >= 0.5 ) return n+1;
	else return n;
	// boost中的算法：(去掉了类型转换和出错处理)
	// return d < 0 ? ceil(d - 0.5) : floor(d + 0.5);
}

//! MJD转化为日期时间
ORBITDYN_API void GetCalendar(double mjd,int &Y,int &M,int &D,int &h,int &min,double &s)
{
	// 增加浮点数的处理
	// 当输入为55197.999999999985(应该是2010-1-1,23:59:59.999...或2010-1-2,0:0:0)时，
	// 发生计算错误，计算J时数字自动进位，导致计算的日期正确(2010-1-2),但是在计算s时使用的方法为
	// s = (mjd-floor(mjd))*86400
	// 这里对mjd取整时，并没有进位，计算出来的时间是23:59:59.999...，与日期拼在一起之后导致多算了一天
	// bug reporter： 张志方   bug fixed：韩冬
	int mjdI = (int)floor(mjd);
	s = (mjd-mjdI)*86400;
	h = (int)floor(s/3600);
	min = (int)floor((s-h*3600)/60);
	s = s-h*3600-min*60;
	s = round(s*1e5)/1e5;
	if(s==60)
	{
		min++;
		s=0;
		if(min==60)
		{
			h++;
			min=0;
			if(h==24)
			{
				mjdI++;
				h = 0;
			}
		}
	}
	int J = (int)floor( mjdI+2400000.5+0.5 );
	int N = 4*(J+68569)/146097; //注意这里的除法为整除
	int L1 = J + 68569 - (N*146097+3)/4;
	int Y1 = 4000*(L1+1)/1461001;
	int L2 = L1 + 31- 1461*Y1/4;
	int M1 = 80*L2/2447;
	D = L2 - 2447*M1/80;
	int L3 = M1/11;
	M = M1 + 2 - 12*L3;
	Y = 100*(N-49) + Y1 + L3;
}

UTCG StrToDateTime(const char* str)
{
	int y,m,d,h,min;
	double sec;
	char c;
#if _MSC_VER < 1400
	sscanf(str,"%d%c%d%c%d%c%d%c%d%c%lf",&y,&c,&m,&c,&d,&c,&h,&c,&min,&c,&sec);
#else
	sscanf_s(str,"%d%c%d%c%d%c%d%c%d%c%lf",&y,&c,&m,&c,&d,&c,&h,&c,&min,&c,&sec);
#endif // _MSC_VER < 1400
	return UTCG(y,m,d,h,min,sec) - 8.0*3600.0;
}

#undef CDATETIME_BIAS

CDateTimeView::CDateTimeView()
{
}

CDateTimeView::CDateTimeView( const UTCG& t )
{
	GetCalendar(t.GetMJD(),_m.year,_m.month,_m.day,_m.hour,_m.minute,_m.second);
}

CDateTimeView::~CDateTimeView()
{
	//nothing to do 
}