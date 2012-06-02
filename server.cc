/// \file
/// server.cc montre comment utiliser Libcom pour recevoir des messages d'un client.

#include <boost/foreach.hpp>
#include <iostream>

#include "example.h"

/// Fonction appelée chaque fois qu'on reçoit un message « hello ».
static void
receive_hello(void)
{
	std::cout << __func__ << std::endl;
}

/// Fonction appelée chaque fois qu'on reçoit des nombres.
static void
receive_numbers(int16_t a, float b)
{
	std::cout << __func__ << " a:" << a << " b:" << b << std::endl;
}

/// Fonction appelée chaque fois qu'on reçoit un texte.
static void
receive_text(const std::string &s)
{
	std::cout << __func__ << " s:" << s << std::endl;
}

/// Fonction appelée chaque fois qu'on reçoit une liste.
static void
receive_list(const std::vector<std::string> &v)
{
	std::cout << __func__;
	BOOST_FOREACH(const std::string &s, v)
	{
		std::cout << " s:" << s;
	}
	std::cout << std::endl;
}

void
receive_custom(const CustomType &v)
{
	std::cerr << __func__ << " x:" << v.x << " y:" << v.y << " z:" << v.z << std::endl;
}

/// Fonction appelée chaque fois qu'un client ouvre une connexion.
/// \param socket La socket qui permet de communiquer avec le client.
static void
run(com::Socket &socket)
{
	// Initialise un objet capable de recevoir les messages du client avec le protocole example.h.
	MyReceiver receive(socket);

	// Attache des callbacks à chaque message qu'on souhaite traiter.
	receive.hello(receive_hello);
	receive.numbers(receive_numbers);
	receive.text(receive_text);
	receive.list(receive_list);
	receive.custom(receive_custom);

	for(;;)
	{
		// Met le serveur en attente d'un message venant du client.
		// Quand un message sera reçu, le callback associé sera appelé.
		receive();
	}
}

int
main(void)
{
	com::about();

	// Met le serveur en attente d'une connexion entrante sur le port 10000.
	// Quand un client aura ouvert une connexion, la fonction run() sera appelée dans un nouveau thread.
	com::open(10000, run);
}
