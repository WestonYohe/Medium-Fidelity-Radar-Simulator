/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description: utility.h contains static functions used for unit conversions throughout simulation. The simulation uses base units
 *              for arithmetic along with degree values and decible values. The class serves as a quick and clean method to convert
 *              such units for calculations.
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef UTILITY_H
#define UTILITY_H
#include <math.h>
#include <vector>
#include <ostream>
#include <string.h>
using namespace std;

class utility {
   public:
    utility(){} 
    ~utility(){}

    static void rectToPolarRads(double Rectangular[]){

        //Resulting polar angles are in radians and elevation is from z to x, not x to z
        double azimuth = atan2(Rectangular[1],Rectangular[0]); //azimuth
        double elevation = atan2(Rectangular[2],(hypot( Rectangular[1],Rectangular[0])));  //elevation
        double range = sqrt(pow(Rectangular[0],2)+pow(Rectangular[1],2)+pow(Rectangular[2],2) ); //range
        Rectangular[0] = azimuth;
        Rectangular[1] = elevation; 
        Rectangular[2] = range;
    }

    static void polarRadsToRect(double Polar[]){
        //Polar values must be in radians, -1.57 gives elevation angle from x to z opposed to z to x
        double x = Polar[2] * sin(1.57-Polar[1]) * cos(Polar[0]); 
        double y = Polar[2] * sin(1.57-Polar[1]) * sin(Polar[0]);
        double z = Polar[2] * cos(1.57-Polar[1]);
        Polar[0] = x;
        Polar[1] = y;
        Polar[2] = z;
    }

    static void radsToDeg(double rads[]){
        double value1 = rads[0] * (180/3.14);
        double value2 = rads[1] * (180/3.14);
        
        rads[0] = value1;
        rads[1] = value2;
        

    }

    static void degToRad(double deg[]){
        double value1 = deg[0] * (3.14/180);
        double value2 = deg[1] * (3.14/180);
        
        deg[0] = value1;
        deg[1] = value2;
        
    }

    static double minsToSec(double mins){
        double sec = mins*60;
        return sec;
    }

    static double miliSectoSec(double miliSec){
        double sec = miliSec/ 1000;
        return sec;
    }

    static double secToMiliSec(double sec){
        double miliSec = sec * 1000;
        return miliSec;
    }
     
    static double wattsTodB(double watts){
        double dB = 10 * log10(watts);
        return dB;
    }

    static double dBtoWatts(double dB){
        double watts = pow(10, dB/10);
        return watts;
    }

    static double mmTodBsm(double mm){
        double dBsm = 10 * log10(mm/1);
        return dBsm;
    }
    
    static double dBsmToMM(double dBsm){
        double mm = pow(10, dBsm/10);
        return dBsm;
    }
   
   static double GhzToHz(double Ghz){
    double hz = Ghz * pow(10,9);
    return hz;
   }

   static double HztoGhz(double hz){
    double Ghz = hz / pow(10,9);
    return Ghz;
   }
   
   static double kiloToBase(double kilo){
    double base = kilo*1000;
    return base;
   }

   static double baseToKilo(double base){
    double kilo = base/1000;
    return kilo;
   }

   static double degSqToSteraian(double degSq){
        double steradian = degSq/3283;
        return steradian;
   }

   private:

};

#endif