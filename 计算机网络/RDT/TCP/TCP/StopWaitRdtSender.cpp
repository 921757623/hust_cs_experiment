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
	if (this->waitingState) { //发送方处于等待确认状态
		return false;
	}

	if (expectSequenceNumberSend < base + N) {
		int current_pos = expectSequenceNumberSend % (2 * N);
		this->packetWaitingAck[current_pos].acknum = -1; //忽略该字段
		this->packetWaitingAck[current_pos].seqnum = this->expectSequenceNumberSend;
		this->packetWaitingAck[current_pos].checksum = 0;
		memcpy(this->packetWaitingAck[current_pos].payload, message.data, sizeof(message.data));
		this->packetWaitingAck[current_pos].checksum = pUtils->calculateCheckSum(this->packetWaitingAck[current_pos]);

		pUtils->printPacket("发送方发送报文", this->packetWaitingAck[current_pos]);

		if (base == expectSequenceNumberSend) {
			cout << "窗口已满，开始进行超时计时" << endl;
			//启动发送方定时器
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);
		}

		//调用模拟网络环境的sendToNetworkLayer，通过网络层发送到对方
		pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[current_pos]);
		expectSequenceNumberSend++;
	}


	//进入等待状态
	if (expectSequenceNumberSend == base + N) {
		this->waitingState = true;
	}																			//进入等待状态
	return true;
}

void StopWaitRdtSender::printWindow() {
	string tmp = "发送方滑动窗口：" + last_window + " ==> ";
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
	//检查校验和是否正确
	int checkSum = pUtils->calculateCheckSum(ackPkt);

	//如果校验和正确，并且确认序号=发送方已发送并等待确认的数据包序号
	if (checkSum == ackPkt.checksum && ackPkt.acknum >= base) {
		
		pUtils->printPacket("发送方正确收到确认", ackPkt);
		int last_base = base;
		// 窗口往前滑动
		base = ackPkt.acknum + 1;
		if (base == expectSequenceNumberSend)
		{
			cout << "已发送分组已全部接送，关闭计时器" << endl;
			this->waitingState = false;
			pns->stopTimer(SENDER, last_base);	//关闭定时器
			printWindow();
		}
		else {
			pns->stopTimer(SENDER, last_base);//还没接收完，继续等待
			pns->startTimer(SENDER, Configuration::TIME_OUT, base);
			this->waitingState = false;
		}
	}
	else {
		if (ackPkt.acknum == lastack) {
			repeat_count++;
			if (repeat_count == 4) {
				cout << "收到了三个冗余的ACK，快速重传序号" << ackPkt.acknum + 1 << endl;
				char buf[64] = { 0 };
				sprintf(buf, "快速重传序号: %d\n", ackPkt.acknum + 1);
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
			cout << "发送方收到的ACK损坏" << endl;
		}
		else {
			cout << "发送方没有收到正确的序号，继续等待" << endl;
		}
	}
	
}

void StopWaitRdtSender::timeoutHandler(int seqNum) {
	//唯一一个定时器,无需考虑seqNum
	pUtils->printPacket("发送方定时器时间到，重发上次发送的报文", this->packetWaitingAck[seqNum % (2 * N)]);
	pns->stopTimer(SENDER,seqNum);										//首先关闭定时器
	pns->startTimer(SENDER, Configuration::TIME_OUT,seqNum);			//重新启动发送方定时器
	pns->sendToNetworkLayer(RECEIVER, this->packetWaitingAck[seqNum % (2 * N)]);			//重新发送数据包
}
