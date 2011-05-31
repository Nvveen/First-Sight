// ============================================================================
// 
//       Filename:  Octree.h
// 
//    Description:  This class implements a templated octree, for easy traversal
//                  of, for example, voxel data.
// 
//        Version:  1.0
//        Created:  05/26/2011 01:37:10 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#ifndef  OCTREE_H
#define  OCTREE_H

#include    <cmath>
#include    <queue>
#include    <vector>


// ============================================================================
//        Class:  Octree
//  Description:  An ADT containing an octree.
// ============================================================================
template <class T>
class Octree
{
    public:
        typedef unsigned int Uint;
        typedef std::queue<Uint> IndexList;

        struct Node {
            Uint location[3];
            Node *children[8];
            Node *neighbors[6];
            Node *parent;
            T value;
        };

        enum Sides {
            CS_Front,
            CS_Left,
            CS_Bottom,
            CS_Back,
            CS_Right,
            CS_Top
        };
        // ====================  LIFECYCLE     ================================
        Octree ( Uint size );                           // constructor
        ~Octree ();
        Node *createNewNode ( Node *parent );
        void print ( Node *node );
        void print ();
        void destructNodes ( Node *node );

        // ====================  ACCESSORS     ================================
        IndexList getNodeIndices ( int x, int y, int z );
        bool isLeaf ( Node *node );
        Node *getNode ( int x, int y, int z );
        std::vector<T> getNeighbors ( Uint x, Uint y, Uint z );

        // ====================  MUTATORS      ================================
        void insert ( Uint x, Uint y, Uint z, T value );
        void remove ( Uint x, Uint y, Uint z );
        void setNeighbors ( Node *node );

        // ====================  OPERATORS     ================================
        T operator() ( Uint x, Uint y, Uint z );

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        Node *rootNode_;
        Uint maxDepth_;
}; // -----  end of template class Octree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  Octree
// Description:  constructor
//-----------------------------------------------------------------------------
    template < class T >
Octree<T>::Octree ( Uint size ) :
    maxDepth_(size)
{
    // Create the root node
    rootNode_ = createNewNode(NULL);
}  // -----  end of constructor of template class Octree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  Octree
// Description:  constructor
//-----------------------------------------------------------------------------
    template < class T >
Octree<T>::~Octree ()
{
    // Clean up everything.
    destructNodes(rootNode_);
}  // -----  end of constructor of template class Octree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  createNewNode
// Description:  To dynamically create new nodes, we need a function that takes
//               a parent as parameter and makes a new Node and sets its correct
//               values afterwards.
//-----------------------------------------------------------------------------
    template < class T >
typename Octree<T>::Node *Octree<T>::createNewNode ( Node *parent )
{
    Node *node = new Node;
    // Set parent
    node->parent = parent;
    // Set all children to NULL
    for ( int i = 0; i < 8; i += 1 ) node->children[i] = NULL;
    for ( int i = 0; i < 6; i += 1 ) node->neighbors[i] = NULL;
    return node;
}		// -----  end of method Octree<T>::createNewNode  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  insert
// Description:  Insert takes an object's location in an octree, gets an index
//               defined by its position, iterates through it by travelling
//               through the octree, and when the correct parent node is found,
//               that child on the last index from the index list is created.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::insert ( Uint x, Uint y, Uint z, T value )
{
    // Get the location's index list.
    IndexList indices = getNodeIndices(x, y, z);
    if ( indices.empty() ) {
        std::cerr << "Invalid Octree-location specified.\n";
        exit(1);
    }
    // Begin at the root.
    Node *node = rootNode_;
    // We need a pointer to take care of the next iteration.
    Node *next = NULL;
    // While the index list is not empty, we need to pop the front value, which
    // corresponds to the next index in the array of children of the current
    // node. If each child of the pointers in each iteration is still empty, we
    // create them on the fly, so we eventually arrive at the correct node and
    // depth.
    while ( !indices.empty() ) {
        // Get the front item from the list, the current index.
        Uint index = indices.front();
        // The next step is the child of the current pointer at the index.
        next = node->children[index];
        // The next pointer doesn't exist yet, but we're not yet at the correct
        // depth (the indexlist isn't empty yet), so we create a new node.
        if ( next == NULL ) {
            next = createNewNode(node);
            // Don't forget to set the parent's child pointer to the current
            // node.
            node->children[index] = next;
        }
        // We switch over to the next level.
        node = next;
        // We remove the current index from the list.
        indices.pop();
    }
    // We have arrived at the correct node (the index list is empty), so we can
    // set the value of the node.
    next->value = value;
    next->location[0] = x;
    next->location[1] = y;
    next->location[2] = z;
    setNeighbors(next);
}		// -----  end of method Octree<T>::insert  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  remove
// Description:  To remove a node from the octree, we have to find its location
//               first (by its index list), after which we can delete him and
//               set the remaining pointer correctly.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::remove ( Uint x, Uint y, Uint z )
{
    Uint index = getNodeIndices(x, y, z).back();
    Node *node = getNode(x, y, z);
    if ( node != NULL ) {
        Node *parent = node->parent;
        for ( int i = 0; i < 6; i += 1 )
            if ( node->neighbors[i] != NULL )
                node->neighbors[i]->neighbors[5-i] = NULL;
        delete parent->children[index];
        parent->children[index] = NULL;
    }
}		// -----  end of method Octree<T>::remove  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  getNodeIndices
// Description:  This method gets a location of an object in the Octree, and
//               depending on where in a cube of 8 subcubes it is located, it
//               returns the correct index list.
//-----------------------------------------------------------------------------
    template < class T >
