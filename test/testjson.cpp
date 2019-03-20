//
//  testjson.cpp
//  OrbitDyn
//
//  Created by 韩 冬 on 2018/11/4.
//

#include <iostream>
using namespace std;
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void ReadOrbitJSON(string filename)
{
    json j;
}

int main(int argc, char* argv[])
{
	// create an empty structure (null)
	json j;

	// add a number that is stored as double (note the implicit conversion of j to an object)
	j["pi"] = 3.141;

	// add a Boolean that is stored as bool
	j["happy"] = true;

	// add a string that is stored as std::string
	j["name"] = "Niels";

	// add another null object by passing nullptr
	j["nothing"] = nullptr;

	// add an object inside the object
	j["answer"]["everything"] = 42;

	// add an array that is stored as std::vector (using an initializer list)
	j["list"] = { 1, 0, 2 };

	// add another object (using an initializer list of pairs)
	j["object"] = { { "currency", "USD" },{ "value", 42.99 } };



	// instead, you could also write (which looks very similar to the JSON above)
	json j2 = {
		{ "pi", 3.141 },
	{ "happy", true },
	{ "name", "Niels" },
	{ "nothing", nullptr },
	{ "answer",{
		{ "everything", 42 }
	} },
	{ "list",{ 1, 0, 2 } },
	{ "object",{
		{ "currency", "USD" },
	{ "value", 42.99 }
	} }
	};

	// serialize to standard output
	std::cout << j << endl;
	std::cout << j2 << endl;
}
