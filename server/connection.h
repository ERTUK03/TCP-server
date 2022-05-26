#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class connection : public boost::enable_shared_from_this<connection>
{
public:
	typedef boost::shared_ptr<connection> pointer;
	boost::asio::ip::tcp::socket socket;

	connection(boost::asio::io_context& context) : socket{ context } {}

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
		boost::asio::async_write(socket, boost::asio::buffer("tak"),
								boost::bind(&connection::handle_write, shared_from_this(),
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred));
	}

	void handle_write(const boost::system::error_code&,
		size_t)
	{
	}
};

