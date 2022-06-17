#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include "tcp_server.h"

std::string getAddress()
{
	std::string address;
	std::cout << "Enter your ip address: ";
	std::getline(std::cin, address);
	return address;
}

int getPort()
{
	std::string port;
	int res;
	std::cout << "Enter your port: ";
	std::getline(std::cin, port);
	return stoi(port);
}

int main()
{
	boost::asio::io_context context;
	std::string address = getAddress();
	int port = getPort();
	std::cout << "Your address: " << address << "\n";
	std::cout << "Port: " << port << "\n";
	tcp_server server(address, port, context);
	context.run();
}