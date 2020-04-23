#ifndef H_OUTPUT
#define H_OUTPUT

#define output(fd, ... ) dprintf(fd, __VA_ARGS__)

int outputopen(int epollfd);
int outputread();
#endif
