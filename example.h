/// \file
/// example.x, example.h et example.cc montrent comment utiliser Libcom pour définir un protocole de communication.

#pragma once

#include <vector>

// Génère le code commun à tous les protocoles (définition des classes MySender et MyReceiver).
#define PROTOCOL_FILE "example.x"
#include "stub.h"
