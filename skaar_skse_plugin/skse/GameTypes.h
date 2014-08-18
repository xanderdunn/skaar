#pragma once

#include "skse/Utilities.h"
#include "GameAPI.h"

// core library types (lists, strings, vectors)
// preferably only the ones that bethesda created (non-netimmerse)

class BSIntrusiveRefCounted
{
public:
	volatile UInt32	m_refCount;	// 00
};

// 04
template <typename T>
class BSTSmartPointer
{
public:
	// refcounted
	T	* ptr;
};

class SimpleLock
{
	enum
	{
		kFastSpinThreshold = 10000
	};

	volatile SInt32	threadID;	// 00
	UInt32			lockCount;	// 04

public:
	SimpleLock() : threadID(0), lockCount(0) {}

	void Lock(void);
	void Release(void);
};
STATIC_ASSERT(sizeof(SimpleLock) == 0x8);

// refcounted threadsafe string storage
// use StringCache::Ref to access everything, other internals are for documentation only
class StringCache
{
public:
	// BSFixedString?
	struct Ref
	{
		const char	* data;

		MEMBER_FN_PREFIX(Ref);
		DEFINE_MEMBER_FN(ctor, Ref *, 0x00A511C0, const char * buf);
		DEFINE_MEMBER_FN(Set, Ref *, 0x00A51210, const char * buf);
		DEFINE_MEMBER_FN(Release, void, 0x00A511B0);

		Ref() :data(NULL) { }
		Ref(const char * buf);

		bool operator==(const Ref& lhs) const { return data == lhs.data; }
		bool operator<(const Ref& lhs) const { return data < lhs.data; }
	};

	struct Lock
	{
		SimpleLock	lock;
	};

	StringCache();
	~StringCache();

	static StringCache *	GetSingleton(void);

	Lock *	GetLock(UInt32 crc16);

private:
	struct Entry
	{
		Entry	* next;		// 00
		union
		{
			struct  
			{
				UInt16	refCount;	// invalid if 0x8000 is set
				UInt16	hash;
			};
			UInt32	refCountAndHash;
		} state;	// 04
		UInt32	length;		// 08
		// data follows
	};

	Entry	* table[0x10000];	// crc16
	Lock	locks[0x20];		// crc16 & 0x1F
	UInt8	unk;
};

typedef StringCache::Ref	BSFixedString;

// 08
class BSString
{
public:
	BSString() :m_data(NULL), m_dataLen(0), m_bufLen(0) { }
	~BSString();

	const char *	Get(void);

private:
	char	* m_data;	// 00
	UInt16	m_dataLen;	// 04
	UInt16	m_bufLen;	// 06
};

// 0C
template <class T>
class tArray
{
public:
	struct Array {
		T* entries;
		UInt32	capacity;

		Array() : entries(NULL), capacity(0) {}
	};

	Array arr;		// 00
	UInt32 count;	// 08

	tArray() : count(0) {}

	T& operator[] (UInt32 index) { return arr.entries[index]; }

	// This could be better
	bool CopyFrom(tArray<T> * rhs)
	{
		if(rhs->count == 0) return false;
		if(!rhs->arr.entries) return false;
		if(!Allocate(rhs->count)) return false;
		memcpy(arr.entries, rhs->arr.entries, sizeof(T) * count);
		return true;
	}

	bool Allocate(UInt32 numEntries)
	{
		arr.entries = (T *)FormHeap_Allocate(sizeof(T) * numEntries);
		if(!arr.entries) return false;

		for(UInt32 i = 0; i < numEntries; i++)
			new (&arr.entries[i]) T;

		arr.capacity = numEntries;
		count = numEntries;

		return true;
	}
	
	bool GetNthItem(UInt32 index, T& pT) const
	{
		if (index < count) {
			pT = arr.entries[index];
			return true;
		}
		return false;
	}

	SInt32 GetItemIndex(T & pFind) const
	{
		for (UInt32 n = 0; n < count; n++) {
			T& pT = arr.entries[n];
			if (pT == pFind)
				return n;
		}
		return -1;
	}
};

typedef tArray<UInt32> UnkArray;
typedef tArray<TESForm*> UnkFormArray;

