#pragma once

#include <memory>
#include <string>
#include <vector>

#include "properties.h"

template <typename D> class geometry {
protected:
	virtual geometry* clone_impl() const = 0;

public:
	geometry() {}
	std::unique_ptr<geometry> clone() const {
		return std::unique_ptr<geometry>(clone_impl());
	}
	virtual ~geometry() {}
	virtual void draw() const = 0;
	virtual std::vector<D> comp_list() const = 0;
	virtual bool operator<(const geometry& oth) const {
		return comp_list() < oth.comp_list();
	}
	properties prop;
};
