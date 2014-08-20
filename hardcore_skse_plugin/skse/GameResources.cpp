#include "GameResources.h"

FaceGenTriDatabase * FaceGenTriDatabase::GetSingleton(void)
{
	return *((FaceGenTriDatabase **)0x01B1A494);
}

BSFaceGenDB::TRI::DBTraits::Data * FaceGenTriDatabase::GetInvalidEntry(void)
{
	return (BSFaceGenDB::TRI::DBTraits::Data *)0x0125CA9C;
}