//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_LOG_BCP_EXECUTION_LOG_H
#define GOC_LOG_BCP_EXECUTION_LOG_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "goc/base/maybe.h"
#include "goc/lib/json.hpp"
#include "goc/linear_programming/model/valuation.h"
#include "goc/log/bc_execution_log.h"
#include "goc/time/duration.h"

namespace goc
{
// All the log options that can be enabled/disabled.
// - ScreenOutput: 		if not included, the output will not be stored.
//						advantage: saving space.
// - RootInformation: 	if not included {root_lp_value, root_int_value, root_int_solution, root_constraint_count,
// 						root_variable_count, root_log} will not be filled.
//						advantage: space and execution time is saved if that information is not relevant.
// - CutInformation: 	if not included {cut_count, cut_iteration_count, cut_time, cut_families, cut_family_cut_count,
//						cut_family_iteration_count, cut_family_cut_time} will not be filled.
//						advantage: saving space.
enum class BCPOption {
	ScreenOutput, RootInformation, CutInformation
};

// This class stores information about the execution of a branch-cut-price solver.
// It is compatible with the Kaleidoscope kd_type "bcp".
class BCPExecutionLog : public BCExecutionLog
{
public:
	Maybe<int> root_constraint_count; // number of constraints after solving the root node.
	Maybe<int> root_variable_count; // number of variables after solving the root node.
	Maybe<int> final_constraint_count; // number of constraints at the end of the execution.
	Maybe<int> final_variable_count; // number of variables at the end of the execution.
	Maybe<Duration> lp_time; // time spent solving linear relaxations.
	Maybe<Duration> pricing_time; // time spent solving pricing problems.
	Maybe<Duration> branching_time; // time spent branching.
	
	BCPExecutionLog() = default;
	
	virtual nlohmann::json ToJSON() const;
};
} // namespace goc

#endif //GOC_LOG_BCP_EXECUTION_LOG_H
