#pragma once
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <iostream>
#include "command.h"

#define READ(function) {\
	socket.async_read_some(boost::asio::buffer(buffer.data(), buffer.size()),\
	boost::bind(function, shared_from_this(),\
	boost::asio::placeholders::error,\
	boost::asio::placeholders::bytes_transferred));\
}

class connection : public boost::enable_shared_from_this<connection>
{
public:
	boost::asio::ip::tcp::socket socket;
	typedef boost::shared_ptr<connection> pointer;

	std::string hostname = "";
	std::vector<char> buffer;

	static std::vector<std::string> client;

	command input;

	connection(boost::asio::io_context& context) : socket{ context }, buffer{ std::vector<char>(1024) }, input{ socket } {}

	~connection()
	{
		if (hostname != "")
		{
			std::cout << "Client " << hostname << " disconnected\n";
			client.erase(std::remove(client.begin(), client.end(), hostname), client.end());
		}
	}

	static pointer create(boost::asio::io_context& context)
	{
		return pointer(new connection(context));
	}

	boost::asio::ip::tcp::socket& return_socket()
	{
		return socket;
	}

	void start()
	{
		READ(&connection::getHostname);
		std::string str = "Enter your username: ";
		socket.write_some(boost::asio::buffer(str.data(), str.size()));
	}

	void getHostname(const boost::system::error_code& ec, std::size_t size)
	{
		if (!ec)
		{
			hostname = std::string(buffer.begin(), buffer.end());
			if (find(client.begin(), client.end(), hostname) == client.end())
			{
				client.push_back(hostname);
				std::cout << hostname << " connected\n";
				READ(&connection::handle_read);
				socket.write_some(boost::asio::buffer(std::string("Successfully connected").data(), std::string("Successfully connected").size()));
			}
			else
			{
				std::cout << "Failed attempt to connect\n";
				socket.write_some(boost::asio::buffer(std::string("Failed attempt to connect").data(), std::string("Failed attempt to connect").size()));
				hostname = "";
			}
		}
	}

	void handle_read(const boost::system::error_code& ec, std::size_t size)
	{
		if (!ec)
		{
			std::string message = "";
			for (int i = 0; i < size; i++)	message += buffer[i];
			std::cout << hostname << ": " << message << "\n";
			READ(&connection::handle_read);
			input.execute(message);
		}
	}
};
