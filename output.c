#include "common.h"
#include "tty.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/un.h>


static int outfd;


static int unixopen() {
    struct sockaddr_un addr;
    char filename[256];
    int fd, err;

    strcpy(filename, settings.output + 7);
   
    /* Create local socket. */
    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == ERR) {
        return fd;
    }
    
    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, filename, sizeof(addr.sun_path) - 1);

    err = connect(
        fd, 
        (const struct sockaddr *) &addr,
        sizeof(struct sockaddr_un)
    );
    if (err == ERR) {
        perrorf("Server is down: %s", settings.output);
        return ERR;
    }

    return fd;
}

static int tcpopen() {
	int err, fd, option = 1;
    struct sockaddr_in serv_addr;
    struct hostent *he;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == ERR) {
        return fd;
    }
    
	err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (err == ERR) { 
        return err;
    }

	memset(&serv_addr, 0, sizeof(serv_addr));
    
    he = gethostbyname(settings.output);
    if (he == NULL) {
        perrorf("Cannot resolve host address: %s", settings.output);
        return ERR;
	}
   
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);
	serv_addr.sin_port = htons(settings.tcpport); 

	infoln(
        "Connecting to %s:%d", 
        inet_ntoa(serv_addr.sin_addr), 
        settings.tcpport
    );
	err = connect(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
	if (err == ERR) {
        return err;
	}
	return fd;
}

/*
 * stdout
 * /dev/tty*
 * example.com:5600
 * /run/serialhub.socket
 */

int outputopen(int epollfd) {
    struct epoll_event ev;
    
    if (settings.output[0] == '-') {
        infoln("Using stdout as output device");
        outfd = STDOUT;
        // Return the std output without registering for EPOLLIN
        return outfd; 
    }

    if (strnstr(settings.output, "/dev/tty", 8) != NULL) {
        infoln("Using tty device: %s as output.", settings.output);
        outfd = serialopen();
        if (outfd == ERR) {
            return ERR;
        }
        // Wait some times to allow marlin to initialize the serial communication
        sleep(2);
    }
    else if (strnstr(settings.output, "unix://", 7) != NULL) {
        outfd = unixopen();
        if (outfd == ERR) {
            return ERR;
        }
    }
    else {
        outfd = tcpopen();
        if (outfd == ERR) {
            return ERR;
        }
    }

    ev.events = EPOLLIN;
    ev.data.fd = outfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, outfd, &ev) == ERR) {
        perrorf("epoll_ctl: ADD, output device");
        return ERR;
    }
    
    return outfd;
}


int outputread() {
    int err;
    char buff[1025];
    err = read(outfd, buff, 1024);
    if (err == ERR) {
        return err;
    }
    buff[err] = 0;
    info("%s", buff);
    return OK;
}



