// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

static HPS::LineKey InsertRectangle(HPS::SegmentKey & seg, float x0, float y0, float x1, float y1)
{
	HPS::PointArray pts;

	pts.push_back(HPS::Point(x0, y0, 0));
	pts.push_back(HPS::Point(x0, y1, 0));
	pts.push_back(HPS::Point(x1, y1, 0));
	pts.push_back(HPS::Point(x1, y0, 0));
	pts.push_back(HPS::Point(x0, y0, 0));
	return seg.InsertLine(pts);
}

static HPS::PolygonKey InsertPolygon(HPS::SegmentKey & seg, float x0, float y0, float x1, float y1)
{
	HPS::PointArray pts;

	pts.push_back(HPS::Point(x0, y0, 0));
	pts.push_back(HPS::Point(x0, y1, 0));
	pts.push_back(HPS::Point(x1, y1, 0));
	pts.push_back(HPS::Point(x1, y0, 0));
	pts.push_back(HPS::Point(x0, y0, 0));
	return seg.InsertPolygon(pts);
}

HPS::ConstructRectangleOperator::ConstructRectangleOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger, bool draw_faces)
	: Operator(in_mouse_trigger, in_modifier_trigger),
	draw_faces(draw_faces),
	use_center_marker(true),
	is_rect_valid(false),
	operator_active(false),
	tracked_touch_ID(-1)
{

}

void HPS::ConstructRectangleOperator::SetupConstructionSegment()
{
	UTF8 segment_name("construction lines");
	if (draw_faces)
		segment_name = "construction polygons";

	scratch_seg = GetAttachedView().GetSegmentKey().Subsegment("construction segments").Subsegment(segment_name);
	scratch_seg.GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::Default);

	scratch_seg.GetVisualEffectsControl().SetPostProcessEffectsEnabled(false);
}

void HPS::ConstructRectangleOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	PortfolioKey portfolio = in_attached_view.GetPortfolioKey();
	HPS::GlyphDefinition gd;

	if(!portfolio.ShowGlyphDefinition("plus", gd))
		gd = portfolio.DefineGlyph("plus", GlyphKit::GetDefault(Glyph::Default::Plus));

	SetupConstructionSegment();
}

bool HPS::ConstructRectangleOperator::OnMouseDown(MouseState const  & in_state)
{
	if (IsMouseTriggered(in_state))
	{
		HPS::KeyPath event_path(in_state.GetEventPath());
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::InnerWindow, inner_window_start_point);
		window_start_point = in_state.GetLocation();
		operator_active = true;
		tracked_touch_ID = -1;
		return true;
	}
	return false;
}

bool HPS::ConstructRectangleOperator::OnMouseUp(MouseState const  & in_state)
{
	if (operator_active && !IsMouseTriggered(in_state))
	{
		try
		{
			scratch_seg.Flush(HPS::Search::Type::Geometry);
		}
		catch(HPS::InvalidObjectException &)
		{
			SetupConstructionSegment();
		}

		HPS::KeyPath event_path(in_state.GetEventPath());
		HPS::Point inner_window_point;
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::InnerWindow, inner_window_point);

		HPS::Point new_window_point(in_state.GetLocation());
		HPS::Point window_points[2] = {new_window_point, window_start_point};
		window_rect = HPS::Rectangle(2, window_points);

		HPS::Point inner_window_points[2] = { inner_window_point, inner_window_start_point };
		inner_window_rect = HPS::Rectangle(2, inner_window_points);

		is_rect_valid = true;

		operator_active = false;
		return true;
	}
	return false;
}

bool HPS::ConstructRectangleOperator::OnMouseMove(MouseState const  & in_state)
{
	if (operator_active && IsMouseTriggered(in_state))
	{
		HPS::KeyPath event_path(in_state.GetEventPath());
		HPS::Point inner_window_point;
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::InnerWindow, inner_window_point);
		return ConstructRectCommon(inner_window_point);
	}
	return false;
}

bool HPS::ConstructRectangleOperator::OnTouchDown(TouchState const  & in_state)
{
	TouchArray touches = in_state.GetActiveEvent().Touches;

	if(tracked_touch_ID == -1 && !touches.empty() && touches[0].TapCount == 1)
	{
		tracked_touch_ID = touches[0].ID;

		HPS::KeyPath event_path(in_state.GetEventPath());
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::InnerWindow, inner_window_start_point);
		window_start_point = touches[0].Location;
		operator_active = true;
	}

	return false;
}

bool HPS::ConstructRectangleOperator::OnTouchUp(TouchState const  & in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;
	if (up_touches.empty())
		return false;

	Touch the_one = up_touches[0];

	for(size_t i = 0, e = up_touches.size(); i < e && tracked_touch_ID != the_one.ID; ++i)
	{
		if(up_touches[i].ID == tracked_touch_ID)
			the_one = up_touches[i];			
	}
	tracked_touch_ID = -1;

	try
	{
		scratch_seg.Flush(HPS::Search::Type::Geometry);
	}
	catch(HPS::InvalidObjectException &)
	{
		SetupConstructionSegment();
	}

	HPS::KeyPath event_path(in_state.GetEventPath());
	HPS::Point inner_window_point;
	event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, the_one.Location, HPS::Coordinate::Space::InnerWindow, inner_window_point);

	HPS::Point new_window_point(the_one.Location);
	HPS::Point window_points[2] = { new_window_point, window_start_point };
	window_rect = HPS::Rectangle(2, window_points);

	HPS::Point inner_window_points[2] = { inner_window_point, inner_window_start_point };
	inner_window_rect = HPS::Rectangle(2, inner_window_points);
	is_rect_valid = true;

	operator_active = false;

	return true;
}

bool HPS::ConstructRectangleOperator::OnTouchMove(TouchState const  & in_state)
{
	TouchArray touches = in_state.GetTouches();
	if (touches.size() == 0)
	{
		temp_line_key.Delete();
		temp_marker_key.Delete();
		temp_polygon_key.Delete();

		GetAttachedView().Update();
		return true;
	}

	Touch the_one = touches[0];

	for(size_t i = 0, e = touches.size(); i < e && the_one.ID != tracked_touch_ID; ++i)
	{
		if(touches[i].ID == tracked_touch_ID)
			the_one = touches[i];
	}

	HPS::KeyPath event_path(in_state.GetEventPath());
	HPS::Point inner_window_point;
	event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, the_one.Location, HPS::Coordinate::Space::InnerWindow, inner_window_point);
	return ConstructRectCommon(inner_window_point);
}

bool HPS::ConstructRectangleOperator::ConstructRectCommon(WindowPoint const & in_loc)
{
	temp_line_key.Delete();
	temp_marker_key.Delete();
	temp_polygon_key.Delete();

	if (draw_faces)
		temp_polygon_key = InsertPolygon(scratch_seg, inner_window_start_point.x, inner_window_start_point.y, in_loc.x, in_loc.y);
	else
		temp_line_key = InsertRectangle(scratch_seg, inner_window_start_point.x, inner_window_start_point.y, in_loc.x, in_loc.y);

	if (use_center_marker)
	{
		HPS::Point center = inner_window_start_point + (in_loc - inner_window_start_point) * 0.5f;
		temp_marker_key = scratch_seg.InsertMarker(center);
	}

	GetAttachedView().Update();
	return true;
}




