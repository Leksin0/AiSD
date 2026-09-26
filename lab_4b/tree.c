#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "tree.h"

Tree * mktree(){
	return malloc(sizeof(Tree));
}

void rmtree(Tree * tree){
	while(tree->root->child[0] != NULL){
		delete(tree, tree->root->key[0], 1);
	} // root is the only node
	free(tree->root);
	free(tree);
}

void split(Node * node) {
	Node * left = malloc(sizeof(Node));
	Node * right = malloc(sizeof(Node));
	left->key[0] = node->key[0];
	left->info[0] = node->info[0];
	left->child[0] = node->child[0];
	left->child[1] = node->child[1];
	left->key[1] = NULL;
	left->key[2] = NULL;
	left->child[2] = NULL;
	left->child[3] = NULL;
	right->key[0] = node->key[2];
	right->info[0] = node->info[2];
	right->child[0] = node->child[2];
	right->child[1] = node->child[3];
	right->key[1] = NULL;
	right->key[2] = NULL;
	right->child[2] = NULL;
	right->child[3] = NULL;	
	if(left->child[0] != NULL)
		left->child[0]->parent = left;
	if(left->child[1] != NULL)
		left->child[1]->parent = left;
	if(right->child[0] != NULL)
		right->child[0]->parent = right;
	if(right->child[1] != NULL)
		right->child[1]->parent = right;
	char * midkey = node->key[1];
	char * midinfo = node->info[1];
	if(node->parent == NULL){ // root
		node->key[0] = midkey;
		node->info[0] = midinfo;
		node->key[1] = NULL;
		node->key[2] = NULL;
		node->info[1] = NULL;
		node->info[2] = NULL;
		node->child[0] = left;
		node->child[1] = right;
		node->child[2] = NULL;
		node->child[3] = NULL;
		left->parent = node;
		right->parent= node;
	}
	else{
		Node * parent = node->parent;
		int pos = 0;
		while(parent->child[pos] != node)
			pos++;
		if(parent->key[2] != NULL){
			split(parent);
			// parent(empty)->child[~] still points to node(empty)
			//but node->parent points to newmade left or right of splitted parent
			parent = node->parent; // forced by line 37-44
		}
		for(int i = 2; i > pos; i--) {
			parent->key[i] = parent->key[i-1];
			parent->info[i] = parent->info[i-1];
			parent->child[i+1] = parent->child[i];
		}
		parent->key[pos] = midkey;
		parent->info[pos] = midinfo;
		parent->child[pos] = left;
		parent->child[pos+1] = right;
		left->parent = parent;
		right->parent = parent;
		free(node);// node is useless now
	}
	return;
}

int insert(Tree * tree, char * key, char * info) {
	if (tree->root == NULL) {
		tree->root = malloc(sizeof(Node));
		for (int i = 0; i < 3; i++) {
			tree->root->key[i] = NULL;
			tree->root->info[i] = NULL;
			tree->root->child[i] = NULL;
		}
		tree->root->child[3] = NULL;
		tree->root->parent = NULL;
		tree->root->key[0] = strdup(key);
		tree->root->info[0] = strdup(info);
		return 0; // ok
	}
	Node * current = tree->root;
	while (current->child[0] != NULL){
		for(int i = 0; i < 3; i++){
			if (current->key[i] == NULL) {
				break; // No more keys in this node
			}
			int cmp = strcmp(key, current->key[i]);
			if (cmp < 0) {
				current = current->child[i];
				break;
			} else if (cmp == 0 || (i == 2 && cmp > 0)) {
				// If equal act as if key > current->key
				current = current->child[i + 1];
				break;
			}
		}
		int pos = 0;
		while(pos < 3) {
			if(current->key[pos] == NULL) // i hope it won't break with pos==0
				break;
			int stc = strcmp(key, current->key[pos]);
			if(stc > 0)
				pos++;
			else
				break;
		}
		current = current->child[pos];
	}
		// Step 2: Check if leaf is full and split if needed
		int key_count = 0;
		for (int i = 0; i < 3; i++) {
			if (current->key[i] != NULL) {
				key_count++;
			}
		}
		if (key_count == 3) {
			split(current);
			// After split, we need to continue searching from the new parent
			// or restart from root if the root was split
			if (current->parent == NULL) {
				tree->root = current; // Update root if it changed
			}
		}
		// Step 3: Insert into the target leaf node
		int insert_pos = -1;
		for (int i = 0; i < 3; i++) {
			if (current->key[i] == NULL) {
				insert_pos = i;
				break;
			}
			int cmp = strcmp(key, current->key[i]);
			if (cmp < 0) {
				// Need to shift existing keys to make space
				insert_pos = i;
				break;
			} else if (cmp == 0) {
				// Duplicate key, insert to the right
				insert_pos = i + 1;
				break;
			}
		}
		// If all keys are occupied and new key is largest
		if (insert_pos == -1) {
			insert_pos = 3;
		}
		// Shift keys and info to make space for insertion
		for (int i = 2; i >= insert_pos; i--) {
			if (i < 3) {
				current->key[i] = current->key[i - 1];
				current->info[i] = current->info[i - 1];
			}
		}
		// Insert the new key and info
		current->key[insert_pos] = strdup(key);
		current->info[insert_pos] = strdup(info);
		return 1;
}












