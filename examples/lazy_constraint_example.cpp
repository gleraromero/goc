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

// In this example we create a BC problem to solve a Asymmetric Traveling Salesman Problem.
// The TSP formulation used is:
//	min \sum_{ij \in A} c_ij x_ij
//	st. \sum_{ij \in A} x_ij = 1, \forall i \in V
//	    \sum_{ij \in A} x_ij = 1, \forall j \in V
//		\sum_{ij \in S:T} x_ij >= 1, \forall S \subseteq V \ {0}. (as lazy constraint).
//		x_ij \in {0,1}
// The input is as follows:
// - D = <V, E> (complete digraph with |V| = 4).
// - Distance matrix:
// - 0	20	42	35
// - 20	0	30	34
// - 42	30	0	12
// - 35	34	12	0
// The output should be route: (0, 1, 2, 3, 0) with cost: 97.

class SubtourEliminationConstraint : public SeparationRoutine
{
public:
	SubtourEliminationConstraint(const Graph& G, const unordered_map<Edge, Variable>& x)
		: G(G), x(x)
	{ }
	
	virtual vector<Constraint> Separate(const Valuation& x_star, int node_number, int count_limit,
		double node_bound) const
	{
		// Add to D the support digraph.
		Digraph D(G.VertexCount());
		for (int i: G.Vertices())
			for (int j: G.Neighbours(i))
				if (epsilon_bigger(x_star[x.at({i,j})], 0.0))
					D.AddArc({i,j});
		
		// Solve max-flow with k = {1,2,...,n-1}.
		vector<Constraint> violated;
		for (int k = 1; k < G.VertexCount(); ++k)
		{
			double F;
			STCut ST;
			tie(F, ST) = maxflow_mincut(D, [&](int i, int j) { return x_star[x.at({i,j})]; }, k, 0);
			
			// Flow from k to 0 should be 1.0. Otherwise, there is a subtour.
			// Break it using \sum_{i \in S} \sum_{j \in T} x[i][j] >= 1.0.
			if (epsilon_smaller(F, 0.9))
			{
				violated.push_back(FSUM(i:ST.S, FSUM(j:ST.T, x.at({i,j}))).GEQ(2.0));
				break;
			}
		}
		return violated;
	}
	
private:
	Graph G;
	unordered_map<Edge, Variable> x;
};

int main()
{
	// Define problem.
	int n = 4;
	Graph G = Graph::Complete(n);
	auto& V = G.Vertices();
	auto& E = G.Edges();
	
	Matrix<int> d(n,n,0);
	d[0][1] = 20; d[0][2] = 42; d[0][3] = 35;
	d[1][0] = 20; d[1][2] = 30; d[1][3] = 34;
	d[2][0] = 42; d[2][1] = 30; d[2][3] = 12;
	d[3][0] = 35; d[3][1] = 34; d[3][2] = 12;
	
	// Create formulation.
	Formulation* f = BCSolver::NewFormulation();
	unordered_map<Edge, Variable> x = {};
	for (Edge e: E) x[e] = f->AddVariable("x_" + STR(E), VariableDomain::Binary, 0.0, 1.0);

	SubtourEliminationConstraint sec(G, x); // Subtour elimination constraint separation routine.
	f->Minimize(FSUM(e:E, d[e.tail][e.head] * x[e])); // Objective function.
	for (int i: V) f->AddConstraint(FSUM(e:G.IncidentEdges(i), x[e]).EQ(2.0)); // degree constraints.
	f->AddLazyConstraint(&sec); // Adding SEC as lazy constraint.
	
	clog << "Formulation:" << endl;
	clog << *f << endl << endl;

	// Solve formulation.
	BCSolver solver;
	solver.time_limit = Duration(2, DurationUnit::Hours);
	solver.screen_output = &clog;
	clog << "Solving BC" << endl;
	auto execution_log = solver.Solve(f, {BCOption::ScreenOutput, BCOption::BestIntSolution, BCOption::RootInformation, BCOption::CutInformation});
	clog << endl;

	// Parse result.
	if (execution_log.status == BCStatus::Optimum)
	{
		clog << "Best value: " << execution_log.best_int_value << endl;
		Valuation& x_star = execution_log.best_int_solution;
		clog << "Best solution: 0";
		unordered_set<Vertex> added = {0};
		Vertex last = 0;
		for (int i = 0; i < n; ++i)
		{
			for (Vertex v: V)
			{
				if (!includes(added, v) && epsilon_equal(x_star[x[{last, v}]], 1.0))
				{
					last = v;
					clog << " " << v;
					added.insert(v);
					break;
				}
			}
		}
		clog << " 0" << endl;
	}
	cout << execution_log << endl;
	return 0;
}