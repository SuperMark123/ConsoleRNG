#pragma once
#ifndef RNG_h
#define RNG_h

#include <memory>
#include <vector>

class ContinuousDistribution;
class BisectionSolver;

class RNG
{
public:
	RNG() = delete;
	RNG(std::shared_ptr<ContinuousDistribution> dist, std::size_t accracyLevel);
	RNG(std::shared_ptr<ContinuousDistribution> dist, std::shared_ptr<BisectionSolver> solver, std::size_t accracyLevel, std::size_t seed);
	~RNG();

	std::shared_ptr<std::vector<double>> generate(std::size_t n) const;

private:
	std::size_t generateRandInt() const;

	std::shared_ptr<ContinuousDistribution> mDist;
	double mAccuracy;
	std::size_t mSeed;
	std::shared_ptr<BisectionSolver> mSolver;
};

#endif