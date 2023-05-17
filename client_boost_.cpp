#include <iostream>
#include <boost/asio.hpp>
#include <fstream>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

void Read(string& str)
{
    std::ifstream in("in.txt");
    getline(in, str);
    in.close();
}

int main() {
    setlocale(LC_ALL, "ru");
    boost::asio::io_service io_service;
    
    tcp::socket socket(io_service);
    
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1234));
    
    std::string msg;
    Read(msg);
    
    
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(msg), error);
    if (error) {
        std::cout << "send failed: " << error.message() << endl;
    }
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof) {
        cout << "receive failed: " << error.message() << endl;
    }
    else {
        std::string data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        std::ofstream off("out.txt");
        off << msg << " = " << data;
        off.close();
    }
    return 0;
}