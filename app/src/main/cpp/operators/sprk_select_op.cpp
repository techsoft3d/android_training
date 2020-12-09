// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::SelectOperator::SelectOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger)
{
	selection_options.SetRelatedLimit(0).SetLevel(HPS::Selection::Level::Entity);
}

bool HPS::SelectOperator::OnMouseDown(MouseState const  & in_state)
{
	if (IsMouseTriggered(in_state))
	{
		HPS::WindowKey window = in_state.GetEventSource();
		return SelectCommon(in_state.GetLocation(), window, in_state.GetModifierKeys());
	}
	return false;
}

bool HPS::SelectOperator::OnTouchDown(TouchState const  & in_state)
{
	HPS::WindowKey window = in_state.GetEventSource();
	return SelectCommon(in_state.GetTouches()[0].Location, window, in_state.GetModifierKeys());
}

bool HPS::SelectOperator::SelectCommon(HPS::Point const & in_loc, HPS::WindowKey & in_window, HPS::ModifierKeys in_modifiers)
{
	try
	{
		HPS::SelectionResults new_selection;
		size_t selected = in_window.GetSelectionControl().SelectByPoint(in_loc, selection_options, new_selection);

		if (active_selection.GetCount() > 0 && in_modifiers.Control() && selected > 0)
		{
			if (!active_selection.SymmetricDifference(new_selection))
				active_selection = new_selection;
		}
		else
			active_selection = new_selection;
	}
	catch(HPS::InvalidObjectException const &)
	{
		//do nothing
	}

	return true;
}

HPS::SelectionResults HPS::SelectOperator::GetActiveSelection() const
{
	return active_selection;
}




