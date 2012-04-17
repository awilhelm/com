/// \file
/// example.x, example.h et example.cc montrent comment utiliser Libcom pour définir un protocole de communication.

PROTOCOL(MySender, MyReceiver,
	MESSAGE(hello, (void), ())
	MESSAGE(numbers, (int16_t a, float b), (a, b))
	MESSAGE(text, (const std::string &s), (s))
	MESSAGE(list, (const std::vector<std::string> &v), (v))
)
