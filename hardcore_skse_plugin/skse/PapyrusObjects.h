#pragma once

#include <vector>

///
/// SKSEObjectHandle
///
class SKSEObjectHandle
{
public:
	typedef UInt32	RawT;
	typedef UInt16	TypeT;
	typedef	UInt16	IndexT;

	static const SKSEObjectHandle	NullHandle;

	SKSEObjectHandle() : data_(NullHandle.data_.raw) {}
	SKSEObjectHandle(TypeT type, IndexT index) : data_(type, index) {}
	// For implicit cast from RawT
	SKSEObjectHandle(RawT raw) : data_(raw) {}

	inline TypeT	Type() const	{ return data_.type; }
	inline IndexT	Index() const	{ return data_.index; }
	inline bool		IsValid() const	{ return data_.raw != NullHandle.data_.raw; }

	inline operator	RawT() const	{ return data_.raw; }

	inline bool operator==(RawT raw) const							{ return data_.raw == raw; }
	inline bool operator==(const SKSEObjectHandle& other) const		{ return data_.raw == other.data_.raw; }

private:
	union Data_
	{
		Data_(RawT aRaw) : raw(aRaw) {}
		Data_(TypeT aType, IndexT aIndex) : type(aType), index(aIndex) {}

		RawT		raw;
		struct
		{
			IndexT	index;
			TypeT	type;
		};
	}
	data_;
};

///
/// SKSEObjectManager
///
template <typename TObj>
class SKSEObjectManager
{
private:
	struct ObjEntry
	{
		ObjEntry() : isUsed(false) {}

		bool	isUsed;
		TObj	object;
	};

	typedef std::vector<ObjEntry>			ObjVectorT;

public:
	typedef SKSEObjectHandle::TypeT		TypeT;
	typedef SKSEObjectHandle::IndexT	IndexT;
	typedef SKSEObjectHandle::RawT		RawT;

	SKSEObjectManager() :
		curIndex_(0),
		entries_(8)
	{
	}

	TObj* CreateObject(SKSEObjectHandle& handleOut)
	{
		SKSEObjectHandle::TypeT index = nextFreeIndex();

		entries_[index].isUsed = true;
		handleOut = SKSEObjectHandle(TObj::kType, index);

		return &entries_[index].object;
	}

	TObj* GetObject(const SKSEObjectHandle& handle)
	{
		TypeT	type = handle.Type();
		IndexT	index = handle.Index();

		if (type != TObj::kType)
		{
			_WARNING("GetObject: Mismatched handle type %X, expected %X.", type, TObj::kType);
			return NULL;
		}

		if (index < entries_.size() && entries_[index].isUsed)
		{
			return &entries_[index].object;
		}
		else
		{
			_WARNING("GetObject: Tried to access invalid handle %X.", handle);
			return NULL;
		}
	}

	void FreeObject(const SKSEObjectHandle& handle)
	{
		TypeT	type = handle.Type();
		IndexT	index = handle.Index();
		
		if (type != TObj::kType)
		{
			_WARNING("FreeObject: Mismatched handle type %X, expected %X.", type, TObj::kType);
			return;
		}

		if (index < entries_.size() && entries_[index].isUsed)
		{
			entries_[index].isUsed = false;
			entries_[index].object.Reset();
		}
		else
		{
			_WARNING("FreeObject: Tried to free invalid handle %X.", handle);
		}
	}

	void FreeAll()
	{
		for (ObjVectorT::iterator it = entries_.begin(); it != entries_.end(); ++it)
		{
			it->isUsed = false;
			it->object.Reset();
		}
	}

private:
	IndexT nextFreeIndex()
	{
		IndexT		i = curIndex_;
		std::size_t	n = entries_.size();

		IndexT cycleMarker = (i - 1) % n;
		if (cycleMarker < 0)
			cycleMarker += n;

		while (entries_[i].isUsed)
		{
			// Grow
			if (i == cycleMarker)
			{
				i = n;
				n <<= 1;
				entries_.resize(n);
				break;
			}

			if (i < (n - 1))
				i++;
			else
				i = 0;
		}

		curIndex_ = (i + 1) % n;
		return i;
	}

	ObjVectorT	entries_;
	IndexT		curIndex_;
};

///
/// SKSEObject
///
template <SKSEObjectHandle::TypeT K>
class SKSEObject
{
public:
	enum	{ kType = K };

	virtual ~SKSEObject() {}

protected:
	// Objects are allocated in a vector and re-used after they have been released.
	// Reset is expected to restore the object back to its default state.
	virtual void Reset() = 0;

	template <typename U>
	friend class SKSEObjectManager;
};

typedef SKSEObjectHandle::RawT	RawHandleT;

enum
{
	kSKSEObjType_UICallback	=	1,
	kSKSEObjType_ModEvent =		2
};