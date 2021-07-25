#include "commands.h"
#include "server_interface.h"

class TestServer : public rssreader::networkcore::ServerInterface<RSSCommand> {
public:
  TestServer(uint16_t port)
      : rssreader::networkcore::ServerInterface<RSSCommand>(port) {}

protected:
  bool OnClientConnect(
      std::shared_ptr<rssreader::networkcore::Connection<RSSCommand>> client)
      override {
    return true; // Some Auth should be integrated here!
  }

  void OnClientValidated(
      std::shared_ptr<rssreader::networkcore::Connection<RSSCommand>> client)
      override {
    rssreader::networkcore::Message<RSSCommand> msg;
    msg.m_header.m_id = RSSCommand::Client_Accepted;
    client->Send(msg);
  }

  void OnClientDisconnect(
      std::shared_ptr<rssreader::networkcore::Connection<RSSCommand>> client)
      override {
    if (client) {
      std::cout << "[SERVER] Client Removed : " +
                       std::to_string(client->GetID()) + "\n";
    }
  }

  void OnMessage(
      std::shared_ptr<rssreader::networkcore::Connection<RSSCommand>> client,
      rssreader::networkcore::Message<RSSCommand>& msg) override {
          switch (msg.m_header.m_id){
              case RSSCommand::Client_Register:{
                  rssreader::networkcore::Message<RSSCommand> response;
                  response.m_header.m_id = RSSCommand::Client_Accepted;
                  MessageClient(client,response);
                  break;
              }
              default: {
                  std::cout << "WORK IN PROGRESS! :(";
              }
          }
      }
};

int main() {
  // uint16_t port;
  // std::cin >> port;
  TestServer tserver(10000);
  tserver.Start();
  while(1){
      tserver.Update(-1,true);
  }
  return 0;
}