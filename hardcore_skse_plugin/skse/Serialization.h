#pragma once

#include "PluginAPI.h"
#include "GameTypes.h"

extern SKSESerializationInterface	g_SKSESerializationInterface;

namespace Serialization
{
	struct PluginCallbacks
	{
		PluginCallbacks()
			:revert(NULL)
			,save(NULL)
			,load(NULL)
			,formDelete(NULL)
			,uid(0)
			,hadData(false)
			,hadUID(false) { }

		SKSESerializationInterface::EventCallback	revert;
		SKSESerializationInterface::EventCallback	save;
		SKSESerializationInterface::EventCallback	load;
		SKSESerializationInterface::FormDeleteCallback	formDelete;
		
		UInt32	uid;

		bool	hadData;
		bool	hadUID;
	};

	// plugin API
	void	SetUniqueID(PluginHandle plugin, UInt32 uid);
	void	SetRevertCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback);
	void	SetSaveCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback);
	void	SetLoadCallback(PluginHandle plugin, SKSESerializationInterface::EventCallback callback);
	void	SetFormDeleteCallback(PluginHandle plugin, SKSESerializationInterface::FormDeleteCallback callback);

	void	SetSaveName(const char * name);
	bool	WriteRecord(UInt32 type, UInt32 version, const void * buf, UInt32 length);
	bool	OpenRecord(UInt32 type, UInt32 version);
	bool	WriteRecordData(const void * buf, UInt32 length);

	bool	GetNextRecordInfo(UInt32 * type, UInt32 * version, UInt32 * length);
	UInt32	ReadRecordData(void * buf, UInt32 length);

	bool	ResolveHandle(UInt64 handle, UInt64 * handleOut);

	// internal event handlers
	void	HandleRevertGlobalData(void);
	void	HandleSaveGlobalData(void);
	void	HandleLoadGlobalData(void);

	void	HandleDeleteSave(std::string saveName);
	void	HandleDeletedForm(UInt64 handle);

	// template helper functions
	template <typename T>
	bool WriteData(SKSESerializationInterface * intfc, const T * data)
	{
		return intfc->WriteRecordData(data, sizeof(T));
	}

	template <typename T>
	bool ReadData(SKSESerializationInterface * intfc, T * data)
	{
		return intfc->ReadRecordData(data, sizeof(T)) > 0;
	}

	template <> bool WriteData<BSFixedString>(SKSESerializationInterface * intfc, const BSFixedString * data);
	template <> bool ReadData<BSFixedString>(SKSESerializationInterface * intfc, BSFixedString * data);
}
