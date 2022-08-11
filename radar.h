/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description: radar.h contains the physical properties/characteristics and fucntionality (search,detect,track) of the radar system being simulated.
 *              radar.h also contains the function, StartSim(), which starts the simulation. Once StartSim() is called, radar.h uses all simulation files
 *              to gather the data/information needed to run the simulation. Thus, radar.h acts as a hub for simulation data.
 */////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef RADAR_H
#define RADAR_H
#include "radarFace.h"
#include "target.h"
#include "tracker.h"
#include "utility.h"

using namespace std;

class radar {
   public:
      radar(){
         time = 0;                  //initializing simulation run time 
         possibleDetections = 0;    //initializing search detection counter
         confirmedDetections = 0;   //initializing track profile counter
      } 
      ~radar(){}
//------------------------------------------------------------------------
//getter and setter related functions

      //Adds radarFace objects to faceVector, storing all radar face objects
      void addRadarFace(radarFace &face){
         faceVector.push_back(&face);
      }

      //Returns vector containing all radar face objects
      vector<radarFace*> getRadarFaceVector(){
         return faceVector;
      }

      //Adds target objects to targetVector, storing all target objects
      void addTarget(target &radarTarget){
         targetVector.push_back(&radarTarget);
      }

      //Returns targetVector, containing all target objects
      vector<target*> getTargetVector(){
         return targetVector;
      }

      //Sets length of simulated time simulation will run
      void setSimLength(double time){
         simLength = 60*time; //Converted from minutes to seconds
      }

      //Returns length of simulated time simulation will run
      double getSimLength(){
         return simLength;
      }

      //Sets Tracker PRF
       void setTrackPRF(double kiloHertz){
        trackPRF = utility::kiloToBase(kiloHertz);
      }
      //Returns tracker prf
      double getTrackPRF(){
         return trackPRF;
      }
      //Sets minimum SNR needed for tracker detection
      void setTrackingSNRmin(double snr){
         trackingSNRmin = snr;
      }
      //Gets minimum SNR needed for tracker detection
      double getTrackingSNRmin(){
         return trackingSNRmin;
      }

      //Sets weighted parameters for alpha-beta-gamma tracking filter
      void setA_B_gammaWeights(double weights[3]){
         A_B_gammaWeights[0] = weights[0];  //Alpha
         A_B_gammaWeights[1] = weights[1];  //Beta
         A_B_gammaWeights[2] = weights[2];  //Gamma
      }

      //Gets weighted parameters for alpha-beta-gamma tracking filter
      double getA_B_gammaWeights(int iter){
         return A_B_gammaWeights[iter];
      }

      //Sets refreshRate of simulated time frames/intervals
      void setRefreshRate(double seconds){
         refreshRate = seconds;
      }

      //Gets refreshRate of simulated time frames/intervals
      double getRefreshRate(){
         return refreshRate;
      }

      //Sets tracker halfpower beamwidth in azimuth and elevation
      void setTrackBeamwidth(double beamwidths[2]){
         trackBeamWidth[0] = beamwidths[0]; //azimuth halfpower beamwidth
         trackBeamWidth[1] = beamwidths[1];  //elevation halfpower beamwidth
      }

      //Gets tracker halfpower beamwidth or azimuth and elevation
      double getTrackBeamwidth(int iter){
         return trackBeamWidth[iter];
      }

   //Getters below are for configurationManager which is used to display all search detection data to user in specified .txt file

      //Gets vector with all search detection times
      vector<double> getSearchDetectTimeVector(){
         return searchDetecTimeVector;
      }
      //Gets vector with all search detection snr values
      vector<double> getSearchDetectSnrVector(){
         return searchDetectSnrVector;
      }
      //Gets vector with all search detection azimuth scan positions
      vector<double> getSearchDetectAzPosVector(){
         return searchDetecAzPosVector;
      }
      //Gets vector with all search detection elevation scan positions
      vector<double> getSearchDetectElPosVector(){
         return searchDetecElPosVector;
      }
      //Gets vector storing radar faces that made search detections
      vector<int> getSearchDetectFaceIterVector(){
         return searchDetecFaceIterVector;
      }
      //Gets vector storing search sectors that made search detections
      vector<int> getSearchDetectSectorIterVector(){
         return searchDetecSectorIterVector;
      }

