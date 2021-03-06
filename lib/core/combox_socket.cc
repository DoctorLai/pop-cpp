
/*
 * Note(BW): The complex functions are completely separated between the windows and linux
 * implementation in order to make them more readable and maintainable. The simple functions
 * are still contains conditional Linux / Windows code for sake of clarity.
 */

#include <cerrno>

#include "pop_intface.h"

#ifndef __WIN32__
#include <fcntl.h>
#endif

#include "pop_combox_socket.h"
#include "pop_system.h"
#include "pop_logger.h"

#define PROTO_TCP 6
#define PROTO_UDP 17

#ifndef __WIN32__

// Following are the Linux implementations

bool pop_combox_socket::Create(int port, bool server) {
    Close();
    isServer = server;

    auto protocol = PROTO_TCP;
    auto type = SOCK_STREAM;
    sockfd = popc_socket(PF_INET, type, protocol);

    if (sockfd < 0) {
        return false;
    }

    if (port > 0) {
        sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = popc_htons(port);

        // lwk : Added this line to allow reuse an earlier socket with the same address
        SetOpt(SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sin), sizeof(sin));

        if (popc_bind(sockfd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin))) {
            return false;
        }
    }

    if (server) {
        pollarray.resize(1);
        pollarray[0].fd = sockfd;
        pollarray[0].events = POLLIN;
        pollarray[0].revents = 0;
        index = 1;
        nready = 0;

        connarray.resize(1);
        connarray[0] = CreateConnection(sockfd);

        return popc_listen(sockfd, 10) == 0;
    } else {
        peer = CreateConnection(-1);
    }

    return true;
}

