#pragma once

#include <memory>
#include <string>
#include <vector>

#include "geometry.h"

template <typename D> class canvas {
private:
	std::vector<std::unique_ptr<geometry<D>>> geo_stack;

public:
	template <typename T, typename... Args> void push(Args&&... args) {
		geo_stack.push_back(std::make_unique<T>(args...));
	}
	void push(std::unique_ptr<geometry<D>>& geo) {
		geo_stack.push_back(std::move(geo));
	}
	void pop() { geo_stack.pop_back(); }
	void draw() {
		// draw all geometry
		for (auto& geo_ptr : geo_stack)
			geo_ptr->draw();
	}

	template <typename T> std::vector<std::reference_wrapper<T>> filter_get() {
		std::vector<std::reference_wrapper<T>> ret;
		for (std::unique_ptr<geometry<D>>& geo : geo_stack) {
			try {
				T& geo_casted = dynamic_cast<T&>(*geo);
				ret.push_back(geo_casted);
			} catch (std::bad_cast& bc) {
				// skip
			}
		}
		return ret;
	}

	std::vector<std::reference_wrapper<geometry<D>>> get() {
		return filter_get<geometry<D>>();
	}
};
