#include <iostream>
#include <string>
#include "RNG.h"
#include "Distribution.h"
#include "Solver.h"

namespace {
    template<class T>
    void printVector(const std::vector<T>& vec)
    {
        for (std::size_t i = 0; i < vec.size(); ++i)
        {
            std::cout << vec[i] << ", ";
        }

        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "Starting random number generator!\n";
    std::string distName = "";
    std::string accuracyStr = "";
    std::string numOfRNStr = "";

    std::cout << "Type of distribution (Uniform, Standard Normal, Chi-Square): ";
    std::getline(std::cin, distName);
    std::cout << "Level of accuracy (1~5): ";
    std::getline(std::cin, accuracyStr);
    std::cout << "Number of random numbers (Non-negative, 1~1000): ";
    std::getline(std::cin, numOfRNStr);

    std::size_t accuracy = 0;
    std::size_t numOfRN = 0;
    try
    {
        accuracy = std::stoul(accuracyStr);
        numOfRN = std::stoul(numOfRNStr);
        if (!(accuracy >= 1 && accuracy <= 5 && numOfRN >= 1 && numOfRN <= 1000))
        {
            std::cout << "Input out of range! Exit.";
            return -1;
        }
    }
    catch (...)
    {
        std::cout << "Invalid Input! Exit.";
        return -1;
    }
    
    std::shared_ptr<ContinuousDistribution> dist;
    if (distName == "Uniform")
    {
        dist = std::make_shared<UniformDistribution>(0, 1);
    }
    else if (distName == "Standard Normal")
    {
        dist = std::make_shared<NormalDistribution>(0, 1);
    }
    else if (distName == "Chi-Square")
    {
        // Use exponential distribution with lambda = 0.5 to generate random number of chi-square distribution with DoF = 2
        // For performance
        //dist = std::make_shared<ChiSquareDistribution>(2);
        dist = std::make_shared<ExponentialDistribution>(0.5);
    }
    else
    {
        std::cout << "Invalid Distribution!\n";
        return -1;
    }
        
    std::shared_ptr<BisectionSolver> solver = std::make_shared<BisectionSolver>(1e-8);
    std::shared_ptr<RNG> rng = std::make_shared<RNG>(dist, solver, accuracy, 123);
    std::shared_ptr<std::vector<double>> res = rng->generate(numOfRN);
    
    printVector(*res);
    //std::cout << normal->cdf(10000000000);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
