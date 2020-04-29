CXX := g++
CXXFLAGS += -std=c++17 -O3

all: vectorMapSync

vectorMapSync: main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: all clean

clean:
	rm -f vectorMapSync