// Returns if/where the element was found, otherwise indexOut can be used as insert position
template <typename T>
bool GetSortIndex(tArray<T> & arr, T & elem, SInt32 & indexOut)
{
	UInt32 count = arr.count;
	if (count == 0)
	{
		indexOut = 0;
		return false;
	}
	
	SInt32 leftIdx = 0;
	SInt32 rightIdx = count - 1;

	while (true)
	{
		UInt32 pivotIdx = leftIdx + ((rightIdx - leftIdx) / 2);

		T & p = arr[pivotIdx];
		
		if (elem == p)
		{
			indexOut = pivotIdx;
			return true;
		}
		else if (elem > p)
		{
			leftIdx = pivotIdx + 1;
		}
		else
		{
			rightIdx = pivotIdx - 1;
		}

		if (leftIdx > rightIdx)
		{
			indexOut = leftIdx;
			return false;
		}
	}
}

enum {
	eListCount = -3,
	eListEnd = -2,
	eListInvalid = -1,		
};

// 08
template <class T>
class tList
{
	struct _Node
	{
		
		T *		item;	// 00
		_Node*	next;	// 04

		T *		Item() const { return item; }
		_Node*	Next() const { return next; }

		T *		Remove(_Node * pred)
		{
			T * pRemoved = item;
			_Node * pNext = next;

			// become the next entry and return my item
			if (pNext)
			{
				item = pNext->item;
				next = pNext->next;
				FormHeap_Free(pNext);
			}
			// tail?
			else
			{
				item = NULL;
				next = NULL;

				// has precedessor, so tail != head
				if (pred)
				{
					pred->next = NULL;
					FormHeap_Free(this);
				}
			}
			return pRemoved;
		}
	};

	_Node m_listHead;	// 00

private:

	_Node * Head() const { return const_cast<_Node*>(&m_listHead); }

	_Node * Tail() const
	{
		_Node * node = const_cast<_Node*>(&m_listHead);

		if (node)
			while (node->next) node = node->next;
		
		return node;
	}

	template <class Op>
	UInt32 FreeNodes(Op &compareOp)
	{
		UInt32 numFreed = 0;

		_Node * pPred = NULL;
		_Node * pCur = Head();

		while (pCur)
		{
			if (pCur->next)
			{
				if (compareOp.Accept(pCur->item))
				{
					pCur->Remove(pPred);
					numFreed++;
				}
				else
				{
					pPred = pCur;
					pCur = pCur->next;
				}
			}
			// List Tail?
			else
			{
				if (compareOp.Accept(pCur->item))
				{
					pCur->Remove(pPred);
					numFreed++;
				}
				break;
			}
		}

		return numFreed;
	}

	struct NodePos
	{
		NodePos(): node(NULL), index(eListInvalid) {}

		_Node*	node;
		SInt32	index;
	};


	NodePos GetNthNode(SInt32 index) const
	{
		NodePos pos;
		SInt32 n = 0;
		_Node* pCur = Head();

		while (pCur && pCur->Item())
		{
			if (n == index) break;
			if (eListEnd == index && !pCur->Next()) break;
			pCur = pCur->Next();
			++n;
		}

		pos.node = pCur;
		pos.index = n;

		return pos;
	}

public:

	// Allocate list as a single empty node
	static tList<T> * Create(void)
	{
		tList<T> * p = (tList<T> *)FormHeap_Allocate(sizeof(tList<T>));
		ASSERT(p);

		p->m_listHead.item = NULL;
		p->m_listHead.next = NULL;

		return p;
	}

	void Delete(void)
	{
		RemoveAll();
		FormHeap_Free(&m_listHead);
	}

	class Iterator
	{
		_Node *	m_cur;
	public:
		Iterator() : m_cur(NULL) {}
		Iterator(_Node* node) : m_cur(node) { }
		Iterator operator++()	{ if (!End()) m_cur = m_cur->Next(); return *this;}
		bool End()	{	return m_cur == NULL;	}
		const T * operator->() { return (m_cur) ? m_cur->Item() : NULL; }
		const T * operator*() { return (m_cur) ? m_cur->Item() : NULL; }
		const Iterator& operator=(const Iterator& rhs) {
			m_cur = rhs.m_cur;
			return *this;
		}
		T * Get() { return (m_cur) ? m_cur->Item() : NULL; }
	};
	
