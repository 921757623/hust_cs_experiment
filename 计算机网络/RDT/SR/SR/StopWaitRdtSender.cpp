#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtSender.h"


StopWaitRdtSender::StopWaitRdtSender():base(0), expectSequenceNumberSend(0),waitingState(false)
{
	memset(isAck, false, sizeof(isAck));
	last_window = " -1 [  0   1   2   3]   4";
	file.open("window_sender.txt", ios::out);
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
		// ���ͳ�ȥ�˵���δ���ܵ�ack��Ӧ
		isAck[current_pos] = false;

		pUtils->printPacket("���ͷ����ͱ���", this->packetWaitingAck[current_pos]);


		pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetWaitingAck[current_pos].seqnum);			//�������ͷ���ʱ��
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[current_pos]);								//����ģ�����绷����sendToNetworkLayer��ͨ������㷢�͵��Է�
		expectSequenceNumberSend++;
		//����ȴ�״̬
		if (expectSequenceNumberSend == base + N) {
			this->waitingState = true;
		}																			
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

	//���У�����ȷ������ȷ�����=���ͷ��ѷ��Ͳ��ȴ�ȷ�ϵ����ݰ����
	if (checkSum == ackPkt.checksum
		) {
		pUtils->printPacket("���ͷ���ȷ�յ�ȷ��", ackPkt);
		if (ackPkt.acknum == base) {
			cout << "��ȷ��ACK���Ϊ" << ackPkt.acknum << "��ACK" << endl;
			pns->stopTimer(SENDER, ackPkt.acknum);
			isAck[base % (2 * N)] = true;
			// ������ǰ��ֱ�������û�е��յ�ACK��λ��
			while (isAck[base % (2 * N)]) {
				isAck[base++ % (2 * N)] = false;
			}
			printWindow();
			waitingState = false;
		}
		else if (ackPkt.acknum > base && !isAck[ackPkt.acknum % (2 * N)]) {
			cout << "��ȷ��ACK���Ϊ" << ackPkt.acknum << "��ACK" << endl;
			pns->stopTimer(SENDER, ackPkt.acknum);
			isAck[ackPkt.acknum % (2 * N)] = true;
		}
		else
		{
			cout << "�յ�����Ҫ�����е�ACK,�����ȴ�" << endl;
		}
	}
	else {
		cout << "���ͷ��յ���ACK��" << endl;
	}
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	//Ψһһ����ʱ��,���迼��seqNum
	pUtils->printPacket("���ͷ���ʱ��ʱ�䵽���ط��ϴη��͵ı���", this->packetWaitingAck[seqNum % (2 * N)]);
	pns->stopTimer(SENDER,seqNum);										//���ȹرն�ʱ��
	pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);			//�����������ͷ���ʱ��
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum % (2 * N)]);			//���·������ݰ�
}
