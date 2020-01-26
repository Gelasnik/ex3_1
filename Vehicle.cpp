
#include "Vehicle.h"

namespace MtmParkingLot {




    ostream &operator<<(ostream &os, const Vehicle &vehicle) {
        return ParkingLotPrinter::printVehicle(os, vehicle.type, vehicle.licensePlate,
                                              vehicle.enterTime1);
    }

    cash Motorbike::parkingPrice(Time1 exitTime1) const{
        Hour parkingTime1 = (exitTime1 - (*this).enterTime1).toHours();
        if (parkingTime1 == 0) {
            return 0;
        }
        cash price = MOTORBIKE_FIRST_HOUR_PRICE;
        if (parkingTime1 <= MOTORBIKE_MAX_HOURS_TO_PAY) { /* Without fine.*/
            return price += MOTORBIKE_EXTRA_HOUR_PRICE * (parkingTime1 - 1);
        }
        price += MOTORBIKE_EXTRA_HOUR_PRICE * (MOTORBIKE_MAX_HOURS_TO_PAY - 1);
        if ((*this).gotFine) {
            price += FINE_PRICE;
        }
        return price;
    }

    cash Car::parkingPrice(Time1 exitTime1) const{
        Hour parkingTime1 = (exitTime1 - (*this).enterTime1).toHours();
        if (parkingTime1 == 0) {
            return 0;
        }
        cash price = CAR_FIRST_HOUR_PRICE;
        if (parkingTime1 <= CAR_MAX_HOURS_TO_PAY) {
            return price += CAR_EXTRA_HOUR_PRICE * (parkingTime1 - 1);
        }
        price += CAR_EXTRA_HOUR_PRICE * (CAR_MAX_HOURS_TO_PAY - 1);
        if ((*this).gotFine) {
            price += FINE_PRICE;
        }
        return price;
    }

    cash Handicapped::parkingPrice(Time1 exitTime1) const {
        cash price = HANDICAPPED_CONSTANT_PRICE;
        if ((*this).gotFine) {
            price += FINE_PRICE;
        }
        return price;
    }





}