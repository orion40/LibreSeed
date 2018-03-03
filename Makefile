CC=g++
IN_DIR=src
OUT_DIR=build

CPPFLAGS+=-ggdb

SQLITE3_LIB=`pkg-config --libs sqlite3`
GTKMM3_LIB=`pkg-config --libs --cflags gtkmm-3.0`

TARGET=seed_manager

all: $(OUT_DIR) $(OUT_DIR)/$(TARGET)

$(OUT_DIR):
	mkdir -p $@

$(OUT_DIR)/$(TARGET): $(OUT_DIR)/main.o $(OUT_DIR)/Controller.o $(OUT_DIR)/SeedEditWindow.o $(OUT_DIR)/Seed.o $(OUT_DIR)/MainWindow.o $(OUT_DIR)/SeedColumnsModel.o
	$(CC) $^ $(GTKMM3_LIB) $(SQLITE3_LIB) $(CPPFLAGS) -o $@

$(OUT_DIR)/main.o: $(IN_DIR)/main.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

$(OUT_DIR)/Controller.o: $(IN_DIR)/Controller.cpp
	$(CC) $(CPPFLAGS) $^ -c -o $@

$(OUT_DIR)/MainWindow.o: $(IN_DIR)/MainWindow.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

$(OUT_DIR)/Seed.o: $(IN_DIR)/Seed.cpp
	$(CC) $(CPPFLAGS) $^ -c -o $@

$(OUT_DIR)/SeedColumnsModel.o: $(IN_DIR)/SeedColumnsModel.cpp
	$(CC) $(CPPFLAGS) $^ -c -o $@ $(GTKMM3_LIB)

$(OUT_DIR)/SeedEditWindow.o: $(IN_DIR)/SeedEditWindow.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

clean:
	rm -rf build/
