//
//  XRProvider.h
//  XRSDKEnvironment
//
//  Created by Tim Mowrer on 12/6/17.
//  Copyright © 2017 Unity. All rights reserved.
//

#pragma once
#include "Singleton.h"

template<typename T, typename T_UNITY_PROVIDER>
class XRProvider
    : public T_UNITY_PROVIDER
    , public Singleton<T>
{ };