typename Octree<T>::IndexList Octree<T>::getNodeIndices ( int x, int y, 
                                                          int z )
{
    // For every level, we need an index, so we use a list of indices.
    IndexList indices;
    if ( !(x >= 0 && x < pow(2, maxDepth_)) ||
         !(y >= 0 && y < pow(2, maxDepth_)) ||
         !(z >= 0 && z < pow(2, maxDepth_)) ) {
        return indices;
    }
    // The size of the current cube begins at the size of the entire object,
    // otherwise defined as 2 to the power of the maximum depth.
    int cubeSize = pow(2, maxDepth_);
    // The split axis are all half of the current cube size.
    int split = cubeSize/2;
    // We're going to iterate through "levels" of cube sizes, that are getting
    // halved at each iteration. We need to do this because each node has an
    // index at each level, like a resolution. The smaller the cube sizes, the
    // deeper into the Octree. We begin with a normal cube like:
    //        7___6
    //       /   /|
    //      3---2 5
    //      |   |/ 
    //      0---1
    //
    // into
    //     7_____6
    //    /_ /_ /|
    //   /  /  /|/
    //  3-----2 /|
    //  |  |  |/|5
    //  |--|--| /
    //  |  |  |/
    //  0-----1
    //
    // which means each cube gets divided into 8 seperate subcubes, and each
    // split axis is the half of the new cubes.
    // We get the index by checking to see if the x, y and z components of the
    // coordinates are bigger or smaller than the split axis.
    while ( cubeSize >= 2 ) {
        int index = 0;
        // This piece of code is a smaller way of switching between 0 to 7
        // different indices depending on the coordinates.
        if ( x % cubeSize >= split ) index |= 1;
        if ( y % cubeSize >= split ) index |= 2;
        if ( z % cubeSize >= split ) index |= 4;
        // We have found the coordinates, so we push it to the cube.
        indices.push(index);
        // For the next iteration, the cubesize and split axis must shrink by
        // half.
        cubeSize /= 2;
        split /= 2;
    }
    return indices;
}		// -----  end of method Octree<T>::getNodeIndices  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  print
// Description:  A debugging method to print all the values in the current
//               octree by recursively going through the nodes and printing the
//               values if the current node is a leaf.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::print ( Node *node )
{
    Node *helper = node;
    if ( helper != NULL ) {
        if ( !isLeaf(helper) )
            for ( int i = 0; i < 8; i += 1 )
                print(helper->children[i]);
        else {
            std::cout << helper->value << " ";
            if ( helper == helper->parent->children[7] )
                std::cout << "\n";
        }
    }
}		// -----  end of method Octree<T>::print  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  print
// Description:  This method is a wrapper method for the main print method were
//               the beginning default value is the root node.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::print ()
{
    print(rootNode_);
}		// -----  end of method Octree<T>::print  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  isLeaf
// Description:  A leaf is defined as a node which has no children pointers.
//               This method iterates through the children, and if even one is a
//               non-NULL pointer, the current node is not a leaf.
//-----------------------------------------------------------------------------
    template < class T >
