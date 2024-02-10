#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//ANSI color codes //!doesn't work in windows cmd
#define RESET   "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"

#define MAX_TREE_HT 50

//    frequency functions
//find size of data in file
int findSize(FILE *fptr){
  char temp;
  int size = 0;
  while (1) {
    temp = fgetc(fptr);
    size++;
    //check
    // printf("temp in findSize: %c ", temp);
    if (feof(fptr)){
      break ;
    } 
  }
  return size;
}

//find all chars
void getChar(char chars[], char arr[], int size){
  for (int i = 0; i < size; i++){
    chars[arr[i]] = arr[i];
  }
}

//find each char's frequency
void getFreq(int freq[], char arr[], int size){
  for (int i = 0; i < size; i++){
    freq[arr[i]]++;
  }
}

//find non-star values' count
int findCount(char chars[]){
  int count = 0;
  for (int i = 0; i < 128; i++){
    if (chars[i] != '*'){
      count++;
      }
  }
  return count;
}

//find non-zero freq
void getNewFreq(int freq[], int newFreq[]){
  int j = 0;
  for (int i = 0; i < 128; i++){
    if (freq[i] > 0){
      newFreq[j] = freq[i];
      j++;
    }
  }
}

//find non-star chars
void getnewChar(char chars[], char newChar[]){
  int j = 0;
  for (int i = 0; i < 128; i++){
    if (chars[i] != '*'){
      newChar[j] = chars[i];
      j++;
    }
  }
}

//    huffman structs
//node struct
struct MinHNode {
  char item;
  unsigned freq;
  struct MinHNode *left, *right;
};

//collection of nodes
struct MinHeap {
  unsigned size;
  unsigned capacity;
  struct MinHNode **array;
};

//    huffman functions
//create nodes
struct MinHNode *newNode(char item, unsigned freq) { 
  struct MinHNode *temp = (struct MinHNode *)malloc(sizeof(struct MinHNode)); 

  temp->left = temp->right = NULL; 
  temp->item = item; 
  temp->freq = freq; 

  return temp;
}

//create min heap (collection of nodes)
struct MinHeap *createMinH(unsigned capacity) {
  struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
  minHeap->size = 0;

  minHeap->capacity = capacity;

  minHeap->array = (struct MinHNode **)malloc(minHeap->capacity * sizeof(struct MinHNode *));
  return minHeap;
}

//swap nodes
void swapMinHNode(struct MinHNode **a, struct MinHNode **b) {
  struct MinHNode *t = *a;
  *a = *b;
  *b = t;
}

//sort nodes
void minHeapify(struct MinHeap *minHeap, int idx) { 
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
    smallest = left; 
  if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) 
    smallest = right;  

  if (smallest != idx) { 
    swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]); 
    minHeapify(minHeap, smallest); 
  } 
}

//check if size of collection is 1
int checkSizeOne(struct MinHeap *minHeap) {
  return (minHeap->size == 1);
}

//get min node          
struct MinHNode *extractMin(struct MinHeap *minHeap) { 
  struct MinHNode *temp = minHeap->array[0]; 
  minHeap->array[0] = minHeap->array[minHeap->size - 1]; 

  --minHeap->size; 
  minHeapify(minHeap, 0);
  return temp;
}

//insert node
void insertMinHeap(struct MinHeap *minHeap, struct MinHNode *minHeapNode) { 
  ++minHeap->size; 
  int i = minHeap->size - 1; 
       
  while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {     
    minHeap->array[i] = minHeap->array[(i - 1) / 2];   
    i = (i - 1) / 2; 
  }
  minHeap->array[i] = minHeapNode;   
}

//remake collection
void buildMinHeap(struct MinHeap *minHeap) { 
  int n = minHeap->size - 1; 
  int i;

  for (i = (n - 1) / 2; i >= 0; --i) 
    minHeapify(minHeap, i); 
}

//check if node is leaf
int isLeaf(struct MinHNode *root) {
  return !(root->left) && !(root->right);
}

//create collection
struct MinHeap *createAndBuildMinHeap(char item[], int freq[], int size) {  
  struct MinHeap *minHeap = createMinH(size);

  for (int i = 0; i < size; ++i)
    minHeap->array[i] = newNode(item[i], freq[i]);

  minHeap->size = size; 
  buildMinHeap(minHeap); 

  return minHeap;
}

//main function
struct MinHNode *buildHuffmanTree(char item[], int freq[], int size) {
  struct MinHNode *left, *right, *top; 
  struct MinHeap *minHeap = createAndBuildMinHeap(item, freq, size);

