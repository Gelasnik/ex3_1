#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
using namespace MtmParkingLot;
using namespace std;


typedef VehicleType Parking;
typedef unsigned int ID;
typedef unsigned int Hour;
inline void printEntryFail(Parking parking, ID index);
inline void printEntrySuccess(VehicleType vehicleType, ID index, LicensePlate& licensePlate, Time1 entranceTime, Parking parking);



class ParkingLot::Vehicle{


    //Vehicle& operator=(const Vehicle&)= default;

public:
    const LicensePlate licensePlate;
    const Time1 enterTime;
    const VehicleType type;

    bool gotFine;
        Parking parking;
        virtual cash parkingPrice(Time1 exitTime)  const = 0;
        Vehicle(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking vehicleParking, VehicleType vehicleType):
        licensePlate(vehicleLicensePlate), enterTime(vehicleEnterTime), type(vehicleType), parking(vehicleParking),  gotFine(false){}
        Vehicle(const Vehicle& vehicle) = default;
        friend ParkingLot::Compare;
};
class ParkingLot::Compare{
public:
    Compare()= default;
    bool operator()(const ParkingLot::Vehicle& vehicle1, const Vehicle& vehicle2){
        return vehicle1.licensePlate == vehicle2.licensePlate;
    }
};

class ParkingLot::Motorbike: public Vehicle{
    cash parkingPrice(Time1 exitTime) const override;
public:
    Motorbike(const Motorbike& motorbike)= default;
    explicit Motorbike(const Vehicle &vehicle) : Vehicle(vehicle){} ;

    Motorbike(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime):
            Vehicle(vehicleLicensePlate, vehicleEnterTime,  MOTORBIKE, MOTORBIKE){
    }

};



class ParkingLot::Car: public Vehicle{
    cash parkingPrice(Time1 exitTime) const  override ;
public:
    explicit Car(const Vehicle &vehicle) : Vehicle(vehicle){} ;
    Car(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking parking = CAR, VehicleType type = CAR):
            Vehicle(vehicleLicensePlate, vehicleEnterTime, parking, type){
        //if((type == CAR && parking == HANDICAPPED) || parking == MOTORBIKE) throw;
    }

};
class ParkingLot::Handicapped: public Car{
    cash parkingPrice(Time1 exitTime) const override ;
public:
    explicit Handicapped(const Vehicle &vehicle) : Car(vehicle){} ;
    Handicapped(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking parking):
            Car(vehicleLicensePlate, vehicleEnterTime, parking, HANDICAPPED){
        //if(parking==MOTORBIKE) throw;
    }

};

ParkingLot::ParkingLot(unsigned int *parkingBlockSizes):
motorbikeParking(parkingBlockSizes[0]), handicappedParking(parkingBlockSizes[1]), carParking(parkingBlockSizes[2]){}

ParkingResult ParkingLot::enterParking(VehicleType vehicleType,
                                       LicensePlate licensePlate,
                                       Time1 entranceTime) {
    unsigned int index;


    switch (vehicleType) {

        case MOTORBIKE:{
            try{
                Motorbike const newMotorbike(licensePlate,entranceTime);
                if (vehicleIsInParking(newMotorbike)){
                    return VEHICLE_ALREADY_PARKED;
                }
                index = motorbikeParking.insert(newMotorbike);
                printEntrySuccess(MOTORBIKE, index, licensePlate, entranceTime, MOTORBIKE);
                return SUCCESS;
            }catch (const UniqueArray<Motorbike, Compare>::UniqueArrayIsFullException& e){
                    ParkingLotPrinter::printEntryFailureNoSpot(cout);
                    return NO_EMPTY_SPOT;
            }
        }

        case CAR:{
            try{
                Car newCar(licensePlate,entranceTime);
                if (vehicleIsInParking(newCar)){
                    return VEHICLE_ALREADY_PARKED;
                }
                index = carParking.insert(newCar);
                printEntrySuccess(CAR, index, licensePlate, entranceTime, CAR);
                return SUCCESS;
               }catch (const UniqueArray<Car, Compare>::UniqueArrayIsFullException& e){
                ParkingLotPrinter::printEntryFailureNoSpot(cout);
                return NO_EMPTY_SPOT;
            }
        }

        case HANDICAPPED:{
            Handicapped newHandicapped(licensePlate, entranceTime,HANDICAPPED);
            try{
                if (vehicleIsInParking(newHandicapped)){
                    return VEHICLE_ALREADY_PARKED;
                }
                index = handicappedParking.insert(newHandicapped);
                printEntrySuccess(HANDICAPPED, index, licensePlate, entranceTime, HANDICAPPED);
                return SUCCESS;

            }catch (const UniqueArray<Handicapped, Compare>::UniqueArrayIsFullException& e){}
            try{
                index = carParking.insert(newHandicapped);
                printEntrySuccess(HANDICAPPED, index, licensePlate, entranceTime, CAR);
                return SUCCESS;
            }catch (const UniqueArray<Car, Compare>::UniqueArrayIsFullException& e){
                ParkingLotPrinter::printEntryFailureNoSpot(cout);
                return NO_EMPTY_SPOT;
            }
        }
    }


    return SUCCESS;//don't suppose to achieve this line
}

