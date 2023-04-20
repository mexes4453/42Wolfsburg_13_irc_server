#ifndef USER_HPP
# define USER_HPP

# include "Server.hpp"
# include "Channel.hpp"
# include <string.h>
# include <sstream>
# include <vector>
# include <poll.h>

class Server;

class User
{
	private:
		Server 						*_server;
		int							_userFd;
		std::string					_ip;
		std::string					_pw;
		std::string					_userName;
		std::string					_nickName;
		std::string					_realName;
		bool						_isRegistered;
		bool						_usernameSet;
		std::list<Channel *>		_channelList;

	public:
		User(int fd, std::string ip, Server *ircserver);
		User&		operator=(User &src);
		~User();

		int			getFd(void);
		std::string	getIP(void);
		void		setPw(const std::vector<std::string>& args);
		void		setNickName(const std::vector<std::string>& args);
		std::string	getNickName(void);
		void		setUserName(std::vector<std::string>& args);
		std::string	getUserName(void);
		void		setRealName(const std::vector<std::string>& args);
		std::string	getRealName(void);

		int			sendMsgToOwnClient(std::string msg);
		int			sendMsgToTargetClient(std::string msg, int targetUserFd);
		void		executeCommand(std::string command, std::vector<std::string>& args);


		// void		changeTopic(channel& currentChannel, std::string newTopic);
		// channel&	createChannel(std::string channelName);
		// void 		inviteUser(channel& currentChannel, std::string nickName);
		void		joinChannel(std::vector<std::string>& args);
		// void		kickUser(std::vector<std::string>& args);
		// void		leaveChannel(std::vector<std::string>& args);
		// void		modifyChannel(std::string channelName, std::string nickName, char mode);


		void		sendNotification(std::vector<std::string>& args);	
		int			sendMsg(std::vector<std::string>& args);
		int			sendPrivateMsg(std::vector<std::string>& args);


		// bool		isOperator(channel& channel);

		std::string	argsToString(std::vector<std::string>::iterator iterBegin, std::vector<std::string>::iterator iterEnd);

		//Exceptions
		class badChannelMask : public std::exception {
			public:
				virtual const char *what() const throw() {
					return ("Bad Channel Mask");
				}
		};

		class notAnOperator : public std::exception {
			public:
				virtual const char *what() const throw() {
					return (":You're not channel operator");
				}
		};

		class notOnTheChannel : public std::exception {
			public:
				virtual const char *what() const throw() {
					return (":Is not on channel");
				}
		};

		class noSuchChannel : public std::exception {
			public:
				virtual const char *what() const throw()
				{
					return (":No such channel");
				}
		};

		class channelNotFoundException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Channel doesn't exist.");
				}
		};

		class SendToTargetCLientException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Error while sending message to target client.");
				}
		};

		class SendToOwnCLientException : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Error while sending message to own client.");
				}
		};

		class cannotJoinChannelPW : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Cannot join channel (+k)");
				}
		};

		class cannotJoinChannelIn : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Cannot join channel (+i)");
				}
		};

		class channelCapacity : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Cannot join channel (+l)");
				}
		};
};

#endif