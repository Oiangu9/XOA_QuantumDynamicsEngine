$$13$1$CODE_Simulator_CN_Conditional_WFs_with_XO_Trajectories.cpp$$

/* Script to generate the closest possible conditonal wave-functions to the positions
of the trajectories evolved using the XO algorithm.

The idea is to extract from the traejctory output file the computed trajectories using
the XO alg., extract the full wavefunction at each time computed with the 2D CN method
and to slice for each trajectory the full WF in those x and y to get the corresponding
"exact" SPCWF. This also allows the computation of the "exact" U_j(x,t) expected for
that trajectory position y_a(t) for an exact WF.
*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <complex>
#include "LIB_dcomplex.h" // Macro for using dcomplex as std::complex<double> and J as the complex 0.0+i*1.0
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/SparseLU>

using namespace std::complex_literals;
using namespace Eigen;
using namespace std;
#define PI 3.141592653589793238463
#define INF 1000000.0


// Declare necessary variables

int xDivs=$3$, yDivs=$4$, jmax=$5$, timeIts=$10$, trajNum=$11$, lastjUsedInItx=0;
double xmin=$6$, ymin=$7$, xmax=$8$, ymax=$9$, chiSumTolerance=$13$, dx, dy, fractionalx, fractionaly;

//The "analytical" expression for the adiabatic states for each x as a function of y and j are expressed
double eigenstatesForSectionsInx(double y, double x, int j){ //the so called psi_x^j(y)
$12$
}

int main(int argNum, char **argVec){

dx = (xmax-xmin)/xDivs;
dy = (ymax-ymin)/yDivs;

// Declare INPUT FILES and necessary variables and arrays ###################################

ifstream DATA_traj_XO_File, DATA_wf_CN_File;

DATA_traj_XO_File.open("$1$"); // de argumento!
DATA_wf_CN_File.open("$2$");

ofstream DATA_conditWF_CN, DATA_CN_Re_Uj, DATA_CN_Im_Uj, DATA_chiInfo, DATA_sumChiInfo;

DATA_conditWF_CN.open("DATA_probDensity_conditional_WF_CN.txt");
DATA_CN_Re_Uj.open("DATA_CN_Re_Uj.txt");
DATA_CN_Im_Uj.open("DATA_CN_Im_Uj.txt");

DATA_chiInfo.open("DATA_chiInfo_CN_w_KA_trajs.txt");
DATA_sumChiInfo.open("DATA_sumChiInfo_CN_with_KA_traj.txt");

// Declare the arrays to use
double** traj=new double*[timeIts+1];
for (int i=0; i<=timeIts; ++i){ traj[i]= new double[2];}
double x_aux, y_aux, posx;

int gridPoints=(xDivs+1)*(yDivs+1);
ArrayXcd WF(gridPoints), psiX(xDivs+1), psiY(yDivs+1);
cdouble arrayEl, Uj;

ArrayXd probDensityx(xDivs+1), probDensityy(yDivs+1);
double N_x, N_y, sumaParaChisx, wholex, wholey;

ArrayXd xgrid(xDivs+1), ygrid(yDivs+1), sumaChisx(jmax+1);
for(int k=0; k<=xDivs; ++k){xgrid(k)=xmin+k*dx;}
for(int k=0; k<=yDivs;++k){ygrid(k)=ymin+dy*k;}

ArrayXXcd Ujx_container(xDivs+1, jmax+1), Chijx_container(xDivs+1, jmax+1);
string line;

// Start trajectory and time iterations #########################################

// For each computed trajectory do
for(int trajIdx=0; trajIdx<trajNum; trajIdx++){

  // Extract the whole trajectory
  for(int tIt=0; tIt<=timeIts; tIt++){
    getline( DATA_traj_XO_File, line);
    istringstream iss(line);
    iss >> x_aux >> y_aux;
    traj[tIt][0] = x_aux;
    traj[tIt][1] = y_aux;
  }

  DATA_wf_CN_File.clear();
  DATA_wf_CN_File.seekg(0);
  getline( DATA_wf_CN_File, line); // First line has simulation parameters

  // For each computed time iteration do
  for(int tIt=0; tIt<=timeIts; tIt++){
      // Extract the full complex WF for this iteration
      getline( DATA_wf_CN_File, line); // the iteration number is written here
      for(int xIt=0; xIt<gridPoints; xIt++){
        getline(DATA_wf_CN_File,line);
        istringstream ss(line);
        ss>>arrayEl;
        WF(xIt)=arrayEl;
      }
      // Condition the WF to the current tIt trajectory position in x and in y
      fractionalx = std::modf((traj[tIt][0]-xmin)/dx, &wholex);
      fractionaly = std::modf((traj[tIt][1]-ymin)/dy, &wholey);

      if(wholex>=xDivs){wholex=xDivs-2;}else if(wholex<0){wholex=0;} //make the correction to avoid indexing outside the matrix
      if(wholey>=yDivs){wholey=yDivs-2;}else if(wholey<0){wholey=0;}

      // psiX will be WF(x,t; y=y_a(t)), while pisY is WF(y,t; x= x_a(t))
      for (int yIt=0; yIt<yDivs; yIt++){
         psiY(yIt) = (1-fractionalx)*WF(wholex*(yDivs+1)+ yIt) + fractionalx*WF((wholex+1)*(yDivs+1)+ yIt);}
      for (int xIt=0; xIt<xDivs; xIt++){
         psiX(xIt) = (1-fractionaly)*WF(xIt*(yDivs+1)+ wholey) + fractionaly*WF(xIt*(yDivs+1)+ wholey+1);}

      // Compute the norms of the conditional WF-s
      probDensityx = abs2(psiX);
      probDensityy = abs2(psiY);

      N_x=0.5*(probDensityx(0)+probDensityx(xDivs));
      for(int i=1; i<xDivs; ++i){N_x+=probDensityx(i);}
      N_x*=dx;
      N_y=0.5*(probDensityy(0)+probDensityy(yDivs));
      for(int i=1; i<yDivs; ++i){N_y+=probDensityy(i);}
      N_y*=dy;

      // Compute the U^j(x,t) for this trajectory y_a(t) and this time for each j in (0,...,jmax)

      lastjUsedInItx=-1.0;
      sumaParaChisx=0.0;

      for (int j=0; j<=jmax; ++j){
          for(int i=0; i<=xDivs; ++i){
            posx = xgrid(i);
            //we get the Uj for this x and this j

            Uj=0.5*(eigenstatesForSectionsInx(ymin,posx,j)*psiY(0) + eigenstatesForSectionsInx(ymax,posx,j)*psiY(yDivs));
            for(int k=1; k<yDivs; ++k){ Uj=Uj+eigenstatesForSectionsInx(ygrid(k),posx,j)*psiY(k);}
            Ujx_container(i, j)=Uj*dy/((cdouble) sqrt(N_x*N_y));
            Chijx_container(i,j)=Ujx_container(i, j)*psiX(i);
          }
          lastjUsedInItx=j;
          sumaParaChisx+=abs2(Chijx_container.col(j)).sum();
          if((sumaChisx(j)=sumaParaChisx*dx)>=chiSumTolerance){break;}
      }

      // Output the results
      // the norm and the conditional wf probability densities
      DATA_conditWF_CN <<"CN-Norm_x=" << N_x<<endl<<probDensityx << endl << endl<<endl;
      DATA_conditWF_CN<<"CN-Norm_y=" << N_y<<endl << probDensityy << endl << endl<<endl;

      // output the U_jx
      DATA_CN_Re_Uj << Ujx_container.real() << endl << endl << endl;
      DATA_CN_Im_Uj << Ujx_container.imag() << endl << endl << endl;


      // output chi_j_x partial sums
      for(int j=0; j<=lastjUsedInItx; ++j){
        DATA_sumChiInfo<<j<<" "<<sumaChisx(j)<<endl;
      }
      DATA_sumChiInfo<<endl<<endl;

      // output chi_j_x coefficient modulous
      DATA_chiInfo <<abs(Chijx_container.leftCols(lastjUsedInItx+1))<<endl<<endl<<endl;


  } //end timeIts

} //end trajectories

for(int i=0; i<=timeIts; i++){ delete[] traj[i]; }
delete[] traj;

DATA_traj_XO_File.close();
DATA_wf_CN_File.close();

DATA_conditWF_CN.close();
DATA_CN_Re_Uj.close();
DATA_CN_Im_Uj.close();

DATA_sumChiInfo.close();
DATA_chiInfo.close();

return 0;
}
