# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g

# Directories
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

# Executable names
TARGET = $(BIN_DIR)/main
TEST_TARGET_UTILS = $(BIN_DIR)/test_utils
TEST_TARGET_IMAGE = $(BIN_DIR)/test_image
TEST_TARGET_STEGO = $(BIN_DIR)/test_stego
TEST_TARGET_HUFFMAN = $(BIN_DIR)/test_huffman
TEST_TARGET_HUFFMAN_SIMPLE = $(BIN_DIR)/test_huffman_simple
TEST_TARGET_HUFFMAN_INTEGRATION = $(BIN_DIR)/test_huffman_integration

test_all: test test_image test_stego test_huffman test_huffman_simple test_ds test_ecc test_dct

.PHONY: all clean test test_image test_stego test_huffman test_huffman_simple test_ds test_ecc test_dct test_huffman_integration test_all

all: $(TARGET)

# Main executable
$(TARGET): $(filter %main.o, $(OBJS)) $(filter %utils.o, $(OBJS)) $(filter %image.o, $(OBJS)) $(filter %dct.o, $(OBJS)) $(filter %stego.o, $(OBJS)) $(filter %huffman.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Test executables
test: $(TEST_TARGET_UTILS)
	$(TEST_TARGET_UTILS)

$(TEST_TARGET_UTILS): $(filter %test_utils.o, $(TEST_OBJS)) $(filter %utils.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test_image: $(TEST_TARGET_IMAGE)
	$(TEST_TARGET_IMAGE)

$(TEST_TARGET_IMAGE): $(filter %test_image.o, $(TEST_OBJS)) $(filter %image.o, $(OBJS)) $(filter %utils.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test_stego: $(TEST_TARGET_STEGO)
	$(TEST_TARGET_STEGO)

$(TEST_TARGET_STEGO): $(filter %test_stego.o, $(TEST_OBJS)) $(filter %stego.o, $(OBJS)) $(filter %dct.o, $(OBJS)) $(filter %image.o, $(OBJS)) $(filter %utils.o, $(OBJS)) $(filter %huffman.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

test_huffman: $(TEST_TARGET_HUFFMAN)
	$(TEST_TARGET_HUFFMAN)

$(TEST_TARGET_HUFFMAN): $(filter %test_huffman.o, $(TEST_OBJS)) $(filter %huffman.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test_huffman_simple: $(TEST_TARGET_HUFFMAN_SIMPLE)
	$(TEST_TARGET_HUFFMAN_SIMPLE)

$(TEST_TARGET_HUFFMAN_SIMPLE): $(filter %test_huffman_simple.o, $(TEST_OBJS)) $(filter %huffman.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test_ds: $(TEST_TARGET_DS)
	$(TEST_TARGET_DS)

$(TEST_TARGET_DS): $(filter %test_ds.o, $(TEST_OBJS)) $(filter %ds.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test_ecc: $(TEST_TARGET_ECC)
	$(TEST_TARGET_ECC)

$(TEST_TARGET_ECC): $(filter %test_ecc.o, $(TEST_OBJS)) $(filter %ecc.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

test_dct: $(TEST_TARGET_DCT)
	$(TEST_TARGET_DCT)

$(TEST_TARGET_DCT): $(filter %test_dct.o, $(TEST_OBJS)) $(filter %dct.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

test_huffman_integration: $(TEST_TARGET_HUFFMAN_INTEGRATION)
	$(TEST_TARGET_HUFFMAN_INTEGRATION)

$(TEST_TARGET_HUFFMAN_INTEGRATION): $(filter %test_huffman_integration.o, $(TEST_OBJS)) $(filter %stego.o, $(OBJS)) $(filter %huffman.o, $(OBJS)) $(filter %utils.o, $(OBJS)) $(filter %image.o, $(OBJS))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Object file rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)