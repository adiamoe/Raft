#include <iostream>
#include <boost/asio.hpp>
#include "jsonxx/json.hpp"
#include "util/Server.h"
#include "util/Client.h"
#include <string>
#include <memory>

using namespace std;
using tcp = boost::asio::ip::tcp;

class Echo{
public:
    explicit Echo(boost::asio::io_context& io) : server_(io, 8080) {
        server_.SetConnection([](shared_ptr<grape::Session> con) {
            std::cout << "new connection : " << std::endl;
        });

        server_.SetMessage([](std::shared_ptr<grape::Session> con) {
            string message = string(con->GetData().data());
            con->DoWrite(message);
        });

        server_.SetClose([](std::shared_ptr<grape::Session> con) {
            con->Shutdown();
        });
    }

private:
    grape::Server server_;
};

int main()
{
    int port = 8080;
    boost::asio::io_context ioc;
    Echo echo(ioc);
    grape::Client client(ioc, "localhost", "8080");

    ioc.run();
    return 0;
}
