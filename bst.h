//
//  main.cpp
//  BST
//
//  Created by Teja  on 3/26/19.
//  Copyright © 2019 Teja . All rights reserved.
//

#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// NECESSARY FUNCTION PROTOTYPING
//static void printInOrder (struct bst_node*); // my add
//int search(bst_node *root, int key, bst_node* parent); // my add

template <typename T>
class bst {
    
private:
    struct bst_node {
        T      val;
        bst_node *left;
        bst_node *right;
        int lCount; // my add
        int rCount; // my add
        
        bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr, int lCount = 0, int rCount = 0)
        : val { _val },  left { l }, right {r}, lCount{lCount}, rCount{rCount}
        { }
    };
    
public:
    // constructor:  initializes an empty tree
    bst(){
        root = nullptr;
    }
    
private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(bst_node *r){
        if(r==nullptr) return;
        delete_nodes(r->left);
        delete_nodes(r->right);
        delete r;
    }
    
public:
    // destructor
    ~bst() {
        delete_nodes(root);
    }
    
    //    static void printInOrder(bst_node * root)
    //    {
    //        if (root == NULL)
    //            return;
    //
    //        /* first recur on left child */
    //        printInOrder(root -> left);
    //
    //        /* then print the data of node */
    //        cout << root -> val << " ";
    //
    //        /* now recur on right child */
    //        printInOrder(root -> right);
    //    }
    
    // rebalance tree
    static bst_node* rebalance(bst_node* root )
    {
        // Store nodes of given BST in sorted order
        vector<bst_node*> nodes;
        storeBSTNodes(root, nodes);
        
        // Constucts BST from nodes[]
        int n = int(nodes.size());
        
        bool s = false;
        return buildTree(nodes, 0, n-1, s);
    }
    
    /* Recursive function to construct binary tree */
    static bst_node* buildTree(vector<bst_node*> &nodes, int start,
                               int end, bool& success)
    {
        success = false;
        // base case
        if (start > end)
            return NULL;
        
        /* Get the middle element and make it root */
        int mid = (start + end)/2;
        bst_node *root = nodes.at(mid);
        success = true;
        
        /* Using index in Inorder traversal, construct
         left and right subtress */
        bool lsuccess= false;
        root->left = buildTree(nodes, start, mid-1, lsuccess);
        
        if(lsuccess)
            root->lCount++;
        
        bool rsuccess = false;
        root->right = buildTree(nodes, mid+1, end, rsuccess);
        
        if(rsuccess)
            root->rCount++;
        
        return root;
    }
    
    /* This function traverse the skewed binary tree and
     stores its nodes pointers in vector nodes[] */
    static void storeBSTNodes(bst_node* root, vector<bst_node*> &nodes)
    {
        // Base case
        if (root==NULL)
            return;
        
        storeBSTNodes(root->left, nodes);
        nodes.push_back(root);
        storeBSTNodes(root->right, nodes);
    }
private:
    
    /**
     * function:  insert()
     * desc:      recursive helper function inserting x into
     *            binary search tree rooted  at r.
     *
     * returns:   pointer to root of tree after insertion.
     *
     * notes:     if x is already in tree, no modifications are made.
     */
    static bst_node * _insert(bst_node *r, T & x, bool &success){
        if(r == nullptr){
            success = true;
            return new bst_node(x, nullptr, nullptr);
        }
        if(r->val == x){
            success = false;
            return r;
        }
        if(x < r->val){
            r->left = _insert(r->left, x, success);
            
            if (success){
                r -> lCount++;
                
                // Check this subtree is bal/un bal - max(nL, nR) <= 2min(nL, nR) + 1
                //                if(std::max(r->left->lCount, r->left->rCount) > 2 * std::min(r->left->lCount, r->left->rCount)+1)
                //                {
                //                    // Rebalance tree
                //                    r->left = rebalance(r->left);
                //                }
                if(std::max(r->lCount, r->rCount) > 2 * std::min(r->lCount, r->rCount)+1)
                {
                    // Rebalance tree
                    r= rebalance(r);
                }
                
            }
            return r;
        }
        else {
            r->right = _insert(r->right, x, success);
            
            if (success)
            {
                r -> rCount++; // my add
                // Check this subtree is bal/un bal - max(nL, nR) <= 2min(nL, nR) + 1
                //                if(std::max(r->right->lCount, r->right->rCount) > 2 * std::min(r->right->lCount, r->right->rCount)+1)
                //                {
                //                    // Rebalance tree
                //                    r->right  = rebalance(r->right);
                //                }
                if(std::max(r->lCount, r->rCount) > 2 * std::min(r->lCount, r->rCount)+1)
                {
                    // Rebalance tree
                    r= rebalance(r);
                }
            }
            return r;
        }
    }
    
