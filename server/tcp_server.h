#pragma once
#include <iostream>
#include "connection.h"

class tcp_server
{
public:
	tcp_server(std::string address, int port, boost::asio::io_context &con) : context{con}, acceptor {con, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(address), port)}
	{
		start_accept();
	}

	void start_accept()
	{
		connection::pointer connection = connection::create(context);
		acceptor.async_accept(connection->return_socket(), boost::bind(&tcp_server::handle_accept, this, connection, boost::asio::placeholders::error));
	}

	void handle_accept(connection::pointer connection, const boost::system::error_code &ec)
	{
		if (!ec)
		{
			std::cout << "hejka";
			connection->start();
		}
		start_accept();
	}

	boost::asio::io_context &context;
	boost::asio::ip::tcp::acceptor acceptor;
};

