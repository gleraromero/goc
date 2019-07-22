//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_LINEAR_PROGRAMMING_SOLVER_CG_SOLVER_H
#define GOC_LINEAR_PROGRAMMING_SOLVER_CG_SOLVER_H

#include <iostream>
#include <unordered_set>
#include <vector>

#include "goc/lib/json.hpp"
#include "goc/linear_programming/colgen/colgen.h"
#include "goc/linear_programming/model/formulation.h"
#include "goc/linear_programming/solver/lp_solver.h"
#include "goc/log/cg_execution_log.h"
#include "goc/time/duration.h"

namespace nlh = nlohmann;

namespace goc
{
// Class representing a solver for column generation. Its purpose is to abstract the
// specific solver implementations from the algorithms.
// The solver has the following parameters:
//	screen_output: pointer to the stream where the output of the algorithm should go. (nullptr for no output).
//	time_limit: maximum time to spend solving the colgen.
//	lp_solver: pointer to a solver for the linear relaxation of the lp.
//	pricing_function: 	a function that receives the current lp iteration and adds variables to the lp.
//						the column generation will continue as long as the pricing function adds variables
//						(or constraints) to the lp at a given iteration.
class CGSolver
{
public:
	std::ostream* screen_output;
	Duration time_limit;
	goc::LPSolver* lp_solver;
	PricingFunction pricing_function;
	
	// Creates a default column generation solver (no output, time_limit=2hs, lp_solver=CPLEX,
	// 	pricing_function=DONOTHING).
	CGSolver();
	
	// Solves the formulation using a column generation procedure.
	// Returns: the execution log with the specified options.
	// Precondition: the formulation must have been created with the NewFormulation() method.
	CGExecutionLog Solve(Formulation* formulation, const std::unordered_set<CGOption>& options) const;
	
	// Returns: a formulation compatible with the solver.
	static Formulation* NewFormulation();
};
} // namespace goc

#endif //GOC_LINEAR_PROGRAMMING_SOLVER_CG_SOLVER_H
