//============================================================================
// Name        : Project2.cpp
// Author      : Tyler Primas
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Program designed to load a data file from a csv file,
//               print a course list in alphanumeric order, and print a 
//               specific course and any prerequisites
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

//displays menu
void DisplayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

//defines a structure to hold course information
struct Course {
    string courseId;
    string courseName;
    vector<string> preRequisite;
};

//internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node() { //Default constructor
        left = nullptr;
        right = nullptr;
    }
    //initialize with a given course
    Node(Course aCourse) : Node() { 
        this->course = aCourse;
    }

};


//define a class containing data members asd
//methods to implemnt a binary search tree
class BinarySearchTree {

private:
   
    void Destruct(Node* node);

public:
    Node* root;
    BinarySearchTree(); 
    virtual ~BinarySearchTree();
    void Insert(BinarySearchTree* tree, Node* node);
    void Search(string courseId);
    void PrintCourse(Node* node);
};

//default constructor
BinarySearchTree::BinarySearchTree() { 
    root = nullptr;
};

//destructor
BinarySearchTree::~BinarySearchTree() {
    Destruct(root);
};

//destructs left and right pointer nodes and deletes them
void BinarySearchTree::Destruct(Node* node) {
    if (node != nullptr) {
        Destruct(node->left);
        node->left = nullptr;
        Destruct(node->right);
        node->right = nullptr;
        delete node;
    }
};


//search for a course
void BinarySearchTree::Search(string courseId) {
    Node* currentNode = root;

    while (currentNode != nullptr) { 
        if (currentNode->course.courseId == courseId) {	 
            //prints course id, name
            cout << currentNode->course.courseId << ", ";
            cout << currentNode->course.courseName;
            cout << endl;
            cout << "Prerequisites: ";
            //prints any prerequisites, followed by , if there are more than 1
            for (string preRequisite : currentNode->course.preRequisite) { 
                if (preRequisite == currentNode->course.preRequisite.back()) {
                    
                    cout << preRequisite << endl; 
                }
                else {
                    cout << preRequisite << ", "; 
                }
            }

            return;
        }
        //searches left pointer if not found
        else if (courseId < currentNode->course.courseId) { 

            if (currentNode->left != nullptr) {
                currentNode = currentNode->left;
            }
        }
        //searches right pointer if not found
        else { 

            currentNode = currentNode->right;
        }
    }
    //course is not in the list
    cout << "Course " << courseId << "not found. " << endl; 	
    return;
}

//inserts course into a course list
void BinarySearchTree::Insert(BinarySearchTree* tree, Node* node) {
    
    if (tree->root == nullptr) { 
        tree->root = node; 
    }
    else { 
        Node* curr = tree->root;
        while (curr != nullptr) { 

            if (node->course.courseId < curr->course.courseId) {
                if (curr->left == nullptr) {
                    curr->left = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->left;
                }
            }
            else { 

                if (curr->right == nullptr) {
                    curr->right = node;
                    curr = nullptr;
                }
                else {
                    curr = curr->right;
                }
            }

        }
       
    }
}
//prints course list
void BinarySearchTree::PrintCourse(Node* node) {

    //Traverse and print entire tree
    if (node == nullptr) {
        return;
    }

    //looks at left node, prints course id, course name, then look at next node
    PrintCourse(node->left); 
    cout << node->course.courseId << ", ";
    cout << node->course.courseName << endl; 
    PrintCourse(node->right); 
};

//loads file and creates course list
void loadCourse(string filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (file.is_open()) { 
        cout << "File loaded." << endl; 

        int num; 
        string line;
        string word;

        while (getline(file, line)) { 

            num = 0;
            Node* node = new Node();
            stringstream str(line);

            while (num < 2) {
                getline(str, word, ',');
                if (num == 0) {
                    node->course.courseId = word;
                }
                else {
                    node->course.courseName = word;
                }
                num++;
            }
            while (getline(str, word, ',')) {
                node->course.preRequisite.push_back(word);
            }

            //inserts node into bst
            bst->Insert(bst, node);
        }
    }
    //file error if file not found
    else {
        cout << "File error, please try again. " << endl;
        return;
    }

}

void main() {

    BinarySearchTree* bst = new BinarySearchTree();

    string fileChoice;
    string courseChoice;

    int userInput = 0;

    cout << "Welcome to the course planner." << endl << endl;
    //main while loop to keep program open while choice is not 9
    while (userInput != 9) { 
        DisplayMenu();
        cin >> userInput;

        switch (userInput) {
        //loads file that user chooses
        case 1: 
            cout << endl;
            cout << "What is the name of the file you would like to load?  Default file is Courses.csv ";
            cin >> fileChoice;
            
            loadCourse(fileChoice, bst);
            cout << endl;
            break;

        //prints courses in alphanumeric order
        case 2:
            cout << endl;
            cout << "******************************************" << endl;
            cout << "COURSE ID |     COURSE NAME" << endl;
            cout << "******************************************" << endl;
            //prints course starting at root   	
            bst->PrintCourse(bst->root); 
            cout << endl;
            break;
            
        //prints course that user chooses and any prerequisites
        case 3:
            cout << endl;
            cout << "What course do you want to know about? ";
            cin >> courseChoice;
            cout << endl;

            std::transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);
            bst->Search(courseChoice);
            
            cout << endl;
            break;

        //exits the program
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        //default message if one of the above choices is not entered
        default:
            cout << userInput << " is not a valid option." << endl << endl;
            break;
        }
    }
}