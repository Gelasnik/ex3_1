#include "ParkingLot.h"

using namespace MtmParkingLot;




class Vehicle{
    typedef unsigned int Cash;
    const LicensePlate licensePlate;
    const Time enterTime;
    const VehicleType type;
    bool gotFine;

    //Vehicle& operator=(const Vehicle&)= default;
public:
        //virtual Cash parkingPrice(Time exitTime) const =0;
        Vehicle(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime, VehicleType vehicleType):
        licensePlate(vehicleLicensePlate), enterTime(vehicleEnterTime), type(vehicleType), gotFine(false){}

        Vehicle(Vehicle& vehicle) = default;


};

class ParkingLot::Motorbike: public Vehicle{

public:
    Motorbike(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, MOTORBIKE){}

};
class ParkingLot::Car: public Vehicle{

public:
    Car(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime, VehicleType type = CAR):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, type){}

};
class ParkingLot::Handicapped: public Car{

public:
    Handicapped(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime):
            Car(vehicleLicensePlate, vehicleEnterTime, HANDICAPPED){}

};

ParkingLot::ParkingLot(unsigned int *parkingBlockSizes):
motorbikeParking(parkingBlockSizes[0]), handicappedParking(parkingBlockSizes[1]), carParking(parkingBlockSizes[2]){}

ParkingResult ParkingLot::enterParking(VehicleType vehicleType,
                                       LicensePlate licensePlate,
                                       Time entranceTime) {
    try{
    switch (vehicleType) {
        case MOTORBIKE:{
                Motorbike newMotorbike(licensePlate,entranceTime);
                motorbikeParking.insert(newMotorbike);
                break;
        }

        case CAR:{
                Car newCar(licensePlate,entranceTime);
                carParking.insert(newCar);
                return SUCCESS;
        }

        case HANDICAPPED:{
            try{

            }catch (const UniqueArray<int>::UniqueArrayIsFullException& e)
                {
                    Handicapped newHandicapped(licensePlate,entranceTime);
                    carParking.insert(newHandicapped);
                    return SUCCESS;
                }
        }
    }
    }catch (const UniqueArray<int>::UniqueArrayIsFullException& e){
        return NO_EMPTY_SPOT;
    }//maybe add std?


}
//SUCCESS,
//NO_EMPTY_SPOT,
//VEHICLE_NOT_FOUND,
//VEHICLE_ALREADY_PARKED