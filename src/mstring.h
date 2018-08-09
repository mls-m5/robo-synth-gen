/*
 * mstring.h
 *
 *  Created on: 9 aug. 2018
 *      Author: mattias
 */


#pragma once
#include <string>
#include <vector>


class mstring: public std::string {
public:
	using std::string::string; //Inherits conscrutors

	~mstring() {}

	operator std::string() {
		return *((std::string*)this);
	}

	std::vector<std::string> split(char delimiter = ' ') {
		using std::string;
		string tmp;
		tmp.reserve(10);
		std::vector<string> ret;
		for (auto c: *this) {
			if (c == delimiter) {
				ret.push_back(tmp);
				tmp.clear();
			}
			else {
				tmp.push_back(c);
			}
		}

		if (!tmp.empty()) {
			ret.push_back(tmp);
		}

		return ret;
	}
};

std::vector<std::string> split(std::string str, char delimiter = ' ') {
	using std::string;
	string tmp;
	tmp.reserve(10);
	std::vector<string> ret;
	for (auto c: str) {
		if (c == delimiter) {
			ret.push_back(tmp);
			tmp.clear();
		}
		else {
			tmp.push_back(c);
		}
	}

	if (!tmp.empty()) {
		ret.push_back(tmp);
	}

	return ret;
}


