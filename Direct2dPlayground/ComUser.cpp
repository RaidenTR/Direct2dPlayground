#include "ComUser.h"

ComUser::ComUser(){
	CoInitialize(nullptr);
}

ComUser::~ComUser() {
	CoUninitialize();
}
