PROJECT_NAME = raytracer
IMG_NAME = img.ppm

NVCC = nvcc

NVCC_FLAGS = -arch=sm_61 -Wno-deprecated-gpu-targets 

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(shell find $(SRC_DIR) -name '*.cu')
HEADERS = $(shell find $(SRC_DIR) -name '*.h')
OBJECTS = $(patsubst $(SRC_DIR)/%.cu, $(BUILD_DIR)/%.o, $(SOURCES))

EXECUTABLE = $(PROJECT_NAME)

all: dirs release

release: NVCC_FLAGS += -O3
release: dirs $(EXECUTABLE)

debug: NVCC_FLAGS += -g
debug: clean dirs $(EXECUTABLE)

$(EXECUTABLE): dirs $(OBJECTS)
	$(NVCC) $(NVCC_FLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cu $(HEADERS)
	$(NVCC) $(NVCC_FLAGS) -c $< -o $@

run: release
	./$(EXECUTABLE) img.ppm 300 20 5

dirs:
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(IMG_NAME)

.PHONY: all clean dirs debug release
