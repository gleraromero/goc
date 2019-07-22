//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_LINEAR_PROGRAMMING_SOLVER_BC_SOLVER_H
#define GOC_LINEAR_PROGRAMMING_SOLVER_BC_SOLVER_H

#include <iostream>
#include <unordered_set>
#include <vector>

#include "goc/lib/json.hpp"
#include "goc/linear_programming/cuts/separation_strategy.h"
#include "goc/linear_programming/model/branch_priority.h"
#include "goc/linear_programming/model/formulation.h"
#include "goc/linear_programming/model/valuation.h"
#include "goc/log/bc_execution_log.h"
#include "goc/time/duration.h"

namespace goc
{
// Class representing a solver for branch and cut. Its purpose is to abstract the
// specific solver implementations from the algorithms.
// The solver has the following parameters:
//	screen_output: pointer to the stream where the output of the algorithm should go. (nullptr for no output).
//	time_limit: maximum time to spend solving the colgen.
//	config: json object with the configuration options to send to the solver. Example: {"CPX_PARAM_REDUCE": 0, ...}.
// 	separation_strategy: object that indicates what families of cuts will be added and the strategy to do so.
//	initial_solutions: a set of initial solutions for the BC.
//	branch_priorities: 	each variable might receive a number containing the priority on how important is to branch on
//						that variable. The higher the priority the earliest the variable will be selected to be branched.
class BCSolver
{
public:
	std::ostream* screen_output;
	Duration time_limit;
	nlohmann::json config;
	SeparationStrategy separation_strategy;
	std::vector<Valuation> initial_solutions;
	std::vector<BranchPriority> branch_priorities;
	
	// Creates a default branch and cut solver. (time limit: 2 hours).
	// Currently: CPLEX.
	BCSolver();
	
	// Solves the formulation.
	// Returns: the execution log with the specified options.
	// Precondition: the formulation must have been created with the NewFormulation() method.
	BCExecutionLog Solve(Formulation* formulation, const std::unordered_set<BCOption>& options) const;
	
	// Returns: a formulation compatible with the solver.
	static Formulation* NewFormulation();
};
} // namespace goc

#endif //GOC_LINEAR_PROGRAMMING_SOLVER_BC_SOLVER_H
