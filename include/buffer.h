#ifndef _BUFFER_H
#define _BUFFER_H
using namespace std;
#include <exception>


class PacketBufferSizeException : public exception {
	private:
		unsigned int size;
		unsigned int requested;
	public:
	PacketBufferSizeException(unsigned int size, unsigned int requested);
	const char* what();
};

/**
 * Buffer class implement a char buffer
 */
class Buffer {
	public:
		Buffer();
		~Buffer();
		unsigned int size();
		char * read(unsigned int size) throw(PacketBufferSizeException);
		void write(const char * data, unsigned int size);
		char * raw();
		void clear();

	protected:
		char * data;
		unsigned int datasize;
		unsigned int allocated;
};
#endif
