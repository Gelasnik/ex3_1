#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
using namespace MtmParkingLot;
using namespace std;
typedef unsigned int Cash;
typedef VehicleType Parking;
typedef unsigned int ID;

inline void printEntryFail(Parking parking, ID index);
inline void printEntrySuccess(VehicleType vehicleType, ID index, LicensePlate& licensePlate, Time1 entranceTime, Parking parking);


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



ParkingResult ParkingLot::exitParking(LicensePlate licensePlate,
									  Time exitTime) 
{
	const Vehicle* vehicle = (*this).findVehicle(licensePlate);
	
	if (vehicle == NULL) {
		ParkingLotPrinter.PrintExitFailure(cout, licensePlate);
		return VEHICLE_NOT_FOUND;
	}
	
	cout << *vehicle; /* Using printVehicle from ParkingLotPrinter.h */
	
	ParkingSpot parkingSpot;
	getParkingSpot(licensePlate, parkingSpot);
	cash price = (*vehicle).parkingPrice(exitTime);
	ParkingLotPrinter.printExitSuccess(cout,parkingSpot,exitTime,price);
	
	vehicleType parkingBlock = (*vehicle).getParkingBlock();
	switch (parkingBlock) {
		case MOTORBIKE: {
			motorbikeParking.remove(*vehicle);
		}
		case CAR: {
			carParking.remove(*vehicle);
		}
		case HANDICAPPED: {
			handicappedParking.remove(*vehicle);
		}
	}
	
	return SUCCESS;
}

ParkingResult parkingLot::getParkingSpot(LicensePlate licensePlate,
										 ParkingSpot& parkingSpot) const 
{
	unsigned int index;
	VehicleType parkingBlock;
	Time time();
	Motorbike comparingMotorbike(licensePlate, time);
	Handicapped comparingHandicapped(licensePlate, time, Handicapped);
	if (motorbikeParking.getIndex(comparingMotorbike,index) {
		parkingBlock = MOTORBIKE;
	}
	else if (hadicappedParking.getIndex(comparingHandicapped,index) {
		parkingBlock = HANDICAPPED;
	}
	else if (carParking.getIndex(comparingHandicapped,index) {
		parkingBlock = CAR;
	}
	else {
		return VEHICLE_NOT_FOUND;
	}
	
	parkingSpot = ParkingSpot(parkingBlock, index);
	return SUCCESS;
}



//SUCCESS,
//NO_EMPTY_SPOT,
//VEHICLE_NOT_FOUND,
//ENTER Handicapped