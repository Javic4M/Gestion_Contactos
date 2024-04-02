CXX = g++
CXXFLAGS = -std=c++17

SRCS = Sistema_Gestion.cpp
OBJS = $(SRCS:.cpp=.exe)
TARGET = Sistema_Gestion

all:
	$(CXX) $(SRCS) -o $(TARGET) $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)