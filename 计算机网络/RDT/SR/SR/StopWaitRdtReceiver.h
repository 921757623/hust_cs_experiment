#ifndef STOP_WAIT_RDT_RECEIVER_H
#define STOP_WAIT_RDT_RECEIVER_H
#include "RdtReceiver.h"
#include <fstream>
class StopWaitRdtReceiver :public RdtReceiver
{
private:
	int base;
	int expectSequenceNumberRcvd;	// �ڴ��յ�����һ���������
	Packet lastAckPkt;				//�ϴη��͵�ȷ�ϱ���
	Packet ReceivedPacket[2 * N];		//�ѷ��Ͳ��ȴ�Ack�����ݰ�
	bool isWait[2 * N];
	string last_window;
	fstream file;
	void printWindow();
public:
	StopWaitRdtReceiver();
	virtual ~StopWaitRdtReceiver();

public:
	
	void receive(const Packet &packet);	//���ձ��ģ�����NetworkService����
};

#endif

