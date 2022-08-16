/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/11/2022
 * Description: tracker.h contains the logic for the current radar tracking algorithm/method. Currently, the simulation
 *              uses a simple alpha-beta-gamma algorithm (due to the constant trajectory of targets currently) to predict
 *              target's locations, once detected. The class estimates target's current location and predicts
 *              the next location. The neccassry scanning position (azimuth,elevation) is then calculated from
 *              prediction.
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TRACKER_H
#define TRACKER_H
#include "utility.h"
#include "target.h"
#include "radar.h"
#include <iostream>

using namespace std;

class tracker {
    public:
        tracker(target* detectedTarget, double detectedTime, double filterWeights[3], double azPos, double elPos){
            //Inputs: detectedTarget = target detected in search beam.
                      //detectedTime = time target was detected in search (seconds)
                      //filterWegihts = filter/algorithm weights. 0 = alpha, 1 = beta, 2 = gamma. (unitless)
                      //azPos = current azimuth scanning position to detect target. (degrees)
                      //elPos = current elevation scanning position to detect target. (degrees)


            detection = true;               //Initial detection bool from search detection
            activeTrack = false;            //Confirmation beam has not yet confirmed detected taraget
            target = detectedTarget;        //Detected target
            detectedTime = initial_current_time[0]; //Initial time detected
            nextScanPosition[0] = azPos;    //detected azimuth beam position
            nextScanPosition[1] = elPos;    //detected elevation beam position

            for(int i = 0; i < 3; i++){
                 A_B_GammaWeights[i] = filterWeights[i]; //Setting filter/algorithm weights
                 targetDetectXYZ[i] = target->getCoordXYZ(i); //Initializing initial detected target cartesian coordinates
            }
            
            
        } 
        ~tracker(){}

    //Function intializes tracking filter to give a rough estimation on targets next cartesian and spherical position
    //Units: seconds
    //TO_DO: Rearrange confirmation scanning beam timing so assumedValuesCalculation() can provide actual assumed velocity and acceleration values
    void filterInit(double time){
        
        initial_current_time[1] = time;
        double deltaTime = initial_current_time[1] - initial_current_time[0]; //Displacement from first detection to confirmation beam
        initial_current_time[0] = initial_current_time[1]; //Moves final detection time to initial detection time for future detections

        //Gathering target coordinates to determine rough assumed velocity and acceleration values 
        for(int iter = 0; iter < 3; iter++){
            targetConfirmXYZ[iter] = target->getCoordXYZ(iter);
        }
        assumedValuesCalculation(deltaTime); //Calling function to calculate assumed values
        
        //Assumed values to be used in initializing tracking logic
         double assumedVel[3] = {estVxVyVz[0],estVxVyVz[1],estVxVyVz[2]};
         double assumedAcc[3] = {estAxAyAz[0],estAxAyAz[1],estAxAyAz[2]};
         double currXYZ[3] = {target->getCoordXYZ(0), target->getCoordXYZ(1), target->getCoordXYZ(2)}; //Current target cartesian coordinates
        
        
         //Estimating the targets rough next position from assumed values and displacement in time
         for(int i = 0; i < 3; i++){
            //x,y,z estimate
            estXYZ[i] = currXYZ[i] + (assumedVel[i]*deltaTime)+ (0.5*assumedAcc[i]*pow(deltaTime,2));
            //Vx,Vy,Vz estimate
            estVxVyVz[i] = assumedVel[i] + (assumedAcc[i]*deltaTime);
            //Ax,Ay,Az estimate
            estAxAyAz[i] = assumedAcc[i];
         }
         
         //Converting cartesian coordinates to sphereical coordinates for tracking beam position
         double convertPolar[3]; 
         for(int i = 0; i < 3; i++){
            convertPolar[i] = estXYZ[i];
         }
         //Converting estimated cartesian coordinates to spherical
         utility::rectToPolarRads(convertPolar);
         utility::radsToDeg(convertPolar);
         
         //Converted estimated sphereical coordinates are set as the next scanning position for tracking beam.
         for(int i = 0; i < 3; i++){
            nextScanPosition[i] = convertPolar[i];
         }
      }
      
      
      //Function updates tracking alogrithm to predict targets next cartesian position. Due to the nature of the filter, the more updates the filter recevives, the more precise target estimations become.
      //Units: seconds
      //TO-DO: Change "measure" coordinates to scanning poistions spherical coordiantes converted to cartesian coordinates
      void updateFilter(double time){
        estXYZtoCurrXYZ(); //Converts future predictions as current target position estimations
        initial_current_time[1] = time; //Making current time the "final" time for time displacement calculations
        double deltaTime = initial_current_time[1] - initial_current_time[0]; //Setting time displacement from last track detection to current track detection.
        initial_current_time[0] = initial_current_time[1]; //Moves final detection time to initial detection time for future detections

        double measuredXYZ[3] = {target->getCoordXYZ(0), target->getCoordXYZ(1), target->getCoordXYZ(2)}; //"measured" target coordinates. 
            
         //Estimating targets next cartesian position, velocity and acceleration components using tracking filters
         for(int i = 0; i < 3; i++){
            estXYZ[i] = currXYZ[i] + A_B_GammaWeights[0]*(measuredXYZ[i]-currXYZ[i]);                   //Alpha weight
            estVxVyVz[i] = currVxVyVz[i] + A_B_GammaWeights[1]*((measuredXYZ[i]-currXYZ[i])/deltaTime); //Beta weight
            estAxAyAz[i] = currAxAyAz[i] + A_B_GammaWeights[2]*((measuredXYZ[i]-currXYZ[i])/deltaTime); //Gamma weight
         }
          
        //Converting estimated cartesian coordinates to sphereical coordinates
         double convertPolar[3];
         for(int i = 0; i < 3; i++){
            convertPolar[i] = estXYZ[i];
         }
         utility::rectToPolarRads(convertPolar);
         utility::radsToDeg(convertPolar);

         //Setting estimated spherical coordinates as next scanning position for track beam
         for(int i = 0; i < 3; i++){
            nextScanPosition[i] = convertPolar[i];
         }
         
      }

