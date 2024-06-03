#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"




StopWaitRdtSender::StopWaitRdtSender():base(0), expectSequenceNumberSend(0), waitingState(false)
{
	last_window = " -1 [  0   1   2   3]   4";
	file.open("window.txt", ios::out);
}


StopWaitRdtSender::~StopWaitRdtSender()
{
	file.close();
}



bool StopWaitRdtSender::getWaitingState() {
	return waitingState;
}




bool StopWaitRdtSender::send(const Message &message) {
	if (this->waitingState) { //���ͷ����ڵȴ�ȷ��״̬
		return false;
	}

	if (expectSequenceNumberSend < base + N) {
		int current_pos = expectSequenceNumberSend % (2 * N);
		this->packetWaitingAck[current_pos].acknum = -1; //���Ը��ֶ�
		this->packetWaitingAck[current_pos].seqnum = this->expectSequenceNumberSend;
		this->packetWaitingAck[current_pos].checksum = 0;
		memcpy(this->packetWaitingAck[current_pos].payload, message.data, sizeof(message.data));
		this->packetWaitingAck[current_pos].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[current_pos]);

		pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck[current_pos]);

		if (base == expectSequenceNumberSend) {
			cout << "������������ʼ���г�ʱ��ʱ" << endl;
			//�������ͷ���ʱ��
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);
		}
		
		//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[current_pos]);
		expectSequenceNumberSend++;
	}

	
	//����ȴ�״̬
	if (expectSequenceNumberSend == base + N) {
		this->waitingState = true;
	}
																						
	return true;
}

void StopWaitRdtSender::printWindow() {
	string tmp = "���ͷ��������ڣ�" + last_window + " ==> ";
	file.write(tmp.c_str(), tmp.size()); 
	cout << tmp;
	char buf[64] = { 0 };
	sprintf(buf, "%3d [%3d %3d %3d %3d] %3d", base - 1, base + 0, base + 1, base + 2, base + 3, base + 4);
	file.write(buf, strlen(buf));
	file.write("\n", 1);
	last_window = string(buf);
	cout << last_window << endl;
}

void StopWaitRdtSender::receive(const Packet &ackPkt) {
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	if (checkSum == ackPkt.checksum && ackPkt.acknum >= base) {
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		int last_base = base;
		// ������ǰ����
		base = ackPkt.acknum + 1;
		printWindow();
		if (base == expectSequenceNumberSend)
		{
			cout << "�ѷ��ͷ�����ȫ�����ͣ��رռ�ʱ��" << endl;
			this->waitingState = false;
			pns->stopTimer(SENDER, last_base);	//�رն�ʱ��
		}
		else {
			pns->stopTimer(SENDER, last_base);//��û�����꣬�����ȴ�
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);
			this->waitingState = false;
		}
	}else {
		if (checkSum != ackPkt.checksum) {
			cout << "���ͷ��յ���ACK��" << endl;
		}
		else {
			cout << "���ͷ�û���յ���ȷ����ţ������ȴ�" << endl;
		}
	}
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	//Ψһһ����ʱ��,���迼��seqNum
	//pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط��ϴη��͵ı���", this->packetWaitingAck);
	//pns->stopTimer(SENDER,seqNum);										//���ȹرն�ʱ��
	//pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);			//�����������ͷ���ʱ��
	//pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck);			//���·������ݰ�

	//Ψһһ����ʱ��,���迼��seqNum
	cout << "���ͳ�ʱ������N��" << endl;
	pns->stopTimer(SENDER, seqNum);//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);//�����������ͷ���ʱ��
	for (int i = base; i != expectSequenceNumberSend; i++) {
		cout << "�ط�" << i << "�ű���" << endl;
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[i % (2 * N)]);			//���·������ݰ�
	}
}
