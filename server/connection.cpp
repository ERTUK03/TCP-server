#include "connection.h"

std::vector<std::string> connection::client;

connection::connection(boost::asio::io_context& context) : socket{ context }, buffer{ std::vector<char>(1024) }, input{ socket, client } {}

connection::~connection()
{
	if (hostname != "")
	{
		std::cout << "Client " << hostname << " disconnected\n";
		client.erase(std::remove(client.begin(), client.end(), hostname), client.end());
	}
}

boost::asio::ip::tcp::socket& connection::return_socket() { return socket; }

void connection::getHostname(const boost::system::error_code& ec, std::size_t size)
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

void connection::start()
{
	READ(&connection::getHostname);
	std::string str = "Enter your username: ";
	SEND(str);
}

inline void connection::handle_read(const boost::system::error_code& ec, std::size_t size)
{
	if (ec) return;
	std::string message = "";
	for (int i = 0; i < size; i++)	message += buffer[i];
	std::cout << hostname << ": " << message << "\n";
	READ(&connection::handle_read);
	input.execute(message);
}