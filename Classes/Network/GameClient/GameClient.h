#pragma once

//------------------------------------------------------------------------
//
//  Name:	GameClient.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <deque>
#include <queue>
#include <tuple>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include "GameMessage.hpp"


class GameClient
{
public:
	GameClient(
		boost::asio::io_service& io_service,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	~GameClient();

	// getters && setters

	unsigned int getId() const;

	int getLevel() const;

	int getGold() const;

	int getTeam() const;

	const std::string& getCharacterName() const;

	const std::string& getClientName() const;


	void setId(unsigned int id);

	void setLevel(int level);

	void setGold(int gold);

	void setTeam(int team);

	void setCharacterName(const std::string& name);

	void setClientName(const std::string& name);


	// main methods

	void send(const GameMessage& msg);

	void close();

private:

	void handleConnect(const boost::system::error_code& error);

	void handleReadHeader(const boost::system::error_code& error);

	void handleReadBody(const boost::system::error_code& error);

	void doSend(GameMessage msg);

	void handleSend(const boost::system::error_code& error);

	void doClose();

private:

	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::socket _socket;
	GameMessage _read_msg;
	std::deque<GameMessage> _send_msgs;

	// client data
	unsigned int _id;
	int _level;
	int _gold;
	std::string _character_name;
	std::string _client_name;
	int _team;


};