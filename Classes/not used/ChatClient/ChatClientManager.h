#pragma once

//------------------------------------------------------------------------
//
//  Name:	ChatClient.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include "ChatClient.h"

#include <string>
#include <memory>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/format.hpp>


#include "../../Singleton.h"


class ChatClientManager : public Singleton<ChatClientManager>
{
public:

	~ChatClientManager()
	{
		_game_client->close();
		_async_io_thread->join();
	}

	void forwardWrite(std::string str)
	{
		ChatMessage msg;
		msg.bodyLength(str.size());
		std::memcpy(msg.body(), str.c_str(), msg.bodyLength());
		msg.encodeHeader();
		_game_client->write(msg);
	}

	const std::unique_ptr<ChatClient>& getChatClient() const
	{
		return _game_client;
	}

private:
	ChatClientManager(const ChatClientManager&) = delete; // no copies
	ChatClientManager& operator=(const ChatClientManager&) = delete; // no self-assignments
	ChatClientManager()
	{
		try
		{
			// 학교 컴퓨터(서버)
			//std::string name = "220.67.128.17";
			//std::string port = "8000";

			// MIT WIFI를 사용하는 내 컴퓨터
			std::string name = "192.168.0.49";
			std::string port = "8001";

			// 멤버십 WIFI
			//std::string name = "112.108.39.213";
			//std::string port = "8001";

			boost::asio::ip::tcp::resolver resolver(_io_service);
			boost::asio::ip::tcp::resolver::query query(name, port);
			boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

			_game_client.reset(new ChatClient(_io_service, iterator));
			_async_io_thread.reset(
				new std::thread(
					boost::bind(&boost::asio::io_service::run, &_io_service)));
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}

	friend class Singleton<ChatClientManager>;

	std::unique_ptr<ChatClient> _game_client;
	std::unique_ptr<std::thread> _async_io_thread;
	boost::asio::io_service _io_service;
};