      //Adds/sets a new tracking profiles to trackVector. Inputs target to track, time of detection, current azimuth and elevation scanning positions
      void addTrack(target* detectedTarget, double detectedTime, double azScan, double elScan){         
            tracker* detection = new tracker(detectedTarget,detectedTime,A_B_gammaWeights,azScan,elScan);
            trackVector.push_back(detection);
      }

      //Gets the current track profile of an inputted target
      tracker* getTrack(target *detectedTarget){
         for(int trackIter = 0; trackIter < trackVector.size(); trackIter++){
            if(trackVector[trackIter]->getTarget() == detectedTarget ){
               tracker* track = trackVector[trackIter];
               return track;
            }
         }
         exit(1);
      }

      //Gets the current numeric iterator value for a radar's face in the faceVector
      int getFaceIter(radarFace* face){
         for(int iter = 0; iter < faceVector.size(); iter++){
            if(faceVector[iter] == face){
               return iter;
            }
         }
         exit(1);
      }
//////////////////////////////////////////////////////////////////////////////////////////////////


/*//////////////////////////////////////////////////////////////////////////////
 * Description: StartSimulation() function contains all the logic to actively run
                the radar simulations. Once this function is called, the simulation
                begins. The function runs for a user-inputted amount
                of time (simLength) and effectivley search, detects and tracks
                targets using all the parameters inputted by the user .txt files.
 *//////////////////////////////////////////////////////////////////////////////
      void startSimulation(){
         cout << "Starting simulation..." << endl << endl;
         
         //Loops over simulation length inputted by user. Effectively the length of active simulation
         while(time <= simLength){          
            
            //Loops through all track profiles to continue/update current track 
            for(int detectIter = 0; detectIter < trackVector.size(); detectIter++){ 
               
               //If target is detected in estimated coordinates (calculated from tracker class),
               //tracking profile is updated and simulation time is increased by tracking beam duration
               if(trackDetection(trackVector[detectIter]) == true){ 
                  trackVector[detectIter]->updateFilter(time);
                  time = time + (1/trackPRF);
               }
               //If target is not detected in estimated coordinates, track profile will no longer update/refresh.
               //User is notified
               else{
                  if(trackVector[detectIter]->getActivTrackBool() == true){
                     trackVector[detectIter]->setActiveTrackBool(false);
                     cout << "Lost track of target at: ";
                     printCurrentPos_Time(trackVector[detectIter]->getTarget(), time);
                  }
               }
               //updating all targets by tracking beam durations
               for(int targetIter = 0; targetIter < targetVector.size(); targetIter++){
                  targetVector[targetIter] ->updateCoordinatesXYZ(1/trackPRF);
               }
            }

            //Loop iterates over every radar face in same time interval/refrence. This effectively
            //makes all faces simulatenous operate after refrence is over and overall simulation is updated by interval length
            for(int faceIter = 0; faceIter < faceVector.size(); faceIter++){

               //Saving targets coordinates before entering interval frame for current radar face
               for(int targetIter = 0; targetIter < targetVector.size(); targetIter++){ 
                  targetVector[targetIter]->saveCurrentPOS();
               }
               
               
               radarFace* face = faceVector[faceIter];            //Current radar face being simulated 
               vector<searchSector*> sectorVector = faceVector[faceIter]->getSearchSectorVector(); //Vector containing radar face's search sectors
               double tempTime = time;                            //Temporary variable containing simulation time before entering interval frame
               double intervalTime = 0;                           //Resetting/initializing interval/frame time
               
               //During loop, the current radar face searches over its search sectors "looking" for detections
               while(intervalTime < refreshRate){ 
                  
                  //Setting the current sector which a search beam will be sent.
                  //Once one search beam is sent in sector, radar face will send next search beam
                  //in next search sector.
                  searchSector* sector = sectorVector[face->getSectorVectorIter()];
                  
                  double additionalPri = 0; //Storing the additional time spent sending tracking/confirmation beams upon possible detection

                  //Loop iterates through all simulated targets and tests if current search beam is within targets position
                  //and within detection parameters, (e.g. snr value).
                  for(int targetIter = 0; targetIter < targetVector.size(); targetIter++){
       
                     //If target is detected within current search beam, logic below determines if the
                     //target already has a tracker profile or if detection is new. This is to avoid 
                     //creating multiple tracking files, speeding simulation duration
                     if(searchDetection(face,sector, targetVector[targetIter])==true){
                        possibleDetections = possibleDetections + 1; //Adds to possbile detections counter
      
                       
                        //If target already has a track profile but tracker lost track, make a new track profile
                        if(targetRedetection(targetVector[targetIter]) == true){
                           tracker* newTrack = getTrack(targetVector[targetIter]);
                           confirmationScan(newTrack, tempTime);
                           additionalPri = additionalPri + 1/trackPRF;
                           //TO-DO: Instead of recreating new track, turn-on/update old track? 
                        }
                        //Logic below creates new track profile for detected target
                        else{
                           if(targetNewDetection(targetVector[targetIter])==false){
                              addTrack(targetVector[targetIter],tempTime,sector->getScanPOS(0),sector->getScanPOS(1));
                              tracker* newTrack = getTrack(targetVector[targetIter]);
                              confirmationScan(newTrack, tempTime); //Confirmation beam sent to same location
                              additionalPri = additionalPri + 1/trackPRF; //Update time by track beam duration
                             
                           }
                        }
                     }
                     //Updating target position by search beam duration (DwellTime) and confirmation/track beam duration (1/trackPRF = PRI)
                     targetVector[targetIter]->updateCoordinatesXYZ(sector->getDwellTime() + additionalPri); //update targetPOS from dwell
                  }

                  
                  
                  sector->incrementBeamPos(face->getHalfPowerBeamWidth(0),face->getHalfPowerBeamWidth(1)); //Positioning sector's scanning posiiton for next search beam
                  //Updating tempTime and current interval time
                  tempTime = tempTime + sector->getDwellTime() + additionalPri; //Update intime by dwell
                  intervalTime = intervalTime + sector->getDwellTime()+  additionalPri; //Increment time by dwell
                  face->incrementSectorVectorIter(); //Increment radar face search sector, for which the next search beam will be sent on
                  
                  //Logic below will be used for the small remaining intervalTime
                  if(intervalTime >= refreshRate){
                     double remainingDwell = intervalTime - refreshRate;
                     //TO-DO: Create logic for remaining dwelltime/overflow
                  }


               }
               //Resetting target position back to position before interval frame
               for(int targetIter = 0; targetIter < targetVector.size(); targetIter++){  
                  targetVector[targetIter]->resetToSavedPOS();
               }
               
            }
            
            //Updating target coordinates to position after interval frame
            for(int targetIter = 0; targetIter < targetVector.size(); targetIter++){ 
               targetVector[targetIter]->updateCoordinatesXYZ(refreshRate);
            }
            
            time = time + refreshRate; //Updating overall simulaton time from time spent in interval refrence

            //Logic will be used for simulation time overflow
            if(time >= simLength){
            //TO-DO:Create logic for remaining simulation time
            }
            
         }
         //Post simulation output to user
         cout << "Simulation concluded at " << time/60 <<" minutes." << endl;
         cout << possibleDetections <<  " search hit(s)" << endl;
         cout << confirmedDetections << " track initiation(s)" << endl;
      }
      


