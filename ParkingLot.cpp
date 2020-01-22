#include "ParkingLot.h"
#include "UniqueArray.h"
using namespace MtmParkingLot;

class Motorbike;
class Car;
class Handicapped;

typedef UniqueArray<Motorbike> MotorbikeParking ;
typedef UniqueArray<Car> CarParking ;
typedef UniqueArray<Handicapped> HandicappedParking ;

class Vehicle{
    typedef unsigned int Cash;
    const LicensePlate licensePlate;
    const Time enterTime;
    bool gotFine;
    const VehicleType type;
    //Vehicle& operator=(const Vehicle&)= default;
public:
        virtual Cash parkingPrice(Time exitTime) const =0;
        Vehicle(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime, VehicleType vehicleType):
        licensePlate(vehicleLicensePlate), enterTime(vehicleEnterTime), type(vehicleType), gotFine(false){}

};

class Motorbike: public Vehicle{

public:
    Motorbike(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, MOTORBIKE){}

};

class Car: public Vehicle{

public:
    Car(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, CAR){}

};
class Handicapped: public Vehicle{

public:
    Handicapped(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, HANDICAPPED){}

};

//1
//2
//3