////////////////////////////////////////////////////////////////////////
// Copyright (C) 2019 Xueyi Yao
//
// This file is part of VPixels.
//
// VPixels is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// VPixels is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with VPixels.  If not, see <https://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////

#ifndef SimpleList_h
#define SimpleList_h

//////////////////////////////
// A simple list
// Used by LuaGif and PyGif to track LuaGifImage and PyGifImage
////////////////////////////////////////////////////////////////
template<typename T>
class SimpleList
{
public:
  SimpleList();
  ~SimpleList();

  // not implemented
  SimpleList( const SimpleList& ) = delete;
  SimpleList( SimpleList&& ) = delete;
  SimpleList& operator=( const SimpleList& ) = delete;
  SimpleList& operator=( SimpleList&& ) = delete;

  // iterating
  void Rewind();
  T* Next();

  // add/remove node
  void Add( T* const pData );
  T* Remove( T* const pData );

  // clear list
  void Clear();

private:
  struct Node
  {
    T* pData;
    Node* pNext;
  };

  Node* m_pHead;
  Node* m_pCurrent;
};

////////////////////
template<typename T> 
SimpleList<T>::SimpleList()
 : m_pHead(nullptr), m_pCurrent(nullptr)
{
}

///////////////////////
template<typename T>
SimpleList<T>::~SimpleList()
{
  Clear();
}

////////////////////
// call it to start a new iterating
////////////////////////////////////
template<typename T>
void SimpleList<T>::Rewind()
{
  m_pCurrent = m_pHead;
}

////////////////
// next node
// return nullptr if reached the tail
///////////////////////////////////////
template<typename T>
T* SimpleList<T>::Next()
{
  if( m_pCurrent == nullptr )
    return nullptr;

  Node* pNode = m_pCurrent;
  m_pCurrent = m_pCurrent->pNext;

  return pNode->pData;
}

//////////////
// add a node
////////////////////////////////////////
template<typename T>
void SimpleList<T>::Add( T* const pData )
{
  // new node
  Node* pNode = new Node();
  pNode->pData = pData;

  // add to the head
  pNode->pNext = m_pHead;
  m_pHead = pNode;  
}

///////////////
// remove a node
// return contained data or nullptr if not in list
///////////////////////////////////////////////////
template<typename T>
T* SimpleList<T>::Remove( T* const pData )
{
  T* Ret = nullptr;
  Node* pPrevNode = nullptr;
  Node* pThisNode = m_pHead;  // start from the head
  while( pThisNode != nullptr )
  {
    if( pThisNode->pData == pData )
    {
      Ret = pThisNode->pData;
      if( pPrevNode == nullptr ) // pThisNode is the head node
        m_pHead = pThisNode->pNext;
      else
        pPrevNode->pNext = pThisNode->pNext;

      delete pThisNode;
      break;
    }

    // next iteration
    pPrevNode = pThisNode;
    pThisNode = pThisNode->pNext;      
  }

  return Ret;
}

///////////////////////
template<typename T>
void SimpleList<T>::Clear()
{
  Node* pThisNode = m_pHead;
  while( pThisNode != nullptr )
  {
    Node* pNode = pThisNode;
    pThisNode = pThisNode->pNext;
    delete pNode;
  }

  m_pHead = nullptr;
}

#endif //SimpleList_h
