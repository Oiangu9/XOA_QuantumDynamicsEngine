/* Script to make a class that generates a random speckle potential given a grain siçe and an interval xmin, xmax 
 Steps are the following:
 
 a) A range of possible V values from -VR to 4VR will be generated given a foxed VR
 b) Using the probability density function,  an array with the probability associated with each possible value of V will be generated
 c) Fixing a grain siçe sigma, a vector with positions from xmin to xmax will be generated such taht the spatial step will be the grain siçe
 d) Each postion will be assigned a random pondered V
 e) The profile of the potential will be interpolated using splines (perhaps it would be a good idea just to smoothiçe the profile instead of fully interpolating it)
 
 f) Inside the main() using the grid discretiçed for the simulation we build the potential field
 
*/

#include <iostream>
#include <fstream>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/unsupported/Eigen/Splines>
#include <random>

using namespace Eigen;
using namespace std;

const double EULER=2.718281828459045;

class SplineFunction {
    public:
    SplineFunction(Eigen::VectorXd const &x_vec,
                    Eigen::VectorXd const &y_vec)
        : x_min(x_vec.minCoeff()),
        x_max(x_vec.maxCoeff()),
        // Spline fitting here. X values are scaled down to [0, 1] for this.
        spline_(Eigen::SplineFitting<Eigen::Spline<double, 1>>::Interpolate(
                    y_vec.transpose(),
                    // No more than cubic spline, but accept short vectors.

                    std::min<int>(x_vec.rows() - 1, 3),
                    scaled_values(x_vec)))
    { }

    double operator()(double x) const {
        // x values need to be scaled down in extraction as well.
        return spline_(scaled_value(x))(0);
    }

    private:
    // Helpers to scale X values down to [0, 1]
    double scaled_value(double x) const {
        return (x - x_min) / (x_max - x_min);
    }

    Eigen::RowVectorXd scaled_values(Eigen::VectorXd const &x_vec) const {
        return x_vec.unaryExpr([this](double x) { return scaled_value(x); }).transpose();
    }

    double x_min;
    double x_max;

    // Spline of one-dimensional "points."
    Eigen::Spline<double, 1> spline_;
};

double VR=3.0, xmin=-35.0, xmax=35.0, grainSigma=5.0;

class SpecklePotential {
public:
    
    SpecklePotential(double VR, double grainSigma, double xmax, double xmin) : VR(VR), grainSigma(grainSigma), xmax(xmax), xmin(xmin)    
    {
    // a) b)
    double Vmin=-VR, Vmax=4*VR;
    int nV=3000;
    double dV=(Vmax-Vmin)/nV;
 
    VectorXd possibleV(nV+1), probabDensity(nV+1);
    double auxV;
    for(int i=0; i<=nV; ++i){
        auxV = Vmin + i*dV;
        possibleV(i) = auxV;
        probabDensity(i) = speckleProbabDensity( auxV, VR);
    }

    double* probabClist = probabDensity.data();
    std::default_random_engine randomGenerator;
    std::discrete_distribution<int> distribution (probabClist,probabClist+nV);
            

    // c) d)

    int nGrainIntervals = (int) (xmax-xmin)/grainSigma;
    VectorXd posGrain(nGrainIntervals + 1), potentialGrain(nGrainIntervals +1);
    for(int i=0; i<=nGrainIntervals; ++i){
        posGrain(i) = xmin + i*grainSigma;
        potentialGrain(i) = possibleV(distribution(randomGenerator));
    }

    // e)

    splinePtr = new SplineFunction(posGrain, potentialGrain);
    
    }
    double evaluate(double x) {
        return (*splinePtr)(x);
    }
    
private:
    double VR, grainSigma, xmax, xmin;
    SplineFunction* splinePtr;
    
    double speckleProbabDensity(double V, double VR){     
        if(V/VR + 1 >=0){ return (1/(EULER*VR))*std::exp(-V/VR);}
        else{ return 0.0; }
    }

};

SpecklePotential speckle(VR, grainSigma, xmax, xmin); // A global instance

double V(double x){
    return speckle.evaluate(x);
}

int main(){
   

    // f)

    int nx = 6000;
    double dx = (xmax-xmin)/nx;
    double auxX;
    VectorXd potential(nx +1), xposition(nx+1);
    for(int i=0; i<=nx; ++i){
        auxX = xmin + i*dx;
        xposition(i) = auxX;
        potential(i) = V(auxX);
    }

    MatrixXd results(nx+1, 2);
    results << xposition, potential;
    ofstream writtenFile;
    writtenFile.open("SpecklePlot.txt");
    writtenFile << results << endl;
    writtenFile.close();

    return 0;
}

