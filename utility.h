/*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 * Unclassified
 * Coder: Weston Yohe
 * Last Modified: 8/10/2022
 * Description: utility.h contains static functions used for unit conversions throughout simulation. The simulation uses base units
 *              for arithmetic along with degree values and decible values. The class serves as a quick and clean method to convert
 *              such units for calculations.
 * TO_DO: -Change functions using pass-by-refrence inputs to outputting values using pass-by-value instead
 *        -Delete redundant functions and create a general metric unit conversion function
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


    //Converts radians to spherical coordinates. 
    //Inputs: 0 = azimuth position (radians). 1 = elevation-position(radians). 2 = range-position (meters)
    //Outputs: Does not expicitly return any values. Converts inputted pointer array as pass-by-reference 
    static void rectToPolarRads(double Rectangular[]){

        //Resulting polar angles are in radians and elevation is from z to x, not x to z
        double azimuth = atan2(Rectangular[1],Rectangular[0]); //azimuth
        double elevation = atan2(Rectangular[2],(hypot( Rectangular[1],Rectangular[0])));  
        double range = sqrt(pow(Rectangular[0],2)+pow(Rectangular[1],2)+pow(Rectangular[2],2) ); 
        Rectangular[0] = azimuth;
        Rectangular[1] = elevation; 
        Rectangular[2] = range;
    }

    //Converts radians to cartesian coordinates
    //Inputs: 0 = x-position.(meters) 1 = y-position(meters). 2 = z-position(meters)
    //Outputs: Does not expicitly return any values. Converts inputted pointer array as pass-by-reference 
    static void polarRadsToRect(double Polar[]){
        
        //1.57 gives elevation angle from x to z opposed to z to x
        double x = Polar[2] * sin(1.57-Polar[1]) * cos(Polar[0]); 
        double y = Polar[2] * sin(1.57-Polar[1]) * sin(Polar[0]);
        double z = Polar[2] * cos(1.57-Polar[1]);
        Polar[0] = x;
        Polar[1] = y;
        Polar[2] = z;
    }

    //Converts radians to degrees
    //Inputs: pass-by-refrence doube array[2]
    //TODO: make as single double input with a double return type
    static void radsToDeg(double rads[]){
        double value1 = rads[0] * (180/3.14);
        double value2 = rads[1] * (180/3.14);
        
        rads[0] = value1;
        rads[1] = value2;
    }

    //Converts degrees to radians, only two values
    //Inputs: pass-by-refrence doube array[2]
    //TODO: make as single double input with a double return type
    static void degToRad(double deg[]){
        double value1 = deg[0] * (3.14/180);
        double value2 = deg[1] * (3.14/180);
        
        deg[0] = value1;
        deg[1] = value2;
        
    }

    //Converts minutes to seconds
    static double minsToSec(double mins){
        double sec = mins*60;
        return sec;
    }

    //Converts miliseconds to seconds
    static double miliSectoSec(double miliSec){
        double sec = miliSec/ 1000;
        return sec;
    }

    //Converts seconds to miliseconds
    static double secToMiliSec(double sec){
        double miliSec = sec * 1000;
        return miliSec;
    }
     
    //Converts watts to decible
    static double wattsTodB(double watts){
        double dB = 10 * log10(watts);
        return dB;
    }

    //Converts decible to watts
    static double dBtoWatts(double dB){
        double watts = pow(10, dB/10);
        return watts;
    }

    //Converts gigahertz to hertz
    static double GhzToHz(double Ghz){
        double hz = Ghz * pow(10,9);
        return hz;
    }

    //Converts hertz to gigahertz
    static double HztoGhz(double hz){
        double Ghz = hz / pow(10,9);
    return Ghz;
    }
   
    //Converts kilo values to base units
    static double kiloToBase(double kilo){
        double base = kilo*1000;
        return base;
    }

    //Converts base units to kilo units
    static double baseToKilo(double base){
        double kilo = base/1000;
        return kilo;
    }

    //Converts degrees^2 to steradians 
    static double degSqToSteraian(double degSq){
        double steradian = degSq/3283;
        return steradian;
    }

   private:

};

#endif