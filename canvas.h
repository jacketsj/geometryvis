#pragma once

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "basic_rect.h"
#include "console.h"
#include "geometry.h"
#include "rect.h"

template <typename D> class canvas {
private:
	struct canvas_state {
		std::vector<std::unique_ptr<geometry<D>>> geo_stack;
		std::set<size_t> selected;
		canvas_state() {}
		canvas_state(const canvas_state& cs) {
			for (const auto& geo_ptr : cs.geo_stack)
				geo_stack.push_back(geo_ptr->clone());
			selected = cs.selected;
		}
	};
	std::vector<canvas_state> cvs_history;
	size_t current_state = -1;

	canvas_state& get_state() { return cvs_history[current_state]; }

public:
	canvas() {
		cvs_history.emplace_back();
		current_state = 0;
	}

	void undo() {
		console::get().print("undo");
		if (current_state > 0)
			--current_state;
	}
	void redo() {
		console::get().print("redo");
		if (current_state + 1 < cvs_history.size())
			++current_state;
	}
	void clear_redos() { cvs_history.resize(current_state + 1); }
	void save_state() {
		clear_redos();
		cvs_history.emplace_back(cvs_history.back());
		++current_state;
	}

	template <typename T, typename... Args> void push(Args&&... args) {
		save_state();
		get_state().geo_stack.push_back(std::make_unique<T>(args...));
	}
	void push(std::vector<std::unique_ptr<geometry<D>>>& geos) {
		save_state();
		for (std::unique_ptr<geometry<D>>& geo : geos) {
			save_state();
			get_state().geo_stack.push_back(std::move(geo));
		}
	}
	void push(std::unique_ptr<geometry<D>>& geo) {
		save_state();
		get_state().geo_stack.push_back(std::move(geo));
	}
	void pop() {
		save_state();
		get_state().geo_stack.pop_back();
	}
	void draw() {
		// draw all geometry
		for (auto& geo_ptr : get_state().geo_stack)
			geo_ptr->draw();
		// draw selected geometry differently (TODO: move this elsewhere)
		for (size_t i : get_state().selected) {
			col::col c = get_state().geo_stack[i]->prop.c;
			get_state().geo_stack[i]->prop.c = col::selected;
			get_state().geo_stack[i]->draw();
			get_state().geo_stack[i]->prop.c = c;
		}
	}

	// returns the selected elements of a particular type, or all of the type if
	// nothing is selected
	template <typename T>
	std::vector<std::reference_wrapper<const T>> filter_get() {
		std::vector<std::reference_wrapper<const T>> ret;
		for (std::unique_ptr<geometry<D>>& geo : get_state().geo_stack) {
			try {
				T& geo_casted = dynamic_cast<T&>(*geo);
				ret.push_back(geo_casted);
			} catch (std::bad_cast& bc) {
				// skip
			}
		}
		return ret;
	}

	std::vector<std::reference_wrapper<geometry<const D>>> get() {
		return filter_get<geometry<D>>();
	}

	std::vector<std::reference_wrapper<geometry<D>>> get_selected() {
		std::vector<std::reference_wrapper<geometry<D>>> ret;
		for (size_t i : get_state().selected) {
			ret.push_back(*get_state().geo_stack[i]);
		}
		return ret;
	}

	void select(const pt<D> p) {
		save_state();
		auto& stack = get_state().geo_stack;
		for (size_t i = 0; i < stack.size(); ++i) {
			size_t elem = stack.size() - i - 1;
			if (rect<D>(stack[elem]->bounding_box()).pad(32).contains(p)) {
				if (get_state().selected.count(elem) > 0)
					get_state().selected.erase(elem);
				else
					get_state().selected.insert(elem);
				break;
			}
		}
	}
	void deselect_all() {
		save_state();
		get_state().selected.clear();
	}

	void delete_selected() {
		save_state();
		std::vector<size_t> sel;
		sel.reserve(get_state().selected.size());
		for (size_t i : get_state().selected)
			sel.push_back(i);
		std::reverse(sel.begin(), sel.end());
		auto& geo = get_state().geo_stack;
		for (size_t i : sel) {
			swap(geo[i], geo.back());
			get_state().geo_stack.pop_back();
		}
		get_state().selected.clear();
	}
};
