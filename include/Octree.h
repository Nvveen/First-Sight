// ============================================================================
// 
//       Filename:  Octree.h
// 
//    Description:  This class implements a templated octree, for easy traversal
//                  for example, voxel data.
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

template <class T>
class Octree
{
    public:
        typedef unsigned int Uint;

        struct Node {
            Uint depth;
            Node *children[8];
            Node *parent;
            T *value;
        };
        // ====================  LIFECYCLE     ================================
        Octree ( Uint size );                           // constructor
        ~Octree ();

        // ====================  ACCESSORS     ================================

        // ====================  MUTATORS      ================================
        void insert ( T value, Uint x, Uint y, Uint z );
        void remove ( Uint x, Uint y, Uint z );
        void buildTree ( Node *node );
        Node *getNode ( Node *node, Uint x, Uint y, Uint z );
        bool hasChildren ( Node *node );
        void destructNodes ( Node *node );

        void print ( Node *node );
        void print ();

        // ====================  OPERATORS     ================================
        T operator() ( Uint x, Uint y, Uint z );

    protected:
        // ====================  DATA MEMBERS  ================================

    private:
        // ====================  DATA MEMBERS  ================================
        Node *rootNode_;
        Uint size_;

}; // -----  end of template class Octree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  Octree
// Description:  constructor
//-----------------------------------------------------------------------------
    template < class T >
Octree<T>::Octree ( Uint size ) :
    size_(size)
{
    // Create the root node.
    rootNode_ = new Node;
    rootNode_->parent = NULL;
    rootNode_->value = NULL;
    for ( int i = 0; i < 8; i += 1 ) rootNode_->children[i] = NULL;
    rootNode_->depth = 0;
    // Build the rest of the tree.
    buildTree(rootNode_);
}  // -----  end of constructor of template class Octree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  Octree
// Description:  constructor
//-----------------------------------------------------------------------------
    template < class T >
Octree<T>::~Octree ()
{
    // Recursively delete each node.
    destructNodes(rootNode_);
}  // -----  end of constructor of template class Octree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  insert
// Description:  Insert a value into the octree.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::insert ( T value, Uint x, Uint y, Uint z )
{
    // Get the node.
    Node *node = getNode(rootNode_, x ,y, z);
    // Create a new value for the node.
    node->value = new T;
    *node->value = value;
}		// -----  end of method Octree<T>::insert  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  remove
// Description:  Remove a value from the Octree.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::remove ( Uint x, Uint y, Uint z )
{
    // Get the node.
    Node *node = getNode(rootNode_, x, y, z);
    if ( node != NULL && node->value != NULL ) {
        // Delete the value belonging to the node.
        delete node->value;
        // Reset the pointer.
        node->value = NULL;
    }
}		// -----  end of method Octree<T>::remove  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  buildTree
// Description:  Recursively go through the method building an octree until a 
//               certain depth that is defined by the size.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::buildTree ( Node *node )
{
    Node *helper = node;
    if ( helper != NULL ) {
        // If we get the leaf node, create it if its depth is smaller than the
        // max depth.
        if ( !hasChildren(helper) && pow(2, helper->depth) < size_ ) {
            // Create a new node for each child.
            for ( int i = 0; i < 8; i += 1 ) {
                // Set its values.
                helper->children[i] = new Node;
                helper->children[i]->depth = helper->depth+1;
                helper->children[i]->parent = helper;
                // Set its children to empty.
                for ( int j = 0; j < 8; j += 1 )
                    helper->children[i]->children[j] = NULL;
            }
            // Recursively call the children.
            for ( int i = 0; i < 8; i += 1 ) buildTree(helper->children[i]);
        }
        else {
            for ( int i = 0; i < 8; i += 1 )
                buildTree(helper->children[i]);
        }
    }
}		// -----  end of method Octree<T>::buildTree  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  getNode
// Description:  Return the node address that belongs to the specific location
//               defined by its index.
//-----------------------------------------------------------------------------
    template < class T >
typename Octree<T>::Node *Octree<T>::getNode ( Node *node, Uint x, Uint y, 
                                               Uint z )
{
    if ( node != NULL ) {
        // We have to look where in a cube the current node belongs. We do that
        // by using an index, and if the coordinates of the node are bigger than
        // the 3 axis that split the cube in half to create 8 pieces, we mask
        // the index against the three values to create an index from 0 to 7.
        int index = 0;
        // The split is defined as the half of a cube/split cube. The depth of a
        // node gives us a way to see how many times the cube has been split.
        int split = size_/(2*node->depth+2);
        // The cube size is the size of the current cube, depending on the
        // depth.
        int cubeSize = size_/pow(2, node->depth);
        // Mask the index.
        if ( x % cubeSize >= split ) index |= 1;
        if ( y % cubeSize >= split ) index |= 2;
        if ( z % cubeSize >= split ) index |= 4;
        // If we don't have a leaf, recursively call the method.
        if ( hasChildren(node) && pow(2, node->depth) < size_ )
            return getNode(node->children[index], x, y, z);
        // Otherwise, we can return that leaf, because it has been found.
        else
            return node;
    }
}		// -----  end of method Octree<T>::getNode  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  hasChildren
// Description:  Return whether the specific node has children (in other words,
//               if its children pointers are all NULL or not).
//-----------------------------------------------------------------------------
    template < class T >
bool Octree<T>::hasChildren ( Node *node )
{
    bool notEmpty = false;
    for ( int i = 0; i < 8; i += 1 )
        if ( node != NULL && node->children[i] != NULL ) notEmpty = true;
    return notEmpty;
}		// -----  end of method Octree<T>::hasChildren  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  print
// Description:  Recursively go through the octree, and print the value if the
//               currently selected node is a leaf.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::print ( Node *node )
{
    Node *helper = node;
    if ( helper != NULL ) {
        // If the current node is not a leaf, recursively call the print
        // function.
        if ( hasChildren(helper) )
            for ( int i = 0; i < 8; i += 1 )
                print(helper->children[i]);
        else
            // We found the node, but need to check if its value is set.
            if ( helper->value != NULL )
                std::cout << *(helper->value) << " ";
            else
                std::cout << "NULL ";
    }
}		// -----  end of method Octree<T>::print  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  print
// Description:  A wrapper method for the main print method by defaulting to the
//               root node.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::print ()
{
    print(rootNode_);
}		// -----  end of method Octree<T>::print  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  destructNodes
// Description:  Iterate through each and every node and delete it.
//-----------------------------------------------------------------------------
    template < class T >
void Octree<T>::destructNodes ( Node *node )
{
    if ( node != NULL ) {
        if ( hasChildren(node) )
            for ( int i = 0; i < 8; i += 1 ) destructNodes(node->children[i]);
        if ( node->parent != NULL ) node->parent = NULL;
        delete node;
        node = NULL;
    }
}		// -----  end of method Octree<T>::destructNodes  -----

//-----------------------------------------------------------------------------
//       Class:  Octree
//      Method:  operator()
// Description:  Access the node specified by its location, and print its value.
//-----------------------------------------------------------------------------
    template < class T >
T Octree<T>::operator() ( Uint x, Uint y, Uint z )
{
    return *(getNode(rootNode_, x, y, z)->value);
}		// -----  end of method Octree<T>::operator()  -----

#endif   // ----- #ifndef OCTREE_H  -----
