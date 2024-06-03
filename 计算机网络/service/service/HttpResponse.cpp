#include "HttpResponse.h"

// ������������
const char HttpResponse::content_type[5][20] = {"text/html", "image/jpeg", "application/pdf", "video/mp4"};
const int HttpResponse::resp_code[5] = {404, 200};

string HttpResponse::resp_head = "HTTP/1.1 %d\r\nDate:%s\r\nContent-type:%s\r\nContent-Length:%d\r\n\r\n";

const char* HttpResponse::getFileType(string url)
{
	int pos = url.find_last_of(".");
	if (pos != url.npos)
	{
		// ��ȡ�ļ���׺��
		string tmp = url.substr(pos);
		if (tmp == ".html")
		{
			return content_type[0];
		}
		else if (tmp == ".jpg" || tmp == ".jpeg")
		{
			return content_type[1];
		}
		else if (tmp == ".pdf")
		{
			return content_type[2];
		}
		else if (tmp == ".mp4")
		{
			return content_type[3];
		}
		else
		{
			return "";
		}
	}
}

int HttpResponse::getFileSize(FILE* fp)
{
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	return size;
}

const char* HttpResponse::getCurrentTime()
{
	char buf[128] = { 0 };
	time_t nowtime;
	time(&nowtime); //��ȡ1970��1��1��0��0��0�뵽���ھ���������
	tm p;
	localtime_s(&p, &nowtime); //������ת��Ϊ����ʱ��,���1900����,��Ҫ+1900,��Ϊ0-11,����Ҫ+1
	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d\0", p.tm_year + 1900, p.tm_mon + 1, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
	return buf;
}

// ������Ӧ����
int HttpResponse::sendResponse(SOCKET fd, FILE *fp, string url)
{
	struct _stat32 stat;
	char buf[256] = { 0 };
	int type = 1;
	if (url.find("404.html") != url.npos)
	{
		type = 0;
	}
	sprintf(buf, resp_head.c_str(), resp_code[type], getCurrentTime(), getFileType(url), getFileSize(fp));
	send(fd, buf, strlen(buf), 0);
	return 0;
}