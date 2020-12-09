// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

HPS::SelectAreaOperator::SelectAreaOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: ConstructRectangleOperator(in_mouse_trigger, in_modifier_trigger, true)
{
	selection_options.SetRelatedLimit(std::numeric_limits<int>::max())
		.SetLevel(HPS::Selection::Level::Entity)
		.SetSorting(Selection::Sorting::Off);
}

void HPS::SelectAreaOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	ConstructRectangleOperator::OnViewAttached(in_attached_view);
}

bool HPS::SelectAreaOperator::OnMouseUp(MouseState const  & in_state)
{
	if(!ConstructRectangleOperator::OnMouseUp(in_state))
		return false;

	HPS::WindowKey window = in_state.GetEventSource();
	return SelectCommon(window, in_state.GetModifierKeys());
}

bool HPS::SelectAreaOperator::OnTouchUp(TouchState const  & in_state)
{
	if(!ConstructRectangleOperator::OnTouchUp(in_state))
		return false;

	HPS::WindowKey window = in_state.GetEventSource();
	return SelectCommon(window, in_state.GetModifierKeys());
}

bool HPS::SelectAreaOperator::SelectCommon(HPS::WindowKey & in_window, HPS::ModifierKeys in_modifiers)
{
	if(IsRectangleValid())
	{
		try
		{
			HPS::SelectionResults new_selection;
			size_t selected = in_window.GetSelectionControl().SelectByArea(GetWindowRectangle(), selection_options, new_selection);

			if(active_selection.GetCount() > 0 && in_modifiers.Control() && selected > 0)
				active_selection.Union(new_selection);
			else
				active_selection = new_selection;
		}
		catch(HPS::InvalidObjectException const &)
		{
			//do nothing
		}
		GetAttachedView().Update();
		return true;
	}
	else
	{
		GetAttachedView().Update();
		return false;
	}
}


HPS::SelectionResults HPS::SelectAreaOperator::GetActiveSelection() const
{
	return active_selection;
}



