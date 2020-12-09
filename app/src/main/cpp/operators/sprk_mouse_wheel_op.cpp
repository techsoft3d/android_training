// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_ops.h"


HPS::MouseWheelOperator::MouseWheelOperator(
	ZoomType in_default_type, 
	HPS::ModifierKeys in_alternate_type_modifiers)
	: zoom_type(in_default_type)
	, zoom_modifier(in_alternate_type_modifiers)
	, zoom_limit(0.0f) 
{ }

void HPS::MouseWheelOperator::UpdateZoomLimit()
{
	HPS::SimpleSphere sphere;
	HPS::SimpleCuboid bounds;
	HPS::BoundingKit bkit;
	
	if(GetAttachedView().GetAttachedModel().GetSegmentKey().ShowBounding(bkit)
		&& bkit.ShowVolume(sphere,bounds))
	{
		HPS::Vector delta = bounds.max - bounds.min;
		zoom_limit = static_cast<float>(delta.Length()) * 0.0002f;
	}
	else
		zoom_limit = 1e-9f;
}


static void AdjustPositionToPlane(HPS::KeyPath const & key_path, HPS::Point & position, HPS::Point point_in_plane)
{

	HPS::Point new_point;

	key_path.ConvertCoordinate(HPS::Coordinate::Space::World, position, HPS::Coordinate::Space::Camera, position);
	key_path.ConvertCoordinate(HPS::Coordinate::Space::World, point_in_plane, HPS::Coordinate::Space::Camera, point_in_plane);
	position.z = point_in_plane.z;
	key_path.ConvertCoordinate(HPS::Coordinate::Space::Camera, position, HPS::Coordinate::Space::World, new_point);
	position = new_point;

}


static HPS::Point ComputeReasonableTarget(HPS::WindowKey & key, HPS::KeyPath const & key_path, HPS::Point const & mouse_win, HPS::Point const & tar_orig)
{
	HPS::SelectionResults results;
	HPS::SelectionOptionsKit sokit;
	sokit.SetInternalLimit(0);
	sokit.SetRelatedLimit(0);
	sokit.SetLevel(HPS::Selection::Level::Entity);
	sokit.SetAlgorithm(HPS::Selection::Algorithm::Analytic);
	sokit.SetSorting(HPS::Selection::Sorting::Default);
	
	bool selected = key.GetSelectionControl().SelectByPoint(mouse_win, sokit, results) != 0;
	
	if (!selected)
		return tar_orig;

	HPS::WorldPoint selection;

	HPS::SelectionResultsIterator iter = results.GetIterator();
	HPS::SelectionItem item = iter.GetItem();
	item.ShowSelectionPosition(selection);

	HPS::Point	new_target = tar_orig;
	AdjustPositionToPlane(key_path, new_target, selection);

	return new_target;
}

namespace
{
	struct Field
	{
		float width;
		float height;

		void operator *=(double number)
		{
			width *= static_cast<float>(number);
			height *= static_cast<float>(number);
		}

		double diagonal_length() const
		{
			return sqrt((width * width) + (height * height));
		}

		bool is_abnormal() const
		{
			return HPS::Float::IsAbnormal(width) || HPS::Float::IsAbnormal(height);
		}
	};
}
static Field ComputeNewField(HPS::Point const & new_target, HPS::CameraKit const & old_camera)
{
	HPS::Point target;
	HPS::Point position;
	Field field;
	old_camera.ShowTarget(target);
	old_camera.ShowPosition(position);
	old_camera.ShowField(field.width, field.height);

	const HPS::Vector old_ptd = target - position;
	const HPS::Vector new_ptd = HPS::Vector(new_target) - HPS::Vector(position);

	const double old_len = old_ptd.Length();
	const double new_len = new_ptd.Length();

	const double ratio = new_len / old_len;

	field *= ratio;
	return field;
}



