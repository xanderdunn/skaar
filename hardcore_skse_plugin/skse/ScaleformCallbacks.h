#pragma once

#include "skse/ScaleformAPI.h"
#include "skse/ScaleformTypes.h"
#include "skse/Utilities.h"
#include <typeinfo>
#include <map>

// see RakNet/DependentExtensions/GFx3/FxGameDelegate.h

class FxDelegateHandler;
class GFxMovieView;
class GFxMovieRoot;

// the double in GFxValue is causing very weird alignment issues
#pragma pack (push, 4)

// 10
class GFxValue
{
public:
	GFxValue()
		:objectInterface(NULL), type(kType_Undefined) { }
	~GFxValue();

	enum
	{
		kType_Undefined =	0,
		kType_Null,
		kType_Bool,
		kType_Number,
		kType_String,
		kType_WideString,
		kType_Object,
		kType_Array,
		kType_DisplayObject,

		kTypeFlag_Managed =	1 << 6,

		kMask_Type =		0x8F,	// not sure why it checks the top bit
	};

	union Data
	{
		double			number;
		bool			boolean;
		const char		* string;
		const char		** managedString;
		const wchar_t	* wideString;
		const wchar_t	** managedWideString;
		void			* obj;
	};

#pragma pack (push, 8)
	class DisplayInfo
	{
	public:
		DisplayInfo() : _varsSet(0) {}
		enum
		{
			kChange_x				= (1 << 0),
			kChange_y				= (1 << 1),
			kChange_rotation		= (1 << 2),
			kChange_xscale			= (1 << 3),
			kChange_yscale			= (1 << 4),
			kChange_alpha			= (1 << 5),
			kChange_visible			= (1 << 6),
			kChange_z				= (1 << 7),
			kChange_xrotation		= (1 << 8),
			kChange_yrotation		= (1 << 9),
			kChange_zscale			= (1 << 10),
			kChange_FOV				= (1 << 11),
			kChange_projMatrix3D	= (1 << 12),
			kChange_viewMatrix3D	= (1 << 13)
		};

		double		_x;
		double		_y;
		double		_rotation;
		double		_xScale;
		double		_yScale;
		double		_alpha;
		bool		_visible;
		double		_z;
		double		_xRotation;
		double		_yRotation;
		double		_zScale;
		double		_perspFOV;
		GMatrix3D	_viewMatrix3D;
		GMatrix3D	_perspectiveMatrix3D;
		UInt16		_varsSet;

		void SetX(double x)					{ SetFlags(kChange_x); _x = x; }
		void SetY(double y)					{ SetFlags(kChange_y); _y = y; }
		void SetRotation(double degrees)	{ SetFlags(kChange_rotation); _rotation = degrees; }
		void SetXScale(double xscale)		{ SetFlags(kChange_xscale); _xScale = xscale; }
		void SetYScale(double yscale)		{ SetFlags(kChange_yscale); _yScale = yscale; }
		void SetAlpha(double alpha)			{ SetFlags(kChange_alpha); _alpha = alpha; }
		void SetVisible(bool visible)		{ SetFlags(kChange_visible); _visible = visible; }
		void SetZ(double z)					{ SetFlags(kChange_z); _z = z; }
		void SetXRotation(double degrees)	{ SetFlags(kChange_xrotation); _xRotation = degrees; }
		void SetYRotation(double degrees)	{ SetFlags(kChange_yrotation); _yRotation = degrees; }
		void SetZScale(double zscale)		{ SetFlags(kChange_zscale); _zScale = zscale; }
		void SetFOV(double fov)				{ SetFlags(kChange_FOV); _perspFOV = fov; }
		void SetProjectionMatrix3D(const GMatrix3D *pmat)  
		{ 
			if (pmat) {
				SetFlags(kChange_projMatrix3D);
				_perspectiveMatrix3D = *pmat;
			} else
				ClearFlags(kChange_projMatrix3D); 
		}
		void SetViewMatrix3D(const GMatrix3D *pmat) 
		{ 
			if (pmat) { 
				SetFlags(kChange_viewMatrix3D); 
				_viewMatrix3D = *pmat;
			} else
				ClearFlags(kChange_viewMatrix3D); 
		}

