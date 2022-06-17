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

	connection(boost::asio::io_context& context);
	~connection();

	static pointer create(boost::asio::io_context& context) { return pointer(new connection(context)); }
	boost::asio::ip::tcp::socket& return_socket();
	void getHostname(const boost::system::error_code& ec, std::size_t size);
	void start();
	inline void handle_read(const boost::system::error_code& ec, std::size_t size);
};
