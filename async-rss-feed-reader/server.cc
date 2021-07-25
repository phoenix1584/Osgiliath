#include "server_interface.h"
#include "commands.h"

class TestServer : public rssreader::networkcore::ServerInterface<RSSCommand>{
    public:
        TestServer(uint16_t port) : rssreader::networkcore::ServerInterface<RSSCommand>(port){}
};

int main(){ 
    // uint16_t port;
    // std::cin >> port;
    TestServer tserver(10000);
    tserver.Start();
    return 0;
}