public:
    /**
     * function:  insert
     * desc:      inserts x into BST given by t.  Note that
     *            a BST stores a SET -- no duplicates.  Thus,
     *            if x is already in t when call made, no
     *            modifications to tree result.
     *
     * note:      helper function does most of the work.
     *
     */
    bool insert(T & x){
        bool success;
        root = _insert(root, x, success);
        return success;
    }
    
    /**
     * function:  contains()
     * desc:      returns true or false depending on whether x is an
     *            element of BST (calling object)
     *
     */
    bool contains(const T & x){
        bst_node *p = root;
        
        while(p != nullptr){
            
            if(p->val == x)
                return true;
            if(x < p->val){
                p = p->left;
            }
            else
                p = p->right;
        }
        return false;
    }
    
private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _min_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->left != nullptr)
            r = r->left;
        return r;
    }
    
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static bst_node * _max_node(bst_node *r ){
        if(r==nullptr)
            return nullptr; // should never happen!
        while(r->right != nullptr)
            r = r->right;
        return r;
    }
    
    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static bst_node * _remove(bst_node *r, T & x, bool &success){
        bst_node *tmp;
        
        if(r==nullptr){
            success = false;
            return nullptr;
        }
        if(r->val == x){
            success = true;
            
            if(r->left == nullptr){
                tmp = r->right;
                delete r;
                return tmp;
            }
            if(r->right == nullptr){
                tmp = r->left;
                delete r;
                return tmp;
            }
            // if we get here, r has two children
            r->val = _min_node(r->right)->val;
            r->rCount--;
            success = false;
            r->right = _remove(r->right, r->val, success);
            
            return r;
        }
        if(x < r->val){
            r->left = _remove(r->left, x, success);
            if(success)
                r -> lCount--; // my add - decrementing left subtree node after deletion
        }
        else {
            r->right = _remove(r->right, x, success);
            if(success)
                r -> rCount--; // my add - decrementing the right subtree node after deletion
        }
        return r;
    }
    
    
public:
    
    bool remove(T & x){
        bool success;
        root = _remove(root, x, success);
        return success;
    }
    
    
private:
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(bst_node *r){
        if(r==nullptr) return 0;
        return _size(r->left) + _size(r->right) + 1;
    }
    
public:
    int size() {
        return _size(root);
    }
    
private:
    
    static int _height(bst_node *r){
        int l_h, r_h;
        
        if(r==nullptr) return -1;
        l_h = _height(r->left);
        r_h = _height(r->right);
        return 1 + (l_h > r_h ? l_h : r_h);
    }
    
