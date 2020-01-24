#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time1.h"
#include "ParkingSpot.h"
#include "UniqueArray.h"
namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;

    class ParkingLot {

        class Vehicle;
        class Motorbike;
        class Car;
        class Handicapped;
        class Compare;


        UniqueArray<Motorbike, Compare> motorbikeParking;
        UniqueArray<Car, Compare> carParking;
        UniqueArray<Handicapped, Compare> handicappedParking;
        bool vehicleIsInParking(const ParkingLot::Vehicle& vehicle) const ;
    public:

        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot()= default;
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time1 entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time1 exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time1 inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

    };


}

#endif //MTMPARKINGLOT_PARKINGLOT_H
