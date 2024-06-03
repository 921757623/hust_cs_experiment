#pragma once
#include "winsock2.h"
#include <iostream>
#include <string>
// �ļ���ȡ
#include <fstream>
#include <io.h>
#include <direct.h>

using namespace std;
/**
* �����ļ���
**/
class Config
{
public:
	static const int BUFFER_LENGTH;		//��������С
	static const u_long BLOCK_MODE;	//SOCKET����ģʽ

	static int PORT;				//�������˿�

	static string SERVER_ADDRESS;  //��������ַ
	static string PATH;
	
	static int setConfig();
	static void showConfigInfo();
private:
	static string CONFIG_FILENAME;
	static bool isFile_exist();
	static int createDefaultConfig();
	
    // ˽�л����캯��
    Config();          
};