  while (!checkSizeOne(minHeap)) { 
    left = extractMin(minHeap); 
    right = extractMin(minHeap); 

    top = newNode('$', left->freq + right->freq);

    top->left = left;
    top->right = right;

    insertMinHeap(minHeap, top); 
  }
  return extractMin(minHeap);   
}

// Print the array --- used for checking only
// void printArray(int arr[], int n) { 
//   int i;
//   for (i = 0; i < n; ++i)
//   printf("%d", arr[i]); 
//   printf("\n");
// }

//print the array in file
void printArrayInFile(int arr[], int n, FILE *fptr) { 
  int i;
  for (i = 0; i < n; ++i)
  fprintf(fptr, "%d", arr[i]);
  fprintf(fptr, "%s", "\n");
}

//find array of code
void printHCodes(struct MinHNode *root, int arr[], int top, FILE *fptr) { 
  if (root->left) {  
    arr[top] = 0;   
    printHCodes(root->left, arr, top + 1, fptr); 
  }
  if (root->right) { 
    arr[top] = 1; 
    printHCodes(root->right, arr, top + 1, fptr); 
  }
  if (isLeaf(root)) {
    // printf("  %c   | ", root->item); //used for checking only
    fprintf(fptr, "%c-", root->item); 
    // printArray(arr, top); //used for checking only
    printArrayInFile(arr, top, fptr);
  }
}

//wrapper function
void HuffmanCodes(char item[], int freq[], int size, FILE *fptr) { 
  struct MinHNode *root = buildHuffmanTree(item, freq, size); 

  int arr[MAX_TREE_HT], top = 0; 

  printHCodes(root, arr, top, fptr);
}

