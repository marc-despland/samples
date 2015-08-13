#ifndef _PACKET_H
#define _PACKET_H
using namespace std;
#include <exception>
#include "buffer.h"
#include "channel.h"


class PacketInvalidHeaderException : public exception {
	const char* what();
};

class PacketNotReadyException : public exception {
	const char* what();
};

/**
 * Packet class manage based data exchange between client and server. 
 * Packet is composed of a code that describe the type of packet, a 
 * length encoded on 4 digit that decribe the size of the data, and a block of data.
 */
class Packet {
	protected:
		/**
		 * Create a new empty packet with thegiven code
		 */
		Packet(unsigned short code);
	public:
		/**
		 * Create a new empty packet
		 */
		Packet();
		/**
		 * Create a Packet initialize with the given parameter. This Packet is ready after its creation
		 * @param code the code of the Packet that describe its type
		 * @param data the data of the packet
		 * @param size the size of the packet data
		 */
		Packet(unsigned short code, char * data, unsigned int size);
		/**
		 * Delete the Packet
		 */
		~Packet();
		/**
		 * Read data from a buffer to create a Packet. The data consume is removed from the buffer and 
		 * size is adjust. If the buffer doesn't contain all the data of a Packet, the packet is flag not Ready
		 * @param buffer a pointer to the buffer, could be modified by the function
		 * @param size a pointer to the buffer size, could be modified by the function
		 * @return true if the Packet is ready
		 */
		bool readData(Buffer * buffer) throw (PacketInvalidHeaderException,PacketBufferSizeException);
		/**
		 * Return the code of the Packet
		 */
		unsigned short getCode();
		/**
		 * Return the length of the Packet's data
		 */
		unsigned int getLength();
		/**
		 * Return the data of the Packet
		 */
		char * getData();
		/**
		 * Return true is all the data of the packet is available
		 */
		bool isReady();
		/**
		 * Return the raw data of teh packet. one digit for the code, 4 digit for the size and then the data
		 */
		unsigned int getRawDataLength();
		/**
		 * Return the lenght of the raw data
		 */
		char * getRawData();
		/**
		 * Send the rawdata of the packet through the given fd
		 */
		long send(int fd) throw (PacketNotReadyException);
		/**
		 * Send the rawdata of the packet through the given channel
		 */
		long send(Channel * out) throw (PacketNotReadyException);
		
		void clear();
		
		static const unsigned short NOCODE=10;
	protected:
		unsigned short code;
		unsigned int length;
		unsigned int datasize;
		char * data;
		
};
#endif
