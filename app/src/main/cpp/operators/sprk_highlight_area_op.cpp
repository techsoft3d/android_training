// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"


HPS::HighlightAreaOperator::HighlightAreaOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: SelectAreaOperator(in_mouse_trigger, in_modifier_trigger), highlight_options(HighlightOptionsKit::GetDefault())
{
	highlight_options.SetStyleName("highlight_style");
	highlight_options.SetOverlay(HPS::Drawing::Overlay::InPlace);
}

void HPS::HighlightAreaOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	SelectAreaOperator::OnViewAttached(in_attached_view);
}

bool HPS::HighlightAreaOperator::OnMouseUp(MouseState const  & in_state)
{
	if(!SelectAreaOperator::OnMouseUp(in_state))
		return false;

	HPS::WindowKey window = in_state.GetEventSource();
	return HighlightCommon(window, in_state.GetModifierKeys());
}

bool HPS::HighlightAreaOperator::OnTouchUp(TouchState const  & in_state)
{
	if(!SelectAreaOperator::OnTouchUp(in_state))
		return false;

	HPS::WindowKey window = in_state.GetEventSource();
	return HighlightCommon(window, in_state.GetModifierKeys());
}

bool HPS::HighlightAreaOperator::HighlightCommon(WindowKey & in_window, ModifierKeys in_modifiers)
{
	HPS::SelectionResults results = GetActiveSelection();
	size_t selected = results.GetCount();

	if (!in_modifiers.Control())
	{
		in_window.GetHighlightControl().Unhighlight(highlight_options);
		HPS::Database::GetEventDispatcher().InjectEvent(HPS::HighlightEvent(HPS::HighlightEvent::Action::Unhighlight));
	}

	if (selected > 0)
	{
		in_window.GetHighlightControl().Highlight(results, highlight_options);
		HPS::Database::GetEventDispatcher().InjectEvent(HPS::HighlightEvent(HPS::HighlightEvent::Action::Highlight, results, highlight_options));
	}

	GetAttachedView().Update();

	return true;
}



