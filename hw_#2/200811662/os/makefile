IN = ../input/input.txt
OUT = ../input/output.txt

all: os

os: os.cpp
	g++ -o os os.cpp

run: os
	./os ${IN} ${OUT}

clean:
	rm ${OUT}
	rm os

