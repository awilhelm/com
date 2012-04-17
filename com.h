/*
	Copyright 2012 Alexis Wilhelm

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <boost/function.hpp>
#include <map>

namespace boost {
	namespace asio {
		namespace ip {
			struct tcp;
		}
		template<class> struct stream_socket_service;
		template<class, class> struct basic_stream_socket;
	}
	namespace posix_time {
		struct time_duration;
	}
}

namespace com {

typedef boost::asio::basic_stream_socket<boost::asio::ip::tcp, boost::asio::stream_socket_service<boost::asio::ip::tcp> > Socket;

struct Receiver {
	void operator()(void) const;
	void operator()(const boost::posix_time::time_duration &) const;
protected:
	explicit Receiver(Socket &);
	Socket &_socket;
	std::map<uint32_t, boost::function<void(const std::string &)> > _callback;
};

void open(const std::string &, uint16_t, const boost::function<void(Socket &)> &);
void open(const std::string &, uint16_t, const boost::function<void(Socket &)> &, const boost::posix_time::time_duration &);
void open(uint16_t, const boost::function<void(Socket &)> &);
void open(uint16_t, const boost::function<void(Socket &)> &, const boost::posix_time::time_duration &);

void about(void);

}
