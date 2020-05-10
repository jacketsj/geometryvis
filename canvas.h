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

	struct page {
		std::vector<canvas_state> cvs_history;
		size_t current_state = -1;

		page() {
			cvs_history.emplace_back();
			current_state = 0;
		}

		canvas_state& get_state() { return cvs_history[current_state]; }

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
	};

	struct pages {
		std::vector<page> page_vec;
		size_t current_page;
		pages() {
			page_vec.emplace_back();
			current_page = 0;
		}
		page& get_page() { return page_vec[current_page]; }
	};

	std::vector<pages> pages_history;
	size_t current_page_state;

	pages& get_pages_state() { return pages_history[current_page_state]; }
	page& get_page() { return get_pages_state().get_page(); }
	canvas_state& get_state() { return get_page().get_state(); }

public:
	canvas() {
		pages_history.emplace_back();
		current_page_state = 0;
	}

	void undo() { get_page().undo(); }
	void redo() { get_page().redo(); }
	void clear_redos() { get_page().clear_redos(); }
	void save_state() { get_page().save_state(); }

	// TODO
	void undo_page() {
		console::get().print("undo page");
		if (current_page_state > 0)
			--current_page_state;
	}
	void redo_page() {
		console::get().print("redo page");
		if (current_page_state + 1 < pages_history.size())
			++current_page_state;
	}
	void clear_page_redos() { pages_history.resize(current_page_state + 1); }
	void save_page_state() {
		clear_page_redos();
		pages_history.emplace_back(pages_history.back());
		++current_page_state;
	}
	void insert_page(page& p) {
		save_page_state();
		pages& ps = get_pages_state();
		ps.page_vec.insert(ps.page_vec.begin() + ps.current_page, p);
	}
	void new_page() { insert_page(page()); }
	void duplicate_page() { insert_page(get_page()); }
	void delete_page() {
		save_page_state();
		pages& ps = get_pages_state();
		ps.page_vec.remove(ps.page_vec.begin() + ps.current_page);
	}

	template <typename T, typename... Args> void push(Args&&... args) {
		save_state();
		get_state().geo_stack.push_back(std::make_unique<T>(args...));
	}
	void push(std::vector<std::unique_ptr<geometry<D>>>& geos) {
		save_state();
		for (std::unique_ptr<geometry<D>>& geo : geos) {
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

	void draw_selected(pt<D> delta = pt<D>(0, 0)) {
		// draw selected geometry differently (TODO: move this elsewhere)
		for (size_t i : get_state().selected) {
			std::unique_ptr<geometry<D>> cur = get_state().geo_stack[i]->clone();
			cur->prop.c = col::selected;
			cur->translate(delta.to_basic());
			cur->draw();
		}
	}
	void draw() {
		// draw all geometry
		for (auto& geo_ptr : get_state().geo_stack)
			geo_ptr->draw();
		draw_selected();
	}

	// returns the selected elements of a particular type, or all of the type if
	// nothing is selected
	template <typename T>
	std::vector<std::reference_wrapper<const T>> filter_get() {
		std::vector<std::reference_wrapper<const T>> ret;
		auto add = [&](std::unique_ptr<geometry<D>>& geo) {
			try {
				T& geo_casted = dynamic_cast<T&>(*geo);
				ret.push_back(geo_casted);
			} catch (std::bad_cast& bc) {
				// skip
			}
		};
		if (get_state().selected.empty())
			for (std::unique_ptr<geometry<D>>& geo : get_state().geo_stack) {
				add(geo);
			}
		else
			for (size_t i : get_state().selected) {
				std::unique_ptr<geometry<D>>& geo = get_state().geo_stack[i];
				add(geo);
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

	void select_all() {
		save_state();
		get_state().selected.clear();
		for (size_t i = 0; i < get_state().geo_stack.size(); ++i)
			get_state().selected.insert(i);
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