	const Iterator Begin() const { return Iterator(Head()); }

	void Insert(T * item)
	{
		// add new node if we aren't empty
		if (m_listHead.item)
		{
			// copy head in to new node
			_Node	* node = (_Node *)FormHeap_Allocate(sizeof(_Node));
			ASSERT(node);

			node->item = m_listHead.item;
			node->next = m_listHead.next;

			m_listHead.next = node;
		}

		m_listHead.item = item;
	}

	void Push(T * item)
	{
		_Node * tail = Tail();
		
		// add new node if we aren't empty
		if (tail->item)
		{
			_Node * node = (_Node *)FormHeap_Allocate(sizeof(_Node));
			ASSERT(node);

			tail->next = node;

			node->item = item;
			node->next = NULL;
		}
		else
		{
			tail->item = item;
		}
	}

	T * AddFront(void)
	{
		T	* item = (T *)FormHeap_Allocate(sizeof(T));
		if(!item)
			return NULL;

		new (item) T;

		Insert(item);
		return item;
	}

	T * AddBack(void)
	{
		T	* item = (T *)FormHeap_Allocate(sizeof(T));
		if(!item)
			return NULL;

		new (item) T;

		Push(item);
		return item;
	}

	void Append(Iterator source)
	{
		while (!source.End())
		{
			Push(source.Get());
			++source;
		}
	}

	UInt32 Count() const
	{
		NodePos pos = GetNthNode(eListCount);
		return (pos.index > 0) ? pos.index : 0;
	};

	T * GetNthItem(SInt32 n) const
	{
		NodePos pos = GetNthNode(n);
		return (pos.index == n && pos.node) ? pos.node->Item() : NULL;
	}

	T * GetLastItem() const
	{
		NodePos pos = GetNthNode(eListEnd);
		return pos.node->Item();
	}

	SInt32 AddAt(T * item, SInt32 index)
	{
		if (!m_listHead.item) {
			m_listHead.item = item;
			return 0;
		}

		NodePos pos = GetNthNode(index);
		_Node* pTargetNode = pos.node;
		_Node* newNode = (_Node*)FormHeap_Allocate(sizeof(newNode));
		if (newNode && pTargetNode) {
			if (index == eListEnd) {
				pTargetNode->next = newNode;
				newNode->item = item;
				newNode->next = NULL;
			} else {
				newNode->item = pTargetNode->item;
				newNode->next = pTargetNode->next;
				pTargetNode->item = item;
				pTargetNode->next = newNode;
			}
			return pos.index;
		}
		return eListInvalid;
	}

	template <class Op>
	void Visit(Op& op, _Node* prev = NULL) const {
		const _Node* pCur = (prev) ? prev->next : Head();
		bool bContinue = true;
		while (pCur && bContinue) {
			bContinue = op.Accept(pCur->Item());
			if (bContinue) {
				pCur = pCur->next;
			}
		}
	}

	template <class Op>
	T * Find(Op& op) const
	{
		const _Node* pCur = Head(); 

		bool bFound = false;
		while (pCur && !bFound)
		{
			if (!pCur->Item())
				pCur = pCur->Next();
			else
			{
				bFound = op.Accept(pCur->Item());
				if (!bFound)
					pCur = pCur->Next();
			}
		}
		return (bFound && pCur) ? pCur->Item() : NULL;
	}

	template <class Op>
	Iterator Find(Op& op, Iterator prev) const
	{
		Iterator curIt = (prev.End()) ? Begin() : ++prev;
		bool bFound = false;
		
		while(!curIt.End() && !bFound) {
			const T * pCur = *curIt;
			if (pCur) {
				bFound = op.Accept(pCur);
			}
			if (!bFound) {
				++curIt;
			}	
		}
		return curIt;
	}

	const _Node* FindString(char* str, Iterator prev) const
	{
		return Find(StringFinder_CI(str), prev);
	}

	template <class Op>
	UInt32 CountIf(Op& op) const
	{
		UInt32 count = 0;
		const _Node* pCur = Head();
		while (pCur)
		{
			if (pCur->Item() && op.Accept(pCur->Item()))
				count++;
			pCur = pCur->Next();
		}
		return count;
	}

