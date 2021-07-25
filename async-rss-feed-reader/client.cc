#include "client_interface.h"
#include "commands.h"

class TestClient: public rssreader::networkcore::ClientInterface<RSSCommand>{
      
};

int main(){ 
    TestClient tclient;
    tclient.Connect("127.0.0.1",10000);
    
    if(tclient.IsConnected()){
        rssreader::networkcore::Message<RSSCommand> greeting;
        greeting.m_header.m_id = RSSCommand::Client_Register;
        tclient.Send(greeting);
    }
    
    bool ugly_quit = false;
    while(!ugly_quit){
        if(tclient.IsConnected()){
            if(!tclient.GetQueue().empty()){
                auto msg = tclient.GetQueue().pop_front().m_message;
                switch(msg.m_header.m_id){
                    case RSSCommand::Client_Accepted:{
                        std::cout << "It Worked!!";
                        ugly_quit = true;
                    }
                    default:{
                        std::cout << "TO BE HANDLED, EXITING FOR NOW!\n";
                        ugly_quit = true;
                    }
                }
            }
        }
    }
    return 0;
}