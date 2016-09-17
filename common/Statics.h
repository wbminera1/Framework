#ifndef STATICS_H_
#define STATICS_H_

namespace statics
{
	template <typename T>
	constexpr T Min(T a, T b)
	{
		return a > b ? b : a;
	}
	
	template <typename T>
	constexpr T Max(T a, T b)
	{
		return a < b ? b : a;
	}
}

#endif // STATICS_H_