public:
    
    int height() {
        return _height(root);
    }
    
    bool min(T & answer) {
        if(root == nullptr){
            return false;
        }
        answer = _min_node(root)->val;
        return true;
    }
    
    T max() {
        return _max_node(root)->val;
    }
    
    
    // Can't figure out how to test the functions from toy.cpp
    
    /******************************************
     *
     * "stubs" for assigned TODO functions below
     *
     *****************************************/
    
    // ** MY FUNCTION FOR VECTOR (HELPER)
    void addVectorElements(bst_node * traversal, vector <T> *elements)
    {
        // int i = 0;
        
        if (traversal == NULL)
            return;
        
        /* first recur on left child */
        addVectorElements(traversal->left, elements);
        
        /* then adds the data of node to vector */
        elements->push_back (traversal -> val);
        
        /* now recur on right child */
        addVectorElements(traversal->right, elements);
    }
    
    /* TODO
     * Function:  to_vector
     * Description:  allocates a vector of type T and populates
     *               it with the elements of the tree in sorted
     *               order.  A pointer to the vector is returned.
     *
     * runtime:  O(n) where n is the tree size.
     *
     */
    
    // demo values: 22,11,88,19,17,33,88,111,91,26
    // actual function given to me
    std::vector<T> * to_vector()
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        // created the vector to store elements
        vector <T>* elements = new vector<T>();
        bst_node *traversal = root;
        // makes call to my helper function
        addVectorElements (traversal, elements);
        
        return elements;
        
    }
    
    /* TODO
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.
     *    i ranges from 1..n where n is the number of elements in the
     *    tree.
     *
     *    If i is outside this range, false is returned.
     *    Otherwise, true is returned (indicating "success").
     *
     * Runtime:  O(h) where h is the tree height
     */
    
    int k_smallest_element(bst_node *root, int k)
    {
        int ret = -1;
        
        if( root )
        {
            /* A crawling pointer */
            bst_node *pTraverse = root;
            
            /* Go to k-th smallest */
            while(pTraverse)
            {
                if( (pTraverse->lCount + 1) == k )
                {
                    ret = pTraverse->val;
                    break;
                }
                else if( k > pTraverse->lCount )
                {
                    /*  There are less nodes on left subtree
                     Go to right subtree */
                    k = k - (pTraverse->lCount + 1);
                    pTraverse = pTraverse->right;
                }
                else
                {
                    /* The node is on left subtree */
                    pTraverse = pTraverse->left;
                }
            }
        }
        
        return ret;
    }
    
    bool get_ith(int i, T &x) {
        int y = k_smallest_element (root, i);
        
        if (y == -1)
        {
            return false;   // placeholder
        }
        else
        {
            x = y;
            return true; // means the value is found in the tree at ith location
        }
    }
    
    /*
     * get_ith_SLOW:  slow version of get_ith which gives the right answer, but
     *   does so by "walking the entire tree".
     * might be helpful for debugging?
     * Private helper below does most of the work
     */
    bool get_ith_SLOW(int i, T &x) {
        int n = size();
        int sofar=0;
        
        if(i < 1 || i > n)
            return false;
        
        _get_ith_SLOW(root, i, x, sofar);
        return true;
    }
    
private:
    // recursive helper function that does most of the work
    static void _get_ith_SLOW(bst_node *t, int i, T &x, int &sofar) {
        if(t==nullptr)
            return;
        _get_ith_SLOW(t->left, i, x, sofar);
        
        if(sofar==i)
            return;
        sofar++;
        if(sofar==i) {
            x = t->val;
            return;
        }
        _get_ith_SLOW(t->right, i, x, sofar);
    }
    
