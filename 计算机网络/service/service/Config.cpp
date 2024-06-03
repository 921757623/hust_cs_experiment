#include "Config.h"

string Config::SERVER_ADDRESS = "0.0.0.0";	//服务器IP地址
string  Config::PATH = "./Res"; // 资源文件名
string Config::CONFIG_FILENAME = "config.ini"; // 配置文件名

const int Config::BUFFER_LENGTH = 4096; // 缓冲区长度
const u_long Config::BLOCK_MODE = 0; // 阻塞模式，0为阻塞，1为非阻塞

int Config::PORT = 80; // 通信端口

bool Config::isFile_exist()  
{
	struct stat buffer;
	return (stat(CONFIG_FILENAME.c_str(), &buffer) == 0);
}

int Config::createDefaultConfig()
{
	//创建流对象
	ofstream fout;

	//打开文件
	fout.open(CONFIG_FILENAME, ios::out);
	if (!fout.is_open())
	{
		cout << "Initial Configuration File Generated Failed!" << endl;
		perror("Casued By:");
		return 1;
	}

	//写数据 默认资源文件为Res，默认连接端口为80,
	fout << "PATH=" << PATH <<endl;
	fout << "PORT=" << PORT <<endl;
	fout << "IP=" << SERVER_ADDRESS <<endl;

	//关闭文件
	fout.close();
	return 0;
}
		
int Config::setConfig()
{
	ifstream fin;
	// 配置文件如果不存在，则创建默认的配置文件
	if (!isFile_exist()) 
	{
		if (createDefaultConfig())
			return 1;
	}
	// 读入配置文件
	fin.open(CONFIG_FILENAME, ios::in);
	if (!fin.is_open()) 
	{
		cout << "Config file Open Failed!" << endl;
		perror("Casued By:");
		return 1;
	}

	string buf;
	//利用getline函数，将一行内容放入字符串中
	while (getline(fin, buf))	
	{
		if (buf.find("PATH=") != buf.npos)
		{
			PATH = buf.substr(5);
		}
		else if (buf.find("PORT=") != buf.npos)
		{
			PORT = stoi(buf.substr(5));
		}
		else if (buf.find("IP=") != buf.npos)
		{
			SERVER_ADDRESS = buf.substr(3);
		}
		else
		{
			cout << "Config File format error!" << endl;
			fin.close();
			return 1;
		}
	}
	// 如果对应的资源文件夹不存在，则创建该文件夹
	if (_access(PATH.c_str(), 0) == -1)
	{
		_mkdir(PATH.c_str());
	}
	fin.close();
	return 0;
}

void Config::showConfigInfo()
{
	cout << "Current Configuration:" << endl;
	cout << "Server Listening IP: " << Config::SERVER_ADDRESS << endl;
	cout << "Server Listening Port: " << Config::PORT << endl;
	cout << "Resource File Path: " << Config::PATH << endl << endl;
}