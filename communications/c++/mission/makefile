#define the suffixes
.SUFFIXES:    .c++ 

#macro definitions
CC=g++
MEX=mex


CFLAGS= -O3 `xml2-config --cflags` -fPIC
MEXFLAGS=-cxx 
# CFLAGS=  -g
LIB=  -lm  -lpthread `xml2-config --libs`
LDLIB = 
INCLUDE = -I/usr/local/MATLAB/R2010b/extern/include -I/usr/include/octave-3.2.4/octave/
OFLAGS = 


LDEP = PollingServer.c++ XMLParser.c++ XMLMessageNetwork.c++ XMLIncomingDIF.c++
HDEP = PollingServer.h   XMLParser.h   XMLMessageNetwork.h   XMLIncomingDIF.h
LOCAL = PollingServer.o  XMLParser.o   XMLMessageNetwork.o   XMLIncomingDIF.o
DEP = 


#.o% : .c++
.c++.o:	
	$(CC) -c $< $(CFLAGS) $(INCLUDE) 


DataPolling.mexa64:    $(LOCAL) $(HDEP) DataPolling.c++ DataPolling.o
	$(MEX) $(MEXFLAGS) -o DataPolling  DataPolling.o $(LOCAL) $(LIB) $(LDLIB) $(OFLAGS) $(INCLUDE) 

trial:	trial.c++ $(LOCAL)
	$(CC) -o trial trial.c++ $(OFLAGS) $(CFLAGS) $(INCLUDE) XMLMessageNetwork.o XMLParser.o XMLIncomingDIF.o $(LIB) 
	
sender:	sender.o
	$(CC) $(OFLAGS) $(CFLAGS) -o $@ $< $(INCLUDE)

clean:
	rm -f DataPolling $(LOCAL) *.mexa64 *.o sender *~ trial