bool pop_combox_socket::Connect(const char* host, int port) {
    sockaddr_in sin;
    memset(reinterpret_cast<char*>(&sin), 0, sizeof(sin));
    sin.sin_family = AF_INET;

    hostent* phe;
    if ((phe = gethostbyname(host))) {
        memcpy(reinterpret_cast<char*>(&sin.sin_addr), phe->h_addr, phe->h_length);
    } else if (static_cast<int>((sin.sin_addr.s_addr = popc_inet_addr(host))) == -1) {
        return false;
    }

    sin.sin_port = popc_htons(port);

    if (timeout <= 0) {
        return popc_connect(sockfd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == 0;
    } else {
        auto flag = fcntl(sockfd, F_GETFL, 0);
        auto newflag = flag | O_NONBLOCK;
        fcntl(sockfd, F_SETFL, newflag);

        auto ret = popc_connect(sockfd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
        auto err = errno;

        if (ret == -1 && errno == EINPROGRESS) {
            pollfd me;
            me.fd = sockfd;
            me.events = POLLOUT;
            me.revents = 0;

            int t;
            while ((t = poll(&me, 1, timeout)) == -1 && errno == EINTR)
                ;

            if (t != 1) {
                err = ETIMEDOUT;
            } else {
                socklen_t len = sizeof(int);
                if (GetOpt(SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err), len) == 0) {
                    if (!err) {
                        ret = 0;
                    }
                } else {
                    err = errno;
                }
            }
        }

        fcntl(sockfd, F_SETFL, flag);

        if (ret != 0) {
            errno = err;
        }

        return ret == 0;
    }
}

pop_connection* pop_combox_socket::Wait() {
    if (sockfd < 0 || isCanceled) {
        isCanceled = false;
        return nullptr;
    }

    if (isServer) {
        pollfd* tmpfd;

        while (1) {
            if (nready > 0) {
                int n = pollarray.size();
                tmpfd = pollarray.data() + index;
                for (int i = index; i >= 0; i--, tmpfd--) {
                    if (tmpfd->revents != 0) {
                        nready--;
                        index = i - 1;
                        tmpfd->revents = 0;

                        if (i == 0) {
                            // Accept new connection....
                            sockaddr addr;
                            socklen_t addrlen = sizeof(addr);
                            int s;
                            while ((s = popc_accept(sockfd, &addr, &addrlen)) < 0 && errno == EINTR)
                                ;
                            if (s < 0) {
                                return nullptr;
                            }

                            pollarray.resize(n + 1);
                            pollarray[n].fd = s;
                            pollarray[n].events = POLLIN;
                            pollarray[n].revents = 0;
                            connarray.resize(n + 1);
                            connarray[n] = CreateConnection(s);
                            auto ret = OnNewConnection(connarray[n]);
                            n++;

                            if (!ret) {
                                return nullptr;
                            }
                        } else {
                            return connarray[i];
                        }
                    }
                }
            }

            // Poll for ready fds....
            do {
                tmpfd = pollarray.data();
                int n = pollarray.size();
                index = n - 1;
                nready = poll(tmpfd, n, timeout);
            } while (nready < 0 && errno == EINTR && sockfd >= 0);

            if (nready <= 0) {
                if (!nready) {
                    errno = ETIMEDOUT;
                }

                return nullptr;
            }
        }
    } else {
        if (timeout >= 0) {
            pollfd tmpfd;
            tmpfd.fd = sockfd;
            tmpfd.events = POLLIN;
            tmpfd.revents = 0;

            int t;
            while ((t = poll(&tmpfd, 1, timeout)) == -1 && errno == EINTR)
                ;

            if (t <= 0) {
                if (!t) {
                    errno = ETIMEDOUT;
                }
                return nullptr;
            }
        }
        return peer;
    }
}

void pop_combox_socket::Close() {
    int fd = sockfd;
    sockfd = -1;
    nready = 0;
    index = -1;

    if (isServer) {
        for (std::size_t i = 0; i < pollarray.size(); i++) {
            if (fd != pollarray[i].fd) {
                OnCloseConnection(connarray[i]);
            }

            popc_close(pollarray[i].fd);

            delete connarray[i];
        }

        pollarray.clear();
        connarray.clear();
    } else {
        if (peer) {
            OnCloseConnection(peer);
            delete peer;
            peer = nullptr;
        }

        if (fd >= 0) {
            popc_close(fd);
        }
    }
}

bool pop_combox_socket::CloseSock(int fd) {
    if (isServer) {
        for (std::size_t i = 0; i < pollarray.size(); i++) {
            if (pollarray[i].fd == fd) {
                isCanceled = !OnCloseConnection(connarray[i]);
                delete connarray[i];
                connarray.erase(connarray.begin() + i);
                pollarray.erase(pollarray.begin() + i);
                if (isCanceled) {
                    errno = ECANCELED;
                }
                popc_close(fd);
                return !isCanceled;
            }
        }
    } else if (peer && fd == sockfd) {
        isCanceled = true;
        popc_close(fd);
        sockfd = -1;
        return true;
    }

    return false;
}

#else

// Following are the Windows implementations

bool pop_combox_socket::Create(int port, bool server) {
    Close();
    isServer = server;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    auto protocol = PROTO_TCP;
    auto type = SOCK_STREAM;
    sockfd = popc_socket(PF_INET, type, protocol);

    if (sockfd < 0) {
        return false;
    }

    if (port > 0) {
        sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = popc_htons(port);

        // lwk : Added this line to allow reuse an earlier socket with the same address
        SetOpt(SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&sin), sizeof(sin));

        if (popc_bind(sockfd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin))) {
            return false;
        }
    }

    if (server) {
        FD_ZERO(&activefdset);
        FD_ZERO(&readfds);

        FD_SET(sockfd, &activefdset);
        highsockfd = sockfd;
        nready = 0;

        connarray.resize(1);
        connarray[0] = CreateConnection(sockfd);

        sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(0);

        if (popc_bind(sockfd, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
            LOG_ERROR("bind error");
            WSACleanup();
            return false;
        }

        if (popc_listen(sockfd, 10) == SOCKET_ERROR) {
            LOG_ERROR("listen error");
            WSACleanup();
            return false;
        }

        return true;
    } else {
        peer = CreateConnection(-1);
    }

    return true;
}

