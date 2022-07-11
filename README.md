# TCP server
Just a basic tcp server with couple commands, made with boost::asio.


# Requirements
-C++17

-Boost

# Description
It's made to work with a client I made https://github.com/ERTUK03/client. Although it's possible to interact with it using just telnet
I don't recommend that because file transfer won't work. To share files you need to create a directory named "shared" in your project, and add your files to it.

To start the server you need to specify ip address and port you will be using. After that the server is running a awaiting clients. For logical reasons server doesn't
accept another client if specified hostname is already in use.

# Commands supported
1. getUsers - Shows client a list of server clients

2. getFile - Sends client the file specified in command. SYNTAX getFile [filename.extension]

3. getFiles - Shows client a list of files in shared folder

4. quit - Disconnects a client

5. help - Shows client a list of commands

If you have any idea for new commands please let me know :)
