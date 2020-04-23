
#include "common.h"
#include "cli.h"
#include "output.h"
#include "input.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/epoll.h>


static char buffer[1024];


static int process_inputevent(int inputfd, int outfd) {
    int i, err;

    err = read(inputfd, buffer, 1023);
    if (err < 1) {
        return OK;
    }
    buffer[err] = 0;

    // Make all chars upper case
    for (i = 0; i < err; i++) {
        buffer[i] = toupper(buffer[i]);
    }

    output(outfd, "%s", buffer);
    
//    err = gcodeget(&jse, gcode, &bytes);
//    if (err == ERR) {
//        perrorf(
//            "Unrecognized command: %d, %d, %d", 
//            jse.type, 
//            jse.number, 
//            jse.value
//        );
//        timerset(TIMER_OFF);
//        return OK;
//    }
//    else if (err == GCODE_REPEAT) {
//        timerset(TIMER_ON);
//    }
//    else if (err == GCODE_STOPREPEATE) {
//        timerset(TIMER_OFF);
//        return OK;
//    }
//    else if (err == GCODE_IGNORE) {
//        timerset(TIMER_OFF);
//        return OK;
//    }
//   
//    output(outfd, "%s\n", gcode);
	return OK;
}




int main(int argc, char **argv) {
    int epollfd, inputfd, err, fdcount, outfd, i;
    struct epoll_event ev, events[EPOLL_MAXEVENTS];
    
    // Parse command line arguments
    cliparse(argc, argv);
    
    // epoll
    epollfd = epoll_create1(0);
    if (epollfd < 0) {
        perrorf("Cannot create epoll file descriptor");
        exit(EXIT_FAILURE);
    }
  
    // input
    inputfd = inputopen(epollfd);
    if (inputfd == ERR) {
        perrorf("Cannot open standard input.");
        exit(EXIT_FAILURE);
    }

    // output
    outfd = outputopen(epollfd);
    if (outfd == ERR) {
        perrorf("Cannot open output: %s", settings.output);
        exit(EXIT_FAILURE);
    }
 
    /* Main Loop */
    while (1) {
        fdcount = epoll_wait(epollfd, events, EPOLL_MAXEVENTS, -1);
        if (fdcount == -1) {
            perrorf("epoll_wait returned: %d", fdcount);
            exit(EXIT_FAILURE);
        }
    
        for (i = 0; i < fdcount; i++) {
            ev = events[i];
            if (ev.data.fd == inputfd) {
                err = process_inputevent(inputfd, outfd);
                if (err == ERR) {
                    exit(EXIT_FAILURE);
                }

            }
            if (ev.data.fd == outfd) {
                err = outputread();
                if (err == ERR) {
                    perrorf("cannot read from out device");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
