﻿/**@file  		TList.h
* @brief		Double link list
* @author		Daphnis Kau
* @date			2019-06-24
* @version		V1.0
* @note			No memory need to be allocated/freed when insert a node to \n
*				or remove node from a TList. Nodes can be removed directly \n
*				without knowing the parent TList. An object can exist in \n
*				multiple lists by inheritance from multiple subclasses of \n
*				CListNode.
*				
*/
#ifndef __XS_LIST_H__
#define __XS_LIST_H__

#include "Help.h"

namespace XS
{
	template<typename ImpClass>
	class TList
	{
		const TList& operator= ( const TList& );
		TList( const TList& );
	public:
		typedef ImpClass CNode;
		class CListNode
		{
			CListNode*	m_pPreNode;
			CListNode*	m_pNextNode;
			friend class TList<CNode>;

		public:
			CListNode() : m_pPreNode(nullptr), m_pNextNode(nullptr)
			{
			}

			~CListNode()
			{ 
				Remove();
			}

			bool IsInList() const
			{ 
				return m_pPreNode != nullptr; 
			}

			void Remove()
			{
				if( !IsInList() )
					return;
				m_pPreNode->m_pNextNode = m_pNextNode;
				m_pNextNode->m_pPreNode = m_pPreNode;
				m_pPreNode = nullptr;
				m_pNextNode = nullptr;
			}

			CNode* GetPre() const
			{
				return m_pPreNode && m_pPreNode->m_pPreNode ? static_cast<CNode*>( m_pPreNode ) : nullptr;
			}

			CNode* GetNext() const
			{
				return m_pNextNode && m_pNextNode->m_pNextNode ? static_cast<CNode*>( m_pNextNode ) : nullptr;
			}

			void InsertBefore( CListNode& Node )
			{
				assert( !Node.IsInList() );
				assert( &Node != this );

				Node.m_pPreNode  = m_pPreNode;
				Node.m_pNextNode = this;
				m_pPreNode->m_pNextNode = &Node;
				m_pPreNode = &Node;
			}

			void InsertAfter( CListNode& Node )
			{
				assert( !Node.IsInList() );
				assert( &Node != this );

				Node.m_pNextNode  = m_pNextNode;
				Node.m_pPreNode = this;
				m_pNextNode->m_pPreNode = &Node;
				m_pNextNode = &Node;
			}
		};

	public:
		class iterator
		{
			CNode* m_pNode;
		public:
			iterator() : m_pNode( nullptr ){}
			iterator( CNode* pNode ) : m_pNode( pNode ){}
			iterator( const iterator& rhs ) : m_pNode( rhs.m_pNode ){}
			iterator operator= ( CNode* pNode ) { m_pNode = pNode; return *this; }
			iterator operator= ( const iterator& rhs ) { m_pNode = rhs.m_pNode; return *this; }
			bool operator == ( CNode* pNode ) const { return m_pNode == pNode; }
			bool operator == ( const iterator& rhs ) const { return m_pNode == rhs.m_pNode; }
			bool operator != ( CNode* pNode ) const { return m_pNode != pNode; }
			bool operator != ( const iterator& rhs ) const { return m_pNode != rhs.m_pNode; }
			iterator& operator++() { m_pNode = m_pNode ? m_pNode->CListNode::GetNext() : nullptr; return *this; }
			iterator operator++( int ) { iterator i = *this; ++*this; return i; }
			iterator& operator--() { m_pNode = m_pNode ? m_pNode->CListNode::GetPre() : nullptr; return *this; }
			iterator operator--( int ) { iterator i = *this; ++*this; return i; }
			CNode& operator* () const { return *m_pNode; }
		};

		TList()
		{
			m_NodeHead.m_pNextNode = &m_NodeTail;
			m_NodeTail.m_pPreNode = &m_NodeHead;
		}

		~TList()
		{
			assert( IsEmpty() );
			m_NodeHead.m_pNextNode = nullptr;
			m_NodeTail.m_pPreNode = nullptr;
		}

		bool IsEmpty() const
		{
			return m_NodeHead.m_pNextNode == &m_NodeTail;
		}

		void PushFront( CListNode& Node )
		{
			InsertAfter( Node, nullptr );
		}

		void PushBack( CListNode& Node )
		{
			InsertBefore( Node, nullptr );
		}

		void InsertBefore( CListNode& Node, CListNode* pNodePos )
		{
			assert( !Node.IsInList() );
			assert( &Node != pNodePos );

			pNodePos = pNodePos ? pNodePos : &m_NodeTail;
			Node.m_pPreNode  = pNodePos->m_pPreNode;
			Node.m_pNextNode = pNodePos;
			pNodePos->m_pPreNode->m_pNextNode = &Node;
			pNodePos->m_pPreNode = &Node;
		}

		void InsertAfter( CListNode& Node, CListNode* pNodePos )
		{
			assert( !Node.IsInList() );
			assert( &Node != pNodePos );

			pNodePos = pNodePos ? pNodePos : &m_NodeHead;
			Node.m_pNextNode  = pNodePos->m_pNextNode;
			Node.m_pPreNode = pNodePos;
			pNodePos->m_pNextNode->m_pPreNode = &Node;
			pNodePos->m_pNextNode = &Node;
		}

		CNode* GetFirst() const
		{
			return IsEmpty() ? nullptr : static_cast<CNode*>( m_NodeHead.m_pNextNode );
		}

		CNode* GetLast() const
		{
			return IsEmpty() ? nullptr : static_cast<CNode*>( m_NodeTail.m_pPreNode );
		}
		
		iterator begin()
		{
			return iterator( GetFirst() );
		}

		iterator end()
		{
			return iterator();
		}

		iterator rbegin()
		{
			return iterator( GetLast() );
		}

		iterator rend()
		{
			return iterator();
		}
	private:
		CListNode	m_NodeHead;
		CListNode	m_NodeTail;
	};

	template<typename DataType>
	class TListNode : 
		public TList< TListNode<DataType> >::CListNode
	{
		DataType m_Data;
	public:
		TListNode() {}
		TListNode( const DataType& v ) : m_Data( v ) {}
		const DataType& Get() { return m_Data; }
		void Set( const DataType& v ) { m_Data = v; }
	};
}

#endif
