/// \file
/// example.h et example.cc montrent comment utiliser Libcom pour définir un protocole de communication.

#pragma once

#include <vector>

/// Définition des messages échangés.
/// Utilisé par stub.h et stub.hh pour générer le protocole.
#define PROTOCOL MySender, MyReceiver,\
	MESSAGE(hello, (void), ())\
	MESSAGE(numbers, (int16_t a, float b), (a, b))\
	MESSAGE(text, (const std::string &s), (s))\
	MESSAGE(list, (const std::vector<std::string> &v), (v))\

// Génère le code commun à tous les protocoles (définition des classes MySender et MyReceiver).
#include "stub.h"