		// Convenience functions
		void SetPosition(double x, double y) { SetFlags(kChange_x | kChange_y); _x = x; _y = y; }
		void SetScale(double xscale, double yscale)  { SetFlags(kChange_xscale | kChange_yscale); _xScale = xscale; _yScale = yscale; }

		void SetFlags(UInt32 flags) { _varsSet |= flags; }
		void ClearFlags(UInt32 flags) { _varsSet &= ~flags; }
	};
#pragma pack (pop)

	// 4
	class ObjectInterface
	{
	public:
		GFxMovieRoot	* root;

		MEMBER_FN_PREFIX(ObjectInterface);
		DEFINE_MEMBER_FN(HasMember, bool, 0x00920C50, void * obj, const char * name, bool isDisplayObj);
		DEFINE_MEMBER_FN(SetMember, bool, 0x00920D20, void * obj, const char * name, GFxValue * value, bool isDisplayObj);
		DEFINE_MEMBER_FN(DeleteMember, bool, 0x00920DE0, void * obj, const char * name, bool isDisplayObj);
		DEFINE_MEMBER_FN(GetMember, bool, 0x00923960, void * obj, const char * name, GFxValue * value, bool isDisplayObj);
		DEFINE_MEMBER_FN(Invoke, bool, 0x00922ED0, void * obj, GFxValue * result, const char * name, GFxValue * args, UInt32 numArgs, bool isDisplayObj);
		DEFINE_MEMBER_FN(AttachMovie, bool, 0x00923060, void * obj, GFxValue * value, const char * symbolName, const char * instanceName, SInt32 depth, void * initArgs);
		DEFINE_MEMBER_FN(PushBack, bool, 0x00920EF0, void * obj, GFxValue * value);
		DEFINE_MEMBER_FN(SetText, bool, 0x009233A0, void * obj, const wchar_t * text, bool html);
		//DEFINE_MEMBER_FN(PopBack, bool, 0x00000000, void * obj, GFxValue * value);
		DEFINE_MEMBER_FN(GetArraySize, UInt32, 0x00920E80, void * obj);
		//DEFINE_MEMBER_FN(SetArraySize, bool, 0x00000000, void * obj, UInt32 size);
		DEFINE_MEMBER_FN(GetElement, bool, 0x00923AD0, void * obj, UInt32 index, GFxValue * value);
		//DEFINE_MEMBER_FN(SetElement, bool, 0x00000000, void * obj, UInt32 index, GFxValue * value);
		//DEFINE_MEMBER_FN(GotoLabeledFrame, bool, 0x00000000, void * obj, const char * frameLabel, bool stop);
		//DEFINE_MEMBER_FN(GotoFrame, bool, 0x00000000, void * obj, UInt32 frameNumber, bool stop);
		DEFINE_MEMBER_FN(GetDisplayInfo, bool, 0x00920F90, void * obj, DisplayInfo * displayInfo);
		DEFINE_MEMBER_FN(SetDisplayInfo, bool, 0x009211B0, void * obj, DisplayInfo * displayInfo);

		DEFINE_MEMBER_FN(AddManaged_Internal, void, 0x00921DA0, GFxValue * value, void * obj);
		DEFINE_MEMBER_FN(ReleaseManaged_Internal, void, 0x00922660, GFxValue * value, void * obj);
	};

	ObjectInterface	* objectInterface;	// 00
	UInt32			type;				// 04
	Data			data;				// 08

	UInt32	GetType(void) const		{ return type & kMask_Type; }
	bool	IsManaged(void) const	{ return (type & kTypeFlag_Managed) != 0; }
	void	CleanManaged(void);
	void	AddManaged(void);

	bool	IsObject(void) const		{ return GetType() == kType_Object; }
	bool	IsDisplayObject(void) const	{ return GetType() == kType_DisplayObject; }

	bool			GetBool(void);
	const char *	GetString(void);
	const wchar_t *	GetWideString(void);
	double			GetNumber(void);

	void	SetUndefined(void);
	void	SetNull(void);
	void	SetBool(bool value);
	void	SetNumber(double value);
	void	SetString(const char * value);
	void	SetWideString(const wchar_t * value);

