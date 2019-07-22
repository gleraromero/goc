//
// Created by Gonzalo Lera Romero.
// Grupo de Optimizacion Combinatoria (GOC).
// Departamento de Computacion - Universidad de Buenos Aires.
//

#ifndef GOC_BASE_MAYBE_H
#define GOC_BASE_MAYBE_H

namespace goc
{
// Represents an object that may have a value or not. It is a more declarative way to do this instead of nullptr.
template<typename T>
class Maybe
{
public:
	// Creates a Maybe value which is not set.
	Maybe()
	{
		is_set_ = false;
		value_ = nullptr;
	}
	
	Maybe(const T& value)
		: Maybe(value, true)
	{
	
	}
	
	// Creates an object with the underlying value and which may or not be set.
	Maybe(const T& value, bool is_set)
	{
		value_ = new T(value);
		is_set_ = is_set;
	}
	
	~Maybe()
	{
		if (value_) delete value_;
	}
	
	operator T&()
	{
		return *value_;
	}
	
	operator T() const
	{
		return *value_;
	}
	
	// Returns: if the value is set.
	bool IsSet() const
	{
		return is_set_;
	}
	
	// Unsets the Maybe value.
	void Unset()
	{
		is_set_ = false;
	}
	
	// Sets the value to the maybe.
	void Set(const T& value)
	{
		if (!value_) value_ = new T(value);
		else *value_ = value;
		is_set_ = true;
	}
	
	// Returns: a pointer to the value.
	// Precondition: IsSet().
	T* operator->()
	{
		return &*value_;
	}
	
	// Returns: a pointer to the value.
	// Precondition: IsSet().
	const T* operator->() const
	{
		return &*value_;
	}
	
	// Returns: a reference to the value.
	T& operator*()
	{
		return *value_;
	}
	
	// Returns: a reference to the value.
	const T& operator*() const
	{
		return *value_;
	}
	
	// Returns: the value.
	// Precondition: IsSet() == true.
	const T& Value() const
	{
		return *value_;
	}
	
	// Returns: the value.
	// Precondition: IsSet() == true.
	T& Value()
	{
		return *value_;
	}
	
	// Assigns the value to the maybe.
	// Returns: a reference to the just assigned value.
	T& operator=(const T& value)
	{
		Set(value);
		return *value_;
	}
	
	// Assigns the value to the maybe.
	// Returns: a reference to the just assigned value.
	Maybe<T>& operator=(const Maybe<T>& maybe)
	{
		is_set_ = maybe.is_set_;
		Set(maybe.Value());
		return *this;
	}
	
private:
	bool is_set_ = false;
	T* value_;
};
} // namespace goc

#endif //GOC_BASE_MAYBE_H