	class AcceptAll {
	public:
		bool Accept(T * item) {
			return true;
		}
	};

	void RemoveAll()
	{
		FreeNodes(AcceptAll());
	}

	T * RemoveNth(SInt32 n) 
	{
		Item* pRemoved = NULL;
		if (n == 0) {
			pRemoved =  m_listHead.RemoveMe();
		} else if (n > 0) {
			NodePos nodePos = GetNthNode(n);
			if (nodePos.node && nodePos.index == n) {
				pRemoved = nodePos.node->RemoveMe();
			}
		}
		return pRemoved;
	};

	T * ReplaceNth(SInt32 n, T* item) 
	{
		Item* pReplaced = NULL;
		NodePos nodePos = GetNthNode(n);
		if (nodePos.node && nodePos.index == n) {
			pReplaced = nodePos.node->item;
			nodePos.node->item = item;
		}
		return pReplaced;
	}

	template <class Op>
	UInt32 RemoveIf(Op& op)
	{
		return FreeNodes(op);
	}

	template <class Op>
	SInt32 GetIndexOf(Op& op)
	{
		SInt32 idx = 0;
		const _Node* pCur = Head();
		while (pCur && pCur->Item() && !op.Accept(pCur->Item()))
		{
			idx++;
			pCur = pCur->Next();
		}

		if (pCur && pCur->Item())
			return idx;
		else
			return -1;
	}

	class AcceptEqual {
	public:
		T * item;

		AcceptEqual(T * a_item) : item(a_item) {}

		bool Accept(T * a_item) {
			return *item == *a_item;
		}
	};

	bool Contains(T * item) const
	{
		return Find(AcceptEqual(item)) != NULL;
	}
	
	void	Dump(void)
	{
		_MESSAGE("tList:");
		_MESSAGE("> count: %d", Count());

		const _Node* pCur = Head();
		UInt32 i = 0;
		while (pCur)
		{
			_MESSAGE("* %d :", i);
			//_MESSAGE("\t\titem: %08X", pCur->item);
			if (pCur->item)
				_MESSAGE("\t\t*item: %d", *pCur->item);
			_MESSAGE("\t\tnext: %08X", pCur->next);

			i++;
			pCur = pCur->next;
		}
	}
};

STATIC_ASSERT(sizeof(tList<void *>) == 0x8);


typedef void (__cdecl * _CRC32_Calc4)(UInt32 * out, UInt32 data);
extern const _CRC32_Calc4 CRC32_Calc4;

typedef void (__cdecl * _CRC32_Calc8)(UInt32 * out, UInt64 data);
extern const _CRC32_Calc8 CRC32_Calc8;

// 01C
// Note: I'm pretty sure that the current implementation is not safe for refcounted Item types and the like.
template <typename Item, typename Key = Item>
class tHashSet
{
	class _Entry
	{
	public:
		Item	item;
		_Entry	* next;

		_Entry() : next(NULL) {}

		bool	IsFree() const	{ return next == NULL; }
		void	SetFree()		{ next = NULL; }

		void	Dump(void)
		{
			item.Dump();
			_MESSAGE("\t\tnext: %08X", next);
		}
	};

	// When creating a new tHashSet, init sentinel pointer with address of this entry
	static _Entry sentinel;

	UInt32		unk_000;		// 000
	UInt32		m_size;			// 004
	UInt32		m_freeCount;	// 008
	UInt32		m_freeOffset;	// 00C
	_Entry 		* m_eolPtr;		// 010
	UInt32		unk_014;		// 014
	_Entry		* m_entries;	// 018


	_Entry * GetEntry(UInt32 hash) const
	{
		return (_Entry*) (((UInt32) m_entries) + sizeof(_Entry) * (hash & (m_size - 1)));
	}

	_Entry * GetEntryAt(UInt32 index) const
	{
		return (_Entry*) (((UInt32) m_entries) + sizeof(_Entry) * index);
	}

	_Entry * NextFreeEntry(void)
	{
		_Entry * result = NULL;

		if (m_freeCount == 0)
			return NULL;

		do
		{
			m_freeOffset = (m_size - 1) & (m_freeOffset - 1);
			_Entry * entry = GetEntryAt(m_freeOffset);

			if (entry->IsFree())
				result = entry;
		}
		while (!result);

		m_freeCount--;

		return result;
	}