   /*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      *Description: searchDetection function contains logic to determine if a current search beam detects a possible target.
                     The criteria for detection is if the received SNR value is above the radar face's minimum detected value, if the
                     inputted target is within the sectors range extent, and if the target is within the current search sectors search
                     beam. If the target fulfils criteria, a detection is made. Criteria for if a target is within the sectors FOV is not
                     needed because of sector logic permitting sending search beams outside of sectors fov extents.
   *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   bool searchDetection(radarFace* face, searchSector* sector, target* target){
      if(searchSNR(face, sector,target) >= face->getSNRmin()){  //SNR criteria
         if((target->getCoordPolar(0) >= (sector->getScanPOS(0) - (face->getHalfPowerBeamWidth(0)/2)))  && (target->getCoordPolar(0) <= (sector->getScanPOS(0)+(face->getHalfPowerBeamWidth(0)/2)))) { //azimuth half-power-beam criteria     
            if(((target->getCoordPolar(1) <= sector->getScanPOS(1)+ (face->getHalfPowerBeamWidth(1)/2))) && (target->getCoordPolar(1) >= sector->getScanPOS(1)-(face->getHalfPowerBeamWidth(1)/2))){  //elevation half-power-beam criteria            
               if(target->getCoordPolar(2) >= sector->getRangeExtent(0) && target->getCoordPolar(2) <= sector->getRangeExtent(1)){ //range extent criteria                
                  storeSearchDetectInfo(face,sector,target); //Add current scanning info for detection
                  return true; //Detection made
               }
            } 
         }
      }
      return false;
   }

      
      //Function determines if a tracking profile already exists for an inputted target
      bool targetNewDetection(target *detectedTarget){
         for(int trackIter = 0; trackIter < trackVector.size(); trackIter++){
            if(trackVector[trackIter]->getTarget() == detectedTarget ){     //Determining repeat tracking profile for target        
               return true;
            }
         }
         return false;
      }

      //Function determines if an inputted target had an existing tracking profile
      bool targetRedetection(target *detectedTarget){
         for(int trackIter = 0; trackIter < trackVector.size(); trackIter++){
            if(trackVector[trackIter]->getTarget() == detectedTarget && trackVector[trackIter]->getActivTrackBool() == false){  //Determining existing tracking profile for target
               return true;
            }
         }
         return false;
      }

      
/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   *Description: Function is called after a search detection is made. A tracking beam is sent to the same position 
               that made initial search detection to confirm the detection as a target. If the confirmation beam results in a detection
               the tracker will estimate the targets next position and start actively tracking the target.

      TO-DO: Introduce some delay between search detection beams and confirmation beams to get a better intial predection of targets
               velocity and acceleration. Currently the two beams are instaneous giving target displacement.
*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      void confirmationScan(tracker *track, double currentTime){
        
         if(trackDetection(track)==true){
            track->setActiveTrackBool(true);
            track->filterInit(currentTime);
            
            cout << "Target detected, starting track at: ";
            printCurrentPos_Time(track->getTarget(),currentTime);
            confirmedDetections = confirmedDetections + 1;
         }
      }
      
/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   *Description: trackDetection function contains the logic to make a detection during tracking phases. The criteria for a track detection is if
                  the tracked target is within any radar's face FOV, if the received SNR is above the trackers minimum detectable value, and
                  if the target is currently within the tracking beam predicted by the tracker filter/algorithm. If target meets all criteria
                  a tracking detection is made and the tracker is updated.
   
   TO-DO: It seems like tracker randomly looses track on some targets with higher velocity and acceleration, even if the target still meets
         all criteria. The azimuth rollover logic below may be the issue. 

*//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
      bool trackDetection(tracker* track){
         target* trackedTarget = track->getTarget();
         for(int faceIter = 0; faceIter < faceVector.size(); faceIter++){
             if(trackSNR(faceVector[faceIter],trackedTarget) >= trackingSNRmin){
               if((trackedTarget->getCoordPolar(0) >= (track->getNextScanPos(0) - (trackBeamWidth[0]/2)))  && (trackedTarget->getCoordPolar(0) <= (track->getNextScanPos(0) +(trackBeamWidth[0]/2)))) { //azimuth beam
                  if(((trackedTarget->getCoordPolar(1) <= track->getNextScanPos(1) + ((trackBeamWidth[1]/2))) && (trackedTarget->getCoordPolar(1) >= track->getNextScanPos(1)-(trackBeamWidth[1]/2)))){  //elevation beam
                     if(trackedTarget->getCoordPolar(1) >= faceVector[faceIter]->getElExtent(0) && trackedTarget->getCoordPolar(1) <= faceVector[faceIter]->getElExtent(1)){     
                        if(faceVector[faceIter]->getFovRollOverBool() == true){
                           if(trackedTarget->getCoordPolar(0) <= faceVector[faceIter]->getAzExtent(0) && trackedTarget->getCoordPolar(0) <= faceVector[faceIter]->getAzExtent(1)){
                              return true;
                           }
                        }
                        else if(faceVector[faceIter]->getFovRollOverBool() == false){
                           if(trackedTarget->getCoordPolar(0) >= faceVector[faceIter]->getAzExtent(0) && trackedTarget->getCoordPolar(0) <= faceVector[faceIter]->getAzExtent(1)){
                              return true;
                           }
                        }
                     }
                  }
               }
            }
         }
         return false;
      }




