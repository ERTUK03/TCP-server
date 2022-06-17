#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <string>
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

	connection(boost::asio::io_context& context) : socket{ context }, buffer{ std::vector<char>(1024) }, input{ socket, client } {}

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

	void getHostname(const boost::system::error_code& ec, std::size_t size)
	{
		if (ec) return;
		hostname = std::string(buffer.begin(), buffer.end());
		std::string response;
		if (find(client.begin(), client.end(), hostname) != client.end())
		{
			response = "Failed attempt to connect";
			std::cout << response << "\n";
			SEND(response);
			hostname = "";
		}
		else
		{
			response = "Successfully connected";
			client.push_back(hostname);
			std::cout << hostname << " connected\n";
			READ(&connection::handle_read);
			SEND(response);
		}
	}

	void start()
	{
		READ(&connection::getHostname);
		std::string str = "Enter your username: ";
		SEND(str);
	}

	inline void handle_read(const boost::system::error_code& ec, std::size_t size)
	{
		if (ec) return;
		std::string message = "";
		for (int i = 0; i < size; i++)	message += buffer[i];
		std::cout << hostname << ": " << message << "\n";
		READ(&connection::handle_read);
		input.execute(message);
	}
};