	UInt32	GetArraySize();
	bool	GetElement(UInt32 index, GFxValue * value);
	bool	HasMember(const char * name);
	bool	SetMember(const char * name, GFxValue * value);
	bool	GetMember(const char * name, GFxValue * value);
	bool	DeleteMember(const char * name);
	bool	Invoke(const char * name, GFxValue * result, GFxValue * args, UInt32 numArgs);
	bool	PushBack(GFxValue * value);
	bool	GetDisplayInfo(DisplayInfo * displayInfo);
	bool	SetDisplayInfo(DisplayInfo * displayInfo);
};

STATIC_ASSERT(sizeof(GFxValue) == 0x10);

#pragma pack (pop)

class GFxFunctionHandler : public GRefCountBase
{
public:
	GFxFunctionHandler();
	virtual ~GFxFunctionHandler();

	// 1C
	class Args
	{
	public:
		GFxValue		* result;	// 00
		GFxMovieView	* movie;	// 04
		GFxValue		* thisObj;	// 08
		GFxValue		* unk0C;	// 0C
		GFxValue		* args;		// 10
		UInt32			numArgs;	// 14
		void			* refCon;	// 18
	};

	virtual void	Invoke(Args * args) = 0;

	MEMBER_FN_PREFIX(GFxFunctionHandler);
	DEFINE_MEMBER_FN(Destroy, GFxFunctionHandler *, 0x00859490, UInt32 flags);
};

typedef std::map <const std::type_info *, GFxFunctionHandler *>	FunctionHandlerCache;
extern FunctionHandlerCache g_functionHandlerCache;

template <typename T>
void RegisterFunction(GFxValue * dst, GFxMovieView * movie, const char * name)
{
	// either allocate the object or retrieve an existing instance from the cache
	GFxFunctionHandler	* fn = NULL;

	// check the cache
	FunctionHandlerCache::iterator iter = g_functionHandlerCache.find(&typeid(T));
	if(iter != g_functionHandlerCache.end())
		fn = iter->second;

	if(!fn)
	{
		// not found, allocate a new one
		fn = new T;

		// add it to the cache
		// cache now owns the object as far as refcounting goes
		g_functionHandlerCache[&typeid(T)] = fn;
	}

	// create the function object
	GFxValue	fnValue;
	movie->CreateFunction(&fnValue, fn);

	// register it
	dst->SetMember(name, &fnValue);
}

// 04
class FxResponseArgsBase
{
public:
	FxResponseArgsBase();
	virtual ~FxResponseArgsBase();

	virtual UInt32	GetValues(GFxValue ** params);

//	void	** _vtbl;	// 00
};

// 0C + T_numArgs * 0x10
template <UInt32 T_numArgs>
class FxResponseArgs : public FxResponseArgsBase
{
public:
	FxResponseArgs();
	virtual ~FxResponseArgs();

	UInt32		align04;				// 04 - needed because GFxValue contains a double
	GFxValue	args[T_numArgs + 1];	// 08
	UInt32		curArg;					// 28 - offsets assume one argument
};

// ?
class FxResponseArgsList : public FxResponseArgsBase
{
public:
	FxResponseArgsList();

	GArray <GFxValue>	args;
};

// 20
class FxDelegateArgs
{
public:
	FxDelegateArgs();
	~FxDelegateArgs();

	GFxValue			responseID;
	FxDelegateHandler	* menu;		// 10
	GFxMovieView		* movie;	// 14
	GFxValue			* args;		// 18
	UInt32				numArgs;	// 1C
};

// 08
class FxDelegateHandler : public GRefCountBase
{
public:
	//FxDelegateHandler();
	//~FxDelegateHandler();

	typedef void (* Callback)(const FxDelegateArgs & params);

	class CallbackProcessor
	{
	public:
		CallbackProcessor();
		virtual ~CallbackProcessor();

		virtual void	Process(const GString & name, Callback method);
	};

	class AddCallbackVisitor : public CallbackProcessor
	{
	public:
		AddCallbackVisitor();
		virtual ~AddCallbackVisitor();

		FxDelegateHandler	* parent;
		void				* callbackHashTable;
	};

	virtual void	Accept(CallbackProcessor * processor) = 0;
};

extern const FxDelegateHandler::Callback PlaySoundCallback;