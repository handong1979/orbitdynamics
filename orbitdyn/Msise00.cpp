#include "Msise00.h"
#include "Utility.h"

CMsise00 Msise;


/* ------------------------------------------------------------------- */
/* --------------------------- PROTOTYPES ---------------------------- */
/* ------------------------------------------------------------------- */

/* GTD7 */
/*   Neutral Atmosphere Empircial Model from the surface to lower
 *   exosphere.
 */
extern "C" void gtd7 (struct nrlmsise_input *input, \
           struct nrlmsise_flags *flags, \
           struct nrlmsise_output *output);


/* GTD7D */
/*   This subroutine provides Effective Total Mass Density for output
 *   d[5] which includes contributions from "anomalous oxygen" which can
 *   affect satellite drag above 500 km. See the section "output" for
 *   additional details.
 */
extern "C" void gtd7d(struct nrlmsise_input *input, \
           struct nrlmsise_flags *flags, \
           struct nrlmsise_output *output);


/* GTS7 */
/*   Thermospheric portion of NRLMSISE-00
 */
extern "C" void gts7 (struct nrlmsise_input *input, \
	   struct nrlmsise_flags *flags, \
	   struct nrlmsise_output *output);


/* GHP7 */
/*   To specify outputs at a pressure level (press) rather than at
 *   an altitude.
 */
void ghp7 (struct nrlmsise_input *input, \
           struct nrlmsise_flags *flags, \
           struct nrlmsise_output *output, \
           double press);

/* -------------------------------------------------------------------- */
/* ---------  N R L M S I S E - 0 0    M O D E L    2 0 0 1  ---------- */
/* -------------------------------------------------------------------- */

/* This file is part of the NRLMSISE-00  C source code package - release
 * 20020503
 *
 * The NRLMSISE-00 model was developed by Mike Picone, Alan Hedin, and
 * Doug Drob. They also wrote a NRLMSISE-00 distribution package in 
 * FORTRAN which is available at
 * http://uap-www.nrl.navy.mil/models_web/msis/msis_home.htm
 *
 * Dominik Brodowski implemented and maintains this C version. You can
 * reach him at devel@brodo.de. See the file "DOCUMENTATION" for details,
 * and check http://www.brodo.de/english/pub/nrlmsise/index.html for
 * updated releases of this package.
 */

//void test_gtd7(void) {
//  	struct nrlmsise_output output[17];
//	struct nrlmsise_input input[17];
//  	struct nrlmsise_flags flags;
//	struct ap_array aph;
//	int i;
//	int j;
//	/* input values */
//  	for (i=0;i<7;i++)
//		aph.a[i]=100;
//	flags.switches[0]=0;
//  	for (i=1;i<24;i++)
//  		flags.switches[i]=1;
//	for (i=0;i<17;i++) {
//		input[i].doy=172;
//		input[i].year=0; /* without effect */
//  		input[i].sec=29000;
//		input[i].alt=400;
//		input[i].g_lat=60;
//		input[i].g_long=-70;
//		input[i].lst=16;
//		input[i].f107A=150;
//		input[i].f107=150;
//		input[i].ap=4;
//	}
//	input[1].doy=81;
//	input[2].sec=75000;
//	input[2].alt=1000;
//	input[3].alt=100;
//	input[10].alt=0;
//	input[11].alt=10;
//	input[12].alt=30;
//	input[13].alt=50;
//	input[14].alt=70;
//	input[16].alt=100;
//	input[4].g_lat=0;
//	input[5].g_long=0;
//	input[6].lst=4;
//	input[7].f107A=70;
//	input[8].f107=180;
//	input[9].ap=40;
//	input[15].ap_a=&aph;
//	input[16].ap_a=&aph;
//	/* evaluate 0 to 14 */
//  	for (i=0;i<15;i++)
//  		gtd7(&input[i], &flags, &output[i]);
//	/* evaluate 15 and 16 */
//  	flags.switches[9]=-1;
//  	for (i=15;i<17;i++)
//  		gtd7(&input[i], &flags, &output[i]);
//	/* output type 1 */
//  	for (i=0;i<17;i++) {
//		printf("\n");
//		for (j=0;j<9;j++)
//			printf("%E ",output[i].d[j]);
//		printf("%E ",output[i].t[0]);
//		printf("%E \n",output[i].t[1]);
//		/* DL omitted */
//  	}
//
//	/* output type 2 */
//	for (i=0;i<3;i++) {
//		printf("\n");
//		printf("\nDAY   ");
//		for (j=0;j<5;j++)
//			printf("         %3i",input[i*5+j].doy);
//		printf("\nUT    ");
//		for (j=0;j<5;j++)
//			printf("       %5.0f",input[i*5+j].sec);
//		printf("\nALT   ");
//		for (j=0;j<5;j++)
//			printf("        %4.0f",input[i*5+j].alt);
//		printf("\nLAT   ");
//		for (j=0;j<5;j++)
//			printf("         %3.0f",input[i*5+j].g_lat);
//		printf("\nLONG  ");
//		for (j=0;j<5;j++)
//			printf("         %3.0f",input[i*5+j].g_long);
//		printf("\nLST   ");
//		for (j=0;j<5;j++)
//			printf("       %5.0f",input[i*5+j].lst);
//		printf("\nF107A ");
//		for (j=0;j<5;j++)
//			printf("         %3.0f",input[i*5+j].f107A);
//		printf("\nF107  ");
//		for (j=0;j<5;j++)
//			printf("         %3.0f",input[i*5+j].f107);
//		printf("\n\n");
//  		printf("\nTINF  ");
//		for (j=0;j<5;j++)
//			printf("     %7.2f",output[i*5+j].t[0]);
//		printf("\nTG    ");
//		for (j=0;j<5;j++)
//			printf("     %7.2f",output[i*5+j].t[1]);
//		printf("\nHE    ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[0]);
//		printf("\nO     ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[1]);
//		printf("\nN2    ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[2]);
//		printf("\nO2    ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[3]);
//		printf("\nAR    ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[4]);
//		printf("\nH     ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[6]);
//		printf("\nN     ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[7]);
//		printf("\nANM 0 ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[8]);
//		printf("\nRHO   ");
//		for (j=0;j<5;j++)
//			printf("   %1.3e",output[i*5+j].d[5]);
//			printf("\n");
//	}
//	printf("\n");
//}

CMsise00::CMsise00(void)
{
	int i;
	flags.switches[0]=1;
  	for (i=1;i<24;i++)
  		flags.switches[i]=1;

	input.f107A = 150;
	input.f107 = 150;
	input.ap = 14.4;   // ~= kp=3
	//  这里的Ap与STK中设置的Kp不是同一个参数，当Kp从0～9变化对应Ap从0～400
	// 具体对应关系见:汤锡生,《载人飞船轨道确定和返回控制》
}

CMsise00::~CMsise00(void)
{
}

/*! 计算大气密度
计算某一时间、地固系某一点的大气密度
\param t 时间
\param ECFr 地固坐标系位置
\return 大气密度，单位kg/m^3
*/
double CMsise00::Density(const CDateTime& t,const vec3& ECFr)
{
	// 使用大地地理坐标系的经纬度和高度--李克行
	CSpherical lla = ECF_LLA(ECFr);  
	input.alt = lla.Altitude;
	input.g_lat = lla.Latitude;
	input.g_long = lla.Longitude; 

	input.doy = t.day_of_year();
	input.sec = t.seconds_in_day();
	input.lst = input.sec/3600.0 + input.g_long/15.0;

	gtd7d(&input, &flags, &output);
	return output.d[5]/1000.0;
}
