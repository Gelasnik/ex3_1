#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
using namespace MtmParkingLot;
using namespace std;
typedef unsigned int Cash;
typedef VehicleType Parking;
typedef unsigned int ID;

inline void printEntryFail(Parking parking, ID index);
inline void printEntrySuccess(VehicleType vehicleType, ID index, LicensePlate& licensePlate, Time entranceTime, Parking parking);

class ParkingLot::Vehicle{

    const LicensePlate licensePlate;
    const Time enterTime;
    const VehicleType type;
    Parking parking;
    bool gotFine;
    //Vehicle& operator=(const Vehicle&)= default;
public:


        //virtual Cash parkingPrice(Time exitTime) const =0;
        Vehicle(LicensePlate &vehicleLicensePlate,  Time vehicleEnterTime,  Parking vehicleParking, VehicleType vehicleType):
        licensePlate(vehicleLicensePlate), enterTime(vehicleEnterTime), type(vehicleType), parking(vehicleParking),  gotFine(false){}
        Vehicle(Vehicle& vehicle) = default;
};

class ParkingLot::Motorbike: public Vehicle{

public:
    Motorbike(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime):
            Vehicle(vehicleLicensePlate, vehicleEnterTime,  MOTORBIKE, MOTORBIKE){}

};
class ParkingLot::Car: public Vehicle{

public:
    Car(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime, Parking parking = CAR,  VehicleType type = CAR):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, parking, type){
        if((type == CAR && parking == HANDICAPPED) || parking == MOTORBIKE) throw;
    }

};
class ParkingLot::Handicapped: public Car{

public:
    Handicapped(LicensePlate &vehicleLicensePlate, Time vehicleEnterTime, Parking parking):
            Car(vehicleLicensePlate, vehicleEnterTime, parking, HANDICAPPED){
        if(parking==MOTORBIKE) throw;
    }

};

ParkingLot::ParkingLot(unsigned int *parkingBlockSizes):
motorbikeParking(parkingBlockSizes[0]), handicappedParking(parkingBlockSizes[1]), carParking(parkingBlockSizes[2]){}

ParkingResult ParkingLot::enterParking(VehicleType vehicleType,
                                       LicensePlate licensePlate,
                                       Time entranceTime) {
    unsigned int index;
    
    try{
    switch (vehicleType) {
        case MOTORBIKE:{
                Motorbike newMotorbike(licensePlate,entranceTime);

                if (motorbikeParking.getIndex(newMotorbike,index)){
                    printEntryFail(MOTORBIKE, index);
                    return VEHICLE_ALREADY_PARKED;
                }
                index = motorbikeParking.insert(newMotorbike);
                printEntrySuccess(MOTORBIKE, index, licensePlate, entranceTime, MOTORBIKE);
                return SUCCESS;
        }

        case CAR:{
                Car newCar(licensePlate,entranceTime);
    
                if (carParking.getIndex(newCar,index)){
                    printEntryFail(CAR, index);
                    return VEHICLE_ALREADY_PARKED;
                }
                index = carParking.insert(newCar);
                printEntrySuccess(CAR, index, licensePlate, entranceTime, CAR);
                return SUCCESS;
        }

        case HANDICAPPED:{
            Handicapped newHandicapped(licensePlate, entranceTime,HANDICAPPED);

            try{

                if (handicappedParking.getIndex(newHandicapped,index)){
                    printEntryFail(HANDICAPPED, index);
                    return VEHICLE_ALREADY_PARKED;
                } else if (carParking.getIndex(newHandicapped,index)){
                    printEntryFail(CAR, index);
                    return VEHICLE_ALREADY_PARKED;
                }

                index = handicappedParking.insert(newHandicapped);
                printEntrySuccess(HANDICAPPED, index, licensePlate, entranceTime, HANDICAPPED);
                handicappedParking.insert(newHandicapped);
                return SUCCESS;

            }catch (const UniqueArray<int>::UniqueArrayIsFullException& e)
                {
                    index = handicappedParking.insert(newHandicapped);
                    printEntrySuccess(HANDICAPPED, index, licensePlate, entranceTime, CAR);
                    handicappedParking.insert(newHandicapped);
                    return SUCCESS;
                }
        }
    }
    }catch (const UniqueArray<int>::UniqueArrayIsFullException& e){
        ParkingLotPrinter::printEntryFailureNoSpot(cout);
        return NO_EMPTY_SPOT;
    }
}

inline void printEntryFail(Parking parking, ID index){
    ParkingSpot parkingSpot(parking, index);
    ParkingLotPrinter::printEntryFailureAlreadyParked(cout, parkingSpot);
}
inline void printEntrySuccess(VehicleType vehicleType, ID index, LicensePlate& licensePlate, Time entranceTime, Parking parking) {
    ParkingSpot parkingSpot(parking, index);
    ParkingLotPrinter::printVehicle(cout, vehicleType, licensePlate, entranceTime);
    ParkingLotPrinter::printEntrySuccess(cout, parkingSpot);
}


//SUCCESS,
//NO_EMPTY_SPOT,
//VEHICLE_NOT_FOUND,
//VEHICLE_ALREADY_PARKED