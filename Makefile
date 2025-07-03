BUILD_DIR = build

.PHONY: all clean re

all:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	mv GB_emu ..

clean:
	rm -rf $(BUILD_DIR)/*

re: clean all
