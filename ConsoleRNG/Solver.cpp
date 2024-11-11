#include "Solver.h"
#include <iostream>

BisectionSolver::BisectionSolver(double tolerance) : 
	mTol{ tolerance }
{
}

BisectionSolver::~BisectionSolver()
{
}

std::pair<double, double> BisectionSolver::findBracket(const RealFunction& func, double target) const
{
	// Assume the input function is an monotonic increasing function and has real domain. 
	// One can negate the function outside of the object if the function is monotonic decreasing.
	double lower = -10;
	double upper = 10;

	//std::cout << func(lower) << std::endl;
	while (func(lower) - target > 0)
	{
		lower *= 2;
	}

	//std::cout << func(upper) << std::endl;
	while (func(upper) - target < 0)
	{
		upper *= 2;
	}
	return std::make_pair(lower, upper);
}

double BisectionSolver::solve(const RealFunction& func, double target) const
{
	std::pair<double, double> boundary = findBracket(func, target);
	double mid = boundary.first * 0.5 + boundary.second * 0.5;
	
	while (std::abs(boundary.first - boundary.second) > mTol)
	{
		if (func(mid) > target)
			boundary.second = mid;
		else if (func(mid) < target)
			boundary.first = mid;
		else
			return mid;

		mid = boundary.first * 0.5 + boundary.second * 0.5;
	}
	
	// Debug print
	// std::cout << "(" << func(mid) << ", " << target << ") \n";

	return mid;
}