#include "common.h"
#include "input.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>


static int inputfd;


int inputopen(int epollfd) {
    struct epoll_event ev;
    infoln("Registering input");
    inputfd = STDIN_FILENO;
    ev.events = EPOLLIN | EPOLLPRI;
    ev.data.fd = inputfd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, inputfd, &ev) == ERR) {
        perrorf("epoll_ctl: EPOLL_CTL_ADD, input device");
        return ERR;
    }
    return inputfd;
}   

