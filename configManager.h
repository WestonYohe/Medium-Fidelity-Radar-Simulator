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

        //Inputtting .txt file containing radar.h parameters. Additionally checks for errors in opening file
        void inputRadarFile(string fileName){
            radarData.open(fileName);   //Opening file
            if(!radarData.is_open()){   //Error checking
            cout << "Error opening " << fileName;
            exit(1); 
            }                   
        }

        //Inputtting .txt file containing radarFace.h parameters. Additionally checks for errors in opening file
        void inputFaceFile(string fileName){
            faceData.open(fileName);   //Opening file
            if(!faceData.is_open()){   //Error checking
            cout << "Error opening " << fileName;
            exit(1); 
            }                   
        }

        //Inputtting .txt file containing searchSector.h parameters. Additionally checks for errors in opening file
        void inputSectorFile(string fileName){
            sectorData.open(fileName);   //Opening file
            if(!sectorData.is_open()){   //Error checking
            cout << "Error opening " << fileName;
            exit(1); 
            }                   
        }

        //Inputtting .txt file containing target.h parameters. Additionally checks for errors in opening file
        void inputTargetFile(string fileName){
            targetData.open(fileName);   //Opening file
            if(!targetData.is_open()){   //Error checking
            cout << "Error opening " << fileName;
            exit(1); 
            }                  
        }

        //Gets ifstream object handling .txt file for radar.h parameters 
        ifstream& getRadarIfstream(){
            return radarData;
        }

        //Gets ifstream object handling .txt file for radarFace.h parameters
        ifstream& getFaceIfstream(){
            return faceData;
        }

        //Gets ifstream object handling .txt file for target.h parameters
        ifstream& getTargetIfstream(){
            return targetData;
        }
        
        //Gets ifstream object handling .txt file for searchSector.h parameters
        ifstream& getSectorIfstream(){
            return sectorData;
        }

        //Function gathers first number after ":" delimiter from sinle line of ifstream object (.txt file). 
        double collectSingleLineData(ifstream &inFile){
            string inputTemp; 

            getline(inFile, inputTemp, '\n');       //Gets current line of .txt file
            if(testForNewLine(inputTemp) == true){  //Determines if first character is '-' delimieter
                getline(inFile, inputTemp, '\n');   //If first character is '-', get next line of .txt file
            }
            
            int pos = inputTemp.find(":");  //Finds position of ":" delimeters signifying the start of data
            string foundValue = inputTemp.substr(pos+1); //Gathers single data value after ":" delimeter
            
            double value = atof(foundValue.c_str());     //Converting substring to double
            return value;
        }

        
        //Function gathers "coordinate" data (three values) after first ":" delimiter then two "," delimiters from single line of ifstream object (.txt file).
        double* collectCoordinateData(ifstream &inFile){
            //Same logic as detailed in collectSingleLineData(), but this function collects three data values instead of one
            string inputTemp;
            
            getline(inFile, inputTemp, '\n');
            if(testForNewLine(inputTemp) == true){
                getline(inFile, inputTemp, '\n');
            }

            int pos = inputTemp.find_first_of(":"); 
            string strX = inputTemp.substr(pos+1);  //First data value
            pos = strX.find_first_of(",");          //Finds "," delimeter for next data input
            string strY = strX.substr(pos+1);       //Second data value
            pos = strY.find_first_of(",");          //Finds "," delimeter for next data input
            string strZ = strY.substr(pos+1);       //Third data value
    
            //Converting substrings to double values
            xyzStore[0] = (atof(strX.c_str()));
            xyzStore[1] = (atof(strY.c_str()));
            xyzStore[2] = (atof(strZ.c_str()));
            return xyzStore;
        }

        //Function gathers "angle" data (two values) after first ":" delimiter then value after following "," delimiter from single line of ifstream object (.txt file).
        double* collectInputAngles(ifstream &inFile){
            string inputTemp;
            //Same logic as detailed in collectSingleLineData(), but this function collects two data values instead of one
            getline(inFile, inputTemp, '\n');
            if(testForNewLine(inputTemp) == true){
                getline(inFile, inputTemp, '\n');
            }

            int pos = inputTemp.find(":");  
            string strAng = inputTemp.substr(pos+1); //First data value
            pos = strAng.find_last_of(",");          //Finds "," delimeter for next data input
            string strElv = strAng.substr(pos+1);    //Second data value

            //Converting substrings to double values
            AzElStore[0] = (atof(strAng.c_str()));
            AzElStore[1] = (atof(strElv.c_str()));
           
            return AzElStore;
        }

        //Function test if first character of newline in file is "-" delimeter, signifying the end of parameter input for current object, moving to next object's parameter values
        bool testForNewLine(string testedLine){
            int pos = testedLine.find("-");
            if(pos == 0){
                return true;
            }
            return false;
        }

        //Function initializes all parameters data found in .txt files. Must be called after initializing .txt file
        void initializeInputData(radar &radarSim){
            initializeRadarData(radarSim);
            initializeFaceData(radarSim);
            initializeSectorData(radarSim);
            intializeTargetData(radarSim);
       }

        //Function gathers radar.h .txt data  and calls radar.h setter funcetions to initialize parameters
        void initializeRadarData(radar &radarSim){
            //Collecting face amount
            temp = collectSingleLineData(radarData);
            if(temp > 8){
                cout << "Error, too many input radar face's " <<endl;
                exit(1); 
            }
            for(int i = 0; i < temp; i++){
                radarFace* face = new radarFace;
                radarSim.addRadarFace(*face);
            }
            //Collecting target amount
            temp = collectSingleLineData(radarData);
            for(int i = 0; i < temp; i++){
                target* radarTarget = new target;
                radarSim.addTarget(*radarTarget);
            }
            //Tracking PRF
            temp = collectSingleLineData(radarData);
            radarSim.setTrackPRF(temp);
            //Tracking SNRmin
            temp = collectSingleLineData(radarData);
            radarSim.setTrackingSNRmin(temp);
            //Tracking Beamwidth
            tempPointer = collectInputAngles(radarData);
            radarSim.setTrackBeamwidth(tempPointer);
            //Filter weights (a-b-gamma)
            tempPointer = collectCoordinateData(radarData);
            radarSim.setA_B_gammaWeights(tempPointer);
            //Simulation Refresh Interval/Rate
            temp = collectSingleLineData(radarData);
            radarSim.setRefreshRate(temp);
            //Radar Simulation Time
            temp = collectSingleLineData(radarData);
            radarSim.setSimLength(temp);
        }

        //Function gathers radarFace.h .txt data and calls radarFace.h setter funcetions to initialize parameters
        void initializeFaceData(radar &radarSim){
            vector<radarFace*> faceVector = radarSim.getRadarFaceVector();
            for(int i = 0; i < faceVector.size(); i++){
                //Boresight
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setBoresight(tempPointer[0],tempPointer[1]);
                //Azimuth FOV
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setAzExtent(tempPointer[0],tempPointer[1]);
                //Elevation FOV
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setElExtent(tempPointer[0],tempPointer[1]);
                 //Searching half power beamwidth
                tempPointer = collectInputAngles(faceData);
                faceVector[i]->setHalfPowerBeamWidth(tempPointer[0],tempPointer[1]);
                //Minimum detectable SNR value
                temp = collectSingleLineData(faceData);
                faceVector[i]->setSNRmin(temp);
                //Wave frequency
                temp = collectSingleLineData(faceData);
                faceVector[i]->setWaveFreq(temp);
                //Wave Bandwidth
                temp = collectSingleLineData(faceData);
                faceVector[i]->setBandwidth(temp);
                //Effective attena area
                temp = collectSingleLineData(faceData);
                faceVector[i]->setEffectiveAttenaArea(temp);
                //Peak Power
                temp = collectSingleLineData(faceData);
                faceVector[i]->setPeakPower(temp);
                //Noise Figure
                temp = collectSingleLineData(faceData);
                faceVector[i]->setNoiseFigure(temp);
                //TotalSystemLoss
                temp = collectSingleLineData(faceData);
                faceVector[i]->setTotalSysLoss(temp);
                //Amount of search sectors for radar face
                temp = collectSingleLineData(faceData);
                if(temp>10){
                cout << "Error, too many search sector's " <<endl;
                exit(1); 
                }
                for(int j = 0; j < temp; j++){
                    searchSector* sector = new searchSector;
                    faceVector[i]->addSearchSector(*sector);
                }

                //Initialization functions
                //Initializing power average 
                faceVector[i]->initializePowerAvg(radarSim.getTrackPRF());
            }
        }

        //Function gathers searchSector.h .txt data and calls searchSector.h setter funcetions to initialize parameters
        void initializeSectorData(radar &radarSim){
            
            vector<radarFace*> faceVector = radarSim.getRadarFaceVector();
            for(int i = 0; i < faceVector.size(); i++){
                vector<searchSector*> sectorVector = faceVector[i]->getSearchSectorVector();
                for(int j = 0; j < sectorVector.size(); j++){
                vector<searchSector*> sectorVector = faceVector[i]->getSearchSectorVector();
                //Azimuth Extent
                tempPointer = collectInputAngles(sectorData);
                sectorVector[j]->setAzExtent(tempPointer[0],tempPointer[1]);
                //Elevation Extenet
                tempPointer = collectInputAngles(sectorData);
                sectorVector[j]->setElExtent(tempPointer[0],tempPointer[1]);
                //Range Extent
                tempPointer = collectInputAngles(sectorData);
                sectorVector[j]->setRangeExtent(tempPointer[0],tempPointer[1]);
                //Refresh Rate
                temp = collectSingleLineData(sectorData);
                sectorVector[j]->setRefreshRate(temp);

                //Initialize functions
                //Initializing/calculating search sector search volume
                sectorVector[j]->initializeSearchVolume();
                //Initializing/calculating dwell time 
                sectorVector[j]->initializeDwellTime(faceVector[i]->getHalfPowerBeamWidth(0), faceVector[i]->getHalfPowerBeamWidth(1));
                //Initializing starting scan position
                sectorVector[j]->randomizedStarScanPos();
                }
            }
        }

        //Function gathers and calls target.h setter funcetions to initialize parameters
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

        
        //Function calls radar.h search detection information and outputs data to user specified .txt file
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

       
        //Function closes all opened/inputted .txt files
        void closeInputFiles(){
            radarData.close(); 
            faceData.close();
            sectorData.close();
            targetData.close();
        }

   private:
    ifstream radarData;     //ifstream object handling parameters data for radar.h
    ifstream faceData;      //ifstream object handling parameters data for radarFace.h
    ifstream targetData;    //ifstream object handling parameters data for target.h
    ifstream sectorData;    //ifstream object handling parameters data for searchSector.h
    double AzElStore[2];    //temporary storage variable used for collecting inputting angle (2 inputs) data
    double xyzStore[3];     //temporary storage variable used for collecting inputting coordinate (3 inputs) data
    double* tempPointer;    //temporary pointer variable used for class functions that input pointer values (TODO: modify class functions to not use pointer inputs)
    double temp;            //temporary storage variable used for collecting inputting data
    
};

#endif