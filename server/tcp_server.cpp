#include "tcp_server.h"

tcp_server::tcp_server(std::string address, int port, boost::asio::io_context& con) : context{ con }, acceptor{ con, boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(address), port) }
{
	start_accept();
}

void tcp_server::start_accept()
{
	connection::pointer connection = connection::create(context);
	acceptor.async_accept(connection->return_socket(), boost::bind(&tcp_server::handle_accept, this, connection, boost::asio::placeholders::error));
}

void tcp_server::handle_accept(connection::pointer connection, const boost::system::error_code& ec)
{
	if (!ec)
	{
		connection->start();
	}
	start_accept();
}