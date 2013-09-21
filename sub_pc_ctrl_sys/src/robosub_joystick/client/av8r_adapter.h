#ifndef __AV8R_ADAPTER_H__
#define __AV8R_ADAPTER_H__
///////////////////////////////////////////////////////////////////////////////
// \file av8r_adapter.h
///////////////////////////////////////////////////////////////////////////////
// Author:      Jay Sidhu (JS) <jaypal.sidhu@gmail.com>
// Project:     RoboSub Joystick Communication
// Description: Implementation class for adapting av8r joystick inputs to 
//              robosub AUV commands
//
// Major Changes:
// 25-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "joystick_adapter_interface.h"
#include "av8r.h"

#include <vector>
#include <map>
#include <utility>


// Joystick Adapter Interface
// \brief interface for applying adapters to joystick axes and buttons
template <class T>
class Av8rAdapter : public JoystickAdapterInterface<T>
{
public:

    // Default Constructor
    Av8rAdapter( Av8r& joystick );

    // Destructor
    virtual ~Av8rAdapter();
                                    
    // MapToButton
    // \brief maps a variable or oject to the specified button
    // \param btnId, the button number to bind
    // \param target, the object that is mapped to the button
    // \param adapter, the operation that is applied to the button
    //                 value prior to assigning it to target
    virtual void MapToButton( unsigned int btnId, T& target,
                        AdapterFunctor<int,T>& adapter );
                              
    // MapToAxis
    // \brief maps a variable or oject to the specified axis
    // \param axisId, the axis number to bind
    // \param target, the object that is mapped to the axis
    // \param adapter, the operation that is applied to the axis
    //                 value prior to assigning it to target
    virtual void MapToAxis( unsigned int axisId, T& target,
                        AdapterFunctor<int,T>& adapter );

    // Update
    // \brief Gets latest joytsick data and updates mapped targets
    virtual void Update();

private:

    // Simplify Type Names:
    typedef std::pair< T*, AdapterFunctor<int, T>*> AdapterPair;
    typedef std::vector< AdapterPair >              AdapterPairVector;
    typedef std::map< int, AdapterPairVector >      ButtonMapGroup;
    typedef std::map< int, AdapterPairVector >      AxisMapGroup;

    // Declare Private Members
    ButtonMapGroup _ButtonMappings;
    AxisMapGroup   _AxisMappings;
    Av8r&          _Joystick;

    // Garbage collection:
    std::vector<AdapterPair *> _AllocatedPairs;
};

/////////////////
// Method Defs //
/////////////////
template <class T>
Av8rAdapter<T>::Av8rAdapter(Av8r& joystick)
    :
    JoystickAdapterInterface<T>(),
    _ButtonMappings(),
    _AxisMappings(),
    _Joystick(joystick),
    _AllocatedPairs()
{ }

template <class T>
Av8rAdapter<T>::~Av8rAdapter()
{ 
    // free allocated pairs
    typename std::vector<AdapterPair *>::iterator it;
    
    for( it = _AllocatedPairs.begin(); it != _AllocatedPairs.end(); ++it )
    {
        delete (*it);
    }
}

template <class T>
void Av8rAdapter<T>::MapToButton( unsigned int btnId, T& target,
                               AdapterFunctor<int,T>& adapter ){
    
    // Allocate the target-adapter pair
    AdapterPair* aP = new AdapterPair;

    aP->first  = &target;
    aP->second = &adapter;

    // Create the mapping
    namespace ae = Av8rEvents;
    if( btnId < ae::NUM_BUTTONS ){

        // Add to button mapping
        _ButtonMappings[btnId].push_back(*aP);

        // Add to garbage collector
        _AllocatedPairs.push_back(aP);
    }
}

template <class T>
void Av8rAdapter<T>::MapToAxis( unsigned int axisId, T& target,
                             AdapterFunctor<int,T>& adapter ){

    // Allocate the target-adapter pair
    AdapterPair* aP = new AdapterPair;
    aP->first  = &target;
    aP->second =  &adapter;

    // Create the mapping
    namespace ae = Av8rEvents;
    if( axisId < ae::NUM_AXES ){

        // Add to axis mapping
        _AxisMappings[axisId].push_back(*aP);

        // Add to garbage collector
        _AllocatedPairs.push_back(aP);
    }
}

template <class T>
void Av8rAdapter<T>::Update() {

    // Call Joystick's Update method
    _Joystick.Update();

    // Update all mappings
    namespace ae = Av8rEvents;

    // Update Button Mappings
    typename ButtonMapGroup::iterator bmgIt;
    for( bmgIt = _ButtonMappings.begin(); 
         bmgIt != _ButtonMappings.end(); ++bmgIt ) {
        
        // Get Button state and vector iterator
        int btnState = _Joystick.Buttons[(*bmgIt).first];
        typename AdapterPairVector::iterator apvIt;

        for( apvIt = (*bmgIt).second.begin(); 
             apvIt != (*bmgIt).second.end(); ++apvIt ){
        
            // Get the target and the button adapter
            T* target = (*apvIt).first;
            AdapterFunctor<int,T>* adapter = (*apvIt).second;
            
            // Update target with value from adapter functor
            *target = (*adapter)(btnState);
        }
    }
    
    // Update Axis Mappings
    typename AxisMapGroup::iterator amgIt;
    for( amgIt = _AxisMappings.begin(); 
         amgIt != _AxisMappings.end(); ++amgIt ) {
        
        // Get Axis state and vector iterator
        int axisState = _Joystick.Axes[(*amgIt).first];
        typename AdapterPairVector::iterator apvIt;

        for( apvIt = (*amgIt).second.begin(); 
             apvIt != (*amgIt).second.end(); ++apvIt ){
        
            // Get the target and the axis adapter
            T* target = (*apvIt).first;
            AdapterFunctor<int,T>* adapter = (*apvIt).second;
            
            // Update target with value from adapter functor
            *target = (*adapter)(axisState);
        }
    }
}

#endif //__AV8R_ADAPTER_H__
