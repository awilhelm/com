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

#include "com.hh"

#define DEFINE_PROTOCOL(sender, receiver, ...)\
enum {\
	__VA_ARGS__\
};\

#define MESSAGE(name, a1, a2)\
name##_,\

DEFINE_PROTOCOL_AUX(PROTOCOL)

#undef DEFINE_PROTOCOL
#undef MESSAGE

#define DEFINE_PROTOCOL(sender, receiver, messages)\
sender::sender(com::Socket &socket): _socket(socket) {}\
receiver::receiver(com::Socket &socket): Receiver(socket) {}\
typedef sender _sender;\
typedef receiver _receiver;\
messages\

#define MESSAGE(name, a1, a2)\
\
void _sender::name a1\
{\
	com::Sender(_socket, name##_) a2;\
}\
\
void _receiver::name(const boost::function<void a1> &function)\
{\
	_callback[name##_] = boost::bind(com::receive_aux<void a1>, function, _1);\
}\

DEFINE_PROTOCOL_AUX(PROTOCOL)

#undef DEFINE_PROTOCOL
#undef MESSAGE
