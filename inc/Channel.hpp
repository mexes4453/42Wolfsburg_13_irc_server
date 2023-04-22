/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsemke <fsemke@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 10:03:27 by cudoh             #+#    #+#             */
/*   Updated: 2023/04/22 19:21:46 by fsemke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <cstring>
# include <list>
# include <exception>
# include <unistd.h>
# include "User.hpp"

# define CHN_MAX_USERS (1024)
# define CHN_MIN_USERS (3)
# define COUT std::cout
# define ENDL std::endl
# define CHN_TRUE (1)
# define CHN_FALSE (0)
# define CHN_DEFAULT_NAME ("Unknown")
# define CHN_DEFAULT_TOPIC ("Undefined")
# define CHN_EXCEPTION_HANDLER() catch(const std::exception &e) \
{\
	std::cerr << e.what() << '\n'; \
}

typedef enum e_chn_action
{
	USR_ADD = 0,
	USR_REMOVE,
	/*Max*/
	MAX_ACTION
}	t_chn_action;

typedef enum e_chn_return
{
	CHN_ERR_FAILED = -127,
	CHN_ERR_InvalidNbrOfUsers,
	CHN_ERR_UserDoesNotExist,
	CHN_ERR_SUCCESS = 0,
}	t_chn_return;

class User;

class Channel
{
    private:
        std::string			_channelName;
        std::string			_topic;
        unsigned int		_channelCapacity;
        std::list<User *>	*_invitedUsers;
        std::list<User *>	*_operators;
        std::list<User *>	*_ordinaryUsers;			// without operators

    public:
    	Channel(std::string name, std::string topic, User *user);	// Parametric constructor
    	~Channel(void);									// Destructor
    
    	/* Getters and Setters */
    	std::string			getChannelName(void) const;
    	std::string			getTopic(void) const;
    	unsigned int		getChannelCapacity(void) const;
    	std::list<User *>	*getListPtrInvitedUsers(void) const;
    	std::list<User *>	*getListPtrOperators(void) const;
    	std::list<User *>	*getListPtrOrdinaryUsers(void) const;
    	void				setChannelName(std::string name);
    	t_chn_return		setChannelCapacity(unsigned int);
    	void				setTopic(std::string topic);

    	/* Methods */
		
		/**
		 * @brief 
		 * This method sends messages passed to users in the a channel by
		 * iterating over all user pointers in the _allUser list using
		 * iterators which deference each user pointer and access their file
		 * descriptor to send message to them.
		 *
		 * @param msg std::string
		 */
    	// void	broadcastMsg(std::string msg_org);
		void 	broadcastMsg(std::string msg_org, std::pair<bool, User*> ownUser);



		/**
		 * @brief 
		 * This method checks if a list of users is empty.
		 * It returns true if empty.
		 * It returns false if not empty
		 * 
		 * @param list_users 
		 * @return true 
		 * @return false 
		 */
    	bool	isUserListEmpty(std::list<User *> *list_users);

		
		/**
		 * @brief 
		 * This method checks if a user already exist in a list of users
		 * It returns true if user exist in list.
		 * It returns false if user is not found in list.
		 * @param list_users 
		 * @param user 
		 * @return true 
		 * @return false 
		 */
		bool	isUserInList(std::list<User *> *list_users, User *user);


		/**
		 * ! This method call must be wrapped with try/catch
		 * @brief 
		 * This method adds a user to a list of users.
		 * It checks if a user is already in the list before adding user.
		 * An exception is thrown if user already exist in list of user.
		 * Hence, a user cannot be added twice in a list.
		 * @param list_users 
		 * @param user 
		 */
		void	addUserToList(std::list<User *> *list_users, User *user);

		
		/**
		 * ! This method call must be wrapped with try/catch
		 * @brief 
		 * This method removes a user from a list of users.
		 * It checks if a user is already in the list before removing user.
		 * An exception is thrown if user is not found in list of user.
		 * Hence, a user non-existent in list of user cannot be removed.
		 * @param list_users 
		 * @param user 
		 */
		void	removeUserFromList(std::list<User *> *list_users, User *user);


		/**
		 * @brief 
		 * This method updates the list of users which includes adding and 
		 * removing users from it.
		 * The action arg passed in specifies which update to perform.
		 * @param list_users 
		 * @param user 
		 * @param action 
		 */
    	void	updateUserList(std::list<User *> *list_users, User *user, t_chn_action action);


		/**
		 * @brief 
		 * This method removes all the users stored in a list if the list
		 * is not empty.
		 * 
		 * @param list_users 
		 * @return int 
		 */
		int		emptyUserList(std::list<User *> *list_users);


		/**
		 * @brief 
		 * This methods deallocates the memory allocated for the user list
		 * during the constructor call.
		 * 
		 * @param list_users 
		 */
		void	deallocPtrs(std::list<User *> *list_users);


		/**
		 * @brief 
		 * This methods returns the user pointer after searching a list of
		 * user with the given nickname.
		 * 
		 * @param nickname 
		 * @return User* 
		 */
		User	*fetchUserPtrFromList(std::list<User *> *user_list, \
								      std::string nickname);


		/**
		 * @brief 
		 * This methods checks if a user is in channel. The user is searched
		 * using an iterator to iterate over all users in the channel and
		 * checking for a matched with the nickname passed as argument.
		 * 
		 * @param nickname 
		 * @return User* 
		 */
		User	*isUserInChannel(std::string nickname);


		/**
		 * @brief 
		 * This method demotes a user from operator to ordinary user.
		 * First the user pointer is retrieved from operator list using 
		 * the nickname.
		 * If found, the user is removed from operator list and added to
		 * list of ordinary users.
		 * It returns 0, if successful
		 * It returns -1, on error
		 * ? Note that an exeception would be raised if user is already
		 * ? in the list during add operation
		 * 
		 * @param nickname 
		 * @return t_chn_return 
		 */
		t_chn_return	demoteUser(std::string nickname);
		 

		
		/**
		 * @brief 
		 * This method promotes a user from ordinary user to operator.
		 * First the user pointer is retrieved from operator list using 
		 * the nickname.
		 * If found, the user is removed from operator list and added to
		 * list of ordinary users.
		 * It returns 0, if successful
		 * It returns -1, on error
		 * ? Note that an exeception would be raised if user is already
		 * ? in the list during add operation
		 * 
		 * @param nickname 
		 * @return t_chn_return 
		 */
		t_chn_return	promoteUser(std::string nickname);
		


		/* Nested class for execeptions */
		 class NullPointerException : public std::exception
        {
            virtual char const *what() const throw()
            {
                return ("Error: null pointer\n");
            }
        };

		class InvalidActionException : public std::exception
		{
			virtual char const *what() const throw()
			{
				return ("Error! Invalid channel action"); 
			}
		};

		class UsrExistException : public std::exception
		{
			virtual char const *what() const throw()
			{
				return ("\nError! User already exist in channel list\n");
			}
		};

		class UsrNotFoundException : public std::exception
		{
			virtual char const *what() const throw()
			{
				return ("\nError! User is not found in channel list\n");
			}
		};

		class EmptyContentException : public std::exception
		{
			virtual char const *what() const throw()
			{
				return ("\nError! String is empty\n");
			}
		};
		
		class InvalidNbrOfUsersException : public std::exception
		{
			virtual char const *what() const throw()
			{
				return ("\nError! Invalid number of user specified.\n");
			}
		};
};


#endif // CHANNEL_HPP
