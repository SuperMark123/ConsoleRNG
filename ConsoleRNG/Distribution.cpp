#include <cmath>
#include <stdexcept>
#include "Distribution.h"
#include "Solver.h"

namespace
{
	// Helper function for the CDF of Chi-Square distribution
	double incompleteLowerGamma(double a, double x) {
		if (a <= 0) {
			throw std::invalid_argument("Invalid input: a must be positive and x must be non-negative.");
		}
		if (x <= 0)
		{
			// To adapt to BisectionSolver 
			return 0;
		}

		auto integrand = [a](double t) { return std::pow(t, a - 1) * std::exp(-t); };

		int numIntervals = 10000; // Adjust for desired accuracy
		double h = x / numIntervals;
		double sum = 0.5 * (integrand(0) + integrand(x));

		// Do integration
		for (int i = 1; i < numIntervals; ++i) {
			sum += integrand(i * h);
		}

		return h * sum;
	}
}

Distribution::Distribution()
{
}

Distribution::~Distribution()
{
}

/*
* Start definition for ContinuousDistribution
*/
ContinuousDistribution::ContinuousDistribution() :
	Distribution{}
{
}

ContinuousDistribution::~ContinuousDistribution()
{
}

double ContinuousDistribution::invCDF(double prob, const BisectionSolver& solver) const
{
	if (prob < 0 || prob > 1)
		return std::nan("InvalidInput");

	RealFunction func = [&](double x) { return this->cdf(x); };
	return solver.solve(func, prob);
}

/*
* Starting definition for Uniform Distribution
*/
UniformDistribution::UniformDistribution() : 
	ContinuousDistribution{},
	mLb{ 0 },
	mRb{ 1 }
{
}

UniformDistribution::UniformDistribution(double lb, double rb) :
	ContinuousDistribution{},
	mLb{ lb },
	mRb{ rb }
{
}

UniformDistribution::UniformDistribution(const UniformDistribution& dist) :
	ContinuousDistribution{},
	mLb{ dist.mLb },
	mRb{ dist.mRb }
{
}

const UniformDistribution& UniformDistribution::operator = (const UniformDistribution& dist)
{
	// Prevent self assignemnt
	if (this == &dist)
		return *this;

	mLb = dist.mLb;
	mRb = dist.mRb;
	return *this;
}

UniformDistribution::~UniformDistribution()
{
}

double UniformDistribution::pdf(double x) const
{
	if (x >= mLb && x <= mRb)
		return 1 / (mRb - mLb);
	else
		return 0;
}

double UniformDistribution::cdf(double x) const
{
	if (x >= mLb && x <= mRb)
		return (x - mLb) / (mRb - mLb);
	else if (x < mLb)
		return 0;
	else
		return 1;
}

double UniformDistribution::invCDF(double prob, const BisectionSolver& solver) const
{
	(void)solver; // To remove unused variable error/warning in some compiler. Can be done by declaring variables with [[maybe_unused]] if using C++17
	if (prob < 0 || prob > 1)
		return std::nan("InvalidInput");

	return mLb + prob * (mRb - mLb);;
}

/*
* Starting definition for Normal Distribution
*/
NormalDistribution::NormalDistribution() :
	ContinuousDistribution{},
	mMu{ 0 },
	mSigma{ 1 }
{
}

NormalDistribution::NormalDistribution(double mu, double sigma) :
	ContinuousDistribution{},
	mMu{ mu },
	mSigma{ sigma }
{
}

NormalDistribution::NormalDistribution(const NormalDistribution& dist) :
	ContinuousDistribution{},
	mMu{ dist.mMu },
	mSigma{ dist.mSigma }
{
}

const NormalDistribution& NormalDistribution::operator = (const NormalDistribution& dist)
{
	// Prevent self assignemnt
	if (this == &dist)
		return *this;

	mMu = dist.mMu;
	mSigma = dist.mSigma;
	return *this;
}

NormalDistribution::~NormalDistribution()
{
}

double NormalDistribution::pdf(double x) const
{
	double coef = 1 / std::sqrt(2 * M_PI) / mSigma;
	double z = (x - mMu) / mSigma;

	return coef * std::exp(-0.5 * std::pow(z, 2));
}

double NormalDistribution::cdf(double x) const
{
	double z = (x - mMu) / mSigma;
	return 0.5 * std::erfc(-z / std::sqrt(2));
}

/*
* Starting definition for ChiSquare Distribution
*/
ChiSquareDistribution::ChiSquareDistribution() :
	ContinuousDistribution{},
	mDoF{ 1 }
{
}

ChiSquareDistribution::ChiSquareDistribution(double dof) :
	ContinuousDistribution{},
	mDoF{ dof }
{
}

ChiSquareDistribution::ChiSquareDistribution(const ChiSquareDistribution& dist) :
	ContinuousDistribution{},
	mDoF{ dist.mDoF }
{
}

const ChiSquareDistribution& ChiSquareDistribution::operator = (const ChiSquareDistribution& dist)
{
	// Prevent self assignemnt
	if (this == &dist)
		return *this;

	mDoF = dist.mDoF;
	return *this;
}

ChiSquareDistribution::~ChiSquareDistribution()
{
}

double ChiSquareDistribution::pdf(double x) const
{
	if (x <= 0)
		return 0;
	double numer = std::pow(x, mDoF/2 - 1) * std::exp(-x/2);
	double denom = std::pow(x, mDoF / 2) * std::tgamma(mDoF / 2);
	return numer / denom;
}

double ChiSquareDistribution::cdf(double x) const
{
	double numer = incompleteLowerGamma(mDoF / 2, x / 2);
	double denom = std::tgamma(mDoF / 2);
	return numer / denom;
}

/*
* Starting definition for Exponential Distribution
*/
ExponentialDistribution::ExponentialDistribution() :
	ContinuousDistribution{},
	mLambda{ 1 }
{
}

ExponentialDistribution::ExponentialDistribution(double lambda) :
	ContinuousDistribution{},
	mLambda{ lambda }
{
}

ExponentialDistribution::ExponentialDistribution(const ExponentialDistribution& dist) :
	ContinuousDistribution{},
	mLambda{ dist.mLambda }
{
}

const ExponentialDistribution& ExponentialDistribution::operator = (const ExponentialDistribution& dist)
{
	// Prevent self assignemnt
	if (this == &dist)
		return *this;

	mLambda = dist.mLambda;
	return *this;
}

ExponentialDistribution::~ExponentialDistribution()
{
}

double ExponentialDistribution::pdf(double x) const
{
	return mLambda * std::exp(-mLambda * x);
}

double ExponentialDistribution::cdf(double x) const
{
	return 1 - std::exp(-mLambda * x);
}

double ExponentialDistribution::invCDF(double prob, const BisectionSolver& solver) const
{
	(void)solver; // To remove unused variable error/warning in some compiler. Can be done by declaring variables with [[maybe_unused]] if using C++17
	return std::log(1 - prob) / -mLambda;
}