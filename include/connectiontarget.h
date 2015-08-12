#ifndef _CONNECTIONTARGET_H
#define _CONNECTIONTARGET_H


class ConnectionTarget {
	public:
		ConnectionTarget();
		virtual ~ConnectionTarget();
		int type();
		
	protected:
		int cnxtype;
};
#endif