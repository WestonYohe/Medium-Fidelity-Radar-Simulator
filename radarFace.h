/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description: radarFace.h contains the internal properties/characteristics of the radar face's used in the simulation.
 *              Such properties include face's FOV extent, minimum detectable SNR (Singal-to-Noise Ratio),
 *              wavelength, bandwidth, supplied power, along with internal and external losses.
 *               
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef RADARFACE_H
#define RADARFACE_H
#include "searchSector.h"
#include "utility.h"
#include "configManager.h"
#include "target.h"
#include <iostream>
#include <stdlib.h>    
#include <fstream>  
#include <vector>

using namespace std;

class radarFace {

   public:
      radarFace(){
         sectorIter = 0;      //Starts all radar faces with 0th sector, or "First" search sector.
      }
      ~radarFace(){}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Getter and setter related functions

      //Sets face's boresight, which essentially is middle line of sight of radar face
      void setBoresight(double azimuth, double elevation){
            boresight[0] = azimuth;
            boresight[1] = elevation;
         }

      //Gets face's boresight in either azimuth or elevation position
      double getBoresight(int iter){
            return boresight[iter];
         }

      //Sets face's azimuth FOV extent and also determines if FOV rolls over from 360 degrees to 0 degrees 
      void setAzExtent(double begin, double end){
         fovExtentAz[0] = begin;
         fovExtentAz[1] = end;
         if(fovExtentAz[1]< fovExtentAz[0]){
            fovRollOver = true; //FOV rolls over 360 degrees
         } 
         else{
            fovRollOver = false; //FOV does not roll over 360 degrees
         }
      }

      //Gets azimuth FOV extent in either leftmost or rightmost position 
      double getAzExtent(int iter){
         return fovExtentAz[iter];
      }

      //Sets elevation FOV extent
      void setElExtent(double begin, double end){
         fovExtentEl[0] = begin; //Lowest position
         fovExtentEl[1] = end;   //Highest position
      
      }

      //Gets elevation FOV extent in either lowest or highest position 
      double getElExtent(int iter){
         return fovExtentEl[iter];
      }

      //Sets face's half-power-beamwidth for search beams
      void setHalfPowerBeamWidth(double az3dB, double el3dB){
         halfPowerBeamWidth[0] = az3dB; //azimuth 3db beamwidth
         halfPowerBeamWidth[1] = el3dB; //elevation 3db beamwidth
      }

      //Gets half-power-beamwidth in either azimuth or elevation 
      double getHalfPowerBeamWidth(int iter){
         return halfPowerBeamWidth[iter];
      }

      //Sets face's minimum detectable SNR value for search detection
      void setSNRmin(double snr){
         SNRmin = snr;
      }

      //Gets face's minimum detectable SNR value for search detection
      double getSNRmin(){
         return SNRmin;
      }

      //Sets frequency of search waves/beams
      void setWaveFreq(double frequency){
         waveFreq = utility::GhzToHz(frequency); //convertz gigahertz to hertz
      }

      //Gets frequency of search waves/beams
      double getWaveFreq(){
         return waveFreq;
      }

      //Sets bandwidth of search waves/beams
      void setBandwidth(double trasnmitBW){
         bandwith = utility::kiloToBase(trasnmitBW); //Converts kilohertz to hertz
      }

      //Gets bandwidth of search waves/beams
      double getBandwidth(){
         return bandwith;
      }

      //Sets effective attena area of radar face (TO-DO: determine if valid for phased array)
      void setEffectiveAttenaArea(double area){
         effectiveAttenaArea = area;
      }

      //Sets effective attena area of radar face
      double getEffectiveAttenaArea(){
         return effectiveAttenaArea;
      }

      //Sets peak power supplied to radar face
      void setPeakPower(double Kwatts){
         peakPower = utility::kiloToBase(Kwatts); //Converts kilowatts to watts
      }

      //Gets peak power supplied to radar face
      double getPeakPower(){
         return peakPower;
      }

      //Sets noise figure of radar face
      void setNoiseFigure(double noise){
         noiseFigure = noise;
      }

      //Gets nosie figure of radar face
      double getNoiseFigure(){
         return noiseFigure;
      }

      //Sets total system loss for specific radar face
      void setTotalSysLoss(double loss){
         totalSysLoss = loss;
      }

      //Gets total system loss for specific radar face
      double getTotalSysLoss(){
         return totalSysLoss;
      }

      //Adds/sets search sector object to sectorVector. Adds search sector to radar face's search sector list.
      void addSearchSector(searchSector& sector){
         searchSectorVector.push_back(&sector);
      }

      //Gets vector containing all search sector objects for radar face
      vector<searchSector*> getSearchSectorVector(){
         return searchSectorVector;
      }

      //Gets the current sector iterator, which is the current search sector the radar face is scanning in
      int getSectorVectorIter(){
         return sectorIter;
      }

      //Sets the SNR receieved from current search beam
      void setReceivedSNR(double snr){
         receivedSNR = snr;
      }

      //Gets the SNR receieved from current search beam
      double getReceivedSNR(){
         return receivedSNR;
      }

      //Gets the average power supplied to radar face
      double getPowerAvg(){
         return powerAvg;
      }

      //Gets wavelength of search beam/wave of radar face
      double getWavelength(){
         return 299792458/waveFreq; //Speed of light divided by wave frequency
      }

      //Gets FOV rollover boolean to determine if FOV extent goes past 360 degrees
      bool getFovRollOverBool(){
         return fovRollOver;
      }
////////////////////////////////////////////////////////////////////////////////////////////

      //Function iterates the current search sector the radar face is using to search
      void incrementSectorVectorIter(){
         sectorIter = sectorIter + 1;
            if(sectorIter == searchSectorVector.size()){ //If statment determines if current iteration is the last sector before iterating back to first sector
               sectorIter = 0;
            }
      }

      //Function determines face's average power
      void initializePowerAvg(double trackPRF){
         powerAvg = peakPower * (1/bandwith) * trackPRF;
         //TO-DO: determine if trackPRF or if dwellTime is used for equation
      }

      

      private:
         double boresight[2];             //Contains the boresight or "center position" of radar face. 0 = azimuth position. 1 = elevation position
         double fovExtentAz[2];           //Field of view in azimuth frame. 0 = leftmost or furtherest counterclockwise position. 1 = rightmost or furthest clockwise position
         double fovExtentEl[2];           //Field of view in elevation frame. 0 = lowest elevation position. 1 = highest elevation posistion
         double halfPowerBeamWidth[2];    //Searching half-power-beamwidth in azimuth and elevation. 0 = azimuth beamwidth. 1 = elevation beamwidth.
         double SNRmin;                   //Minimum detectable SNR value for search detection    
         double waveFreq;                 //Frequency of search beam 
         double bandwith;                 //Bandwidth of wave beam
         double effectiveAttenaArea;      //Effective attena area (including gain)
         double peakPower;                //Peak power supplied to radar face
         double noiseFigure;              //Internal noise/loss for radar face, in dB
         double totalSysLoss;             //Total system loss, e.g. atmospheric, DSP, transmit/receive losses  
         double powerAvg;                 //Average power supplied to radar face
         int sectorIter;                  //The current search sector which the radar face is searching
         double receivedSNR;              //The received SNR detected by radar face
         bool fovRollOver;                //Boolean determining if azimuth FOV "rolls over" 360. Example-> FOV from 345degrees to 45degrees.
         vector<searchSector*> searchSectorVector; //Vector contains all sector objects for radar face
      };

#endif