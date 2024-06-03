﻿/*
 * @Description:
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2023-09-26 19:15:35
 * @LastEditTime: 2023-09-26 22:04:47
 */
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "winsock2.h"
#include "Config.h"
#include "HttpResponse.h"
#include <stdio.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

// 根据错误码，返回报错信息
char* getErrorMessage(DWORD errorCode)
{
	LPSTR errorMessage = nullptr;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&errorMessage, 0, NULL);
	return errorMessage;
}

// 解析浏览器请求，获取请求方法和请求连接
int requestHandle(char* buf, char* method, char* url)
{
	if (buf == nullptr || method == nullptr || url == nullptr)
	{
		return -1;
	}
	int i = 0;
	char* ptr = nullptr;
	for (ptr = buf; *ptr != ' '; ptr++)
	{
		method[i++] = *ptr;
	}
	method[i] = '\0';
	// 跳过空格
	ptr++;
	i = 0;
	for (; *ptr != ' '; ptr++)
	{
		url[i++] = *ptr;
	}
	url[i] = '\0';
	return 0;
}

// 只负责发送响应报文和文件
int sendFile(SOCKET fd, string filename)
{
	FILE* fp = nullptr;
	if ((fp = fopen(filename.c_str(), "rb")) == nullptr)
	{
		perror("Casued By:");
		return 1;
	}
	// 首先发送响应报文
	HttpResponse::sendResponse(fd, fp, filename);
	// 再读取并发送文件
	char* buf = new char[Config::BUFFER_LENGTH];
	memset(buf, '\0', Config::BUFFER_LENGTH);

	while (!feof(fp))
	{
		int len = fread(buf, sizeof(char), Config::BUFFER_LENGTH, fp);
		send(fd, buf, len, 0);
	}

	delete[] buf;
	fclose(fp);
	return 0;
}

// 判断发送何种资源文件
int setResFile(SOCKET fd, string url)
{
	if (url.length() == 1)
	{
		string home = Config::PATH + "/homepage.html";
		return sendFile(fd, home);
	}
	string tmp = Config::PATH + url;
	struct _stat32 stat;
	if (_stat32(tmp.c_str(), &stat) == -1)
	{
		// 不存在此文件，返回404页面
		string notfound = Config::PATH + "/404.html";
		cout << "page is not found!" << endl;
		return sendFile(fd, notfound);
	}
	else 
	{
		// 存在则返回相应文件
		return sendFile(fd, tmp);
	}
}


