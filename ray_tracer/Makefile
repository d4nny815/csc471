PROJECT_NAME = raytracer
IMG_NAME = img.ppm

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Winline -Werror=return-type -fopenmp

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
HEADERS = $(shell find $(SRC_DIR) -name '*.h')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

EXECUTABLE = $(PROJECT_NAME)

all: dirs release

release: CXXFLAGS += -O3
release: dirs $(EXECUTABLE)

debug: CXXFLAGS += -g
debug: clean dirs $(EXECUTABLE)

$(EXECUTABLE): dirs $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: release
	./$(EXECUTABLE) img.ppm 300 20 5
	# ./$(EXECUTABLE) img100_50_small.ppm 600 100 50
	# ./$(EXECUTABLE) img100_50.ppm 1800 100 50
	# ./$(EXECUTABLE) img500_50.ppm 1800 500 50
	# ./$(EXECUTABLE) img500_500.ppm 1800 500 500

dirs:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(IMG_NAME)

.PHONY: all clean dirs debug release
