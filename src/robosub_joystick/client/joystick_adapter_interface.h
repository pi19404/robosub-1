#ifndef __JOYSTICK_ADAPTER_INTERFACE_H__
#define __JOYSTICK_ADAPTER_INTERFACE_H__
///////////////////////////////////////////////////////////////////////////////
// \file joystick_adapter_interface.h
///////////////////////////////////////////////////////////////////////////////
// Author:      Jay Sidhu (JS) <jaypal.sidhu@gmail.com>
// Project:     RoboSub Joystick Communication
// Description: Specifies the interface for adapting joystick inputs to 
//              robosub AUV commands
//
// Major Changes:
// 25-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////

// Adapter Function Object
// \brief applied to a a value type
//        to form another value type
template <class From, class To>
class AdapterFunctor
{
public:
    AdapterFunctor(){ }
    virtual ~AdapterFunctor(){ }
    virtual To operator()(From val) = 0;
};

// Joystick Adapter Interface
// \brief interface for applying adapters to joystick axes and buttons
template <class T>
class JoystickAdapterInterface
{
public:

    // Default Constructor
    JoystickAdapterInterface(){ }

    // Destructor
    virtual ~JoystickAdapterInterface(){ }
                                    
    // MapToButton
    // \brief maps a variable or oject to the specified button
    // \param btnId, the button number to bind
    // \param target, the object that is mapped to the button
    // \param adapter, the operation that is applied to the button
    //                 value prior to assigning it to target
    virtual void MapToButton( unsigned int btnId, T& target,
                        AdapterFunctor<int,T>& adapter ) = 0;
                              
    // MapToAxis
    // \brief maps a variable or oject to the specified axis
    // \param axisId, the axis number to bind
    // \param target, the object that is mapped to the axis
    // \param adapter, the operation that is applied to the axis
    //                 value prior to assigning it to target
    virtual void MapToAxis( unsigned int axisId, T& target,
                        AdapterFunctor<int,T>& adapter ) = 0;

    // Update
    // \brief Gets latest joytsick data and updates mapped targets
    virtual void Update() = 0;
};

#endif //__JOYSTICK_ADAPTER_INTERFACE_H__
