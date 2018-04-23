CC=g++
IN_DIR=src
OUT_DIR=build

CPPFLAGS+=-ggdb
LDFLAGS=

SQLITE3_LIB=`pkg-config --libs sqlite3`
GLIBMM_LIB=`pkg-config --cflags --libs glibmm-2.4`
GTKMM3_LIB=`pkg-config --libs --cflags gtkmm-3.0`

TARGET=seed_manager

all: $(OUT_DIR) $(OUT_DIR)/$(TARGET)

$(OUT_DIR):
	mkdir -p $@

$(OUT_DIR)/$(TARGET): $(OUT_DIR)/main.o $(OUT_DIR)/Controller.o $(OUT_DIR)/SeedAddWindow.o $(OUT_DIR)/Seed.o $(OUT_DIR)/MainWindow.o $(OUT_DIR)/SeedColumnsModel.o $(OUT_DIR)/Model.o $(OUT_DIR)/SeedInfoWindow.o
	$(CC) $^ $(GTKMM3_LIB) $(SQLITE3_LIB) $(GLIBMM_LIB) $(CPPFLAGS) $(LDFLAGS) -o $@

$(OUT_DIR)/main.o: $(IN_DIR)/main.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

$(OUT_DIR)/Model.o: $(IN_DIR)/Model.cpp
	$(CC) $(CPPFLAGS) $^ -c -o $@ $(GLIBMM_LIB) $(SQLITE3_LIB)

$(OUT_DIR)/Controller.o: $(IN_DIR)/Controller.cpp
	$(CC) $(CPPFLAGS) $^ -c -o $@ $(GLIBMM_LIB)

$(OUT_DIR)/MainWindow.o: $(IN_DIR)/MainWindow.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

$(OUT_DIR)/Seed.o: $(IN_DIR)/Seed.cpp
	$(CC) $(CPPFLAGS) $(SQLITE3_LIB) $(GLIBMM_LIB) $^ -c -o $@

$(OUT_DIR)/SeedColumnsModel.o: $(IN_DIR)/SeedColumnsModel.cpp
	$(CC) $(CPPFLAGS) $^ -c -o $@ $(GTKMM3_LIB)

$(OUT_DIR)/SeedAddWindow.o: $(IN_DIR)/SeedAddWindow.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

$(OUT_DIR)/SeedInfoWindow.o: $(IN_DIR)/SeedInfoWindow.cpp
	$(CC) $(CPPFLAGS) $^ $(GTKMM3_LIB) -c -o $@

clean:
	rm -rf build/
