#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>

#define COM(name) {#name, &command::##name}

class command
{
public:
	boost::asio::ip::tcp::socket& socket;

	command(boost::asio::ip::tcp::socket& sock) : socket{sock}
	{

	}

	void quit()
	{
		std::string mes = "hola";
		std::cout << mes;
		socket.write_some(boost::asio::buffer(mes.data(), mes.size()));
	}

	void ping()
	{

	}

	void getFile()
	{

	}

	void help()
	{

	}

	~command()
	{

	}

	struct
	{
		const char* name;
		void (command::* function) (void);
	} 
	commands[4] = 
	{
		{"quit",&command::quit},
		{"ping",&command::ping},
		{"getFile",&command::getFile},
		{"help",&command::help}
	};

	void execute(std::string message)
	{
		//socket.write_some(boost::asio::buffer(hostname.data(), hostname.size()));
		for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++)
		{
			if (commands[i].name == message)
			{
				(this->*commands[i].function)();
				break;
			}
		}
	}
};