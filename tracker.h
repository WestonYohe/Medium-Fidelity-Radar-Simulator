/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
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
            detection = true;
            activeTrack = false;
            target = detectedTarget;
            detectedTime = initial_current_time[0];
            nextScanPosition[0] = azPos;
            nextScanPosition[1] = elPos; 

            for(int i = 0; i < 3; i++){
                 A_B_GammaWeights[i] = filterWeights[i];
                 targetDetectXYZ[i] = target->getCoordXYZ(i);
            }
            
            
        } 
        ~tracker(){}

    void filterInit(double time){
        
        initial_current_time[1] = time;
        double deltaTime = initial_current_time[1] - initial_current_time[0];
        initial_current_time[0] = initial_current_time[1];

        for(int iter = 0; iter < 3; iter++){
            targetConfirmXYZ[iter] = target->getCoordXYZ(iter);
        }

        assumedValuesCalculation(deltaTime);
        

         double assumedVel[3] = {estVxVyVz[0],estVxVyVz[1],estVxVyVz[2]};
         double assumedAcc[3] = {estAxAyAz[0],estAxAyAz[1],estAxAyAz[2]};
         double currXYZ[3] = {target->getCoordXYZ(0), target->getCoordXYZ(1), target->getCoordXYZ(2)};
        
        
         //rough initial predictions for xyz values
         for(int i = 0; i < 3; i++){
            //x,y,z prediction
            estXYZ[i] = currXYZ[i] + (assumedVel[i]*deltaTime)+ (0.5*assumedAcc[i]*pow(deltaTime,2));
            //Vx,Vy,Vz prediction
            estVxVyVz[i] = assumedVel[i] + (assumedAcc[i]*deltaTime);
            //Ax,Ay,Az prediction
            estAxAyAz[i] = assumedAcc[i];
         }
         
         //Converting rect to polar for scan pos
         double convertPolar[3];
         for(int i = 0; i < 3; i++){
            convertPolar[i] = estXYZ[i];
         }
         //Seting new scan pos to estimated target pos
         utility::rectToPolarRads(convertPolar);
         utility::radsToDeg(convertPolar);
         for(int i = 0; i < 3; i++){
            nextScanPosition[i] = convertPolar[i];
         }
      }
      
      
      void updateFilter(double time){
        estXYZtoCurrXYZ();
        initial_current_time[1] = time;
        double deltaTime = initial_current_time[1] - initial_current_time[0];
        initial_current_time[0] = initial_current_time[1];
        double measuredXYZ[3] = {target->getCoordXYZ(0), target->getCoordXYZ(1), target->getCoordXYZ(2)};
            
         //posXYZ is measured coord, currXYZ is guessed coords
         for(int i = 0; i < 3; i++){
            estXYZ[i] = currXYZ[i] + A_B_GammaWeights[0]*(measuredXYZ[i]-currXYZ[i]);
            estVxVyVz[i] = currVxVyVz[i] + A_B_GammaWeights[1]*((measuredXYZ[i]-currXYZ[i])/deltaTime);
            estAxAyAz[i] = currAxAyAz[i] + A_B_GammaWeights[2]*((measuredXYZ[i]-currXYZ[i])/deltaTime);
         }
          
          //Converting rect to polar for scan pos
         double convertPolar[3];
         for(int i = 0; i < 3; i++){
            convertPolar[i] = estXYZ[i];
         }
         utility::rectToPolarRads(convertPolar);
         utility::radsToDeg(convertPolar);
         for(int i = 0; i < 3; i++){
            nextScanPosition[i] = convertPolar[i];
         }
         
      }


        double getNextScanPos(int iter){
            return nextScanPosition[iter];
        }

        void setActiveTrackBool(bool logic){
            activeTrack = logic;
        }

        bool getActivTrackBool(){
            return activeTrack;
        }

        void estXYZtoCurrXYZ(){
            for(int i = 0; i < 3; i++){
                currXYZ[i] = estXYZ[i];
            }
        }

        target* getTarget(){
            return target;
        }

        //Currently reduntant due to instananeous confirmation beam->target has no displacement under current conditions
        void assumedValuesCalculation(double deltaTime){
            for(int iter = 0; iter < 3; iter++){
                double displacement = targetConfirmXYZ[iter] - targetConfirmXYZ[iter];
                estVxVyVz[iter] = displacement / deltaTime;
                estAxAyAz[iter] = (2*(displacement - (estVxVyVz[iter]*deltaTime))) / (deltaTime * deltaTime);
            }
        }


        private:
        bool detection;
        bool activeTrack;
        target* target;
        double initial_current_time[2]; 
        double currXYZ[3];
        double currVxVyVz[3];
        double currAxAyAz[3];
        double estXYZ[3];
        double estVxVyVz[3];
        double estAxAyAz[3];
        double nextScanPosition[3];
        double A_B_GammaWeights[3];
        double targetDetectXYZ[3];
        double targetConfirmXYZ[3];
        
};

#endif