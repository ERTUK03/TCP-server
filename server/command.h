#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <fstream>
#include <sstream>

#define COMMAND(name) {#name, &command::##name}
#define SEND(message) socket.write_some(boost::asio::buffer(message.data(), message.size()))

class command
{
public:
	std::vector<std::string> words;

	boost::asio::ip::tcp::socket& socket;

	command(boost::asio::ip::tcp::socket& sock) : socket{sock} {}

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
		if (words.size() == 2)
		{
			std::string message = "";
			std::string line;
			std::ifstream file("shared/" + words[1], std::ios::in | std::ios::binary);
			if (!file)
			{
				SEND(std::string("An error occured while opening the file"));
			}
			else
			{
				while (file >> line)
				{
					message += line;
				}
				SEND(message);
			}
		}
		else
		{
			SEND(std::string("Wrong syntax"));
		}
	}

	void getFileHandle(std::string filename)
	{
		std::string message = "";
		std::string line;
		std::ifstream file("shared/" + filename, std::ios::in | std::ios::binary);
		if (!file)
		{
			SEND(std::string("An error occured while opening the file"));
		}
		else
		{
			while (file >> line)
			{
				message += line;
			}
			SEND(message);
		}
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
			sendMessage.pop_back();
			SEND(sendMessage);
		}
		else
		{
			SEND(std::string("Wrong syntax"));
		}
	}

	struct
	{
		std::string name;
		void (command::* function) (void);
	}

	commands[3] =
	{
		COMMAND(quit),
		COMMAND(getFile),
		COMMAND(help)
	};

	void execute(std::string receiveMessage)
	{
		std::string word;
		words.clear();
		std::stringstream ss(receiveMessage);
		while (ss >> word)	words.push_back(word);
		for (int i = 0; i < sizeof(commands)/sizeof(commands[0]); i++)
		{
			if (commands[i].name == words[0])
			{
				(this->*commands[i].function)();
				return;
			}
		}
		SEND(std::string("Unknown command. Try again"));
	}
};