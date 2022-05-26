#include "tcp_server.h"

int main()
{
	boost::asio::io_context context;
	tcp_server server("192.168.1.104", 1010, context);
	context.run();
}