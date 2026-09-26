#ifndef CENTRAL_SERVER_HPP
#define CENTRAL_SERVER_HPP

class CentralServer {
private:
    int m_serverFd = -1;
    int m_port;

    static void handleClient(int clientSocket);

public:
    CentralServer(int port);
    ~CentralServer();

    bool init();
    void run();
};

#endif
