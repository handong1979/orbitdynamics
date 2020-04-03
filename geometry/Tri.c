#include <stdio.h>
#include <math.h>
#include <conio.h>
#define PI 3.141592
  float A,B,C,a,b,c;

void getab() /* 已知A,B,C,c，用正弦公式求a,b */
{ float tt,aa,bb;
	tt=sin(C)/sin(c);
	aa=tt*sin(A);bb=tt*sin(B);
	if(aa>=-1&&aa<0) a=asin(aa)+PI/2;
	else if(aa>=0&&aa<=1) a=asin(aa);
	if(bb>=-1&&bb<0) b=asin(bb)+PI/2;
	else if(bb>=0&&bb<=1) b=asin(bb);
}

void getAB()  /* 已知a,b,c,C,用正弦公式求A,B */
{ float tt,aa,bb;
	tt=sin(C)/sin(c);
	aa=tt*sin(a); bb=tt*sin(b);
	if(aa>=-1&&aa<0) A=asin(aa)+PI/2;
	else if(aa>=0&&aa<=1) A=asin(aa);
	if(bb>=-1&&bb<0) B=asin(bb)+PI/2;
	else if(bb>=0&&bb<=1) B=asin(bb);
}

void getCc()  /* 已知a,b,A,B,求c,C */
{
float cosc,cosC;
cosc=(cos(a)*cos(b)-cos(A)*cos(B)*sin(a)*sin(b))/(1-sin(A)*sin(B)*sin(a)*sin(b));
if(cosc>=-1&&cosc<=1)  c=acos(cosc);
cosC=-cos(A)*cos(B)+sin(A)*sin(B)*cos(c);
if(cosC>=-1&&cosC<=1)  C=acos(cosC);
}

float trans(float xx) /*把角度x转化为弧度*/
{
	return(xx*PI/180);
}

void out(float y,int *x) /*把角度x转化为度分秒形式*/
{ float xx;
	y=y/PI*180;
	*x=floor(y);
	xx=(y-*x)*60;
	*(x+1)=floor(xx);
	*(x+2)=floor((xx-*(x+1))*60);
}

void deg() /*转化工具*/
{ char s;
  int d,f,m;
  float h;
do{	cprintf("请选择:\n\r");
 	cprintf("1.度分秒--->度数\n\r");
	cprintf("2.弧度--->度数\n\r");
	cprintf("3.时分秒--->度数\n\r");
	cprintf("0.返回\n\r");
	s=getch();
	cprintf("\n\r");
	if(s=='1') {cprintf("度"); scanf("%d",&d);
			  cprintf("分"); scanf("%d",&f);
			  cprintf("秒"); scanf("%d",&m);
			  cprintf("度数为%f\n\r",d+f/60.0+m/3600.0);}
	else if(s=='2') {cprintf("弧度为"); scanf("%f",&h);
				   cprintf("度数为%f\n\r",h*180/PI);}
	else if(s=='3') {cprintf("时(0-12)"); scanf("%d",&d);
				   cprintf("分(0-59)"); scanf("%d",&f);
				   cprintf("秒(0-59)"); scanf("%d",&m);
				   cprintf("度数为%f\n\r",(d+f/60.0+m/3600.0)*15);}
	cprintf("继续吗(y/n)?\n\r");}
	while(getch()=='y');
}


void func1() /*已知三条边*/
{ float tt;
input:   cprintf("请输入三条边的值：\n\r");
   cprintf("a="); scanf("%f",&a);
   cprintf("b="); scanf("%f",&b);
   cprintf("c="); scanf("%f",&c);
   if(a>0&&b>0&&c>0&&a<180&&b<180&&c<180)
    {  
       a=trans(a); b=trans(b); c=trans(c);  /*把输入的度数转化为弧度*/
       tt=(cos(c)-cos(a)*cos(b))/(sin(a)*sin(b));
       if(tt>=-1&&tt<=1)  C=acos(tt);
       else return;
       getAB(); 
    }
    else 
    {
        cprintf("输入不正确\n\r");
     goto input;
	}
}

void func2() /*已知三个角*/
{ float tt;
input:	cprintf("请输入三个角：\n\r");
		cprintf("A="); scanf("%f",&A);
		cprintf("B="); scanf("%f",&B);
		cprintf("C="); scanf("%f",&C);
  if(A>0&&A<180&&B>0&&B<180&&C<180&&C>0)
	{
        A=trans(A); B=trans(B); C=trans(C);  /*把输入的度数转化为弧度*/
	tt=(cos(C)+cos(A)*cos(B))/(sin(A)*sin(B));
	if(tt>=-1&&tt<=1)  c=acos(tt);
	else return;
	getab();
	}
   else 
    {
        cprintf("输入不正确\n\r");
     goto input;
	}
}

void func3() /*已知两边及其夹角*/
{ float tt;
input:	cprintf("请输入两边:\n\r");
	cprintf("a="); scanf("%f",&a);
	cprintf("b="); scanf("%f",&b);
	cprintf("请输入夹角：C="); scanf("%f",&C);
	if(a>0&&a<180&&b>0&&b<180&&C>0&&C<180)
	{
	a=trans(a); b=trans(b); C=trans(C);
	tt=cos(a)*cos(b)+sin(a)*sin(b)*cos(C);
	if(tt>=-1&&tt<=1) c=acos(tt);
	else return;
	getAB();
	}
	else 
    {
        cprintf("输入不正确\n\r");
     goto input;
	}
}

