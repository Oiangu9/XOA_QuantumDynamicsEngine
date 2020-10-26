#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int main(int argNum, char **argVec){
if (argNum<14){
	cout << "Error while reading the arguments. Too few arguments introduced? \n";
	return -1;
}
int option=1;
sscanf(argVec[14], "%d", &option);
ofstream writtenFile;

if(option==1){
	writtenFile.open("CODE_Simulator_CN_Conditional_WFs_with_XO_Trajectories.cpp");
	writtenFile << "\n/* Script to generate the closest possible conditonal wave-functions to the positions\nof the trajectories evolved using the XO algorithm.\nThe idea is to extract from the traejctory output file the computed trajectories using\nthe XO alg., extract the full wavefunction at each time computed with the 2D CN method\nand to slice for each trajectory the full WF in those x and y to get the corresponding\n\"exact\" SPCWF. This also allows the computation of the \"exact\" U_j(x,t) expected for\nthat trajectory position y_a(t) for an exact WF.\n*/\n#include <iostream>\n#include <iomanip>\n#include <fstream>\n#include <string>\n#include <complex>\n#include \"LIB_dcomplex.h\" // Macro for using dcomplex as std::complex<double> and J as the complex 0.0+i*1.0\n#include <eigen3/Eigen/Core>\n#include <eigen3/Eigen/Sparse>\n#include <eigen3/Eigen/SparseLU>\nusing namespace std::complex_literals;\nusing namespace Eigen;\nusing namespace std;\n#define PI 3.141592653589793238463\n#define INF 1000000.0\n// Declare necessary variables\nint xDivs="<< argVec[3] <<", yDivs="<< argVec[4] <<", jmax="<< argVec[5] <<", timeIts="<< argVec[10] <<", trajNum="<< argVec[11] <<", lastjUsedInItx=0;\ndouble xmin="<< argVec[6] <<", ymin="<< argVec[7] <<", xmax="<< argVec[8] <<", ymax="<< argVec[9] <<", chiSumTolerance="<< argVec[13] <<", dx, dy, fractionalx, fractionaly;\n//The \"analytical\" expression for the adiabatic states for each x as a function of y and j are expressed\ndouble eigenstatesForSectionsInx(double y, double x, int j){ //the so called psi_x^j(y)\n"<< argVec[12] <<"\n}\nint main(int argNum, char **argVec){\ndx = (xmax-xmin)/xDivs;\ndy = (ymax-ymin)/yDivs;\n// Declare INPUT FILES and necessary variables and arrays ###################################\nifstream DATA_traj_XO_File, DATA_wf_CN_File;\nDATA_traj_XO_File.open(\""<< argVec[1] <<"\"); // de argumento!\nDATA_wf_CN_File.open(\""<< argVec[2] <<"\");\nofstream DATA_conditWF_CN, DATA_CN_Re_Uj, DATA_CN_Im_Uj, DATA_chiInfo, DATA_sumChiInfo;\nDATA_conditWF_CN.open(\"DATA_probDensity_conditional_WF_CN.txt\");\nDATA_CN_Re_Uj.open(\"DATA_CN_Re_Uj.txt\");\nDATA_CN_Im_Uj.open(\"DATA_CN_Im_Uj.txt\");\nDATA_chiInfo.open(\"DATA_chiInfo_CN_w_KA_trajs.txt\");\nDATA_sumChiInfo.open(\"DATA_sumChiInfo_CN_with_KA_traj.txt\");\n// Declare the arrays to use\ndouble** traj=new double*[timeIts+1];\nfor (int i=0; i<=timeIts; ++i){ traj[i]= new double[2];}\ndouble x_aux, y_aux, posx;\nint gridPoints=(xDivs+1)*(yDivs+1);\nArrayXcd WF(gridPoints), psiX(xDivs+1), psiY(yDivs+1);\ncdouble arrayEl, Uj;\nArrayXd probDensityx(xDivs+1), probDensityy(yDivs+1);\ndouble N_x, N_y, sumaParaChisx, wholex, wholey;\nArrayXd xgrid(xDivs+1), ygrid(yDivs+1), sumaChisx(jmax+1);\nfor(int k=0; k<=xDivs; ++k){xgrid(k)=xmin+k*dx;}\nfor(int k=0; k<=yDivs;++k){ygrid(k)=ymin+dy*k;}\nArrayXXcd Ujx_container(xDivs+1, jmax+1), Chijx_container(xDivs+1, jmax+1);\nstring line;\n// Start trajectory and time iterations #########################################\n// For each computed trajectory do\nfor(int trajIdx=0; trajIdx<trajNum; trajIdx++){\n// Extract the whole trajectory\nfor(int tIt=0; tIt<timeIts; tIt++){\ngetline( DATA_traj_XO_File, line);\nistringstream iss(line);\niss >> x_aux >> y_aux;\ntraj[tIt][0] = x_aux;\ntraj[tIt][1] = y_aux;\n}\nDATA_wf_CN_File.clear();\nDATA_wf_CN_File.seekg(0);\ngetline( DATA_wf_CN_File, line); // First line has simulation parameters\n// For each computed time iteration do\nfor(int tIt=0; tIt<timeIts; tIt++){\n// Extract the full complex WF for this iteration\ngetline( DATA_wf_CN_File, line); // the iteration number is written here\nfor(int xIt=0; xIt<gridPoints; xIt++){\ngetline(DATA_wf_CN_File,line);\nistringstream ss(line);\nss>>arrayEl;\nWF(xIt)=arrayEl;\n}\n// Condition the WF to the current tIt trajectory position in x and in y\nfractionalx = std::modf((traj[tIt][0]-xmin)/dx, &wholex);\nfractionaly = std::modf((traj[tIt][1]-ymin)/dy, &wholey);\nif(wholex>=xDivs){wholex=xDivs-2;}else if(wholex<0){wholex=0;} //make the correction to avoid indexing outside the matrix\nif(wholey>=yDivs){wholey=yDivs-2;}else if(wholey<0){wholey=0;}\n// psiX will be WF(x,t; y=y_a(t)), while pisY is WF(y,t; x= x_a(t))\nfor (int yIt=0; yIt<yDivs; yIt++){\npsiY(yIt) = (1-fractionalx)*WF(wholex*(yDivs+1)+ yIt) + fractionalx*WF((wholex+1)*(yDivs+1)+ yIt);}\nfor (int xIt=0; xIt<xDivs; xIt++){\npsiX(xIt) = (1-fractionaly)*WF(xIt*(yDivs+1)+ wholey) + fractionaly*WF(xIt*(yDivs+1)+ wholey+1);}\n// Compute the norms of the conditional WF-s\nprobDensityx = abs2(psiX);\nprobDensityy = abs2(psiY);\nN_x=0.5*(probDensityx(0)+probDensityx(xDivs));\nfor(int i=1; i<xDivs; ++i){N_x+=probDensityx(i);}\nN_x*=dx;\nN_y=0.5*(probDensityy(0)+probDensityy(yDivs));\nfor(int i=1; i<yDivs; ++i){N_y+=probDensityy(i);}\nN_y*=dy;\n// Compute the U^j(x,t) for this trajectory y_a(t) and this time for each j in (0,...,jmax)\nlastjUsedInItx=-1.0;\nsumaParaChisx=0.0;\nfor (int j=0; j<=jmax; ++j){\nfor(int i=0; i<=xDivs; ++i){\nposx = xgrid(i);\n//we get the Uj for this x and this j\nUj=0.5*(eigenstatesForSectionsInx(ymin,posx,j)*psiY(0) + eigenstatesForSectionsInx(ymax,posx,j)*psiY(yDivs));\nfor(int k=1; k<yDivs; ++k){ Uj=Uj+eigenstatesForSectionsInx(ygrid(k),posx,j)*psiY(k);}\nUjx_container(i, j)=Uj*dy/((cdouble) sqrt(N_x*N_y));\nChijx_container(i,j)=Ujx_container(i, j)*psiX(i);\n}\nlastjUsedInItx=j;\nsumaParaChisx+=abs2(Chijx_container.col(j)).sum();\nif((sumaChisx(j)=sumaParaChisx*dx)>=chiSumTolerance){break;}\n}\n// Output the results\n// the norm and the conditional wf probability densities\nDATA_conditWF_CN <<\"CN-Norm_x=\" << N_x<<endl<<probDensityx << endl << endl<<endl;\nDATA_conditWF_CN<<\"CN-Norm_y=\" << N_y<<endl << probDensityy << endl << endl<<endl;\n// output the U_jx\nDATA_CN_Re_Uj << Ujx_container.real() << endl << endl << endl;\nDATA_CN_Im_Uj << Ujx_container.imag() << endl << endl << endl;\n// output chi_j_x partial sums\nfor(int j=0; j<=lastjUsedInItx; ++j){\nDATA_sumChiInfo<<j<<\" \"<<sumaChisx(j)<<endl;\n}\nDATA_sumChiInfo<<endl<<endl;\n// output chi_j_x coefficient modulous\nDATA_chiInfo <<abs(Chijx_container.leftCols(lastjUsedInItx+1))<<endl<<endl<<endl;\n} //end timeIts\n} //end trajectories\nfor(int i=0; i<=timeIts; i++){ delete[] traj[i]; }\ndelete[] traj;\nDATA_traj_XO_File.close();\nDATA_wf_CN_File.close();\nDATA_conditWF_CN.close();\nDATA_CN_Re_Uj.close();\nDATA_CN_Im_Uj.close();\nDATA_sumChiInfo.close();\nDATA_chiInfo.close();\nreturn 0;\n}\n";
}
writtenFile.close();

return 0;
}