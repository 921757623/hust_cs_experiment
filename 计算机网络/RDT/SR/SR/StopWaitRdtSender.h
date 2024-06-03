#define _CRT_SECURE_NO_WARNINGS
#ifndef STOP_WAIT_RDT_SENDER_H
#define STOP_WAIT_RDT_SENDER_H
#include "RdtSender.h"
#include <fstream>
class StopWaitRdtSender :public RdtSender
{
private:
	int base;
	int expectSequenceNumberSend;	// ��һ��������� 
	bool waitingState;				// �Ƿ��ڵȴ�Ack��״̬
	Packet packetWaitingAck[2 * N];		//�ѷ��Ͳ��ȴ�Ack�����ݰ�
	bool isAck[2 * N];
	string last_window;
	fstream file;
	void printWindow();
public:

	bool getWaitingState();
	bool send(const Message &message);						//����Ӧ�ò�������Message����NetworkServiceSimulator����,������ͷ��ɹ��ؽ�Message���͵�����㣬����true;�����Ϊ���ͷ����ڵȴ���ȷȷ��״̬���ܾ�����Message���򷵻�false
	void receive(const Packet &ackPkt);						//����ȷ��Ack������NetworkServiceSimulator����	
	void timeoutHandler(int seqNum);					//Timeout handler������NetworkServiceSimulator����

public:
	StopWaitRdtSender();
	virtual ~StopWaitRdtSender();
};

#endif