//    driver code
int main(int argc, char *argv[]){

  //check command line args
	// printf("The value of argc is %d\n", argc);
	// for (int i = 0; i < argc; i++) {
  //       printf("argv[%d]: %s\n", i, argv[i]);
	// }

  //error not enough args
  if (argc < 2){
    printf("%s%s%s%s%s", RED, "Something went wrong.\n"RESET, "try: ./run help", RESET);
    return 1;
  }

  //    help 
  if (strcmp(argv[1], "help") == 0){
    printf("%s%s", "If you want to ", YELLOW);
    printf("%s%s", "compress ", RESET);
    printf("%s%s", "a file:\n", YELLOW);
    printf("%s%s", "./run compress file.txt\n", RESET);
    printf("%s%s", "If you're file is located in a diffrent folder, for example in drive D\n", YELLOW);
    printf("%s%s", "./run compress D:\\path\\to\\your\\file.txt\n", RESET);

    printf("%s%s", "If you want to ", YELLOW);
    printf("%s%s", "decompress ", RESET);
    printf("%s%s", "a file:\n", YELLOW);
    printf("%s%s", "./run decompress file.txt\n", RESET);
    printf("%s%s", "If you're file is located in a diffrent folder, for example in drive D\n", YELLOW);
    printf("%s%s", "./run decompress D:\\path\\to\\your\\file.txt\n", RESET);
    }

  //    compress
  else if (strcmp(argv[1], "compress") == 0){
    //error file not addressed 
    if (argc < 3){
      printf("%s%s%s",RED, "File is needed.\n", RESET);
      return 1;
    }
    printf("%s%s%s", BLUE, "compressing...\n", RESET);
    
    //file pointers
    FILE *fptr, *fhuff, *fout;

    //get file name from the file path
    char *filePath = argv[2];
    //strrchr searches for the last occurrence of a character
    char *fileName = strrchr(filePath, '\\'); //Windows 
    if (fileName == NULL){
      fileName = strrchr(filePath, '/'); //Unix-like systems
    }
    //if no directory separator is found, use the entire path as file name
    if (fileName == NULL){
      fileName = filePath;
    } 
    else{
      //move to the next character after the directory separator
      fileName++;
    }
    //check file name
    // printf("File name: %s\n", fileName);

    //open the file for reading
    fptr = fopen(filePath, "r");
    //error opening file
    if (fptr == NULL){
      printf("%s%s%s",RED, "Could not open file.\n", RESET);
      return 1;
    }

    //get size of data in file       
    int size = findSize(fptr);
    //check
    // printf("size: %d\n", size);
        
    //reset fptr
    fseek(fptr, 0, SEEK_SET);

    //find lines
    char arr[size];
    int numLines = 0;
    if (fgets(arr, sizeof(arr), fptr) == NULL){
        printf("%s%s%s",RED, "Error reading arr.\n", RESET);
        return 1;
    }
    //reset fptr
    fseek(fptr, 0, SEEK_SET);
    while (fgets(arr, sizeof(arr), fptr) != NULL) {
      numLines++;
    }
    //check numLines
    // printf("\nnumLines: %d\n", numLines);

    //reset fptr
    fseek(fptr, 0, SEEK_SET);

    //allocate memory for the array of strings
    char **dataArray = (char **)malloc(numLines * sizeof(char *));
    if (dataArray == NULL){
      printf("%s%s%s",RED, "Memory allocation failed for dataArray.\n", RESET);
      return 1;
    }

    //read each line from the file and store it in the array
    for (int i = 0; i < numLines; i++){
      dataArray[i] = (char *)malloc(size * sizeof(char)); //CHANGE SIZE??
      //error reading line
      if (fgets(dataArray[i], size, fptr) == NULL){
        printf("%s%s%s",RED, "Error reading line %d from the file.\n", RESET);
        return 1;
      }
    }
    //check each line
    // for (int i = 0; i < numLines; i++){
    //   printf("Line %d: %s", i + 1, dataArray[i]);
    // }
    // printf("\n");

    //find total length
    int totalLength = 0;
    for (int i = 0; i < numLines; i++){
      totalLength += strlen(dataArray[i]);
    }
    //check
    // printf("totalLenght: %d\n", totalLength);

    //store data in a 1D array
    char *oneDArray = (char *)malloc(totalLength + 1); //+1 null 
    if (oneDArray == NULL){
      printf("%s%s%s",RED, "Memory allocation failed for oneDArray.\n", RESET);
      return 1;
    }
    //copy each line into oneDArray
    int currentPosition = 0; //works like index in arrays
    for (int i = 0; i < numLines; i++){
      strcpy(oneDArray + currentPosition, dataArray[i]);
      currentPosition += strlen(dataArray[i]);
    }
    //check 1D array
    // printf("One-Dimensional Array:\n%s", oneDArray);
    // printf("\n");

    //arr to collect the chars (i had no idea how i could initialize it :( )
    char chars[128] = {'*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*', '*'};
    getChar(chars, oneDArray, totalLength + 1);
    //check chars
    // for (int i = 0; i < 128; i++){
    //   printf("chars[%d] = %c\t", i, chars[i]);
    // }
    // printf("%s", "\n\n");

    //arr to find frequency of chars
    int freq[128] = {0};
    getFreq(freq, oneDArray, size);
    //check freq
    // for (int i = 0; i < 128; i++){
    //   printf("freq[%d] = %d\t", i, freq[i]);
    // }
    // printf("%s", "\n");

    //find number of non-zero values
    int count = findCount(chars);
    //check
    // printf("count: %d\n", count);

    //new freq to store frequencies > 0
    int newFreq[count];
    getNewFreq(freq, newFreq);
        
    //new arr to store chars
    char newChar[count];
    getnewChar(chars, newChar);

    //check new char and freq
    // for (int i = 0; i < count; i++){
    //   printf("newFreq[%d] = %d ", i, newFreq[i]);
    // }
    // printf("%s", "\n");
    // for (int i = 0; i < count; i++){
    //   printf("newChar[%d] = %c ", i, newChar[i]);
    // }
    // printf("%s", "\n");

    //huffman
    // printf(" Char | Huffman code ");
    // printf("\n--------------------\n");

    //file of huffman code
    fhuff = fopen("huffman.txt", "w");
    HuffmanCodes(newChar, newFreq, count, fhuff);
    fclose(fhuff);
    fhuff = fopen("huffman.txt", "r");
    fout = fopen("output.txt", "w");

    //free memory
    for (int i = 0; i < numLines; i++) {
      free(dataArray[i]);
    }
    free(dataArray);
   

    //    convert

    fseek(fhuff, 0, SEEK_SET);
    
    //find lines
    int numLinesHuff = 0;
    
    if (fgets(arr, sizeof(arr), fhuff) == NULL){
        printf("%s%s%s",RED, "Error reading arr.\n", RESET);
        return 1;
    }
    while (fgets(arr, sizeof(arr), fhuff) != NULL) {
      numLinesHuff++;
    }
    //reset fptr
    fseek(fhuff, 0, SEEK_SET);
    // printf("numLinesHuff: %d\n", numLinesHuff);

    //allocate memory for the array of strings
    char **huffArray = (char **)malloc(numLinesHuff * sizeof(char *));
    if (huffArray == NULL){
      printf("%s%s%s",RED, "Memory allocation failed for huffArray.\n", RESET);
      return 1;
    }

    //read each line from the file and store it in the array
    for (int i = 0; i < numLinesHuff; i++){
      huffArray[i] = (char *)malloc((count + 1) * sizeof(char)); //count + 1
      
      if (huffArray[i] == NULL){
      printf("%s%s%s",RED, "Memory allocation failed for huffArray's strings.\n", RESET);
      return 1;
     }

      //error reading line
      if (fgets(huffArray[i], (count + 1), fhuff) == NULL){ //count + 1
        printf("%s%s%s",RED, "Error reading line %d from the file.\n", RESET);
        return 1;
      }
    }
    //reset fhuff
    fseek(fhuff, 0, SEEK_SET);

    //check each line
    // for (int i = 0; i < numLinesHuff; i++){
    //   printf("Line %d: %s", i + 1, huffArray[i]);
    // }
    // printf("\n");

    char temp;
    char *bin = (char *)malloc(10000 * sizeof(char)); //MAYBE CHANGE 1000?
    if (bin == NULL){
      printf("%s%s%s",RED, "Memory allocation failed for bin.\n", RESET);
      return 1;
    }
    //bin is binary of input file using huffman code 
    int idx = 0;
    for (int i = 0; i < totalLength; i++){
      temp = oneDArray[i];
      // printf("%c ", temp);
      for (int j = 0; j < 12; j++){
        if (temp == huffArray[j][0] && (temp == '\n' || temp == '\r')){
          for(int k = 1; huffArray[j+1][k+1] != '\0'; k++){
            bin[idx] = huffArray[j+1][k];
            idx++;
          }
        }
        else if (temp == huffArray[j][0] && huffArray[j][1] != '\0'){ 
          for(int k = 2; huffArray[j][k+1] != '\0'; k++){
            bin[idx] = huffArray[j][k];
            idx++;
          }
          break;
        }      
      }
    }
    
    //free huffarray
    free(huffArray);
    free(oneDArray);

    //check bin
    // printf("\n%d\n", bin[0]);
    // int z = 0;
    // printf("%s", "\nbin: ");
    // while(bin[z] != '\0'){
    //   printf("%c", bin[z]);
    //   z++;
    // }

    int length = strlen(bin);
    // printf("\nlength: %d", length);
    //oneZero is the same thing as bin, but int (not char)
    int *oneZero = (int *)malloc(length * sizeof(int));
    if (oneZero == NULL){
      printf("%s%s%s",RED, "Memory allocation failed for oneZero.\n", RESET);
      return 1;
    }
    
    for (int i = 0; bin[i] != '\0'; i++){
      oneZero[i] = bin[i] - '0';
    }

    //check oneZero
    // printf("\n%s", "oneZero: ");
    // for (int i = 0; i < length; i++){
    //   printf("%d", oneZero[i]);
    // }

    int six[6];
    int o = 0; //index for oneZero
    int mod = length % 6;
    int x = ceil((length/6));
    // printf("\n ceil((length/6)): %d\n", x);
    if (mod == 0){
      for (int i = 0; i < x; i++){
      int s = 0;
      for (o; s < 6; o++){
      six[s] = oneZero[o];
      s++;
      }
      //check
      // printf("\nsix: ");
      // for (int i = 0; i < 6; i++){
      //   printf("%d", six[i]);
      // }
      int ascii = 0;
      for(int i = 0; i < 6; i++){
        ascii += six[i] * pow(2, 5-i);
      }
      // printf("\nascii: %d", ascii);
      char asciiChar;
      asciiChar = ascii + '0';
      // printf("\nasciiChar: %c", asciiChar);
      fputc(asciiChar, fout);
      } 
    }
    else{
      for (int i = 0; i < x + 1; i++){
      int s = 0;
      if (mod != 0 && i == x){
        int zeros = 6 - mod;
        for (int i = 0; i < zeros; i++){
          six[s] = 0;
          s++;
        }
      }
      for (o; s < 6; o++){
      six[s] = oneZero[o];
      s++;
      }
      //check
      // printf("\nsix: ");
      // for (int i = 0; i < 6; i++){
      //   printf("%d", six[i]);
      // }
      int ascii = 0;
      for(int i = 0; i < 6; i++){
        ascii += six[i] * pow(2, 5-i);
      }
      // printf("\nascii: %d", ascii);
      char asciiChar;
      asciiChar = ascii + '0';
      // printf("\nasciiChar: %c", asciiChar);
      fputc(asciiChar, fout);
      }
    }

    //free memory
    free(bin);
    free(oneZero);
    //close file 
    fclose(fptr);
    fclose(fhuff);
    fclose(fout);
    }

    //decompress
    else if (strcmp(argv[1], "decompress") == 0){
      if (argc < 3){
      printf("%s%s%s",RED, "File is needed.\n", RESET);
      return 1;
      }
      printf("%s%s%s", BLUE, "decompressing...", RESET);

    //pretend like i've coded this part :)

    }

    //error in executing
    else{
      printf("%s%s%s%s%s", RED, "Something went wrong.\n"RESET, "try: ./run help", RESET);
      return 1;
    }

    return 0;
}