bool ParkingLot::vehicleIsInParking(const Vehicle &vehicle) const {
    unsigned int index;
    Motorbike m(vehicle);
    Handicapped h(vehicle);

    if (motorbikeParking.getIndex(m, index)) {
        printEntryFail(MOTORBIKE, index);
    } else if (carParking.getIndex(h, index)) {
        printEntryFail(CAR, index);
    } else if (handicappedParking.getIndex(h, index)) {
        printEntryFail(HANDICAPPED, index);
    } else return false;
    return true;
}

inline void printEntryFail(Parking parking, ID index){
    ParkingSpot parkingSpot(parking, index);
    ParkingLotPrinter::printEntryFailureAlreadyParked(cout, parkingSpot);
}

inline void printEntrySuccess(VehicleType vehicleType, ID index, LicensePlate& licensePlate, Time1 entranceTime, Parking parking) {
    ParkingSpot parkingSpot(parking, index);
    ParkingLotPrinter::printVehicle(cout, vehicleType, licensePlate, entranceTime);
    ParkingLotPrinter::printEntrySuccess(cout, parkingSpot);
}




ostream& MtmParkingLot::operator<<(ostream& os, const ParkingLot::Vehicle& vehicle) {
    return ParkingLotPrinter::printVehicle(os,vehicle.type,vehicle.licensePlate,
                                          vehicle.enterTime);
}

cash ParkingLot::Motorbike::parkingPrice(Time1 exitTime) const {
    Hour parkingTime = (exitTime - (*this).enterTime).toHours();
    if (parkingTime == 0) {
        return 0;
    }
    cash price = MOTORBIKE_FIRST_HOUR_PRICE;
    if (parkingTime <= MOTORBIKE_MAX_HOURS_TO_PAY) { /* Without fine.*/
        return price += MOTORBIKE_EXTRA_HOUR_PRICE*( parkingTime - 1);
    }
    price += MOTORBIKE_EXTRA_HOUR_PRICE * (MOTORBIKE_MAX_HOURS_TO_PAY - 1);
    if ((*this).gotFine) {
        price += FINE_PRICE;
    }
    return price;
}

cash ParkingLot::Car::parkingPrice(Time1 exitTime) const{
    Hour parkingTime = (exitTime - (*this).enterTime).toHours();
    if (parkingTime == 0) {
        return 0;
    }
    cash price = CAR_FIRST_HOUR_PRICE;
    if (parkingTime <= CAR_MAX_HOURS_TO_PAY) {
        return price += CAR_EXTRA_HOUR_PRICE * (parkingTime - 1);
    }
    price += CAR_EXTRA_HOUR_PRICE * (CAR_MAX_HOURS_TO_PAY - 1);
    if ((*this).gotFine) {
        price += FINE_PRICE;
    }
    return price;
}

cash ParkingLot::Handicapped::parkingPrice(Time1 exitTime) const{
    cash price = HANDICAPPED_CONSTANT_PRICE;
    if ((*this).gotFine) {
        price += FINE_PRICE;
    }
    return price;
}




ParkingResult ParkingLot::exitParking(LicensePlate licensePlate,
                                      Time1 exitTime)
{
    const Vehicle* vehicle = (*this).findVehicle(licensePlate);

    if (vehicle == nullptr) {
        ParkingLotPrinter::printExitFailure(cout, licensePlate);
        return VEHICLE_NOT_FOUND;
    }

    cout << *vehicle; /* Using printVehicle from ParkingLotPrinter.h */

    ParkingSpot parkingSpot;
    getParkingSpot(licensePlate, parkingSpot);
    cash price = (*vehicle).parkingPrice(exitTime);
    ParkingLotPrinter::printExitSuccess(cout,parkingSpot,exitTime,price);

    VehicleType parkingBlock = (*vehicle).parking;
    switch (parkingBlock) {
        case MOTORBIKE: {
            motorbikeParking.remove(Motorbike(*vehicle));
        }
        case CAR: {
            carParking.remove(Car(*vehicle));
        }
        case HANDICAPPED: {
            handicappedParking.remove(Handicapped(*vehicle));
        }
    }

    return SUCCESS;
}

ParkingResult ParkingLot::getParkingSpot(LicensePlate licensePlate,
                                         ParkingSpot& parkingSpot) const
{
    unsigned int index;
    VehicleType parkingBlock;
    Time1 time;
    Motorbike comparingMotorbike(licensePlate, time);
    Handicapped comparingHandicapped(licensePlate, time, HANDICAPPED);
    if (motorbikeParking.getIndex(comparingMotorbike,index) ){
        parkingBlock = MOTORBIKE;
    } else if (ParkingLot::handicappedParking.getIndex(comparingHandicapped,index)) {
        parkingBlock = HANDICAPPED;
    } else if (carParking.getIndex(comparingHandicapped,index) ){
        parkingBlock = CAR;
    } else {
        return VEHICLE_NOT_FOUND;
    }

    parkingSpot = ParkingSpot(parkingBlock, index);
    return SUCCESS;
}

const ParkingLot::Vehicle *MtmParkingLot::ParkingLot::findVehicle(LicensePlate& licensePlate){
Time1 time=Time1();
    Motorbike m(licensePlate,time );
    Handicapped h(licensePlate, time, HANDICAPPED);
    const Vehicle *vehicle = motorbikeParking[m];
    if (vehicle == NULL){
        vehicle = carParking[h];
    } else return vehicle;
    if (vehicle == NULL){
        vehicle = handicappedParking[h];
    } else return vehicle;

    return vehicle;
}



//SUCCESS,
//NO_EMPTY_SPOT,
//VEHICLE_NOT_FOUND,
//ENTER Handicapped