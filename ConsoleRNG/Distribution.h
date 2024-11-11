#ifndef Distribution_h
#define Distribution_h

constexpr double M_PI = 3.14159265358979323846;
class BisectionSolver;

/*
* Abstract class Distribution
*/
class Distribution
{
public:
	Distribution();
	virtual ~Distribution() = 0;
};

class ContinuousDistribution : public Distribution
{
public:
	ContinuousDistribution();
	~ContinuousDistribution();

	// Only continous distributions have density functions
	virtual double pdf(double x) const = 0;
	virtual double cdf(double x) const = 0;
	virtual double invCDF(double prob, const BisectionSolver& solver) const;
};

// Skipping the definition of move constructors and assignment operators for concrete distributions. No pointer data members.
class UniformDistribution : public ContinuousDistribution
{
public:
	UniformDistribution();
	UniformDistribution(double lb, double rb);
	UniformDistribution(const UniformDistribution& dist);
	const UniformDistribution& operator = (const UniformDistribution& dist);
	~UniformDistribution();

	double pdf(double x) const override;
	double cdf(double x) const override;
	double invCDF(double prob, const BisectionSolver& solver) const override;

private:
	double mLb;
	double mRb;
};

class NormalDistribution : public ContinuousDistribution
{
public:
	NormalDistribution();
	NormalDistribution(double mu, double sigma);
	NormalDistribution(const NormalDistribution& dist);
	const NormalDistribution& operator = (const NormalDistribution& dist);
	~NormalDistribution();

	double pdf(double x) const override;
	double cdf(double x) const override;

private:
	double mMu;
	double mSigma;
};

class ChiSquareDistribution : public ContinuousDistribution
{
public:
	ChiSquareDistribution();
	ChiSquareDistribution(double dof);
	ChiSquareDistribution(const ChiSquareDistribution& dist);
	const ChiSquareDistribution& operator = (const ChiSquareDistribution& dist);
	~ChiSquareDistribution();

	double pdf(double x) const override;
	double cdf(double x) const override;

private:
	double mDoF;
};

class ExponentialDistribution : public ContinuousDistribution
{
public:
	ExponentialDistribution();
	ExponentialDistribution(double lambda);
	ExponentialDistribution(const ExponentialDistribution& dist);
	~ExponentialDistribution();
	const ExponentialDistribution& operator = (const ExponentialDistribution& dist);

	double pdf(double x) const override;
	double cdf(double x) const override;
	double invCDF(double prob, const BisectionSolver& solver) const override;

private:
	double mLambda;
};
#endif