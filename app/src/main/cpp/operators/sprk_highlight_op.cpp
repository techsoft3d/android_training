// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"


HPS::HighlightOperator::HighlightOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: SelectOperator(in_mouse_trigger, in_modifier_trigger), highlight_options(HighlightOptionsKit::GetDefault())
{
	highlight_options.SetStyleName("highlight_style");
	highlight_options.SetOverlay(HPS::Drawing::Overlay::InPlace);
}

bool HPS::HighlightOperator::OnMouseDown(MouseState const  & in_state)
{
	if(IsMouseTriggered(in_state) && SelectOperator::OnMouseDown(in_state))
	{
		HPS::WindowKey window = in_state.GetEventSource();
		return HighlightCommon(window, in_state.GetModifierKeys());
	}
	return false;
}

bool HPS::HighlightOperator::OnTouchDown(TouchState const  & in_state)
{
	if(!SelectOperator::OnTouchDown(in_state))
		return false;

	HPS::WindowKey window = in_state.GetEventSource();
	return HighlightCommon(window, in_state.GetModifierKeys());
}

bool HPS::HighlightOperator::HighlightCommon(HPS::WindowKey & in_window, HPS::ModifierKeys in_modifiers)
{
	HPS_UNREFERENCED(in_modifiers);
	HPS::SelectionResults results = GetActiveSelection();
	size_t selected = results.GetCount();

	in_window.GetHighlightControl().UnhighlightEverything();
	HPS::Database::GetEventDispatcher().InjectEvent(HPS::HighlightEvent(HPS::HighlightEvent::Action::Unhighlight));

	if (selected > 0)
	{
		in_window.GetHighlightControl().Highlight(results, highlight_options);
		HPS::Database::GetEventDispatcher().InjectEvent(HPS::HighlightEvent(HPS::HighlightEvent::Action::Highlight, results, highlight_options));
	}

	GetAttachedView().Update();

	return true;
}


