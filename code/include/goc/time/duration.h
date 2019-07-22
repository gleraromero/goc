//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_TIME_DURATION_H
#define GOC_TIME_DURATION_H

#include <iostream>

#include "goc/lib/json.hpp"
#include "goc/print/printable.h"

namespace goc
{
enum class DurationUnit
{
	Milliseconds, Seconds, Minutes, Hours
};

// Represents a time span (e.g. 2 minutes, 100 milliseconds).
class Duration : public Printable
{
public:
	// Returns: the empty duration (i.e. 0 seconds).
	static Duration None();
	
	// Returns: a duration which is bigger than all durations.
	static Duration Max();
	
	// Returns: the empty duration (None).
	Duration();
	
	// Returns: the duration with 'amount' 'unit'.
	// Example: Duration(2, DurationUnit::Seconds) == "2 seconds".
	Duration(double amount, DurationUnit unit);
	
	// Returns: the amount of time in the unit specified.
	double Amount(DurationUnit unit) const;
	
	Duration operator+(const Duration& d) const;
	
	Duration operator-(const Duration& d) const;
	
	bool operator<(const Duration& d) const;
	
	bool operator>(const Duration& d) const;
	
	bool operator<=(const Duration& d) const;
	
	bool operator>=(const Duration& d) const;
	
	bool operator==(const Duration& d) const;
	
	bool operator!=(const Duration& d) const;
	
	Duration& operator+=(const Duration& d);
	
	Duration& operator-=(const Duration& d);
	
	// Prints the duration in seconds in the os stream.
	// Format: 'amount_in_seconds'.
	// Example: 20.
	virtual void Print(std::ostream& os) const;

private:
	// Convert the duration which is in milliseconds to the unit 'to_unit'.
	double ConvertFromMilliseconds(double duration_ms, DurationUnit to_unit) const;
	
	// Convert the duration which is in 'from_unit' to the unit milliseconds.
	double ConvertToMilliseconds(double duration, DurationUnit from_unit) const;
	
	double amount_in_ms_; // the duration amount in milliseconds.
};

// Serialize duration.
// Format: 	amount_in_secs.
void to_json(nlohmann::json& j, const Duration& d);
} // namespace goc

namespace std
{
// Returns: the minimum of two durations.
inline goc::Duration min(const goc::Duration& d1, const goc::Duration& d2)
{
	return d1 <= d2 ? d1 : d2;
}

// Returns: the maximum of two durations.
inline goc::Duration max(const goc::Duration& d1, const goc::Duration& d2)
{
	return d1 >= d2 ? d1 : d2;
}
} // namespace std

#endif //GOC_TIME_DURATION_H
