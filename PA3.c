/*
Author: Dylan Skokan

Sources Used:
https://stackoverflow.com/questions/10468128/how-do-you-make-an-array-of-structs-in-c
https://www.programiz.com/dsa/binary-tree
https://stackoverflow.com/questions/7608714/why-is-my-pointer-not-null-after-free
https://www.tutorialandexample.com/define-in-c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLINELENGTH 500

struct node {
	int value;
	int stage;
	int level;
	struct node *left;
	struct node *right;
};

struct node* createNode(int value, int stage, int level){
	struct node* newNode = (struct node*)malloc(sizeof(struct node));
	newNode->value = value;
	newNode->stage = stage;
	newNode->level = level;
	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

/*
FUNCTION: buildTree

DESCRIPTION:
Builds a tree data structure in a breadth-first fashion based on the array of numbers from the readFile function.

PARAMETERS:
root: a pointer to the root of the tree.
numsPtr: a pointer to the array of integers built in readFile.
numSizePtr: a pointer to the size of the array.
*/
void buildTree(struct node *root, int *numsPtr, int *numSizePtr){
	struct node *nodeArray[*numSizePtr];
	int levelCounter = 1, currLevel = 0, i = 0, j = 0;
	
	for(i = 0; i < *numSizePtr; i++){
		j++;
		if(i == 0){
			*root = *createNode(numsPtr[i], 0, currLevel);
			nodeArray[i] = root;
		} else {
			struct node *newNode = createNode(numsPtr[i], 0, currLevel);
			nodeArray[i] = newNode;
		}
		if(levelCounter == j){
			j = 0;
			levelCounter *= 2;
			currLevel++;
		}
	}
	
	nodeArray[i]->stage = 1;
	for(i = 0; i < *numSizePtr; i++){
		for(j = i + 1; j < *numSizePtr; j++){
			if(nodeArray[j]->stage == 0){
				if(j < *numSizePtr - 1){
					nodeArray[i]->left = nodeArray[j];
					nodeArray[i]->right = nodeArray[j + 1];
					
					nodeArray[j]->stage = 1;
					nodeArray[j + 1]->stage = 1;
					break;
				} else {
					nodeArray[i]->left = nodeArray[j];
						
					nodeArray[j]->stage = 1;
					break;
				}
			}
		}
	}
}

/*
FUNCTION: readFile

DESCRIPTION:
Reads a text file based on a file name passed to it in a parameter and builds an array of integers from the file.
After building the array from the file it calls the buildTree function.

PARAMETERS:
root: a pointer to the root of the.
inputFile: a string to hold the name of the file to process.
numSizePtr: a pointer to the size of the array.

ERROR HANDLING:
The program will exit from this function if:
- The file does not exist or is inaccessible.
- There is a line with a negative symbol that is not at the start of the line or there is a non-digit character in a line.
- The given input file is empty.
*/
void readFile(struct node *root, char inputFile[], int *numSizePtr){
	FILE *filePtr = fopen(inputFile, "r");
	
	if(filePtr == NULL){
      	fprintf(stderr, "Failed accessing input file, exiting...");
      	exit(-1);
	}
	
	char currLine[MAXLINELENGTH];
	int lineCount = 0, i = 0, j = 0;
	
	while (fgets(currLine, MAXLINELENGTH, filePtr) != NULL){
    	i++;
	}

    *numSizePtr = i;
    int nums[*numSizePtr + 1];
    	
    i = 0;
    fclose(filePtr);
    filePtr = fopen(inputFile, "r");
    while (fgets(currLine, MAXLINELENGTH, filePtr) != NULL){
        for(j = 0; j < strlen(currLine) - 1; j++){
      		if(!isdigit(currLine[j]) && !(j == 0 && currLine[j] == '-')) {
      			fprintf(stderr, "Invalid line at line %d, exiting...", i + 1);
      			exit(-1);
			 }
		}
    	nums[i++] = atoi(currLine);
	}
	
    fclose(filePtr);
    
    if(*numSizePtr == 0){
    	fprintf(stderr, "Empty file, exiting...");
      	exit(-1);
	}
    
    buildTree(root, nums, numSizePtr);
}

/*
FUNCTION: breadthFirst

DESCRIPTION:
Traverses the tree in a breadth-first fashion using a queue implementation.

PARAMETERS:
root: a pointer to the root of the tree.
numSizePtr: a pointer to the size of the array/tree.
mode: tells the function if it will traverse the whole tree (0), or it is looking for a specific node (1). More specifically,
mode 1 looks for the parent of the node of the integer passed for unlinking the parent to child for deletion.
numToFind: the integer of the node in the tree to look for, if in mode (1).
*/
struct node* breadthFirst(struct node *root, int *numSizePtr, int mode, int numToFind){
	struct node *queue[*numSizePtr];
	struct node *currNode = root;
	int searchCounter = 0, i = 0;
	
	for(i = 0; i < *numSizePtr; i++){
		queue[i] = NULL;
	}
	
	queue[0] = currNode;
	
