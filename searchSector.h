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
    //Input: begin= "leftmost" azimuth position. end = "rightmost" azimuth position
    //Unit: degrees
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
    //Input: 0= "leftmost" azimuth position. 1 = "rightmost" azimuth position
    //Unit: degrees
    double getAzExtent(int iter){
        return azExtent[iter];
    }

    //Sets elevation search extent
    //Input: begin= lowest elevation position. end = highest elevation position
    //Unit: degrees
    void setElExtent(double begin, double end){
        elExtent[0] = begin; //lowest position
        elExtent[1] = end; //highest position
    
    }

    //Gets elevation extent in either lowest or hightest position
    //Input: 0= lowest elevation position. 1 = highest elevation position
    //Unit: degrees
    double getElExtent(int iter){
        return elExtent[iter];
    }

    //Sets search range minimum and maximum extent
    //Units: kilometers
    void setRangeExtent(double minKm, double maxKm){
        //Converting from kilometers to meters
        rangeExtent[0] = utility::kiloToBase(minKm);
        rangeExtent[1] = utility::kiloToBase(maxKm);
    }

    //Gets range extent in either nearest or furthest position
    //Input: 0 = minimum range extent. 1 = maximum range extent
    //Unit: meters
    double getRangeExtent(int iter){
        return rangeExtent[iter];
    }


    //Sets refresh rate of search sector
    //Units: seconds
    void setRefreshRate(double time){
        refreshRate = time;
    }

    //Gets refresh rate of search sector
    //Units: seconds
    double getRefreshRate(){
        return refreshRate;
    }

    //Sets current scanning position of search beam within search sector
    //Units: degrees
     void setScanPOS(double azimuth, double elevation){
        scanPOS[0] = azimuth;
        scanPOS[1] = elevation;
    }

    //Gets current scanning position in either azimuth or elevation position
    //Input: 0 = azimuth scanning position. 1 = elevation scanning position.
    //Units: degrees
    double getScanPOS(int iter){
        return scanPOS[iter];
    }
    
    //Gets sector's dwell time
    //Unit: seconds
    double getDwellTime() {
        return dwellTime;
    }

    //Gets FOVRollOver bool, which determines if azimuth search extent rolls over 360 degrees
    bool getFovRollOverBool(){
        return fovRollOver;
    }

    //Gets sectors angular search volume
    //Unit: degrees^2
    double getAngularSearchVolume(){
        return angularSearchVolume;
    }
///////////////////////////////////////////////////////////////////////////////

    //Initializes/calculates sectors angular search volume.
    void initializeSearchVolume(){
        if(fovRollOver == false){
            angularSearchVolume = abs(azExtent[1]-azExtent[0]) * abs(elExtent[1]-elExtent[0]);
        }
        if(fovRollOver==true){
            angularSearchVolume = abs(azExtent[1]-(azExtent[0]+360)) * abs(elExtent[1]-elExtent[0]);
        }
    }

    //Initializes/calculates sectors dwell time.
    //Inputs: az3db = azimuth search beam half-power-beamwidth. el3db = elevation search beam half-power-beamwidth.
    //Units: degrees
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
    //Input: azInc = azimuth increment/displacement from current position. elInc = elevation increment/displacement from current position.
    //Units: degrees
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
    double azExtent[2];         //Search extent in azimuth frame. 0 = leftmost or furtherest counterclockwise position. 1 = rightmost or furthest clockwise position. (degrees)
    double elExtent[2];         //Search extent in elevation frame. 0 = lowest elevation position. 1 = highest elevation posistion. (degrees)
    double rangeExtent[2];      //Searching range extent. 0 = closest position. 1 = furthest position. (meters)
    double refreshRate;         //Time it takes to search the entire search sector. (seconds)
    double angularSearchVolume; //The angular search volume of the search sector. (degrees^2)
    double dwellTime;           //Time a search beam spends in one scanning position. (seconds)
    double scanPOS[2];          //Current scanning position. 0 = azimuth position. 1 = elvation position. (degrees)
    bool fovRollOver;           //boolean used if the azimuth search extent rolls over 360 degrees.  Example-> extent from 345degrees to 45degrees.

};

#endif