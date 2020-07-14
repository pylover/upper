#include "common.h"
#include "input.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <termios.h>


static int inputfd;


int inputopen(int epollfd) {
    struct epoll_event ev;
    struct termios ttystate; //, ttysave;

    infoln("Registering input");
    inputfd = STDIN_FILENO;
    
    if (settings.noncanonical) {
        //get the terminal state
        tcgetattr(STDIN_FILENO, &ttystate);
        //ttysave = ttystate;
        //turn off canonical mode and echo
        //ttystate.c_lflag &= ~(ICANON | ECHO);
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;

        //set the terminal attributes.
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    }
    ev.events = EPOLLIN | EPOLLET;  // EPOLLPRI | 
    ev.data.fd = inputfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, inputfd, &ev) == ERR) {
        perrorf("epoll_ctl: EPOLL_CTL_ADD, input device");
        return ERR;
    }
    return inputfd;
}   

