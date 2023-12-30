#pragma once
typedef int DataType;
class listNodeClass
{
public:
	listNodeClass();
	~listNodeClass();
	typedef struct ListNode
	{
		struct ListNode* next;
		struct ListNode* pre;
		DataType data;
	}LTNode;

	LTNode* BuyListNode(DataType x)//������
	{
		LTNode* node = (LTNode*)malloc(sizeof(LTNode));
		if (node == NULL)
		{
			perror("malloc fail");
			exit(-1);
		}
		node->next = NULL;
		node->pre = NULL;
		node->data = x;
		return node;
	}

	LTNode* LTInit()//��ʼ������ͷ���
	{
		LTNode* phead = BuyListNode(0);
		phead->next = phead;
		phead->pre = phead;
		return phead;
	}

	void LTPrint(LTNode* phead)//��ӡ����
	{
		assert(phead);//�������ǲ��ǿ�
		LTNode* cur = phead->next;
		while (cur != phead)
		{
			printf("%d ", cur->data);
			cur = cur->next;
		}
		printf("\n");
	}

	void LTInsert(LTNode* pos, DataType x)//��posλ��֮ǰ��������
	{
		assert(pos);
		LTNode* node = BuyListNode(x);
		LTNode* bef = pos->pre;
		bef->next = node;
		node->pre = bef;
		node->next = pos;
		pos->pre = node;
	}

	void LTErase(LTNode* pos)//ɾ��posλ������
	{
		assert(pos);
		pos->pre->next = pos->next;
		pos->next->pre = pos->pre;
		free(pos);
	}

	void LTPushBack(LTNode* phead, DataType x)//β��
	{
		/*assert(phead);//���ӷ���
		/*LTNode* newnode = BuyListNode(x);
		LTNode* tail = phead->prev;

		tail->next = newnode;
		newnode->prev = tail;

		newnode->next = phead;
		phead->prev = newnode;*/
		assert(phead);//��㷽��
		LTInsert(phead, x);
	}


private:

};

listNodeClass::listNodeClass()
{

}

listNodeClass::~listNodeClass()
{

}