	enum InsertResult
	{
		kInsert_Duplicate = -1,
		kInsert_OutOfSpace = 0,
		kInsert_Success = 1
	};

	InsertResult Insert(Item * item)
	{
		if (! m_entries)
			return kInsert_OutOfSpace;

		Key k = (Key)*item;
		_Entry * targetEntry = GetEntry(Item::GetHash(&k));

		// Case 1: Target entry is free
		if (targetEntry->IsFree())
		{
			targetEntry->item = *item;
			targetEntry->next = m_eolPtr;
			--m_freeCount;

			return kInsert_Success;
		}

		// -- Target entry is already in use

		// Case 2: Item already included
		_Entry * p = targetEntry;
		do
		{
			if (p->item == *item)
				return kInsert_Duplicate;
			p = p->next;
		}
		while (p != m_eolPtr);

		// -- Either hash collision or bucket overlap

		_Entry * freeEntry = NextFreeEntry();
		// No more space?
		if (!freeEntry)
			return kInsert_OutOfSpace;

		// Original position of the entry that currently uses the target position
		k = (Key)targetEntry->item;
		p = GetEntry(Item::GetHash(&k));

		// Case 3a: Hash collision - insert new entry between target entry and successor
        if (targetEntry == p)
        {
			freeEntry->item = *item;
			freeEntry->next = targetEntry->next;
			targetEntry->next = freeEntry;

			return kInsert_Success;
        }
		// Case 3b: Bucket overlap
		else
		{
			while (p->next != targetEntry)
				p = p->next;

			freeEntry->item = targetEntry->item;
			freeEntry->next = targetEntry->next;

			p->next = freeEntry;
			targetEntry->item = *item;
			targetEntry->next = m_eolPtr;

			return kInsert_Success;
		}
	}

	bool CopyEntry(_Entry * sourceEntry)
	{
		if (! m_entries)
			return false;

		Key k = (Key)sourceEntry->item;
		_Entry * targetEntry = GetEntry(Item::GetHash(&k));

		// Case 1: Target location is unused
		if (!targetEntry->next)
		{
			targetEntry->item = sourceEntry->item;
			targetEntry->next = m_eolPtr;
			--m_freeCount;

			return true;
		}

		// Target location is in use. Either hash collision or bucket overlap.

		_Entry * freeEntry = NextFreeEntry();
		k = (Key)targetEntry->item;
		_Entry * p = GetEntry(Item::GetHash(&k));

		// Case 2a: Hash collision - insert new entry between target entry and successor
		if (targetEntry == p)
		{
			freeEntry->item = sourceEntry->item;
			freeEntry->next = targetEntry->next;
			targetEntry->next = freeEntry;

			return true;
		}

		// Case 2b: Bucket overlap - forward until hash collision is found, then insert
		while (p->next != targetEntry)
			p = p->next;

		// Source entry takes position of target entry - not completely understood yet
		freeEntry->item = targetEntry->item;
		freeEntry->next = targetEntry->next;
		p->next = freeEntry;
		targetEntry->item = sourceEntry->item;
		targetEntry->next = m_eolPtr;

		return true;
	}

	void Grow(void)
	{
		UInt32 oldSize = m_size;
		UInt32 newSize = oldSize ? 2*oldSize : 8;

		_Entry * oldEntries = m_entries;
		_Entry * newEntries = (_Entry*)FormHeap_Allocate(newSize * sizeof(_Entry));
		ASSERT(newEntries);
		
		m_entries = newEntries;
		m_size = m_freeCount = m_freeOffset = newSize;

		// Initialize new table data (clear next pointers)
		_Entry * p = newEntries;
		for (UInt32 i = 0; i < newSize; i++, p++)
			p->SetFree();

		// Copy old entries, free old table data
		if (oldEntries)
		{
			_Entry * p = oldEntries;
			for (UInt32 i = 0; i < oldSize; i++, p++)
				if (p->next)
					CopyEntry(p);
			FormHeap_Free(oldEntries);
		}
	}

public:

	tHashSet() : m_size(0), m_freeCount(0), m_freeOffset(0), m_entries(NULL), m_eolPtr(&sentinel) { }

