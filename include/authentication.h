#ifndef _AUTHENTICATION_H
#define _AUTHENTICATION_H
#include <exception>
#include <security/pam_appl.h>
#include <string>
#include <sys/types.h>
#include "runnable.h"
#include "encoder.h"

using namespace std;

class PamException : public exception {}; 
class AuthentFailureException : public exception {}; 
class UserNotAuthenticateException : public exception {}; 


class Authentication: public Runnable, Encoder {
	public: 
		Authentication();
		void authenticate(string username, string password) throw (PamException, AuthentFailureException);
		bool authenticate(int encodedin, int encodedout);
		uid_t uid() throw(UserNotAuthenticateException);
		gid_t gid() throw(UserNotAuthenticateException);
	protected:
		static int pam_conversation(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr);
		string username;
		string password;
		uid_t userid;
		gid_t groupid;
		bool authentok;
		int failurecount;
		void executecmd(Command * cmd);
};
#endif
