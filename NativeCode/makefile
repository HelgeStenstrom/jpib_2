VERSION = 1.01
G++      = g++
GCC      = gcc
AR	 = ar
CFLAGS  = -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\" -z noexecstack -c
#LDFLAGS = -lm -lpthread `gtk-config --cflags` `gtk-config --libs` -lgthread
#LDFLAGS = -c
DEPENDFILE = .depend

SRC = GPIBDriver.cpp
SRC_O = GPIBDriver.o
INC = -Iinclude -I/usr/java/jdk1.8.0_112/include -I/usr/java/jdk1.8.0_112/include/linux

ifeq (x86, $(ARCHITECTURE))
  COMPILER_ARCHITECTURE = -m32

  OBJ_NI = lib32/gcc/ni4882
  OBJ = jpib_32.o
  LIB = libjpib_32.so
else
  COMPILER_ARCHITECTURE = -m64

  OBJ_NI = lib64/gcc/ni4882
  OBJ = jpib_64.o
  LIB = libjpib_64.so
endif 




#dep: $(SRC)
#	$(CC) -MM $(SRC) > $(DEPENDFILE)
# 	-include $(DEPENDFILE)



#prog: $(OBJ)
#	$(CC) $(CFLAGS) -Iinclude -o prog $(OBJ) $(LDFLAGS)

$(LIB): $(SRC)
	gcc $(COMPILER_ARCHITECTURE) -c -fPIC $(INC) $(SRC) -l$(OBJ_NI)
	gcc $(COMPILER_ARCHITECTURE) -shared -o $(LIB) $(SRC_O) $(OBJ_NI).o
#	cp $(LIB) ../../$(LIB)

#$(LIB): $(OBJ)
#	$(GCC) -shared -Wl,-soname,$(LIB) -o $(LIB) $(OBJ) -ldl	

	

.PHONY: clean
clean:
	rm -rf $(OBJ) $(LIB)
