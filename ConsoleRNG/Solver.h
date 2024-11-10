#ifndef Solver_h
#define Solver_h

#include <functional>

using RealFunction = std::function<double(double)>;

/*
* Naive bisection solver
*/
class BisectionSolver
{
public:
	BisectionSolver() = delete;
	BisectionSolver(double tolerance);
	~BisectionSolver();

	std::pair<double, double> findBracket(const RealFunction& func, double target) const;
	double solve(const RealFunction& func, double target) const;

private:
	double mTol;
};

#endif