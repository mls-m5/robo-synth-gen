#include "common.h"
#include <iostream>
#include <vector>
#include "wavfile.h"
#include <cmath>
#include <map>

#include "mstring.h"


using namespace std;

mstring morse = R"apa(
a .-
b -...
c -.-.
d -..
e .
f ..-.
g --.
h ....
i ..
j .---
k -.-
l .-..
m --
n -.
o ---
p .--.
q --.-
r .-.
s ...
t -
u ...-
v ...-
w .--
x -..-
y -.--
z --..
å .--.-
ä .-.-
ö ---.)apa";


std::map <string, string> lookup;

float triangle(float phase) {
	return abs((phase - (int) phase) * -2 + 1) - 1;
}

vector <int> extractData(string data, int length) {
	vector <int>ret;

	ret.resize(length, 0);

	for (size_t i = 0; i < data.size() && i < length; ++i) {
		ret[i] = data[i] == '.' ? 1: 2;
	}

	return ret;
}

void printData(vector<int> data) {
	cout << "Data: " ;
	for (auto it: data) {
		cout << it << " ";
	}
	cout << endl;
}

void generateFile(string name, string dataString) {
	WavFile file("output/" + name + ".wav", 1);
	auto data = extractData(dataString, 8);

	constexpr int framerate = 44100;
	float duration = .1; //Duration in seconds
	float transpose = 1.;
	switch(data[2]) {
	case 0:
		break;
	case 1:
		transpose = .5;
		break;
	case 2:
		transpose = 2;
		break;
	}
	float f1 = 880 * transpose;
	float f2 = 880 * exp2(7./12.) * transpose;

	vector<float> samples(framerate * duration);
	double phase = 0;
	double t = 0;
	double amount = 0;

	double fmod1 = 0;
	double fmod1amplitude = 2;
	double fmod2 = 0;

	cout << "Generating file " << name << " with data " << dataString << endl;
	printData(data);


	for (int i = 0; i < samples.size(); ++i) {
		t = (double)i / framerate;
		amount = t / duration;
		if (data[0] == 1) {
			samples[i] = sin(pi2 * phase);
		}
		else {
			samples[i] = triangle(phase);
		}

		auto f = 0;
		switch(data[1]) {
		case 0:
			f = f1;
			break;
		case 1:
			f = f1 * amount + f2 * (1-amount);
			break;
		case 2:
			f = f2 * amount + f1 * (1-amount);
			break;
		}

		switch(data[3]) {
		case 0:
			fmod1 = 0;
			break;
		case 1:
			fmod1 = sin(amount * pi);
			break;
		case 2:
			fmod1 = -sin(amount * pi);
		}

		f *= exp2(fmod1);



		switch(data[4]) {
		case 0:
			fmod2 = 0;
			break;
		case 1:
			fmod2 = sin(t*600.) / 2;
			break;
		case 2:
			fmod2 = sin(t*600.);
			break;
		}

		f *= exp2(fmod2);

		phase += f / (double)framerate;
	}

	file.write(samples);
}


int main(int argc, char **arv) {
	auto var = pi;


	
	auto str = morse.split('\n');
	for (auto s: str) {
		if (s == "") {
			continue;
		}
		auto str2 = split(s, ' ');
		lookup[str2[0]] = str2[1];
	}

	for (auto it: lookup) {
		generateFile(it.first, it.second);
	}


	cout << "pi is " << pi << endl;
}
