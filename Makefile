
all:	tcpserver tcpclient terminal

tcpserver:
	make -C tcpserver 

tcpclient:
	make -C tcpclient

terminal:
	make -C terminal

.PHONY: tcpserver tcpclient terminal
