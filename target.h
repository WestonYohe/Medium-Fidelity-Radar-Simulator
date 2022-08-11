/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
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
//getter and setter related functions
        void initializePOS(double XYZcoordinates[]){

            double XYZtoPolar[3];
            for(int i = 0; i < 3; i++){

                coordXYZ[i] = (utility::kiloToBase(XYZcoordinates[i]));
                startPos[i] = (utility::kiloToBase(XYZcoordinates[i]));
                XYZtoPolar[i] =(utility::kiloToBase(XYZcoordinates[i]));
            }
            convertCoordXYZtoPolar(XYZtoPolar);
        }

        void setCoordXYZ(double x, double y, double z){
            coordXYZ[0] = x;
            coordXYZ[1] = y;
            coordXYZ[2] = z;
            convertCoordXYZtoPolar(coordXYZ);
        }

        double getStartPos(int iter){
            return startPos[iter];
        }

        double getCoordXYZ(int iter){
            return coordXYZ[iter];
        }

        double getCoordPolar(int iter){
            return coordPolar[iter];
        }

        void setVelocityVector(double x, double y, double z){
            velocityVector[0] = x;
            velocityVector[1] = y;
            velocityVector[2] = z;
        }

        double getVelocityValue(int iter){
            return velocityVector[iter];
        }

        void setAccelerationVector(double x, double y, double z){
            accelerationVector[0] = x;
            accelerationVector[1] = y;
            accelerationVector[2] = z;
        }

        double getAccelerationValue(int iter){
            return accelerationVector[iter];
        }

        void setRCS(double targetRCS){
            rcs = targetRCS;
        }

        double getRCS(){
            return rcs;
        }
///////////////////////////////////////////////////////////////////////

        void updateCoordinatesXYZ(double time){
            for(int i = 0; i < 3; i++){
            double distance = ((velocityVector[i]*time) + (0.5*accelerationVector[i]*pow(time,2)));
            coordXYZ[i] = coordXYZ[i] + distance;
            } 
            convertCoordXYZtoPolar(coordXYZ);
        }


        void convertCoordXYZtoPolar(double xyzCoord[3]){
            for(int i = 0; i < 3; i++){
            coordPolar[i] = xyzCoord[i];
            }
            
            utility::rectToPolarRads(coordPolar);
            utility::radsToDeg(coordPolar);
           
            coordPolar[0] = fmod(coordPolar[0]+360,360);
            
        }


        void saveCurrentPOS(){
            for(int i = 0; i < 3;i++){
                tempCoordXYZ[i] = coordXYZ[i];
            }
        }

        void resetToSavedPOS(){
            setCoordXYZ(tempCoordXYZ[0],tempCoordXYZ[1],tempCoordXYZ[2]);
        }

    private:
        double coordXYZ[3];
        double coordPolar[3];
        double velocityVector[3];
        double startPos[3];
        double accelerationVector[3];
        double rcs;
        double tempCoordXYZ[3];
        
};



#endif