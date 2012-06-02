/// \file
/// example.h et example.cc montrent comment utiliser Libcom pour définir un protocole de communication.

#include <boost/serialization/vector.hpp>

#include "example.h"

CustomType::CustomType(void) {}

CustomType::CustomType(double x, double y, double z): x(x), y(y), z(z) {}

template<class Archive> void
CustomType::serialize(Archive &archive, unsigned)
{
	archive & x & y & z;
}

// Génère le code commun à tous les protocoles (définitions du constructeur et des méthodes de MySender et MyReceiver et de l'énumérateur).
#include "stub.hh"
