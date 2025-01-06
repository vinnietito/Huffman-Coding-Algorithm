#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a node in the Huffman tree
struct MinHeapNode {
    char data;
    int freq;
    struct MinHeapNode *left, *right;
};

// Structure to represent a min heap
struct MinHeap {
    int size;
    int capacity;
    struct MinHeapNode** array;
};

// Function to create a new min heap node
struct MinHeapNode* newNode(char data, int freq) {
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// Function to create a min heap
struct MinHeap* createMinHeap(int capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// Function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b) {
    struct MinHeapNode* temp = *a;
    *a = *b;
    *b = temp;
}

// Function to heapify the min heap
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;
    
    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// Function to build the min heap
void buildMinHeap(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(data[i], freq[i]);
    }
    minHeap->size = size;
    for (int i = (minHeap->size - 1) / 2; i >= 0; i--) {
        minHeapify(minHeap, i);
    }
}

// Function to build the Huffman tree
struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(data[i], freq[i]);
    }
    minHeap->size = size;
    while (minHeap->size != 1) {
        struct MinHeapNode* left = minHeap->array[0];
        minHeap->array[0] = minHeap->array[minHeap->size - 1];
        --minHeap->size;
        minHeapify(minHeap, 0);

        struct MinHeapNode* right = minHeap->array[0];
        minHeap->array[0] = minHeap->array[minHeap->size - 1];
        --minHeap->size;
        minHeapify(minHeap, 0);

        struct MinHeapNode* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        minHeap->array[minHeap->size] = top;
        ++minHeap->size;
        minHeapify(minHeap, minHeap->size - 1);
    }
    return minHeap->array[0];
}

// Function to print the Huffman codes from the root of the Huffman tree
void printCodes(struct MinHeapNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (!(root->left) && !(root->right)) {
        printf("Character: %c Code: ", root->data);
        for (int i = 0; i < top; ++i) {
            printf("%d", arr[i]);
        }
        printf("\n");
    }
}

// Function to calculate the size of the compressed text
int calculateCompressedSize(struct MinHeapNode* root, char* text, int* arr) {
    int size = 0;
    for (int i = 0; text[i] != '\0'; ++i) {
        struct MinHeapNode* temp = root;
        while (temp->left || temp->right) {
            if (text[i] == temp->data) {
                size += 1;
            }
            temp = temp->left; // Following left child as an example
        }
    }
    return size;
}

// Main function to drive the program
int main() {
    char text[1000];
    printf("Enter text to compress: ");
    fgets(text, sizeof(text), stdin);

    // Frequency of characters in the text
    int freq[256] = {0};
    for (int i = 0; text[i] != '\0'; ++i) {
        freq[(int)text[i]]++;
    }

    // Prepare data for the Huffman tree
    char data[256];
    int freqCount = 0;
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) {
            data[freqCount] = i;
            freqCount++;
        }
    }

    // Build the Huffman tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, freqCount);

    // Calculate the original text size in bits
    int originalSize = strlen(text) * 8;

    // Print Huffman codes for each character
    int arr[256], top = 0;
    printf("\nHuffman Codes:\n");
    printCodes(root, arr, top);

    // Calculate the compressed size
    int compressedSize = calculateCompressedSize(root, text, arr);

    // Output the original and compressed sizes
    printf("\nOriginal Size: %d bits\n", originalSize);
    printf("Compressed Size: %d bits\n", compressedSize);

    // Calculate and output the compression ratio
    double compressionRatio = ((double)(originalSize - compressedSize) / originalSize) * 100;
    printf("Compression Ratio: %.2f%% space saved\n", compressionRatio);

    return 0;
}

