#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

using std::cout;
using std::endl;

boost::asio::io_service io;
boost::asio::ip::tcp::socket s(io);

void send() {
    const std::string msg = "Hello from Client!";// \n";
    boost::system::error_code error;
    boost::asio::write(s, boost::asio::buffer(msg + "\n"), error);
    if (!error) {
        cout << "Message to server: " << msg << endl;
    }
    else {
        cout << "send failed: " << error.message() << endl;
    }
}

void receive() {
    boost::asio::streambuf receive_buffer;
    boost::system::error_code error;
    boost::asio::read(s, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof) {
        cout << "receive failed: " << error.message() << endl;
    }
    else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
    }
}

int main() {
    bool connected = false;
    while (!connected) {
        try {
            s.connect(ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));
        }
        catch (...) {
            continue;
        }
        connected = true;
    }
    
    
    while (1)
        send();
    receive();
    return 0;
}
