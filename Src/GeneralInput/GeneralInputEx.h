// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#ifndef _EXTERN_GENERAL_INPUT_H_
#define _EXTERN_GENERAL_INPUT_H_

#if defined _AFXDLL && !defined _APROJECT_STATIC_
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"GeneralInputU.lib") 
			#pragma message("Automatically linking with GeneralInputU.dll")
		#else
			#pragma comment(lib,"GeneralInputD.lib") 
			#pragma message("Automatically linking with GeneralInputD.dll")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"GeneralInputU.lib") 
			#pragma message("Automatically linking with GeneralInputU.dll") 
		#else
			#pragma comment(lib,"GeneralInput.lib") 
			#pragma message("Automatically linking with GeneralInput.dll") 
		#endif
	#endif
#endif

#include "GeneralInput.h"

#endif