#pragma once
#include "vector_2d.h"
#include "car_way.h"
#include "dynamyc_car_params.h"

#define $$ [[nodiscard]]

class car_control
{
	const car_way& car_way_;
	size_t cur_cp_;
	size_t cur_round_;

protected:

	void update_check_points(const dynamic_car_params& params);
	$$ vector_2d<float> get_cur_cp() const;
	
public:

	explicit car_control(const car_way& car_way);
	
	virtual ~car_control() = default;
	virtual void get_control(dynamic_car_params& params) = 0;
	$$ size_t get_rounds() const;
	$$ size_t get_cp_count() const;
	
};

#undef $$