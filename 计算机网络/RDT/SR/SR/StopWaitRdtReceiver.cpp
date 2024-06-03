#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"


StopWaitRdtReceiver::StopWaitRdtReceiver():base(0), expectSequenceNumberRcvd(N)
{
	lastAckPkt.acknum = -1; //��ʼ״̬�£��ϴη��͵�ȷ�ϰ���ȷ�����Ϊ-1��ʹ�õ���һ�����ܵ����ݰ�����ʱ��ȷ�ϱ��ĵ�ȷ�Ϻ�Ϊ-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//���Ը��ֶ�
	for(int i = 0; i < Configuration::PAYLOAD_SIZE;i++){
		lastAckPkt.payload[i] = '.';
	}
	memset(isWait, false, sizeof(isWait));
	lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
	last_window = " -1 [  0   1   2   3]   4";
	file.open("window_receiver.txt", ios::out);
}


StopWaitRdtReceiver::~StopWaitRdtReceiver()
{
	file.close();
}

void StopWaitRdtReceiver::printWindow() {
	string tmp = "���շ��������ڣ�" + last_window + " ==> ";
	file.write(tmp.c_str(), tmp.size());
	cout << tmp;
	char buf[64] = { 0 };
	sprintf(buf, "%3d [%3d %3d %3d %3d] %3d", base - 1, base + 0, base + 1, base + 2, base + 3, base + 4);
	file.write(buf, strlen(buf));
	file.write("\n", 1);
	last_window = string(buf);
	cout << last_window << endl;
}

void StopWaitRdtReceiver::receive(const Packet &packet) {
	//���У����Ƿ���ȷ
	int checkSum = pUtils->calculateCheckSum(packet);

	//���У�����ȷ��ͬʱ�յ����ĵ���ŵ��ڽ��շ��ڴ��յ��ı������һ��
	if (checkSum == packet.checksum) {
		
		cout << "���շ��յ��ı������Ϊ" << packet.seqnum << endl;
	
		int current_pos = packet.seqnum % (2 * N);
		if (base == packet.seqnum) 
		{
			pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
			
			isWait[current_pos] = true;
			// �������ݰ�
			ReceivedPacket[current_pos] = packet;
			ReceivedPacket[current_pos].acknum = 0;
			// ������ǰ�Ƶ���һ��δ�յ��ķ���
			while (isWait[base % (2 * N)])
			{
				//ȡ��Message�����ϵݽ���Ӧ�ò�
				Message msg;
				memcpy(msg.data, ReceivedPacket[base % (2 * N)].payload, sizeof(ReceivedPacket[base % (2 * N)].payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				// ����ǰ��
				isWait[base++ % (2 * N)] = false;
				isWait[expectSequenceNumberRcvd++ % (2 * N)] = false;

				ReceivedPacket[current_pos].acknum = -1;
			}
			printWindow();
		}
		else if (base < packet.seqnum && packet.seqnum < expectSequenceNumberRcvd)
		{
			pUtils->printPacket("���շ���ȷ�յ����ͷ��ı���", packet);
			//�ŵ�������
			ReceivedPacket[current_pos] = packet;
			isWait[current_pos] = true;

			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		}
		// ��Ȼ����ȷ�Ϲ������ٴη���n��ȷ��ACK(n)
		else if (base - N <= packet.seqnum && packet.seqnum < base)
		{
			pUtils->printPacket("���շ���ȷ�յ���ȷ�ϵĹ�ʱ����", packet);
			lastAckPkt.acknum = packet.seqnum; //ȷ����ŵ����յ��ı������
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("���շ�����ȷ�ϱ���", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢��ȷ�ϱ��ĵ��Է�
		}
		else
		{
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,������Ų���", packet);
			cout << "��ʱ���շ��ڴ��������" << base << "~" << expectSequenceNumberRcvd << "֮��" << endl;
		}
	}
	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,����У�����", packet);
		}
		else {
			pUtils->printPacket("���շ�û����ȷ�յ����ͷ��ı���,������Ų���", packet);
		}
		pUtils->printPacket("���շ����·����ϴε�ȷ�ϱ���", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//����ģ�����绷����sendToNetworkLayer��ͨ������㷢���ϴε�ȷ�ϱ���

	}
}