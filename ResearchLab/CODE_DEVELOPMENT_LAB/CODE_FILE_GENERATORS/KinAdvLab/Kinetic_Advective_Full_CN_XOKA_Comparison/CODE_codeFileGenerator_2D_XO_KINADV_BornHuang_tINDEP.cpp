#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(int argNum, char **argVec){
if (argNum<2){
	cout << "Error while reading the arguments. Too few arguments introduced? \n";
	return -1;
}
int option=1;
sscanf(argVec[argNum-1], "%d", &option);
ofstream writtenFile;

if(option==1){
	writtenFile.open("CODE_simulator_XO_KinAdv.cpp");
	writtenFile << "\n// 2D SCHRODINGER EQUATION SOLVER - XO ALGORITHM Kinetic and Advective Correlation Potential approximation of G and J:\n// The Conditional Single Particle Wave Function (CSPWF) of the dimensions (x,y) will be evolved for each initial conditions using a 1D Cranck Nicolson method for the Pseudo Schrodinger Equations\n//WARNING! The eigenstates for the sections are not set cdouble because in the box case they are fully real. However, this should be adjusted for a generalized algorithm. As such, complex conjugate should be done in the integrals for the Ujx!!!\n//We include the necessary libraries\n#include <iostream>\n#include <iomanip>\n#include <fstream>\n#include <string>\n#include <complex>\n#include \"LIB_dcomplex.h\" // Macro for using dcomplex as std::complex<double> and J as the complex 0.0+i*1.0\n#include <eigen3/Eigen/Core>\n#include <eigen3/Eigen/Sparse>\n#include <eigen3/Eigen/SparseLU>\n#include <cmath>\n#include <random>\nusing namespace std::complex_literals;\nusing namespace Eigen;\nusing namespace std;\n#define PI 3.141592653589793238463\n#define INF 1000000.0\n//USER INPUT DECLARATION----------------------------------------------------------------------\n//We declare the Spatial and Time grids - names are self-explaning\ndouble xmin = "<< argVec[7] <<", xmax = "<< argVec[8] <<", ymin = "<< argVec[9] <<", ymax = "<< argVec[10] <<", t0=0.0, dt="<< argVec[11] <<", posx, posy, Nx, Ny;\ncdouble Uj;\nint xDivs="<< argVec[5] <<", yDivs = "<< argVec[6] <<", timeIts="<< argVec[12] <<", xBound="<< argVec[27] <<", aux;\ndouble dx=(xmax-xmin)/xDivs;\ndouble dy=(ymax-ymin)/yDivs;\nint outputDataEvery = "<< argVec[16] <<"; // data will be outputed to the external file only every x time iterations, if 1, every time iteration will be outputted\n//The constants: hbar and the masses of each dimension are declared\ndouble hbar="<< argVec[15] <<", mx="<< argVec[3] <<", my="<< argVec[4] <<";\n//We declare the initial full Wave Function (x,y,t=t0). It will be used in order to obtain the initial CSPWF by evaluating at each dimension the initial position of the other trajectory\ncdouble initialFullWF(double x, double y){\n"<< argVec[1] <<"\n}\n//We declare the static external Potential Field\ndouble W(double x,double y){\n"<< argVec[2] <<"\n}\n//The highest used adiabatic states' index in the Born Huang expansion's truncated version\nint xjmax = "<< argVec[23] <<", yjmax = "<< argVec[24] <<", lastjUsedInItx=0, lastjUsedInIty=0;\n//The tolerance for the sum squared of the chis, the algorithm will use in each time iteration as many chis as required to achieve this squared sum tolerance (or rather they will arrive the maximum tolerated j)\ndouble chiSumTolerance="<< argVec[26] <<";\ndouble sumaParaChisx, sumaParaChisy;\nArrayXd sumaChisx(xjmax+1), sumaChisy(yjmax+1);\ncdouble correlPot, advectiveCor, kineticCor;\n//The \"analytical\" expression for the adiabatic states for each x as a function of y and j are expressed\ndouble eigenstatesForSectionsInx(double y, double x, int j){ //the so called psi_x^j(y)\n"<< argVec[17] <<"\n}\n//The \"analytical\" expression for the adiabatic states for each y as a function of x and j are expressed\ndouble eigenstatesForSectionsIny(double x, double y, int j){ //the so called psi_y^j(x)\n"<< argVec[20] <<"\n}\n//The analytical expression for the above functions' first and second derivatives\ndouble diffyEigenstatesForSectionsInx(double y, double x, int j){ //the so called d psi_x^j(y)/dy\n"<< argVec[18] <<"\n}\ndouble diffyyEigenstatesForSectionsInx(double y, double x, int j){ //the so called d**2 psi_x^j(y)/dy**2\n"<< argVec[19] <<"\n}\ndouble diffxEigenstatesForSectionsIny(double x, double y, int j){ //the so called d psi_y^j(x)/dx\n"<< argVec[21] <<"\n}\ndouble diffxxEigenstatesForSectionsIny(double x, double y, int j){ //the so called d**2 psi_y^j(x)/dx**2\n"<< argVec[22] <<"\n}\n//We choose if we want the Uj for y dimension to be calculated as well\ndouble b_y="<< argVec[25] <<";\n//We declare the matrix that will save the Uj(x,t) values for each x and j, in order to allow the calculation of the normalized Uj\nArrayXXcd Ujx_container(xDivs+1, xjmax+1), Ujy_container(yDivs+1, yjmax +1), Chijx_container(xDivs+1, xjmax+1), Chijy_container(yDivs+1, yjmax+1);\n//ArrayXd Ujx_normFactors(xjmax+1), Ujy_normFactors(yjmax+1);\n//We declare a matrix to store the computed correlation potential G(x)+i*J(x) for a certain time iteration in order to allow its plot. Another one for G(y)+i*J(y)\n//We also declare a matrix to store the real and imaginary parts of the Kinetic and Advective correlation potentials approximated for x and for y. (4 columns -> Re{Kin}, Im{Kin}, Re{Adv}, Im{Adv}\nArrayXXd G_J_x(xDivs+1, 2), G_J_y(yDivs+1, 2), KinAdv_x(xDivs+1, 4), KinAdv_y(yDivs+1, 4);\n//We initialize a vector that will number the trajectories that have crossed a certain boundary in each time\nArrayXd trajProportionCrossed = ArrayXd::Zero(timeIts+1);\n//The grid positions in x and y are saved into a vector as they are heavily used in the computation of Uj\nArrayXd xgrid(xDivs+1), ygrid(yDivs+1);\nint main(){\nfor(int k=0; k<=xDivs; ++k){xgrid(k)=xmin+k*dx;}\nfor(int k=0; k<=yDivs;++k){ygrid(k)=ymin+dy*k;}\n//The initial positions of each trajectory that will be evolved using the algorithm are chosen according to the probability distribution given by the modulus squared of the initial wave function\nint numTrajs="<< argVec[13] <<"; // we choose the number of trajectories that will be evolved\nint gridPointsFullWF=(xDivs+1)*(yDivs+1);\ndouble fractional, whole;\nArrayXd probabDensity(gridPointsFullWF);\nArrayXcd initialFullPsi(gridPointsFullWF);\ndouble* initialPosx = (double*) malloc(numTrajs*sizeof(double));\ndouble* initialPosy = (double*) malloc(numTrajs*sizeof(double));\n// the initial state of the full wavefunction is generated in order to obtain its modulus squared in each point\nfor(int i=0; i<=xDivs; ++i){\nfor(int j=0; j<=yDivs; ++j){\ninitialFullPsi(i*(yDivs+1) + j) = initialFullWF(xgrid(i), ygrid(j));\n}\n}\n// the probability associated with each space point is generated\nprobabDensity =100*abs2(initialFullPsi);\n// the random number generator initialised\ndouble* probabClist = probabDensity.data();\nstd::default_random_engine generator;\nstd::discrete_distribution<int> distribution (probabClist,probabClist+gridPointsFullWF-1);\n// and the initial positions chosen according to the associated probabilities\nfor(int i=0; i<numTrajs; i++){\naux=distribution(generator); //returns the winner index of the prob vector -> we must revert the indexing to 2d indexes\ninitialPosx[i] = xmin + ((int) aux/(yDivs+1))*dx;\ninitialPosy[i] = ymin + (aux%(yDivs+1))*dy;\n}\n// begin the time iterations for each evolved trajectory - UL, UR must be renamed in every iteration - as if it was a time dependant potential algorithm for a 1D particle\n//we declare and prepare the propagator matrices for the Cranck Nicolson (CN) evolution\nSparseMatrix<cdouble> U1x(xDivs+1, xDivs+1), U2x(xDivs+1, xDivs+1);\nSparseMatrix<cdouble> U1y(yDivs+1, yDivs+1), U2y(yDivs+1, yDivs+1);\nU1x.reserve(VectorXi::Constant(xDivs+1,3));\nU2x.reserve(VectorXi::Constant(xDivs+1,3));\nU1y.reserve(VectorXi::Constant(yDivs+1,3));\nU2y.reserve(VectorXi::Constant(yDivs+1,3));\ncdouble ax=J*dt*hbar/(4.0*mx*dx*dx);\ncdouble ay=J*dt*hbar/(4.0*my*dy*dy);\nfor(int i=1;i<xDivs;i++){\nU1x.insert(i,i)= 1.0*J; //just initialise to some random variable\nU1x.insert(i,i+1)= -ax;\nU1x.insert(i,i-1)= -ax;\n}\nU1x.insert(0,0)= 1.0*J;\nU1x.insert(0,1)= -ax;\nU1x.insert(xDivs,xDivs)= 1.0*J;\nU1x.insert(xDivs,xDivs-1)= -ax;\nU1x.makeCompressed();\nU2x = U1x.conjugate();\nU2x.makeCompressed();\nfor(int i=1;i<yDivs;i++){\nU1y.insert(i,i)= 1.0*J; //just initialise to some random variable\nU1y.insert(i,i+1)= -ay;\nU1y.insert(i,i-1)= -ay;\n}\nU1y.insert(0,0)= 1.0*J;\nU1y.insert(0,1)= -ay;\nU1y.insert(yDivs,yDivs)= 1.0*J;\nU1y.insert(yDivs,yDivs-1)= -ay;\nU1y.makeCompressed();\nU2y = U1y.conjugate();\nU2y.makeCompressed();\n//We initialise the LU solvers\nSparseLU<SparseMatrix<cdouble>> LUsolverx;\nSparseLU<SparseMatrix<cdouble>> LUsolvery;\n//We declare the SPCWFs and the auxiliar vectors for velocity field computation\nVectorXcd psiX(xDivs+1), psiY(yDivs+1), U2psix(xDivs+1), U2psiy(yDivs+1);\nVectorXcd conjPsix(xDivs+1), conjPsiy(yDivs+1), auxX(xDivs+1), auxY(yDivs+1);\nArrayXd probDensityx(xDivs+1), probDensityy(yDivs+1), velocityFieldx(xDivs+1), velocityFieldy(yDivs+1), auxArrayx(xDivs+1), auxArrayy(yDivs+1);\n//we define the trajectory matrix\ndouble** traj=new double*[timeIts+1];\nfor (int i=0; i<=timeIts; ++i){ traj[i]= new double[2];} // the trajectory is saved in an array of timeIts arrays of 2 doubles (xi, yi)\n// each of the timeIts arrays contains the value for the trajectory in each of the x,y at that iteration\ndouble vx, vy;\n//We open the output streams\nofstream probabDataFile, trajDataFile, DATA_chiInfo, DATA_sumChiInfo, DATA_G_J_x, DATA_G_J_y, DATA_KinAdv_x, DATA_KinAdv_y, DATA_XO_Re_Uj_x, DATA_XO_Im_Uj_x;\n//psiDataFile.open(\"DATA_rawSimulationData_nD_XO_ZERO_CN_ABC_tDEP.txt\");\nprobabDataFile.open(\"DATA_probabilityToPlot_2D_XO_KinAdv_BornHuang_tINDEP.txt\");\ntrajDataFile.open(\"DATA_trajectoriesToPlot_2D_XO_CN_KinAdv_BornHuang_tINDEP_k="<< argVec[28] <<".txt\");\nDATA_chiInfo.open(\"DATA_chiInfo_XO.txt\");\nDATA_sumChiInfo.open(\"DATA_sumChiInfo_XO.txt\");\nDATA_G_J_x.open(\"DATA_G_J_x.txt\");\n//DATA_G_J_y.open(\"DATA_G_J_y.txt\");\nDATA_XO_Re_Uj_x.open(\"DATA_XO_Re_Uj.txt\");\nDATA_XO_Im_Uj_x.open(\"DATA_XO_Im_Uj.txt\");\nDATA_KinAdv_x.open(\"DATA_KinAdv_x.txt\");\n//DATA_KinAdv_y.open(\"DATA_KinAdv_y.txt\");\n//psiDataFile << std::setprecision(17);\nprobabDataFile << std::setprecision(17);\ntrajDataFile << std::setprecision(17);\n//BEGINNING OF THE ALGORITHM FOR EACH OF THE INITIAL CONDITIONS----------------------------------------------------------------\nfor(int trajNum=0; trajNum<numTrajs; ++trajNum){ //this is a potential multithreading branching point\n//We initialise the SPCWF conditioning the full WF to the intial values of the trajectories of this iteration\nfor(int i=0; i<=xDivs; ++i){\npsiX(i) = initialFullWF(xgrid(i), initialPosy[trajNum]);\n}\nfor(int i=0; i<=yDivs; ++i){\npsiY(i) = initialFullWF(initialPosx[trajNum], ygrid(i));\n}\ntraj[0][0]=initialPosx[trajNum];\ntraj[0][1]=initialPosy[trajNum];\n// TIME ITERATIONS BEGIN -----------------------------------------------------------------------------------------------------\nfor(int it=0; it<timeIts; ++it){\n//NEXT POSITION OF THE TRAJECTORY OBTAINED -----------------------------\n//Using the current SPCWF, the velocity field of each dimension at this time is obtained and the next position of the trajectory calculated\n//first for the x dimension-------------------------------------\n//we first get the probability density function and the inverse psi\nprobDensityx =abs2(psiX.array());\nconjPsix = conj(psiX.array());\n// psi_qi^-1* diff(psi_qi,qi) is computed:\n//the borders are get with an Euler difference o(dq) and the immediate divisions with a central difference o(dq^2)\nauxX(0) = conjPsix(0)*(psiX(1)-psiX(0))/(dx*probDensityx(0));\nauxX(1) = conjPsix(1)*(psiX(2)-psiX(0))/(2.0*dx*probDensityx(1));\n//the rest of points are got with a o(dq^4) difference\nfor(int i=2; i<=xDivs-2; ++i){\nauxX(i) = conjPsix(i)*(-psiX(i+2) + 8.0*psiX(i+1) - 8.0*psiX(i-1) +psiX(i-2))/(12*dx*probDensityx(i));\n}\nauxX(xDivs-1) = conjPsix(xDivs-1)*(psiX(xDivs)-psiX(xDivs-2))/(2.0*dx*probDensityx(xDivs-1));\nauxX(xDivs) = conjPsix(xDivs)*(psiX(xDivs)-psiX(xDivs-1))/(dx*probDensityx(xDivs));\n// imaginary part is extracted and the velocity field obtained\nvelocityFieldx = (hbar/mx)*imag(auxX.array());\n//now the y dimension------------------------------------\n//we first get the probability density function and the inverse psi\nprobDensityy =abs2(psiY.array());\nconjPsiy = conj(psiY.array());\n// psi_qi^-1* diff(psi_qi,qi) is computed:\n//the borders are get with an Euler difference o(dq) and the immediate divisions with a central difference o(dq^2)\nauxY(0) = conjPsiy(0)*(psiY(1)-psiY(0))/(dy*probDensityy(0));\nauxY(1) = conjPsiy(1)*(psiY(2)-psiY(0))/(2.0*dy*probDensityy(1));\n//the rest of points are got with a o(dq^4) difference\nfor(int i=2; i<=yDivs-2; ++i){\nauxY(i) = conjPsiy(i)*(-psiY(i+2) + 8.0*psiY(i+1) - 8.0*psiY(i-1) +psiY(i-2))/(12*dy*probDensityy(i));\n}\nauxY(yDivs-1) = conjPsiy(yDivs-1)*(psiY(yDivs)-psiY(yDivs-2))/(2.0*dy*probDensityy(yDivs-1));\nauxY(yDivs) = conjPsiy(yDivs)*(psiY(yDivs)-psiY(yDivs-1))/(dy*probDensityy(yDivs));\n// imaginary part is extracted and the velocity field obtained\nvelocityFieldy = (hbar/my)*imag(auxY.array());\n//we apply the discretisation of the grid to the traj positions\nfractional = std::modf((traj[it][0]-xmin)/dx, &whole);\nif(whole>=xDivs){whole=xDivs-2;}else if(whole<0){whole=0;}\nvx=(1-fractional)*velocityFieldx(whole)+fractional*velocityFieldx(whole+1);\ntraj[it+1][0] = traj[it][0]+vx*dt;\nfractional = std::modf((traj[it][1]-ymin)/dy, &whole);\nif(whole>=yDivs){whole=yDivs-2;}else if(whole<0){whole=0;}\nvy= (1-fractional)*velocityFieldy(whole)+fractional*velocityFieldy(whole+1);\ntraj[it+1][1] = traj[it][1]+vy*dt;\n//The norms of the SPCWFs Nx and Ny for Uj term calculation are obtained with a composed trapezium rule--------------------------------------------------------------------\n//for Nx\nNx=0.5*(probDensityx(0)+probDensityx(xDivs));\nfor(int i=1; i<xDivs; ++i){Nx+=probDensityx(i);}\nNx*=dx;\nNy=0.5*(probDensityy(0)+probDensityy(yDivs));\nfor(int i=1; i<yDivs; ++i){Ny+=probDensityy(i);}\nNy*=dy;\n//Uj(x,t) values of the XO algorithm are calculated so they can be ----------------------------------------------------------------------------\nlastjUsedInItx=-1.0;\nsumaParaChisx=0.0;\nfor(int j=0; j<=xjmax; ++j){\nfor(int i=0; i<=xDivs; ++i){\nposx = xgrid(i);\n//we get the Uj for this x and this j\nUj=0.5*(eigenstatesForSectionsInx(ymin,posx,j)*psiY(0) + eigenstatesForSectionsInx(ymax,posx,j)*psiY(yDivs));\nfor(int k=1; k<yDivs; ++k){ Uj=Uj+eigenstatesForSectionsInx(ygrid(k),posx,j)*psiY(k);}\nUjx_container(i, j)=Uj*dy/((cdouble) sqrt(Nx*Ny));\nChijx_container(i,j)=Ujx_container(i, j)*psiX(i);\n}\nlastjUsedInItx=j;\nsumaParaChisx+=abs2(Chijx_container.col(j)).sum();\nif((sumaChisx(j)=sumaParaChisx*dx)>=chiSumTolerance){break;}\n//Ujx_normFactors(j) = sqrt((abs2(psiX.array()*Ujx_container.col(j)).sum())*(xjmax+1)); //sure we miss the 1/2 for the first and last elements in the trapezium rule, but due to the number of entries in the vector this will turn out negligible\n}\nif(b_y!=0){\nlastjUsedInIty=-1.0;\nsumaParaChisy=0.0;\nfor(int j=0; j<=yjmax; ++j){\nfor(int i=0; i<=yDivs; ++i){\nposy = ygrid(i);\n//we get the Uj for this y and this j\nUj=0.5*(eigenstatesForSectionsIny(xmin,posy,j)*psiX(0) + eigenstatesForSectionsIny(xmax,posy,j)*psiX(xDivs));\nfor(int k=1; k<xDivs; ++k){ Uj=Uj+eigenstatesForSectionsIny(xgrid(k),posy,j)*psiX(k);}\nUjy_container(i, j)=Uj*dx/((cdouble) sqrt(Nx*Ny));\nChijy_container(i,j)=Ujy_container(i, j)*psiY(i);\n}\nlastjUsedInIty=j;\nsumaParaChisy+=abs2(Chijy_container.col(j)).sum();\nif((sumaChisy(j)=sumaParaChisy*dy)>=chiSumTolerance){break;}\n//Ujy_normFactors(j) = sqrt((abs2(psiY.array()*Ujy_container.col(j)).sum())*(yjmax+1));\n}\n}\n//The Ui propagator matrices of each dimension x,y are updated for this time iteration-------------------------------------------------------------------------------------\nposy = traj[it][1];\nfor(int i=0; i<=xDivs; ++i){\nposx = xgrid(i);\nkineticCor = 0.0;\nadvectiveCor = 0.0;\n//correlPot =0.0;\nfor(int j=0; j<=lastjUsedInItx; ++j){ //generate the kinetic and advective correlation potentials for this spatial grid point posx\nkineticCor = kineticCor - Ujx_container(i,j)* 0.5*hbar*hbar*diffyyEigenstatesForSectionsInx(posy, posx, j)/my;\nadvectiveCor = advectiveCor + Ujx_container(i,j)* vy*hbar*diffyEigenstatesForSectionsInx(posy, posx, j);\n//correlPot = correlPot + ( Ujx_container(i,j) )*(kineticCor + J*advectiveCor);\n}\ncorrelPot="<< argVec[29] <<"*kineticCor+J*advectiveCor*"<< argVec[30] <<";\nU1x.coeffRef(i,i) = 1.0+J*dt*(hbar*hbar/(mx*dx*dx)+ W(posx, posy) + "<< argVec[31] <<"*correlPot.real()+J*correlPot.imag()*"<< argVec[32] <<" )/((cdouble)2.0*hbar);\nU2x.coeffRef(i,i) = 1.0-J*dt*(hbar*hbar/(mx*dx*dx)+ W(posx, posy) + "<< argVec[31] <<"*correlPot.real()+J*correlPot.imag()*"<< argVec[32] <<" )/((cdouble)2.0*hbar);\nG_J_x(i,0) = "<< argVec[31] <<"*correlPot.real();\nG_J_x(i,1) = "<< argVec[32] <<"*correlPot.imag();\nKinAdv_x(i,0)="<< argVec[29] <<"*kineticCor.real();\nKinAdv_x(i,1)="<< argVec[29] <<"*kineticCor.imag();\nKinAdv_x(i,2)="<< argVec[30] <<"*advectiveCor.real();\nKinAdv_x(i,3)="<< argVec[30] <<"*advectiveCor.imag();\n}\nposx = traj[it][0];\nfor(int i=0; i<=yDivs; ++i){\nposy = ygrid(i);\ncorrelPot =0.0;\nif(b_y!=0){\nfor(int j=0; j<=lastjUsedInIty; ++j){ //generate the kinetic and advective correlation potentials for this spatial grid point posx\nkineticCor = -0.5*hbar*hbar*diffxxEigenstatesForSectionsIny(posx, posy, j)/mx;\nadvectiveCor = vx*hbar*diffxEigenstatesForSectionsIny(posx, posy, j);\ncorrelPot = correlPot + ( Ujy_container(i,j) )*(kineticCor + J*advectiveCor);\n}\n}\nU1y.coeffRef(i,i)= 1.0+J*dt*(hbar*hbar/(my*dy*dy)+ W(posx,posy) + b_y*correlPot)/((cdouble)2.0*hbar);\nU2y.coeffRef(i,i)= 1.0-J*dt*(hbar*hbar/(my*dy*dy)+ W(posx,posy) + b_y*correlPot)/((cdouble)2.0*hbar);\n//G_J_y(i,0) = correlPot.real();//nG_J_y(i,1) = correlPot.imag();\n}\nU1x.makeCompressed();\nU1y.makeCompressed();\nU2x.makeCompressed();\nU2y.makeCompressed();\n//LU decomposition done\nLUsolverx.compute(U1x);\nif(LUsolverx.info()!=Success) {\ncout << \"LUx decomposition FAILED!\" << endl;\nreturn 1;\n}\nU2psix= U2x*psiX;\npsiX = LUsolverx.solve(U2psix); //the wavefunction of the next time iteration is generated\nLUsolvery.compute(U1y);\nif(LUsolvery.info()!=Success) {\ncout << \"LUy decomposition FAILED!\" << endl;\nreturn 1;\n}\nU2psiy= U2y*psiY;\npsiY = LUsolvery.solve(U2psiy);\nif( it%outputDataEvery == 0){ //then we output the data\nprobabDataFile <<\"KA-Norm_x=\" << Nx<<endl<<probDensityx << endl << endl<<endl;\nprobabDataFile<<\"KA-Norm_y=\" << Ny<<endl << probDensityy << endl << endl<<endl;\nfor(int j=0; j<=lastjUsedInItx; ++j){\nDATA_sumChiInfo<<j<<\" \"<<sumaChisx(j)<<endl;\n}\nDATA_sumChiInfo<<endl<<endl;\nDATA_chiInfo <<abs(Chijx_container.leftCols(lastjUsedInItx+1))<<endl<<endl<<endl;\nDATA_G_J_x<<G_J_x<<endl<<endl<<endl;\n//DATA_G_J_y<<G_J_y<<endl<<endl<<endl;\nDATA_KinAdv_x << KinAdv_x<<endl<<endl<<endl;\n//DATA_KinAdv_y << KinAdv_y<<endl<<endl<<endl;\nDATA_XO_Re_Uj_x << Ujx_container.real() << endl<<endl<<endl;\nDATA_XO_Im_Uj_x << Ujx_container.imag() << endl << endl << endl;\n}\n}\nfor(int it=0; it<timeIts; ++it){\nif( it%outputDataEvery == 0){ //then we output the data\ntrajDataFile << traj[it][0] << \" \" << traj[it][1] << \" \";\ntrajDataFile <<\" 0\"<<endl;\nif(traj[it][0]>=xBound){trajProportionCrossed(it)+=1;}\n}\n}\n}\nprobabDataFile.close();\ntrajDataFile.close();\nDATA_chiInfo.close();\nDATA_sumChiInfo.close();\nDATA_XO_Re_Uj_x.close();\nDATA_XO_Im_Uj_x.close();\n//We output the shape of the potential in order to be able to plot it\nofstream potentialToPlot, trajProps;\n//we define some output finnes parameters in case it is not necessary to plot the potential to full accuracy (make the algorithm faster)\ndouble potentialPlotFinness="<< argVec[14] <<";\nint enoughStepx=xDivs*potentialPlotFinness;\nint enoughStepy=yDivs*potentialPlotFinness;\ndouble* posArx=new double[xDivs+1];\ndouble* posAry=new double[yDivs+1];\nfor(int i=0; i<=xDivs; i+=enoughStepx){\nposArx[i]=xgrid(i);}\nfor(int j=0; j<=yDivs; j+=enoughStepy){\nposAry[j]=ygrid(j);}\npotentialToPlot.open(\"DATA_potentialToPlot_2D_XO_CN_KinAdv_BornHuang_tINDEP.txt\");\ntrajProps.open(\"DATA_XO_KA_trajProps_k="<< argVec[28] <<".txt\");\ntrajProportionCrossed = (1/(double)numTrajs)*trajProportionCrossed;\nfor(int it=0; it<=timeIts; ++it){\nif(it%outputDataEvery ==0){trajProps<<trajProportionCrossed(it)<<endl;\nfor(int i=0; i<=xDivs; i+=enoughStepx){\nfor(int j=0; j<=yDivs; j+=enoughStepy){\npotentialToPlot << posArx[i] << \" \" << posAry[j] << \" \" << W(posArx[i], posAry[j])<< endl;\n}potentialToPlot<<endl;\n}potentialToPlot<<endl;\n}}\npotentialToPlot.close();\ntrajProps.close();\nreturn 0;\n}\n// #################################################################################################\n// #################################################################################################\n";

}else if(option==2){
	writtenFile.open("CODE_simulator_XO_NoGJ.cpp");
	writtenFile << "\n// 2D SCHRODINGER EQUATION SOLVER - XO ALGORITHM NO G, J:\n// The Conditional Single Particle Wave Function (CSPWF) of the dimensions (x,y) will be evolved for each initial conditions using a 1D Cranck Nicolson method for the Pseudo Schrodinger Equations\n//We include the necessary libraries\n#include <iostream>\n#include <iomanip>\n#include <fstream>\n#include <string>\n#include <complex>\n#include \"LIB_dcomplex.h\" // Macro for using dcomplex as std::complex<double> and J as the complex 0.0+i*1.0\n#include <eigen3/Eigen/Core>\n#include <eigen3/Eigen/Sparse>\n#include <eigen3/Eigen/SparseLU>\n#include <cmath>\n#include <random>\nusing namespace std::complex_literals;\nusing namespace Eigen;\nusing namespace std;\n#define PI 3.141592653589793238463\n#define INF 1000000.0\n//USER INPUT DECLARATION----------------------------------------------------------------------\n//We declare the Spatial and Time grids - names are self-explaning\ndouble xmin = "<< argVec[7] <<", xmax = "<< argVec[8] <<", ymin = "<< argVec[9] <<", ymax = "<< argVec[10] <<", t0=0.0, dt="<< argVec[11] <<", posx, posy, Nx, Ny;\ncdouble Uj;\nint xDivs="<< argVec[5] <<", yDivs = "<< argVec[6] <<", timeIts="<< argVec[12] <<", xBound="<< argVec[27] <<", aux;\ndouble dx=(xmax-xmin)/xDivs;\ndouble dy=(ymax-ymin)/yDivs;\nint outputDataEvery = "<< argVec[16] <<"; // data will be outputed to the external file only every x time iterations, if 1, every time iteration will be outputted\n//The constants: hbar and the masses of each dimension are declared\ndouble hbar="<< argVec[15] <<", mx="<< argVec[3] <<", my="<< argVec[4] <<";\n//We declare the initial full Wave Function (x,y,t=t0). It will be used in order to obtain the initial CSPWF by evaluating at each dimension the initial position of the other trajectory\ncdouble initialFullWF(double x, double y){\n"<< argVec[1] <<"\n}\n//We declare the static external Potential Field\ndouble W(double x,double y){\n"<< argVec[2] <<"\n}\n//The highest used adiabatic states' index in the Born Huang expansion's truncated version\nint xjmax = "<< argVec[23] <<", yjmax = "<< argVec[24] <<", lastjUsedInItx=0, lastjUsedInIty=0;\n//The tolerance for the sum squared of the chis, the algorithm will use in each time iteration as many chis as required to achieve this squared sum tolerance (or rather they will arrive the maximum tolerated j)\ndouble chiSumTolerance="<< argVec[26] <<";\ndouble sumaParaChisx;\ncdouble kineticCor, advectiveCor;\nArrayXd sumaChisx(xjmax+1);\ncdouble correlPot;\n//The \"analytical\" expression for the adiabatic states for each x as a function of y and j are expressed\ndouble eigenstatesForSectionsInx(double y, double x, int j){ //the so called psi_x^j(y)\n"<< argVec[17] <<"\n}\n//The \"analytical\" expression for the adiabatic states for each y as a function of x and j are expressed\ndouble eigenstatesForSectionsIny(double x, double y, int j){ //the so called psi_y^j(x)\n"<< argVec[20] <<"\n}\n//The analytical expression for the above functions' first and second derivatives\ndouble diffyEigenstatesForSectionsInx(double y, double x, int j){ //the so called d psi_x^j(y)/dy\n"<< argVec[18] <<"\n}\ndouble diffyyEigenstatesForSectionsInx(double y, double x, int j){ //the so called d**2 psi_x^j(y)/dy**2\n"<< argVec[19] <<"\n}\ndouble diffxEigenstatesForSectionsIny(double x, double y, int j){ //the so called d psi_y^j(x)/dx\n"<< argVec[21] <<"\n}\ndouble diffxxEigenstatesForSectionsIny(double x, double y, int j){ //the so called d**2 psi_y^j(x)/dx**2\n"<< argVec[22] <<"\n}\n//We choose if we want the Uj for y dimension to be calculated as well\ndouble b_y="<< argVec[25] <<";\n//We declare the matrix that will save the Uj(x,t) values for each x and j, in order to allow the calculation of the normalized Uj\nArrayXXcd Ujx_container(xDivs+1, xjmax+1), Ujy_container(yDivs+1, yjmax +1), Chijx_container(xDivs+1, xjmax+1), Chijy_container(yDivs+1, yjmax+1);\n//ArrayXd Ujx_normFactors(xjmax+1), Ujy_normFactors(yjmax+1);\n//We initialize a vector that will number the trajectories that have crossed a certain boundary in each time\nArrayXd trajProportionCrossed = ArrayXd::Zero(timeIts+1);\n//The grid positions in x and y are saved into a vector as they are heavily used in the computation of Uj\nArrayXd xgrid(xDivs+1), ygrid(yDivs+1);\nint main(){\nfor(int k=0; k<=xDivs; ++k){xgrid(k)=xmin+k*dx;}\nfor(int k=0; k<=yDivs;++k){ygrid(k)=ymin+dy*k;}\n//The initial positions of each trajectory that will be evolved using the algorithm are chosen according to the probability distribution given by the modulus squared of the initial wave function\nint numTrajs="<< argVec[13] <<"; // we choose the number of trajectories that will be evolved\nint gridPointsFullWF=(xDivs+1)*(yDivs+1);\ndouble fractional, whole;\nArrayXd probabDensity(gridPointsFullWF);\nArrayXcd initialFullPsi(gridPointsFullWF);\ndouble* initialPosx = (double*) malloc(numTrajs*sizeof(double));\ndouble* initialPosy = (double*) malloc(numTrajs*sizeof(double));\n// the initial state of the full wavefunction is generated in order to obtain its modulus squared in each point\nfor(int i=0; i<=xDivs; ++i){\nfor(int j=0; j<=yDivs; ++j){\ninitialFullPsi(i*(yDivs+1) + j) = initialFullWF(xgrid(i), ygrid(j));\n}\n}\n// the probability associated with each space point is generated\nprobabDensity =100*abs2(initialFullPsi);\n// the random number generator initialised\ndouble* probabClist = probabDensity.data();\nstd::default_random_engine generator;\nstd::discrete_distribution<int> distribution (probabClist,probabClist+gridPointsFullWF-1);\n// and the initial positions chosen according to the associated probabilities\nfor(int i=0; i<numTrajs; i++){\naux=distribution(generator); //returns the winner index of the prob vector -> we must revert the indexing to 2d indexes\ninitialPosx[i] = xmin + ((int) aux/(yDivs+1))*dx;\ninitialPosy[i] = ymin + (aux%(yDivs+1))*dy;\n}\n// begin the time iterations for each evolved trajectory - UL, UR must be renamed in every iteration - as if it was a time dependant potential algorithm for a 1D particle\n//we declare and prepare the propagator matrices for the Cranck Nicolson (CN) evolution\nSparseMatrix<cdouble> U1x(xDivs+1, xDivs+1), U2x(xDivs+1, xDivs+1);\nSparseMatrix<cdouble> U1y(yDivs+1, yDivs+1), U2y(yDivs+1, yDivs+1);\nU1x.reserve(VectorXi::Constant(xDivs+1,3));\nU2x.reserve(VectorXi::Constant(xDivs+1,3));\nU1y.reserve(VectorXi::Constant(yDivs+1,3));\nU2y.reserve(VectorXi::Constant(yDivs+1,3));\ncdouble ax=J*dt*hbar/(4.0*mx*dx*dx);\ncdouble ay=J*dt*hbar/(4.0*my*dy*dy);\nfor(int i=1;i<xDivs;i++){\nU1x.insert(i,i)= 1.0*J; //just initialise to some random variable\nU1x.insert(i,i+1)= -ax;\nU1x.insert(i,i-1)= -ax;\n}\nU1x.insert(0,0)= 1.0*J;\nU1x.insert(0,1)= -ax;\nU1x.insert(xDivs,xDivs)= 1.0*J;\nU1x.insert(xDivs,xDivs-1)= -ax;\nU1x.makeCompressed();\nU2x = U1x.conjugate();\nU2x.makeCompressed();\nfor(int i=1;i<yDivs;i++){\nU1y.insert(i,i)= 1.0*J; //just initialise to some random variable\nU1y.insert(i,i+1)= -ay;\nU1y.insert(i,i-1)= -ay;\n}\nU1y.insert(0,0)= 1.0*J;\nU1y.insert(0,1)= -ay;\nU1y.insert(yDivs,yDivs)= 1.0*J;\nU1y.insert(yDivs,yDivs-1)= -ay;\nU1y.makeCompressed();\nU2y = U1y.conjugate();\nU2y.makeCompressed();\n//We initialise the LU solvers\nSparseLU<SparseMatrix<cdouble>> LUsolverx;\nSparseLU<SparseMatrix<cdouble>> LUsolvery;\n//We declare the SPCWFs and the auxiliar vectors for velocity field computation\nVectorXcd psiX(xDivs+1), psiY(yDivs+1), U2psix(xDivs+1), U2psiy(yDivs+1);\nVectorXcd conjPsix(xDivs+1), conjPsiy(yDivs+1), auxX(xDivs+1), auxY(yDivs+1);\nArrayXd probDensityx(xDivs+1), probDensityy(yDivs+1), velocityFieldx(xDivs+1), velocityFieldy(yDivs+1), auxArrayx(xDivs+1), auxArrayy(yDivs+1);\n//we define the trajectory matrix\ndouble** traj=new double*[timeIts+1];\nfor (int i=0; i<=timeIts; ++i){ traj[i]= new double[2];} // the trajectory is saved in an array of timeIts arrays of 2 doubles (xi, yi)\n// each of the timeIts arrays contains the value for the trajectory in each of the x,y at that iteration\ndouble vx, vy;\n//We open the output streams\nofstream probabDataFile, trajDataFile;\n//psiDataFile.open(\"DATA_rawSimulationData_nD_XO_ZERO_CN_ABC_tDEP.txt\");\nprobabDataFile.open(\"DATA_probabilityToPlot_2D_XO_CN_KinAdv_BornHuang_tINDEP.txt\");\ntrajDataFile.open(\"DATA_trajectoriesToPlot_2D_XO_CN_NoGJ_BornHuang_tINDEP_k="<< argVec[28] <<".txt\");\n//psiDataFile << std::setprecision(17);\nprobabDataFile << std::setprecision(17);\ntrajDataFile << std::setprecision(17);\n//BEGINNING OF THE ALGORITHM FOR EACH OF THE INITIAL CONDITIONS----------------------------------------------------------------\nfor(int trajNum=0; trajNum<numTrajs; ++trajNum){ //this is a potential multithreading branching point\n//We initialise the SPCWF conditioning the full WF to the intial values of the trajectories of this iteration\nfor(int i=0; i<=xDivs; ++i){\npsiX(i) = initialFullWF(xgrid(i), initialPosy[trajNum]);\n}\nfor(int i=0; i<=yDivs; ++i){\npsiY(i) = initialFullWF(initialPosx[trajNum], ygrid(i));\n}\ntraj[0][0]=initialPosx[trajNum];\ntraj[0][1]=initialPosy[trajNum];\n// TIME ITERATIONS BEGIN -----------------------------------------------------------------------------------------------------\nfor(int it=0; it<timeIts; ++it){\n//NEXT POSITION OF THE TRAJECTORY OBTAINED -----------------------------\n//Using the current SPCWF, the velocity field of each dimension at this time is obtained and the next position of the trajectory calculated\n//first for the x dimension-------------------------------------\n//we first get the probability density function and the inverse psi\nprobDensityx =abs2(psiX.array());\nconjPsix = conj(psiX.array());\n// psi_qi^-1* diff(psi_qi,qi) is computed:\n//the borders are get with an Euler difference o(dq) and the immediate divisions with a central difference o(dq^2)\nauxX(0) = conjPsix(0)*(psiX(1)-psiX(0))/(dx*probDensityx(0));\nauxX(1) = conjPsix(1)*(psiX(2)-psiX(0))/(2.0*dx*probDensityx(1));\n//the rest of points are got with a o(dq^4) difference\nfor(int i=2; i<=xDivs-2; ++i){\nauxX(i) = conjPsix(i)*(-psiX(i+2) + 8.0*psiX(i+1) - 8.0*psiX(i-1) +psiX(i-2))/(12*dx*probDensityx(i));\n}\nauxX(xDivs-1) = conjPsix(xDivs-1)*(psiX(xDivs)-psiX(xDivs-2))/(2.0*dx*probDensityx(xDivs-1));\nauxX(xDivs) = conjPsix(xDivs)*(psiX(xDivs)-psiX(xDivs-1))/(dx*probDensityx(xDivs));\n// imaginary part is extracted and the velocity field obtained\nvelocityFieldx = (hbar/mx)*imag(auxX.array());\n//now the y dimension------------------------------------\n//we first get the probability density function and the inverse psi\nprobDensityy =abs2(psiY.array());\nconjPsiy = conj(psiY.array());\n// psi_qi^-1* diff(psi_qi,qi) is computed:\n//the borders are get with an Euler difference o(dq) and the immediate divisions with a central difference o(dq^2)\nauxY(0) = conjPsiy(0)*(psiY(1)-psiY(0))/(dy*probDensityy(0));\nauxY(1) = conjPsiy(1)*(psiY(2)-psiY(0))/(2.0*dy*probDensityy(1));\n//the rest of points are got with a o(dq^4) difference\nfor(int i=2; i<=yDivs-2; ++i){\nauxY(i) = conjPsiy(i)*(-psiY(i+2) + 8.0*psiY(i+1) - 8.0*psiY(i-1) +psiY(i-2))/(12*dy*probDensityy(i));\n}\nauxY(yDivs-1) = conjPsiy(yDivs-1)*(psiY(yDivs)-psiY(yDivs-2))/(2.0*dy*probDensityy(yDivs-1));\nauxY(yDivs) = conjPsiy(yDivs)*(psiY(yDivs)-psiY(yDivs-1))/(dy*probDensityy(yDivs));\n// imaginary part is extracted and the velocity field obtained\nvelocityFieldy = (hbar/my)*imag(auxY.array());\n//we apply the discretisation of the grid to the traj positions\nfractional = std::modf((traj[it][0]-xmin)/dx, &whole);\nif(whole>=xDivs){whole=xDivs-2;}else if(whole<0){whole=0;}\nvx=(1-fractional)*velocityFieldx(whole)+fractional*velocityFieldx(whole+1);\ntraj[it+1][0] = traj[it][0]+vx*dt;\nfractional = std::modf((traj[it][1]-ymin)/dy, &whole);\nif(whole>=yDivs){whole=yDivs-2;}else if(whole<0){whole=0;}\nvy= (1-fractional)*velocityFieldy(whole)+fractional*velocityFieldy(whole+1);\ntraj[it+1][1] = traj[it][1]+vy*dt;\n//The norms of the SPCWFs Nx and Ny for Uj term calculation are obtained with a composed trapezium rule--------------------------------------------------------------------\n//for Nx\nNx=0.5*(probDensityx(0)+probDensityx(xDivs));\nfor(int i=1; i<xDivs; ++i){Nx+=probDensityx(i);}\nNx*=dx;\nNy=0.5*(probDensityy(0)+probDensityy(yDivs));\nfor(int i=1; i<yDivs; ++i){Ny+=probDensityy(i);}\nNy*=dy;\n//The Ui propagator matrices of each dimension x,y are updated for this time iteration-------------------------------------------------------------------------------------\nposy = traj[it][1];\nfor(int i=0; i<=xDivs; ++i){\nposx = xgrid(i);\nU1x.coeffRef(i,i) = 1.0+J*dt*(hbar*hbar/(mx*dx*dx)+ W(posx, posy))/((cdouble)2.0*hbar);\nU2x.coeffRef(i,i) = 1.0-J*dt*(hbar*hbar/(mx*dx*dx)+ W(posx, posy) )/((cdouble)2.0*hbar);\n}\nposx = traj[it][0];\nfor(int i=0; i<=yDivs; ++i){\nposy = ygrid(i);\nU1y.coeffRef(i,i)= 1.0+J*dt*(hbar*hbar/(my*dy*dy)+ W(posx,posy) )/((cdouble)2.0*hbar);\nU2y.coeffRef(i,i)= 1.0-J*dt*(hbar*hbar/(my*dy*dy)+ W(posx,posy))/((cdouble)2.0*hbar);\n}\nU1x.makeCompressed();\nU1y.makeCompressed();\nU2x.makeCompressed();\nU2y.makeCompressed();\n//LU decomposition done\nLUsolverx.compute(U1x);\nif(LUsolverx.info()!=Success) {\ncout << \"LUx decomposition FAILED!\" << endl;\nreturn 1;\n}\nU2psix= U2x*psiX;\npsiX = LUsolverx.solve(U2psix); //the wavefunction of the next time iteration is generated\nLUsolvery.compute(U1y);\nif(LUsolvery.info()!=Success) {\ncout << \"LUy decomposition FAILED!\" << endl;\nreturn 1;\n}\nU2psiy= U2y*psiY;\npsiY = LUsolvery.solve(U2psiy);\nif( it%outputDataEvery == 0){ //then we output the data\nprobabDataFile <<\"Norm_x=\" << Nx<<endl<<probDensityx << endl << endl<<endl;\nprobabDataFile<<\"Norm_y=\" << Ny<<endl << probDensityy << endl << endl<<endl;\n}\n}\nfor(int it=0; it<timeIts; ++it){\nif( it%outputDataEvery == 0){ //then we output the data\ntrajDataFile << traj[it][0] << \" \" << traj[it][1] << \" \";\ntrajDataFile <<\" 0\"<<endl;\nif(traj[it][0]>=xBound){trajProportionCrossed(it)+=1;}\n}\n}\n}\nprobabDataFile.close();\ntrajDataFile.close();\n//We output the shape of the potential in order to be able to plot it\nofstream potentialToPlot, trajProps;\n//we define some output finnes parameters in case it is not necessary to plot the potential to full accuracy (make the algorithm faster)\ndouble potentialPlotFinness="<< argVec[14] <<";\nint enoughStepx=xDivs*potentialPlotFinness;\nint enoughStepy=yDivs*potentialPlotFinness;\ndouble* posArx=new double[xDivs+1];\ndouble* posAry=new double[yDivs+1];\nfor(int i=0; i<=xDivs; i+=enoughStepx){\nposArx[i]=xgrid(i);}\nfor(int j=0; j<=yDivs; j+=enoughStepy){\nposAry[j]=ygrid(j);}\npotentialToPlot.open(\"DATA_potentialToPlot_2D_XO_CN_KinAdv_BornHuang_tINDEP.txt\");\ntrajProps.open(\"DATA_XO_NoGJ_trajProps_k="<< argVec[28] <<".txt\");\ntrajProportionCrossed = (1/(double)numTrajs)*trajProportionCrossed;\nfor(int it=0; it<=timeIts; ++it){\nif(it%outputDataEvery ==0){trajProps<<trajProportionCrossed(it)<<endl;\nfor(int i=0; i<=xDivs; i+=enoughStepx){\nfor(int j=0; j<=yDivs; j+=enoughStepy){\npotentialToPlot << posArx[i] << \" \" << posAry[j] << \" \" << W(posArx[i], posAry[j])<< endl;\n}potentialToPlot<<endl;\n}potentialToPlot<<endl;\n}}\npotentialToPlot.close();\ntrajProps.close();\nreturn 0;\n}\n";
}
writtenFile.close();

return 0;
}