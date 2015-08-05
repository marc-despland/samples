#include "authentication.h"
#include "log.h"
#include <string.h>
#include <pwd.h>
#include <iostream>
#include "runnable.h"
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define MAXFAILURE 3

int Authentication::pam_conversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) {
	Log::logger->log("AUTHENT", DEBUG) << "Received pam conversation : num_msg="<<num_msg<<endl;
	for (int i=0;i<num_msg;i++) {
		Log::logger->log("AUTHENT", DEBUG) << "\tmsg["<<i<<"]: "<<msg[i]->msg<<endl;
	}
	if (appdata_ptr!=NULL) {
		Authentication * auth=(Authentication *) appdata_ptr;
  		*resp = (struct pam_response *)malloc(sizeof(struct pam_response));
		(*resp)[0].resp=strdup(auth->password.c_str());
		(*resp)[0].resp_retcode=0;
		return PAM_SUCCESS;	
	}
	return PAM_AUTH_ERR;
}

Authentication::Authentication():Runnable(), Encoder(this,"Authentication") {
	this->username="";
	this->password="";
	this->userid=-1;
	this->groupid=-1;
	this->authentok=false;
	this->failurecount=0;
}


uid_t Authentication::uid() throw (UserNotAuthenticateException) {
	if (!this->authentok) throw UserNotAuthenticateException();
	return this->userid;
}
gid_t Authentication::gid() throw (UserNotAuthenticateException) {
	if (!this->authentok) throw UserNotAuthenticateException();
	return this->groupid;
}

void Authentication::authenticate(string username, string password) throw (PamException, AuthentFailureException) {
	this->authentok=false;
	this->userid=-1;
	this->groupid=-1;
	Log::logger->log("AUTHENT", DEBUG) << "Authentication::authenticate : username="<<username<<endl;
	pam_handle_t *local_auth_handle = NULL;
	this->username=username;
	this->password=password;
	struct pam_conv * local_conversation = (struct pam_conv *) malloc(sizeof(struct pam_conv));
	local_conversation->conv=Authentication::pam_conversation;
	local_conversation->appdata_ptr=(void *)this;
	int retval;
	
	// local_auth_handle gets set based on the service
  	retval = pam_start("system-auth", this->username.c_str(), local_conversation, &local_auth_handle);
	if (retval != PAM_SUCCESS) {
    		Log::logger->log("AUTHENT", EMERGENCY) << "Pam Start failure : " << retval << std::endl;
    		throw PamException();
  	}

  	retval = pam_authenticate(local_auth_handle, 0);
	Log::logger->log("AUTHENT", DEBUG) << "PAM authenticate for username="<<username <<" return " << retval<<endl;
	
  	if (retval != PAM_SUCCESS) {
    		if (retval == PAM_AUTH_ERR) {
            		Log::logger->log("AUTHENT", INFO) << "Authentication failure for user " << this->username << std::endl;
            		throw AuthentFailureException();
    		} else {
        		Log::logger->log("AUTHENT", EMERGENCY) << "Pam Authentification failure : " << retval << std::endl;
        		throw PamException();
    		}
  	}
  	retval = pam_end(local_auth_handle, retval);

  	if (retval != PAM_SUCCESS) {
        	Log::logger->log("AUTHENT", EMERGENCY) << "Pam End failure : " << retval << std::endl;
        	throw PamException();
  	}
  	struct passwd * pwd=getpwnam(this->username.c_str());
	if (pwd!=NULL) {
		this->userid=pwd->pw_uid;
		this->groupid=pwd->pw_gid;
	}
	this->authentok=true;
}


bool Authentication::authenticate(int encodedin, int encodedout) {
	this->setEncodedFd(encodedin, encodedout);
	this->start();
	Command auth(Command::REQUESTAUTHENT,"");
	auth.send(encodedout);
	this->encode();
	return this->authentok;
}

void Authentication::executecmd(Command * cmd) {
	char login[100];
	char password[100];
	memset(login,0,100);
	memset(password,0,100);
	Log::logger->log("AUTHENT", NOTICE)  << "We receive a command " << cmd->command()<< endl;
	switch (cmd->command()) {
		case Command::QUIT: //quit
			this->stop();
		case Command::AUTHENTICATE: 
			cmd->parameters("%s %s",&login,&password);
			Log::logger->log("AUTHENT", DEBUG)  << "Request authentication for user " << login<< endl;
			try {
				this->authenticate(login, password);
				// success
				this->stop();
			}  catch (PamException &e) {
				Log::logger->log("AUTHENT", EMERGENCY) << "Can't comlete PAM authentification" << endl;
				this->stop();
			} catch (AuthentFailureException &e) {
				Log::logger->log("AUTHENT", INFO) << "Invalid credentials for user "<<login << endl;
				this->failurecount++;
				if (this->failurecount<MAXFAILURE) {
					Command failure(Command::FAILEDAUTHENT,"");
					failure.send(this->encodedout);
				} else {
					Command quit(Command::FAILEDAUTHENT,"");
					quit.send(this->encodedout);
					this->stop();
				}
			}
		break;
	}
}

