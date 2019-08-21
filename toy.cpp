//
//  toy.cpp
//  BST
//
//  Created by Teja  on 3/26/19.
//  Copyright © 2019 Teja . All rights reserved.
//
#include <stdio.h>
#include <iostream>
#include "bst.h"
#include <vector>

using namespace std;

int main(){
    int x = 0;
    int h;
    
    bst<int> *t = new bst<int>();
    
    // Taking input from user
    cout << "Enter some numbers: ";
    while (x != 999){
        cin >> x;
        if (x != 999){
            t->insert(x);
        }
        else{
            break;
        }
    }
    
    //**JACOB
//    t->childTest();
    
    //**ENDJACOB
    
    // displays the tree in order from vector
    cout << "Function: to_vector";
    cout << "Tree contents from vector in order: " << endl;
    t -> to_vector ();
    cout << endl;
    
//    // gets the ith element in the tree
//    cout << "get _ ith  smallest" << endl;
//    int ith;
//    cout << "Please enter which element you want from the tree: ";
//    cin >> ith;
//    cout << "Getting the ith element from the tree: " << endl;
//    int locationFound;
//    t -> get_ith(ith, locationFound);
//    cout << locationFound << endl;

//    // gets the location of an element in the tree
//    cout << "Position_of function" << endl;
//    int position;
//    cout << "Enter the element that you are searching for: ";
//    cin >> position;
//    cout << "Position of " << position << " is: ";
//    int y;
//    y = t -> position_of(position);
//    cout << y << endl;
//
    // gets all the values greater than or equal to the value provided
    cout << "Enter the element for which you want to see the greater elements: ";
    int greater;
    cin >> greater;
    cout << "No. of elements greater than or equal to " << greater << " is: ";
    greater = t -> num_geq (greater);
    cout << greater << endl;
//
//    // gets all the values less than or equal to the value provided
//    cout << "Enter the element for which you want to see the lesser elements: ";
//    int lesser;
//    cin >> lesser;
//    cout << "No. of elements less than or equal to " << lesser << " is ";
//    lesser = t -> num_leq(lesser);
//    cout << lesser << endl;
    
//    // gets all the elements in between the two range of numbers
//    cout << "Enter the minimum element number: ";
//    int minimum;
//    cin >> minimum;
//    cout << "Enter the maximum element number: ";
//    int maximum;
//    cin >> maximum;
//    int answer = t -> num_range (minimum,maximum);
//    cout << endl << endl;
//    cout << answer;
    
    // extracts the vector elements
//    cout << "Enter the minimum element number: ";
//    int minimum;
//    cin >> minimum;
//    cout << "Enter the maximum element number: ";
//    int maximum;
//    cin >> maximum;
//    t -> extract_range (minimum, maximum);
//    
//    if(t->size() <= 20) {
//        t->inorder();
//        t->preorder();
//    }
//    h = t->height();
//    std::cout << "\n#### Reported height of tree:   " << h << "\n";
//    
    //cout << t -> printInOrder(t) <<endl << endl;
    
    //    vector<int> *func1 = t->to_vector();
    //     cout<<"size"<<func1 -> size();
    //    for (int i = 0; i < func1 -> size(); i++)
    //    {
    //        cout<<"v";
    //        cout << func1->at(i) << " " ;
    //    }
    
    delete t;
    
    return 0;
}