        //Gets scanning position of track beam
        //Input: 0 = azimuth. 1 = elevation
        //Units: degrees
        double getNextScanPos(int iter){
            return nextScanPosition[iter];
        }

        //Sets active track bool so begin tracking process
        void setActiveTrackBool(bool logic){
            activeTrack = logic;
        }

        //Gets active track bool which determines if tracking profile is in current track
        bool getActivTrackBool(){
            return activeTrack;
        }

        //Converts future cartesian position estimation to current cartesian position estimation
        void estXYZtoCurrXYZ(){
            for(int i = 0; i < 3; i++){
                currXYZ[i] = estXYZ[i];
            }
        }

        //Gets target object which is being tracked
        target* getTarget(){
            return target;
        }


        //Calculates rough assumptions of targets veocity and acceleration components after initiating track profile
        //Inputs: deltaTime = time from search beam detection to confirmation tracking beam detection
        //Units: seconds
        //TO-DO: Currently reduntant due to instananeous confirmation beam after searach detection->target has no displacement under current conditions
        void assumedValuesCalculation(double deltaTime){
            for(int iter = 0; iter < 3; iter++){
                double displacement = targetConfirmXYZ[iter] - targetConfirmXYZ[iter];
                estVxVyVz[iter] = displacement / deltaTime;
                estAxAyAz[iter] = (2*(displacement - (estVxVyVz[iter]*deltaTime))) / (deltaTime * deltaTime);
            }
        }


        private:
        bool detection;     //Used to determine initial search detection on target
        bool activeTrack;   //Determines if tracker is actively in traack
        target* target;     //The target the tracker is traacking
        double initial_current_time[2]; //Start and stop times used for time displacement arithmetic. 0 = initial time. 1 = final time (relative refrence). (seconds)
        double currXYZ[3];      //Current estimation on targets cartesian coordinates. 0 = x-position. 1 = y-position. 2 = z-position. (meters)
        double currVxVyVz[3];   //Current estimation on targets velcoity components. 0 = x-velocity. 1 = y-velocity. 2 = z-velocity. (m/s)
        double currAxAyAz[3];   //Current estimation on targets velcoity components. 0 = x-acceleration. 1 = y-acceleration. 2 = z-acceleration. (m/s^2)
        double estXYZ[3];       //Estimation on targets next cartesian coordinates. 0 = x-position. 1 = y-position. 2 = z-position. (meters)
        double estVxVyVz[3];    //Estimation on targets next velcoity components. 0 = x-velocity. 1 = y-velocity. 2 = z-velocity. (m/s)
        double estAxAyAz[3];    //Estimation on targets next acceleration components. 0 = x-acceleration. 1 = y-acceleration. 2 = z-acceleration. (m/s^2)
        double nextScanPosition[3]; //Spherical scanning position from estimation on targets next cartesian coordinates. 0 = azimuth position(degrees). 1 = elevation position(degrees). 2 = range(not used)(meters)
        double A_B_GammaWeights[3]; //Tracking weights. 0 = alpha. 1 = beta. 2 = gamma. (unitless)
        double targetDetectXYZ[3];  //Target's initial detected cartestian position. 0 = x-position. 1 = y-position. 2 = z-position. (meters)
        double targetConfirmXYZ[3]; //Target's confirmed cartestian position. 0 = x-position. 1 = y-position. 2 = z-position. (meters)
        
};

#endif