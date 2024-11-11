#pragma once
#ifndef RNG_h
#define RNG_h

#include <memory>
#include <vector>

class ContinuousDistribution;
class BisectionSolver;

/*
* RNG only generates random numbers that follow continous distributions
* Need a separate class to generate discrete random numbers.
* Skipping copy/move constructors and assignment operators. Move semantic will have performance impact for RNG class.
*/
class RNG
{
public:
	RNG() = delete;
	RNG(std::shared_ptr<ContinuousDistribution> dist, std::size_t accracyLevel);
	RNG(std::shared_ptr<ContinuousDistribution> dist, std::shared_ptr<BisectionSolver> solver, std::size_t accracyLevel);
	~RNG();

	std::shared_ptr<std::vector<double>> generate(std::size_t n) const;

private:
	std::size_t generateRandInt() const;

	std::shared_ptr<ContinuousDistribution> mDist;
	double mAccuracy;
	std::shared_ptr<BisectionSolver> mSolver;
};

#endif