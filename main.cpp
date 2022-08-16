/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description:   This program was assigned/created as a solo summer intership project with the objective of
 *                creating a medium fidelity radar simulator which simulates a radar system 
 *                consiting of multiple radar faces which possesse a varied amount of unique
 *                search sectors per radar face. The ulitmate objective of this program is to give
 *                user's a accurate, but quick, reference to search, detect and track data from a 
 *                user-defined radar system in a user-defined simuation environment. The short-term
 *                objectives (tasks to be completed during summer internship) was to create a framework 
 *                neccessary to enable such user-configurability and program expandability. Additioinally,
 *                the program would need to be capable of basic searching, detecttion and tracking
 *                of simulated targets. Currently, the simulator can simulate a unique user-defined radar system
 *                with search, detection, and tracking functionality. Thus meeting the short term objectives.
 *                Going forward, simulator fidelity needs to be improved by implementing more radar functionality,
 *                along with, improving searching, detection and tracking algorithms. 
 * 
 *                Updated source code, along with a PDF file that documents the program in further detail,
 *                can be found on at this GitHub repository:
 *                LINK
 
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "configManager.h"
#include <time.h>

using namespace std;

int main() {
    //Simulation setup
   srand(time(0));      //Creates unique seed which is used to randomize starting scanning positions 
   configManager test1; //Creating configuration manager object which handles .txt files containing input/output data

    //Below is inputting all .txt files containing input parameters needed to run simulation
    test1.inputRadarFile("inputFiles/RadarInfo.txt"); //File containing radar.h parameter data
    test1.inputFaceFile("inputFiles/FaceInfo.txt"); //File containing radarFace.h parameter data
    test1.inputSectorFile("inputFiles/SectorInfo.txt"); //File containing searchSector.h parameter data
    test1.inputTargetFile("inputFiles/TargetInfo.txt"); //File containing target.h parameter data

    radar simRadar; //Creating radar object to be simulated
    test1.initializeInputData(simRadar); //Assigns parameter data from .txt files above to simuated radar object
    //simRadar.printSimInfo(simRadar);   //Optional function, prints to console all parameter/characteristic data gathered from .txt files to radar object
    
    //Start of simulation
    simRadar.startSimulation(); //Function starts radar simulation for created radar object

    //Post simulation
    //test1.searchDataOutput(simRadar,"outputFiles/searchData.txt"); //Optional function, prints simulation search data to .txt file  
    test1.closeInputFiles(); //Closes all files (from above) used to input data for simulation
    
    return 0;
}
