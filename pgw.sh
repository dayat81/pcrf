g++ -std=gnu++11        -O2 -g -Wall -fmessage-length=0   -c -o pgw.o pgw.cpp
g++ -o pgw  pgw.o avp.o diameter.o avputil.o -lrocksdb
