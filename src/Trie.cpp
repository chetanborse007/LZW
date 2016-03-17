/******************************************************************************//*!
* @File          Trie.cpp
* 
* @Title         Implementation of Trie data structure.
* 
* @Author        Chetan Borse
* 
* @Created       03/14/2016
* 
* @Platform      ?
* 
* @Description   This file implements member functions of Trie class.
*
*//*******************************************************************************/ 

#include "Trie.h"


/* Initializing Trie's static data member. */
Trie *Trie::s_pTrie = NULL;


/******************************************************************************
* @Function		Trie::GetRootNode
*
* @Description  Get a Root Node of Trie data structure.
*
* @Return       Node*                   Returns pointer to Trie's Root Node
******************************************************************************/
Node* const Trie::GetRootNode()
{
    return m_pRoot;
}


/******************************************************************************
* @Function		Trie::GetInstance
*
* @Description  Get an instance of singleton Trie class.
*
* @Return       Trie*                   Returns pointer to Trie class
******************************************************************************/
Trie* Trie::GetInstance()
{
    if (!s_pTrie)
        s_pTrie = new Trie;
        
    return s_pTrie;
}


/******************************************************************************
* @Function		Trie::DestroyInstance
*
* @Description	Destroy an instance of singleton Trie class.
*
* @Return		void					Returns nothing
******************************************************************************/
void Trie::DestroyInstance()
{
    if (s_pTrie)
        delete s_pTrie;
        
    s_pTrie = NULL;
}


/******************************************************************************
* @Function		Trie::AddWord
*
* @Description	Store a word into Trie data structure.
*
* @Input		Node*		pNode		Pointer to Trie node
*
* @Input		string		pszWord		Word to be added into Trie
*
* @Input		uint16_t	u2Code		Equivalent code for a word
*
* @Return		void					Returns nothing
******************************************************************************/
void Trie::AddWord(Node *pNode, std::string pszWord, uint16_t u2Code)
{
    char chSymbol;
    Node *pChildNode;
    
    // Store equivalent code and mark it as a 'Word',
    // when reached at the end of word.
    if (pszWord.empty())
    {
        pNode->SetCode(u2Code);
        pNode->SetIsWord(true);
        return;
    }

    // Extract leftmost character from a word.
    chSymbol = pszWord.at(0);
    
    // Search extracted character into a collection of children.
    // If not found, create a new node and add it into the vector of children.
    pChildNode = pNode->SearchChildNode(chSymbol);
    if (pChildNode == NULL)
    {
        pChildNode = new Node(chSymbol);
        pNode->AddChildNode(pChildNode);
    }
    
    // Recursively traverse Trie downwards, until entire new word is added.
    AddWord(pChildNode, pszWord.erase(0, 1), u2Code);
}


/******************************************************************************
* @Function	Trie::SearchWord
*
* @Description	Search for a word into Trie data structure.
*
* @Input		Node*		pNode		Pointer to Trie node
*
* @Input		string		pszWord		Word to be searched into Trie
*
* @Return		Node*					Returns pointer to Trie node
******************************************************************************/
Node* Trie::SearchWord(Node *pNode, std::string pszWord)
{
    char chSymbol;
    Node *pChildNode;
    
    // Return pointer to Trie node, when reached at the end of word and
    // if current Trie node is marked as 'Word'.
    if (pszWord.empty())
    {
        if (pNode->IsWord())
            return pNode;
        else
            return NULL;
    }
    
    // Extract leftmost character from a word.
    chSymbol = pszWord.at(0);

    // Search extracted character into a collection of children.
    // If not found, return NULL.
    pChildNode = pNode->SearchChildNode(chSymbol);
    if (pChildNode == NULL)
        return NULL;
    
    // Recursively traverse Trie downwards, until entire word is found.
    return SearchWord(pChildNode, pszWord.erase(0, 1));
}