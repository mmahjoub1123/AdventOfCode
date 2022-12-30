
all: all day_01 day_02 day_03 day_04 day_05 day_06


day_01: bday_01
	echo "Day 01 :"
	./day_01 inputs/day_01.txt

day_02: bday_02
	./day_02 inputs/day_02.txt
	
day_03: bday_03
	./day_03 inputs/day_03.txt

day_04: bday_04
	./day_04 inputs/day_04.txt

day_05: bday_05
	./day_05 inputs/day_05.txt

day_06: bday_06
	./day_06 inputs/day_06.txt


bday_01: $(bday_01)
	g++ -o day_01 src/day_01.cpp

bday_02: $(bday_02)
	g++ -o day_02 src/day_02.cpp
	
bday_03: $(bday_03)
	g++ -o day_03 src/day_03.cpp

bday_04: $(bday_04)
	g++ -o day_04 src/day_04.cpp

bday_05: $(bday_05)
	g++ -o day_05 src/day_05.cpp

bday_06: $(bday_06)
	g++ -o day_06 src/day_06.cpp
	
clean:
	rm -f *.o

.PHONY: all clean