bool Octree<T>::isLeaf ( Node *node )
{
    if ( node != NULL ) {
        for ( int i = 0; i < 8; i += 1 )
            if ( node->children[i] != NULL )
                return false;
        return true;
    }
    return false;
}		// -----  end of method Octree<T>::isLeaf  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  destructNodes
// Description:  This method recursively iterates through every node, by going
//               to all its children (and their children, etc.), deleting that
//               node, resetting the pointers pointing to the node and exiting
//               the method.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::destructNodes ( Node *node )
{
    if ( node != NULL ) {
        // The current node exists, so nowe we check if we're at the bottom.
        if ( !isLeaf(node) ) {
            // We're not at the bottom, so we iterate all the way to make sure
            // we delete children first.
            for ( int i = 0; i < 8; i += 1 )
                destructNodes(node->children[i]);
        }
        // If the current node is a child (or the current node was a parent but
        // its children have recently been deleted, we reset all its children
        // pointers to point to nothing, and set its own parent pointer pointing
        // to himself to NULL.
        if ( isLeaf(node) ) {
            for ( int i = 0; i < 8; i += 1 ) {
                node->children[i] = NULL;
                if ( node->parent != NULL && 
                     node->parent->children[i] == node )
                    node->parent->children[i] = NULL;
            }
            for ( int i = 0; i < 6; i += 1 )
                if ( node->neighbors[i] != NULL )
                    node->neighbors[i]->neighbors[5-i] = NULL;
            // Now nothing points to the current node, so we can delete it.
            node->parent = NULL;
            delete node;
            // For good measure, we reset the current node too.
            node = NULL;
        }
    }
}		// -----  end of method Octree<T>::destructNodes  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  operator()
// Description:  Return the value of the node specified by its location.
//-----------------------------------------------------------------------------
    template < class T >
T Octree<T>::operator() ( Uint x, Uint y, Uint z )
{
    Node *node = getNode(x, y, z);
    return node->value;
}		// -----  end of method Octree<T>::operator()  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  getNode
// Description:  Return the node based on its location.
//-----------------------------------------------------------------------------
    template < class T >
typename Octree<T>::Node *Octree<T>::getNode ( int x, int y, int z )
{
    // Get the index list for the node.
    IndexList indices = getNodeIndices(x, y, z);
    // Start at the root
    Node *node = rootNode_;
    // If the index list is empty, no node is found, so return NULL.
    if ( indices.empty() ) {
        return NULL;
    }
    // Walk through the octree via the node's index list.
    while ( !indices.empty() ) {
        Uint index = indices.front();
        // Return NULL on faulty index list/octree.
        if ( node == NULL ) {
            return NULL;
        }
        // Set next node.
        node = node->children[index];
        // Remove index from list.
        indices.pop();
    }
    // Node has been found.
    return node;
}		// -----  end of method Octree<T>::getNode  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  setNeighbors
// Description:  Set the neighbors for each node.
//-----------------------------------------------------------------------------
template < class T >
void Octree<T>::setNeighbors ( Node *node )
{
    // Every leaf has its xyz position stored in a location array.
    int x = node->location[0];
    int y = node->location[1];
    int z = node->location[2];
    if ( node != NULL && isLeaf(node) ) {
        // Find the neighboring nodes for the front, left, bottom, back, right
        // and top respectively, set the pointer to those neighbors, and set
        // neighbor pointer of that neighbor to the current node, where the
        // node's position is mirrored.
        Node *neighbor = getNode(x, y, z-1);
        node->neighbors[CS_Front] = neighbor;
        if ( neighbor != NULL )
            neighbor->neighbors[CS_Back] = node;

        neighbor = getNode(x-1, y, z);
        node->neighbors[CS_Left] = neighbor;
        if ( neighbor != NULL )
            neighbor->neighbors[CS_Right] = node;

        neighbor = getNode(x, y-1, z);
        node->neighbors[CS_Bottom] = neighbor;
        if ( neighbor != NULL )
            neighbor->neighbors[CS_Top] = node;

        neighbor = getNode(x, y, z+1);
        node->neighbors[CS_Back] = neighbor;
        if ( neighbor != NULL )
            neighbor->neighbors[CS_Front] = node;

        neighbor = getNode(x+1, y, z);
        node->neighbors[CS_Right] = neighbor;
        if ( neighbor != NULL )
            neighbor->neighbors[CS_Left] = node;

        neighbor = getNode(x, y+1, z);
        node->neighbors[CS_Top] = neighbor;
        if ( neighbor != NULL )
            neighbor->neighbors[CS_Bottom] = node;
    }
}		// -----  end of method Octree<T>::setNeighbors  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  getNeighbors
// Description:  Return a list of neighboring objects for a leaf specified by
//               its location.
//-----------------------------------------------------------------------------
    template < class T >
std::vector<T> Octree<T>::getNeighbors ( Uint x, Uint y, Uint z )
{
    // Get the current node.
    Node *node = getNode(x, y, z);
    // Set an array to store the neighboring objects.
    std::vector<T> values;
    if ( node != NULL ) {
        // Iterate through the neighbors and add existing neighbors to the
        // array.
        for ( int i = 0; i < 6; i += 1 ) {
            Node *neighbor = node->neighbors[i];
            if ( neighbor != NULL )
                values.push_back(neighbor->value);
        }
    }
    // Return the array.
    return values;
}		// -----  end of method Octree<T>::getNeighbors  -----

#endif   // ----- #ifndef OCTREE_H  -----
