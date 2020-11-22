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
	writtenFile.open("CODE_CN2D_ChiCalculator.cpp");
	writtenFile << "\n// This is a programm intended to calculate the chi coefficients given a 2D input waveFunction and the adiabatic states for the sections as analytic expressions\n#include <iostream>\n#include <iomanip>\n#include <fstream>\n#include <string>\n#include <complex>\n#include \"LIB_dcomplex.h\" // Macro for using dcomplex as std::complex<double> and J as the complex 0.0+i*1.0\n#include <eigen3/Eigen/Core>\n#include <eigen3/Eigen/Sparse>\n#include <eigen3/Eigen/SparseLU>\n#include <cmath>\n#include <random>\nusing namespace std::complex_literals;\nusing namespace Eigen;\nusing namespace std;\n#define PI 3.141592653589793238463\n#define INF 1000000.0\n//USER INPUT------\n//The information for the input 2D wavefunction psi(x,y)\nifstream inputFileWF;\nint nx1="<< argVec[4] <<",nx2="<< argVec[5] <<", numIt="<< argVec[10] <<", numTrajs="<< argVec[12] <<", gridPoints, aux;\ndouble x1min="<< argVec[6] <<",x1max="<< argVec[7] <<",x2min="<< argVec[8] <<",x2max="<< argVec[9] <<",xBound="<< argVec[11] <<",dx1,dx2, normWF, sumCorners, sumBorders, sumInterior, sumaParaChisx, hbar="<< argVec[14] <<", m1="<< argVec[15] <<", m2="<< argVec[16] <<", fractionalx1, fractionalx2, wholex1, wholex2, dt="<< argVec[17] <<";\ndouble xmin="<< argVec[6] <<",xmax="<< argVec[7] <<",ymin="<< argVec[8] <<",ymax="<< argVec[9] <<";\ndouble * trajectoriesx1, * trajectoriesx2;\n//The adiabatic states for sections in x and the maximum desired j to calculate\nint jmax="<< argVec[3] <<";\ncdouble integral;\nArrayXXcd Chijx_container(nx1+1, jmax+1); // Each column k contains for the energy level j=k the value of chi as a function of the section in x (the position in x)\nArrayXd sumaChisHasta(jmax+1);\ncdouble EigenstatesForSectionsInx(double y, double x, int j){"<< argVec[2] <<"}\n//We initialize the transmitance counter vector\nArrayXd trajProportionCrossed = ArrayXd::Zero(numIt+1);\nArrayXd areaProportionCrossed = ArrayXd::Zero(numIt+1);\n//WF EXTRACTION FROM FILE-------\nint main(){\ninputFileWF.open(\""<< argVec[1] <<"\"); //the path of the file where the WF is saved in a sequentialized matrix vector fashion\nstring line;\ncdouble arrayEl;\ngetline(inputFileWF,line);\n//istringstream ss(line);\n//ss >> nx1 >> nx2 >> nt >> dt >>x1min >> x1max >> x2min >> x2max>>m1>>m2>>hbar; //nx is the number of space division, which is one less than the number of spatial points\ntrajectoriesx1=(double*) malloc(numTrajs*sizeof(double));\ntrajectoriesx2=(double*) malloc(numTrajs*sizeof(double));\ngridPoints=(nx1+1)*(nx2+1);\nArrayXcd WF(gridPoints);\nArrayXcd auxComplexVectorx1(gridPoints), auxComplexVectorx2(gridPoints), conjPsi(gridPoints);\nArrayXd vFieldx1(gridPoints), vFieldx2(gridPoints);\nArrayXd posx1(gridPoints), posx2(gridPoints), absWF(gridPoints);\nMatrixXd results(nx2+1, 3);\n//Prepare the grid in x y\ndx1=(x1max-x1min)/nx1;\ndx2=(x2max-x2min)/nx2;\nfor(int ix=0; ix<=nx1; ix++){\nfor(int iy=0; iy<=nx2; iy++){\nposx1(ix*(nx2+1)+iy) = x1min+ix*dx1;\nposx2(ix*(nx2+1)+iy) = x2min+iy*dx2;\n}\n}\n//We define the initial trajectories randomly according to the probability density of the initial wf\nfor(int xIt=0; xIt<gridPoints; xIt++){\ngetline(inputFileWF,line);\nistringstream ss(line);\nss>>arrayEl;\nWF(xIt)=arrayEl;\n}\nabsWF=100*abs2(WF);\ndouble* probabClist = absWF.data();\nstd::default_random_engine generator;\nstd::discrete_distribution<int> distribution (probabClist,probabClist+gridPoints-1);\nfor(int i=0; i<numTrajs; i++){\naux=distribution(generator);\ntrajectoriesx1[i] = posx1(aux);\ntrajectoriesx2[i] = posx2(aux);\n}\ninputFileWF.seekg(0);\n//Output files initialized\nofstream chiInfo, sumChiInfo, wfPlot, trajFile, trajProps, areaProps;\nchiInfo.open(\"DATA_chiInfo_CN.txt\");\nsumChiInfo.open(\"DATA_sumChiInfo_CN.txt\");\nwfPlot.open(\"DATA_probDensity_WF_CN.txt\");\ntrajFile.open(\"DATA_CN_Trajs_k="<< argVec[13] <<".txt\");\n//BEGINING OF TIME ITERATIONS!--\ngetline(inputFileWF,line);\nistringstream ss(line);\nfor(int tIt=0; tIt<=numIt; ++tIt){\n//Actually extract the wf\ngetline(inputFileWF,line);\nfor(int xIt=0; xIt<gridPoints; xIt++){\ngetline(inputFileWF,line);\nistringstream ss(line);\nss>>arrayEl;\nWF(xIt)=arrayEl;\n}\nabsWF=abs2(WF);\n//CAlculate the velocity fields in x and y\nconjPsi= conj(WF);\n// psi^-1* diff(psi,x)/ absWF is computed\nfor(int i=0; i<nx1; i++){\nfor(int j=0; j<nx2;j++){\nif(i==0){\nauxComplexVectorx1(j)=conjPsi(j)*(WF(nx2+1+j)-WF(j))/(absWF(j)*dx1);\n} else if(i==nx1){\nauxComplexVectorx1(nx1*(nx2+1)+j)=conjPsi(nx1*(nx2+1)+j)*(WF(nx1*(nx2+1)+j)-WF((nx1-1)*(nx2+1)+j))/(absWF(nx1*(nx2+1)+j)*dx1);\n} else if(i==1 || i==(nx1-1)){\nauxComplexVectorx1(i*(nx2+1)+j)=conjPsi(i*(nx2+1)+j)*(WF((i+1)*(nx2+1)+j)-WF((i-1)*(nx2+1)+j))/(2*absWF(i*(nx2+1)+j)*dx1);\n} else{\nauxComplexVectorx1(i*(nx2+1)+j)=conjPsi(i*(nx2+1)+j)*(-WF((i+2)*(nx2+1)+j)+8.0*WF((i+1)*(nx2+1)+j)-8.0*WF((i-1)*(nx2+1)+j)+WF((i-2)*(nx2+1)+j))/(12.0*absWF(i*(nx2+1)+j)*dx1);\n}\nif(j==0){\nauxComplexVectorx2(i*(nx2+1))=conjPsi(i*(nx2+1))*(WF(i*(nx2+1)+1)-WF(i*(nx2+1)))/(absWF(i*(nx2+1))*dx2);\n} else if(j==nx2){\nauxComplexVectorx2(i*(nx2+1)+nx2)=conjPsi(i*(nx2+1)+nx2)*(WF(i*(nx2+1)+nx2)-WF(i*(nx2+1)+nx2-1))/(absWF(i*(nx2+1)+nx2)*dx2);\n} else if(j==1 || j==(nx2-1)){\nauxComplexVectorx2(i*(nx2+1)+j)=conjPsi(i*(nx2+1)+j)*(WF(i*(nx2+1)+j+1)-WF(i*(nx2+1)+j-1))/(2*absWF(i*(nx2+1)+j)*dx2);\n}else{\nauxComplexVectorx2(i*(nx2+1)+j)=conjPsi(i*(nx2+1)+j)*(-WF(i*(nx2+1)+j+2)+8.0*WF(i*(nx2+1)+j+1)-8.0*WF(i*(nx2+1)+j-1)+WF(i*(nx2+1)+j-2))/(12.0*absWF(i*(nx2+1)+j)*dx2);\n} //THERE IS SOMETHING WRONG WITH THE X1 AXIS!\n}\n}\n// imaginary part is extracted and Jk obtained\nvFieldx1 = (hbar/m1)*imag(auxComplexVectorx1);\nvFieldx2 = (hbar/m2)*imag(auxComplexVectorx2);\nfor(int i=0; i<numTrajs;i++){\n//we apply the discretisation of the grid to the traj positions\nfractionalx1 = std::modf((trajectoriesx1[i]-x1min)/dx1, &wholex1);\nfractionalx2 = std::modf((trajectoriesx2[i]-x2min)/dx2, &wholex2);\nif(wholex1>=nx1-1){wholex1=nx1-2;}else if(wholex1<0){wholex1=0;}\nif(wholex2>=nx2-1){wholex2=nx2-2;}else if(wholex2<0){wholex2=0;}\ntrajectoriesx1[i] = trajectoriesx1[i]+( (1-fractionalx1)*vFieldx1( wholex1*(nx2+1) + wholex2)+fractionalx1*vFieldx1( (wholex1+1)*(nx2+1) + wholex2 ))*dt;\ntrajectoriesx2[i] = trajectoriesx2[i]+( (1-fractionalx2)*vFieldx2( wholex1*(nx2+1) + wholex2)+fractionalx2*vFieldx2( wholex1*(nx2+1) + wholex2+1 ))*dt;\ntrajFile << trajectoriesx1[i] << \" \" << trajectoriesx2[i] << endl;\nif (trajectoriesx1[i]>=xBound){trajProportionCrossed(tIt)+=1;}\n}\ntrajFile<<endl<<endl;\n// calculate the normWF of the WF using the 2d trapezium rule\nsumCorners = 0;\nsumBorders = 0;\nsumInterior = 0;\nsumCorners = absWF(0) + absWF(nx2) + absWF(gridPoints-1) + absWF(nx1*(nx2+1));\nfor(int ix=1; ix<nx1; ++ix){\nfor(int iy=1; iy<nx2; ++iy){\nsumInterior += absWF(ix*(nx2+1)+iy);\n}\n}\nfor(int iy=1; iy<nx2; ++iy){\nsumBorders += absWF(iy) + absWF(nx1*(nx2+1) + iy); // ix=0 row and ix=nx1 row\n}\nfor(int ix=1; ix<nx1; ++ix){\nsumBorders += absWF(ix*(nx2+1)) + absWF(ix*(nx2+1)+nx2); //iy=0 row and iy=nx2 row\n}\nnormWF=0.25*dx1*dx2*(sumCorners + 2*sumBorders +4* sumInterior);\nfor(int ix=0; ix<=nx1; ++ix){\nfor(int iy=0; iy<=nx2; ++iy){\nif(posx1(ix*(nx2+1)+iy)>=xBound){\nareaProportionCrossed(tIt)+=absWF(ix*(nx2+1)+iy);\n}\n}\n}\nareaProportionCrossed(tIt)=areaProportionCrossed(tIt)/normWF;\n//CALCULATE PROBABILITY AREA PASSING xBOUND inside the for loop with j and i in the next section\n//CACLCULATE CHI_j(x,t)-----------\nsumaParaChisx=0;\nfor(int j=0; j<=jmax; ++j){\nfor(int ix=0; ix<=nx1; ++ix){\n// for each section in x we integrate the WF in y restricted to this x\nintegral=0.5*(WF(ix*(nx2+1))+WF(ix*(nx2+1)+nx2));\nfor(int iy=1; iy<nx2; ++iy){\nintegral += EigenstatesForSectionsInx(x2min+iy*dx2, x1min+ix*dx1, j)*WF(ix*(nx2+1)+iy);\n}\nChijx_container(ix, j)=integral*dx2;\n}\n//we calculate the incremental sum of the moduluous of chi integrated\nsumaParaChisx+=abs2(Chijx_container.col(j)).sum();\nsumaChisHasta(j)=sumaParaChisx*dx1;\n} //OUTPUT Obtained CHI information for plots--------\n//chiInfo << \"x_position\";\nfor(int j=0; j<=jmax; ++j){\n//chiInfo << \" \"<< sumaChisHasta(j); //header like the sum of the chis will be printed until the given j\nsumChiInfo<<j<<\" \"<<sumaChisHasta(j)<<endl;\n}\nsumChiInfo<<endl<<endl;\nchiInfo << abs(Chijx_container) <<endl <<endl <<endl;\n//OUTPUT WF information for plots-------------\nwfPlot <<\"Norm=\"<< normWF << endl;\n//output the position together with the probability\nfor(int ix=0; ix<=nx1; ix++){\nresults << posx1.block(ix*nx2+ix,0, nx2+1, 1), posx2.block(ix*nx2+ix,0, nx2+1, 1), absWF.block(ix*nx2+ix,0, nx2+1, 1);\nwfPlot <<results<<endl<< endl;\n}\nwfPlot <<endl;\n}\nwfPlot.close();\nchiInfo.close();\nsumChiInfo.close();\ntrajFile.close();\ninputFileWF.close();\nareaProportionCrossed= dx1*dx2*areaProportionCrossed;\ntrajProps.open(\"DATA_CN_trajProps_k="<< argVec[13] <<".txt\");\nareaProps.open(\"DATA_CN_areaProps_k="<< argVec[13] <<".txt\");\ntrajProportionCrossed = (1/(double)numTrajs)*trajProportionCrossed;\ntrajProps<<trajProportionCrossed<<endl;\nareaProps<<areaProportionCrossed<<endl;\ntrajProps.close();\nareaProps.close();\nreturn 0;\n}\n";
}
writtenFile.close();

return 0;
}