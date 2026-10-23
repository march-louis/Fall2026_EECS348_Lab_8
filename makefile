CXX = g++
CXXFLAGS = -Wall -g
TARGET = prog
OBJECTS = lab8.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

lab8.o: lab8.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f *.o $(TARGET)