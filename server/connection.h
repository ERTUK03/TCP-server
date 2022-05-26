#pragma once
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <iostream>

class connection : public boost::enable_shared_from_this<connection>
{
public:
	std::vector<char> buffer;
	typedef boost::shared_ptr<connection> pointer;
	boost::asio::ip::tcp::socket socket;

	std::string str;

	connection(boost::asio::io_context& context) : socket{ context }, buffer{ std::vector<char>(1024) } {}

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
		socket.async_read_some(boost::asio::buffer(buffer, 1024),
			boost::bind(&connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		socket.async_write_some(boost::asio::buffer("Welcome\n"),
								boost::bind(&connection::handle_write, shared_from_this(),
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred));
	}

	void handle_write(const boost::system::error_code, std::size_t)
	{
		std::cin >> str;
		socket.async_write_some(boost::asio::buffer(str),
			boost::bind(&connection::handle_write, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(boost::system::error_code, std::size_t)
	{
		for (auto ch : buffer)	std::cout << ch;
		socket.async_read_some(boost::asio::buffer(buffer, 1024),
							   boost::bind(&connection::handle_read, shared_from_this(),
							   boost::asio::placeholders::error,
							   boost::asio::placeholders::bytes_transferred));
	}
};

