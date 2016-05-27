
#include "ChatClient.h"

#include <deque>
#include <iostream>
#include <sstream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include "ChatMessage.hpp"


ChatClient::ChatClient(
	boost::asio::io_service& io_service,
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
	:
	_io_service(io_service),
	_socket(io_service)
{
	boost::asio::async_connect(
		_socket,
		endpoint_iterator,
		boost::bind(
			&ChatClient::handleConnect,
			this,
			boost::asio::placeholders::error));
}

void ChatClient::write(const ChatMessage& msg)
{
	_io_service.post(boost::bind(&ChatClient::doWrite, this, msg));
}

void ChatClient::close()
{
	_io_service.post(boost::bind(&ChatClient::doClose, this));
}

void ChatClient::handleConnect(const boost::system::error_code& error)
{
	if (!error)
	{
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.data(),
				ChatMessage::HEADER_LENGTH),
			boost::bind(
				&ChatClient::handleReadHeader,
				this,
				boost::asio::placeholders::error));
	}
}

void ChatClient::handleReadHeader(const boost::system::error_code& error)
{
	if (!error && _read_msg.decodeHeader())
	{
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.body(),
				_read_msg.bodyLength()),
			boost::bind(
				&ChatClient::handleReadBody,
				this,
				boost::asio::placeholders::error));
	}
	else
	{
		doClose();
	}
}

void ChatClient::handleReadBody(const boost::system::error_code& error)
{
	if (!error)
	{
		std::stringstream ss;
		ss.write(_read_msg.body(), _read_msg.bodyLength());

		CCLOG(ss.str().c_str());

		_read_msg_queue.emplace(ss.str());

		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
				_read_msg.data(),
				ChatMessage::HEADER_LENGTH),
			boost::bind(
				&ChatClient::handleReadHeader,
				this,
				boost::asio::placeholders::error));
	}
	else
	{
		doClose();
	}
}

void ChatClient::doWrite(ChatMessage msg)
{
	bool write_in_progress = !_write_msgs.empty();
	_write_msgs.push_back(msg);
	if (!write_in_progress)
	{
		boost::asio::async_write(
			_socket,
			boost::asio::buffer(
				_write_msgs.front().data(),
				_write_msgs.front().length()),
			boost::bind(
				&ChatClient::handleWrite,
				this,
				boost::asio::placeholders::error));
	}
}

void ChatClient::handleWrite(const boost::system::error_code& error)
{
	if (!error)
	{
		_write_msgs.pop_front();
		if (!_write_msgs.empty())
		{
			boost::asio::async_write(
			_socket,
				boost::asio::buffer(
					_write_msgs.front().data(),
					_write_msgs.front().length()),
				boost::bind(&ChatClient::handleWrite, this,
					boost::asio::placeholders::error));
		}
	}
	else
	{
		doClose();
	}
}

void ChatClient::doClose()
{
	_socket.close();
}

