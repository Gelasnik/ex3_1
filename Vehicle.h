

namespace MtmParkingLot {




 class Vehicle{

	 static const cash FINE_PRICE 250
	 static const unsigned int MOTORBIKE_MAX_HOURS_TO_PAY 6
	 static const cash MOTORBIKE_FIRST_HOUR_PRICE 10
	 static const cash MOTORBIKE_EXTRA_HOUR_PRICE 5
	 static const unsigned int CAR_MAX_HOURS_TO_PAY 6
	 static const cash CAR_FIRST_HOUR_PRICE 20
	 static const cash CAR_EXTRA_HOUR_PRICE 10
	 static const cash HANDICAPPED_CONSTANT_PRICE 15

     const LicensePlate licensePlate;
     const Time1 enterTime;
     const VehicleType type;
     Parking parking;
     bool gotFine;
     //Vehicle& operator=(const Vehicle&)= default;

 public:


     virtual Cash parkingPrice(Time exitTime) const =0;
     Vehicle(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking vehicleParking, VehicleType vehicleType):
     licensePlate(vehicleLicensePlate), enterTime(vehicleEnterTime), type(vehicleType), parking(vehicleParking),  gotFine(false){}
     Vehicle(const Vehicle& vehicle) = default;
     friend ParkingLot::Compare;
	 friend ostream& operator<<(ostream& os, const Vehicle& vehicle);
	 vehicleType getParkingBlock();
	 
	 class Compare;

 };


 class Vehicle::Compare{
 public:
     Compare()= default;
     bool operator()(const ParkingLot::Vehicle& vehicle1, const Vehicle& vehicle2){
        return vehicle1.licensePlate == vehicle2.licensePlate;
     }
 };


 class Motorbike: public Vehicle{

 public:
     Motorbike(const Motorbike& motorbike)= default;

     explicit Motorbike(const Vehicle &vehicle) : Vehicle(vehicle){} ;
 
     Motorbike(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime):
             Vehicle(vehicleLicensePlate, vehicleEnterTime,  MOTORBIKE, MOTORBIKE){}

 };

 class Car: public Vehicle{

 public:
     explicit Car(const Vehicle &vehicle) : Vehicle(vehicle){} ;
     Car(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking parking = CAR, VehicleType type = CAR):
             Vehicle(vehicleLicensePlate, vehicleEnterTime, parking, type){
         //if((type == CAR && parking == HANDICAPPED) || parking == MOTORBIKE) throw;
     }

 };
 class Handicapped: public Car{

 public:
     explicit Handicapped(const Vehicle &vehicle) : Car(vehicle){} ;
     Handicapped(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking parking):
             Car(vehicleLicensePlate, vehicleEnterTime, parking, HANDICAPPED){
         //if(parking==MOTORBIKE) throw;
     }

 };
 
}