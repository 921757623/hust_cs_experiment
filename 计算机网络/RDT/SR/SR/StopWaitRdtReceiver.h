#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H
#include "RdtReceiver.h"
#include <fstream>
class StopWaitRdtReceiver :public RdtReceiver
{
private:
	int base;
	int expectSequenceNumberRcvd;	// 期待收到的下一个报文序号
	Packet lastAckPkt;				//上次发送的确认报文
	Packet ReceivedPacket[2 * N];		//已发送并等待Ack的数据包
	bool isWait[2 * N];
	string last_window;
	fstream file;
	void printWindow();
public:
	StopWaitRdtReceiver();
	virtual ~StopWaitRdtReceiver();

public:
	
	void receive(const Packet &packet);	//接收报文，将被NetworkService调用
};

#endif

