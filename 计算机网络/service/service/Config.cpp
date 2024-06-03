#include "Config.h"

string Config::SERVER_ADDRESS = "0.0.0.0";	//������IP��ַ
string  Config::PATH = "./Res"; // ��Դ�ļ���
string Config::CONFIG_FILENAME = "config.ini"; // �����ļ���

const int Config::BUFFER_LENGTH = 4096; // ����������
const u_long Config::BLOCK_MODE = 0; // ����ģʽ��0Ϊ������1Ϊ������

int Config::PORT = 80; // ͨ�Ŷ˿�

bool Config::isFile_exist()  
{
	struct stat buffer;
	return (stat(CONFIG_FILENAME.c_str(), &buffer) == 0);
}

int Config::createDefaultConfig()
{
	//����������
	ofstream fout;

	//���ļ�
	fout.open(CONFIG_FILENAME, ios::out);
	if (!fout.is_open())
	{
		cout << "Initial Configuration File Generated Failed!" << endl;
		perror("Casued By:");
		return 1;
	}

	//д���� Ĭ����Դ�ļ�ΪRes��Ĭ�����Ӷ˿�Ϊ80,
	fout << "PATH=" << PATH <<endl;
	fout << "PORT=" << PORT <<endl;
	fout << "IP=" << SERVER_ADDRESS <<endl;

	//�ر��ļ�
	fout.close();
	return 0;
}
		
int Config::setConfig()
{
	ifstream fin;
	// �����ļ���������ڣ��򴴽�Ĭ�ϵ������ļ�
	if (!isFile_exist()) 
	{
		if (createDefaultConfig())
			return 1;
	}
	// ���������ļ�
	fin.open(CONFIG_FILENAME, ios::in);
	if (!fin.is_open()) 
	{
		cout << "Config file Open Failed!" << endl;
		perror("Casued By:");
		return 1;
	}

	string buf;
	//����getline��������һ�����ݷ����ַ�����
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
	// �����Ӧ����Դ�ļ��в����ڣ��򴴽����ļ���
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