#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

class client_manager {
public:
//    client_manager(tcp::socket& _socket);
    client_manager(boost::asio::io_service&);
    client_manager(client_manager&& cm) : socket_(std::move(cm.socket_)) {}
    ~client_manager();
    string read_(tcp::socket &socket);
    void send_(tcp::socket &socket, const string &&message);
    void echo_loop();
    tcp::socket& get_socket() {return socket_;};
private:
    tcp::socket socket_;
    //client_manager();
    //client_manager(const client_manager&);
};

client_manager::client_manager(boost::asio::io_service &context)
    :socket_(context)
{
       //echo_loop();
}

client_manager::~client_manager() {
    std::cout << "Exiting ..." << std::endl;
}

void client_manager::echo_loop() {
    string message="";
    std::cout << "Waiting ..." << std::endl;

    while(message.compare("QUIT\r\n") !=0) {

        message = read_(socket_);
        std::cout << "Server received \'" << message <<"\'" << std::endl;
        send_(socket_, "-> " + message);
    }
}
string client_manager::read_(tcp::socket &socket) {
    boost::asio::streambuf buf;
    try{
    boost::asio::read_until( socket, buf, "\n" );
    } catch (std::exception& e) {
        std::cerr << "ERROR OCCURRED: "<<e.what();
    }
    string data = boost::asio::buffer_cast<const char*>(buf.data());
    return data;
}
void client_manager::send_(tcp::socket & socket, const string &&message) {
    const string msg = message + "\r\n";
    boost::asio::write( socket, boost::asio::buffer(message) );
    cout << "Servent sent back \'"<< message <<"\' to Client!" << endl;
}

int main() {
    boost::asio::io_service io_service;

    std::vector<client_manager> clients;
    //listen for new connection
    tcp::acceptor acceptor_(io_service, tcp::endpoint(tcp::v4(), 1234 ));

    //waiting for connection
    while((clients.size() < 3)) {
        //socket creation
        //tcp::socket socket_(io_service);
        {
            client_manager cm(io_service);
            acceptor_.async_accept(cm.get_socket(), std::bind(&client_manager::echo_loop, &cm));
            clients.push_back(std::move(cm));
        }
    }
    io_service.run();
    //cm.send_(socket_, "Hello From Server!");
    //read operation
    //string message = cm.read_(socket_);
    //cout << message << endl;
    //write operation


    return 0;
}
