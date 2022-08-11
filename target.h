/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/11/2022
 * Description: target.h contains the properties/characteristics of the targets used in the radar simulation. Such as
 *              target's RCS (Radar-Cross-Section), position, velocity, and acceleration. Additionally, there is logic
 *              to update/change the targets location using basic laws of motion equaitons. Currently target's
 *              characteristics are all static throughout entire simulation, except for target's position.            
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TARGET_H
#define TARGET_H
#include "utility.h"
#include <iostream>
using namespace std;

class target {
    public:
        target(){} 
        ~target(){}

///////////////////////////////////////////////////////////////
//Setter and getter related functions
       
       //Sets current cartesian coordinates position.
       //Unit: meters
        void setCoordXYZ(double x, double y, double z){
            coordXYZ[0] = x;
            coordXYZ[1] = y;
            coordXYZ[2] = z;
            convertCoordXYZtoPolar(coordXYZ); //Additionally converts the coordinates to sphereical form
        }

        //Gets initial starting position values in cartesian form.
        //Input: 0 = x-position. 1 = y-position. 2 = z-position
        //Unit: meters
        double getStartPos(int iter){
            return startPos[iter];
        }

        //Gets current position values in cartesian form.
        //Input: 0 = x-position. 1 = y-position. 2 = z-position
        //Unit: meters
        double getCoordXYZ(int iter){
            return coordXYZ[iter];
        }

        //Gets targets current position in either azimuth, elevation, or range position.
        //Input: 0 = azimuth-position. 1 = y-position. 2 = z-position
        //Unit: azimuth & elevation = degrees. range = meters
        double getCoordPolar(int iter){
            return coordPolar[iter];
        }

        //Sets velocity values in cartesian form
        //Unit: m/s
        void setVelocityVector(double x, double y, double z){
            velocityVector[0] = x;
            velocityVector[1] = y;
            velocityVector[2] = z;
        }

        //Gets velocity values in cartesian form
        //Unit: m/s
        double getVelocityValue(int iter){
            return velocityVector[iter];
        }

        //Sets velocity values in cartesian form
        //Unit: m/s^2
        void setAccelerationVector(double x, double y, double z){
            accelerationVector[0] = x;
            accelerationVector[1] = y;
            accelerationVector[2] = z;
        }

        //Gets velocity values in cartesian form
        //Inputs: 0 = x-position, 1 = y-position, 2 = z-position
        //Unit: m/s^2
        double getAccelerationValue(int iter){
            return accelerationVector[iter];
        }

        //Sets RCS value
        //Unit: dBm^2
        void setRCS(double targetRCS){
            rcs = targetRCS;
        }

        //Gets RCS value
        //Unit: dBm^2
        double getRCS(){
            return rcs;
        }
///////////////////////////////////////////////////////////////////////


        //Function updates targets cartesian position to a new position based on the inputted amount of time change
        //Inputs: time 
        //Units: seconds
        void updateCoordinatesXYZ(double time){
            for(int i = 0; i < 3; i++){             //Loop iterates over each cartesian component
            double distance = ((velocityVector[i]*time) + (0.5*accelerationVector[i]*pow(time,2))); //Distance traveled based on targets current speed and change in time
            coordXYZ[i] = coordXYZ[i] + distance;   //Updates current coordinates
            } 
            convertCoordXYZtoPolar(coordXYZ);       //Updates new coordinates to spherical form as well
        }


        //Function converts current cartesian coordinates to spherical coordinates
        //Inputs: double array[3].  0 = x-position, 1 = y-position, 2 = z-position.
        //Units: meters
        void convertCoordXYZtoPolar(double xyzCoord[3]){
            for(int i = 0; i < 3; i++){     //Loop iterates over each cartesian component
            coordPolar[i] = xyzCoord[i];    //Copies current cartesian coordinates
            }
            
            utility::rectToPolarRads(coordPolar);   //Converts cartesian coordinates to spherical radians
            utility::radsToDeg(coordPolar);         //Converts radians to degrees
            coordPolar[0] = fmod(coordPolar[0]+360,360); //Converts negative values to positive and rids of possible 360degree overflow
        }

        //Function temporarily saves current cartesian coordinates retreived by resetToSavedPos() function.
        void saveCurrentPOS(){
            for(int i = 0; i < 3;i++){
                tempCoordXYZ[i] = coordXYZ[i]; //Copies current cartesian coordiantes to temp variable 
            }
        }

        //Function retrieves saved cartesian coordinate position (from function saveCurrentPoS()) and sets current cartesian position to saved coordinates
        void resetToSavedPOS(){
            setCoordXYZ(tempCoordXYZ[0],tempCoordXYZ[1],tempCoordXYZ[2]); //Setting current cartesian position as saved position
        }

        //Function initializes starting cartesian position and initializes current cartesian and spherical position as starting position. Used before starting simulation 
        //Input: double array[3].  0 = x-position, 1 = y-position, 2 = z-position. 
        //Units: kilometers
        void initializePOS(double XYZcoordinates[3]){

            double XYZtoPolar[3]; //Used for cartesian to polar conversion without modify origianl cartesian values
            for(int i = 0; i < 3; i++){

                coordXYZ[i] = (utility::kiloToBase(XYZcoordinates[i])); //Setting current cartesian position
                startPos[i] = (utility::kiloToBase(XYZcoordinates[i])); //Setting target's starting cartesian position
                XYZtoPolar[i] =(utility::kiloToBase(XYZcoordinates[i])); //Copying current cartesian position to be converted to spherical position 
            }
            convertCoordXYZtoPolar(XYZtoPolar); //Converting and setting current spherical position
        }

    private:
        double startPos[3];             //Target initial starting coordinates in cartesian form. 0 = x-position, 1 = y-position, 2 = z-position. (meters)
        double coordXYZ[3];             //Target coordinates in cartesian coordinates. 0 = x-position, 1 = y-position, 2 = z-position. (meters)
        double coordPolar[3];           //Target coordinates in spherical coordinates. 0 = azimuth position(deg), 1 = polar position(deg), 2 = range position(meter)
        double velocityVector[3];       //Target velocity components in cartesian form. 0 = x-velocity, 1 = y-velocity, 2 = z-velocity. (m/s)
        double accelerationVector[3];   //Target acceleration components in cartesian form. 0 = x-acceleration, 1 = y-acceleration, 2 = z-acceleration. (m/s^2)
        double rcs;                     //Target RCS value (Radar-Cross-Section) (dB^2)
        double tempCoordXYZ[3];         //Used as a refrence/temporary coordinates. (meters)
        
};



#endif