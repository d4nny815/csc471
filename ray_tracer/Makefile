PROJECT_NAME = raytracer
IMG_NAME = img.ppm

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Winline -Werror=return-type

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

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(EXECUTABLE)
	@./$(EXECUTABLE) > $(IMG_NAME)

dirs:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(IMG_NAME)

.PHONY: all clean dirs debug release
