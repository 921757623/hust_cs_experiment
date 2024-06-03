#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include <string>
#include <ctime>
#include "winsock2.h"

using namespace std;

class HttpResponse
{
public:
	static const char content_type[][20];
	static const int resp_code[5];
	static string resp_head;

	static const int OK = 0;
	static const int NOTFOUND = 1;

	static int sendResponse(SOCKET, FILE *, string);
private:
	HttpResponse();
	static const char* getFileType(string);
	static int getFileSize(FILE *);
	static const char* getCurrentTime();
};

