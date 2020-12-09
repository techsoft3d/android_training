// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::ZoomFitTouchOperator::ZoomFitTouchOperator() : HPS::Operator(), tracked_touch_ID(-1)
{}

bool HPS::ZoomFitTouchOperator::OnTouchDown(const HPS::TouchState &in_state)
{
    TouchArray touches = in_state.GetActiveEvent().Touches;
    
    if (touches[0].TapCount == 2 && tracked_touch_ID == -1)
	{
		Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
        GetAttachedView().FitWorld().Update();
	}
    
    return true;
}

bool HPS::ZoomFitTouchOperator::OnTouchUp(const HPS::TouchState &in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;
    
    for (size_t i = 0, e= up_touches.size(); i < e && tracked_touch_ID != -1; ++i)
    {
        if (up_touches[i].ID == tracked_touch_ID)
            tracked_touch_ID = -1;
    }
	return true;
}
