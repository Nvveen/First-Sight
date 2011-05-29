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
        typedef std::vector<std::vector<std::vector<T> > > Array3D;
        struct Node {
            Uint depth;
            Node *children[8];
            Node *parent;
            T *value;
            Uint index;
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
        Array3D dataArray_;

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
    dataArray_ = std::vector<std::vector<std::vector<T> > >(size_,
                    std::vector<std::vector<T> >(size_, std::vector<T>(
                            size_)));
    rootNode_ = new Node;
    rootNode_->index = 0;
    rootNode_->value = NULL;
    rootNode_->parent = NULL;
    for ( int i = 0; i < 8; i += 1 ) rootNode_->children[i] = NULL;
    rootNode_->depth = 0;
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
    destructNodes(rootNode_);
    for ( int i = 0; i < size_; i += 1 ) {
        for ( int j = 0; j < size_; j += 1 ) {
            dataArray_[i][j].clear();
        }
        dataArray_[i].clear();
    }
    dataArray_.clear();
}  // -----  end of constructor of template class Octree  -----

    template < class T >
void Octree<T>::insert ( T value, Uint x, Uint y, Uint z )
{
    dataArray_[x][y][z] = value;
    Node *node = getNode(rootNode_, x ,y, z);
    node->value = &dataArray_[x][y][z];
}		// -----  end of method Octree<T>::insert  -----

    template < class T >
void Octree<T>::remove ( Uint x, Uint y, Uint z )
{
    Node *node = getNode(rootNode_, x, y, z);
    node->value = NULL;
}		// -----  end of method Octree<T>::remove  -----

    template < class T >
void Octree<T>::buildTree ( Node *node )
{
    Node *helper = node;
    if ( helper != NULL ) {
        if ( !hasChildren(helper) && pow(2, helper->depth) < size_ ) {
            for ( int i = 0; i < 8; i += 1 ) {
                helper->children[i] = new Node;
                helper->children[i]->depth = helper->depth+1;
                for ( int j = 0; j < 8; j += 1 )
                    helper->children[i]->children[j] = NULL;
                helper->children[i]->index = i;
            }
            for ( int i = 0; i < 8; i += 1 ) buildTree(helper->children[i]);
        }
        else {
            for ( int i = 0; i < 8; i += 1 )
                buildTree(helper->children[i]);
        }
    }
}		// -----  end of method Octree<T>::buildTree  -----

    template < class T >
typename Octree<T>::Node *Octree<T>::getNode ( Node *node, Uint x, Uint y, 
                                               Uint z )
{
    if ( node != NULL ) {
        int index = 0;
        int split = size_/(2*node->depth+2);
        int cubeSize = size_/pow(2, node->depth);
        if ( x % cubeSize >= split ) index |= 1;
        if ( y % cubeSize >= split ) index |= 2;
        if ( z % cubeSize >= split ) index |= 4;
        if ( hasChildren(node) && pow(2, node->depth) < size_ )
            return getNode(node->children[index], x, y, z);
        else
            return node;
    }
}		// -----  end of method Octree<T>::getNode  -----
    template < class T >
bool Octree<T>::hasChildren ( Node *node )
{
    bool notEmpty = false;
    for ( int i = 0; i < 8; i += 1 )
        if ( node->children[i] != NULL ) notEmpty = true;
    return notEmpty;
}		// -----  end of method Octree<T>::hasChildren  -----

    template < class T >
void Octree<T>::print ( Node *node )
{
    Node *helper = node;
    if ( helper != NULL ) {
        if ( hasChildren(helper) )
            for ( int i = 0; i < 8; i += 1 )
                print(helper->children[i]);
        else
            if ( helper->value != NULL )
                std::cout << *(helper->value) << " ";
            else
                std::cout << "NULL ";
    }
}		// -----  end of method Octree<T>::print  -----

    template < class T >
void Octree<T>::print ()
{
    print(rootNode_);
}		// -----  end of method Octree<T>::print  -----

    template < class T >
void Octree<T>::destructNodes ( Node *node )
{
    if ( node != NULL ) {
        if ( hasChildren(node) ) {
            for ( int i = 0; i < 8; i += 1 ) destructNodes(node->children[i]);
        }
        if ( node->value != NULL ) node->value = NULL;
        if ( node->parent != NULL ) node->parent = NULL;
        delete node;
        node = NULL;
    }
}		// -----  end of method Octree<T>::destructNodes  -----

    template < class T >
T Octree<T>::operator() ( Uint x, Uint y, Uint z )
{
    return dataArray_[x][y][z];
}		// -----  end of method Octree<T>::operator()  -----

#endif   // ----- #ifndef OCTREE_H  -----
