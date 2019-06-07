#pragma once

#include <windows.h>
#include <tchar.h>
#include <iostream>

/**
\example:
\code
void main()
{
	// make a ConsoleUtility, set title
	ConsoleUtility console(_T("console window title"));
	
	for(int i=1;i<1000;i++)
	{
		// do some job
		
		// before print, roll the cursor to top-left corner
		console.RolltoTop();
		
		// printf something, allways begin at top-left of the console screen
	}
}
\endcode
*/

class ConsoleUtility{
public:
	ConsoleUtility(){
		Set();
	}
	ConsoleUtility(TCHAR* title){
		Set();
		SetConsoleTitle(title);
	}
	~ConsoleUtility(){}
	// 滚动到开头
	void RolltoTop(){
		cursorPos.X = 0;
		cursorPos.Y = 0;
		SetConsoleCursorPosition(hStdOut, cursorPos);
	}
	// 滚动到某一行
	void Rollto(SHORT row){
		cursorPos.X = 0;
		cursorPos.Y = row;
		SetConsoleCursorPosition(hStdOut, cursorPos);
	}
	// 跳到某个特定光标位置
	void JumpTo(SHORT x,SHORT y){
		cursorPos.X = x;
		cursorPos.Y = y;
		SetConsoleCursorPosition(hStdOut, cursorPos);
	}
	// 初始设置
	void Set(){
#ifdef _MSC_VER
		//////////////////////////////////////////////////////////////////////////
		//  DOS 窗口设置
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);	// 获取标准输出设备句柄
		cursorPos.X = 0;
		cursorPos.Y = 0;
		COORD a;
		a.X = 120; a.Y = 300;
		//if(!SetConsoleScreenBufferSize(hStdOut,a))
		//	std::cout << "failed to Set Console Screen Buffer Size\n";
		SMALL_RECT rect;
		rect.Top = 0; rect.Left = 10;
		rect.Right = a.X-1;   rect.Bottom = a.Y-190;
		if(!SetConsoleWindowInfo(hStdOut,TRUE,&rect))
			std::cout << "failed to set console window info\n";
		//////////////////////////////////////////////////////////////////////////
#endif // _MSC_VER;
	}
	
	// 设置标题
	void SetTitle(TCHAR* title){
		SetConsoleTitle(title);
	}
	// 在原有标题的后面增加字符
	void SetTitle2(TCHAR* title){
		//TCHAR oldtile[128];
		//int len = GetConsoleTitle(oldtile,128);
		//wcscpy(oldtile+len,title);
		//SetConsoleTitle(oldtile);
	}
private:
#ifdef _MSC_VER
	HANDLE hStdOut;
	COORD cursorPos;
#endif // _MSC_VER
};