struct ObjStatusNode
{
	int s_nData;
	ObjStatusNode *pNextNode, *pPrevNode;
};

class CObjStatusList
{
public:
	CObjStatusList()
	{
		m_nOpCount = 0;
		m_nNum = 0;
		pHead = NULL;
		pTail = NULL;
	}

	void Create(int nNum)
	{
		m_nNum = nNum;
		ObjStatusNode *pNode = new ObjStatusNode;
		for (int n=0; n<nNum; ++n)
		{
			pNode->s_nData = n;
			if (n == nNum-1)
			{
				pNode->pNextNode = NULL;
				pTail = pNode;
			}
			else 
			{
				ObjStatusNode *pNext = new ObjStatusNode;
				pNode->pNextNode = pNext;
				pNext->pPrevNode = pNode;
				if (n == 0)
				{
					pHead = pNode;
					pNode->pPrevNode = NULL;
				}
				pNode = pNext;
			}
		}
	}

	~CObjStatusList()
	{
		ObjStatusNode* pNode = pHead;
		while (pNode)
		{
			ObjStatusNode* pNext = pNode->pNextNode;
			pNode->s_nData = -1;
			pNode->pPrevNode = NULL;
			pNode->pNextNode = NULL;
			delete pNode;
			pNode = pNext;
		}
		pHead = NULL;
		pTail = NULL;
	}

	ObjStatusNode* GetHeadDataAndMoveBack()
	{
		if (m_nOpCount == m_nNum)
			return NULL;

		ObjStatusNode* pHeadI1 = pHead->pNextNode;
		pHeadI1->pPrevNode = NULL;

		pTail->pNextNode = pHead;
		pHead->pPrevNode = pTail;
		pHead->pNextNode = NULL;
		pTail = pHead;
		pHead = pHeadI1;
		m_nOpCount++;
		return pTail;
	}

	void MoveForward(ObjStatusNode* pNode)
	{
		pNode->pPrevNode->pNextNode = pNode->pNextNode;
		if (pNode->pNextNode)
			pNode->pNextNode->pPrevNode = pNode->pPrevNode;
		else
			pTail = pNode->pPrevNode;
		pHead->pPrevNode = pNode;
		pNode->pPrevNode = NULL;
		pNode->pNextNode = pHead;
		pHead = pNode;
		m_nOpCount--;
	}
	
	void GetSequence(char* chSeq)
	{
		ObjStatusNode* pNode = pHead;
		while (pNode)
		{
			char chd[4];
			sprintf(chd, "%d ", pNode->s_nData);
			strcat(chSeq, chd);

			pNode = pNode->pNextNode;
		}
		chSeq[strlen(chSeq)-1] = 0;
	}

protected:
	ObjStatusNode *pHead, *pTail;
	int m_nNum, m_nOpCount;
};