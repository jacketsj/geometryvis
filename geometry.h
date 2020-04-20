#pragma once

#include <string>
#include <vector>

template <typename D> class geometry {
public:
	virtual void draw() const = 0;
	virtual std::vector<D> comp_list() const = 0;
	virtual bool operator<(const geometry& oth) const {
		return comp_list() < oth.comp_list();
	}
};
