//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "goc/log/bc_execution_log.h"

#include "goc/collection/collection_utils.h"
#include "goc/string/string_utils.h"

using namespace std;
using namespace nlohmann;

namespace goc
{
BCExecutionLog::BCExecutionLog()
{
	screen_output = "";
	time = 0.0_sec;
	status = BCStatus::DidNotStart;
	constraint_count = variable_count = nodes_open = nodes_closed = 0;
	root_lp_value = root_int_value = best_bound = best_int_value = 0.0;
	cut_count = 0;
	cut_time = 0.0_sec;
}

json BCExecutionLog::ToJSON() const
{
	json j;
	j["kd_type"] = "bc";
	j["screen_output"] = screen_output;
	j["time"] = time.Amount(DurationUnit::Seconds);
	j["status"] = STR(status);
	j["constraint_count"] = constraint_count;
	j["variable_count"] = variable_count;
	j["nodes_open"] = nodes_open;
	j["nodes_closed"] = nodes_closed;
	j["root_lp_value"] = root_lp_value;
	j["root_int_solution"] = root_int_solution;
	j["root_int_value"] = root_int_value;
	j["best_bound"] = best_bound;
	j["best_int_solution"] = best_int_solution;
	j["best_int_value"] = best_int_value;
	j["cut_count"] = cut_count;
	j["cut_time"] = cut_time;
	if (!cut_families.empty())
	{
		j["cut_families"] = vector<json>();
		for (auto& family: cut_families)
		{
			json cut_family_json;
			cut_family_json["name"] = family;
			if (includes_key(cut_family_cut_count, family))
				cut_family_json["cut_count"] = cut_family_cut_count.at(family);
			if (includes_key(cut_family_iteration_count, family))
				cut_family_json["cut_iterations"] = cut_family_iteration_count.at(family);
			if (includes_key(cut_family_cut_time, family))
				cut_family_json["cut_time"] = cut_family_cut_time.at(family);
			j["cut_families"].push_back(cut_family_json);
		}
	}
	return j;
}

ostream& operator<<(ostream& os, BCStatus status)
{
	unordered_map<BCStatus, string> mapper = {{BCStatus::DidNotStart, "DidNotStart"},
											   {BCStatus::Infeasible, "Infeasible"},
											   {BCStatus::Unbounded, "Unbounded"},
											   {BCStatus::TimeLimitReached, "TimeLimitReached"},
											   {BCStatus::MemoryLimitReached, "MemoryLimitReached"},
											   {BCStatus::Optimum, "Optimum"},
											   {BCStatus::NodeLimitReached, "NodeLimitReached"}};
	return os << mapper[status];
}
} // namespace goc