int main() {
	// 依据配置文件配置初始信息
	if (Config::setConfig())
	{
		cout << "Configuration File Init Failed!" << endl;
		return 1;
	}
	// 打印配置信息
	Config::showConfigInfo();
	

	WSADATA wsaData;
	/*
		select()机制中提供的fd_set的数据结构，实际上是long类型的数组，
		每一个数组元素都能与一打开的文件句柄（不管是socket句柄，还是其他文件或命名管道或设备句柄）建立联系，建立联系的工作由程序员完成.
		当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执行了select()的进程哪个socket或文件句柄发生了可读或可写事件。
	*/
	fd_set rfds;				//用于检查socket是否有数据到来的的文件描述符，用于socket非阻塞模式下等待网络事件通知（有数据到来）
	fd_set wfds;				//用于检查socket是否可以发送的文件描述符，用于socket非阻塞模式下等待网络事件通知（可以发送数据）
	bool first_connetion = true;

	int nRc = WSAStartup(0x0202, &wsaData);

	if (nRc) {
		printf("Winsock  startup failed with error!\n");
	}

	if (wsaData.wVersion != 0x0202) {
		printf("Winsock version is not correct!\n");
	}

	printf("Winsock  startup Ok!\n");


	//监听socket
	SOCKET srvSocket;

	//服务器地址和客户端地址
	sockaddr_in addr, clientAddr;

	//会话socket，负责和client进程通信
	SOCKET sessionSocket;

	//ip地址长度
	int addrLen;

	//创建监听socket
	srvSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (srvSocket != INVALID_SOCKET)
		printf("Socket create Ok!\n");


	//设置服务器的端口和地址
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Config::PORT);
	// addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); 
	//主机上任意一块网卡的IP地址
	addr.sin_addr.S_un.S_addr = inet_addr(Config::SERVER_ADDRESS.c_str());


	//binding
	int rtn = bind(srvSocket, (LPSOCKADDR)&addr, sizeof(addr));
	if (rtn != SOCKET_ERROR)
	{
		printf("Socket bind Ok!\n");
	}
	else
	{
		cout << "Socket bind failed!" << endl;
		cout << getErrorMessage(WSAGetLastError()) << endl;
		closesocket(srvSocket);
		return 1;
	}
		

	//监听
	rtn = listen(srvSocket, 5);
	if (rtn != SOCKET_ERROR)
		printf("Socket listen Ok!\n");
	else {
		cout << "Socket listen Failed!" << endl;
		cout << getErrorMessage(WSAGetLastError()) << endl;
		return 1;
	}

	clientAddr.sin_family = AF_INET;
	addrLen = sizeof(clientAddr);

	//设置接收缓冲区
	char *recvBuf = new char[Config::BUFFER_LENGTH];

	u_long blockMode = Config::BLOCK_MODE;//将srvSock设为非阻塞模式以监听客户连接请求

	//调用ioctlsocket，将srvSocket改为非阻塞模式，改成反复检查fd_set元素的状态，看每个元素对应的句柄是否可读或可写
	if ((rtn = ioctlsocket(srvSocket, FIONBIO, &blockMode) == SOCKET_ERROR)) { //FIONBIO：允许或禁止套接口s的非阻塞模式。
		cout << "ioctlsocket() failed with error!\n";
		return 1;
	}
	cout << "ioctlsocket() for server socket ok!	Waiting for client connection and data\n\n";

	while (true) {
		//清空rfds和wfds数组
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);

		//将srvSocket加入rfds数组
		//即：当客户端连接请求到来时，rfds数组里srvSocket对应的的状态为可读
		//因此这条语句的作用就是：设置等待客户连接请求
		FD_SET(srvSocket, &rfds);

		//如果first_connetion为true，sessionSocket还没有产生
		if (!first_connetion) {
			//将sessionSocket加入rfds数组和wfds数组
			//即：当客户端发送数据过来时，rfds数组里sessionSocket的对应的状态为可读；当可以发送数据到客户端时，wfds数组里sessionSocket的对应的状态为可写
			//因此下面二条语句的作用就是：
			//设置等待会话SOKCET可接受数据或可发送数据
			if (sessionSocket != INVALID_SOCKET) { //如果sessionSocket是有效的
				FD_SET(sessionSocket, &rfds);
				FD_SET(sessionSocket, &wfds);
			}

		}

		/*
			select工作原理：传入要监听的文件描述符集合（可读、可写，有异常）开始监听，select处于阻塞状态。
			当有可读写事件发生或设置的等待时间timeout到了就会返回，返回之前自动去除集合中无事件发生的文件描述符，返回时传出有事件发生的文件描述符集合。
			但select传出的集合并没有告诉用户集合中包括哪几个就绪的文件描述符，需要用户后续进行遍历操作(通过FD_ISSET检查每个句柄的状态)。
		*/
		//开始等待，等待rfds里是否有输入事件，wfds里是否有可写事件
		//The select function returns the total number of socket handles that are ready and contained in the fd_set structure
		//返回总共可以读或写的句柄个数
		int nTotal = select(0, &rfds, &wfds, NULL, NULL);

		//如果srvSock收到连接请求，接受客户连接请求
		if (FD_ISSET(srvSocket, &rfds)) {
			nTotal--;   //因为客户端请求到来也算可读事件，因此-1，剩下的就是真正有可读事件的句柄个数（即有多少个socket收到了数据）

			//产生会话SOCKET
			sessionSocket = accept(srvSocket, (LPSOCKADDR)&clientAddr, &addrLen);
			if (sessionSocket != INVALID_SOCKET) {
				printf("Socket listen one client request!\n");
				cout << "Client's IP: " << inet_ntoa(((sockaddr_in*)&clientAddr)->sin_addr) << endl;
				cout << "Client's Port: " << clientAddr.sin_port << endl;
			}

			//把会话SOCKET设为非阻塞模式
			if ((rtn = ioctlsocket(sessionSocket, FIONBIO, &blockMode) == SOCKET_ERROR)) { //FIONBIO：允许或禁止套接口s的非阻塞模式。
				cout << "ioctlsocket() failed with error!\n";
				return 1;
			}

			//设置等待会话SOKCET可接受数据或可发送数据
			FD_SET(sessionSocket, &rfds);
			FD_SET(sessionSocket, &wfds);

			first_connetion = false;

		}

		//检查会话SOCKET是否有数据到来或者是否可以发送数据
		if (nTotal > 0) {
			//如果还有有可读事件，说明是会话socket有数据到来，则接受客户的数据
			if (FD_ISSET(sessionSocket, &rfds)) {
				//receiving data from client
				memset(recvBuf, '\0', Config::BUFFER_LENGTH);
				rtn = recv(sessionSocket, recvBuf, Config::BUFFER_LENGTH, 0);
				if (rtn > 0) {
					printf("Received %d bytes from client: %s\n", rtn, recvBuf);
					char method[10] = { 0 };
					char url[20] = { 0 };
					if (requestHandle(recvBuf, method, url) >= 0)
					{
						cout << "Request URL: " << inet_ntoa(((sockaddr_in*)&clientAddr)->sin_addr) << url << endl;
						if (strcmp(method, "GET") == 0)
						{
							if (setResFile(sessionSocket, url) == SOCKET_ERROR)
							{
								cout << "Send Failed!!" << endl;
								cout << getErrorMessage(WSAGetLastError()) << endl << endl;
							}
							else
							{
								cout << "Send Success!!\n" << endl;
							}
						}
					}
				}
				else if (rtn == SOCKET_ERROR) {
					cout << "Occur Error When Receive Data From Client: " << getErrorMessage(WSAGetLastError()) << endl;
				}else { //否则是收到了客户端断开连接的请求，也算可读事件。但rtn = 0
					printf("Client leaving ...\n");
					closesocket(sessionSocket);  //既然client离开了，就关闭sessionSocket
					nTotal--;	//因为客户端离开也属于可读事件，所以需要-1
					sessionSocket = INVALID_SOCKET; //把sessionSocket设为INVALID_SOCKET
				}
			}
		}
	}
	return 0;
}