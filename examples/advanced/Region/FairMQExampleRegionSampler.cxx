/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * FairMQExampleRegionSampler.cpp
 *
 * @since 2014-10-10
 * @author A. Rybalchenko
 */

#include "FairMQExampleRegionSampler.h"
#include "FairMQLogger.h"
#include "FairMQProgOptions.h" // device->fConfig

#include <thread>
#include <atomic>

using namespace std;

FairMQExampleRegionSampler::FairMQExampleRegionSampler()
    : fMsgSize(10000)
    , fRegion(nullptr)
    , fNumUnackedMsgs(0)
{
}

void FairMQExampleRegionSampler::InitTask()
{
    fMsgSize = fConfig->GetValue<int>("msg-size");

    fRegion = FairMQUnmanagedRegionPtr(NewUnmanagedRegionFor("data",
                                                             0,
                                                             10000000,
                                                             [this](void* data, size_t size) { --fNumUnackedMsgs; } // callback to be called when message buffers no longer needed by transport
                                                             ));
}

bool FairMQExampleRegionSampler::ConditionalRun()
{
    FairMQMessagePtr msg(NewMessageFor("data", // channel
                                        0, // sub-channel
                                        fRegion, // region
                                        fRegion->GetData(), // ptr within region
                                        fMsgSize // offset from ptr
                                        ));
    if (Send(msg, "data", 0) > 0)
    {
        ++fNumUnackedMsgs;
    }

    return true;
}

void FairMQExampleRegionSampler::ResetTask()
{
    // if not all messages acknowledged, wait for a bit. But only once, since receiver could be already dead.
    if (fNumUnackedMsgs != 0)
    {
        LOG(DEBUG) << "waiting for all acknowledgements... (" << fNumUnackedMsgs << ")";
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    fRegion.reset();
}

FairMQExampleRegionSampler::~FairMQExampleRegionSampler()
{
}