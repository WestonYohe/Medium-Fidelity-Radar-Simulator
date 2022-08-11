/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/11/2022
 * Description: searchSector.h contains the searching characteristics/parameters for a given radar face. A radar face 
 *              is capable of having many unique search sectors within a radar face's FOV (field-of-view) extent.
 *              Such characteristics include sectors searching FOV, time needed to search entire FOV, and the 
 *              current scanning position in azimuth and elevation. Currently all sectors are
 *              initialized to a random scanning position within their specific FOV, but increments scanning positions
 *              the same way.
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SEARCHSECTOR_H
#define SEARCHSECTOR_H
#include "utility.h"
#include <iostream>

using namespace std;

class searchSector {
   public:
    searchSector(){}
    ~searchSector(){}

////////////////////////////////////////////////////////////////////////////////////
//Setter and getter related functions

    //Sets azimuth search extent and determines if the extent rolls over 360 degrees
    void setAzExtent(double begin, double end){
            azExtent[0] = begin; //leftmost position
            azExtent[1] = end;  //rightmost position
            if(azExtent[1]< azExtent[0]){
                fovRollOver = true; //Search extent rolls over
            }
            else{
                fovRollOver = false; //Search extent does not roll over
            }
    }

    //Gets azimuth search extent in either the leftmost or rightmost position
    double getAzExtent(int iter){
        return azExtent[iter];
    }

    //Sets elevation search extent
    void setElExtent(double begin, double end){
        elExtent[0] = begin; //lowest position
        elExtent[1] = end; //highest position
    
    }

    //Gets elevation extent in either lowest or hightest position
    double getElExtent(int iter){
        return elExtent[iter];
    }

    //Sets search range extent
    void setRangeExtent(double minKm, double maxKm){
        //Converting from kilometers to meters
        rangeExtent[0] = utility::kiloToBase(minKm);
        rangeExtent[1] = utility::kiloToBase(maxKm);
    }

    //Gets range extent in either nearest or furthest position
    double getRangeExtent(int iter){
        return rangeExtent[iter];
    }


    //Sets refresh rate of search sector
    void setRefreshRate(double time){
        refreshRate = time;
    }

    //Gets refresh rate of search sector
    double getRefreshRate(){
        return refreshRate;
    }

    //Sets current scanning position of search beam within search sector
     void setScanPOS(double azimuth, double elevation){
        scanPOS[0] = azimuth;
        scanPOS[1] = elevation;
    }

    //Gets current scanning position in either azimuth or elevation position
    double getScanPOS(int iter){
        return scanPOS[iter];
    }
    
    //Gets sector's dwell time
    double getDwellTime() {
        return dwellTime;
    }

    //Gets FOVRollOver bool, which determines if azimuth search extent rolls over 360 degrees
    bool getFovRollOverBool(){
        return fovRollOver;
    }

    //Gets sectors angular search volume
    double getAngularSearchVolume(){
        return angularSearchVolume;
    }
///////////////////////////////////////////////////////////////////////////////

    //Initializes/calculates sectors angular search volume. Needed for dwell time calculation
    void initializeSearchVolume(double az3db, double el3db){
        if(fovRollOver == false){
            angularSearchVolume = abs(azExtent[1]-azExtent[0]) * abs(elExtent[1]-elExtent[0]);
        }
        if(fovRollOver==true){
            angularSearchVolume = abs(azExtent[1]-(azExtent[0]+360)) * abs(elExtent[1]-elExtent[0]);
        }
    }

    //Initializes/calculates sectors dwell time. az3db = azimuth search beam half-power-beamwidth. el3db = elevation search beam half-power-beamwidth.
    void initializeDwellTime(double az3db, double el3db){
        dwellTime = (refreshRate*az3db*el3db) / angularSearchVolume;
    }


    //Initializes a random starting scanning position within sectors extents
    void randomizedStarScanPos(){

        //If statement is used to determine if azimuth extent rolls over 360, which leads to slight differing arithmetic
        if(fovRollOver ==true){
            int extentLengthAz = (360 + azExtent[1]) - azExtent[0]; //Finds the extent displacement between beginning and end azimuth extent
            double randomPosAz = rand() % extentLengthAz;           //Generates a random number within that extent displacement
            scanPOS[0] = fmod(azExtent[0] + randomPosAz,360);       //Adds that random number the the beginning azimuth extent value and sets as starting scanning position
        }
        else if(fovRollOver == false){
            int extentLengthAz = (azExtent[1] - azExtent[0]); //Same logic as above but without the need to add 360 degrees
            double randomPosAz = rand() % extentLengthAz;
            scanPOS[0] = azExtent[0] + randomPosAz;
        }

        //Same logic as above, but now for elevation scanning position instead of elevation
       int extentLengthEl = elExtent[1] - elExtent[0]; 
       double randomPosEl = rand() % extentLengthEl;
       scanPOS[1] = elExtent[0] + randomPosEl; 
    }


    //Function increments scanning position in search sector once search beams scans current position
    //azInc = azimuth increment/displacement from current position. elInc = elevation increment/displacement from current position.
    void incrementBeamPos(double azInc, double elInc){

        //If statement is used to determine if azimuth extent rolls over 360, which leads to slight differing arithmetic
        if(fovRollOver ==true){

            scanPOS[0] =  fmod(scanPOS[0]+azInc,360); //Increments azimuth scanning position

            if(scanPOS[0] > azExtent[1] && scanPOS[0] < azExtent[0]){ //Determines if current position is outside of sector's azimuth extent
                scanPOS[0] = azExtent[0];           //Resets current azimuth scanning position back to leftmost azimuth extent position
                scanPOS[1] =  scanPOS[1] + elInc;   //Increments elevation scanning position
            }
            if(scanPOS[1] > elExtent[1]){           //Determines if current position is outside of sector's elevation extent
                scanPOS[1] = elExtent[0];           //Positions current elevation scanning position to lowest elevation extent position
            }   
            return;
        }

        //Same functionality as above except for the logic used to determine if the current search beam is outside sectors extents
        if(fovRollOver == false){
            
            scanPOS[0] =  fmod(scanPOS[0]+azInc,360);

            if(scanPOS[0] > azExtent[1]){
                scanPOS[0] = azExtent[0];
                scanPOS[1] =  scanPOS[1] + elInc;
            }
                if(scanPOS[1] > elExtent[1]){
                    scanPOS[1] = elExtent[0];
                }
            return;
        }
    }

private:
    double azExtent[2];         //Search extent in azimuth frame. 0 = leftmost or furtherest counterclockwise position. 1 = rightmost or furthest clockwise position
    double elExtent[2];         //Search extent in elevation frame. 0 = lowest elevation position. 1 = highest elevation posistion
    double rangeExtent[2];      //Searching range extent. 0 = closest position. 1 = furthest position.
    double refreshRate;         //Time it takes to search the entire search sector.
    double angularSearchVolume; //The angular search volume of the search sector
    double dwellTime;           //Time a search beam spends in one scanning position.
    double scanPOS[2];          //Current scanning position. 0 = azimuth position. 1 = elvation position.
    bool fovRollOver;           //boolean used if the azimuth search extent rolls over 360 degrees.  Example-> extent from 345degrees to 45degrees.

};

#endif