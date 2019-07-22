//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef BURO_LINEAR_PROGRAMMING_VARIABLE_SET_H
#define BURO_LINEAR_PROGRAMMING_VARIABLE_SET_H

#include <string>
#include <vector>

#include "goc/base/maybe.h"
#include "goc/graph/arc.h"
#include "goc/linear_programming/model/variable.h"

namespace goc
{
// Represents a set of variables.
// Example: x_{ij}.
// In this case, the name of the variable set is "x", and it has two indices.
class VariableSet
{
public:
	// Creates the variable set with the parameter 'name'.
	VariableSet(const std::string& name);
	
	// Returns: Name of the variable set.
	const std::string& Name() const;
	
	// Adds a variable to the x_{i1} position.
	const void Add(int i1, const Variable& v);
	
	// Adds a variable to the x_{i1,i2} position.
	const void Add(int i1, int i2, const Variable& v);
	
	// Adds a variable to the x_{i1,i2,i3} position.
	const void Add(int i1, int i2, int i3, const Variable& v);
	
	// Returns: if variable x_{i1} exists.
	bool Includes(int i1) const;
	
	// Returns: if variable x_{i1,i2} exists.
	bool Includes(int i1, int i2) const;
	
	// Returns: if variable x_{i1,i2,i3} exists.
	bool Includes(int i1, int i2, int i3) const;
	
	// Returns: variable x_{i1}.
	// Exception: if x_{i1} does not exist.
	const Variable& operator()(int i1) const;
	
	// Returns: variable x_{i1, i2}.
	// Exception: if x_{i1, i2} does not exist.
	const Variable& operator()(int i1, int i2) const;
	
	// Returns: variable x_{i1, i2, i3}.
	// Exception: if x_{i1, i2, i3} does not exist.
	const Variable& operator()(int i1, int i2, int i3) const;
	
	// Returns: variable x_{e.tail, e.head}.
	// Exception: if x_{e.tail, e.head} does not exist.
	const Variable& operator()(const Arc& e) const;
	
	// Returns: variable x_{e.tail, e.head, i3}.
	// Exception: if x_{e.tail, e.head, i3} does not exist.
	const Variable& operator()(const Arc& e, int i3) const;
	
private:
	std::string name_;
	std::vector<Maybe<Variable>> one_index_variables_;
	std::vector<std::vector<Maybe<Variable>>> two_index_variables_;
	std::vector<std::vector<std::vector<Maybe<Variable>>>> three_index_variables_;
};
} // namespace goc

#endif //BURO_LINEAR_PROGRAMMING_VARIABLE_SET_H