void func4() /*已知两角及其夹边*/
{ float tt;
input:	cprintf("请输入两角:\n\r");
	cprintf("A="); scanf("%f",&A);
	cprintf("B="); scanf("%f",&B);
	cprintf("请输入夹边:c="); scanf("%f",&c);
    if(A>0&&A<180&&B>0&&B<180&&c>0&&c<180)
	{A=trans(A); B=trans(B); c=trans(c);
	tt=-cos(A)*cos(B)+sin(A)*sin(B)*cos(c);
	if(tt>=-1&&tt<=1)  C=acos(tt);
	else return;
	getab();
	}
	else 
    {
        cprintf("输入不正确\n\r");
     goto input;
	}
}

void func5() /*已知两边及一边的对角*/
	{ float tt;
input:	cprintf("输入一边及它的对角:边a="); scanf("%f",&a);
	cprintf("对角A="); scanf("%f",&A);
	cprintf("输入另一边:b="); scanf("%f",&b);
	if(a>0&&a<180&&A>0&&A<180&&b>0&&b<180)
	{ a=trans(a); A=trans(A); b=trans(b);
	tt=sin(A)*sin(c)/sin(a);
	if(tt>=-1&&tt<0)  B=asin(tt)+PI/2;
	else if(tt>=0&&tt<=1) B=asin(tt);
	else return;
	getCc();
	}
	else 
    {
        cprintf("输入不正确\n\r");
     goto input;
	}
}

void func6()  /*已知两角及一角的对边*/
{ float tt;
input:	cprintf("输入一角及它的对边:角A=");  scanf("%f",&A);
	cprintf("对边a="); scanf("%f",&a);
	cprintf("输入另一个角:B="); scanf("%f",&B);
	if(A>0&&A<180&&a>0&&a<180&&B>0&&B<180)
	{ A=trans(A); a=trans(a); B=trans(B);
	tt=sin(a)*sin(B)/sin(A);
	if(tt>=-1&&tt<0)  b=asin(tt)+PI/2;
	else if(tt>=0&&tt<=1) b=asin(tt);
	else return;
	getCc();
	}
	else 
    {
        cprintf("输入不正确\n\r");
     goto input;
	}
}


void main()
{  char select;
int aa[3],bb[3],cc[3],AA[3],BB[3],CC[3];
  int *pa,*pb,*pc;
  int *pA,*pB,*pC;
  pa=aa;  pb=bb;   pc=cc;
  pA=AA;  pB=BB;   pC=CC;
start: do{
	window(1,1,80,8);
	textbackground(BLUE);
	textcolor(YELLOW);
	clrscr();
  A=B=C=a=b=c=0;
cprintf("----------------------------欢迎使用球面三角计算小程序--------------------------\n\r");
gotoxy(1,2);    cprintf("请选择已知条件的类型\n\r");
				cprintf("1.已知三条边");
gotoxy(40,3);   cprintf("2.已知三个角\n\r");
				cprintf("3.已知两边及其夹角");
gotoxy(40,4);	cprintf("4.已知两角及其夹角\n\r");
				cprintf("5.已知两边及一边的对角");
gotoxy(40,5);	cprintf("6.已知两角及一角的对边\n\r");
				cprintf("0.退出\n\r");
				cprintf("输入数据请用角度(0--180)表示，按9使用角度转化工具");
 select=getch();
 cprintf("\n\r");
	window(1,9,80,25);
	textbackground(3);
	textcolor(5);
	clrscr();

if(select=='1')    /*已知三条边*/
    func1();
else if(select=='2')    /*已知三个角*/
	func2();
else if(select=='3')    /*已知两边及其夹角*/
	func3();
else if(select=='4')    /*已知两角及其夹边*/
	func4();
else if(select=='5')    /*已知两边及一边的对角*/
	func5();
else if(select=='6')    /*已知两角及一角的对边*/
	func6();
else if(select=='9') {deg(); goto start ;}
else goto loop;
if(a>0&&b>0&&c>0&&A>0&&B>0&&c>0)
{
	out(a,pa);   out(b,pb);   out(c,pc);
    out(A,pA);   out(B,pB);   out(C,pC);
	cprintf("球面三角的边角分别为：\n\r");
	cprintf("a=%d度%d分%d秒  b=%d度%d分%d秒  c=%d度%d分%d秒\n\r",aa[0],aa[1],aa[2],bb[0],bb[1],bb[2],cc[0],cc[1],cc[2]);
	cprintf("A=%d度%d分%d秒  B=%d度%d分%d秒  C=%d度%d分%d秒\n\r",AA[0],AA[1],AA[2],BB[0],BB[1],BB[2],CC[0],CC[1],CC[2]);
}
else cprintf("您给的条件不能组成一个球面三角形!!!!!\n\r");
cprintf("要继续吗(y/n)?  \n\r");
}

while(getch()=='y');

loop: cprintf("\n\r\n\r      制作者：韩冬   于2000年6月\n\r");
       getch();
}
