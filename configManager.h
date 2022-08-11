/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description:  configManager.h is responsible for "file" handling. It contains the logic to input .txt files and
 *               sort/gather the data contained in the inputted .txt file. This class then calls other classes setter/getter functions
 *               to assign the data gathered from the .txt files. This class also contains functions to output post-
 *               simulation results/data to a user-specified .txt file.
 */////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H
#include "radar.h"
#include "radarFace.h"
#include "searchSector.h"
#include "target.h"
#include <iostream>
#include <fstream>
#include <string.h> 
#include <vector>

using namespace std;

class configManager {
    public:
        configManager(){} 
        ~configManager(){}

        //Input file functions
        void inputRadarFile(string fileName){
            radarData.open(fileName);   
            if(!radarData.is_open()){
            cout << "Error opening " << fileName;
            exit(1); 
            }                   
        }

        void inputFaceFile(string fileName){
            faceData.open(fileName);   //Radar information file
            if(!faceData.is_open()){
            cout << "Error opening " << fileName;
            exit(1); 
            }                   
        }

        void inputSectorFile(string fileName){
            sectorData.open(fileName);   //Radar information file
            if(!sectorData.is_open()){
            cout << "Error opening " << fileName;
            exit(1); 
            }                   
        }

        void inputTargetFile(string fileName){
            targetData.open(fileName);   //Radar information file
            if(!targetData.is_open()){
            cout << "Error opening " << fileName;
            exit(1); 
            }                  
        }

        //Getter functions
        ifstream& getRadarIfstream(){
            return radarData;
        }

        ifstream& getFaceIfstream(){
            return faceData;
        }

        ifstream& getTargetIfstream(){
            return targetData;
        }
        
        ifstream& getSectorIfstream(){
            return sectorData;
        }

        //Functions containing logic to read input files
        double collectSingleLineData(ifstream &inFile){
            string inputTemp; 

            getline(inFile, inputTemp, '\n');
            if(testForNewLine(inputTemp) == true){
                getline(inFile, inputTemp, '\n');
            }
            
            int pos = inputTemp.find(":");  
            string foundValue = inputTemp.substr(pos+1);
        
            //Converting substring to double
            double value = atof(foundValue.c_str());
            return value;
        }

        double* collectCoordinateData(ifstream &inFile){
            string inputTemp;
            getline(inFile, inputTemp, '\n');
            if(testForNewLine(inputTemp) == true){
                getline(inFile, inputTemp, '\n');
            }

            int pos = inputTemp.find_first_of(":"); 
            string strX = inputTemp.substr(pos+1);
            pos = strX.find_first_of(",");
            string strY = strX.substr(pos+1);
            pos = strY.find_first_of(","); 
            string strZ = strY.substr(pos+1);
    
            //Converting substring to double
            xyzStore[0] = (atof(strX.c_str()));
            xyzStore[1] = (atof(strY.c_str()));
            xyzStore[2] = (atof(strZ.c_str()));
            return xyzStore;
        }

        double* collectInputAngles(ifstream &inFile){
            string inputTemp;

            getline(inFile, inputTemp, '\n');
            if(testForNewLine(inputTemp) == true){
                getline(inFile, inputTemp, '\n');
            }

            int pos = inputTemp.find(":");  
            string strAng = inputTemp.substr(pos+1);
            pos = strAng.find_last_of(","); 
            string strElv = strAng.substr(pos+1);
    
            AzElStore[0] = (atof(strAng.c_str()));
            AzElStore[1] = (atof(strElv.c_str()));
           
            return AzElStore;
        }

        bool testForNewLine(string testedLine){
            int pos = testedLine.find("-");
            if(pos == 0){
                return true;
            }
            return false;
        }

        //Simulation initialization functions
        void initializeInputData(radar &radarSim){
            initializeRadarData(radarSim);
            initializeFaceData(radarSim);
            initializeSectorData(radarSim);
            intializeTargetData(radarSim);
       }

        void initializeRadarData(radar &radarSim){
            //collecting face amount
            temp = collectSingleLineData(radarData);
            if(temp > 8){
                cout << "Error, too many input radar face's " <<endl;
                exit(1); 
            }
            for(int i = 0; i < temp; i++){
                radarFace* face = new radarFace;
                radarSim.addRadarFace(*face);
            }
            //collecting target amount
            temp = collectSingleLineData(radarData);
            for(int i = 0; i < temp; i++){
                target* radarTarget = new target;
                radarSim.addTarget(*radarTarget);
            }
            //TrackingPRF
            temp = collectSingleLineData(radarData);
            radarSim.setTrackPRF(temp);
            //TrackingSNRmin
            temp = collectSingleLineData(radarData);
            radarSim.setTrackingSNRmin(temp);
            //TrackingBeamwidth
            tempPointer = collectInputAngles(radarData);
            radarSim.setTrackBeamwidth(tempPointer);

            //Filter weights (a-b-gamma)
            tempPointer = collectCoordinateData(radarData);
            radarSim.setA_B_gammaWeights(tempPointer);
            //SimulatorRefreshRate
            temp = collectSingleLineData(radarData);
            radarSim.setRefreshRate(temp);
            //RadarSimTime
            temp = collectSingleLineData(radarData);
            radarSim.setSimLength(temp);
        }