      //Function contains the "track" form of the Radar Range Equation (RRE) to determine received SNR in units of decible
      double trackSNR(radarFace* face, target* target){
         double snr = 10*log10(face->getPeakPower())  + 20*log10(face->getEffectiveAttenaArea() /3283) + target->getRCS()
          - 30*log10(3.14*4) - 40*log10(target->getCoordPolar(2)) - 10*log10(1.38*pow(10,-23)) - 10*log10(290) -  face->getTotalSysLoss() - face->getNoiseFigure() - 20*log10(face->getWavelength());
         face->setReceivedSNR(snr);
         return snr;
      }
      
      //Function contains the "search" form of the Radar Range Equation (RRE) to determine received SNR in units of decible
      double searchSNR(radarFace* face, searchSector* sector, target* target){
         double snr = 10*log10(face->getPowerAvg()) + 10*log10(face->getEffectiveAttenaArea()/3283) + 10*log10(sector->getRefreshRate()) + target->getRCS()
          - 10*log10(3.14*4) - 10*log10(sector->getAngularSearchVolume() /3283) - 40*log10(target->getCoordPolar(2)) - 10*log10(1.38*pow(10,-23)) - 10*log10(290) -  face->getTotalSysLoss() - face->getNoiseFigure();
          face->setReceivedSNR(snr);
         return snr;
      }