public:
    
    /* TODO
     * Function: position_of
     * Description:  this is like the inverse of
     *       get_ith:  given a value x, determine the
     *       position ("i") where x would appear in a
     *       sorted list of the elements and return
     *       the position as an integer.
     *       If x is not in the tree, -1 is returned.
     *       Examples:
     *           if x happens to be the minimum, 1 is returned
     *           if x happens to be the maximum, n is returned where
     *               n is the tree size.
     *
     *       Notice the following property:  for a bst t with n nodes,
     *           pick an integer pos:1 <= pos <= n.
     *           Now consider the following:
     *
     T x;
     int pos, pos2;
     // set pos to a value in {1..n}
     t.get_ith(pos, x);  // must return true since pos is in {1..n}
     
     // now let's find the position of x (just retrieved)
     pos2 = t.position_of(x);
     if(pos != pos2) {
     std::cout << "THERE MUST BE A BUG!\n";
     }
     
     See how position_of performs the inverse operation of get_ith?
     *
     * Return:  -1 if x is not in the tree; otherwise, returns the position where x
     *          would appear in the sorted sequence of the elements of the tree (a
     *          value in {1..n}
     *
     * Runtime:  O(h) where h is the tree height
     */
    
    // HELPER FUNCTION
    //    int search (bst_node * root, T x, int& count)
    //    {
    //        if (root == NULL)
    //            return count;
    //        // if root node is equal to the value
    //        if (x == root -> val)
    //        {
    //            return count;
    //        }
    //        // first recur on left child
    //        else if (x < root -> val)
    //        {
    //            if (root -> right != NULL && root -> left != NULL)
    //            {
    //                count = count - root -> left -> rCount - 1;
    //                search (root -> left, x, count);
    //            }
    //        }
    //
    //        /* now recur on right child */
    //        else if (x > root -> val)
    //        {
    //            if (root -> right != NULL && root -> left != NULL)
    //
    //            count = count + root -> right -> lCount + 1;
    //            search (root -> right, x, count);
    //        }
    //        return count;
    //    }
    
    // function of bool type whereas we were trying with int previously
    bool search (bst_node * root, T x, int & count, bool & found)
    {
        // if the tree is empty, then return false
        if (root == NULL)
            return found;
        // the following condition executes if one of the two conditions is true:
        // if root node is equal to the value
        // or
        // if we found the value we are searching for
        if (x == root -> val)
        {
            found = true;
            return found;
        }
        // first recur on left child
        else if (x < root -> val)
        {
            if ( root -> left != NULL)
            {
                count = count - root -> left -> rCount - 1;
                search (root -> left, x, count, found);
            }
        }
        /* now recur on right child */
        else if (x > root -> val)
        {
            if (root -> right != NULL)
            {
                count = count + root -> right -> lCount + 1;
                search (root -> right, x, count, found);
            }
        }
        return found;
    }
    
    // given function call;
    // demo values: 22,11,88,19,17,33,88,111,91,26
    int position_of(const T & x)
    {
        int y;
        bool found = false;
        int count = root -> lCount + 1;
        found = search (root, x, count, found);
        //cout << "bool Value: " << found << endl;
        //cout << "count Value: " << count << endl;
        // based on bool value, we decide our return value
        if (found == 0)
        {
            y= -1;
        }
        else
        {
            y = count;
        }
        return y;
    }
    
    /* TODO
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */
    
    // HELPER FUNCTION
    //    void length (bst_node* root, T x, int& counter, bool& found, int& min ){
    //        // means tree is null or the has run out of nodes
    //        if(root == NULL)
    //        {
    //
    //            return;
    //        }
    //        // if the given input matches one of the element
    //        if(x == root->val)
    //        {
    //            found = true;
    //            counter += root -> rCount + 1;
    //            return;
    //        }
    //        // if the given input is greater than the current node value
    //        if(x > root -> val)
    //        {
    //            //counter += root -> rCount;
    //            min = root -> val;
    //            length (root->right, x, counter, found, min);
    //        }
    //        // if the given input is less than the current node value
    //        if(x < root->val)
    //        {
    //
    //            if(root -> left == nullptr)
    //            {
    //                if (min < root -> val)
    //                    min = root -> val;
    //                counter += root->rCount + 1;
    //                return;
    //            }
    //            else
    //            {
    //                min = root -> val;
    //                counter += root->rCount;
    //                length (root->left, x, counter, found, min);
    //            }
    //        }
    //        // returns the counter value
    //        //return counter;
    //    }
    int length(bst_node* root, T x, int& counter, int& isEquals, bool& exist){
        if(root == NULL){ // reached the end of a subtree
            return counter;
        }
        else if(x == root->val){
            isEquals++;
            exist = true;
            return counter += root->rCount+1; // did +1
        }
        else if(x > root->val){
            length(root->right, x, counter, isEquals, exist);
        }
        else if(x < root->val){
            counter += root->rCount + 1;
            length(root->left, x, counter, isEquals, exist);
        }
        return counter;
    }
    
    
    // given function call
    // demo values: 22,11,88,19,17,33,88,111,91,26
    int num_geq(const T & x)
    {
        int counter = 0; // to keep count of number of elements greater than or equal to given element
        //        bool found = false;
        //        int min = root -> val;
        int isEquals = 0;
        bool exist = false;
        int IFTHISWORKS_NOT_A_CONST = length (root, x, counter, isEquals, exist);
        return IFTHISWORKS_NOT_A_CONST;
        //return counter;
    }
    
    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *                (private helper does most of the work)
     *
     */
    int num_geq_SLOW(const T & x) {
        return _num_geq_SLOW(root, x);
    }
    
