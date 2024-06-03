#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"
#include <fstream>
class StopWaitRdtSender :public RdtSender
{
private:
	int base;
	int repeat_count = 0;
	int lastack = -1;
	int expectSequenceNumberSend;	// 下一个发送序号 
	bool waitingState;				// 是否处于等待Ack的状态
	string last_window;
	Packet packetWaitingAck[2 * N];		//已发送并等待Ack的数据包
	fstream file;
	void printWindow();
public:

	bool getWaitingState();
	bool send(const Message &message);						//发送应用层下来的Message，由NetworkServiceSimulator调用,如果发送方成功地将Message发送到网络层，返回true;如果因为发送方处于等待正确确认状态而拒绝发送Message，则返回false
	void receive(const Packet &ackPkt);						//接受确认Ack，将被NetworkServiceSimulator调用	
	void timeoutHandler(int seqNum);					//Timeout handler，将被NetworkServiceSimulator调用

public:
	StopWaitRdtSender();
	virtual ~StopWaitRdtSender();
};

#endif

