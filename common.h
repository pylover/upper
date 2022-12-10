#ifndef H_COMMON
#define H_COMMON

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define VERSION "1.3.0"
#define OK  0
#define ERR -1

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


extern int errno;

#define STDOUT  0
#define EPOLL_MAXEVENTS 10

/* Used to create string literal from integer during preprocess. */
#define _STR(X) #X
#define STR(X) _STR(X)


#define LOG(file, cr, ...) do {	\
	fprintf(file, __VA_ARGS__); \
    if (cr) { \
	    fprintf(file, "\n" ); \
    } \
    if ((file == stderr) && errno) { \
        fprintf(file, "Additional info: %s\n", strerror(errno)); \
    } \
	fflush(file); \
} while(0)
 

#define info( ... ) if (settings.verbose) { LOG(stderr, 0, __VA_ARGS__); }
#define infoln( ... ) if (settings.verbose) { LOG(stderr, 1, __VA_ARGS__); }
#define perrorf( ... ) LOG(stderr, 1, __VA_ARGS__)
#define infosocket(m, a) \
    infoln("%s%s:%d", m, inet_ntoa(a.sin_addr), ntohs(a.sin_port))


#define DEFAULT_BAUDRATE    115200
#define DEFAULT_OUTPUT      "-"
#define DEFAULT_TCPPORT     5600

struct Settings {
    char *output;
    unsigned int baudrate;
    bool verbose;
    int tcpport;
    bool noupper;
    bool noncanonical;
};


extern struct Settings settings;

char * strnstr(const char *s, const char *find, size_t slen);

#endif
