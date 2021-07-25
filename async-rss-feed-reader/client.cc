#include "client_interface.h"
#include "commands.h"

class TestClient: public rssreader::networkcore::ClientInterface<RSSCommand>{
    public:
        TestClient();
};

int main(){ 
    
    return 0;
}