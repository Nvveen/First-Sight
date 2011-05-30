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

// ============================================================================
//        Class:  Octree
//  Description:  An ADT containing an octree.
// ============================================================================

#ifndef  OCTREE_H
#define  OCTREE_H

#include    <cmath>
#include    <queue>

template <class T>
class Octree
{
    public:
        typedef unsigned int Uint;
        typedef std::queue<Uint> IndexList;

        struct Node {
            Uint depth;
            Node *children[8];
            Node *parent;
            T value;
        };
        // ====================  LIFECYCLE     ================================
        Octree ( Uint size );                           // constructor
        ~Octree ();
        Node *createNewNode ( Node *parent );
        void print ( Node *node );
        void print ();
        void destructNodes ( Node *node );

        // ====================  ACCESSORS     ================================
        IndexList getNodeIndices ( Uint x, Uint y, Uint z );
        bool isLeaf ( Node *node );

        // ====================  MUTATORS      ================================
        void insert ( Uint x, Uint y, Uint z, T value );
        void remove ( Uint x, Uint y, Uint z );

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
    // The root node has no parent, so check for that, then set its depth.
    if ( parent != NULL ) node->depth = parent->depth+1;
    else node->depth = 0;
    // Set all children to NULL
    for ( int i = 0; i < 8; i += 1 ) node->children[i] = NULL;
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
    // Get index list.
    IndexList indices = getNodeIndices(x, y, z);
    // We begin at the root.
    Node *node = rootNode_;
    Node *next = NULL;
    // For simplicity, we'll remember the last index, so the size must be bigger
    // than 1.
    while ( indices.size() > 1 ) {
        // Get the current index.
        Uint index = indices.front();
        // Get the next node defined by its index in the current pointer's
        // children array.
        next = node->children[index];
        // If we haven't arrived at the destination yet but the pointer is
        // already NULL, something went wrong with building the array, and we
        // have to exit.
        if ( next == NULL ) {
            std::cerr << "Error removing node (" << x << "," << y << "," << z;
            std::cerr << ")\n";
            exit(1);
        }
        // Switch over to the next level.
        node = next;
        // Remove the current index from the list.
        indices.pop();
    }
    // We have arrived at the correct node (the index list is empty), so we can
    // remove the node and set its parent's child pointer to empty.
    delete node->children[indices.front()];
    node->children[indices.front()] = NULL;
}		// -----  end of method Octree<T>::remove  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  getNodeIndices
// Description:  This method gets a location of an object in the Octree, and
//               depending on where in a cube of 8 subcubes it is located, it
//               returns the correct index list.
//-----------------------------------------------------------------------------
    template < class T >
typename Octree<T>::IndexList Octree<T>::getNodeIndices ( Uint x, Uint y, 
                                                          Uint z )
{
    // For every level, we need an index, so we use a list of indices.
    IndexList indices;
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
    // Find the current node by its index list in the octree.
    IndexList indices = getNodeIndices(x, y, z);
    Node *node = rootNode_;
    // Iterate through the octree.
    while ( !indices.empty() ) {
        Uint index = indices.front();
        if ( node != NULL ) {
            node = node->children[index];
        }
        else {
            // The index list isn't empty, but we have arrived at an empty node,
            // so the current location was invalid.
            std::cerr << "Invalid Octree-location specified.\n";
            exit(1);
        }
        indices.pop();
    }
    // We have found the node belonging to that location, so we can return its
    // value.
    return node->value;
}		// -----  end of method Octree<T>::operator()  -----
#endif   // ----- #ifndef OCTREE_H  -----
