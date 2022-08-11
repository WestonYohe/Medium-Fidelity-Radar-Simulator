/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description: 
 *            
 *                
 *             
 * 
 * 
 * 
 * 
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include "configManager.h"
#include <time.h>

using namespace std;

int main() {
   srand(time(0));
   configManager test1;
    test1.inputRadarFile("RadarInfo.txt");
    test1.inputFaceFile("FaceInfo.txt");
    test1.inputSectorFile("SectorInfo.txt");
    test1.inputTargetFile("TargetInfo.txt");

    
   radar simRadar;
    test1.initializeInputData(simRadar);
    //simRadar.printSimInfo(simRadar);
    simRadar.startSimulation();
    //test1.searchDataOutput(simRadar,"searchData.txt");
    test1.closeInputFiles();
    
    return 0;
}
