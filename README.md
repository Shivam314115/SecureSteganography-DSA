# Secure Steganography (DSA)

This project implements a secure steganography system in C, designed to hide secret text messages within digital images. It utilizes bit-level manipulation and incorporates concepts from Data Structures and Algorithms (DSA), such as bit manipulation, binary conversions, and modular programming.

## Features

*   **Message Embedding:** Converts text messages into binary, encrypts them (using a simple XOR scheme), and embeds them within the Least Significant Bits (LSB) of a BMP image.
*   **Message Extraction:** Extracts and decrypts hidden messages from a stego image.
*   **Huffman Coding:** Implements Huffman coding for message compression, reducing the size of the secret message before embedding, thereby increasing efficiency and demonstrating tree-based data structures.
*   **Data Structures:** Includes a basic stack implementation.
*   **Error-Correcting Code (ECC):** Features a simple repetition code for basic error correction.
*   **Discrete Cosine Transform (DCT):** Includes functions for forward and inverse DCT, though the primary steganography currently uses LSB for simplicity and robustness.

## How to Use

### 1. Compile the Project

To compile the project, navigate to the root directory of the project and run the `make` command:

```bash
make
```

This will compile all source files and create an executable named `main` in the `bin` directory.

### 2. Running the Executable

The `main` executable is a command-line tool for embedding and extracting messages.

#### Embed a Message

To hide a secret message within a cover image:

```bash
./bin/main embed <cover_image_path> <secret_message_file> <stego_image_path>
```

*   `<cover_image_path>`: Path to the original image (must be a 24-bit uncompressed BMP).
*   `<secret_message_file>`: Path to the text file containing the secret message.
*   `<stego_image_path>`: Path where the output image (with the hidden message) will be saved.

**Example:**

```bash
./bin/main embed data/input.bmp data/secret.txt data/stego.bmp
```

#### Extract a Message

To retrieve a hidden message from a stego image:

```bash
./bin/main extract <stego_image_path> <output_file_path>
```

*   `<stego_image_path>`: Path to the image containing the hidden message.
*   `<output_file_path>`: Path to the text file where the extracted message will be saved.

**Example:**

```bash
./bin/main extract data/stego.bmp data/extracted_message.txt
```

### 3. Supported Image Format

Currently, the project only supports **24-bit uncompressed BMP** images. If your image is in a different format (e.g., PNG, JPEG), you will need to convert it to BMP first.

You can use tools like **ImageMagick** for conversion. For example:

```bash
convert your_image.png your_image.bmp
```

### 4. Running Tests

To ensure all components are working correctly, you can run the various test suites:

*   **Utility Functions:**
    ```bash
    make test
    ```
*   **Image Handling:**
    ```bash
    make test_image
    ```
*   **Steganography Functions (LSB-based):**
    ```bash
    make test_stego
    ```
*   **Huffman Coding (Unit Test):**
    ```bash
    make test_huffman
    ```
*   **Huffman Coding (Simple Test):**
    ```bash
    make test_huffman_simple
    ```
*   **Huffman Coding (Integration Test):**
    ```bash
    make test_huffman_integration
    ```
*   **Data Structures (Stack):**
    ```bash
    make test_ds
    ```
*   **Error-Correcting Code:**
    ```bash
    make test_ecc
    ```
*   **Discrete Cosine Transform:**
    ```bash
    make test_dct
    ```

To run all tests:

```bash
make test_all
```
