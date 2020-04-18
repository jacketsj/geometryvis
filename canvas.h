#pragma once

#include <memory>
#include <string>
#include <vector>

#include "geometry.h"

class canvas {
private:
	std::vector<std::unique_ptr<geometry>> geo_stack;

public:
	template <typename T, typename... Args> void push(Args&&... args) {
		geo_stack.push_back(std::make_unique<T>(args...));
	}
	void pop() { geo_stack.pop_back(); }
	void draw() {
		// draw all geometry
		for (auto& geo_ptr : geo_stack)
			geo_ptr->draw();
	}
};
