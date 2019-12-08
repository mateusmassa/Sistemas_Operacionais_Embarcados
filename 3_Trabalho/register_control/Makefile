MODULE := register_control
TARGET := $(MODULE)
CC := g++
CFLAGS := -g -Wall

INC_DIR := include
SRC_DIR := src
BUILD_DIR := build

LIB :=  -L/usr/local/lib -lopencv_core -lopencv_face -lopencv_highgui -lopencv_imgproc -lopencv_objdetect -lopencv_videoio -lopencv_imgcodecs -L/usr/local/lib/mariadb/ -lmariadb
INCLUDE := -I/usr/local/include/opencv4 -I/usr/local/include/mariadb -I/usr/local/include/mariadb/mysql -I$(INC_DIR)

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJ)
	@echo "Building target: $@"
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)
	@echo " "
	@echo "Finished building target: $@"
	@echo " "

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "Building file: $<"
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<
	@echo " "
	@echo "Finished building file: $<"
	@echo " "

clean:
	@echo "Removing build files..."
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "Finished removing files!"

.PHONY: clean
