#pragma once

#include <map>
#include <set>
#include <sl.h>

class button_handler {
private:
	// TODO map and set are slow for small values
	std::map<char, bool> cur, prev; // just pressed std::set<char> watching;
	std::set<char> watching;

	enum input_device { MOUSE, KEY };
	input_device device;

	button_handler(input_device device) : device(device) {}

public:
	static button_handler& get_mouse_handler() {
		static button_handler instance(MOUSE);
		return instance;
	}
	static button_handler& get_key_handler() {
		static button_handler instance(KEY);
		return instance;
	}

	button_handler(const button_handler&) = delete;
	void operator=(const button_handler&) = delete;

public:
	void watch(char c) { watching.insert(c); }
	void unwatch(char c) { watching.erase(c); }
	void update() {
		prev = cur;
		for (char c : watching) {
			switch (device) {
			case MOUSE:
				cur[c] = slGetMouseButton(c);
				break;
			case KEY:
				cur[c] = slGetKey(c);
				break;
			}
		}
	}
	bool pressed(char c) { return cur[c] && !prev[c]; }
	bool released(char c) { return !cur[c] && prev[c]; }
	bool down(char c) { return cur[c]; }
};
