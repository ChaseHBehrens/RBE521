/**********************************************************************
 Copyright (c) 2020-2023, Unitree Robotics.Co.Ltd. All rights reserved.
***********************************************************************/
#include "Gait/WaveGenerator.h"
#include <iostream>
#include <sys/time.h>
#include <math.h>

/**
 * Constructor: Initializes gait parameters.
 * @param period Total time for one full step cycle (stance + swing).
 * @param stancePhaseRatio The fraction of the period spent on the ground (0.0 to 1.0).
 * @param bias Phase offset for each leg (0.0 to 1.0) to create different gaits (trot, pace, etc.).
 */
WaveGenerator::WaveGenerator(double period, double stancePhaseRatio, Vec4 bias)
    : _period(period), _stRatio(stancePhaseRatio), _bias(bias)
{
    // Validate stance ratio: must be a fraction between 0 and 1
    if ((_stRatio >= 1) || (_stRatio <= 0))
    {
        std::cout << "[ERROR] The stancePhaseRatio of WaveGenerator should between (0, 1)" << std::endl;
        exit(-1);
    }

    // Validate leg biases: must be between 0 and 1
    for (int i(0); i < bias.rows(); ++i)
    {
        if ((bias(i) > 1) || (bias(i) < 0))
        {
            std::cout << "[ERROR] The bias of WaveGenerator should between [0, 1]" << std::endl;
            exit(-1);
        }
    }

    _startT = getSystemTime();      // Record start time for phase tracking
    _contactPast.setZero();         // Initialize previous contact state to zero
    _phasePast << 0.5, 0.5, 0.5, 0.5;
    _statusPast = WaveStatus::SWING_ALL; // Default starting state
}

WaveGenerator::~WaveGenerator()
{
}

/**
 * Main calculation function to determine if legs should be on the ground and their current phase.
 */
void WaveGenerator::calcContactPhase(Vec4 &phaseResult, VecInt4 &contactResult, WaveStatus status)
{
    // 1. Calculate the theoretical current wave based on system time
    calcWave(_phase, _contact, status);

    // 2. Handle state transitions (e.g., switching from Walking to Standing)
    if (status != _statusPast)
    {
        // If a transition just started, flag all legs for a "switch" check
        if (_switchStatus.sum() == 0)
        {
            _switchStatus.setOnes();
        }
        
        // Calculate what the phase/contact *would* have been under the previous status
        calcWave(_phasePast, _contactPast, _statusPast);
        
        // Special logic for instant transitions between global stance and global swing
        if ((status == WaveStatus::STANCE_ALL) && (_statusPast == WaveStatus::SWING_ALL))
        {
            _contactPast.setOnes();
        }
        else if ((status == WaveStatus::SWING_ALL) && (_statusPast == WaveStatus::STANCE_ALL))
        {
            _contactPast.setZero();
        }
    }

    // 3. Smoothing/Transition Logic: Ensures legs finish their current motion 
    // before switching to a new gait pattern to prevent jerky movements.
    if (_switchStatus.sum() != 0)
    {
        for (int i(0); i < 4; ++i)
        {
            // If the leg's target contact matches its current contact, it has finished transitioning
            if (_contact(i) == _contactPast(i))
            {
                _switchStatus(i) = 0;
            }
            else
            {
                // Force the leg to stay in its "Past" state until the wave naturally flips
                _contact(i) = _contactPast(i);
                _phase(i) = _phasePast(i);
            }
        }
        
        // Once all 4 legs have synchronized with the new status, update statusPast
        if (_switchStatus.sum() == 0)
        {
            _statusPast = status;
        }
    }

    // Output results
    phaseResult = _phase;
    contactResult = _contact;
}

float WaveGenerator::getTstance() { return _period * _stRatio; }
float WaveGenerator::getTswing()  { return _period * (1 - _stRatio); }
float WaveGenerator::getT()       { return _period; }

/**
 * Core Waveform Logic: Maps time to a 0.0-1.0 phase and determines contact.
 */
void WaveGenerator::calcWave(Vec4 &phase, VecInt4 &contact, WaveStatus status)
{
    // Standard walking/running gait
    if (status == WaveStatus::WAVE_ALL)
    {
        // Calculate elapsed time in seconds
        _passT = (double)(getSystemTime() - _startT) * 1e-6;
        
        for (int i(0); i < 4; ++i)
        {
            // Normalize time within the period [0, 1] and apply the leg's unique bias (offset)
            _normalT(i) = fmod(_passT + _period - _period * _bias(i), _period) / _period;
            
            // If normalized time is within the stance ratio, leg is on the ground
            if (_normalT(i) < _stRatio)
            {
                contact(i) = 1; 
                // Phase goes from 0.0 to 1.0 specifically for the stance duration
                phase(i) = _normalT(i) / _stRatio;
            }
            else
            {
                // Leg is in the air (swing)
                contact(i) = 0;
                // Phase goes from 0.0 to 1.0 specifically for the swing duration
                phase(i) = (_normalT(i) - _stRatio) / (1 - _stRatio);
            }
        }
    }
    // All legs in air
    else if (status == WaveStatus::SWING_ALL)
    {
        contact.setZero();
        phase << 0.5, 0.5, 0.5, 0.5;
    }
    // All legs on ground
    else if (status == WaveStatus::STANCE_ALL)
    {
        contact.setOnes();
        phase << 0.5, 0.5, 0.5, 0.5;
    }
}
