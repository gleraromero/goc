//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//
#include <iostream>
#include <vector>

#include "goc/goc.h"

using namespace std;
using namespace goc;

// In this example we create a LP problem to solve a fractional knapsack problem.
// The input is as follows:
// - Capacity (W): 50.
// - Item weights (w): 	[10, 20 , 30].
// - Item values (v): 	[60, 100, 120].
// The output should be: 240 by taking items (1) and (2) and 2/3rd of the third item.
int main()
{
	// Define problem.
	double W = 50.0;
	vector<double> w = {10.0, 20.0, 30.0};
	vector<double> v = {60.0, 100.0, 120.0};
	int n = 3;
	
	// Create formulation.
	Formulation* f = LPSolver::NewFormulation();
	vector<Variable> x;
	x.push_back(f->AddVariable("x0"));
	x.push_back(f->AddVariable("x1"));
	x.push_back(f->AddVariable("x2"));
	f->Maximize(v[0]*x[0]+v[1]*x[1]+v[2]*x[2]);
	f->AddConstraint((w[0]*x[0]+w[1]*x[1]+w[2]*x[2]).LEQ(W));
	for (auto& xi: x) f->SetVariableDomain(xi, VariableDomain::Real);
	for (auto& xi: x) f->SetVariableBound(xi, 0.0, 1.0);
	
	clog << "Formulation:" << endl;
	clog << *f << endl << endl;
	
	// Solve formulation.
	LPSolver solver;
	solver.time_limit = 2.0_hr;
	solver.screen_output = &clog;
	clog << "Solving LP" << endl;
	auto execution_log = solver.Solve(f, {LPOption::ScreenOutput, LPOption::Incumbent, LPOption::Duals});
	clog << endl;
	
	// Parse result.
	if (execution_log.status == LPStatus::Optimum)
	{
		clog << "Best value: " << execution_log.incumbent_value << endl;
		Valuation& x_star = execution_log.incumbent;
		for (int i = 0; i < n; ++i) clog << "Take " << x_star[x[i]] << " of item " << i << endl;
	}
	cout << execution_log << endl;
	return 0;
}