
# NOTE: This is just the simplest form of makefile.
# Project was developed using cmake. (the makefile generated from cmake used absolute paths in my system so I could not use that)
# There is no need for creating a huge makefile that will be used just once.
# (Especially since it has to handle installation of Boost library which does not use makefile on itself.)


all: lsh cube
	

lsh:
	g++ -std=c++11 -I./include -I./libs/boost_1_68_0/ ./src/* -D RUN_LSH=1 -L./libs/boost_1_68_0/stage/lib -lboost_program_options -o ./lsh

cube:
	g++ -std=c++11 -I./include -I./libs/boost_1_68_0/ ./src/* -D RUN_RPH=1 -L./libs/boost_1_68_0/stage/lib -lboost_program_options -o ./cube

.PHONY: clean
clean:
	rm lsh cube
