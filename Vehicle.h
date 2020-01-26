
#ifndef EX3_VEHICLE_H
#define EX3_VEHICLE_H

#include "ParkingLotPrinter.h"
#include "ParkingLotTypes.h"
#include "Time1.h"

namespace MtmParkingLot {



using namespace ParkingLotUtils;
    typedef VehicleType Parking;
    typedef unsigned int cash;
    typedef unsigned int Hour;
    const cash FINE_PRICE = 250;
    const unsigned int MOTORBIKE_MAX_HOURS_TO_PAY = 6;
    const cash MOTORBIKE_FIRST_HOUR_PRICE = 10;
    const cash MOTORBIKE_EXTRA_HOUR_PRICE = 5;
    const unsigned int CAR_MAX_HOURS_TO_PAY = 6;
    const cash CAR_FIRST_HOUR_PRICE = 20;
    const cash CAR_EXTRA_HOUR_PRICE = 10;
    const cash HANDICAPPED_CONSTANT_PRICE = 15;

    class Vehicle{





        //Vehicle& operator=(const Vehicle&)= default;

    public:

        const LicensePlate licensePlate;
        const Time1 enterTime1;
        const VehicleType type;
        Parking parking;
        bool gotFine;
        virtual cash parkingPrice(Time1 exitTime1) const =0;
        Vehicle(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime1, Parking vehicleParking, VehicleType vehicleType):
                licensePlate(vehicleLicensePlate), enterTime1(vehicleEnterTime1), type(vehicleType), parking(vehicleParking),  gotFine(false){}
        Vehicle(const Vehicle& vehicle) = default;
        friend ostream& operator<<(ostream& os, const Vehicle& vehicle);


        class Compare;

    };


    class Vehicle::Compare{
    public:
        Compare()= default;
        bool operator()(const Vehicle& vehicle1, const Vehicle& vehicle2){
            return vehicle1.licensePlate == vehicle2.licensePlate;
        }
    };


    class Motorbike: public Vehicle{

    public:
        Motorbike(const Motorbike& motorbike)= default;

        explicit Motorbike(const Vehicle &vehicle) : Vehicle(vehicle){} ;
        cash parkingPrice(Time1 exitTime) const override;
        Motorbike(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime1):
                Vehicle(vehicleLicensePlate, vehicleEnterTime1,  MOTORBIKE, MOTORBIKE){}

    };

    class Car: public Vehicle{

    public:
        explicit Car(const Vehicle &vehicle) : Vehicle(vehicle){} ;
        cash parkingPrice(Time1 exitTime) const override;
        Car(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime1, Parking parking = CAR, VehicleType type = CAR):
                Vehicle(vehicleLicensePlate, vehicleEnterTime1, parking, type){
            //if((type == CAR && parking == HANDICAPPED) || parking == MOTORBIKE) throw;
        }

    };
    class Handicapped: public Car{

    public:
        explicit Handicapped(const Vehicle &vehicle) : Car(vehicle){} ;
        cash parkingPrice(Time1 exitTime) const override;
        Handicapped(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime1, Parking parking):
                Car(vehicleLicensePlate, vehicleEnterTime1, parking, HANDICAPPED){
            //if(parking==MOTORBIKE) throw;
        }

    };

}





#endif //EX3_VEHICLE_H
