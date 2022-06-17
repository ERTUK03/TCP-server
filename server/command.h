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

	command(boost::asio::ip::tcp::socket& sock, std::vector<std::string>& client) : socket{ sock }, clients{ client } {}

	~command() {}

	void quit()
	{
		if (words.size() == 1)
		{
			SEND(std::string("Disconnected"));
			socket.close();
		}
		else
		{
			SEND(std::string("Wrong syntax"));
		}
	}

	void getFile()
	{
		if (words.size() != 2)
		{
			SEND(std::string("Wrong syntax"));
			return;
		}

		std::ifstream file("shared/" + words[1], std::ios::in | std::ios::binary);
		if (!file)	SEND(std::string("An error occured while opening the file"));
		else
		{
			char* buffer = new char[(4*1024)-1];
			int length;
			for (;;) 
			{
				file.read(buffer, (16*1024)-1);
				length = file.gcount();
				std::string packet;
				if (file.eof()) packet = "Y";
				else packet = "N";
				for (int i = 0; i < length; i++)
				{
					packet += buffer[i];
				}
				SEND(packet);
				if (file.eof()) break;
			}
			delete[] buffer;
		}
		file.close();
	}

	void help()
	{
		if (words.size() == 1)
		{
			std::string sendMessage = "";
			for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
			{
				sendMessage += commands[i].name + '\n';
			}
			SEND(sendMessage);
		}
		else
		{
			SEND(std::string("Wrong syntax\n"));
		}
	}

	void getUsers()
	{
		if (words.size() == 1)
		{
			std::string sendMessage = "";
			for (int i = 0;i<clients.size(); i++)
			{
				sendMessage += clients[i] + '\n';
			}
			SEND(sendMessage);
		}
		else
		{
			SEND(std::string("Wrong syntax\n"));
		}

	}

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

	void execute(std::string receiveMessage)
	{
		std::string word;
		words.clear();
		std::stringstream ss(receiveMessage);
		while (ss >> word)	words.push_back(word);
		if (words.size() == 0)
		{
			SEND(std::string("Unknown command. Try again\n"));
			return;
		}
		for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++)
		{
			if (commands[i].name == words[0])
			{
				(this->*commands[i].function)();
				return;
			}
		}
		SEND(std::string("Unknown command. Try again\n"));
	}
};