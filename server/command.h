#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>

#define COMMAND(name) {#name, &command::##name}
#define SEND(message) socket.write_some(boost::asio::buffer(message.data(), message.size()))

class command
{
public:
	std::vector<std::string> words;
	std::vector<std::string>& clients;
	boost::asio::ip::tcp::socket& socket;

	command(boost::asio::ip::tcp::socket& sock, std::vector<std::string>& client);

	void quit();
	void getFile();
	void help();
	void getUsers();

	struct
	{
		std::string name;
		void (command::* function) (void);
	}

	commands[4] =
	{
		COMMAND(getFile),
		COMMAND(getUsers),
		COMMAND(help),
		COMMAND(quit)
	};

	void execute(std::string receiveMessage);
};