      //Function displays all target, radar, radarFace, and search sector parameters to user's console
      void printSimInfo(radar &radarSim ){
         
         cout <<"Simulated Radar: " << endl;
         cout <<"Simulating " << faceVector.size() << " faces"<< endl;
         cout <<"Simulating " << targetVector.size() << " targets"<< endl;
         cout <<"Tracking PRF= (" << trackPRF << ")Hz" << endl;
         cout <<"Tracking minimum SNR = (" << trackingSNRmin << ")dB" << endl;
         cout <<"Tracking beamwidth= (" << trackBeamWidth[0] << "," << trackBeamWidth[1] << ")degree" << endl;
         cout <<"Tracking filter weights= (" << A_B_gammaWeights[0] << ","  << A_B_gammaWeights[1] << "," <<A_B_gammaWeights[2] << ")" <<endl;
         cout <<"Simulation frame refresh rate= (" << refreshRate << ")sec" << endl;
         cout <<"Simulation run duration= (" << simLength << ")sec" << endl << endl;
         cout << "---------------------------------------------------------------------------------" << endl << endl;
         
         cout << "Targets" << endl;
         for(int targetIter = 0; targetIter < targetVector.size(); targetIter++){
            cout << "Target #" << targetIter + 1 << ":" << endl;
            cout << "Target RCS= (" << targetVector[targetIter]->getRCS() <<")dBm^2" << endl;       
            cout << "Target starting Position XYZ= (" << targetVector[targetIter]->getStartPos(0) << ","<< targetVector[targetIter]->getStartPos(1) <<"," <<targetVector[targetIter]->getStartPos(2) <<")m" << endl;
            cout << "Target velocity= (" << targetVector[targetIter]->getVelocityValue(0) << ","<< targetVector[targetIter]->getVelocityValue(1) <<"," <<targetVector[targetIter]->getVelocityValue(2) <<")m/s" << endl;
            cout << "Target acceleration= (" << targetVector[targetIter]->getAccelerationValue(0) << ","<< targetVector[targetIter]->getAccelerationValue(1) <<"," <<targetVector[targetIter]->getAccelerationValue(2) <<")m^s/s" << endl;
            cout << "------------------------------------------------------------------------------------" << endl << endl;
         }

         cout << "Radar faces" << endl;
         for(int faceIter = 0; faceIter < faceVector.size(); faceIter++){
            cout << "Face #" << faceIter + 1 << ":" << endl;
            cout << "Boresight= (" << faceVector[faceIter]->getBoresight(0) <<"," << faceVector[faceIter]->getBoresight(1) << ")degree" << endl;          
            cout << "FOV extent azimuth= (" << faceVector[faceIter]->getAzExtent(0) <<"," << faceVector[faceIter]->getAzExtent(1) << ")degree" << endl;
            cout << "FOV extent elevation= (" << faceVector[faceIter]->getElExtent(0) <<"," << faceVector[faceIter]->getElExtent(1) << ")degree" << endl;
            cout << "3dB beamwidth= (" << faceVector[faceIter]->getHalfPowerBeamWidth(0) << ")degree" << endl;
            cout << "Minimum detectable SNR= (" << faceVector[faceIter]->getSNRmin() << ")dB" << endl;
            cout << "Wavefrequency= (" << faceVector[faceIter]->getWaveFreq() << ")Hz" << endl;
            cout << "Bandwidth= (" << faceVector[faceIter]->getBandwidth() << ")Hz" << endl;
            cout << "Effective attena area= (" << faceVector[faceIter]->getEffectiveAttenaArea() << ")m^2" << endl;
            cout << "Peak power= (" << faceVector[faceIter]->getPeakPower() << ")kW" << endl;
            cout << "Average power= (" << faceVector[faceIter]->getPowerAvg() << ")kW" << endl;
            cout << "Noise figure= (" << faceVector[faceIter]->getNoiseFigure() << ")dB" << endl;
            cout << "Total system loss= (" << faceVector[faceIter]->getTotalSysLoss() << ")dB" << endl;
            

            vector<searchSector*> sectorVector = faceVector[faceIter]->getSearchSectorVector();
            cout <<  sectorVector.size() << " search sectors for face" << endl;
            for(int sectorIter = 0; sectorIter < sectorVector.size(); sectorIter++){
               cout << "Sector " << sectorIter+1 << " for face " << faceIter + 1 << ":"<< endl;
               cout <<"Current scan position= (" << sectorVector[sectorIter]->getScanPOS(0) << "," << sectorVector[sectorIter]->getScanPOS(1) << ")degrees" << endl;
               cout <<"Azimuth extent= (" << sectorVector[sectorIter]->getAzExtent(0) << "," << sectorVector[sectorIter]->getAzExtent(1) << ")degrees" << endl;
               cout <<"Elevation extent= (" << sectorVector[sectorIter]->getElExtent(0) << "," << sectorVector[sectorIter]->getElExtent(1) << ")degrees" << endl;
               cout <<"Range extent= (" << sectorVector[sectorIter]->getRangeExtent(0) << "," << sectorVector[sectorIter]->getRangeExtent(1) << ")Km" << endl;
               cout <<"Refresh Rate= (" << sectorVector[sectorIter]->getRefreshRate() << ")Sec" << endl;
               cout <<"Angular search volume= (" << sectorVector[sectorIter]->getAngularSearchVolume() << ")degree^2" << endl;
               cout << "Dwell Time= (" << sectorVector[sectorIter]->getDwellTime() <<")Sec" << endl<<endl;;
               
            }
            cout << "--------------------------------------------------------------------------------------" << endl;
         }
         
      }

