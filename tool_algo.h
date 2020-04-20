#pragma once

#include <memory>
#include <vector>

#include "canvas.h"
#include "point.h"
#include "tool.h"

template <typename D> class tool_algo : public tool<D> {
private:
	pt<D> cur_mp;

protected:
	virtual void run_algo(canvas<D>& can, const pt<D>& mp) = 0;

public:
	virtual void l_click(canvas<D>& can) { run_algo(can, cur_mp); }
	virtual void l_release(canvas<D>& can) {}
	virtual void r_click(canvas<D>& can) {}
	virtual void r_release(canvas<D>& can) {}
	virtual void draw() const {}
	virtual void update(canvas<D>& c, const pt<D>& mp) { cur_mp = mp; }
};
