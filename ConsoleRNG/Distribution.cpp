#include <cmath>
#include <stdexcept>
#include "Distribution.h"
#include "Solver.h"

namespace
{
	double incompleteLowerGamma(double a, double x) {
		if (a <= 0) {
			throw std::invalid_argument("Invalid input: a must be positive and x must be non-negative.");
		}
		if (x <= 0)
		{
			return 0;
		}

		auto integrand = [a](double t) { return std::pow(t, a - 1) * std::exp(-t); };

		int numIntervals = 10000; // Adjust for desired accuracy
		double h = x / numIntervals;
		double sum = 0.5 * (integrand(0) + integrand(x));

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
	return std::log(1 - prob) / -mLambda;
}