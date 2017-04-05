#ifndef structiwezel_h
#define structiwezel_h

struct iwezel//pipe_inode_info
{
	unsigned int start;//wskaznik do poczatku
	unsigned int koniec;//wskaznik do konca
	unsigned int reader;//ilu czyta
	unsigned int writer;//ilu pisze
	int lock;
}node;
#endif
