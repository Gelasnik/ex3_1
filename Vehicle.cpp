



namespace MtmParkingLot {
	
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

 
 