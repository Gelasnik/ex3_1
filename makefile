
CC = g++
OBJS = MtmParkingLot.o ParkingLot.o Vehicle.o
EXEC = MtmParkingLot
DEBUG_FLAG = #-g
CFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG

$(EXEC)	: $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

ParkingLot.o : ParkingLot.h Vehicle.h UniqueArrayImp.h UniqueArray.h Time1.h ParkingSpot.h \
ParkingLotTypes.h ParkingLotPrinter.h

MtmParkingLot.o : ParkingLot.h Vehicle.h UniqueArrayImp.h UniqueArray.h Time1.h ParkingSpot.h \
ParkingLotTypes.h ParkingLotPrinter.h

Vehicle.o : Vehicle.h Time1.h ParkingSpot.h ParkingLotTypes.h ParkingLotPrinter.h

clean:
	rm -f $(OBJS) $(EXEC)