CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Iinclude

TARGET = campus_reservations
SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard include/*.h)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

.PHONY: run clean

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
