#pragma once
#include "winsock2.h"
#include <iostream>
#include <string>
// 文件读取
#include <fstream>
#include <io.h>
#include <direct.h>

using namespace std;
/**
* 配置文件类
**/
class Config
{
public:
	static const int BUFFER_LENGTH;		//缓冲区大小
	static const u_long BLOCK_MODE;	//SOCKET阻塞模式

	static int PORT;				//服务器端口

	static string SERVER_ADDRESS;  //服务器地址
	static string PATH;
	
	static int setConfig();
	static void showConfigInfo();
private:
	static string CONFIG_FILENAME;
	static bool isFile_exist();
	static int createDefaultConfig();
	
    // 私有化构造函数
    Config();          
};

