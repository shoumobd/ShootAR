//
//  Singleton.h
//  XRSDKEnvironment
//
//  Created by Tim Mowrer on 12/6/17.
//  Copyright © 2017 Unity. All rights reserved.
//

#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <array>

template<typename T>
class Singleton
{
    static T* s_Instance;
    
public:
    
    template<typename ...Args>
    static void Construct(Args&&... args)
    {
        if (s_Instance != nullptr)
            return;
        
        s_Instance = new T(std::forward<Args>(args)...);
    }
    
    static void Destroy()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }
    
    static T* GetInstance() { return s_Instance; }
};

template<typename T> T* Singleton<T>::s_Instance = nullptr;
