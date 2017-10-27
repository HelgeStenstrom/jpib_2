VERSION = 1.00
G++      = g++
GCC      = gcc
CFLAGS  = -Wall -g -D_REENTRANT -DVERSION=\"$(VERSION)\"
#LDFLAGS = -lm -lpthread `gtk-config --cflags` `gtk-config --libs` -lgthread
DEPENDFILE = .depend

SRC = LinuxGPIBDriver.cpp
OBJ = gpib_lin64.o
LIB = gpib_lin64.so
INC = -Iinclude -I/usr/lib/jvm/java-1.5.0-gcj-4.5/include

#dep: $(SRC)
#	$(CC) -MM $(SRC) > $(DEPENDFILE)
# 	-include $(DEPENDFILE)



#prog: $(OBJ)
#	$(CC) $(CFLAGS) -Iinclude -o prog $(OBJ) $(LDFLAGS)

$(LIB): $(SRC)
#	$(GCC) $(CFLAGS) -I$(INC)  -c $<
	$(GCC) $(SRC) $(INC) -ldl -o $(LIB)


#$(LIB): $(OBJ)
#	$(GCC) -shared -Wl,-soname,$(LIB) -o $(LIB) $(OBJ) -ldl	

	

.PHONY: clean
clean:
	rm -rf $(OBJ) $(LIB)
