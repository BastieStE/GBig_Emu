BUILD_DIR = build

.PHONY: all clean re purge

all:
	rm -f GB_emu
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && make -s
	mv $(BUILD_DIR)/GB_emu .

clean:
	rm -rf $(BUILD_DIR)/*
	rm -f GB_emu

re:
	cd $(BUILD_DIR) && make clean -s
	cd $(BUILD_DIR) && make -s
	mv $(BUILD_DIR)/GB_emu .

purge: clean all
