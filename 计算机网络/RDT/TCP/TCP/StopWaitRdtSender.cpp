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
	}																			//����ȴ�״̬
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

	//���У�����ȷ������ȷ�����=���ͷ��ѷ��Ͳ��ȴ�ȷ�ϵ����ݰ����
	if (checkSum == ackPkt.checksum && ackPkt.acknum >= base) {
		
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		int last_base = base;
		// ������ǰ����
		base = ackPkt.acknum + 1;
		if (base == expectSequenceNumberSend)
		{
			cout << "�ѷ��ͷ�����ȫ�����ͣ��رռ�ʱ��" << endl;
			this->waitingState = false;
			pns->stopTimer(SENDER, last_base);	//�رն�ʱ��
			printWindow();
		}
		else {
			pns->stopTimer(SENDER, last_base);//��û�����꣬�����ȴ�
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);
			this->waitingState = false;
		}
	}
	else {
		if (ackPkt.acknum == lastack) {
			repeat_count++;
			if (repeat_count == 4) {
				cout << "�յ������������ACK�������ش����" << ackPkt.acknum + 1 << endl;
				char buf[64] = { 0 };
				sprintf(buf, "�����ش����: %d\n", ackPkt.acknum + 1);
				string tmp = buf;
				file.write(tmp.c_str(), tmp.size());
				pns->stopTimer(SENDER, ackPkt.acknum + 1);
				pns->startTimer(SENDER, Configuration::TIME_OUT, ackPkt.acknum + 1);
				pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[base % (2 * N)]);
			}
		}
		else {
			lastack = ackPkt.acknum;
			repeat_count = 1;
		}
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
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط��ϴη��͵ı���", this->packetWaitingAck[seqNum % (2 * N)]);
	pns->stopTimer(SENDER,seqNum);										//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);			//�����������ͷ���ʱ��
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum % (2 * N)]);			//���·������ݰ�
}
