#!/bin/bash


rm ./SOURCE_FILES/EXE_codeFileGenerator_1D_CN
rm ./SOURCE_FILES/EXE_prepareDataToPlot_1D_CN
rm ./SOURCE_FILES/EXE_simulator_1D_CN_ABC_tINDEP
rm ./SOURCE_FILES/EXE_simulator_1D_CN_ABC_tDEP
rm ./SOURCE_FILES/EXE_codeFileGenerator_2D_CN
rm ./SOURCE_FILES/EXE_prepareDataToPlot_2D_CN
rm ./SOURCE_FILES/EXE_simulator_2D_CN_tINDEP
rm ./SOURCE_FILES/EXE_simulator_2D_CN_tDEP
rm ./SOURCE_FILES/EXE_codeFileGenerator_nD_XO_ZERO_CN_ABC_tDEP
rm ./SOURCE_FILES/CODE_simulator_nD_XO_ZERO_CN_ABC_tDEP
rm ./SOURCE_FILES/EXE_simulator_nD_XO_ZERO_CN_ABC_tDEP
rm ./SOURCE_FILES/EXE_simulator_1D_CN_tINDEP_ImTimeEv
rm ./SOURCE_FILES/EXE_simulator_1D_CN_tDEP_ImTimeEv
rm ./SOURCE_FILES/EXE_simulator_2D_CN_tINDEP_ImTimeEv
rm ./SOURCE_FILES/EXE_simulator_nD_XO_ZERO_CN_tDEP_ImTimeEv
rm ./SOURCE_FILES/EXE_codeFileGenerator_2D_XO_KINADV_BornHeun
rm ./SOURCE_FILES/EXE_simulator_2D_XO_KINADV_BornHeun_tINDEP

rm ./SOURCE_FILES/DATA_rawSimulationData_1D_CN.txt
rm ./SOURCE_FILES/DATA_dataToPlot_1D_CN.txt
rm ./SOURCE_FILES/DATA_potentialData_1D_CN.txt
rm ./SOURCE_FILES/DATA_rawSimulationData_2D_CN.txt
rm ./SOURCE_FILES/DATA_dataToPlot_2D_CN.txt
rm ./SOURCE_FILES/DATA_trajectoriesToPlot_1D_CN.txt
rm ./SOURCE_FILES/DATA_trajectoriesToPlot_2D_CN.txt
rm ./SOURCE_FILES/DATA_rawSimulationData_nD_XO_ZERO_CN_ABC_tDEP.txt
rm ./SOURCE_FILES/DATA_trajectoriesToPlot_nD_XO_ZERO_CN_ABC_tDEP.txt
rm ./SOURCE_FILES/DATA_probabilityToPlot_nD_XO_ZERO_CN_ABC_tDEP.txt
rm ./SOURCE_FILES/DATA_potentialToPlot_nD_XO_ZERO_CN_ABC_tDEP.txt
rm ./SOURCE_FILES/DATA_tensorProduct_nD_XO_ZERO_CN_ABC_tDEP.txt
rm ./SOURCE_FILES/DATA_potentialToPlot_2D_XO_CN_KinAdv_BornHeun_tINDEP.txt
rm ./SOURCE_FILES/DATA_probabilityToPlot_2D_XO_CN_KinAdv_BornHeun_tINDEP.txt
rm ./SOURCE_FILES/DATA_trajectoriesToPlot_2D_XO_CN_KinAdv_BornHeun_tINDEP.txt

rm ./SOURCE_FILES/CODE_simulator_1D_CN_ABC_tINDEP.cpp
rm ./SOURCE_FILES/CODE_simulator_1D_CN_ABC_tDEP.cpp
rm ./SOURCE_FILES/CODE_simulator_2D_CN_tINDEP.cpp
rm ./SOURCE_FILES/CODE_simulator_2D_CN_tDEP.cpp
rm ./SOURCE_FILES/CODE_simulator_nD_XO_ZERO_CN_ABC_tDEP.cpp
rm ./SOURCE_FILES/CODE_simulator_1D_CN_tINDEP_ImTimeEv.cpp
rm ./SOURCE_FILES/CODE_simulator_1D_CN_tDEP_ImTimeEv.cpp
rm ./SOURCE_FILES/CODE_simulator_2D_CN_tINDEP_ImTimeEv.cpp
rm ./SOURCE_FILES/CODE_simulator_nD_XO_ZERO_CN_tDEP_ImTimeEv.cpp
rm ./SOURCE_FILES/CODE_simulator_2D_XO_KINADV_BornHeun_tINDEP.cpp

g++ -Wall -O ./SOURCE_FILES/CODE_codeFileGenerator_1D_CN.cpp -o ./SOURCE_FILES/EXE_codeFileGenerator_1D_CN
g++ -Wall -O ./SOURCE_FILES/CODE_prepareDataToPlot_1D_CN.cpp -o ./SOURCE_FILES/EXE_prepareDataToPlot_1D_CN
g++ -Wall -O ./SOURCE_FILES/CODE_codeFileGenerator_2D_CN.cpp -o ./SOURCE_FILES/EXE_codeFileGenerator_2D_CN
g++ -Wall -O ./SOURCE_FILES/CODE_prepareDataToPlot_2D_CN.cpp -o ./SOURCE_FILES/EXE_prepareDataToPlot_2D_CN
g++ -Wall -O ./SOURCE_FILES/CODE_codeFileGenerator_nD_XO_ZERO_CN_ABC_tDEP.cpp -o ./SOURCE_FILES/EXE_codeFileGenerator_nD_XO_ZERO_CN_ABC_tDEP
g++ -Wall -O ./SOURCE_FILES/CODE_codeFileGenerator_2D_XO_KINADV_BornHeun.cpp -o ./SOURCE_FILES/EXE_codeFileGenerator_2D_XO_KINADV_BornHeun
