#include "clientencoder.h"
#include "log.h"



template <typename Cnx>   
void ClientEncoder<Cnx>::ioChannel(Channel * clear) {
	clear->modeTerminal();
	this->Encoder::channelClear(clear);
}

template <typename Cnx>   
ClientEncoder<Cnx>::ClientEncoder():Client<Cnx>(),Encoder(new Runnable(),"Client") {
	/*this->mask=new sigset_t();
	//Define the sigmask  to catch SIGWINCH with pselect
	sigemptyset (this->mask);
	sigaddset (this->mask,SIGWINCH);
	sigaddset (this->mask,SIGINT);
	sigprocmask(SIG_BLOCK, this->mask, NULL);*/
	::signal(SIGWINCH, ClientEncoder::signalHandler);
	::signal(SIGINT, ClientEncoder::signalHandler);
	
}

template <typename Cnx>   
ClientEncoder<Cnx> * ClientEncoder<Cnx>::client() {
	if (theclient==NULL) theclient=new ClientEncoder();
	return theclient;
}

template <typename Cnx>   
ClientEncoder<Cnx>::~ClientEncoder() {
}

template <typename Cnx>   
void ClientEncoder<Cnx>::signalHandler(int sig) {
	switch(sig) {
		case SIGWINCH:
			Log::logger->log("CLIENTENCODER",DEBUG) << "Received signal SIGWINCH" << endl;
			theclient->resizetty();
		break;
		case SIGINT:
			Log::logger->log("CLIENTENCODER",DEBUG) << "Received signal SIGINT" << endl;
			try {
				Command cmd(Command::QUIT,"");
				cmd.send(theclient->encoded);
			}catch(InvalidCmdCodeException &e) {
				//impossible
			}
		break;
		default:
			Log::logger->log("CLIENTENCODER",DEBUG) << "Received a not handled signal : " << sig<< endl;
		break;
	}
}


template <typename Cnx>   
void ClientEncoder<Cnx>::resizetty() {
	screensize size=this->clear->termsize();
	try {
		Command cmd(Command::RESIZETTY,"%06u %06u",size.h,size.w);
		cmd.send(this->encoded);
	}catch(InvalidCmdCodeException &e) {
		//impossible
	}
}

template <typename Cnx>   
void ClientEncoder<Cnx>::execute() {
	this->setEncodedFd(this->cnx->socket(), this->cnx->socket());
	try {
		this->encode();
	}catch(EncoderInvalidFdException &e) {
		Log::logger->log("CLIENTENCODER",ERROR) << "Invalid encoder fd" << endl;
	}

}


template <typename Cnx>   
void ClientEncoder<Cnx>::executecmd(Command * cmd) {
	Log::logger->log("CLIENTENCODER",DEBUG) << "Client Received a command : " << cmd->command() << endl;
	switch (cmd->command()) {
		case Command::QUIT: //quit
			this->close();
		break;
		case Command::REQUESTTTYSIZE:
			this->resizetty();
		break;
		case Command::REQUESTAUTHENT: 
			try {
				this->authenticate();
			} catch(InterruptAuthentException &e) {
				Log::logger->log("CLIENTENCODER",DEBUG) << "Authent process interrupted" << endl;
			}
		break;
		case Command::FAILEDAUTHENT:
			try {
				this->authenticate();
			} catch(InterruptAuthentException &e) {
				Log::logger->log("CLIENTENCODER",DEBUG) << "Authent process interrupted" << endl;
			}
		break;
	}
}


template <typename Cnx>   
void ClientEncoder<Cnx>::authenticate() throw(InterruptAuthentException){
	Log::logger->log("CLIENT",DEBUG) << "Server request authentication" << endl;
	string login;
	string password;
	char buffer[100];
	
	this->clear->modeNormal();
	this->clear->write( "Login:", 6);
	int nb=this->clear->iread(buffer, 100);
	if (nb==0) throw InterruptAuthentException();
	buffer[nb]=0;
	login=buffer;
	
	this->clear->write("Password:", 9);
	this->clear->modePassword();
	nb=this->clear->iread(buffer, 100);
	if (nb==0) throw InterruptAuthentException();
	buffer[nb]=0;
	password=buffer;
	this->clear->write("\n", 1);
	
	this->clear->modeTerminal();
	Command auth(Command::AUTHENTICATE,"%s %s",login.c_str(), password.c_str());
	auth.send(this->encoded);
}

template <typename Cnx>   
ClientEncoder<Cnx> * ClientEncoder<Cnx>::theclient=NULL;


template class ClientEncoder<Connection>;

#ifdef HAVE_TCP_CONNECTION
template class ClientEncoder<ConnectionTCP>;
#endif

#ifdef HAVE_RFCOMM_CONNECTION
template class ClientEncoder<ConnectionRFCOMM>;
#endif

