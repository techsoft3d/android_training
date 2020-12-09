// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"

static const float MAX_ZOOM_LEVEL = 0.001f;
static const float MINIMUM_FIELD_SIZE = 1e-9f;

static void AdjustPositionToPlane(HPS::KeyPath const & key_path, HPS::Point const & point_in_plane, HPS::Point & in_out_pos)
{
	HPS::Point npos, npip;

	key_path.ConvertCoordinate(HPS::Coordinate::Space::World, in_out_pos, HPS::Coordinate::Space::Camera, npos);
	key_path.ConvertCoordinate(HPS::Coordinate::Space::World, point_in_plane, HPS::Coordinate::Space::Camera, npip);
	npos.z = npip.z;
	key_path.ConvertCoordinate(HPS::Coordinate::Space::Camera, npos, HPS::Coordinate::Space::World, in_out_pos);
}

static void ComputeNewField(HPS::KeyPath const &key_path, HPS::Point const &min, HPS::Point const &max,
		HPS::Point const &new_tar, float &out_width, float &out_height)
{
	HPS::Point center = min + (max - min) * 0.5f;
	HPS::Point xmax(max.x, center.y, center.z);
	HPS::Point ymax(center.x, max.y, center.z);
	HPS::Point xmin(min.x, center.y, center.z);
	HPS::Point ymin(center.x, min.y, center.z);

	key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, xmax, HPS::Coordinate::Space::World, xmax);
	key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, ymax, HPS::Coordinate::Space::World, ymax);
	key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, xmin, HPS::Coordinate::Space::World, xmin);
	key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, ymin, HPS::Coordinate::Space::World, ymin);

	AdjustPositionToPlane(key_path, new_tar, xmax);
	AdjustPositionToPlane(key_path, new_tar, ymax);
	AdjustPositionToPlane(key_path, new_tar, xmin);
	AdjustPositionToPlane(key_path, new_tar, ymin);

	HPS::Vector x = xmax - xmin;
	HPS::Vector y = ymax - ymin;

	out_width = static_cast<float>(x.Length());
	out_height = static_cast<float>(y.Length());
}

static bool ComputeReasonableTarget(HPS::WindowKey windowKey, HPS::KeyPath const &key_path,
		HPS::Point const &min, HPS::Point const &max, HPS::Point const &orig_tar, HPS::Point &out_target)
{
	HPS::SelectionItem		item;
	HPS::WorldPoint			wp;
	bool					found_sel = false;
	HPS::Point				new_target;
	HPS::SegmentKey			startSeg = windowKey;			
	HPS::Point				center = min + (max - min) * 0.5f;
	HPS::SelectionResults	results;
	HPS::SelectionOptionsKit sokit;
	sokit.SetLevel(HPS::Selection::Level::Entity);
	
	size_t selected = windowKey.GetSelectionControl().SelectByPoint(center, sokit, results);

	if (selected > 0)
	{
		HPS::SelectionResultsIterator it = results.GetIterator();
		item = it.GetItem();
		item.ShowSelectionPosition(wp);
		new_target = wp;
		found_sel = true;
	}
	else
	{
		HPS::Rectangle area((min.x < max.x ? min.x : max.x),
			(min.x > max.x ? min.x : max.x),
			(min.y < max.y ? min.y : max.y),
			(min.y > max.y ? min.y : max.y));
		if ((selected = windowKey.GetSelectionControl().SelectByArea(area, results)) == 0)
			return false;
		int count = 0;
		HPS::Point found(0,0,0);

		HPS::SelectionResultsIterator it = results.GetIterator();
		while(it.IsValid())
		{
			item = it.GetItem();
			item.ShowSelectionPosition(wp);
			new_target = wp;
			found.x += new_target.x;
			found.y += new_target.y;
			found.z += new_target.z;
			count++;
			it.Next();
		}

		new_target = min + (max - min) * 0.5f;
		key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, new_target, HPS::Coordinate::Space::World, new_target);

		if (count > 0)
		{
			AdjustPositionToPlane(key_path, found/static_cast<float>(count), new_target);
			found_sel = true;
		}
		else
			AdjustPositionToPlane(key_path, orig_tar, new_target);
	}

	out_target = new_target;
	return found_sel;
}


HPS::ZoomBoxOperator::ZoomBoxOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: ConstructRectangleOperator(in_mouse_trigger, in_modifier_trigger)
{
}

bool HPS::ZoomBoxOperator::OnMouseUp(MouseState const  & in_state)
{
	if (!ConstructRectangleOperator::OnMouseUp(in_state))
		return false;

	if (!IsRectangleValid())
		return true;

	return ZoomCommon(in_state.GetEventSource(), in_state.GetEventPath());
}

bool HPS::ZoomBoxOperator::OnTouchUp(TouchState const  & in_state)
{
	if (!ConstructRectangleOperator::OnTouchUp(in_state))
		return false;

	if (!IsRectangleValid())
		return true;

	return ZoomCommon(in_state.GetEventSource(), in_state.GetEventPath());
}

bool HPS::ZoomBoxOperator::ZoomCommon(HPS::WindowKey const & in_window, HPS::KeyPath const & in_event_path)
{
	HPS::Rectangle rectangle = GetWindowRectangle().Intersect(HPS::Rectangle::FullScope());

	if (rectangle.right - rectangle.left < MINIMUM_FIELD_SIZE &&
		rectangle.top - rectangle.bottom < MINIMUM_FIELD_SIZE)
	{
		GetAttachedView().Update();
		return true;
	}
		

	HPS::Point p0(rectangle.left, rectangle.top, 0);
	HPS::Point p1(rectangle.right, rectangle.bottom, 0);

	HPS::CameraKit old_cam;
	GetAttachedView().GetSegmentKey().ShowCamera(old_cam);
	HPS::CameraKit new_cam = HPS::CameraKit(old_cam);

	HPS::Point old_tar;
	old_cam.ShowTarget(old_tar);

	HPS::Point new_tar;
	bool anything_selected = ComputeReasonableTarget(in_window, in_event_path, p0, p1, old_tar, new_tar);

	if (anything_selected)
	{
		float new_width, new_height;
		ComputeNewField(in_event_path, p0, p1, new_tar, new_width, new_height);
		if (new_width < 0.001f * MAX_ZOOM_LEVEL)
			new_width = 0.01f * new_height;
		if (new_height < 0.001f * MAX_ZOOM_LEVEL)
			new_height = 0.01f * new_width;

		float new_diag = sqrt(new_width*new_width + new_height*new_height);

		HPS::Point old_pos;
		old_cam.ShowPosition(old_pos);
		HPS::Vector old_view_vec = old_pos - old_tar;
		old_view_vec.Normalize();

		HPS::Point new_pos = new_tar + old_view_vec*2.5f*new_diag;

		float maxZoomLevel = MAX_ZOOM_LEVEL;

		if (Min(new_width, new_height) > maxZoomLevel /* && camera different*/)
		{
			new_cam.SetPosition(new_pos);
			new_cam.SetTarget(new_tar);
			new_cam.SetField(new_width, new_height);
			GetAttachedView().GetSegmentKey().SetCamera(new_cam);
			Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
		}
	}
	GetAttachedView().Update();
	return true;
}


