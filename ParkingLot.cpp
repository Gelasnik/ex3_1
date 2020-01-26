#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
using namespace MtmParkingLot;
using namespace std;


typedef unsigned int ID;

inline void printEntryFail(Parking parking, ID index);
inline void printEntrySuccess(VehicleType vehicleType, ID index, LicensePlate& licensePlate, Time1 entranceTime, Parking parking);

void addToSpotsArray (ParkingSpots** arrayOfSpots, parkingSpot* newSpot, unsigned long spotsCounter);

/*
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
        ~Vehicle()= default;
        friend ParkingLot::Compare;
};


class ParkingLot::Compare{
public:
    Compare()= default;
    bool operator()(const ParkingLot::Vehicle& vehicle1, const Vehicle& vehicle2){
        return vehicle1.licensePlate == vehicle2.licensePlate;
    }
    ~Compare()= default;
};

class ParkingLot::Motorbike: public Vehicle{
    cash parkingPrice(Time1 exitTime) const override;
public:
    Motorbike(const Motorbike& motorbike)= default;
    explicit Motorbike(const Vehicle &vehicle) : Vehicle(vehicle){} ;
    ~Motorbike()= default;
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
    ~Car()= default;

};
class ParkingLot::Handicapped: public Car{
    cash parkingPrice(Time1 exitTime) const override ;
public:
    explicit Handicapped(const Vehicle &vehicle) : Car(vehicle){} ;
    Handicapped(LicensePlate &vehicleLicensePlate, Time1 vehicleEnterTime, Parking parking):
            Car(vehicleLicensePlate, vehicleEnterTime, parking, HANDICAPPED){
        //if(parking==MOTORBIKE) throw;
    }
    ~Handicapped()= default;
};
*/

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
            }catch (const UniqueArray<Motorbike, Vehicle::Compare>::UniqueArrayIsFullException& e){
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
               }catch (const UniqueArray<Car, Vehicle::Compare>::UniqueArrayIsFullException& e){
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

            }catch (const UniqueArray<Handicapped, Vehicle::Compare>::UniqueArrayIsFullException& e){}
            try{
                index = carParking.insert(newHandicapped);
                printEntrySuccess(HANDICAPPED, index, licensePlate, entranceTime, CAR);
                return SUCCESS;
            }catch (const UniqueArray<Car, Vehicle::Compare>::UniqueArrayIsFullException& e){
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





/*

cash ParkingLot::Motorbike::parkingPrice(Time1 exitTime) const {
    Hour parkingTime = (exitTime - (*this).enterTime).toHours();
    if (parkingTime == 0) {
        return 0;
    }
    cash price = MOTORBIKE_FIRST_HOUR_PRICE;
    if (parkingTime <= MOTORBIKE_MAX_HOURS_TO_PAY) {
 Without fine.

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


*/


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



void MtmParkingLot::ParkingLot::inspectParkingLot(Time1 inspectionTime) {
    UniqueArray<Motorbike, Vehicle::Compare>::MyIterator mottoIter(motorbikeParking);
    unsigned int i=0;
    unsigned int numFined =0;
    while(i<motorbikeParking.getSize()){
        if ((**mottoIter)==NULL) {
            mottoIter++;
            i++;
            continue;
        }
        Motorbike* motto = **mottoIter;
        if(inspectionTime.toHours()-motto->enterTime1.toHours()>24) {
            motto->gotFine=true;
            numFined++;
        }
        mottoIter++;
        i++;
    }
    i=0;
    UniqueArray<Car,Vehicle::Compare>::MyIterator carIter(carParking);
    while(i<carParking.getSize()){
        if (**carIter==NULL) {
            carIter++;
            i++;
            continue;
        }
        Car* car = **carIter;
        if(inspectionTime.toHours()-car->enterTime1.toHours()>24) {
            car->gotFine=true;
            numFined++;
        }
        carIter++;
        i++;
    }
    i=0;
    UniqueArray<Handicapped,Vehicle::Compare>::MyIterator handicapIter(handicappedParking);
    while(i<handicappedParking.getSize()){
        if (**handicapIter==NULL) {
            handicapIter++;
            i++;
            continue;
        }
        Handicapped* handicapped = **handicapIter;
        if(inspectionTime.toHours()-handicapped->enterTime1.toHours()>24) {
            handicapped->gotFine=true;
            numFined++;
        }
        handicapIter++;
        i++;
    }

    ParkingLotPrinter::printInspectionResult(cout,inspectionTime,numFined);
}





ostream& operator<<(ostream& os, const Vehicle& vehicle) {
    return ParkingLotPrinter::printVehicle(os,vehicle.type,vehicle.licensePlate,
                                           vehicle.enterTime1);
}
const Vehicle* ParkingLot::findVehicle(LicensePlate& licensePlate)  {
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

ostream& ParkingLot::operator<<(ostream& os, const ParkingLot& parkingLot) {
	ParkingLotPrinter.printParkingLotTitle(os);
	unsigned long parkingLotSize = parkingLot.motorbikeParking.getSize();
	parkingLotSize += parkingLot.carParking.getSize();
	parkingLotSize += parkingLot.handicappedParking.getSize();
	ParkingSpot** arrayOfSpots = new ParkingSpot*[ParkingLotSize];
	unsigned long spotsCounter = 0;
	ParkingSpot* currSpot;
	
	UniqueArray<Motorbike, Vehicle::Compare>::MyIterator mottoIter(motorbikeParking);
	
	for (unsigned int i=0; i<motorbikeParking.getSize() ; ++i , ++mottoIter) {
		if ((**mottoIter) != NULL) {
			currSpot = new parkingSpot(MOTORBIKE,mottoIter.getIndex());
			addToSpotsArray(arrayOfSpots,currSpot,spotsCounter);
			spotsCounter++;
		}
	}
	
	UniqueArray<Car,Vehicle::Compare>::MyIterator carIter(carParking);
	for (unsigned int i=0; i<carParking.getSize(); ++i , ++carIter) {
		if  ((**carIter) != NULL) {
			currSpot = new parkingSpot(CAR,carIter.getIndex());
			addToSpotsArray(arrayOfSpots,currSpot,spotsCounter);
			spotsCounter++;
		}
	}
	
    UniqueArray<Handicapped,Vehicle::Compare>::MyIterator handicapIter(handicappedParking);	
	for (unsigned int i=0; i<handicappedParking.getSize(); ++i , ++handicapIter) {
		if ( (**handicapIter) != NULL) {
			currSpot = new parkingSpot(HANDICAPPED,handicapIter.getIndex());
			addToSpotsArray(arrayOfSpots,currSpot,spotsCounter);
			spotsCounter++;
		}
	}
	
	/* Prints the vehicles according to the sorted array.*/
	const Vehicle* vehicle;
	for (unsigned long i=0; i<spotsCounter; ++i) {
		Vehicle = getVehicleBySpot(arrayOfSpots[i]);
		os << (*vehicle); /* printVehicle.*/
		ParkingLotPrinter.printParkingSpot(os,*arrayOfSpots[i]);
	}
	
	/* Delete memory.*/
	for (unsigned long i=0; i<spotsCounter ; ++i) {
		delete arrayOfSpots[i];
	}
	
	delete[] arrayOfSpots;
}

const Vehicle* ParkingLot::getVehicleBySpot(const ParkingSpot* parkingSpot) const {
	unsigned int index = (*parkingSpot).getParkingNumber();
	switch ( (*parkingSpot).getParkingBlock() ) {
		case MOTORBIKE: {
			return motorbikeParking.getElement(index);
		}
		case CAR: {
			return carParking.getElement(index);
		}
		case HANDICAPPED: {
			return handicappedParking.getElement(index);
		}
	}
}


/* Add new ParkingSpot to a sorted array.*/
void addToSpotsArray (ParkingSpots** arrayOfSpots, parkingSpot* newSpot,
					  unsigned long spotsCounter) 
{
	unsigned long i=0;
	while ( (i<spotsCounter) && ( (*arrayOfSpots[i]) < (*newSpot) ) ) {
		i++;
	}
	for (unsigned long j = spotsCounter; j > i ; --j ) {
		arrayOfSpots[j] = arrayOfSpots[j-1];
	}
	arrayOfSpots[i] = newSpot;
}

	
	

/*void MtmParkingLot::ParkingLot::printParkingLot(Time1 inspectionTime) {

    UniqueArray<Motorbike,Compare>::MyIterator mottoIter(motorbikeParking);
    unsigned int i=0;

    while(i<motorbikeParking.getSize()){
        if ((**mottoIter)==NULL) {
            mottoIter++;
            i++;
            continue;
        }
        Motorbike* motto = **mottoIter;
        cout<<motto->gotFine<<endl;
        mottoIter++;
        i++;
    }
    i=0;
    UniqueArray<Car,Compare>::MyIterator carIter(carParking);
    while(i<carParking.getSize()){
        if (**carIter==NULL) {
            carIter++;
            i++;
            continue;
        }
        Car* car = **carIter;
        cout<<car->gotFine<<endl;

        carIter++;
        i++;
    }
    i=0;
    UniqueArray<Handicapped,Compare>::MyIterator handicapIter(handicappedParking);
    while(i<handicappedParking.getSize()){
        if (**handicapIter==NULL) {
            handicapIter++;
            i++;
            continue;
        }
        Handicapped* handicapped = **handicapIter;
        cout<<handicapped->gotFine<<endl;

        handicapIter++;
        i++;
    }


}*/





//SUCCESS,
//NO_EMPTY_SPOT,
//VEHICLE_NOT_FOUND,
//ENTER Handicapped