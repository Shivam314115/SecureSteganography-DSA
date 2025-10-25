#include "../include/huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



HuffmanNode* newNode(char data, unsigned freq) {
    HuffmanNode* temp = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (HuffmanNode**)malloc(minHeap->capacity * sizeof(HuffmanNode*));
    return minHeap;
}

void swapHuffmanNode(HuffmanNode** a, HuffmanNode** b) {
    HuffmanNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx) {
        swapHuffmanNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

HuffmanNode* extractMin(MinHeap* minHeap) {
    HuffmanNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, HuffmanNode* huffmanNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && huffmanNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = huffmanNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

HuffmanNode* build_huffman_tree(char data[], int freq[], int size) {
    HuffmanNode *left, *right, *top;
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    while (minHeap->size != 1) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void printCodes(HuffmanNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
    if (!(root->left) && !(root->right)) {
        printf("%c: ", root->data);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// Not implemented yet
void generate_codes_recursive(HuffmanNode* root, int arr[], int top, int codes[][MAX_TREE_HT]) {
    if (root->left) {
        arr[top] = 0;
        generate_codes_recursive(root->left, arr, top + 1, codes);
    }
    if (root->right) {
        arr[top] = 1;
        generate_codes_recursive(root->right, arr, top + 1, codes);
    }
    if (!(root->left) && !(root->right)) {
        codes[root->data][0] = top;
        for (int i = 0; i < top; ++i)
            codes[root->data][i + 1] = arr[i];
    }
}

void generate_codes(HuffmanNode* root, int codes[][MAX_TREE_HT]) {
    int arr[MAX_TREE_HT], top = 0;
    generate_codes_recursive(root, arr, top, codes);
}
void huffman_encode(char *message, int codes[][MAX_TREE_HT], char *encoded_message) {
    int encoded_index = 0;
    for (int i = 0; message[i] != '\0'; ++i) {
        int char_code_len = codes[message[i]][0];
        for (int j = 0; j < char_code_len; ++j) {
            encoded_message[encoded_index++] = codes[message[i]][j + 1] + '0';
        }
    }
    encoded_message[encoded_index] = '\0';
}
void huffman_decode(char *encoded_message, char *decoded_message, HuffmanNode* root) {
    int decoded_index = 0;
    HuffmanNode* current = root;
    for (int i = 0; encoded_message[i] != '\0'; ++i) {
        if (encoded_message[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (!(current->left) && !(current->right)) {
            if (current->data == (char)255) {
                break;
            }
            decoded_message[decoded_index++] = current->data;
            current = root;
        }
    }
    decoded_message[decoded_index] = '\0';
}
