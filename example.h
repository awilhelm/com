/// \file
/// example.h et example.cc montrent comment utiliser Libcom pour définir un protocole de communication.

#pragma once

#include <vector>

struct CustomType {
	CustomType(void);
	CustomType(double, double, double);
	template<class Archive> void serialize(Archive &, unsigned);
	double x, y, z;
};

/// Définition des messages échangés.
/// Utilisé par stub.h et stub.hh pour générer le protocole.
#define PROTOCOL MySender, MyReceiver,\
	MESSAGE(hello, (void), ())\
	MESSAGE(numbers, (int16_t a, float b), (a, b))\
	MESSAGE(text, (const std::string &s), (s))\
	MESSAGE(list, (const std::vector<std::string> &v), (v))\
	MESSAGE(custom, (const CustomType &v), (v))\

// Génère le code commun à tous les protocoles (définition des classes MySender et MyReceiver).
#include "stub.h"
