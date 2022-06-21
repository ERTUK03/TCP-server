#pragma once
#include <boost/asio.hpp>
#include "connection.h"

class tcp_server
{
public:
	boost::asio::io_context& context;
	boost::asio::ip::tcp::acceptor acceptor;

	tcp_server(std::string address, int port, boost::asio::io_context& con);

private:
	void start_accept();
	void handle_accept(connection::pointer connection, const boost::system::error_code& ec);
};