void merge(Node * node){
    Node* parent = node->parent;
    if (parent == NULL){  // root
        // Если корень пустой, но имеет детей - делаем первого ребенка новым корнем
        if (node->child[0] != NULL) {
            Node* newRoot = node->child[0];
            newRoot->parent = NULL;

            // Освобождаем память пустого корня
            free(node);
            // Обновляем корень дерева (если нужно передать наружу)
            // В реальной реализации нужно обновить указатель на корень дерева
        }
        return;
    }
	int nodeIndex = 0;
    for (int i = 0; i < 4; i++) {
        if (parent->child[i] == node)
        	nodeIndex = i;
            break;
    }
    if (nodeIndex > 0){
        Node* leftSibling = parent->child[nodeIndex - 1];
        // Проверяем, может ли левый брат отдать ключ (у него больше 1 ключа)
        int leftKeyCount = 0;
        for (int i = 0; i < 3; i++) {
            if (leftSibling->key[i] != NULL) leftKeyCount++;
        }

        if (leftKeyCount > 1) {
            // Берем максимальный ключ у левого брата
            char* keyToTake = NULL;
            char* infoToTake = NULL;
            Node* childToTake = NULL;

            for (int i = 2; i >= 0; i--) {
                if (leftSibling->key[i] != NULL) {
                    keyToTake = leftSibling->key[i];
                    infoToTake = leftSibling->info[i];
                    childToTake = leftSibling->child[i + 1];
                    leftSibling->key[i] = NULL;
                    leftSibling->info[i] = NULL;
                    leftSibling->child[i + 1] = NULL;
                    break;
                }
            }

            // Переносим ключ из родителя в текущий узел
            node->key[0] = parent->key[nodeIndex - 1];
            node->info[0] = parent->info[nodeIndex - 1];

            // Обновляем родительский ключ
            parent->key[nodeIndex - 1] = keyToTake;
            parent->info[nodeIndex - 1] = infoToTake;

            // Переносим ребенка
            if (childToTake != NULL) {
                node->child[1] = childToTake;
                childToTake->parent = node;
            }

            return;
        }
    }

    // Случай 3: Есть правый брат, у которого можно взять ключ
    if (nodeIndex < 3 && parent->child[nodeIndex + 1] != NULL) {
        Node* rightSibling = parent->child[nodeIndex + 1];

        // Проверяем, может ли правый брат отдать ключ
        int rightKeyCount = 0;
        for (int i = 0; i < 3; i++) {
            if (rightSibling->key[i] != NULL) rightKeyCount++;
        }

        if (rightKeyCount > 1) {
            // Берем минимальный ключ у правого брата
            char* keyToTake = NULL;
            char* infoToTake = NULL;
            Node* childToTake = NULL;

            for (int i = 0; i < 3; i++) {
                if (rightSibling->key[i] != NULL) {
                    keyToTake = rightSibling->key[i];
                    infoToTake = rightSibling->info[i];
                    childToTake = rightSibling->child[i];

                    // Сдвигаем ключи правого брата
                    for (int j = i; j < 2; j++) {
                        rightSibling->key[j] = rightSibling->key[j + 1];
                        rightSibling->info[j] = rightSibling->info[j + 1];
                        rightSibling->child[j] = rightSibling->child[j + 1];
                    }
                    rightSibling->key[2] = NULL;
                    rightSibling->info[2] = NULL;
                    rightSibling->child[3] = rightSibling->child[3];
                    rightSibling->child[2] = rightSibling->child[3];
                    rightSibling->child[3] = NULL;

                    break;
                }
            }

            // Переносим ключ из родителя в текущий узел
            node->key[0] = parent->key[nodeIndex];
            node->info[0] = parent->info[nodeIndex];

            // Обновляем родительский ключ
            parent->key[nodeIndex] = keyToTake;
            parent->info[nodeIndex] = infoToTake;

            // Переносим ребенка
            if (childToTake != NULL) {
                node->child[1] = childToTake;
                childToTake->parent = node;
            }

            return;
        }
    }

    // Случай 4: Слияние с братом (ни у кого нельзя взять ключ)
    if (nodeIndex > 0) {
        // Слияние с левым братом
        Node* leftSibling = parent->child[nodeIndex - 1];
        for (int i = 2; i >= 0; i--) {
            if (leftSibling->key[i] != NULL) {
                leftSibling->key[i + 1] = parent->key[nodeIndex - 1];
                leftSibling->info[i + 1] = parent->info[nodeIndex - 1];
                break;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (node->child[i] != NULL) {
                for (int j = 0; j < 4; j++) {
                    if (leftSibling->child[j] == NULL) {
                        leftSibling->child[j] = node->child[i];
                        node->child[i]->parent = leftSibling;
                        break;
                    }
                }
            }
        }
        for (int i = nodeIndex - 1; i < 2; i++) {
            parent->key[i] = parent->key[i + 1];
            parent->info[i] = parent->info[i + 1];
            parent->child[i + 1] = parent->child[i + 2];
        }
        parent->key[2] = NULL;
        parent->info[2] = NULL;
        parent->child[3] = NULL;
        free(node);
        int parentKeyCount = 0;
        for (int i = 0; i < 3; i++) {
            if (parent->key[i] != NULL) parentKeyCount++;
        }
        if (parentKeyCount == 0 && parent->parent != NULL) {
            merge(parent);
        }
    } else if (nodeIndex < 3 && parent->child[nodeIndex + 1] != NULL) {
        Node* rightSibling = parent->child[nodeIndex + 1];
        node->key[0] = parent->key[nodeIndex];
        node->info[0] = parent->info[nodeIndex];
        for (int i = 0; i < 3; i++) {
            if (rightSibling->key[i] != NULL) {
                node->key[i + 1] = rightSibling->key[i];
                node->info[i + 1] = rightSibling->info[i];
                node->child[i + 1] = rightSibling->child[i];
                if (rightSibling->child[i] != NULL) {
                    rightSibling->child[i]->parent = node;
                }
            }
        }
        node->child[3] = rightSibling->child[3];
        if (rightSibling->child[3] != NULL) {
            rightSibling->child[3]->parent = node;
        }
        for (int i = nodeIndex; i < 2; i++) {
            parent->key[i] = parent->key[i + 1];
            parent->info[i] = parent->info[i + 1];
            parent->child[i + 1] = parent->child[i + 2];
        }
        parent->key[2] = NULL;
        parent->info[2] = NULL;
        parent->child[3] = NULL;
        free(rightSibling);
        int parentKeyCount = 0;
        for (int i = 0; i < 3; i++) {
            if (parent->key[i] != NULL) parentKeyCount++;
        }
        if (parentKeyCount == 0 && parent->parent != NULL) {
            merge(parent);
        }
    }
}












void traversal(Node * node, char * key) {
	// if key != NULL print nodes with node->key > key
	if(node->child[0] != NULL){ //node or root
		if(node->child[3] != NULL){
			traversal(node->child[3], key);
			if(key != NULL && strcmp(node->key[2], key) <= 0)
				return;
			printf("%s: %s\n", node->key[2], node->info[2]);
		}
		if(node->child[2] != NULL){
			traversal(node->child[2], key);
			if(key != NULL && strcmp(node->key[1], key) <= 0)
				return;
			printf("%s: %s\n", node->key[1], node->info[1]);
		}
		traversal(node->child[1], key);
		if(key != NULL && strcmp(node->key[2], key) <= 0)
			return;
		printf("%s: %s\n", node->key[0], node->info[0]);
		traversal(node->child[0], key);
	}
	else{ // leaf
		if(node->key[2] != NULL)
			if(key != NULL && strcmp(node->key[2], key) <= 0)
				return;
		printf("%s: %s\n", node->key[2], node->info[2]);
		if(node->key[1] != NULL)
			if(key != NULL && strcmp(node->key[1], key) <= 0)
				return;
		printf("%s: %s\n", node->key[1], node->info[1]);
		if(key != NULL && strcmp(node->key[0], key) <= 0)
			return;
		printf("%s: %s\n", node->key[0], node->info[0]);
	}
}


int search(Tree * tree, char * key, int rel, char ** res){ // res - info
	if(tree->root == NULL)
		return 1; // dumb user
	Node * node = tree->root;
	*res = NULL;
	int pos, stc;
	while(node->child[0] != NULL){
		pos = 0;
		while(pos < 3 && node->key[pos] != NULL){
			stc = strcmp(key, node->key[pos]);
			if(stc > 0)
				pos++;
			else if(stc < 0)
				break;
			else{ // key = node->key[pos]
				rel--; //input rel must be >=1, if rel > amount of duplicates latest dup is returned
				*res = node->info[pos];
				if(rel == 0)
					return 0; // ok
			}
		}
		node = node->child[pos];
	}
	if(*res != NULL)
		return 0; // ok, but rel > amount of duplicates
	else
		return 1; // not found
}

int specsearch(Tree * tree, char * key, int rel, char ** reskey, char ** resinfo){ // closest but different
	if(tree->root == NULL)
		return -1; // dumb user
	Node * node = tree->root;
	*reskey = NULL;
	*resinfo = NULL;
	int pos, stc, mindif = INT_MAX;
	while(node->child[0] != NULL){
		pos = 0;
		while(pos < 3 && node->key[pos] != NULL){
			stc = strcmp(key, node->key[pos]);
			if(abs(stc) < mindif && stc != 0){
				mindif = abs(stc);
				*reskey = node->key[pos];
				*resinfo = node->info[pos];
				if(mindif == 1)
					return 0; // cannot be less than 1
			}
			if(stc >= 0)
				pos++;
			else if(stc < 0)
				break;
		}
		node = node->child[pos];
	}
	return 0; // ok
}

int importfile(Tree *tree, char *filename){
	FILE * file = fopen(filename, "r");
	if (!file)
		return 2; // bad file
	char key[256], info[256];
	int ch;
	while(1){
		if(fgets(key, 255, file) == NULL)
			break;
		ch = fgetc(file);
		while(ch != '\n' && ch != EOF);
		if(fgets(info, 255, file) == NULL)
			break;
		insert(tree, key, info);
	}
	fclose(file);
	return 0;
}

void recurnodetodot(FILE * file, Node * node){
	if(node == NULL)
		return;
	fprintf(file, "\"%s\" [label=\"{{<k1>%s|<k2>%s|<k3%s>}|{<1>|<2>|<3>|<4>}}\"];\n", node->key[0], node->key[0], 
			(node->key[1]!=NULL ? node->key[1] : " "), 
			(node->key[2]!=NULL ? node->key[2] : " "));
	if (node->child[0] != NULL){
		recurnodetodot(file, node->child[0]);
		fprintf(file, "\"%s\":1 -> \"%s\";\n", node->key[0], node->child[0]->key[0]);
	}
	if (node->child[1] != NULL){
		recurnodetodot(file, node->child[1]);
		fprintf(file, "\"%s\":2 -> \"%s\";\n", node->key[0], node->child[1]->key[0]);
	}
	if (node->child[2] != NULL){
		recurnodetodot(file, node->child[2]);
		fprintf(file, "\"%s\":3 -> \"%s\";\n", node->key[0], node->child[2]->key[0]);
	}
	if (node->child[3] != NULL){
		recurnodetodot(file, node->child[3]);
		fprintf(file, "\"%s\":4 -> \"%s\";\n", node->key[0], node->child[3]->key[0]);
	}
	return;
}
int graphviz(Tree * tree){
	if (tree->root == NULL)
		return 1;
	FILE * file = fopen("graph.dot", "w");
	fprintf(file, "digraph G {\n");
	fprintf(file, "node[shape=record];\n");
	recurnodetodot(file, tree->root);
	fprintf(file, "}\n");
	fclose(file);
	return 0;
}

