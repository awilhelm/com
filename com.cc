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

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "com.hh"

using boost::asio::ip::tcp;

namespace com {

namespace {

static void
open_server(const boost::function<void(tcp::socket &)> &callback, const boost::shared_ptr<boost::asio::io_service> &, const boost::shared_ptr<tcp::socket> &socket)
{
	try
	{
		callback(*socket);
	}
	catch(const boost::system::system_error &e)
	{
		std::cerr << e.code() << ": " << e.what() << std::endl;
	}
}

static bool
try_accept(tcp::acceptor &acceptor, tcp::socket &socket, const boost::posix_time::time_duration &timeout)
{
	boost::optional<boost::system::error_code> accept_result, wait_result;
	boost::asio::deadline_timer timer(acceptor.get_io_service(), timeout);
	timer.async_wait(boost::bind(&boost::optional<boost::system::error_code>::reset, &wait_result, _1));
	acceptor.async_accept(socket, boost::bind(&boost::optional<boost::system::error_code>::reset, &accept_result, _1));
	acceptor.get_io_service().reset();
	while(acceptor.get_io_service().run_one())
	{
		if(accept_result)
		{
			timer.cancel();
		}
		else if(wait_result)
		{
			acceptor.cancel();
		}
	}
	return !*accept_result;
}

template<class Buffer> static bool
try_read(tcp::socket &socket, const Buffer &buffer, const boost::posix_time::time_duration &timeout)
{
	boost::optional<boost::system::error_code> read_result, wait_result;
	boost::asio::deadline_timer timer(socket.get_io_service(), timeout);
	timer.async_wait(boost::bind(&boost::optional<boost::system::error_code>::reset, &wait_result, _1));
	async_read(socket, buffer, boost::bind(&boost::optional<boost::system::error_code>::reset, &read_result, _1));
	socket.get_io_service().reset();
	while(socket.get_io_service().run_one())
	{
		if(read_result)
		{
			timer.cancel();
		}
		else if(wait_result)
		{
			socket.cancel();
		}
	}
	if(*read_result)
	{
		static const boost::system::error_code e(boost::system::errc::operation_canceled, boost::system::system_category);
		if(*read_result == e)
		{
			return false;
		}
		throw boost::system::system_error(*read_result);
	}
	return true;
}

}

void
open(const std::string &host, uint16_t port, const boost::function<void(tcp::socket &)> &callback)
{
	open(host, port, callback, boost::posix_time::seconds(1));
}

void
open(const std::string &host, uint16_t port, const boost::function<void(tcp::socket &)> &callback, const boost::posix_time::time_duration &timeout)
{
	boost::asio::io_service service;
	bool error = false;
	for(;;)
	{
		try
		{
			tcp::socket socket(service);
			socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(host), port));
			error = false;
			callback(socket);
			break;
		}
		catch(const boost::system::system_error &e)
		{
			if(!error)
			{
				std::cerr << e.code() << ": " << e.what() << std::endl;
				error = true;
			}
			boost::this_thread::sleep(timeout);
		}
	}
}

void
open(uint16_t port, const boost::function<void(tcp::socket &)> &callback)
{
	open(port, callback, boost::posix_time::seconds(1));
}

void
open(uint16_t port, const boost::function<void(tcp::socket &)> &callback, const boost::posix_time::time_duration &timeout)
{
	boost::asio::io_service service;
	tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), port));
	boost::thread_group threads;
	try
	{
		for(;;)
		{
			const boost::shared_ptr<boost::asio::io_service> new_service(new boost::asio::io_service);
			const boost::shared_ptr<tcp::socket> socket(new tcp::socket(*new_service));
			if(try_accept(acceptor, *socket, timeout))
			{
				threads.create_thread(boost::bind(open_server, callback, new_service, socket));
			}
			boost::this_thread::interruption_point();
		}
	}
	catch(const boost::thread_interrupted &)
	{
		threads.interrupt_all();
		threads.join_all();
	}
}

Sender::Sender(tcp::socket &socket, uint32_t type):
	socket(socket),
	type(type) {}

void
Sender::send(const std::string &data)
{
	const uint32_t size = data.size();
	std::vector<boost::asio::const_buffer> bytes;
	bytes.push_back(boost::asio::buffer(&type, sizeof(type)));
	bytes.push_back(boost::asio::buffer(&size, sizeof(size)));
	bytes.push_back(boost::asio::buffer(data));
	write(socket, bytes);
}

void
Sender::operator()(void)
{
	static const std::string data;
	send(data);
}

Receiver::Receiver(tcp::socket &socket):
	_socket(socket) {}

void
Receiver::operator()(void) const
{
	(*this)(boost::posix_time::seconds(1));
}

void
Receiver::operator()(const boost::posix_time::time_duration &timeout) const
{
	uint32_t type, size;
	if(try_read(_socket, boost::asio::buffer(&type, sizeof(type)), timeout))
	{
		read(_socket, boost::asio::buffer(&size, sizeof(size)));
		char data[size];
		read(_socket, boost::asio::buffer(data, size));
		const std::map<uint32_t, boost::function<void(const std::string &)> >::const_iterator p = _callback.find(type);
		if(p != _callback.end() && p->second)
		{
			p->second(std::string(data, size));
		}
	}
	boost::this_thread::interruption_point();
}

template<> void
receive_aux<void(void)>(const boost::function<void(void)> &callback, const std::string &)
{
	callback();
}

void
about(void)
{
	std::cerr << "Ce programme utilise Libcom (nom sujet à modification), une bibliothèque de communication réseau libre développée au printemps 2012 par Alexis Wilhelm." << std::endl;
}

}
