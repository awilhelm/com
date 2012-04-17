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

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include "com.h"

#define MAX_PARAMS 10

#define SERIALIZE(z, n, dummy)\
archive << p##n;\

#define DESERIALIZE(z, n, dummy)\
typename boost::remove_cv<typename boost::remove_reference<T##n>::type>::type p##n;\
archive >> p##n;\

#define DEFINE_WORKER(z, n, dummy)\
\
template<BOOST_PP_ENUM_PARAMS(n, class T)>\
void receive(const boost::function<void(BOOST_PP_ENUM_PARAMS(n, T))> &callback, const std::string &data)\
{\
	std::istringstream ss(data);\
	boost::archive::binary_iarchive archive(ss, boost::archive::no_header);\
	BOOST_PP_REPEAT(n, DESERIALIZE, ());\
	callback(BOOST_PP_ENUM_PARAMS(n, p));\
}\
\
template<BOOST_PP_ENUM_PARAMS(n, class T)>\
void Sender::operator()(BOOST_PP_ENUM_BINARY_PARAMS(n, T, p))\
{\
	std::ostringstream ss;\
	boost::archive::binary_oarchive archive(ss, boost::archive::no_header);\
	BOOST_PP_REPEAT(n, SERIALIZE, ());\
	send(ss.str());\
}\

#define DECLARE_WORKER(z, n, dummy)\
template<BOOST_PP_ENUM_PARAMS(n, class T)>\
void operator()(BOOST_PP_ENUM_PARAMS(n, T));\

namespace com {

struct Sender {
	Sender(Socket &, uint32_t);
	void operator()(void);
	BOOST_PP_REPEAT_FROM_TO(1, MAX_PARAMS, DECLARE_WORKER, ());
private:
	void send(const std::string &);
	Socket &socket;
	const uint32_t type;
};

template<class T> void
receive_aux(const boost::function<T> &callback, const std::string &data)
{
	receive(callback, data);
}

template<> void receive_aux<void(void)>(const boost::function<void(void)> &, const std::string &);
BOOST_PP_REPEAT_FROM_TO(1, MAX_PARAMS, DEFINE_WORKER, ());

}
