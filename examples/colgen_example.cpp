//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//
#include <bitset>
#include <iostream>
#include <vector>

#include "goc/goc.h"

using namespace std;
using namespace goc;

// Define the bitset of vertices with N=maximum number of vertices.
#define MAX_N 11
typedef bitset<MAX_N> VertexSet;

// Solves a maximum weight independent set problem.
//	n: number of vertices in the graph.
//	e: edges of the graph.
//	w[i]: weight of the vertex i.
//	W: output parameter with the solution value.
//	S: output parameter with the solution.
// Returns: the execution log.
BCExecutionLog maximum_weight_independent_set(const Graph& G, const vector<double>& w, double* W, VertexSet* S)
{
	// Create formulation:
	// max w[i] x[i]
	// s.t. x[i]+x[j] <= 1, forall (i, j) \in E.
	//	x[i] \in {0, 1}.
	Formulation* f = BCSolver::NewFormulation();
	vector<Variable> x;
	for (int i = 0; i < G.VertexCount(); ++i) x.push_back(f->AddVariable("x_" + STR(i), VariableDomain::Binary, 0.0, 1.0));
	f->Maximize(ESUM(i:G.Vertices(), w[i] * x[i]));
	for (Edge e: G.Edges()) f->AddConstraint((x[e.tail] + x[e.head]).LEQ(1.0));
	
	// Solve with a BC solver.
	BCSolver solver;
	auto log = solver.Solve(f, {BCOption::BestIntSolution});
	if (log.status != BCStatus::Optimum) fail("BC did not finish correctly.");
	
	// Get the solution.
	*W = log.best_int_value;
	Valuation& x_star = log.best_int_solution;
	for (Vertex i: G.Vertices()) if (epsilon_equal(x_star[x[i]], 1.0)) S->set(i);
	delete f;
	return log;
}

// Initializes a formulation with the empty set partitioning formulation:
//	min {empty expression}
// 	s.t.
//	{empty expression} == 1.0, forall i \in V
//
// n: number of vertices in the graph.
// rmp: formulation that will contain the restricted master problem.
void init_set_partitioning_formulation(int n, Formulation* rmp)
{
	rmp->Minimize(Expression());
	for (int i = 0; i < n; ++i) rmp->AddConstraint(Expression().EQ(1.0));
}

// Adds the independent set S to the rmp.
// n: number of vertices in the graph.
// rmp: restricted master problem.
// S: set to add to the formulation.
// y: vector of set variables.
// I: vector of independent sets associated with variables y.
void add_independent_set(int n, Formulation* rmp, const VertexSet& S, vector<Variable>* y, vector<VertexSet>* I)
{
	int j = y->size();
	I->push_back(S);
	y->push_back(rmp->AddVariable("y_" + STR(j), VariableDomain::Real, 0.0, INFTY));
	rmp->SetObjectiveCoefficient((*y)[j], 1.0);
	for (int i = 0; i < n; ++i) if (S.test(i)) rmp->SetConstraintCoefficient(i, (*y)[j], 1.0);
}

// In this example we get an upper bound on the Vertex-Coloring Problem using column generation.
// Let G=<V, E> be a graph, the formulation (SPF) used is:
//	min \sum_{j \in I} y_j								(0)
//	st. \sum_{i \in I} a_ij y_j = 1, \forall i \in V	(1)
//		y_j \in {0, 1} 				 \forall j \in I 	(2)
//
//	I: the set of all independent sets of G.
//	y_j: if the set of vertices j is painted with the same colour.
//	a_ij: if the set of vertics j includes vertex i.
// The input is as follows:
// - D=<V, E> (with n=11 vertices).
// - E = {	(1,2), (1,4), (1,7), (1,9), (2,3), (2,6), (2,8), (3,5), (3,7), (3,10), (4,5), (4,6), (4,10), (5,8), (5,9),
// 			(6,11), (7,11), (8,11), (9,11), (10,11) }
// The output should be: 3.0.
int main()
{
	// Define problem.
	int n = 11;
	Graph G(n);
	G.AddEdges({{0,1}, {0,3}, {0,6}, {0,8}, {1,2}, {1,5}, {1,7}, {1,4}, {1,6}, {2,9}, {3,4}, {3,5}, {3,9}, {4,7},
				{4,8}, {5,10}, {6,10}, {7,10}, {8,10}, {9,10}});
	auto& V = G.Vertices();
	auto& E = G.Edges();
	
	// Create formulation.
	Formulation* rmp = LPSolver::NewFormulation();
	vector<Variable> y;
	vector<VertexSet> I;
	init_set_partitioning_formulation(n, rmp);
	for (int i = 0; i < n; ++i) add_independent_set(n, rmp, create_bitset<MAX_N>({i}), &y, &I); // Add the trivial independent sets.
	CGSolver cg_solver;
	LPSolver lp_solver;
	cg_solver.time_limit = 2.0_hr;
	cg_solver.screen_output = &clog;
	cg_solver.lp_solver = &lp_solver;
	cg_solver.pricing_function = [&] (const vector<double>& duals, double incumbent_value, Duration time_limit,
									  CGExecutionLog* execution_log) {
		// Solve a maximum weight independent set problem.
		double W;
		VertexSet S;
		auto pricing_log = maximum_weight_independent_set(G, duals, &W, &S);
		if (epsilon_bigger(W, 1.0)) add_independent_set(n, rmp, S, &y, &I);
		execution_log->iterations->push_back(pricing_log);
	};
	
	clog << "Restricted Master Problem" << endl;
	clog << *rmp << endl;
	
	// Solving CG.
	clog << "Solving CG" << endl;
	auto execution_log = cg_solver.Solve(rmp, {CGOption::ScreenOutput, CGOption::IterationsInformation});

	// Parse result.
	if (execution_log.status == CGStatus::Optimum)
	{
		clog << "Best bound: " << execution_log.incumbent_value << endl;
		for (int i = 0; i < y.size(); ++i)
			if (epsilon_bigger(execution_log.incumbent->at(y[i]), 0.0))
				clog << I[i] << ": " << execution_log.incumbent->at(y[i]) << endl;
				
	}
	cout << execution_log << endl;
	return 0;
}