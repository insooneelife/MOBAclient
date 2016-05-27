#include <deque>
#include <iostream>
#include <sstream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>

#include "GameClient.h"
#include "GameMessage.hpp"

#include "../Packet/PacketFactory.h"

GameClient::GameClient(
	boost::asio::io_service& io_service,
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	:
	_io_service(io_service),
	_socket(io_service),
	_id(0),
	_level(1),
	_gold(0),
	_character_name("Andariel"),
	_client_name(""),
	_team(1)
{
	boost::asio::async_connect(
		_socket,
		endpoint_iterator,
		boost::bind(
			&GameClient::handleConnect,
			this,
			boost::asio::placeholders::error));
}

GameClient::~GameClient()
{
	//doClose();
}

void GameClient::send(const GameMessage& msg)
{
	_io_service.post(boost::bind(&GameClient::doSend, this, msg));
}

void GameClient::close()
{
	_io_service.post(boost::bind(&GameClient::doClose, this));
}

void GameClient::handleConnect(const boost::system::error_code& error)
{
	if (!error)
	{
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getData(),
				GameMessage::HEADER_LENGTH),
			boost::bind(
				&GameClient::handleReadHeader,
				this,
				boost::asio::placeholders::error));
	}
}

void GameClient::handleReadHeader(const boost::system::error_code& error)
{
	if (!error && _read_msg.decodeHeader())
	{
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getBody(),
				_read_msg.getBodyLength()),
			boost::bind(
				&GameClient::handleReadBody,
				this,
				boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "doClose()" << std::endl;
		doClose();
	}
}

void GameClient::handleReadBody(const boost::system::error_code& error)
{
	if (!error)
	{
		PacketFactory::getInstance().getQ().push(_read_msg);

		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.getData(),
				GameMessage::HEADER_LENGTH),
			boost::bind(
				&GameClient::handleReadHeader,
				this,
				boost::asio::placeholders::error));
	}
	else
	{
		std::cout << "doClose()" << std::endl;
		doClose();
	}
}

void GameClient::doSend(GameMessage msg)
{
	bool write_in_progress = !_send_msgs.empty();
	_send_msgs.push_back(msg);
	if (!write_in_progress)
	{
		boost::asio::async_write(
			_socket,
			boost::asio::buffer(
				_send_msgs.front().getData(),
				_send_msgs.front().getLength()),
			boost::bind(
				&GameClient::handleSend,
				this,
				boost::asio::placeholders::error));
	}
}

void GameClient::handleSend(const boost::system::error_code& error)
{
	if (!error)
	{
		_send_msgs.pop_front();
		if (!_send_msgs.empty())
		{
			boost::asio::async_write(
			_socket,
				boost::asio::buffer(
					_send_msgs.front().getData(),
					_send_msgs.front().getLength()),
				boost::bind(&GameClient::handleSend, this,
					boost::asio::placeholders::error));
		}
	}
	else
	{
		doClose();
	}
}

void GameClient::doClose()
{
	_socket.close();
}



unsigned int GameClient::getId() const
{
	return _id;
}

int GameClient::getLevel() const
{
	return _level;
}

int GameClient::getGold() const
{
	return _gold;
}

int GameClient::getTeam() const
{
	return _team;
}


const std::string& GameClient::getCharacterName() const
{
	return _character_name;
}

const std::string& GameClient::getClientName() const
{
	return _client_name;
}


void GameClient::setId(unsigned int id)
{
	_id = id;
}

void GameClient::setLevel(int level)
{
	_level = level;
}

void GameClient::setGold(int gold)
{
	_gold = gold;
}

void GameClient::setTeam(int team)
{
	_team = team;
}

void GameClient::setCharacterName(const std::string& name)
{
	_character_name = name;
}

void GameClient::setClientName(const std::string& name)
{
	_client_name = name;
}