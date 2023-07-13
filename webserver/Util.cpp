#include "Util.h"

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int MAX_BUFF = 4096;
ssize_t readn(int fd, void* buff, size_t n) {
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char* ptr = (char*)buff;
    while (nleft > 0) {
        if((nread = read(fd, ptr, nleft)) < 0) {
            if(errno == EINTR) {
                nread = 0;
            } else if(errno == EAGAIN) {
                return readSum;
            } else {
                return -1;
            }
        } else if(nread == 0) {
            break;
        }
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inBuffer, bool &zero) {
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while (true) {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN) {
                return readSum;
            } else {
                perror("read error");
                return -1;
            }
        } else if (nread == 0) {
            zero = true;
            break;
        }
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inBuffer) {
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while (true) {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0) {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN) {
                return readSum;
            } else {
                perror("read error");
                return -1;
            }
        } else if (nread == 0) {
            break;
        }
        readSum += nread;
        inBuffer += std::string(buff, buff + nread);
    }
    return readSum;
}