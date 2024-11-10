#include <cmath>
#include <random>
#include "RNG.h"
#include "Distribution.h"
#include <limits>
#include <iostream>
#include <bitset>

RNG::RNG(std::shared_ptr<ContinuousDistribution> dist, std::size_t accuracyLevel) :
	mDist{ dist },
	mAccuracy{ 1 / std::pow(10.0, accuracyLevel) },
	mSeed{ 123 }
{
}

RNG::RNG(std::shared_ptr<ContinuousDistribution> dist, std::shared_ptr<BisectionSolver> solver, std::size_t accuracyLevel, std::size_t seed) :
	mDist{ dist },
	mSolver{ solver },
	mAccuracy{ 1 / std::pow(10.0, accuracyLevel)},
	mSeed{ seed }
{
}

RNG::~RNG()
{
}

std::size_t RNG::generateRandInt() const
{
	std::random_device rd{};
	std::mt19937 rnd{ rd() };
	std::bernoulli_distribution dist{ 0.5 };

	std::size_t rNum = 0;
	for (std::size_t i = 0; i < sizeof(std::size_t) * 8; ++i)
	{
		rNum = rNum << 1;
		// Debug print num to bitset
		//std::cout << std::bitset<64>(rNum) << std::endl;
		if (dist(rnd))
			rNum += 1;
	}

	// Debug print random num
	//std::cout << rNum << std::endl;

	return rNum;
}

std::shared_ptr<std::vector<double>> RNG::generate(std::size_t n) const
{
	std::shared_ptr<std::vector<double>> randNums = std::make_shared<std::vector<double>>(n);
	for (std::size_t i = 0; i < n; ++i)
	{
		double prob = 1.0 * generateRandInt() / SIZE_MAX;
		prob = std::round(prob / mAccuracy) * mAccuracy;
		// Debug print probability
		// std::cout << prob << ", ";
		double res = mDist->invCDF(prob, *mSolver);
		(*randNums)[i] = std::round(res / mAccuracy) * mAccuracy;
	}

	// Debug print EOL
	//std::cout << std::endl;

	return randNums;
}