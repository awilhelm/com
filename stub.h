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

#include "com.h"

#undef PROTOCOL
#define PROTOCOL(sender, receiver, messages)\
struct sender {\
	explicit sender(com::Socket &);\
	com::Socket &_socket;\
	messages\
};\

#undef MESSAGE
#define MESSAGE(name, a1, a2)\
void name a1;\

#include PROTOCOL_FILE

#undef PROTOCOL
#define PROTOCOL(sender, receiver, messages)\
struct receiver: com::Receiver {\
	explicit receiver(com::Socket &);\
	messages\
};\

#undef MESSAGE
#define MESSAGE(name, a1, a2)\
void name(const boost::function<void a1> &);\

#include PROTOCOL_FILE
