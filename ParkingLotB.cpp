

#include "ParkingLotPrinter.h"


ostream& operator<<(ostream& os, const Vehicle& vehicle) {
	return ParkingLotPrinter.printVehicle(os,vehicle.type,vehicle.licensePlate,
											vehicle.enterTime);
}
                                         
cash Motorbike::parkingPrice(Time exitTime) {
	Hour parkingTime = (exitTime - (*this).enterTime).toHours();
	if (parkingTime == 0) {
		return 0;
	}
	cash price = MOTORBIKE_FIRST_HOUR_PRICE
	if (parkingTime <= MOTORBIKE_MAX_HOURS_TO_PAY) { /* Without fine.*/
		return price += MOTORBIKE_EXTRA_HOUR_PRICE*( parkingTime - 1);
	}
	price += MOTORBIKE_EXTRA_HOUR_PRICE * (MOTORBIKE_MAX_HOURS_TO_PAY - 1);
	if ((*this).gotFine) {
		price += FINE_PRICE;
	}
	return price;
}

cash Car::parkingPrice(Time exitTime) {
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

cash Handicapped::parkingPrice(Time exitTime) {
	cash price = HANDICAPPED_CONSTANT_PRICE;
	if ((*this).gotFine) {
		price += FINE_PRICE;
	}
	return price;
}

	
vehicleType Vehicle::getParkingBlock() {
	return parking;
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


	