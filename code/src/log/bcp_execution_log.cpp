//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "goc/log/bcp_execution_log.h"

using namespace std;
using namespace nlohmann;

namespace goc
{
BCPExecutionLog::BCPExecutionLog()
{
	root_constraint_count = root_variable_count = final_constraint_count = final_variable_count = 0;
	lp_time = pricing_time = branching_time = 0.0_sec;
}

json BCPExecutionLog::ToJSON() const
{
	json j = BCExecutionLog::ToJSON();
	j["kd_type"] = "bcp";
	j["root_constraint_count"] = root_constraint_count;
	j["root_variable_count"] = root_variable_count;
	j["final_constraint_count"] = final_constraint_count;
	j["final_variable_count"] = final_variable_count;
	j["root_log"] = root_log;
	j["lp_time"] = lp_time;
	j["pricing_time"] = pricing_time;
	j["branching_time"] = branching_time;
	return j;
}
} // namespace goc