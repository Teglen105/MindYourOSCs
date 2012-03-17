CC 				= $(shell fltk-config --cxx) -arch i386
CCFLAGS		= $(shell fltk-config --cxxflags ) -I. 
LDFLAGS 	= $(shell fltk-config --ldflags )
LDSTATIC 	= $(shell fltk-config --ldstaticflags )
LINK			= $(CC)

TARGET 	= MindYourOSCs

OUTPUT 	= bin
SRC 		= src
OBJOUT 	= build

OBJECTS = main.o  Driver.o AppGUI.o Emotiv.o EmotivHandler.o 
OBJS 		:= $(foreach OBJ, $(OBJECTS), $(OBJOUT)/$(OBJ) ) 
SRC_DIR = src
SRC 		= main.cc AppGUI.cc Driver.cc Emotiv.cc EmotivHandler.cc

OSCPACK_DIR = ./oscpack
OSCPACK_OBJ_DIR = $(OSCPACK_DIR)/build
OSCPACK_OBJECTS = IpEndpointName.o NetworkingUtils.o UdpSocket.o \
									OscOutboundPacketStream.o OscTypes.o 
OSCPACK_OBJ := $(foreach OBJ, $(OSCPACK_OBJECTS), $(OSCPACK_OBJ_DIR)/$(OBJ)) -I$(OSCPACK_DIR)

EMOTIV_INCLUDE_DIR 	= -I/usr/local/include/Emotiv -I./include
EMOTIV_LIB_DIR 			= -L/usr/local/lib
EMOTIV_LIBS					= -ledk.1 -ledk_ultils_mac -liomp5
EMOTIV = $(EMOTIV_INCLUDE_DIR) $(EMOTIV_LIB_DIR) $(EMOTIV_LIBS)									

BOOST_DIR 		= -I/opt/local/include
BOOST_LIB_DIR = -L/opt/local/lib
BOOST_LIBS 		= -lboost_thread-mt -lboost_signals-mt
BOOST = $(BOOST_DIR) $(BOOST_LIB_DIR) $(BOOST_LIBS)


all: $(TARGET)
	$(LINK) -o $(OUTPUT)/$(TARGET) $(OBJS) $(EMOTIV) $(OSCPACK_OBJ) $(BOOST) $(LDSTATIC)

$(TARGET): $(OBJS)

.SUFFIXES: .o .cc
%.o: ../src/%.cc
	$(CC) $(CCFLAGS) $(DEBUG) -c -o $@ $< $(BOOST) $(EMOTIV) $(OSCPACK_OBJ)