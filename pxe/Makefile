VERSION = 1.1

TARGET_ARCH = x86
CROSS_COMPILE = #arm-
DEBUG = -g #-O2

TOP_DIR := $(shell pwd)
OBJ_DIR := $(TOP_DIR)/obj
outdir := $(TOP_DIR)/bin
QT_DIR := $(TOP_DIR)/qt_pro

SDL_DIR := ./SDL/$(TARGET_ARCH)
FFMPEG_DIR := ./ffmpeg/$(TARGET_ARCH)

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
QMAKE= qmake
CP = cp

CONFIG_COMPILER=gnu

outdir = ./bin

exeobj = PXE

dllobj = PXE.dll

libobj = libPXE.dll.a

qtobj = 

mainobj = main.o tftpd.o log.o socket.o dhcpd.o ping.o client.o list.o dict.o queue.o rbtree.o alarm.o event.o \
		httpd.o signals.o version.o inirw.o ui.o configs.o  tools.o logger.o network.o utils.o pipe.o external.o \
		#mmap_cache.o

cppobj = 

all: $(exeobj)

DEFINES := #-D DLL

CFLAGS := -I.  -I./include/ -L ./lib

ifeq ($(TARGET_ARCH), arm)
CFLAGS +=  
else ifeq ($(TARGET_ARCH), x86)
CFLAGS += -lmingw32 -lm -lws2_32 -lgdi32  -lpthreadGC2  -liphlpapi 
else ifeq ($(TARGET_ARCH), x64)
CFLAGS += -lpthread
endif

CXXFLAGS = $(CFLAGS)

VPATH = .:./include:./lib:./src:


$(exeobj):$(mainobj) $(cppobj)
	$(CXX) $(DEBUG)  -o $(outdir)/$(exeobj) $(mainobj) $(cppobj) $(CXXFLAGS) 
	rm -f *.o
ifeq ($(TARGET_ARCH), x86)
	$(CP) ./lib/*.dll ./bin
endif
	@echo 
	@echo "Build  $(TARGET_ARCH) server program $(exeobj) OK"
	@echo "Version $(VERSION)"
	@echo 

$(mainobj):%.o:%.c
	$(CC) -Wall $(DEBUG) $(CFLAGS) $(DEFINES) -c $< -o $@

$(cppobj):%.o:%.cpp
	$(CXX) -Wall $(DEBUG) $(CXXFLAGS) $(DEFINES) -c $< -o $@


$(dllobj):$(mainobj) $(cppobj)
	$(CXX) $(DEBUG) -shared -o $(outdir)/$(dllobj) $(mainobj) $(cppobj) $(CXXFLAGS) $(DLL) -Wl,--kill-at,--out-implib,$(outdir)/$(libobj) $(DEFINES)
	rm -f *.o
	@echo "Version $(VERSION)"
ifeq ($(TARGET_ARCH), x86)
	#$(CP) ./lib/*.dll $(outdir)
	#$(CP) ./$(QT_DIR)/bin/*.dll $(outdir)
	#$(CP) ./$(SDL_DIR)/bin/*.dll $(outdir)
endif

dll:$(dllobj)
	@echo "build  only dll $(dllobj)"


clean:
	rm -f *.o $(outdir)/$(exeobj) $(outdir)/*.dll $(outdir)/*.a
	#cd $(QT_DIR) && $(MAKE) clean
	#$(MAKE) -C $(QT_DIR) clean