	UInt32	Size() const		{ return m_size; }
	UInt32	FreeCount() const	{ return m_freeCount; }
	UInt32	FillCount() const	{ return m_size - m_freeCount; }

	Item * Find(Key * key) const
	{
		if (!m_entries)
			return NULL;

		_Entry * entry = GetEntry(Item::GetHash(key));
		if (! entry->next)
			return NULL;

		while (!(entry->item == *key))
		{
			entry = entry->next;
			if (entry == m_eolPtr)
				return NULL;
		}

		return &entry->item;
	}

	bool Add(Item * item)
	{
		InsertResult result;

		for (result = Insert(item); result == kInsert_OutOfSpace; result = Insert(item))
			Grow();

		return result == kInsert_Success;
	}

	bool Remove(Key * key)
	{
		if ( !m_entries)
			return false;

		_Entry * entry = GetEntry(Item::GetHash(key));
		if (! entry->next)
			return NULL;

		_Entry * prevEntry = NULL;
		while (! (entry->item == *key))
		{
			prevEntry = entry;
			entry = entry->next;
			if (entry == m_eolPtr)
				return false;
		}

		// Remove tail?
		_Entry * nextEntry = entry->next;
		if (nextEntry == m_eolPtr)
		{
			if (prevEntry)
				prevEntry->next = m_eolPtr;
			entry->next = NULL;
		}
		else
		{
			entry->item = nextEntry->item;
			entry->next = nextEntry->next;
			nextEntry->next = NULL;
		}

		++m_freeCount;
		return true;
	}

	void Clear(void)
	{
		if (m_entries)
		{
			_Entry * p = m_entries;
			for (UInt32 i = 0; i < m_size; i++, p++)
				p->next = NULL;
		}
		else
		{
			m_size = 0;
		}
		m_freeCount = m_freeOffset = m_size;
	}

	template <typename T>
	void ForEach(T& functor)
	{
		if (!m_entries)
			return;

		_Entry * cur	= m_entries;
		_Entry * end	= GetEntryAt(m_size); // one index beyond the entries data to check if we reached that point

		if (cur == end)
			return;

		if (cur->IsFree())
		{
			// Forward to first non-free entry
			do cur++;
			while (cur != end && cur->IsFree());
		}

		do
		{
			if (! functor(&cur->item))
				return;

			// Forward to next non-free entry
			do cur++;
			while (cur != end && cur->IsFree());

		} while (cur != end);
	}

	void Dump(void)
	{
		_MESSAGE("tHashSet:");
		_MESSAGE("> size: %d", Size());
		_MESSAGE("> free: %d", FreeCount());
		_MESSAGE("> filled: %d", FillCount());
		if (m_entries)
		{
			_Entry * p = m_entries;
			for (UInt32 i = 0; i < m_size; i++, p++) {
				_MESSAGE("* %d %s:", i, p->IsFree()?"(free)" : "");
				if (!p->IsFree())
					p->Dump();
			}
		}
	}
};
STATIC_ASSERT(sizeof(tHashSet<void*,void*>) == 0x1C);

template <typename Key, typename Item>
typename tHashSet<Key,Item>::_Entry tHashSet<Key,Item>::sentinel = tHashSet<Key,Item>::_Entry();

// Don't know if this really is a native type or if sometimes locks are just placed before data structures.
template <typename T>
class SafeDataHolder
{
protected:
	SimpleLock	m_lock;
public:
	T			m_data;

	void	Lock(void) { m_lock.Lock(); }
	void	Release(void) { m_lock.Release(); }
};

// 0C
// Some generic type but I haven't figured it out yet
class UpdateRegistrationHolder
{
	// 018
	class Registration
	{
	public:
		volatile SInt32	refCount;	// 00
		bool			runOnce;	// 04
		UInt8			pad05;		// 05
		UInt16			pad06;		// 06
		UInt32			schedTime;	// 08
		UInt32			interval;	// 0C
		UInt64			handle;		// 10

		void Release(void)
		{
			if (InterlockedDecrement(&refCount) == 0) FormHeap_Free(this);
		}
	};

	tArray<Registration*>	m_regs;	// 00

	void	Order(UInt32 index);

public:
	bool	Remove(UInt64 & handle);
};
