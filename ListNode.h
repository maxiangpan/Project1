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

	LTNode* BuyListNode(DataType x)//申请结点
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

	LTNode* LTInit()//初始化创建头结点
	{
		LTNode* phead = BuyListNode(0);
		phead->next = phead;
		phead->pre = phead;
		return phead;
	}

	void LTPrint(LTNode* phead)//打印链表
	{
		assert(phead);//看链表是不是空
		LTNode* cur = phead->next;
		while (cur != phead)
		{
			printf("%d ", cur->data);
			cur = cur->next;
		}
		printf("\n");
	}

	void LTInsert(LTNode* pos, DataType x)//在pos位置之前插入数据
	{
		assert(pos);
		LTNode* node = BuyListNode(x);
		LTNode* bef = pos->pre;
		bef->next = node;
		node->pre = bef;
		node->next = pos;
		pos->pre = node;
	}

	void LTErase(LTNode* pos)//删除pos位置数据
	{
		assert(pos);
		pos->pre->next = pos->next;
		pos->next->pre = pos->pre;
		free(pos);
	}

	void LTPushBack(LTNode* phead, DataType x)//尾插
	{
		/*assert(phead);//复杂方法
		/*LTNode* newnode = BuyListNode(x);
		LTNode* tail = phead->prev;

		tail->next = newnode;
		newnode->prev = tail;

		newnode->next = phead;
		phead->prev = newnode;*/
		assert(phead);//简便方法
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