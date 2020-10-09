#include <iostream>

#include "nlohmann/json.hpp"

int main() {
	nlohmann::json test;
	test["example"] = "Hello World!";
    std::cout << test["example"];
}
