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

// This class represents the separation routine for the Odd Hole inequalities.
class OddHoleSeparation : public SeparationRoutine
{
public:
	OddHoleSeparation(const Graph& G, const Matrix<Variable>& x)
		: G(G), x(x)
	{ }
	
	virtual vector<Constraint> Separate(const Valuation& x_star, int node_number, int count_limit,
		double node_bound) const
	{
		vector<Constraint> violated_inequalities;
		// <Code to separate odd hole inequalities>.
		clog << "Odd-hole separation." << endl;
		return violated_inequalities;
	}
	
private:
	Graph G;
	Matrix<Variable> x;
};

// This class represents the separation routine for the Clique inequalities.
class CliqueSeparation : public SeparationRoutine
{
public:
	CliqueSeparation(const Graph& G, const Matrix<Variable>& x)
		: G(G), x(x)
	{ }
	
	virtual vector<Constraint> Separate(const Valuation& x_star, int node_number, int count_limit,
										double node_bound) const
	{
		vector<Constraint> violated_inequalities;
		// <Code to separate clique inequalities>.
		clog << "Clique separation." << endl;
		return violated_inequalities;
	}

private:
	Graph G;
	Matrix<Variable> x;
};

// In this example we create a BC problem to solve a Vertex-Coloring Problem.
// The formulation used is:
//	C = {0..n-1}
//	min \sum_{j \in C} y_j								(0)
//	st. \sum_{j \in C} x_vj = 1, \forall v \in V		(1)
//		x_vj + x_wj <= y_j, \forall vw \in E, j \in C	(2)
//		x_vj, y_j \in {0, 1} \forall v \in V, j \in C.	(3)
// The input is as follows:
// - D=<V, E> (with n=11 vertices).
// - E = {	(0,1), (0,3), (0,6), (0,8), (1,2), (1,5), (1,7), (1,4), (1,6), (2,9), (3,4), (3,5), (3,9), (4,7), (4,8),
// 			(5,10), (6,10), (7,10), (8,10), (9,10) }
// The output should be: colors: {3, 4, 8}, with vertex colors {3 4 8 8 3 3 8 8 8 3 4}.
int main()
{
	// Define problem.
	int n = 11;
	vector<Vertex> C = range(0, n); // Colours.
	Graph G(n);
	G.AddEdges({{0,1}, {0,3}, {0,6}, {0,8}, {1,2}, {1,5}, {1,7}, {1,4}, {1,6}, {2,9}, {3,4}, {3,5}, {3,9}, {4,7},
			 {4,8}, {5,10}, {6,10}, {7,10}, {8,10}, {9,10}});
	auto& V = G.Vertices();
	auto& E = G.Edges();
	
	// Create formulation.
	Formulation* f = BCSolver::NewFormulation();
	vector<Variable> y(n);
	for (int j: C) y[j] = f->AddVariable("y_" + STR(j));
	Matrix<Variable> x(n,n);
	for (int v: V)
		for (int j: C)
			x[v][j] = f->AddVariable("x_" + STR(v) + "_" + STR(j));
				
	f->Minimize(ESUM(j:C, y[j])); // (0)
	for (int v: V) f->AddConstraint(ESUM(j:C, x[v][j]).EQ(1.0)); // (1)
	for (Edge e: E) for (int j: C) f->AddConstraint((1.0*x[e.tail][j]+1.0*x[e.head][j]).LEQ(1.0*y[j])); // (1)
	for (int v: V) for (int j: C) f->SetVariableBound(x[v][j], 0.0, 1.0);
	for (int v: V) for (int j: C) f->SetVariableDomain(x[v][j], VariableDomain::Binary);
	for (int j: C) f->SetVariableBound(y[j], 0.0, 1.0);
	for (int j: C) f->SetVariableDomain(y[j], VariableDomain::Binary);
	
	clog << "Formulation:" << endl;
	clog << *f << endl << endl;

	// Solve formulation.
	BCSolver solver;
	solver.time_limit = 2.0_hr;
	solver.screen_output = &clog;
	solver.config = {{"CPX_PARAM_CUTSFACTOR", 0}}; // Disable CPLEX cuts.
	
	// Add cut families to the cut strategy.
	OddHoleSeparation odd_hole_separation(G, x);
	solver.separation_strategy.AddFamily("odd-hole");
	solver.separation_strategy.SetSeparationRoutine("odd-hole", &odd_hole_separation);
	
	CliqueSeparation clique_separation(G, x);
	solver.separation_strategy.AddFamily("clique");
	solver.separation_strategy.SetSeparationRoutine("clique", &clique_separation);
	
	// Configure limits for the cuts.
	solver.separation_strategy.cut_limit["odd-hole"] = 500; // Add 500 odd-hole inequalities as a maximum.
	solver.separation_strategy.node_limit["clique"] = 10; // Separate clique inequalities in the first 10 nodes.
	solver.separation_strategy.cuts_per_iteration["clique"] = 3; // Add up to 3 clique cuts per iteration.
	solver.separation_strategy.iteration_limit["odd-hole"] = 1; // Only execute 1 iteration of the odd-hole separation.
	solver.separation_strategy.AddDependency("odd-hole", "clique"); // Odd-Hole separation is executed if no Clique inequality was found.
	
	clog << "Solving BC" << endl;
	auto execution_log = solver.Solve(f, {BCOption::ScreenOutput, BCOption::BestIntSolution, BCOption::RootInformation, BCOption::CutInformation});
	clog << endl;

	// Parse result.
	if (execution_log.status == BCStatus::Optimum)
	{
		clog << "Best value: " << execution_log.best_int_value << endl;
		Valuation& z_star = execution_log.best_int_solution;
		clog << "Best solution: ";
		for (int v: V) for (int j: C) if (epsilon_equal(z_star[x[v][j]], 1.0)) clog << j << " ";
		clog << endl;
	}
	cout << execution_log << endl;
	return 0;
}