      //Function is used to display a targets current coordinates and current simulation time to the user's console. Function is called when a target starts being tracked
      //or when a target looses track.
      void printCurrentPos_Time(target* trackedTarget, double time){
         cout << "position XYZ: (" << trackedTarget->getCoordXYZ(0) << ","<< trackedTarget->getCoordXYZ(1) <<"," <<trackedTarget->getCoordXYZ(2) <<")m at " << time/60 << "mins" << endl;
      }

      //Upon a search detection, the face,sector,time,snr and scanning position is recorded and saved for .txt output specified by user using "searchDataOutput" function in configManager 
      void storeSearchDetectInfo(radarFace* face, searchSector* sector, target* target){
         searchDetecFaceIterVector.push_back(getFaceIter(face));
         searchDetecSectorIterVector.push_back(face->getSectorVectorIter());
         searchDetecTimeVector.push_back(time);
         searchDetectSnrVector.push_back(face->getReceivedSNR());
         searchDetecAzPosVector.push_back(sector->getScanPOS(0));
         searchDetecElPosVector.push_back(sector->getScanPOS(1)); 
      }

      

      


    private:
         vector<radarFace*> faceVector;   //Contains all the radar face objects in current radar simulation
         vector<target*> targetVector;    //Contains all target objects simulate in current simulation
         vector<tracker*> trackVector;    //Contains all tracking profiles
         double simLength;                //Simulated time spent running simulation
         double time;                     //Current simulation time
         double refreshRate;              //Time simulation spends in a frame of refrence
         double trackBeamWidth[2];        //HalfPower beamwidth used for tracking. 0 = azimuth beamwidth. 1 = elevation beamwidth
         double trackPRF;                 //Theoretical amount of scans used in tracking per second
         double trackingSNRmin;           //Floor threshold for Signal-to-Noise ratio during tracking
         double A_B_gammaWeights[3];      //Alpha-Beta-Gamma weights for tracking filter. 0 = alpha. 1 = beta. 2 = gamma.
         
         //Variables used for outputting information
         int possibleDetections;                   //Holds all detections from search
         int confirmedDetections;                  //Holds all detections that resulted in a track
         vector<double> searchDetecTimeVector;     //Stores time values for all detections made in search
         vector<double> searchDetectSnrVector;     //Stores SNR values of all detections made in search
         vector<double> searchDetecAzPosVector;    //Stores azimuth scan position when search detection was made
         vector<double> searchDetecElPosVector;    //Stores elevation scan position when search detection was made
         vector<int> searchDetecFaceIterVector;    //Stores the radar face that made search detection
         vector<int> searchDetecSectorIterVector;  //Stores the search sector that made search detection       
};

#endif