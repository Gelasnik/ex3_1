#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"

const cash FINE_PRICE 250
const unsigned int MOTORBIKE_MAX_HOURS_TO_PAY 6
const cash MOTORBIKE_FIRST_HOUR_PRICE 10
const cash MOTORBIKE_EXTRA_HOUR_PRICE 5
const unsigned int CAR_MAX_HOURS_TO_PAY 6
const cash CAR_FIRST_HOUR_PRICE 20
const cash CAR_EXTRA_HOUR_PRICE 10
const cash HANDICAPPED_CONSTANT_PRICE 15

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;

    class ParkingLot {
    public:

        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot();
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

    };

}

#endif //MTMPARKINGLOT_PARKINGLOT_H
