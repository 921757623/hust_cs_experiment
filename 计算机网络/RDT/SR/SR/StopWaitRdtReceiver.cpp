#include "stdafx.h"
#include "Global.h"
#include "StopWaitRdtReceiver.h"


StopWaitRdtReceiver::StopWaitRdtReceiver():base(0), expectSequenceNumberRcvd(N)
{
	lastAckPkt.acknum = -1; //初始状态下，上次发送的确认包的确认序号为-1，使得当第一个接受的数据包出错时该确认报文的确认号为-1
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;	//忽略该字段
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
	string tmp = "接收方滑动窗口：" + last_window + " ==> ";
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
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(packet);

	//如果校验和正确，同时收到报文的序号等于接收方期待收到的报文序号一致
	if (checkSum == packet.checksum) {
		
		cout << "接收方收到的报文序号为" << packet.seqnum << endl;
	
		int current_pos = packet.seqnum % (2 * N);
		if (base == packet.seqnum) 
		{
			pUtils->printPacket("接收方正确收到发送方的报文", packet);
			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
			
			isWait[current_pos] = true;
			// 缓存数据包
			ReceivedPacket[current_pos] = packet;
			ReceivedPacket[current_pos].acknum = 0;
			// 将窗口前推到下一个未收到的分组
			while (isWait[base % (2 * N)])
			{
				//取出Message，向上递交给应用层
				Message msg;
				memcpy(msg.data, ReceivedPacket[base % (2 * N)].payload, sizeof(ReceivedPacket[base % (2 * N)].payload));
				pns->delivertoAppLayer(RECEIVER, msg);
				// 窗口前移
				isWait[base++ % (2 * N)] = false;
				isWait[expectSequenceNumberRcvd++ % (2 * N)] = false;

				ReceivedPacket[current_pos].acknum = -1;
			}
			printWindow();
		}
		else if (base < packet.seqnum && packet.seqnum < expectSequenceNumberRcvd)
		{
			pUtils->printPacket("接收方正确收到发送方的报文", packet);
			//放到缓存区
			ReceivedPacket[current_pos] = packet;
			isWait[current_pos] = true;

			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		}
		// 虽然曾经确认过，仍再次发送n的确认ACK(n)
		else if (base - N <= packet.seqnum && packet.seqnum < base)
		{
			pUtils->printPacket("接收方正确收到已确认的过时报文", packet);
			lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
			lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
			pUtils->printPacket("接收方发送确认报文", lastAckPkt);
			pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
		}
		else
		{
			pUtils->printPacket("接收方没有正确收到发送方的报文,报文序号不对", packet);
			cout << "此时接收方期待的序号是" << base << "~" << expectSequenceNumberRcvd << "之间" << endl;
		}
	}
	else {
		if (checkSum != packet.checksum) {
			pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
		}
		else {
			pUtils->printPacket("接收方没有正确收到发送方的报文,报文序号不对", packet);
		}
		pUtils->printPacket("接收方重新发送上次的确认报文", lastAckPkt);
		pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送上次的确认报文

	}
}