	while(currNode != NULL){
		if(currNode->left != NULL){
			for(i = 0; i < *numSizePtr; i++){
				if(queue[i] == NULL){
					queue[i] = currNode->left;
					break;
				}
			}
		}
		
		if(currNode->right != NULL){
			for(i = 0; i < *numSizePtr; i++){
				if(queue[i] == NULL){
					queue[i] = currNode->right;
					break;
				}
			}
		}
		
		if(mode == 0){
			printf("%d\n", queue[0]->value);
			queue[0] = NULL;
		} else if(mode == 1){
			searchCounter++;
			if(searchCounter > *numSizePtr){
				return NULL;
			}
			if(queue[0]->left != NULL){
				if(queue[0]->left->value == numToFind){
					return queue[0];
				}
			}
			if(queue[0]->right != NULL){
				if(queue[0]->right->value == numToFind){
					return queue[0];
				}
			}
		}

		int moveCounter = 0;
		for(i = 0; i < *numSizePtr; i++){
			if(queue[i] != NULL && moveCounter != i){
				queue[moveCounter] = queue[i];
				queue[i] = NULL;
				moveCounter++;
			}
		}

		currNode = queue[0];
	}
}

/*
FUNCTION: depthFirst

DESCRIPTION:
Traverses the tree in a depth-first fashion using recursion.

PARAMETERS:
root: a pointer to the root of the tree.
*/
void depthFirst(struct node *root){
	printf("%d\n", root->value);
	if(root->left != NULL){
		depthFirst(root->left);
	}
	if(root->right != NULL){
		depthFirst(root->right);
	}
}

/*
FUNCTION: removeNode

DESCRIPTION:
Given an integer, finds the node in the tree with that integer if it exists, displays the level the node
is on, and deletes that node and everything below it.

PARAMETERS:
root: a pointer to the root of the tree.
numSizePtr: a pointer to the size of the array/tree.
numToRemove: the integer to look for in the tree for removal.
*/
struct node* removeNode(struct node *root, int *numSizePtr, int numToRemove){
	struct node *nodeToRemoveParent, *nodeToRemove;
	int nodeRemovePosition = -1;
	
	if(root->value == numToRemove){
		nodeToRemove = root;
		nodeRemovePosition = 0;
	} else {
		nodeToRemoveParent = breadthFirst(root, numSizePtr, 1, numToRemove);
		
		if(nodeToRemoveParent == NULL){
			printf("Integer not found in tree.\n");
			return root;
		}
		
		if(nodeToRemoveParent->left->value == numToRemove){
			nodeToRemove = nodeToRemoveParent->left;
			nodeRemovePosition = 1;
		} else if(nodeToRemoveParent->right->value == numToRemove){
			nodeToRemove = nodeToRemoveParent->right;
			nodeRemovePosition = 2;
		}
	}
	
	printf("The node %d at level %d will be removed and everything below it.\n", nodeToRemove->value, nodeToRemove->level);
		
	if (nodeRemovePosition == 0) {
		root = NULL;
	} else if (nodeRemovePosition == 1) {
		free(nodeToRemoveParent->left);
		nodeToRemoveParent->left = NULL;
	} else if (nodeRemovePosition == 2) {
		free(nodeToRemoveParent->right);
		nodeToRemoveParent->right = NULL;	
	}

	return root;
}

/*
FUNCTION: main

DESCRIPTION:
Based on a text file of which its location can be specified by either the command line or as an argument passed
to the program, gathers the integers from the file and builds a tree in a breadth-first fashion. Then the
user can input characters 'a' through 'd' to perform certain actions.
*/
int main(int argc, char *argv[]) {
	struct node root;
	char operation = 'z';
	int numsSize = 0;
	
	if (argv[1] == NULL){
		char inputFile[MAXLINELENGTH];
		
		printf("Specify file with integers:\n");
		scanf("%s", &inputFile);
		readFile(&root, inputFile, &numsSize);
	} else {
		readFile(&root, argv[1], &numsSize);
	}
	
	while (operation != 'd'){
		printf("\nEnter one of the following characters to perform an operation:\n");	
		printf("a: Type in an integer, and output which level of the tree the integer is in, "
		"then remove that integer and everything below it from the tree.\n");
		printf("b: Do a breadth-first traversal of the tree.\n");
		printf("c: Do a depth-first traversal of the tree.\n");
		printf("d: Exit.\n\n");
		
		operation = getchar();
		if(operation == '\n'){
			operation = getchar();	
		}
		
		if (operation == 'a'){
			char opAInput[MAXLINELENGTH] = "";
			int validInput = 0;
			
			printf("\nWhich intenger would you like to remove?\n");

			while(validInput == 0){
				scanf("%s", &opAInput);
				int length = strlen(opAInput);
				validInput = 1;
				
				for (int i = 0; i < length; i++){
					if (!isdigit(opAInput[i]) && !(i == 0 && opAInput[i] == '-')) {
           				validInput = 0;
        			}
				}
				if(validInput == 0){
					printf("Please enter an integer.\n");
				}
			}
			
			int toRemove = atoi(opAInput);
			
			struct node *tempRoot = removeNode(&root, &numsSize, toRemove);
			
			root = (tempRoot == NULL) ? (*createNode(0, 0, 0)) : (*tempRoot);
			
		} else if (operation == 'b'){
			printf("\nBreadth-First Search:\n");
			breadthFirst(&root, &numsSize, 0, 0);
		} else if (operation == 'c'){
			printf("\nDepth-First Search:\n");
			depthFirst(&root);
		} else if (operation == 'd'){
			printf("\nExiting...\n");
			break;
		} else {
			printf("\nPlease choose one of the listed operations.\n");
		}
	}
	
	printf("Program successful");
	return 0;
}