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

#include <deque>
#include <queue>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include "ChatMessage.hpp"

#include "ui/UIText.h"

class ChatClient
{
public:
	ChatClient(
		boost::asio::io_service& io_service,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

	void write(const ChatMessage& msg);

	void close();

	std::queue<std::string>& getReadMsgQ()
	{
		return _read_msg_queue;
	}

private:

	void handleConnect(const boost::system::error_code& error);

	void handleReadHeader(const boost::system::error_code& error);

	void handleReadBody(const boost::system::error_code& error);

	// 비동기 IO에서 가장 조심해야 하는 것은,
	// write가 다 끝나지 않은 상태에서 write해야 할 buffer를 건드리는 것이다.
	// 그 문제를 해결하기 위해 doWrite와 handleWrite에 다음과 같은 장치를 해 둔다.
	// 1. 먼저 deque에 msg가 남아있는지 확인한다.
	//    msg가 남아있다는 것은 아직 write가 다 끝나지 않았다는 의미이고, 이를 flag에 체크해둔다.
	// 2. 그 후 doWrite에서는 deque에 write할 msg를 넣는다.
	//    doWrite에서 호출한 async_write가 끝나면, 정말로 비동기 IO가 끝났다는 의미이다.
	// 3. 그러므로 doWrite이 호출하는 callback함수인 handleWrite에서는 
	//    deque의 msg를 빼준다.
	// 4. deque에 더 msg가 남아있을 수 있다. 
	//    그러므로 남아있다면 다시 callback으로 handleWrite를 넣어준다.
	void doWrite(ChatMessage msg);

	void handleWrite(const boost::system::error_code& error);

	void doClose();

private:

	//boost::lockfree::queue<char> chat_queue;

	boost::asio::io_service& _io_service;
	boost::asio::ip::tcp::socket _socket;
	ChatMessage _read_msg;
	std::deque<ChatMessage> _write_msgs;
	
	std::queue<std::string> _read_msg_queue;
};