        void initializeFaceData(radar &radarSim){
            vector<radarFace*> faceVector = radarSim.getRadarFaceVector();
            for(int i = 0; i < faceVector.size(); i++){
                //Boresight
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setBoresight(tempPointer[0],tempPointer[1]);
                //FovAz
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setAzExtent(tempPointer[0],tempPointer[1]);
                //FovEl
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setElExtent(tempPointer[0],tempPointer[1]);
                 //3db
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setHalfPowerBeamWidth(tempPointer[0],tempPointer[1]);
                //SNRmin
                temp = collectSingleLineData(faceData);
                faceVector[i]->setSNRmin(temp);
                
                //WaveFrequency
                temp = collectSingleLineData(faceData);
                faceVector[i]->setWaveFreq(temp);
                //Bandwidth
                temp = collectSingleLineData(faceData);
                faceVector[i]->setBandwidth(temp);
                //Effective attena area
                temp = collectSingleLineData(faceData);
                faceVector[i]->setEffectiveAttenaArea(temp);
                //Peak Power
                temp = collectSingleLineData(faceData);
                faceVector[i]->setPeakPower(temp);
                //NoiseFigure
                temp = collectSingleLineData(faceData);
                faceVector[i]->setNoiseFigure(temp);
                
                //TotalSystemLoss
                temp = collectSingleLineData(faceData);
                faceVector[i]->setTotalSysLoss(temp);
                
                //AmountSearchSectors
                temp = collectSingleLineData(faceData);
                if(temp>10){
                cout << "Error, too many search sector's " <<endl;
                exit(1); 
                }
                for(int j = 0; j < temp; j++){
                    searchSector* sector = new searchSector;
                    faceVector[i]->addSearchSector(*sector);
                }

                //Initialize functions
                faceVector[i]->initializePowerAvg(radarSim.getTrackPRF());
            }
        }

        void initializeSectorData(radar &radarSim){
            
            vector<radarFace*> faceVector = radarSim.getRadarFaceVector();
            for(int i = 0; i < faceVector.size(); i++){
                vector<searchSector*> sectorVector = faceVector[i]->getSearchSectorVector();
                for(int j = 0; j < sectorVector.size(); j++){
                vector<searchSector*> sectorVector = faceVector[i]->getSearchSectorVector();
                //AzExtent
                tempPointer = collectInputAngles(sectorData);
                sectorVector[j]->setAzExtent(tempPointer[0],tempPointer[1]);
                //ElExtenet
                tempPointer = collectInputAngles(sectorData);
                sectorVector[j]->setElExtent(tempPointer[0],tempPointer[1]);
                //RangeExtent
                tempPointer = collectInputAngles(sectorData);
                sectorVector[j]->setRangeExtent(tempPointer[0],tempPointer[1]);
                //RefreshRate
                temp = collectSingleLineData(sectorData);
                sectorVector[j]->setRefreshRate(temp);

                //Initialize functions
                //Search Volume
                sectorVector[j]->initializeSearchVolume(faceVector[i]->getHalfPowerBeamWidth(0), faceVector[i]->getHalfPowerBeamWidth(1));
                //Dwell
                sectorVector[j]->initializeDwellTime(faceVector[i]->getHalfPowerBeamWidth(0), faceVector[i]->getHalfPowerBeamWidth(1));
                //Random scan pos
                sectorVector[j]->randomizedStarScanPos();
                }
            }
        }

        void intializeTargetData(radar &radarSim){
            vector<target*> targetVector = radarSim.getTargetVector();
            for(int i = 0; i < targetVector.size(); i++){
                //Target Starting Coordinates
                tempPointer = collectCoordinateData(targetData);
                targetVector[i]->initializePOS(tempPointer);
                //Target velcoity 
                tempPointer = collectCoordinateData(targetData);
                targetVector[i]->setVelocityVector(tempPointer[0],tempPointer[1],tempPointer[2]);
                //Target acceleration 
                tempPointer = collectCoordinateData(targetData);
                targetVector[i]->setAccelerationVector(tempPointer[0],tempPointer[1],tempPointer[2]);
                //Target RCS
                temp = collectSingleLineData(targetData);
                targetVector[i]->setRCS(temp);
            }
        }

        

        void searchDataOutput(radar &radarSim, string filename){
            ofstream searchData(filename);

            vector<double> timeVector = radarSim.getSearchDetectTimeVector();
            vector<double> SnrVector = radarSim.getSearchDetectSnrVector();
            vector<double> azPosVector = radarSim.getSearchDetectAzPosVector();
            vector<double> elPosVector = radarSim.getSearchDetectElPosVector();
            vector<int> faceIterVector = radarSim.getSearchDetectFaceIterVector();
            vector<int> sectorIterVector = radarSim.getSearchDetectSectorIterVector();
            
            for(int iter = 0; iter < timeVector.size(); iter++){
                searchData << "Detection(" << azPosVector[iter] <<"," << elPosVector[iter] << ") at: " << timeVector[iter]/60 << "mins on, face " << faceIterVector[iter] << "-> sector " << sectorIterVector[iter] << endl;
            }
             
        }

       
        //End of simulation functions
        void closeInputFiles(){
            radarData.close(); 
            faceData.close();
            sectorData.close();
            targetData.close();
        }

   private:
    ifstream radarData;
    ifstream faceData;
    ifstream targetData;
    ifstream sectorData;
    ofstream trackData;
    double AzElStore[2];    
    double xyzStore[3];
    double* tempPointer;
    double temp;
    
};

#endif