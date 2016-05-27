#pragma once

//------------------------------------------------------------------------
//
//  Name:	ChatMessage.h
//
//  Desc:	
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>

class ChatMessage
{
public:
	enum { HEADER_LENGTH = 4 };
	enum { MAX_BODY_LENGTH = 50000 };

	ChatMessage()
		: _body_length(0)
	{
	}

	const char* data() const
	{
		return _data;
	}

	char* data()
	{
		return _data;
	}

	size_t length() const
	{
		return HEADER_LENGTH + _body_length;
	}

	const char* body() const
	{
		return _data + HEADER_LENGTH;
	}

	char* body()
	{
		return _data + HEADER_LENGTH;
	}

	size_t bodyLength() const
	{
		return _body_length;
	}

	void bodyLength(size_t new_length)
	{
		_body_length = new_length;
		if (_body_length > MAX_BODY_LENGTH)
			_body_length = MAX_BODY_LENGTH;
	}

	bool decodeHeader()
	{
		using namespace std; // For strncat and atoi.
		char header[HEADER_LENGTH + 1] = "";
		strncat(header, _data, HEADER_LENGTH);
		_body_length = atoi(header);
		if (_body_length > MAX_BODY_LENGTH)
		{
			_body_length = 0;
			return false;
		}
		return true;
	}

	void encodeHeader()
	{
		using namespace std; // For sprintf and memcpy.
		char header[HEADER_LENGTH + 1] = "";
		sprintf(header, "%4d", _body_length);
		memcpy(_data, header, HEADER_LENGTH);
	}

private:
	char _data[HEADER_LENGTH + MAX_BODY_LENGTH];
	size_t _body_length;
};