bool pop_combox_socket::Connect(const char* host, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sockaddr_in sin;
    memset(reinterpret_cast<char*>(&sin), 0, sizeof(sin));
    sin.sin_family = AF_INET;

    hostent* phe;
    if ((phe = gethostbyname(host))) {
        memcpy(reinterpret_cast<char*>(&sin.sin_addr), phe->h_addr, phe->h_length);
    } else if (static_cast<int>((sin.sin_addr.s_addr = popc_inet_addr(host))) == -1) {
        return false;
    }

    sin.sin_port = popc_htons(port);

    if (timeout <= 0) {
        return popc_connect(sockfd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == 0;
    } else {
        unsigned long ul = 1;
        ioctlsocket(sockfd, FIONBIO, &ul);

        auto ret = popc_connect(sockfd, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
        auto err = errno;
        if (ret == -1 /*&& GetLastError()== WSAEINPROGRESS*/) {
            fd_set tmpwritefds;
            FD_ZERO(&tmpwritefds);
            FD_SET(sockfd, &tmpwritefds);

            timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = timeout % 1000;

            int t;

            while ((t = select(sockfd + 1, (fd_set*)0, &tmpwritefds, (fd_set*)0, &tv)) == -1 &&
                   GetLastError() == WSAEINTR)
                ;

            if (t != 1) {
                err = ETIMEDOUT;
            } else {
                socklen_t len = sizeof(int);
                if (GetOpt(SOL_SOCKET, SO_ERROR, (char*)(&err), len) == 0) {
                    if (err == 0) {
                        ret = 0;
                    }
                } else {
                    err = errno;
                }
            }
        }

        ul = 0;
        ioctlsocket(sockfd, FIONBIO, &ul);

        if (ret) {
            errno = err;
        }

        return ret == 0;
    }
}

pop_connection* pop_combox_socket::Wait() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (sockfd < 0 || isCanceled) {
        isCanceled = false;
        return nullptr;
    }

    if (isServer) {
        int tmpfdset;
        while (1) {
            if (nready > 0) {
                FD_ZERO(&readfds);
                readfds = activefdset;
                int n = readfds.fd_count;
                for (int i = n - 1; i >= 0; i--) {
                    if (readfds.fd_array[i] == tmpfdset) {
                        nready--;

                        if (i == 0) {
                            // Accept new connection....
                            sockaddr addr;
                            socklen_t addrlen = sizeof(addr);

                            int s;
                            while ((s = popc_accept(sockfd, &addr, &addrlen)) < 0 && GetLastError() == WSAEINTR)
                                ;

                            if (s < 0) {
                                return nullptr;
                            }

                            for (int j = 0; j < readfds.fd_count; j++)
                                if (readfds.fd_array[j] < s) {
                                    highsockfd = s;
                                }

                            FD_SET(s, &activefdset);

                            connarray.resize(activefdset.fd_count);
                            connarray[activefdset.fd_count - 1] = CreateConnection(s);

                            auto ret = OnNewConnection(connarray[activefdset.fd_count - 1]);
                            if (!ret) {
                                return nullptr;
                            }
                        } else {
                            return connarray[i];
                        }
                    }
                }
            }

            // Poll for ready fds....
            do {
                FD_ZERO(&readfds);
                readfds = activefdset;
                int n = readfds.fd_count;

                timeval tv;
                if (timeout < 0) {
                    tv.tv_sec = 1000000;
                } else {
                    tv.tv_sec = timeout / 1000;
                    tv.tv_usec = timeout % 1000;
                }

                nready = select(highsockfd + 1, &readfds, (fd_set*)0, (fd_set*)0, &tv);
                for (int tmp = 0; tmp < n; tmp++) {
                    if (FD_ISSET(readfds.fd_array[tmp], &readfds)) {
                        tmpfdset = readfds.fd_array[tmp];
                    }
                }
            } while (nready < 0 && errno == EINTR && sockfd >= 0);

            if (nready <= 0) {
                if (!nready) {
                    errno = ETIMEDOUT;
                }
                return nullptr;
            }
        }
    } else {
        if (timeout >= 0) {
            fd_set tempfds;
            FD_ZERO(&tempfds);
            highsockfd = sockfd;
            FD_SET(sockfd, &tempfds);

            timeval tv;
            tv.tv_sec = timeout / 1000;
            tv.tv_usec = timeout % 1000;

            int t;
            while ((t = select(highsockfd + 1, &tempfds, (fd_set*)0, (fd_set*)0, &tv)) == -1 &&
                   GetLastError() == WSAEINTR)
                ;

            if (t <= 0) {
                if (!t) {
                    errno = ETIMEDOUT;
                }
                return nullptr;
            }
        }
        return peer;
    }
}

void pop_combox_socket::Close() {
    int fd = sockfd;
    highsockfd = -1;
    sockfd = -1;
    nready = 0;
    index = -1;

    if (isServer) {
        int n = activefdset.fd_count;
        for (int i = 0; i < n; i++) {
            if (fd != activefdset.fd_array[i]) {
                /*FD_CLR(activefdset.fd_array[i], &activefdset);*/
                OnCloseConnection(connarray[i]);
            }
        }

        FD_ZERO(&activefdset);
        FD_ZERO(&readfds);

        for (int i = 0; i < n; i++) {
            delete connarray[i];
        }

        connarray.RemoveAll();
    } else {
        if (peer) {
            OnCloseConnection(peer);
            delete peer;
            peer = nullptr;
        }

        if (fd >= 0) {
            popc_close(fd);
        }
    }
}

bool pop_combox_socket::CloseSock(int fd) {
    if (isServer) {
        int n = activefdset.fd_count;
        for (int i = 0; i < n; i++) {
            if (activefdset.fd_array[i] == fd) {
                isCanceled = !OnCloseConnection(connarray[i]);
                delete connarray[i];
                connarray.RemoveAt(i);
                FD_CLR(activefdset.fd_array[i], &activefdset);

                popc_close(fd);
                return !isCanceled;
            }
        }
    } else if (peer && fd == sockfd) {
        isCanceled = true;
        popc_close(fd);
        sockfd = -1;
        return true;
    }

    return false;
}

#endif

// Normal implementations that are not separated by arch

pop_connection_sock::pop_connection_sock(pop_combox* cb) : pop_connection(cb) {
    sockfd = -1;
#ifndef __WIN32__
    popc_signal(popc_SIGPIPE, popc_SIG_IGN);
#endif
}

pop_connection_sock::pop_connection_sock(int fd, pop_combox* cb) : pop_connection(cb) {
    sockfd = fd;
}

pop_connection_sock::pop_connection_sock(pop_connection_sock& me)
    : pop_connection(me.GetCombox(), me.GetBufferFactory()) {
    sockfd = me.sockfd;
}

pop_connection* pop_connection_sock::Clone() {
    return new pop_connection_sock(*this);
}

pop_combox_socket::pop_combox_socket() {
    peer = nullptr;
    sockfd = -1;
    index = 0;
    nready = 0;
    isCanceled = false;
    isServer = false;
}

pop_combox_socket::~pop_combox_socket() {
    Close();
}

bool pop_combox_socket::Connect(const char* url) {
    if (url == nullptr) {
#ifdef __WIN32__
        errno = WSAEDESTADDRREQ;
#else
        errno = EDESTADDRREQ;
#endif
        return false;
    }

    while (isspace(*url)) {
        url++;
    }

    char* host;
    if (strncmp(url, "socket://", 9) == 0) {
        host = popc_strdup(url + 9);
    } else {
        host = popc_strdup(url);
    }

    char* s = strchr(host, ':');
    int port;
    if (!s || sscanf(s + 1, "%d", &port) != 1) {
        free(host);

        return false;
    }

    *s = 0;

    auto ret = Connect(host, port);
    free(host);

    if (ret) {
        peer->sockfd = sockfd;
    }

    return ret;
}

int pop_combox_socket::Send(const char* s, int len) {
    int count = 0;

    while (len > 0) {
#ifndef __WIN32__
        int n = write(sockfd, s, len);
#else
        int n = send(sockfd, s, len, 0);
#endif
        if (n > 0) {
            count += n;
            s += n;
            len -= n;
        } else if (errno != EINTR) {
            break;
        }
    }

    return count;
}

int pop_combox_socket::Send(const char* s, int len, pop_connection* conn) {
    if (conn == nullptr) {
        return Send(s, len);
    }

    int fd = ((pop_connection_sock*)conn)->sockfd;

    if (fd < 0) {
        return -1;
    }

    int count = 0;
    while (len > 0) {
#ifndef __WIN32__
        int n = write(fd, s, len);  // vanhieu.nguyen
#else
        int n = send(fd, s, len, 0);
#endif
        if (n > 0) {
            count += n;
            s += n;
            len -= n;
        } else if (errno != EINTR) {
            break;
        }
    }
    return count;
}

int pop_combox_socket::Recv(char* s, int len) {
    int n;
    isCanceled = false;
    do {
        pop_connection_sock* t = (pop_connection_sock*)Wait();
        if (t == nullptr) {
            return -1;
        }

        auto fd = t->sockfd;
#ifndef __WIN32__
        while ((n = read(fd, s, len)) == -1 && errno == EINTR)
            ;
#else
        while ((n = read(fd, s, len)) == -1 && GetLastError() == WSAEINTR)
            ;
#endif
        if (n <= 0) {
            if (CloseSock(fd)) {
                continue;
            }

            return -1;
        }
    } while (n <= 0);

    if (!n) {
        errno = EIO;
    }

    return n;
}

int pop_combox_socket::Recv(char* s, int len, pop_connection* iopeer) {
    int fd, n;
    pop_connection_sock* t;
    isCanceled = false;

    do {
        if (!iopeer) {
            t = (pop_connection_sock*)Wait();
            if (!t) {
                LOG_ERROR("[CORE] Wait failed with code %d", t);
                return -1;
            }

            fd = t->sockfd;
        } else {
            fd = ((pop_connection_sock*)iopeer)->sockfd;
        }

        if (fd < 0) {
            LOG_ERROR("[CORE] Cannot open socket");
            return -1;
        }

#ifndef __WIN32__
        while ((n = read(fd, s, len)) < 0 && errno == EINTR)
            ;
#else
        while ((n = recv(fd, s, len, 0)) < 0 && GetLastError() == WSAEINTR)
            ;
#endif

#ifndef __WIN32__
        if (n == 0) {
            if (CloseSock(fd) && iopeer == nullptr) {
                continue;
            }

            LOG_DEBUG("[CORE] CloseSock failed iopeer=%d", iopeer);
            return -1;
        }
#else
        if (GetLastError() == WSAECONNRESET) {
            if (CloseSock(fd) && iopeer == nullptr) {
                continue;
            }

            LOG_ERROR("[CORE] CloseSock failed");
            return -1;
        }
#endif
    } while (n <= 0);

    if (!iopeer) {
        iopeer = t;
    }

    if (n <= 0) {
        errno = EIO;
    }

    return n;
}

std::string pop_combox_socket::GetProtocol() {
    return "socket";
}

std::string pop_combox_socket::GetUrl() {
    return "socket://" + pop_system::GetHost() + ":" + std::to_string(GetPort());
}

int pop_combox_socket::GetSockInfo(sockaddr& info, socklen_t& len) {
    return popc_getsockname(sockfd, &info, &len);
}

int pop_combox_socket::GetPort() {
    sockaddr_in sin;
    socklen_t len = sizeof(sin);
    GetSockInfo((sockaddr&)sin, len);
    return popc_ntohs(sin.sin_port);
}

int pop_combox_socket::GetOpt(int level, int opt, char* buf, socklen_t& len) {
    return popc_getsockopt(sockfd, level, opt, buf, &len);
}
int pop_combox_socket::SetOpt(int level, int opt, char* buf, socklen_t len) {
    return setsockopt(sockfd, level, opt, buf, len);
}

pop_connection_sock* pop_combox_socket::CreateConnection(int fd) {
    return new pop_connection_sock(fd, this);
}
