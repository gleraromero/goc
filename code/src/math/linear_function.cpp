//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#include "goc/math/linear_function.h"

#include <vector>

#include "goc/exception/exception_utils.h"
#include "goc/math/number_utils.h"

using namespace std;
using namespace nlohmann;

namespace goc
{
LinearFunction::LinearFunction(const Point2D& p1, const Point2D& p2)
	: domain(p1.x, p2.x), image(min(p1.y, p2.y), max(p1.y, p2.y))
{
	slope = epsilon_equal(p2.x, p1.x) ? 0.0 : (p2.y - p1.y) / (p2.x - p1.x);
	if (epsilon_equal(slope, 0.0)) slope = 0.0;
	intercept = p2.y - slope * p2.x;
}

double LinearFunction::Value(double x) const
{
	return slope * x + intercept;
}

double LinearFunction::PreValue(double y) const
{
	if (epsilon_equal(slope, 0.0))
	{
		if (epsilon_equal(y, intercept))
		{
			return domain.right;
		}
		fail("Function is not inversible.");
	}
	return (y - intercept) / slope;
}

bool LinearFunction::Intersects(const LinearFunction& f) const
{
	// If both pieces have the same slope, check if they have the same intercept.
	if (epsilon_equal(slope, f.slope)) return epsilon_equal(intercept, f.intercept);
	// Otherwise check if intersection is inside both functions domains.
	double intersection = Intersection(f);
	return domain.Includes(intersection) && f.domain.Includes(intersection);
}

double LinearFunction::Intersection(const LinearFunction& l) const
{
	return (l.intercept - intercept) / (slope - l.slope);
}

void LinearFunction::Print(ostream& os) const
{
	os << "{" << Point2D(domain.left, Value(domain.left)) << "->" << Point2D(domain.right, Value(domain.right)) << "]";
}

bool LinearFunction::operator==(const LinearFunction& f) const
{
	return domain == f.domain && image == f.image && epsilon_equal(slope, f.slope) && epsilon_equal(intercept, f.intercept);
}

bool LinearFunction::operator!=(const LinearFunction& f) const
{
	return !(*this == f);
}

void from_json(const json& j, LinearFunction& f)
{
	f = LinearFunction(j[0], j[1]);
}

void to_json(json& j, const LinearFunction& f)
{
	j = vector<vector<Point2D>>(2);
	j[0].push_back(Point2D(f.domain.left, f.Value(f.domain.left)));
	j[1].push_back(Point2D(f.domain.right, f.Value(f.domain.right)));
}
} // namespace goc