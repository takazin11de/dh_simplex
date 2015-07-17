#include "user_std.hpp"
#include "common_data.hpp"

#ifndef BEZIERS_HPP
#define BESIERS_HPP

void Beziers(std::vector<xydata> data_org,
			std::vector<xydata> &data,
			std::vector<xydata> &data_dot,
			std::vector<xydata> &data_ddot);

#endif
