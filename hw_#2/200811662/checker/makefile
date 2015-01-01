IN = ../input/input.txt
OUT = ../input/output.txt

all: checker

checker: checker.cpp
	g++ -o checker checker.cpp

run: checker
	./checker ${IN} ${OUT}

clean:
	rm checker

