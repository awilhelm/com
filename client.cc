/// \file
/// client.cc montre comment utiliser Libcom pour envoyer des messages à un serveur.

#include <boost/thread/thread.hpp>

#include "example.h"

/// Fonction appelée quand la connexion vers le serveur est établie.
/// \param socket La socket qui permet de communiquer avec le serveur.
static void
run(com::Socket &socket)
{
	// Initialise un objet capable d'envoyer des messages au serveur avec le protocole example.h.
	MySender send(socket);

	for(;;)
	{
		// Envoie des messages au serveur.
		send.hello();
		send.numbers(3, 2.5f);
		send.text("hello world");
		send.list(std::vector<std::string>(3, "hello"));

		// Attend une seconde avant d'envoyer le prochain message.
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}

int
main(void)
{
	com::about();

	// Connecte ce client au port 10000 du serveur 127.0.0.1.
	// Quand la connexion sera établie, la fonction run() sera appelée.
	// Si la connexion est perdue, ce client cherche à se reconnecter.
	com::open("127.0.0.1", 10000, run);
}
