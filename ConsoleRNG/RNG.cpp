#include <cmath>
#include <random>
#include "RNG.h"
#include "Distribution.h"
#include <limits>
#include <iostream>
#include <bitset>

RNG::RNG(std::shared_ptr<ContinuousDistribution> dist, std::size_t accuracyLevel) :
	mDist{ dist },
	mAccuracy{ 1 / std::pow(10.0, accuracyLevel) }
{
}

RNG::RNG(std::shared_ptr<ContinuousDistribution> dist, std::shared_ptr<BisectionSolver> solver, std::size_t accuracyLevel) :
	mDist{ dist },
	mSolver{ solver },
	mAccuracy{ 1 / std::pow(10.0, accuracyLevel)}
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
	// Use binary random number to generate each bit of a 64bit unsigned int
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
	std::size_t segmentLen = std::floor(SIZE_MAX / (1/mAccuracy + 1));
	for (std::size_t i = 0; i < n; ++i)
	{
		/*
		* Use the generated random unsigned int to compute the random probability.
		* There will be 10^AccuracyLevel + 1 probabilities to be chosen. 
		* To make all probabilities equally possible, all possible random integers are equally divided into 10^accuracy + 1 segments.
		* Assign (i-1)*mAccuracy to prob if the random int lies between [(i-1)*segmentLen, i*segmentLen).
		* If the random int is greater than (10^AccuracyLevel + 1) * segmentLen, regenerate the random int once again.
		* Given that SIZE_MAX mod (1/mAccuracy + 1) <= 1/mAccuracy and generateRandInt() mod segmentLen < segmentLen,
		* the probability of regenerating the random int is less than 2 * segmentLen / SIZE_MAX <= 2 * mAccuracy (accuracyLevel <= 5)
		*/
		double prob = mAccuracy * std::floor(1.0 * generateRandInt() / segmentLen);
		while (prob > 1)
		{
			prob = mAccuracy * std::floor(generateRandInt() / segmentLen);
		}
		// Debug print probability
		//std::cout << prob << ", ";
		double res = mDist->invCDF(prob, *mSolver);
		(*randNums)[i] = std::round(res / mAccuracy) * mAccuracy;
	}

	// Debug print EOL
	//std::cout << std::endl;

	return randNums;
}