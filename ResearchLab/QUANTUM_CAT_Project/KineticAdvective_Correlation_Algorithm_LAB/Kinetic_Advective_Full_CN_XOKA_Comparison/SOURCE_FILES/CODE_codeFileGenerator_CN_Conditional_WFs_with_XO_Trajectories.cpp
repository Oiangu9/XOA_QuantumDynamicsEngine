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
	writtenFile.open("CODE_Simulator_CN_Conditional_WFs_with_XO_Trajectories.cpp");
	writtenFile << "\n/* Script to generate the closest possible conditonal wave-functions to the positions\nof the trajectories evolved using the XO algorithm.\nThe idea is to extract from the traejctory output file the computed trajectories using\nthe XO alg., extract the full wavefunction at each time computed with the 2D CN method\nand to slice for each trajectory the full WF in those x and y to get the corresponding\n\"exact\" SPCWF. This also allows the computation of the \"exact\" U_j(x,t) expected for\nthat trajectory position y_a(t) for an exact WF.\n*/\n#include <iostream>\n#include <iomanip>\n#include <fstream>\n#include <string>\n#include <complex>\n#include \"LIB_dcomplex.h\" // Macro for using dcomplex as std::complex<double> and J as the complex 0.0+i*1.0\n#include <eigen3/Eigen/Core>\n#include <eigen3/Eigen/Sparse>\n#include <eigen3/Eigen/SparseLU>\nusing namespace std::complex_literals;\nusing namespace Eigen;\nusing namespace std;\n#define PI 3.141592653589793238463\n#define INF 1000000.0\n// Declare necessary variables\nint xDivs="<< argVec[3] <<", yDivs="<< argVec[4] <<", jmax="<< argVec[5] <<", timeIts="<< argVec[10] <<", trajNum="<< argVec[11] <<", lastjUsedInItx=0;\ndouble xmin="<< argVec[6] <<", ymin="<< argVec[7] <<", xmax="<< argVec[8] <<", ymax="<< argVec[9] <<", chiSumTolerance="<< argVec[13] <<", mx="<< argVec[14] <<", my="<< argVec[15] <<", hbar="<< argVec[16] <<", dx, dy, fractionalx, fractionaly;\n//The \"analytical\" expression for the adiabatic states for each x as a function of y and j are expressed\ndouble eigenstatesForSectionsInx(double y, double x, int j){ //the so called psi_x^j(y)\n"<< argVec[12] <<"\n}\nint main(int argNum, char **argVec){\ndx = (xmax-xmin)/xDivs;\ndy = (ymax-ymin)/yDivs;\n// Declare INPUT FILES and necessary variables and arrays ###################################\nifstream DATA_traj_XO_File, DATA_wf_CN_File, DATA_read_Derivatives;\nDATA_traj_XO_File.open(\""<< argVec[1] <<"\"); // de argumento!\nDATA_wf_CN_File.open(\""<< argVec[2] <<"\");\nofstream DATA_conditWF_CN, DATA_CN_Re_Uj, DATA_CN_Im_Uj, DATA_chiInfo, DATA_sumChiInfo, DATA_G_J_Conditional, DATA_Full_WF_G_J, DATA_derivatives;\nDATA_Full_WF_G_J.open(\"DATA_Full_WF_G_J_CN.txt\");\nDATA_conditWF_CN.open(\"DATA_probDensity_conditional_WF_CN.txt\");\nDATA_CN_Re_Uj.open(\"DATA_CN_Re_Uj.txt\");\nDATA_CN_Im_Uj.open(\"DATA_CN_Im_Uj.txt\");\nDATA_chiInfo.open(\"DATA_chiInfo_CN_w_KA_trajs.txt\");\nDATA_sumChiInfo.open(\"DATA_sumChiInfo_CN_with_KA_traj.txt\");\nDATA_G_J_Conditional.open(\"DATA_G_J_with_CN_and_KA_trajs.txt\");\nDATA_derivatives.open(\"DATA_derivatives_S_R.txt\");\n// Declare the arrays to store the trajectories\ndouble** traj = new double*[timeIts+1];\nfor (int i=0; i<=timeIts; ++i){ traj[i]= new double[4];} //the trajectory will include both position x,y and velocity x,y\ndouble posx;\nint gridPoints=(xDivs+1)*(yDivs+1), wholex, wholey;\nArrayXcd WF(gridPoints), psiX(xDivs+1), psiY(yDivs+1), auxComplexVector(gridPoints), conjPsi(gridPoints);\ncdouble arrayEl, Uj;\nArrayXd probDensityx(xDivs+1), probDensityy(yDivs+1), abs2WF(gridPoints), absWF(gridPoints), dS_dy(gridPoints), dR_dy(gridPoints), d2S_dy2(gridPoints), d2R_dy2(gridPoints), C(gridPoints), dC_dy(gridPoints), d2C_dy2(gridPoints);\ndouble N_x, N_y, sumaParaChisx, wholexf, wholeyf;\nMatrixXd x_y_Gx_Jx(gridPoints, 4), G_J_x_cond(xDivs+1, 2);\nArrayXd xgrid(xDivs+1), ygrid(yDivs+1), sumaChisx(jmax+1);\nfor(int k=0; k<=xDivs; ++k){xgrid(k)=xmin+k*dx;}\nfor(int k=0; k<=yDivs;++k){ygrid(k)=ymin+dy*k;}\nArrayXXcd Ujx_container(xDivs+1, jmax+1), Chijx_container(xDivs+1, jmax+1);\nstring line;\n// Start trajectory and time iterations #########################################\n// TODO: Due to the order in which traejctories are recorded, there are lots of repeated computations (like the extraction of WF for every j-th iteration of each trajectory or the obtention of dS/dy,...d2R/dy2 for each iteration) - All of them are repeated as many times as trajectories we have\n// One could save them just once in enormous arrays of numIt columns of gridPoints rows. However these might be way too big and cause RAM overflow. Thus, a better sterategy might be to save it into output files and the reading them only for doing the conditioning. The other solution would be to compute the stuff for the same iteration of every particle at once (reverse the order of the time iteration and trajectory iteration loops).\n// This however would make things complicated when trying to plot the potentials together with the XOKA results.\n// For each computed trajectory do\nfor(int trajIdx=0; trajIdx<trajNum; trajIdx++){\n// Extract the whole trajectory\nfor(int tIt=0; tIt<=timeIts; tIt++){\ngetline( DATA_traj_XO_File, line);\nistringstream iss(line);\niss >> traj[tIt][0] >> traj[tIt][1] >> wholexf >> traj[tIt][2] >> traj[tIt][3]; //In the file each line is: x y 0 vx vy\n}\nDATA_wf_CN_File.clear();\nDATA_wf_CN_File.seekg(0);\ngetline( DATA_wf_CN_File, line); // First line has simulation parameters\n// For each computed time iteration do\nfor(int tIt=0; tIt<=timeIts; tIt++){\n// Extract the full complex WF for this iteration\ngetline( DATA_wf_CN_File, line); // the iteration number is written here\nfor(int xIt=0; xIt<gridPoints; xIt++){\ngetline(DATA_wf_CN_File,line);\nistringstream ss(line);\nss>>arrayEl;\nWF(xIt)=arrayEl;\n}\n// Condition the WF to the current tIt trajectory position in x and in y\nfractionalx = std::modf((traj[tIt][0]-xmin)/dx, &wholexf);\nfractionaly = std::modf((traj[tIt][1]-ymin)/dy, &wholeyf);\nwholex = wholexf;\nwholey = wholeyf;\nif(wholex>=xDivs){wholex=xDivs-2;}else if(wholex<0){wholex=0;} //make the correction to avoid indexing outside the matrix\nif(wholey>=yDivs){wholey=yDivs-2;}else if(wholey<0){wholey=0;}\n// psiX will be WF(x,t; y=y_a(t)), while pisY is WF(y,t; x= x_a(t))\nfor (int yIt=0; yIt<yDivs; yIt++){\npsiY(yIt) = (1-fractionalx)*WF(wholex*(yDivs+1)+ yIt) + fractionalx*WF((wholex+1)*(yDivs+1)+ yIt);}\nfor (int xIt=0; xIt<xDivs; xIt++){\npsiX(xIt) = (1-fractionaly)*WF(xIt*(yDivs+1)+ wholey) + fractionaly*WF(xIt*(yDivs+1)+ wholey+1);}\n// Compute the norms of the conditional WF-s\nprobDensityx = abs2(psiX);\nprobDensityy = abs2(psiY);\nN_x=0.5*(probDensityx(0)+probDensityx(xDivs));\nfor(int i=1; i<xDivs; ++i){N_x+=probDensityx(i);}\nN_x*=dx;\nN_y=0.5*(probDensityy(0)+probDensityy(yDivs));\nfor(int i=1; i<yDivs; ++i){N_y+=probDensityy(i);}\nN_y*=dy;\n// Compute the U^j(x,t) for this trajectory y_a(t) and this time for each j in (0,...,jmax)\nlastjUsedInItx=-1.0;\nsumaParaChisx=0.0;\nfor (int j=0; j<=jmax; ++j){\nfor(int i=0; i<=xDivs; ++i){\nposx = xgrid(i);\n//we get the Uj for this x and this j\nUj=0.5*(eigenstatesForSectionsInx(ymin,posx,j)*psiY(0) + eigenstatesForSectionsInx(ymax,posx,j)*psiY(yDivs));\nfor(int k=1; k<yDivs; ++k){ Uj=Uj+eigenstatesForSectionsInx(ygrid(k),posx,j)*psiY(k);}\nUjx_container(i, j)=Uj*dy/((cdouble) sqrt(N_x*N_y));\nChijx_container(i,j)=Ujx_container(i, j)*psiX(i);\n}\nlastjUsedInItx=j;\nsumaParaChisx+=abs2(Chijx_container.col(j)).sum();\nif((sumaChisx(j)=sumaParaChisx*dx)>=chiSumTolerance){break;}\n}\n// Output the results\n// the norm and the conditional wf probability densities\nDATA_conditWF_CN <<\"CN-Norm_x=\" << N_x<<endl<<probDensityx/N_x << endl << endl<<endl;\nDATA_conditWF_CN<<\"CN-Norm_y=\" << N_y<<endl << probDensityy/N_y << endl << endl<<endl;\n// output the U_jx\nDATA_CN_Re_Uj << Ujx_container.real() << endl << endl << endl;\nDATA_CN_Im_Uj << Ujx_container.imag() << endl << endl << endl;\n// output chi_j_x partial sums\nfor(int j=0; j<=lastjUsedInItx; ++j){\nDATA_sumChiInfo<<j<<\" \"<<sumaChisx(j)<<endl;\n}\nDATA_sumChiInfo<<endl<<endl;\n// output chi_j_x coefficient modulous\nDATA_chiInfo <<abs(Chijx_container.leftCols(lastjUsedInItx+1))<<endl<<endl<<endl;\n//COMPUTE \"EXACT\" G,J correlation potentials for this trajectory position and velocity\n/* The strategy is the following:\n- Only compute teh derivatives of S and R once, in the first trajectory iteration and\noutput them to a file.\n- For the rest of trajectories simply extract the derivatives form the file\n*/\nabs2WF = abs2(WF); //there is a coefficient wise inverse! maybe it works for complex\nabsWF = abs(WF);\nif(trajIdx==0){\n//conjPsi = inverse(WF);\nconjPsi = conj(WF);\n//C = log(absWF);\n// We first compute the numerical derivatives we will need\n// dS(x,y,t)/dy = hbar/R^2*Im(Psi_conj dPsi(x,y,t)/dy) = hbar*Im(Psi^-1 dPsi(x,y,t)/dy)\n// dR(x,y,t)/dR\nfor(int i=0; i<=xDivs; i++){\nfor(int j=0; j<=yDivs;j++){\nif(j==0){\nauxComplexVector(i*(yDivs+1))=conjPsi(i*(yDivs+1))*(WF(i*(yDivs+1)+1)-WF(i*(yDivs+1)))/dy;\ndR_dy(i*(yDivs+1)) = (absWF(i*(yDivs+1)+1)-absWF(i*(yDivs+1)))/dy;\n//dC_dy(i*(yDivs+1)) = (C(i*(yDivs+1)+1)-C(i*(yDivs+1)))/dy;\n} else if(j==yDivs){\nauxComplexVector(i*(yDivs+1)+yDivs)=conjPsi(i*(yDivs+1)+yDivs)*(WF(i*(yDivs+1)+yDivs)-WF(i*(yDivs+1)+yDivs-1))/dy;\ndR_dy(i*(yDivs+1)+yDivs) = (absWF(i*(yDivs+1)+yDivs)-absWF(i*(yDivs+1)+yDivs-1))/dy;\n//dC_dy(i*(yDivs+1)+yDivs) = (C(i*(yDivs+1)+yDivs)-C(i*(yDivs+1)+yDivs-1))/dy;\n} else if(j==1 || j==(yDivs-1)){\nauxComplexVector(i*(yDivs+1)+j)=conjPsi(i*(yDivs+1)+j)*(WF(i*(yDivs+1)+j+1)-WF(i*(yDivs+1)+j-1))/(2.0*dy);\ndR_dy(i*(yDivs+1)+j) = (absWF(i*(yDivs+1)+j+1)-absWF(i*(yDivs+1)+j-1))/(2.0*dy);\n//dC_dy(i*(yDivs+1)+j) = (C(i*(yDivs+1)+j+1)-C(i*(yDivs+1)+j-1))/(2.0*dy);\n}else{\nauxComplexVector(i*(yDivs+1)+j)=conjPsi(i*(yDivs+1)+j)*(-WF(i*(yDivs+1)+j+2)+8.0*WF(i*(yDivs+1)+j+1)-8.0*WF(i*(yDivs+1)+j-1)+WF(i*(yDivs+1)+j-2))/(12.0*dy);\ndR_dy(i*(yDivs+1)+j) = (-absWF(i*(yDivs+1)+j+2)+8.0*absWF(i*(yDivs+1)+j+1)-8.0*absWF(i*(yDivs+1)+j-1)+absWF(i*(yDivs+1)+j-2))/(12.0*dy);\n//dC_dy(i*(yDivs+1)+j) = (-C(i*(yDivs+1)+j+2)+8.0*C(i*(yDivs+1)+j+1)-8.0*C(i*(yDivs+1)+j-1)+C(i*(yDivs+1)+j-2))/(12.0*dy);\n}\n}\n}\n// imaginary part is extracted and dS/dy = J/|psi|^2 obtained\ndS_dy = hbar*imag(auxComplexVector);\n//d^2S(x,y,t)/dy^2 = d/dy(dS/dy)\n//d^2R(x,y,t)/dy^2 = d/dy(dR/dy)\n// We will use a fourth order second derivative formula for d^2R(x,y,t)/dy^2 as\n// we have an explicit R(x,y,t). However, for S we do not, so we will compute a\n// first derivative of the fourth order to dS/dy to obtain d2S/dy2\nfor(int i=0; i<=xDivs; i++){\nfor(int j=0; j<=yDivs;j++){\nif(j==0){\nd2S_dy2(i*(yDivs+1))=(dS_dy(i*(yDivs+1)+1)-dS_dy(i*(yDivs+1)))/dy;\nd2R_dy2(i*(yDivs+1)) = (dR_dy(i*(yDivs+1)+1)-dR_dy(i*(yDivs+1)))/dy;\n//d2C_dy2(i*(yDivs+1)) = (dC_dy(i*(yDivs+1)+1)-dC_dy(i*(yDivs+1)))/dy;\n} else if(j==yDivs){\nd2S_dy2(i*(yDivs+1)+yDivs)=(dS_dy(i*(yDivs+1)+yDivs)-dS_dy(i*(yDivs+1)+yDivs-1))/dy;\nd2R_dy2(i*(yDivs+1)+yDivs) = (dR_dy(i*(yDivs+1)+yDivs)-dR_dy(i*(yDivs+1)+yDivs-1))/dy;\n//d2C_dy2(i*(yDivs+1)+yDivs) = (dC_dy(i*(yDivs+1)+yDivs)-dC_dy(i*(yDivs+1)+yDivs-1))/dy;\n} else if(j==1 || j==(yDivs-1)){\nd2S_dy2(i*(yDivs+1)+j)=(dS_dy(i*(yDivs+1)+j+1)-dS_dy(i*(yDivs+1)+j-1))/(2.0*dy);\nd2R_dy2(i*(yDivs+1)+j) = (absWF(i*(yDivs+1)+j+1)-2*absWF(i*(yDivs+1)+j)+absWF(i*(yDivs+1)+j-1))/(dy*dy);\n//d2C_dy2(i*(yDivs+1)+j) = (C(i*(yDivs+1)+j+1)-2*C(i*(yDivs+1)+j)+C(i*(yDivs+1)+j-1))/(dy*dy);\n}else{\nd2S_dy2(i*(yDivs+1)+j)=(-dS_dy(i*(yDivs+1)+j+2)+8.0*dS_dy(i*(yDivs+1)+j+1)-8.0*dS_dy(i*(yDivs+1)+j-1)+dS_dy(i*(yDivs+1)+j-2))/(12.0*dy);\nd2R_dy2(i*(yDivs+1)+j) = (-absWF(i*(yDivs+1)+j+2)+16.0*absWF(i*(yDivs+1)+j+1)-30.0*absWF(i*(yDivs+1)+j)+16.0*absWF(i*(yDivs+1)+j-1)-absWF(i*(yDivs+1)+j-2))/(12.0*dy*dy);\n//d2C_dy2(i*(yDivs+1)+j) = (-C(i*(yDivs+1)+j+2)+16.0*C(i*(yDivs+1)+j+1)-30.0*C(i*(yDivs+1)+j)+16.0*C(i*(yDivs+1)+j-1)-C(i*(yDivs+1)+j-2))/(12.0*dy*dy);\n}\n}\n}\n// We output the computations to a file\nx_y_Gx_Jx.col(0) = dS_dy;\nx_y_Gx_Jx.col(1) = d2S_dy2;\nx_y_Gx_Jx.col(2) = dR_dy;\nx_y_Gx_Jx.col(3) = d2R_dy2;\nDATA_derivatives << x_y_Gx_Jx<<endl;\n// get it ready for the true output - will only be repeated in the first trajectory\nfor(int ix=0; ix<=xDivs; ix++){\nfor (int iy=0; iy<=yDivs;iy++){\nx_y_Gx_Jx(ix*(yDivs+1)+iy, 0) = xgrid(ix);\nx_y_Gx_Jx(ix*(yDivs+1)+iy, 1) = ygrid(iy);\n}\n}\n} else{\nfor(int k=0; k<gridPoints; ++k){\ngetline( DATA_read_Derivatives, line);\nistringstream iss(line);\niss >> dS_dy(k) >> d2S_dy2(k) >> dR_dy(k) >> d2R_dy2(k);\n}\n}\n// Compute G_x(x,y,t) and J_x(x,y,t) for this trajectory\nx_y_Gx_Jx.col(2) = dS_dy*(dS_dy*(1.0/(2.0*my)) - abs2WF*traj[tIt][3])-hbar*hbar/(2.0*my)*d2R_dy2*absWF*abs2WF;\nx_y_Gx_Jx.col(3) = hbar*dR_dy*absWF*( abs2WF*traj[tIt][3]-2*dS_dy/my )-(hbar/2.0*my)*abs2WF*d2S_dy2;\n/*\nx_y_Gx_Jx.col(2)=dS_dy;\nx_y_Gx_Jx.col(3)=dR_dy;\nx_y_Gx_Jx.col(2) = dS_dy*(dS_dy*(1.0/(2.0*my)) - traj[tIt][3])-hbar*hbar/(2.0*my)*d2R_dy2/absWF;\nx_y_Gx_Jx.col(3) = hbar*dR_dy/absWF*( traj[tIt][3]-dS_dy/my )-(hbar/2.0*my)*d2S_dy2;\n*/\n// We get the index j corresponding to the current point in the trajectory y(t). If i and j were saved the other way around, slicing the matrices in a certain j would be way simpler -they would be seqNuential pieces-. As they are not, one would need a previous step to slice it trivially.\n// Fortunately, one can slice an eigen Array using a list of indices: dS_dy(seqN(j_slice, xDivs+1, yDivs+1)) meaning take xDivs+1 elements of dS_dy starting from j_slice and jumping yDivs+1 away per index\nif (fractionaly>0.5){\nwholey++; //wholey already contains the correct index j_slice\n}\n// Conditioning the expressions and using the trajectory velocity we can compute G_x(x,t):=G(x,t;y(t)) and J_x(x,t):=J(x,t;y(t))\nG_J_x_cond.col(0) = x_y_Gx_Jx(seqN(wholey, xDivs+1, yDivs+1), 2);\nG_J_x_cond.col(1) = x_y_Gx_Jx(seqN(wholey, xDivs+1, yDivs+1), 3);\nDATA_G_J_Conditional << G_J_x_cond << endl <<endl <<endl;\n//DATA_Full_WF_G_J << x_y_Gx_Jx << endl << endl << endl;\nfor(int ix=0; ix<=xDivs; ix++){\nDATA_Full_WF_G_J << x_y_Gx_Jx.block(ix*yDivs+ix, 0, yDivs+1, 4)<< endl<<endl;\n}\nDATA_Full_WF_G_J<< endl;\n} //end timeIts\nif(trajIdx==0){\n// this only in the first trajectory iteration\nDATA_derivatives.close();\nDATA_read_Derivatives.open(\"DATA_derivatives_S_R.txt\");\n}else{\nDATA_read_Derivatives.clear();\nDATA_read_Derivatives.seekg(0);\n}\n} //end trajectories\nfor(int i=0; i<=timeIts; i++){ delete[] traj[i]; }\ndelete[] traj;\nDATA_traj_XO_File.close();\nDATA_wf_CN_File.close();\nDATA_conditWF_CN.close();\nDATA_CN_Re_Uj.close();\nDATA_CN_Im_Uj.close();\nDATA_sumChiInfo.close();\nDATA_chiInfo.close();\nDATA_G_J_Conditional.close();\nDATA_read_Derivatives.close();\nDATA_Full_WF_G_J.close();\nreturn 0;\n}\n";
}
writtenFile.close();

return 0;
}