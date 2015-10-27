rm -rf pgw.o pgw peer.o sub.o
g++ -std=gnu++11        -O2 -g -Wall -fmessage-length=0   -c -o pgw.o pgw.cpp
g++ -std=gnu++11        -O2 -g -Wall -fmessage-length=0   -c -o sub.o sub.cpp
g++ -std=gnu++11        -O2 -g -Wall -fmessage-length=0   -c -o peer.o peer.cpp
g++ -o pgw  pgw.o avp.o diameter.o avputil.o sub.o peer.o -lpthread
./pgw
