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
	std::string address;
	int port;
	try
	{
		address = getAddress();
		port = getPort();
	}
	catch (...)
	{
		std::cout << "Wrong input.";
		return 0;
	}
	std::cout << "Your address: " << address << "\n";
	std::cout << "Port: " << port << "\n";
	try
	{
		tcp_server server(address, port, context);
	}
	catch (...)
	{
		std::cout << "Wrong address";
		return 0;
	}
	context.run();
}