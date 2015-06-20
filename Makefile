
all:	tcpserver tcpclient terminal options

tcpserver:
	make -C tcpserver 

tcpclient:
	make -C tcpclient

terminal:
	make -C terminal

options:
	make -C options


.PHONY: tcpserver tcpclient terminal options
