#include "SkaarUtilities.h"
#include <iostream>
#include <stdlib.h>

namespace SkaarUtilities {

	UInt32 randomArrayValue(const UInt32 myArray[], int size) {
		int randomIndex = rand() % size;
		_MESSAGE("randomIndex = %i", randomIndex);

		return myArray[randomIndex];
	}
}
