#pragma once

#include <memory>
#include <vector>

#include "canvas.h"
#include "geometry.h"
#include "point.h"
#include "tool.h"
#include "tool_algo.h"

template <typename T, typename D>
class tool_algo_analysis : public tool_algo<D> {
private:
	virtual std::vector<std::unique_ptr<geometry<D>>>
	analyse(std::vector<std::reference_wrapper<const T>>&& geo_arr) = 0;

protected:
	virtual void run_algo(canvas<D>& can, const pt<D>& mp) {
		std::vector<std::unique_ptr<geometry<D>>> result =
				analyse(can.template filter_get<T>());
		can.push(result);
	}
};
