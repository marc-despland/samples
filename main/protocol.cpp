#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;
#include "packet.h"
#include "message.h"
#include "command.h"

int main(int argc, char* argv[]) {
	Buffer * buffer=new Buffer();
	Packet * packet=new Packet();
	if (packet->isReady()) {
		cout << "FAILED : " ;
	} else {
		cout << "PASSED : ";
	}
	cout << "Empty packet should not be ready. isReady=" << packet->isReady() << endl;
	char * tmp=new char[100];
	memset(tmp,0,100);
	tmp[0]='g';
	memcpy(tmp+1, "0012",4);
	memcpy(tmp+5, "abcdefghijkl10006mnopqr",23);
	buffer->write(tmp,3);
	packet=new Packet();
	bool ready=false;
	try {
		ready=packet->readData(buffer);
		if (!ready && (buffer->size()==3)) {
			cout << "PASSED : Read to small header ("<<packet->getCode()<<","<<packet->getLength()<<") , return false, nothing changed" << endl;
		} else {
			cout << "FAILED : Read to small header ("<<packet->getCode()<<","<<packet->getLength()<<") , return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "FAILED : Read to small header, exception occurs" << endl;
	}catch (PacketBufferSizeException &e) {
		cout << "PASSED : Read to small header ("<<packet->getCode()<<","<<packet->getLength()<<") , return false, nothing changed" << endl;
	}
		
	try {
		packet=new Packet();
		buffer=new Buffer();
		buffer->write(tmp,6);
		ready=packet->readData(buffer);
		if (!ready && (buffer->size()==1)) {
			cout << "FAILED : Invalid header code ("<<packet->getCode()<<","<<packet->getLength()<<") , return false, nothing changed, no exception" << endl;
		} else {
			cout << "FAILED : Invalid header code("<<packet->getCode()<<","<<packet->getLength()<<") , no exeception return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "PASSED : Invalid header code, exception occurs" << endl;
	}
	try {
		packet=new Packet();
		tmp[0]='1';
		tmp[4]='z';
		buffer=new Buffer();
		buffer->write(tmp,6);
		ready=packet->readData(buffer);
		if (!ready && (buffer->size()==1)) {
			cout << "FAILED : Invalid header length ("<<packet->getCode()<<","<<packet->getLength()<<") , return false, nothing changed, no exception" << endl;
		} else {
			cout << "FAILED : Invalid header length ("<<packet->getCode()<<","<<packet->getLength()<<") , no exeception return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "PASSED : Invalid header length, exception occurs" << endl;
	}
	try {
		packet=new Packet();
		tmp[4]='2';
		buffer=new Buffer();
		buffer->write(tmp,6);
		ready=packet->readData(buffer);
		if (!ready && (buffer->size()==0)) {
			cout << "PASSED : Valid header all the data as been put on the Packet (1/3)"<< packet->getRawData()<<"("<<packet->getCode()<<","<<packet->getLength()<<") - return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size()<< endl;
		} else {
			cout << "FAILED : Valid header ("<<packet->getCode()<<","<<packet->getLength()<<") - return "<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "FAILED : Valid header , exception occurs" << endl;
	}
	try {
		buffer->write(tmp+6,6);
		ready=packet->readData(buffer);
		if (!ready && (buffer->size()==0)) {
			cout << "PASSED : Valid header all the data as been put on the Packet (2/3)"<< packet->getRawData()<<"("<<packet->getCode()<<","<<packet->getLength()<<") - return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size()<< endl;
		} else {
				cout << "FAILED : Valid header ("<<packet->getCode()<<","<<packet->getLength()<<") - return "<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "FAILED : Valid header , exception occurs" << endl;
	}
	try {
		buffer->write(tmp+12,6);
		ready=packet->readData(buffer);
		if (ready && (buffer->size()==1)) {
			cout << "PASSED : Valid header all the data as been put on the Packet (3/3) packet ready, one extra char leave in buffer "<< packet->getRawData()<<"("<<packet->getCode()<<","<<packet->getLength()<<") - return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size()<< endl;
		} else {
				cout << "FAILED : Valid header ("<<packet->getCode()<<","<<packet->getLength()<<") - return "<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "FAILED : Valid header , exception occurs" << endl;
	}
	
	Message msg="ceci est un message";
	if (19+5==msg.getRawDataLength()) {
		cout << "PASSED : Create message from text : size="<<msg.getRawDataLength()<< " raw=" <<msg.getRawData() << endl;
	} else {
		cout << "FAILED : Create message from text : size="<<msg.getRawDataLength()<< " raw=" <<msg.getRawData() << endl;
	}
	try {
		packet=new Packet();
		buffer=new Buffer();
		buffer->write(msg.getRawData(),msg.getRawDataLength());
		ready=packet->readData(buffer);
		if (ready && (buffer->size()==0)) {
			cout << "PASSED : Read packet create from Message -  packet ready, "<< packet->getRawData()<<"("<<packet->getCode()<<","<<packet->getLength()<<") - return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size()<< endl;
		} else {
			cout << "FAILED : Read packet create from Message ("<<packet->getCode()<<","<<packet->getLength()<<") - return "<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "FAILED : Read packet create from Message , exception occurs" << endl;
	}
	
	
	try {
		Message * msg2=new Message(packet);
		if (msg2->isReady() && (msg2->getLength()==19)) {
			cout << "PASSED : Create Message from Packet length=" << msg2->getLength() << " code="<<msg2->getCode() << endl;
		} else {
			cout << "FAILED : Create Message from Packet length=" << msg2->getLength() << " code="<<msg2->getCode() << endl;
		}
	} catch(InvalidMessageException &e) {
		cout << "FAILED : Create Message from Packet , exception occurs" << endl;
	}
	Command *cmd=NULL;
	try {
		cmd=new Command(1000,"");
		cout << "FAILED : Creating a command with an invalid code - no exception thrown size="<<cmd->getRawDataLength()<< " raw=" << cmd->getRawData() << endl;
	}catch (InvalidCmdCodeException &e) {
		cout << "PASSED : Creating a command with an invalid code " << endl;
	}
	try {
		cmd=new Command(99,"%04d %02d", 45, 12);
		if (Command::isCommand(cmd) && cmd->command()==99) {
			cout << "PASSED : Creating a command with an valid cmd code : "<<cmd->command()<< " size="<<cmd->getRawDataLength()<< " raw=" <<cmd->getRawData() << endl;
		} else {
			cout << "FAILED : Creating a command with an valid code size="<<cmd->getRawDataLength()<< " raw=" <<cmd->getRawData() << " not ready or not a command" << endl;
		}
	}catch (InvalidCmdCodeException &e) {
		cout << "FAILED : Creating a command with an valid code - an exception was thrown" << endl;
	}
	
	int p1,p2,result;
	result=cmd->parameters("%04d %02d",&p1,&p2);
	if (result==2 && p1==45 && p2==12) {
		cout << "PASSED : Reading command parameters cmd="<<cmd->command()<< " result="<<result<<" p1="<<p1<< " p2="<< p2 <<endl;
	}else {
		cout << "FAIELD : Reading command parameters cmd="<<cmd->command()<< " result="<<result<<" p1="<<p1<< " p2="<< p2 <<endl;
	}
	
	
	try {
		packet=new Packet();
		buffer=new Buffer();
		buffer->write(cmd->getRawData(),cmd->getRawDataLength());
		ready=packet->readData(buffer);
		if (ready && (buffer->size()==0)) {
			cout << "PASSED : Read packet create from Command -  packet ready, "<< packet->getRawData()<<"("<<packet->getCode()<<","<<packet->getLength()<<") - return="<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size()<< endl;
		} else {
			cout << "FAILED : Read packet create from Command ("<<packet->getCode()<<","<<packet->getLength()<<") - return "<<ready<<" buffer="<<buffer->raw()<<" size="<<buffer->size() << endl;
		}
			
	}catch (PacketInvalidHeaderException &e) {
		cout << "FAILED : Read packet create from Command , exception occurs" << endl;
	}
	if (Command::isCommand(packet)) {
		cout << "PASSED : Created packet is a command ("<<packet->getCode()<<","<<packet->getLength()<<") "<< packet->getRawData() << endl;
	} else {
		cout << "FAILED : Created packet is not a command ("<<packet->getCode()<<","<<packet->getLength()<<") "<< packet->getRawData()<< endl;
	}
	try {
		Command * cmd2=new Command(packet);
		if (cmd2->command()==99) {
			cout << "PASSED : Creating a command from packet : "<<cmd->command()<< " size="<<cmd->getRawDataLength()<< " raw=" <<cmd->getRawData() << endl;
		}
	} catch (InvalidCommandException &e1) {
		cout << "FAILED : Creating command from packet : InvalidCommandException occurs ("<<packet->getCode()<<","<<packet->getLength()<<") "<< packet->getRawData()<< endl;
	}catch (PacketNotReadyException &e2) {
		cout << "FAILED : Creating command from packet : PacketNotReadyException occurs ("<<packet->getCode()<<","<<packet->getLength()<<") "<< packet->getRawData()<< endl;
	}catch (InvalidCmdCodeException &e3) {
		cout << "FAILED : Creating command from packet : InvalidCmdCodeException occurs ("<<packet->getCode()<<","<<packet->getLength()<<") "<< packet->getRawData()<< endl;
	}
	result=p1=p2=0;
	result=cmd->parameters("%04d %02d",&p1,&p2);
	if (result==2 && p1==45 && p2==12) {
		cout << "PASSED : Reading command parameters cmd="<<cmd->command()<< " result="<<result<<" p1="<<p1<< " p2="<< p2 <<endl;
	}else {
		cout << "FAIELD : Reading command parameters cmd="<<cmd->command()<< " result="<<result<<" p1="<<p1<< " p2="<< p2 <<endl;
	}
	
	return 0;
}
