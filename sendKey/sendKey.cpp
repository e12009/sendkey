// sendKey.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <windows.h>
#include <map>

#include "winio.h"               //winio头文件
#pragma comment(lib,"winio32.lib")  //包含winio库


//
//95 visible characters can be grouped into 2, one with shift, and one without shift.
// The first column in the following table are char -> scan code , without shift,
// the second column are char => scan code,  with sift needed, as indicated by 0x80
// scan code can be found at: 
// https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa299374(v%3dvs.60)


std::map<char, char> char2ScanMap = {

    {'1',  0x02}, {'!', 0x02 | 0x80},
    {'2',  0x03}, {'@', 0x03 | 0x80},
    {'3',  0x04}, {'#', 0x04 | 0x80},
    {'4',  0x05}, {'$', 0x05 | 0x80},
    {'5',  0x06}, {'%', 0x06 | 0x80},
    {'6',  0x07}, {'^', 0x07 | 0x80},
    {'7',  0x08}, {'&', 0x08 | 0x80},
    {'8',  0x09}, {'*', 0x09 | 0x80},
    {'9',  0x0A}, {'(', 0x0A | 0x80},
    {'0',  0x0B}, {')', 0x0B | 0x80},
    {'-',  0x0C}, {'_', 0x0C | 0x80},
    {'=',  0x0D}, {'+', 0x0D | 0x80},
    {'q',  0x10}, {'Q', 0x10 | 0x80},
    {'w',  0x11}, {'W', 0x11 | 0x80},
    {'e',  0x12}, {'E', 0x12 | 0x80},
    {'r',  0x13}, {'R', 0x13 | 0x80},
    {'t',  0x14}, {'T', 0x14 | 0x80},
    {'y',  0x15}, {'Y', 0x15 | 0x80},
    {'u',  0x16}, {'U', 0x16 | 0x80},
    {'i',  0x17}, {'I', 0x17 | 0x80},
    {'o',  0x18}, {'O', 0x18 | 0x80},
    {'p',  0x19}, {'P', 0x19 | 0x80},
    {'[',  0x1A}, {'{', 0x1A | 0x80},
    {']',  0x1B}, {'}', 0x1B | 0x80},
    {'a',  0x1E}, {'A', 0x1E | 0x80},
    {'s',  0x1F}, {'S', 0x1F | 0x80},
    {'d',  0x20}, {'D', 0x20 | 0x80},
    {'f',  0x21}, {'F', 0x21 | 0x80},
    {'g',  0x22}, {'G', 0x22 | 0x80},
    {'h',  0x23}, {'H', 0x23 | 0x80},
    {'j',  0x24}, {'J', 0x24 | 0x80},
    {'k',  0x25}, {'K', 0x25 | 0x80},
    {'l',  0x26}, {'L', 0x26 | 0x80},
    {';',  0x27}, {':', 0x27 | 0x80},
    {'\'', 0x28}, {'"', 0x28 | 0x80},
    {'`',  0x29}, {'~', 0x29 | 0x80},
    {'\\', 0x2B}, {'|', 0x2B | 0x80},
    {'z',  0x2C}, {'Z', 0x2C | 0x80},
    {'x',  0x2D}, {'X', 0x2D | 0x80},
    {'c',  0x2E}, {'C', 0x2E | 0x80},
    {'v',  0x2F}, {'V', 0x2F | 0x80},
    {'b',  0x30}, {'B', 0x30 | 0x80},
    {'n',  0x31}, {'N', 0x31 | 0x80},
    {'m',  0x32}, {'M', 0x32 | 0x80},
    {',',  0x33}, {'<', 0x33 | 0x80},
    {'.',  0x34}, {'>', 0x34 | 0x80},
    {'/',  0x35}, {'?', 0x35 | 0x80},
    {' ',  0x39},
};

void shiftDown() {
	Sleep(10);
	SetPortVal(0x64, 0xD2, 1);
	Sleep(10);
	SetPortVal(0x60, 0x2A, 1);
	Sleep(10);
}

void shiftUp() {
	Sleep(10);
	SetPortVal(0x64, 0xD2, 1);
	Sleep(10);
	SetPortVal(0x60, 0x2A | 0x80, 1);
	Sleep(10);
}

 void typeChar(char a) 
{
    UINT8 code;
    try {
        code = char2ScanMap.at(a);
    } catch ( ... ) {
        printf("char %a not supported\n", &a);
        return;
    }

    bool shift = code & 0x80;
    UINT8 scanCode = code & 0x7F;

    if(shift) {
        shiftDown();
    }

    SetPortVal(0x64, 0xD2, 1);  // 键盘写入命令
    Sleep(10); // 等待
    SetPortVal(0x60, scanCode, 1);  // Key press
    Sleep(10);
    SetPortVal(0x64, 0xD2, 1);   // Keyboard write command
    Sleep(10);
    SetPortVal(0x60, scanCode | 0x80, 1); //Key release

    if (shift) {
        shiftUp();
    }
}

void main(int argc, char** argv)
{
	if (argc < 2) {
		printf("Usage: %s <chars>", argv[0]);
		return;
	}

    // 初始化WinIo  
    if (!InitializeWinIo())
    {
        printf("Error In InitializeWinIo!\n");
        exit(1);
    }

	for (auto i = 0; i < strlen(argv[1]); i++) {
		typeChar(argv[1][i]);
		//Sleep(1000);
	 }

    //关闭WinIo  
    ShutdownWinIo();
}
