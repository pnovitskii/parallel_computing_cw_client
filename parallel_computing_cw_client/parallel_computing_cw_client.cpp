#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;

boost::asio::io_service io;
boost::asio::ip::tcp::socket s(io);

void send(const std::string& msg) {
    //const std::string msg = "Hello from Client!";// \n";
    boost::system::error_code error;
    boost::asio::write(s, boost::asio::buffer(msg + "\n"), error);
    if (!error) {
        std::cout << "Message to server: " << msg << std::endl;
    }
    else {
        std::cout << "send failed: " << error.message() << std::endl;
    }
}

std::string receive() {
    boost::asio::streambuf receive_buffer;
    boost::system::error_code error;
    boost::asio::read(s, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof) {
        std::cout << "receive failed: " << error.message() << std::endl;
        return {};
    }
    
    auto data = std::string(boost::asio::buffers_begin(receive_buffer.data()), boost::asio::buffers_end(receive_buffer.data()));
    //std::cout << data << std::endl;
    return data;
}

int main() {
    bool connected = false;
    std::cout << "Connecting to server";
    while (!connected) {
        try {
            s.connect(ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));
        }
        catch (...) {
            std::cout << ".";
            continue;
        }
        std::cout << "connected!\n";
        connected = true;
    }
    
    
    while (1) {
        std::string msg;
        std::cout << "Enter command: ";
        std::getline(std::cin, msg);
        send(msg);
        msg = receive();
        std::cout << "\nGot: " << msg << std::endl;
    }
        
    
    return 0;
}
