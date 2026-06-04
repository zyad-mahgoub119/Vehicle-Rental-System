#pragma once
#include <vector>
#include <algorithm>
#include "Car.h"

//Node
struct AVLNode {
    Car carData;
    int height;
    AVLNode* leftChild;
    AVLNode* rightChild;

    //Constructor
    AVLNode(Car car) {
        carData = car;
        height = 1;
        leftChild = nullptr;
        rightChild = nullptr;
    }
};

class AVLTree {
private:
    AVLNode* root;

    int getNodeHeight(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    int getBalanceFactor(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        //Balance Factor = height(left) - height(right) [cite: 41]
        return getNodeHeight(node->leftChild) - getNodeHeight(node->rightChild);
    }

    void updateNodeHeight(AVLNode* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(getNodeHeight(node->leftChild), getNodeHeight(node->rightChild));
        }
    }

    AVLNode* rotateRight(AVLNode* criticalNode) {
        AVLNode* newSubtreeRoot = criticalNode->leftChild;
        AVLNode* temporaryChild = newSubtreeRoot->rightChild;

        newSubtreeRoot->rightChild = criticalNode;
        criticalNode->leftChild = temporaryChild;

        updateNodeHeight(criticalNode);
        updateNodeHeight(newSubtreeRoot);

        return newSubtreeRoot;
    }

    AVLNode* rotateLeft(AVLNode* criticalNode) {
        AVLNode* newSubtreeRoot = criticalNode->rightChild;
        AVLNode* temporaryChild = newSubtreeRoot->leftChild;

        newSubtreeRoot->leftChild = criticalNode;
        criticalNode->rightChild = temporaryChild;

        updateNodeHeight(criticalNode);
        updateNodeHeight(newSubtreeRoot);

        return newSubtreeRoot;
    }

    AVLNode* insertHelper(AVLNode* currentNode, Car car) {
        if (currentNode == nullptr) {
            return new AVLNode(car);
        }

        if (car.id < currentNode->carData.id) {
            currentNode->leftChild = insertHelper(currentNode->leftChild, car);
        }
        else if (car.id > currentNode->carData.id) {
            currentNode->rightChild = insertHelper(currentNode->rightChild, car);
        }
        else {
            return currentNode;
        }

        updateNodeHeight(currentNode);

        int balanceFactor = getBalanceFactor(currentNode);

        //Case 1: Left-Left
        if (balanceFactor > 1 && car.id < currentNode->leftChild->carData.id) {
            return rotateRight(currentNode);
        }

        // Case 2: Right-Right
        if (balanceFactor < -1 && car.id > currentNode->rightChild->carData.id) {
            return rotateLeft(currentNode);
        }

        // Case 3: Left-Right
        if (balanceFactor > 1 && car.id > currentNode->leftChild->carData.id) {
            currentNode->leftChild = rotateLeft(currentNode->leftChild);
            return rotateRight(currentNode);
        }

        // Case 4: Right-Left
        if (balanceFactor < -1 && car.id < currentNode->rightChild->carData.id) {
            currentNode->rightChild = rotateRight(currentNode->rightChild);
            return rotateLeft(currentNode);
        }

        return currentNode;
    }

    AVLNode* findMinimumValueNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->leftChild != nullptr) {
            current = current->leftChild;
        }
        return current;
    }

    AVLNode* removeHelper(AVLNode* currentNode, int id) {
        if (currentNode == nullptr) {
            return currentNode;
        }

        if (id < currentNode->carData.id) {
            currentNode->leftChild = removeHelper(currentNode->leftChild, id);
        }
        else if (id > currentNode->carData.id) {
            currentNode->rightChild = removeHelper(currentNode->rightChild, id);
        }
        else {

            if (currentNode->leftChild == nullptr || currentNode->rightChild == nullptr) {
                AVLNode* temporaryChild = currentNode->leftChild ? currentNode->leftChild : currentNode->rightChild;

                if (temporaryChild == nullptr) {
                    // Zero children case
                    temporaryChild = currentNode;
                    currentNode = nullptr;
                }
                else {
                    // One child case
                    *currentNode = *temporaryChild;
                }
                delete temporaryChild;
            }
            else {
                AVLNode* temporarySuccessor = findMinimumValueNode(currentNode->rightChild);

                currentNode->carData = temporarySuccessor->carData;

                currentNode->rightChild = removeHelper(currentNode->rightChild, temporarySuccessor->carData.id);
            }
        }

        if (currentNode == nullptr) {
            return currentNode;
        }

        updateNodeHeight(currentNode);

        int balanceFactor = getBalanceFactor(currentNode);

        // Case 1: Left-Left
        if (balanceFactor > 1 && getBalanceFactor(currentNode->leftChild) >= 0) {
            return rotateRight(currentNode);
        }

        // Case 2: Left-Right
        if (balanceFactor > 1 && getBalanceFactor(currentNode->leftChild) < 0) {
            currentNode->leftChild = rotateLeft(currentNode->leftChild);
            return rotateRight(currentNode);
        }

        // Case 3: Right-Right
        if (balanceFactor < -1 && getBalanceFactor(currentNode->rightChild) <= 0) {
            return rotateLeft(currentNode);
        }

        // Case 4: Right-Left
        if (balanceFactor < -1 && getBalanceFactor(currentNode->rightChild) > 0) {
            currentNode->rightChild = rotateRight(currentNode->rightChild);
            return rotateLeft(currentNode);
        }

        return currentNode;
    }

    void gatherInOrder(AVLNode* currentNode, std::vector<Car>& sortedCarsList) {
        if (currentNode == nullptr) {
            return;
        }
        gatherInOrder(currentNode->leftChild, sortedCarsList);       // Visit Left Subtree
        sortedCarsList.push_back(currentNode->carData);              // Visit Current Subtree
        gatherInOrder(currentNode->rightChild, sortedCarsList);      // Visit Right Subtree
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insert(Car car) {
        root = insertHelper(root, car);
    }

    void remove(int id) {
        root = removeHelper(root, id);
    }

    Car* search(int id) {
        AVLNode* currentElement = root;
        while (currentElement != nullptr) {
            if (id == currentElement->carData.id) {
                return &(currentElement->carData);
            }
            else if (id < currentElement->carData.id) {
                currentElement = currentElement->leftChild;
            }
            else {
                currentElement = currentElement->rightChild;
            }
        }
        return nullptr;
    }

    std::vector<Car> inOrder() {
        std::vector<Car> sortedCarsList;
        gatherInOrder(root, sortedCarsList);
        return sortedCarsList;
    }
};