bool HPS::MouseWheelOperator::OnMouseWheel(HPS::MouseState const & in_state)
{
	const MouseEvent event = in_state.GetActiveEvent();
	const float mouse_wheel_sensitivity = 1.0f;	// Can add interface to modify this later
	bool invert_mouse = true;					// Can add interface to modify this later
	float zDelta = event.WheelDelta / mouse_wheel_sensitivity / 120.0f / 8.0f;

	const float SQRT_MAX_FLOAT = 1e15f;

	if (invert_mouse)
		zDelta *= -1;

	HPS::CameraKit orig;
	GetAttachedView().GetSegmentKey().ShowCamera(orig);

	HPS::CameraKit adjusted(orig);

	Point position;
	Point target;
	Field field;
	HPS::Camera::Projection type;
	orig.ShowPosition(position);
	orig.ShowTarget(target);
	orig.ShowField(field.width, field.height);
	orig.ShowProjection(type);

	Point adjusted_position = position;
	Point adjusted_target = target;
	Field adjusted_field = field;
	
	HPS::KeyPath key_path = in_state.GetEventPath();
	HPS::SegmentKey view_key = GetAttachedView().GetSegmentKey();

	if((zoom_type == ZoomType::Accurate && !event.GetModifierKeys().HasAny(zoom_modifier))|| 
		(zoom_type == ZoomType::Fast && event.GetModifierKeys().HasAny(zoom_modifier)))
	{
		HPS::WindowKey window_key = in_state.GetEventSource();
		adjusted_target = ComputeReasonableTarget(window_key, key_path, in_state.GetLocation(), target);
		if (type == HPS::Camera::Projection::Perspective)
			adjusted_field = ComputeNewField(adjusted_target, orig);
		adjusted_field *= fabs(1 + zDelta);
	}
	else
	{
		Vector to_target = target - position;
		adjusted_target = position + to_target * (1.0f - zDelta);
		if (type == HPS::Camera::Projection::Orthographic)
			adjusted_field *= fabs(1 + zDelta);
	}

	float diag_len = static_cast<float>(adjusted_field.diagonal_length());

	// If the camera is about to be too small... 
	if (diag_len < zoom_limit)
	{
		if (zDelta < 0)
			return false;

		//go with what we knew
		adjusted_field = field;
		adjusted_field *= fabs(1 + zDelta);
		adjusted_target = target;
		diag_len = static_cast<float>(adjusted_field.diagonal_length());
	}
	// If the camera is about to be too big... 
	if (diag_len > fabs(SQRT_MAX_FLOAT))
		return false;

	// Shift the target slightly toward the mouse pointer. 
	Point mouse_position;
	key_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::World, mouse_position);
	AdjustPositionToPlane(key_path, mouse_position, adjusted_target);

	Vector change = (adjusted_target - mouse_position) * zDelta;
	adjusted_target = adjusted_target + change;

	Vector dir_to_position = position - target;
	dir_to_position = dir_to_position.Normalize();
	adjusted_position = adjusted_target + dir_to_position * ((type == HPS::Camera::Projection::Orthographic) ? 2.5f : 1.8f) * static_cast<float>(diag_len);

	if (!adjusted_field.is_abnormal())
	{
		if (adjusted_position != adjusted_target)
		{
			adjusted.SetPosition(adjusted_position);
			adjusted.SetField(adjusted_field.width, adjusted_field.height);
			adjusted.SetTarget(adjusted_target);
			view_key.SetCamera(adjusted);
			Database::GetEventDispatcher().InjectEvent(CameraChangedEvent(GetAttachedView()));
			GetAttachedView().Update();
			return true;
		}
	}

	return false;
}

void HPS::MouseWheelOperator::OnModelAttached()
{
	UpdateZoomLimit();
}

void HPS::MouseWheelOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	HPS::Model attached_model = in_attached_view.GetAttachedModel();
	if (attached_model.Type() != HPS::Type::None)
		UpdateZoomLimit();
}

