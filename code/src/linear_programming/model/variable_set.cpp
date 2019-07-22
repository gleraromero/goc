//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "goc/linear_programming/model/variable_set.h"

#include "goc/collection/collection_utils.h"
#include "goc/exception/exception_utils.h"
#include "goc/string/string_utils.h"

using namespace std;

namespace goc
{

VariableSet::VariableSet(const string& name) : name_(name)
{ }

const string& VariableSet::Name() const
{
	return name_;
}

const void VariableSet::Add(int i1, const Variable& v)
{
	stretch_to_size(one_index_variables_, i1+1, Maybe<Variable>());
	one_index_variables_[i1] = v;
}

const void VariableSet::Add(int i1, int i2, const Variable& v)
{
	stretch_to_size(two_index_variables_, i1+1, vector<Maybe<Variable>>());
	stretch_to_size(two_index_variables_[i1], i2+1, Maybe<Variable>());
	two_index_variables_[i1][i2] = v;
}

const void VariableSet::Add(int i1, int i2, int i3, const Variable& v)
{
	stretch_to_size(three_index_variables_, i1+1, vector<vector<Maybe<Variable>>>());
	stretch_to_size(three_index_variables_[i1], i2+1, vector<Maybe<Variable>>());
	stretch_to_size(three_index_variables_[i1][i2], i3+1, Maybe<Variable>());
	three_index_variables_[i1][i2][i3] = v;
}

bool VariableSet::Includes(int i1) const
{
	return one_index_variables_.size() > i1 && one_index_variables_[i1].IsSet();
}

bool VariableSet::Includes(int i1, int i2) const
{
	return two_index_variables_.size() > i1 && two_index_variables_[i1].size() > i2 &&
		   two_index_variables_[i1][i2].IsSet();
}

bool VariableSet::Includes(int i1, int i2, int i3) const
{
	return three_index_variables_.size() > i1 && three_index_variables_[i1].size() > i2 &&
		   three_index_variables_[i1][i2].size() > i3 && three_index_variables_[i1][i2][i3].IsSet();
}

const Variable& VariableSet::operator()(int i1) const
{
	if (!Includes(i1))
	{
		fail("Variable " + STR(name_) + "_" + STR(i1) + " not included in variable set");
	}
	return *one_index_variables_[i1];
}

const Variable& VariableSet::operator()(int i1, int i2) const
{
	if (!Includes(i1, i2))
	{
		fail("Variable " + STR(name_) + "_" + STR(i1) + "_" + STR(i2) + " not included in variable set");
	}
	return *two_index_variables_[i1][i2];
}

const Variable& VariableSet::operator()(int i1, int i2, int i3) const
{
	if (!Includes(i1, i2, i3))
	{
		fail("Variable " + STR(name_) + "_" + STR(i1) + "_" + STR(i2) + "_" + STR(i3) + " not included in variable set");
	}
	return *three_index_variables_[i1][i2][i3];
}

const Variable& VariableSet::operator()(const Arc& e) const
{
	return this->operator()(e.tail, e.head);
}

const Variable& VariableSet::operator()(const Arc& e, int i3) const
{
	return this->operator()(e.tail, e.head, i3);
}
} // namespace goc