private:
    static int _num_geq_SLOW(bst_node * t, const T & x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);
        
        if(t->val >= x)
            total++;
        return total;
    }
    
public:
    
    /* TODO
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    
    // HELPER FUNCTION
    int lessThan(bst_node* root, T x, int& counter){
        if(root == NULL){ // reached the end of a subtree
            return counter;
        }
        else if(x == root->val){
            return counter += root->lCount + 1;
        }
        else if(x > root->val){
            counter += root->lCount + 1;
            lessThan(root -> right, x, counter);
        }
        else if( x < root -> val){
            lessThan (root -> left, x, counter);
        }
        return counter;
    }
    
    // demo values: 22,11,88,19,17,33,88,111,91,26
    // actual function given
    int num_leq(const T &x) {
        int counter = 0; // to keep count of number of elements greater than or equal to given element
        int y = lessThan (root, x, counter);
        return y;
    }
    
    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *               (private helper _num_leq_SLOW does most of the work)
     */
    int num_leq_SLOW(const T & x) {
        return _num_leq_SLOW(root, x);
    }
private:
    
    // helper for num_leq_SLOW
    static int _num_leq_SLOW(bst_node *t, const T &x) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);
        
        if(t->val <= x)
            total++;
        return total;
    }
    
public:
    
    /* TODO
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    
    // HELPER FUNCTION
    
    int lessThan1(bst_node* root, T x, int& counter, bool& found){
        if(root == NULL){ // reached the end of a subtree
            return counter;
        }
        else if(x == root->val){
            found = true;
            return counter += root->lCount + 1;
        }
        else if(x > root->val){
            counter += root->lCount + 1;
            lessThan1(root -> right, x, counter, found);
        }
        else if( x < root -> val){
            lessThan1 (root -> left, x, counter, found);
        }
        return counter;
    }
    
    // given function call
    // demo values: 22,11,88,19,17,33,88,111,91,26
    int num_range(const T & min, const T & max) {
        
        int isEquals = 0;
        int countGeqMin = 0;
        bool minExists = false,maxExists=false;
        length (root, min, countGeqMin, isEquals, minExists);
        int countGeqMax = 0;
        length (root, max, countGeqMax, isEquals, maxExists);
        if(min==max) // if min and max are same
        {
            if(isEquals) return 1;
            return 0;
        }
        if(isEquals == 1) // if atleast min/max exist in tree
        {
            if(countGeqMin - countGeqMax == 1 && minExists) // 26 and 32
                return 1;
            if(maxExists)
                return countGeqMin - countGeqMax + 1;
        }
        else if(isEquals == 2) // both min max exist in tree
        {
            return countGeqMin - countGeqMax + 1;
        }
        else // if both min and max doesn't exist in tree
            return countGeqMin - countGeqMax;
        
        return countGeqMin - countGeqMax;
        
    }
    // 1 2 3 4 5 6 7 8 =min 2 max 5 - 5-3+1 = 3
    /*
     * function:     num_range_SLOW
     * description:  same functionality as num_range but sloooow (linear time)
     *               (private helper _num_range_SLOW does most of the work)
     *
     */
    int num_range_SLOW(const T & min, const T & max) {
        return _num_range_SLOW(root, min, max);
    }
    
private:
    static int _num_range_SLOW(bst_node *t, const T &min, const T &max) {
        int total;
        
        if(t==nullptr) return 0;
        total =_num_range_SLOW(t->left, min, max) +
        _num_range_SLOW(t->right, min, max);
        
        if(t->val >= min && t->val <= max)
            total++;
        return total;
    }
    
    int getRange(bst_node *root, int low, int high, vector<T>* range)
    {
        // Base case
        if (!root) return 0;
        
        // Special Optional case for improving efficiency
        if (root->val == high && root->val == low)
        {
            range->push_back(root->val);
            return 1;
        }
        
        // If current node is in range, then include it in count and
        // recur for left and right children of it
        if (root->val <= high && root->val >= low)
        {
            int left = getRange(root->left, low, high, range);
            range->push_back(root->val);
            int right = getRange(root->right, low, high, range);
            return 1 + left + right;
        }
        // If current node is not smaller than low
        else if (!(root->val < low)) return getRange(root->left, low, high, range);
        // If current node is smaller than low, then recur for right
        // child
        else
            return getRange(root->right, low, high, range);
    }
