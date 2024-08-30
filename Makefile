# Project: WaSans
# Makefile created by Embarcadero Dev-C++ 6.3

CPP      = g++.exe
CC       = gcc.exe
WINDRES  = windres.exe
SRC		 = $(wildcard src/*.c)
OBJ      = $(addprefix build/obj/,$(notdir $(SRC:.c=.o)))
LINKOBJ	 = $(addprefix build/obj/,$(notdir $(SRC:.c=.o)))
LIBS     = -L"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/lib" -L"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/x86_64-w64-mingw32/lib" -static-libgcc
INCS     = -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/include" -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/x86_64-w64-mingw32/include" -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/9.2.0/include"
CXXINCS  = -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/include" -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/x86_64-w64-mingw32/include" -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/9.2.0/include" -I"C:/Program Files (x86)/Embarcadero/Dev-Cpp/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/9.2.0/include/c++"
BIN      = build/bin/WaSans.exe
CXXFLAGS = $(CXXINCS) 
CFLAGS   = $(INCS) 
# DEL      = C:\Program Files (x86)\Embarcadero\Dev-Cpp\devcpp.exe INTERNAL_DEL

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

ifeq ($(detected_OS),Windows)
clean:
	del /Q my_program build\obj\*.o
else
clean:
	rm -f my_program build/obj/*.o
endif

# clean: clean-custom
# 	${DEL} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

build/obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# main.o: main.c
# 	$(CC) -c main.c -o main.o $(CFLAGS)

# render.o: render.c
# 	$(CC) -c render.c -o render.o $(CFLAGS)

# settings.o: settings.c
# 	$(CC) -c settings.c -o settings.o $(CFLAGS)

# utils.o: utils.c
# 	$(CC) -c utils.c -o utils.o $(CFLAGS)