public:
    
    /*
     * TODO
     * function:     extract_range
     * Description:  allocates a vector of element type T
     *               and populates it with the tree elements
     *               between min and max (inclusive) in order.
     *               A pointer to the allocated and populated
     *               is returned.
     *
     * notes/comments:  even if the specified range is empty, a
     *                  vector is still allocated and returned;
     *                  that vector just happens to be empty.
     *                  (The function NEVER returns nullptr).
     *
     * runtime:  the runtime requirement is "output dependent".
     *           Let k be the number of elements in the specified range
     *           (and so the length of the resulting vector) and let h
     *           be the height of the tree.  The runtime must be:
     *
     *                  O(h + k)
     *
     *           So...while k can be as large as n, it can be as small
     *           as zero.
     *
     */
    std::vector<T> * extract_range(const T & min, const T & max) {
        vector <T> *range = new vector<T>();
        //inorder tree
        getRange(root, min, max, range);
        
        return range;
    }
    
    
    
    /***************************************************
     *  PREORDER AND INORDER TREE TRAVERSALS BELOW      *
     *                                                  *
     ***************************************************/
    
private:
    static void indent(int m){
        int i;
        for(i=0; i<m; i++)
            std::cout << "-";
    }
    
    static void _inorder(bst_node *r){
        if(r==nullptr) return;
        _inorder(r->left);
        std::cout << "[ " << r->val << " ]\n";
        _inorder(r->right);
    }
    
    static void _preorder(bst_node *r, int margin){
        if(r==nullptr) {
            indent(margin);
            std::cout << " nullptr \n";
        }
        else {
            indent(margin);
            std::cout << "[ " << r->val << " ]\n";
            _preorder(r->left, margin+3);
            _preorder(r->right, margin+3);
        }
    }
    
public:
    void inorder() {
        std::cout << "\n======== BEGIN INORDER ============\n";
        _inorder(root);
        std::cout << "\n========  END INORDER  ============\n";
    }
    
    
    void preorder() {
        
        std::cout << "\n======== BEGIN PREORDER ============\n";
        _preorder(root, 0);
        std::cout << "\n========  END PREORDER  ============\n";
        
    }
    
    /***************************************************
     *  END PREORDER AND INORDER TREE TRAVERSALS        *
     *                                                  *
     ***************************************************/
    
private:
    /*
     * Recursive  helper function _from_vec, used by
     * from_sorted_arr(...). The function must return a sub-tree that is
     * perfectly balanced, given a sorted array of elements a.
     */
    static bst_node * _from_vec(const std::vector<T> &a, int low, int hi){
        int m;
        bst_node *root;
        
        if(hi < low) return nullptr;
        m = (low+hi)/2;
        root = new bst_node(a[m]);
        root->left  = _from_vec(a, low, m-1);
        root->right = _from_vec(a, m+1, hi);
        return root;
        
    }
    
public:
    static bst * from_sorted_vec(const std::vector<T> &a, int n){
        
        bst * t = new bst();
        t->root = _from_vec(a, 0, n-1);
        return t;
    }
    
private:
    
    /*
     * DO NOT MODIFY THE TWO FUNCTIONS BELOW!!!
     */
    static void _to_preorder_vec(bst_node *t, std::vector<T> &vec) {
        if(t==nullptr) return;
        vec.push_back(t->val);
        _to_preorder_vec(t->left, vec);
        _to_preorder_vec(t->right, vec);
    }
    std::vector<T> * to_preorder_vec() {
        std::vector<T> *vec = new std::vector<T>();
        _to_preorder_vec(root, *vec);
        return vec;
    }
    
    /*
     * Finally, the data members of the bst class
     */
private:
    bst_node *root;
    
public:
    bst_node* getRoot()
    {
        return root;
    }
    
}; // end class bst


#endif
