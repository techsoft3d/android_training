// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.
#include "sprk_ops.h"

using namespace HPS;

HandlesOperator::HandlesOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger)
	, tracked_touch_id(-1)
	, style_name("handles_operator_highlight_style")
	, something_is_highlighted(false)
	, are_handles_on(false)
	, remove_handles(false)
	, move_geometry(Movement::None)
	, previous_movement(Movement::None)
	, handles_length(0.08f)
	, handles_radius(0.0025f)
	, arc_offset(0.045f)
	, arc_angle(25.0f)
	, arc_points(30)
	, tip_size(0.15f)
	, plane_offset(0.2f)
	, plane_length(0.6f)
	, center_radius(0.2f)
	, display_trailing_geometry(true)
	, rotation_direction(0.0f)
	, cad_model_type(HPS::Type::None)
{
	for (size_t i = 0; i <= InternalHandleType::last; ++i)
	{
		requested_handle_type.push_back(true);
		current_handle_type.push_back(false);
	}

	geometry_selection_options.SetLevel(Selection::Level::Entity)
		.SetRelatedLimit(0)
		.SetAlgorithm(Selection::Algorithm::Visual)
		.SetProximity(0);

	handles_selection_options.SetLevel(Selection::Level::Entity)
		.SetRelatedLimit(6)
		.SetSorting(Selection::Sorting::ZSorting)
		.SetAlgorithm(Selection::Algorithm::Analytic)
		.SetProximity(0);

	style_segment = Database::CreateRootSegment();
	highlight_color = RGBColor(1, 0.55f, 0);
	style_segment.GetMaterialMappingControl().SetFaceColor(highlight_color);

	transparency_style_segment = Database::CreateRootSegment();
	transparency_style_segment.GetMaterialMappingControl().SetFaceAlpha(0.2f);

	MaterialKit x_material;
	x_material.SetDiffuseColor(RGBColor(0.658f, 0.219f, 0.231f));
	handles_colors.push_back(x_material);

	MaterialKit y_material;
	y_material.SetDiffuseColor(RGBColor(0.376f, 0.652f, 0.196f));
	handles_colors.push_back(y_material);

	MaterialKit z_material;
	z_material.SetDiffuseColor(RGBColor(0.160f, 0.317f, 0.727f));
	handles_colors.push_back(z_material);

	handles_colors.push_back(x_material);
	handles_colors.push_back(y_material);
	handles_colors.push_back(z_material);

	handles_colors.push_back(x_material);
	handles_colors.push_back(y_material);
	handles_colors.push_back(z_material);

	MaterialKit center_material;
	center_material.SetDiffuseColor(RGBColor(1, 1, 1));
	handles_colors.push_back(center_material);
}

HPS::HandlesOperator::~HandlesOperator()
{
	style_segment.Delete();
	transparency_style_segment.Delete();
}

HPS::HandlesOperator::GeometryTransformedEvent::~GeometryTransformedEvent()
{
}

void HandlesOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	are_handles_on = false;
	last_attached_view = in_attached_view;

	handles = last_attached_view.GetSegmentKey().Subsegment("construction segments").Subsegment("handles");

	handles_segment = handles.Subsegment("handles");
	handles_segment.GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::Default);

	reference_segment = handles.Subsegment("handles_reference_geometry");

	handles_trail = handles.Subsegment("handles_trailing_geometry");
	handles_trail.GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::Default);
	handles_trail.GetVisibilityControl().SetMarkers(true).SetLines(true);
	handles_trail.GetMaterialMappingControl().SetLineColor(RGBColor(1, 0.55f, 0)).SetMarkerColor(RGBColor(1, 0.55f, 0));

	PortfolioKey portfolio = last_attached_view.GetPortfolioKey();
	portfolio.DefineNamedStyle(style_name, style_segment);
	portfolio.DefineMaterialPalette("HPS_handles_material_map", handles_colors);
	highlight_options.SetStyleName(style_name).SetOverlay(Drawing::Overlay::Default);
	hide_highlight_options.SetStyleName("sprk_view_attribute_hide").SetOverlay(Drawing::Overlay::InPlace);

	portfolio.DefineNamedStyle("HPS_handles_transparency", transparency_style_segment);
	partial_transparency_options.SetStyleName("HPS_handles_transparency").SetOverlay(Drawing::Overlay::Default);

	LayoutArray layouts = last_attached_view.GetOwningLayouts();
	for (auto const & layout : layouts)
	{
		CanvasArray some_canvases = layout.GetOwningCanvases();
		canvases.insert(canvases.end(), some_canvases.begin(), some_canvases.end());
	}
}

void HandlesOperator::OnViewDetached(HPS::View const & in_detached_view)
{
	DismissHandles();

	handles_selection_options.UnsetScope();

	for (auto & one_canvas : canvases)
		one_canvas.GetWindowKey().GetHighlightControl().Unhighlight(highlight_options);

	HPS::View detached_view = in_detached_view;
	detached_view.GetPortfolioKey().UndefineNamedStyle("handles_operator_highlight_style");
	canvases.clear();
}

bool HandlesOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
		return InputDown(in_state.GetActiveEvent().ClickCount, in_state.GetEventSource(), in_state.GetEventPath(), in_state.GetLocation());

	return false;
}

bool HandlesOperator::OnMouseMove(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state) && move_geometry != Movement::None)
	{
		KeyPath path = in_state.GetEventPath();
		return InputMove(path, in_state.GetLocation());
	}

	if (are_handles_on)
	{
		WindowKey window = in_state.GetEventSource();
		return HighlightHandles(window, in_state.GetEventPath(), in_state.GetLocation());
	}

	return false;
}

bool HandlesOperator::OnMouseUp(MouseState const & in_state)
{
	if(!IsMouseTriggered(in_state))
		InputUp(in_state.GetLocation());

	return false;
}

bool HandlesOperator::OnTouchDown(TouchState const & in_state)
{
	if (tracked_touch_id != -1)
		return false;

	HPS::TouchArray touches = in_state.GetActiveEvent().Touches;
	if (touches.size() != 1)
		return false;

	bool track_touch = InputDown(touches[0].TapCount, in_state.GetEventSource(), in_state.GetEventPath(), touches[0].Location);

	if (track_touch)
		tracked_touch_id = touches[0].ID;

	return track_touch;
}

bool HandlesOperator::OnTouchMove(TouchState const & in_state)
{
	if (tracked_touch_id == -1 && move_geometry != Movement::None)
		return false;

	TouchArray touches = in_state.GetTouches();
	if (touches.size() != 1 || touches[0].TapCount != 1 || touches[0].ID != tracked_touch_id)
		return false;

	KeyPath path(in_state.GetEventPath());
	return InputMove(path, touches[0].Location);

	return true;
}

bool HandlesOperator::OnTouchUp(TouchState const & in_state)
{
	if (tracked_touch_id == -1 && !remove_handles)
		return false;

	TouchArray up_touches = in_state.GetActiveEvent().Touches;
	if (up_touches.empty())
		return false;

	if (up_touches.size() != 1 || up_touches[0].TapCount != 1 || (up_touches[0].ID != tracked_touch_id && !remove_handles))
		return false;

	InputUp(up_touches[0].Location);

	tracked_touch_id = -1;

	return false;
}

bool HandlesOperator::InputDown(size_t in_click_count, WindowKey const & in_window, KeyArray const & in_event_path, Point const & in_location)
{
	if (!are_handles_on && in_click_count == 2)
	{
		//if there is a double click, check if we have selected geometry
		//if that is the case, and handles are not already in the scene, then we insert handles there

		SelectionResults results;
		if (in_window.GetSelectionControl().SelectByPoint(in_location, geometry_selection_options, results) > 0)
		{
			SelectionItem item = results.GetIterator().GetItem();
			Key selected_item;
			item.ShowSelectedItem(selected_item);
			segment_to_move = selected_item.Owner();
			item.ShowSelectionPosition(handles_origin_point);
			trailing_line_start = handles_origin_point;
			trailing_circle_center = handles_origin_point;
			item.ShowPath(path_to_segment_to_move);

			if (cad_model_type != HPS::Type::None)
			{
				component_path_to_move = cad_model.GetComponentPath(path_to_segment_to_move);
				component_to_move = Factory::DeInstanceComponent(component_path_to_move);
				
				auto it = std::find(component_path_to_move.begin(), component_path_to_move.end(), component_to_move);
				if (it != component_path_to_move.end())
				{
					ComponentArray affected_components;
					affected_components.insert(affected_components.end(), it, component_path_to_move.end());
					component_path_to_move = ComponentPath(affected_components);
					KeyPath key_path = component_path_to_move.GetKeyPaths()[0];

					
					Key front = key_path.PopFront();
					segment_to_move = SegmentKey(front);
					path_to_segment_to_move = key_path;
				}
			}
			else
				path_to_segment_to_move.PopFront();

			//shortcut in case all handles have been disabled by the user
			bool all_handles_disabled = true;
			for (auto const & one_request : requested_handle_type)
			{
				if (one_request)
				{
					all_handles_disabled = false;
					break;
				}
			}

			if (all_handles_disabled)
				return true;

			InsertHandles();

			ReferenceGeometry(path_to_segment_to_move);

			last_attached_view.Update();
			return true;
		}
	}
	else if (in_click_count == 1 && are_handles_on)
	{
		//if there is a single click, check to see if we have selected the handles
		//if so, set up for moving geometry
		//otherwise set up for removing the handles on mouse up

		remove_handles = true;
		input_down_position = in_location;

		KeyPath path_to_handles = handles_segment + in_event_path;
		handles_selection_options.SetScope(path_to_handles);
		SelectionResults results;
		if (in_window.GetSelectionControl().SelectByPoint(in_location, handles_selection_options, results) > 0)
		{
			SelectionResultsIterator it = results.GetIterator();
			while (it.IsValid())
			{
				Key selected_item;
				it.GetItem().ShowSelectedItem(selected_item);
				SegmentKey owner = selected_item.Owner();

				if (owner == handles_geometry || owner == center_sphere)
				{
					last_attached_view.GetSegmentKey().GetCameraControl().ShowProjection(camera_projection);

					remove_handles = false;
					it.GetItem().ShowSelectionPosition(movement_start_point);

					if (selected_item == x)
					{
						move_geometry = Movement::Translation;
						movement_direction = Vector::XAxis();
					}
					else if (selected_item == y)
					{
						move_geometry = Movement::Translation;
						movement_direction = Vector::YAxis();
					}
					else if (selected_item == z)
					{
						move_geometry = Movement::Translation;
						movement_direction = Vector::ZAxis();
					}
					else if (selected_item == arc_x)
					{
						move_geometry = Movement::Rotation;
						movement_direction = Vector::YAxis();
						base_vector = Vector::XAxis();
					}
					else if (selected_item == arc_y)
					{
						move_geometry = Movement::Rotation;
						movement_direction = Vector::ZAxis();
						base_vector = Vector::YAxis();
					}
					else if (selected_item == arc_z)
					{
						move_geometry = Movement::Rotation;
						movement_direction = Vector::XAxis();
						base_vector = Vector::ZAxis();
					}
					else if (selected_item == xy)
					{
						move_geometry = Movement::PlaneTranslation;
						movement_plane = Plane(0, 0, 1, 0);
					}
					else if (selected_item == yz)
					{
						move_geometry = Movement::PlaneTranslation;
						movement_plane = Plane(1, 0, 0, 0);
					}
					else if (selected_item == xz)
					{
						move_geometry = Movement::PlaneTranslation;
						movement_plane = Plane(0, 1, 0, 0);
					}
					else if (selected_item == center)
					{
						move_geometry = Movement::ViewPlaneTranslation;

						CameraKit camera;
						GetAttachedView().GetSegmentKey().ShowCamera(camera);

						Point target, position;
						camera.ShowTarget(target);
						camera.ShowPosition(position);

						Vector camera_direction(target - position);
						camera_direction = camera_direction.Normalize();

						movement_plane = Plane(camera_direction.x, camera_direction.y, camera_direction.z, 0);
					}

					//dim the handles which are not used
					for (auto & one_canvas : canvases)
					{
						HighlightControl highlight_control = one_canvas.GetWindowKey().GetHighlightControl();

						if (current_handle_type[InternalHandleType::axis_translation_x] && selected_item != x)
							highlight_control.Highlight(x, partial_transparency_options);
						if (current_handle_type[InternalHandleType::axis_translation_y] && selected_item != y)
							highlight_control.Highlight(y, partial_transparency_options);
						if (current_handle_type[InternalHandleType::axis_translation_z] && selected_item != z)
							highlight_control.Highlight(z, partial_transparency_options);

						if (current_handle_type[InternalHandleType::rotation_x] && selected_item != arc_x)
							highlight_control.Highlight(arc_x, partial_transparency_options);
						if (current_handle_type[InternalHandleType::rotation_y] && selected_item != arc_y)
							highlight_control.Highlight(arc_y, partial_transparency_options);
						if (current_handle_type[InternalHandleType::rotation_z] && selected_item != arc_z)
							highlight_control.Highlight(arc_z, partial_transparency_options);

						if (current_handle_type[InternalHandleType::plane_translation_x] && selected_item != xy)
							highlight_control.Highlight(xy, partial_transparency_options);
						if (current_handle_type[InternalHandleType::plane_translation_y] && selected_item != yz)
							highlight_control.Highlight(yz, partial_transparency_options);
						if (current_handle_type[InternalHandleType::plane_translation_z] && selected_item != xz)
							highlight_control.Highlight(xz, partial_transparency_options);

						if (current_handle_type[InternalHandleType::center_point_translation] && selected_item != center)
								highlight_control.Highlight(center, partial_transparency_options);
					}


					handles_trail.GetVisibilityControl().SetEverything(true);
					trailing_circle.Delete();
					trailing_circle_start = movement_start_point;
					trailing_rotation = movement_direction;

					if (previous_movement == Movement::Translation && !trailing_line.Empty())
					{
						PointArray points;
						trailing_line.ShowPoints(points);
						trailing_line_start = points[1];
					}

					previous_movement = move_geometry;

					return true;
				}

				it.Next();
			}
		}
	}

	return false;
}

static void ClosestPointLineLine(Point const & p1, Point const & q1, Point const & p2, Point const & q2, Point & c1, Point & c2)
{
	//Implementation taken from Real-Time Collision Detection
	//Computes closes points c1 and c2
	//returns the squared distance between segments

	Vector d1 = q1 - p1;
	Vector d2 = q2 - p2;
	Vector r = p1 - p2;
	float a = d1.Dot(d1);
	float e = d2.Dot(d2);
	float f = d2.Dot(r);
	float s = 0;
	float t = 0;

	float epsilon = 0.00000001;

	//Check if either or both segments degenerate into points
	if (a <= epsilon && e <= epsilon)
	{
		//Both segments degenerate into points
		s = t = 0.0f;
		c1 = p1;
		c2 = p2;
	}

	if (a <= epsilon)
	{
		//First segment degenerates into a point
		s = 0.0f;
		t = f / e;
	}
	else
	{
		float c = d1.Dot(r);
		if (e <= epsilon)
		{
			//Second segment degenerates into a point
			t = 0.0f;
			s = -c / a;
		}
		else
		{
			//General case
			float b = d1.Dot(d2);
			float denom = a * e - b * b;

			//if lines are non parallel compute closest point on L1 to L2. Else pick arbitrary s (here 0)
			if (denom != 0.0f)
				s = (b * f - c* e) / denom;
			else
				s = 0.0f;

			float tnom = b * s + f;
			if (tnom < 0.0f)
			{
				t = 0.0f;
				s = -c / a;
			}
			else if (tnom > e)
			{
				t = 1.0f;
				s = (b - c) / a;
			}
			else
				t = tnom / e;
		}
	}

	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
}

bool HandlesOperator::InputMove(KeyPath const & in_path, Point const & in_location)
{
	//project the point we get from the mouse event on the plane determined by the movement direction
	Point point_one = in_location;
	Point point_two = point_one;
	if (camera_projection == Camera::Projection::Perspective)
	{
		point_one.z = 0.0f;
		point_two.z = 0.5f;
	}
	else
	{
		point_one.z = -10000;
		point_two.z = 10000;
	}

	in_path.ConvertCoordinate(Coordinate::Space::Window, point_one, Coordinate::Space::World, point_one);
	in_path.ConvertCoordinate(Coordinate::Space::Window, point_two, Coordinate::Space::World, point_two);

	MatrixKit handles_xform = MatrixKit::GetDefault();
	handles_segment.ShowModellingMatrix(handles_xform);
	Vector transformed_movement_direction = movement_direction;
	transformed_movement_direction = handles_xform.Transform(movement_direction);

	Point intersection_point;
	if (move_geometry == HandlesOperator::Movement::Rotation)
	{
		Plane plane(transformed_movement_direction, movement_start_point);
		intersection_point = plane.IntersectLineSegment2(point_one, point_two);
	}
	else if (move_geometry == HandlesOperator::Movement::PlaneTranslation || move_geometry == HandlesOperator::Movement::ViewPlaneTranslation)
	{
		Plane plane = movement_plane;
		if (move_geometry == HandlesOperator::Movement::PlaneTranslation)
			plane = handles_xform.Transform(plane);
		plane = Plane(Vector(plane), movement_start_point);
		intersection_point = plane.IntersectLineSegment2(point_one, point_two);
	}
	else
	{
		Point origin(0, 0, 0);
		origin = handles_xform.Transform(origin);
		Point point_along_movement_axis =  origin + transformed_movement_direction;
		Point dummy;
		ClosestPointLineLine(point_one, point_two, origin, point_along_movement_axis, intersection_point, dummy);
	}

	if (move_geometry == Movement::Translation)
	{
		Vector movement(intersection_point - movement_start_point);
		float displacement = movement.Dot(transformed_movement_direction);
		Vector displacement_vector = displacement * movement_direction;

		displacement_vector = handles_xform.Transform(displacement_vector);

		//move the handles
		handles_segment.GetModellingMatrixControl().Translate(displacement_vector.x, displacement_vector.y, displacement_vector.z);

		trailing_circle_center += displacement_vector;
		if (display_trailing_geometry)
		{
			//add trailing geometry
			Point trailing_line_end;
			if (!trailing_line.Empty())
			{
				PointArray points;
				trailing_line.ShowPoints(points);
				trailing_line_end = points[1];
				trailing_line_end += displacement_vector;
			}
			else
			{
				float trailing_line_length = (float)displacement_vector.Length();
				Vector trailing_line_direction = displacement_vector;
				trailing_line_end = trailing_line_start + trailing_line_direction.Normalize() * trailing_line_length;
			}

			trailing_circle.Delete();
			trailing_line.Delete();
			trailing_line.Reset();
			trail_marker.Delete();

			trail_marker = handles_trail.InsertMarker(trailing_line_start);
			trailing_line = handles_trail.InsertLine(trailing_line_start, trailing_line_end);
		}

		//move the geometry
		MatrixKit matrix;
		if (reference_segment.ShowModellingMatrix(matrix))
		{
			matrix = matrix.Invert();
			displacement_vector = matrix.Transform(displacement_vector);

			//take scaling into account
			FloatArray elements;
			if (matrix.ShowElements(elements))
			{
				Vector x_scale = Vector(elements[0], elements[1], elements[2]);
				Vector y_scale = Vector(elements[4], elements[5], elements[6]);
				Vector z_scale = Vector(elements[8], elements[9], elements[10]);

				displacement_vector.x *= (float)x_scale.LengthSquared();
				displacement_vector.y *= (float)y_scale.LengthSquared();
				displacement_vector.z *= (float)z_scale.LengthSquared();
			}
		}
		temporary_segment_to_move.GetModellingMatrixControl().Translate(displacement_vector.x, displacement_vector.y, displacement_vector.z);

		movement_start_point = intersection_point;

		last_attached_view.Update();
	}
	else if (move_geometry == Movement::PlaneTranslation || move_geometry == Movement::ViewPlaneTranslation)
	{
		Vector movement(intersection_point - movement_start_point);

		//move the handles
		handles_segment.GetModellingMatrixControl().Translate(movement.x, movement.y, movement.z);

		//update trailing geometry
		trailing_circle.Delete();
		trail_marker.Delete();

		if (!trailing_line.Empty())
		{
			PointArray points;
			trailing_line.ShowPoints(points);
			points[1] = HPS::Point(points[1].x + movement.x, points[1].y + movement.y, points[1].z + movement.z);
			trailing_line_start = points[1];
		}
		else
			trailing_line_start = HPS::Point(trailing_line_start.x + movement.x, trailing_line_start.y + movement.y, trailing_line_start.z + movement.z);

		trailing_circle_center = HPS::Point(trailing_circle_center.x + movement.x, trailing_circle_center.y + movement.y, trailing_circle_center.z + movement.z);

		trailing_line.Delete();
		trailing_line.Reset();
		trailing_rotation = Vector::Zero();

		//move the geometry
		MatrixKit matrix;
		if (reference_segment.ShowModellingMatrix(matrix))
		{
			matrix = matrix.Invert();
			movement = matrix.Transform(movement);

			//take scaling into account
			FloatArray elements;
			if (matrix.ShowElements(elements))
			{
				Vector x_scale = Vector(elements[0], elements[1], elements[2]);
				Vector y_scale = Vector(elements[4], elements[5], elements[6]);
				Vector z_scale = Vector(elements[8], elements[9], elements[10]);

				movement.x *= (float)x_scale.LengthSquared();
				movement.y *= (float)y_scale.LengthSquared();
				movement.z *= (float)z_scale.LengthSquared();
			}
		}
		temporary_segment_to_move.GetModellingMatrixControl().Translate(movement.x, movement.y, movement.z);

		movement_start_point = intersection_point;

		last_attached_view.Update();
	}
	else if (move_geometry == Movement::Rotation)
	{
		//calculate the angle between the place where we last stopped moving and where we are now
		FloatArray elements;
		handles_xform.ShowElements(elements);
		Vector handles_translation(elements[12], elements[13], elements[14]);

		Vector base(movement_start_point - handles_translation);
		base = base.Normalize();
		Vector movement(intersection_point - handles_translation);
		movement = movement.Normalize();

		float direction = base.Cross(movement).Dot(transformed_movement_direction);

		float angle = ACos(movement.Dot(base));
		if (direction < 0)
			angle = -angle;

		//transform both the handles and the geometry associated with it accordingly
		//the rotation happens around the handles center
		MatrixKit matrix;
		matrix.Translate(-handles_translation.x, -handles_translation.y, -handles_translation.z);
		matrix.RotateOffAxis(transformed_movement_direction, angle);
		matrix.Translate(handles_translation.x, handles_translation.y, handles_translation.z);
		handles_segment.GetModellingMatrixControl().Concatenate(matrix);

		if (display_trailing_geometry)
		{
			//add trailing geometry
			Point trailing_circle_end;
			Point trailing_circle_middle;

			auto compute_middle_point = [](Vector & end_to_start, Point const & start, Point const & center)
			{
				float chord_length = (float)end_to_start.Length();
				end_to_start = end_to_start.Normalize();
				Point point_on_chord = start + end_to_start * (chord_length / 2.0f);

				float radius = (float)Vector(start - center).Length();
				Vector direction_v(point_on_chord - center);
				direction_v = direction_v.Normalize();
				return Point(center + direction_v * radius);
			};

			if (trailing_circle.Type() != HPS::Type::None)
			{
				trailing_circle.ShowEnd(trailing_circle_end);
				trailing_circle_end = matrix.Transform(trailing_circle_end);
				trailing_circle.ShowMiddle(trailing_circle_middle);

				Vector one(trailing_circle_end - trailing_circle_center);
				Vector end_to_start(one);
				Vector two(trailing_circle_start - trailing_circle_center);
				one = one.Normalize();
				two = two.Normalize();

				Vector cross_p = one.Cross(two);
				float current_direction = cross_p.Dot(transformed_movement_direction);
				if (current_direction * rotation_direction <= 0)
				{
					//we went from left to right
					if (one.Dot(two) > 0)
					{
						//we need to recompute the middle point
						trailing_circle_middle = compute_middle_point(end_to_start, trailing_circle_start, trailing_circle_center);
					}
				}
				rotation_direction = current_direction;
			}
			else
			{
				trailing_circle_end = matrix.Transform(trailing_circle_start);
				Vector one(trailing_circle_end - trailing_circle_start);

				trailing_circle_middle = compute_middle_point(one, trailing_circle_start, trailing_circle_center);

				Vector two(trailing_circle_start - trailing_circle_center);
				two = two.Normalize();
				Vector cross_p = one.Cross(two);
				rotation_direction = cross_p.Dot(transformed_movement_direction);
			}

			trailing_line.Delete();
			trailing_line.Reset();
			trailing_circle.Delete();
			trail_marker.Delete();

			trail_marker = handles_trail.InsertMarker(trailing_circle_start);
			trailing_circle = handles_trail.InsertCircularArc(trailing_circle_start, trailing_circle_middle, trailing_circle_end);
		}

		//move the geometry
		MatrixKit ref_matrix;
		if (reference_segment.ShowModellingMatrix(ref_matrix))
		{
			matrix = MatrixKit::GetDefault();
			MatrixKit inv_ref_matrix = ref_matrix.Invert();

			Point displacement(handles_translation.x, handles_translation.y, handles_translation.z);
			displacement = inv_ref_matrix.Transform(displacement);
			transformed_movement_direction = inv_ref_matrix.Transform(transformed_movement_direction);

			matrix.Translate(-displacement.x, -displacement.y, -displacement.z);
			matrix.RotateOffAxis(transformed_movement_direction, angle);
			matrix.Translate(displacement.x, displacement.y, displacement.z);
		}
		temporary_segment_to_move.GetModellingMatrixControl().Concatenate(matrix);

		movement_start_point = intersection_point;

		last_attached_view.Update();
	}

	return true;
}

void HandlesOperator::InputUp(Point const & in_location)
{
	if (are_handles_on && remove_handles)
	{
		if (in_location != input_down_position)
		{
			remove_handles = false;
			return;
		}
		DismissHandles();
	}
	else if (are_handles_on && move_geometry != HandlesOperator::Movement::None)
	{
		for (auto & one_canvas : canvases)
		{
			HighlightControl highlight_control = one_canvas.GetWindowKey().GetHighlightControl();

			if (current_handle_type[InternalHandleType::axis_translation_x])
				highlight_control.Unhighlight(x, partial_transparency_options);
			if (current_handle_type[InternalHandleType::axis_translation_y])
				highlight_control.Unhighlight(y, partial_transparency_options);
			if (current_handle_type[InternalHandleType::axis_translation_z])
				highlight_control.Unhighlight(z, partial_transparency_options);
			
			if (current_handle_type[InternalHandleType::rotation_x])
				highlight_control.Unhighlight(arc_x, partial_transparency_options);
			if (current_handle_type[InternalHandleType::rotation_y])
				highlight_control.Unhighlight(arc_y, partial_transparency_options);
			if (current_handle_type[InternalHandleType::rotation_z])
				highlight_control.Unhighlight(arc_z, partial_transparency_options);

			if (current_handle_type[InternalHandleType::plane_translation_x])
				highlight_control.Unhighlight(xy, partial_transparency_options);
			if (current_handle_type[InternalHandleType::plane_translation_y])
				highlight_control.Unhighlight(yz, partial_transparency_options);
			if (current_handle_type[InternalHandleType::plane_translation_z])
				highlight_control.Unhighlight(xz, partial_transparency_options);

			if (current_handle_type[InternalHandleType::center_point_translation])
				highlight_control.Unhighlight(center, partial_transparency_options);
		}
		
		handles_trail.GetVisibilityControl().SetEverything(false);
		last_attached_view.Update();
	}

	move_geometry = Movement::None;
}

bool HandlesOperator::HighlightHandles(WindowKey & in_window, KeyArray const & in_event_path, Point const & in_location)
{
	//highlight the handles as the user mouses over them
	KeyPath path_to_handles = handles_segment + in_event_path;
	handles_selection_options.SetScope(path_to_handles);
	SelectionResults results;
	if (in_window.GetSelectionControl().SelectByPoint(in_location, handles_selection_options, results) > 0)
	{
		SelectionResultsIterator it = results.GetIterator();
		while (it.IsValid())
		{
			Key selected_item;
			it.GetItem().ShowSelectedItem(selected_item);
			SegmentKey owner = selected_item.Owner();
			if (owner == handles_geometry || owner == center_sphere)
			{
				if (!highlighted_handle.Empty())
					in_window.GetHighlightControl().Unhighlight(highlighted_handle, highlight_options);

				in_window.GetHighlightControl().Highlight(selected_item, highlight_options);
				highlighted_handle = selected_item;
				last_attached_view.Update();
				something_is_highlighted = true;
				return false;
			}
			it.Next();
		}
	}

	//if we get here we selected nothing, so we unhighlight
	if (something_is_highlighted)
	{
		something_is_highlighted = false;
		in_window.GetHighlightControl().Unhighlight(highlighted_handle, highlight_options);
		highlighted_handle = Key();
		last_attached_view.Update();
		return true;
	}

	return false;
}

void HandlesOperator::InsertHandles()
{
	handles_segment.SetMaterialPalette("HPS_handles_material_map");
	handles_geometry = handles_segment.Subsegment("handles_geometry");

	Point origin(0, 0, 0);
	Vector translation = (handles_origin_point - origin);

	if (requested_handle_type[InternalHandleType::axis_translation_x] ||
		requested_handle_type[InternalHandleType::axis_translation_y] || 
		requested_handle_type[InternalHandleType::axis_translation_z])
		InsertAxisTranslationHandles(requested_handle_type[InternalHandleType::axis_translation_x], requested_handle_type[InternalHandleType::axis_translation_y], requested_handle_type[InternalHandleType::axis_translation_z]);
	if (requested_handle_type[InternalHandleType::rotation_x] ||
		requested_handle_type[InternalHandleType::rotation_y] ||
		requested_handle_type[InternalHandleType::rotation_z])
		InsertRotationHandles(requested_handle_type[InternalHandleType::rotation_x], requested_handle_type[InternalHandleType::rotation_y], requested_handle_type[InternalHandleType::rotation_z]);
	if (requested_handle_type[InternalHandleType::plane_translation_x] ||
		requested_handle_type[InternalHandleType::plane_translation_y] ||
		requested_handle_type[InternalHandleType::plane_translation_z])
		InsertPlaneTranslationHandles(requested_handle_type[InternalHandleType::plane_translation_x], requested_handle_type[InternalHandleType::plane_translation_y], requested_handle_type[InternalHandleType::plane_translation_z]);
	if (requested_handle_type[InternalHandleType::center_point_translation])
		InsertCenterPoint();

	for (size_t i = 0; i <= InternalHandleType::last; ++i)
		current_handle_type[i] = requested_handle_type[i];

	handles_segment.GetVisualEffectsControl().SetSimpleReflection(false).SetSimpleShadow(false).SetSilhouetteEdgesEnabled(false).SetPostProcessEffectsEnabled(false);
	handles_segment.GetBoundingControl().SetExclusion(true);
	handles_segment.GetModellingMatrixControl().Translate(translation.x, translation.y, translation.z);
	handles_geometry.GetTransformMaskControl().SetCameraScale(true).SetCameraPerspectiveScale(true);

	are_handles_on = true;
}

void HandlesOperator::InsertRotationHandles(bool in_x, bool in_y, bool in_z)
{
	Point origin(0, 0, 0);

	float arc_radius = handles_length * 2.0f + arc_offset;
	int number_of_semiarc_points = HPS::Ceiling((int)arc_points / 2.0f);

	PointArray arc_points_x;
	PointArray arc_points_y;
	PointArray arc_points_z;
	for (int i = -number_of_semiarc_points; i < number_of_semiarc_points; ++i)
	{
		float angle = (float)i / (float)number_of_semiarc_points * arc_angle;

		arc_points_x.push_back(Point(origin.x + arc_radius * Cos(angle), origin.y, origin.z + arc_radius * Sin(angle)));
		arc_points_y.push_back(Point(origin.x + arc_radius * Sin(angle), origin.y + arc_radius * Cos(angle), origin.z));
		arc_points_z.push_back(Point(origin.x, origin.y + arc_radius * Sin(angle), origin.z + arc_radius * Cos(angle)));
	}

	int tip_point_size = HPS::Ceiling(arc_points * tip_size);
	float radius = handles_radius * 2.0f;
	float tip_base_radius = radius * 2.5f;
	float slope = tip_base_radius / (tip_point_size - 1);
	FloatArray radii(arc_points, radius);

	CylinderKit cylinder;
	for (int i = 0; i < tip_point_size; ++i)
		radii[i] = slope * i;

	for (int i = (int)arc_points - 1, j = 0; j < tip_point_size; --i, ++j)
		radii[i] = slope * j;
	cylinder.SetRadii(radii).SetCaps(Cylinder::Capping::Both);

	if (in_y)
	{
		cylinder.SetPoints(arc_points_x);
		arc_x = handles_geometry.InsertCylinder(cylinder);
		arc_x.SetVertexIndexColorsByRange(0, FloatArray(number_of_semiarc_points * 2, 3.0f));
	}

	if (in_z)
	{
		cylinder.SetPoints(arc_points_y);
		arc_y = handles_geometry.InsertCylinder(cylinder);
		arc_y.SetVertexIndexColorsByRange(0, FloatArray(number_of_semiarc_points * 2, 4.0f));
	}

	if (in_x)
	{
		cylinder.SetPoints(arc_points_z);
		arc_z = handles_geometry.InsertCylinder(cylinder);
		arc_z.SetVertexIndexColorsByRange(0, FloatArray(number_of_semiarc_points * 2, 5.0f));
	}
}

void HandlesOperator::InsertAxisTranslationHandles(bool in_x, bool in_y, bool in_z)
{
	Point origin(0, 0, 0);

	//the sizes for the handles need to make sense in a default camera space
	float length = handles_length * 2.0f;
	float radius = handles_radius * 2.0f;

	FloatArray radii(arc_points, radius);
	int tip_point_size = HPS::Ceiling(arc_points * tip_size);
	int tip_offset = (int)radii.size() - tip_point_size;

	float tip_base_radius = radius * 2.5f;
	float slope = tip_base_radius / (tip_point_size - 1);
	for (int i = 0; i < tip_point_size; ++i)
		radii[tip_offset + i] = slope * (tip_point_size - 1 - i);

	CylinderKit cylinder;
	cylinder.SetCaps(Cylinder::Capping::Both).SetRadii(radii);

	if (in_x)
	{
		PointArray x_points;
		for (int i = 0; i < (int)arc_points; ++i)
			x_points.push_back(Point(origin + Vector::XAxis() * length * (float)i / (float)arc_points));
		cylinder.SetPoints(x_points);
		x = handles_geometry.InsertCylinder(cylinder);
		x.SetVertexIndexColorsByRange(0, FloatArray(arc_points, 0.0f));
	}

	if (in_y)
	{
		PointArray y_points;
		for (int i = 0; i < (int)arc_points; ++i)
			y_points.push_back(Point(origin + Vector::YAxis() * length * (float)i / (float)arc_points));
		cylinder.SetPoints(y_points);
		y = handles_geometry.InsertCylinder(cylinder);
		y.SetVertexIndexColorsByRange(0, FloatArray(arc_points, 1.0f));
	}

	if (in_z)
	{
		PointArray z_points;
		for (int i = 0; i < (int)arc_points; ++i)
			z_points.push_back(Point(origin + Vector::ZAxis() * length * (float)i / (float)arc_points));
		cylinder.SetPoints(z_points);
		z = handles_geometry.InsertCylinder(cylinder);
		z.SetVertexIndexColorsByRange(0, FloatArray(arc_points, 2.0f));
	}
}

void HandlesOperator::InsertPlaneTranslationHandles(bool in_x, bool in_y, bool in_z)
{
	Point origin(0, 0, 0);
	float default_handle_length = 0.08f;
	float offset = default_handle_length * plane_offset;
	float length = default_handle_length * plane_length;

	PointArray points;

	//xy
	IntArray face_list = { 4, 0, 1, 2, 3 };
	ShellKit shell;
	shell.SetFacelist(face_list);
	if (in_x)
	{
		points.push_back(Point(offset, offset, 0));
		points.push_back(Point(offset + length, offset, 0));
		points.push_back(Point(offset + length, offset + length, 0));
		points.push_back(Point(offset, offset + length, 0));

		shell.SetPoints(points);
		xy = handles_geometry.InsertShell(shell);
		xy.SetFaceIndexColorsByRange(0, 1, 7);
	}

	//xz
	if (in_z)
	{
		points.clear();
		points.push_back(Point(offset, 0, offset));
		points.push_back(Point(offset + length, 0, offset));
		points.push_back(Point(offset + length, 0, offset + length));
		points.push_back(Point(offset, 0, offset + length));

		shell.SetPoints(points);
		xz = handles_geometry.InsertShell(shell);
		xz.SetFaceIndexColorsByRange(0, 1, 6);
	}

	//yz
	if (in_y)
	{
		points.clear();
		points.push_back(Point(0, offset, offset));
		points.push_back(Point(0, offset + length, offset));
		points.push_back(Point(0, offset + length, offset + length));
		points.push_back(Point(0, offset, offset + length));

		shell.SetPoints(points);
		yz = handles_geometry.InsertShell(shell);
		yz.SetFaceIndexColorsByRange(0, 1, 8);
	}
	handles_geometry.GetVisibilityControl().SetPerimeterEdges(true);
}

void HandlesOperator::InsertCenterPoint()
{
	Point origin(0, 0, 0);
	float default_handles_length = 0.08f;

	//center
	float sphere_radius = default_handles_length * center_radius;
	SphereKit sphere;
	sphere.SetCenter(origin).SetRadius(sphere_radius).SetBasis(Vector::XAxis(), Vector::YAxis());
	center_sphere = handles_geometry.Subsegment("center");
	center = center_sphere.InsertSphere(sphere);
	center_sphere.GetMaterialMappingControl().SetFaceMaterialByIndex(9);
}

void HandlesOperator::DismissHandles()
{
	handles_segment.Flush(Search::Type::Geometry, Search::Space::Subsegments);
	handles_segment.GetModellingMatrixControl().UnsetEverything();
	handles_segment.GetTransformMaskControl().UnsetEverything();
	are_handles_on = false; 
	remove_handles = false;

	trailing_rotation = Vector::Zero();

	trailing_circle.Delete();
	trailing_line.Delete();
	trailing_line.Reset();
	trail_marker.Delete();
	
	rotation_direction = 0.0f;

	CommitChanges();

	move_geometry = Movement::None;
	last_attached_view.Update();
}

void HandlesOperator::SetHighlightColor(RGBAColor const & in_highlight_color)
{
	highlight_color = in_highlight_color;
	style_segment.GetMaterialMappingControl().SetFaceColor(in_highlight_color);
}

void HandlesOperator::ReferenceGeometry(KeyPath const & in_path)
{
	complete_referenced_path = segment_to_move + in_path;
	for (auto & one_canvas : canvases)
	{
		HighlightControl highlight_control = one_canvas.GetWindowKey().GetHighlightControl();
		highlight_control.Unhighlight(complete_referenced_path);
		highlight_control.Highlight(complete_referenced_path, hide_highlight_options);
	}

	KeyArray keys;
	in_path.ShowKeys(keys);
	KeyArray trimmed_keys;

	size_t start_index = 0;
	while (keys[start_index].Type() != HPS::Type::SegmentKey)
		start_index++;

	trimmed_keys.insert(trimmed_keys.end(), keys.begin() + start_index, keys.end());
	KeyPath trimmed_path(trimmed_keys);

	bool contains_text = false;
	bool contains_markers = false;
	bool contains_lines = false;
	bool contains_shells = false;
	bool contains_polygons = false;

	SearchResults results;
	size_t count = segment_to_move.Find(Search::Type::Text, Search::Space::SegmentOnly, results);
	if (count > 0)
		contains_text = true;

	count = segment_to_move.Find(Search::Type::Marker, Search::Space::SegmentOnly, results);
	if (count > 0)
		contains_markers = true;

	Search::Type lines [] = {Search::Type::CircularArc, Search::Type::EllipticalArc, Search::Type::InfiniteLine, Search::Type::InfiniteRay, Search::Type::Line, Search::Type::NURBSCurve};
	count = segment_to_move.Find(6, lines, Search::Space::SegmentOnly, results);
	if (count > 0)
		contains_lines = true;

	Search::Type shell [] = {Search::Type::Shell, Search::Type::Mesh, Search::Type::Grid};
	count = segment_to_move.Find(3, shell, Search::Space::SegmentOnly, results);
	if (count > 0)
		contains_shells = true;

	Search::Type polygon [] = {Search::Type::Polygon, Search::Type::Circle, Search::Type::CircularWedge, Search::Type::Cylinder, Search::Type::Ellipse, Search::Type::NURBSSurface, Search::Type::Sphere};
	count = segment_to_move.Find(7, polygon, Search::Space::SegmentOnly, results);
	if (count > 0)
		contains_polygons = true;

	MatrixKit net_modelling_matrix;
	if (trimmed_path.ShowNetModellingMatrix(net_modelling_matrix))
		reference_segment.SetModellingMatrix(net_modelling_matrix);

	VisibilityKit net_visibility;
	if (trimmed_path.ShowNetVisibility(net_visibility))
		reference_segment.SetVisibility(net_visibility);

	DrawingAttributeKit net_drawing_attributes;
	if (trimmed_path.ShowNetDrawingAttribute(net_drawing_attributes))
		reference_segment.SetDrawingAttribute(net_drawing_attributes);

	TransformMaskKit net_transform_mask;
	if (trimmed_path.ShowNetTransformMask(net_transform_mask))
		reference_segment.SetTransformMask(net_transform_mask);

	UTF8Array conditions;
	if (trimmed_path.ShowNetConditions(conditions))
		reference_segment.SetConditions(conditions);

	MaterialMappingKit net_material;
	if (trimmed_path.ShowNetMaterialMapping(net_material))
		reference_segment.SetMaterialMapping(net_material);

	if (contains_text)
	{
		TextAttributeKit net_text_attributes;
		if (trimmed_path.ShowNetTextAttribute(net_text_attributes))
			reference_segment.SetTextAttribute(net_text_attributes);
	}

	if (contains_markers)
	{
		MarkerAttributeKit net_marker_attributes;
		if (trimmed_path.ShowNetMarkerAttribute(net_marker_attributes))
			reference_segment.SetMarkerAttribute(net_marker_attributes);
	}

	if (contains_lines)
	{
		LineAttributeKit net_line_attributes;
		if (trimmed_path.ShowNetLineAttribute(net_line_attributes))
			reference_segment.SetLineAttribute(net_line_attributes);
	}

	if (contains_shells || contains_polygons)
	{
		EdgeAttributeKit net_edge_attributes;
		if (trimmed_path.ShowNetEdgeAttribute(net_edge_attributes))
			reference_segment.SetEdgeAttribute(net_edge_attributes);
	}

	try 
	{
		SegmentKey model_segment = last_attached_view.GetAttachedModel().GetSegmentKey();
		PortfolioKeyArray portfolios;
		if (model_segment.GetPortfolioControl().Show(portfolios))
			reference_segment.GetPortfolioControl().Set(portfolios);
	}
	catch (InvalidObjectException const &)
	{ }
	

	temporary_segment_to_move = reference_segment.Subsegment("HPS_handles_temporary_segment");
	temporary_segment_to_move.IncludeSegment(segment_to_move);
}

void HandlesOperator::CommitChanges()
{
	HPS::CanvasArray valid_canvases;
	for (auto & one_canvas : canvases)
	{
		//make sure we do not use canvases which are not valid anymore
		try
		{
			one_canvas.GetWindowKey().GetHighlightControl().Unhighlight(complete_referenced_path, hide_highlight_options);
			valid_canvases.push_back(one_canvas);
		}
		catch (HPS::InvalidObjectException const &)
		{ }
	}
	canvases = valid_canvases;

	try
	{
		MatrixKit matrix;
		if (temporary_segment_to_move.ShowModellingMatrix(matrix))
		{
			if (cad_model_type != HPS::Type::ExchangeCADModel &&
				cad_model_type != HPS::Type::ParasolidCADModel)
			{
				/* If the segment we want to move, or one of its parents, is included in multiple places,
				 * we need to make a copy of it and flatten out the subtree that needs to be moved.
				 * this is done because otherwise all pieces of geometry from that include will also be transformed.
				 * NOTE: If you are operating on components, this will BREAK the component structure. */
				bool multiple_includes = false;

				/* Start with the segment that we are trying to move. This will always be included in at least
				 * one more segment, since the operator includes segment_to_move somewhere above the static tree
				 * For this reason, for this segment alone we check whether it is included more than twice. */
				size_t minimum_number_of_includes = 1;
				if (path_to_segment_to_move.At(0).Type() == HPS::Type::IncludeKey)
					minimum_number_of_includes = 2;

				//Show the includes used for the segment we moved
				HPS::IncludeKeyArray includes;
				size_t includers = segment_to_move.ShowIncluders(includes);

				SegmentKeyArray segments_which_include_segment_to_move;
				segments_which_include_segment_to_move.resize(includes.size());
				for (size_t i = 0; i < includes.size(); ++i)
					segments_which_include_segment_to_move[i] = includes[i].Owner();

				//Check if the segment we are trying to move is included into one of the segments along its path
				//If this is the case we will need to break this include
				bool included_along_path = false;
				SegmentKey model_segment = last_attached_view.GetAttachedModel().GetSegmentKey();
				for (auto const & one_key : path_to_segment_to_move)
				{
					if (one_key.Type() == HPS::Type::SegmentKey)
					{
						HPS::SegmentKey one_segment(one_key);
						if (one_segment == model_segment)
							break;

						if (std::find(segments_which_include_segment_to_move.begin(), segments_which_include_segment_to_move.end(), one_segment) != segments_which_include_segment_to_move.end())
						{
							included_along_path = true;
							break;
						}
					}
				}

				//Check to see if the segment we want to move is included somewhere, and if
				//its parent segments are also included from somewhere.
				//If we have a chain of includes we have to break all of them, in this case
				//we mark segment_to_move as having multiple_includes
				for (auto const & one_key : path_to_segment_to_move)
				{
					if (one_key == model_segment)
						break;
					else if (one_key.Type() == HPS::Type::SegmentKey)
					{
						SegmentKey one_segment(one_key);
						SegmentKeyArray key_includers;
						if (one_segment.ShowIncluders(key_includers) > 1)
						{
							multiple_includes = true;
							break;
						}
					}
				}

				//Simple case:
				//Either segment_to_move is included in other places, or is an include itself, but its parent segments are not included from somewhere
				if (!multiple_includes && (includers > minimum_number_of_includes || included_along_path))
				{
					//Moving the segment without breaking the include will cause all the included entities to also be transformed
					//To break the include we do the following:
					//1. Copy the included segment to the target of the include and
					//2. Remove the include
					for (size_t i = 0; i < segments_which_include_segment_to_move.size(); ++i)
					{
						//Disregard the include created by the handles operator
						if (segments_which_include_segment_to_move[i] == temporary_segment_to_move)
							continue;

						//If we are moving a segment which contains an include, only break the include if it is along the path of segment_to_move
						if (included_along_path && std::find(path_to_segment_to_move.begin(), path_to_segment_to_move.end(), (HPS::Key)segments_which_include_segment_to_move[i]) == path_to_segment_to_move.end())
							continue;

						//Copy the segment we want to move to the place where it was included
						HPS::SegmentKey copied_segment = HPS::SegmentKey(segment_to_move.CopyTo(segments_which_include_segment_to_move[i]));

						//If we are going to break the include somewhere along the path of the segment we want to move, move the newly copied segment instead
						//to avoid moving all the other includes
						if (included_along_path)
							segment_to_move = copied_segment;

						//Delete the old include
						includes[i].Delete();
					}
				}

				//Difficult case:
				//segment_to_move requires a chain of includes to all be broken
				else if (multiple_includes)
				{
					size_t last_include_index = 0;
					while (path_to_segment_to_move.At(last_include_index + 1).Type() != HPS::Type::SegmentKey)
						last_include_index++;

					SegmentKey start_flattening_here;
					Key parent_include;
					SegmentKey parent_segment;

					//mark the target segment with some user data
					HPS::byte move_data[] = { 'm', 'o', 'v', 'e' };
					static intptr_t handles_user_data_index = 10;
					segment_to_move.SetUserData(handles_user_data_index, 4, move_data);

					if (last_include_index > 0 && path_to_segment_to_move.At(last_include_index - 1).Type() != HPS::Type::IncludeKey)
					{
						start_flattening_here = SegmentKey(path_to_segment_to_move.At(last_include_index - 1));
						parent_include = path_to_segment_to_move.At(last_include_index);
						parent_segment = SegmentKey(path_to_segment_to_move.At(last_include_index + 1));
					}
					else
					{
						start_flattening_here = SegmentKey(path_to_segment_to_move.At(last_include_index + 1));
						parent_include = path_to_segment_to_move.At(last_include_index + 2);
						parent_segment = SegmentKey(path_to_segment_to_move.At(last_include_index + 3));
					}

					/* Flatten the segment tree along the selection path */
					if (parent_include.Type() != HPS::Type::IncludeKey)
					{
						//something went wrong
						return;
					}
					parent_segment = (SegmentKey)start_flattening_here.CopyTo(parent_segment);
					parent_include.Delete();

					SegmentKeyArray targets;
					SegmentKeyArray parents;
					SegmentKeyArray children;

					int flattening_index = (int)(last_include_index + 1);
					for (; flattening_index >= 0;)
					{
						HPS::Type key_type = path_to_segment_to_move.At(flattening_index).Type();
						if (key_type == HPS::Type::IncludeKey)
						{
							targets.push_back(IncludeKey(path_to_segment_to_move.At(flattening_index)).GetTarget());

							if (flattening_index + 1 == (int)last_include_index + 1)
								parents.push_back(SegmentKey(parent_segment));
							else
								parents.push_back(SegmentKey(path_to_segment_to_move.At(flattening_index + 1)));

							if (flattening_index > 0)
								children.push_back(SegmentKey(path_to_segment_to_move.At(flattening_index - 1)));
							else
								children.push_back(segment_to_move);

							flattening_index = flattening_index - 2;
						}
						else
							flattening_index--;
					}

					if (children.empty())
					{
						SearchResults results;
						size_t found = parent_segment.Find(Search::Type::UserData, Search::Space::Subsegments, results);
						if (found == 0)
						{
							//something went wrong
							
						}
						else
						{
							SearchResultsIterator it = results.GetIterator();
							while (it.IsValid())
							{
								SegmentKey candidate(it.GetItem());
								ByteArray data;
								if (candidate.ShowUserData(handles_user_data_index, data))
								{
									segment_to_move = candidate;
									break;
								}
								it.Next();
							}
								
							segment_to_move.UnsetUserData(handles_user_data_index);
						}
					}
					else
					{
						segment_to_move.UnsetUserData(handles_user_data_index);
						for (size_t i = 0; i < children.size(); ++i)
						{
							segment_to_move = (SegmentKey)children[i].CopyTo(parents[i]);
							if (i + 1 < parents.size())
								parents[i + 1] = segment_to_move;

							SearchResults results;
							parents[i].Find(Search::Type::Include, Search::Space::SegmentOnly, results);
							SearchResultsIterator it = results.GetIterator();
							while (it.IsValid())
							{
								IncludeKey search_item = (IncludeKey)it.GetItem();
								if (search_item.GetTarget() == targets[i])
								{
									search_item.Delete();
									break;
								}
								it.Next();
							}
						}
					}
				}

				segment_to_move.GetModellingMatrixControl().Concatenate(matrix);
			}
			else
			component_to_move.AddTransform(matrix);

			GeometryTransformedEvent event(last_attached_view, segment_to_move, matrix);
			HPS::Database::GetEventDispatcher().InjectEvent(event);
		}
	}
	catch (InvalidObjectException const &)
	{ }

	temporary_segment_to_move.Delete();
	reference_segment.Flush();

	last_attached_view.Update();
}

void HandlesOperator::SetHandlesAppearance(float in_length, float in_radius, size_t in_arc_points)
{
	if (in_length <= 0 || in_length > 1)
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Handles Operator - Invalid Handles Length. The valid range is (0, 1]. Request ignored."));
	else
		handles_length = in_length / 2.0f;

	if (in_radius <= 0 || in_radius > 1)
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Handles Operator - Invalid Handles Radius. The valid range is (0, 1]. Request ignored."));
	else
		handles_radius = in_radius / 2.0f;

	arc_points = in_arc_points;
}

void HandlesOperator::SetRotationHandleAppearance(float in_offset, float in_angle, float in_tip_size)
{
	if (in_offset <= 0 || in_offset > 1)
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Handles Operator - Invalid Handles Offset. The valid range is (0, 1]. Request ignored."));
	else
		arc_offset = in_offset;

	if (in_angle < 0)
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Handles Operator - Invalid arc angle. The angle has to be greater than zero. Request ignored."));
	else
		arc_angle = in_angle;

	if (in_tip_size < 0 || in_tip_size > 1)
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Handles Operator - Invalid tip size. The angle has to be between 0 and one. Request ignored."));
	else
		tip_size = in_tip_size;
}

void HandlesOperator::SetPlaneTranslationAppearance(float in_plane_offset, float in_plane_length, float in_center_radius)
{
	plane_offset = in_plane_offset;
	plane_length = in_plane_length;
	center_radius = in_center_radius;
}

void HandlesOperator::SetCADModel(CADModel const & in_cad_model)
{
	if (!in_cad_model.GetAllSubcomponents(HPS::Component::ComponentType::ExchangeDrawingModel).empty())
	{
		Database::GetEventDispatcher().InjectEvent(WarningEvent("Handles Operator - Cannot operate on Exchange Drawing Models. Request ignored."));
		return;
	}

	cad_model = in_cad_model;
	cad_model_type = cad_model.Type();
}

void HandlesOperator::GetHandlesAppearance(float & out_length, float & out_radius, size_t & out_arc_points) const
{
	out_length = handles_length * 2.0f;
	out_radius = handles_radius * 2.0f;
	out_arc_points = arc_points;
}

void HandlesOperator::GetRotationHandleAppearance(float & out_offset, float & out_angle, float & out_tip_size) const
{
	out_offset = arc_offset;
	out_angle = arc_angle;
	out_tip_size = tip_size;
}

void HandlesOperator::GetPlaneTranslationAppearance(float & out_plane_offset, float & out_plane_length, float & out_center_radius) const
{
	out_plane_offset = plane_offset;
	out_plane_length = plane_length;
	out_center_radius = center_radius;
}

void HandlesOperator::AddHandles(HandleType in_handle_type)
{
	switch (in_handle_type)
	{
	case HPS::HandlesOperator::HandleType::Rotation:
	{
		requested_handle_type[HandlesOperator::rotation_x] = true;
		requested_handle_type[HandlesOperator::rotation_y] = true;
		requested_handle_type[HandlesOperator::rotation_z] = true;
	} break;
	case HPS::HandlesOperator::HandleType::RotationX:
		requested_handle_type[HandlesOperator::rotation_x] = true;
	break;
	case HPS::HandlesOperator::HandleType::RotationY:
		requested_handle_type[HandlesOperator::rotation_y] = true;
	break;
	case HPS::HandlesOperator::HandleType::RotationZ:
		requested_handle_type[HandlesOperator::rotation_z] = true;	
	break;
	case HPS::HandlesOperator::HandleType::AxisTranslation:
	{
		requested_handle_type[HandlesOperator::axis_translation_x] = true;
		requested_handle_type[HandlesOperator::axis_translation_y] = true;
		requested_handle_type[HandlesOperator::axis_translation_z] = true;
	} break;
	case HPS::HandlesOperator::HandleType::AxisTranslationX:
		requested_handle_type[HandlesOperator::axis_translation_x] = true;
	break;
	case HPS::HandlesOperator::HandleType::AxisTranslationY:
		requested_handle_type[HandlesOperator::axis_translation_y] = true;
		break;
	case HPS::HandlesOperator::HandleType::AxisTranslationZ:
		requested_handle_type[HandlesOperator::axis_translation_z] = true;
		break;
	case HPS::HandlesOperator::HandleType::PlaneTranslation:
	{
		requested_handle_type[HandlesOperator::plane_translation_x] = true;
		requested_handle_type[HandlesOperator::plane_translation_y] = true;
		requested_handle_type[HandlesOperator::plane_translation_z] = true;
	} break;
	case HPS::HandlesOperator::HandleType::PlaneTranslationXY:
		requested_handle_type[HandlesOperator::plane_translation_x] = true;
		break;
	case HPS::HandlesOperator::HandleType::PlaneTranslationYZ:
		requested_handle_type[HandlesOperator::plane_translation_y] = true;
		break;
	case HPS::HandlesOperator::HandleType::PlaneTranslationZX:
		requested_handle_type[HandlesOperator::plane_translation_z] = true;
		break;
	case HPS::HandlesOperator::HandleType::CenterPoint:
		requested_handle_type[HandlesOperator::center_point_translation] = true;
		break;
	default:
		break;
	}
}

void HandlesOperator::RemoveHandles(HandleType in_handle_type)
{
	switch (in_handle_type)
	{
	case HPS::HandlesOperator::HandleType::Rotation:
	{
		requested_handle_type[HandlesOperator::rotation_x] = false;
		requested_handle_type[HandlesOperator::rotation_y] = false;
		requested_handle_type[HandlesOperator::rotation_z] = false;
	} break;
	case HPS::HandlesOperator::HandleType::RotationX:
		requested_handle_type[HandlesOperator::rotation_x] = false;
		break;
	case HPS::HandlesOperator::HandleType::RotationY:
		requested_handle_type[HandlesOperator::rotation_y] = false;
		break;
	case HPS::HandlesOperator::HandleType::RotationZ:
		requested_handle_type[HandlesOperator::rotation_z] = false;
		break;
	case HPS::HandlesOperator::HandleType::AxisTranslation:
	{
		requested_handle_type[HandlesOperator::axis_translation_x] = false;
		requested_handle_type[HandlesOperator::axis_translation_y] = false;
		requested_handle_type[HandlesOperator::axis_translation_z] = false;
	} break;
	case HPS::HandlesOperator::HandleType::AxisTranslationX:
		requested_handle_type[HandlesOperator::axis_translation_x] = false;
		break;
	case HPS::HandlesOperator::HandleType::AxisTranslationY:
		requested_handle_type[HandlesOperator::axis_translation_y] = false;
		break;
	case HPS::HandlesOperator::HandleType::AxisTranslationZ:
		requested_handle_type[HandlesOperator::axis_translation_z] = false;
		break;
	case HPS::HandlesOperator::HandleType::PlaneTranslation:
	{
		requested_handle_type[HandlesOperator::plane_translation_x] = false;
		requested_handle_type[HandlesOperator::plane_translation_y] = false;
		requested_handle_type[HandlesOperator::plane_translation_z] = false;
	} break;
	case HPS::HandlesOperator::HandleType::PlaneTranslationXY:
		requested_handle_type[HandlesOperator::plane_translation_x] = false;
		break;
	case HPS::HandlesOperator::HandleType::PlaneTranslationYZ:
		requested_handle_type[HandlesOperator::plane_translation_y] = false;
		break;
	case HPS::HandlesOperator::HandleType::PlaneTranslationZX:
		requested_handle_type[HandlesOperator::plane_translation_z] = false;
		break;
	case HPS::HandlesOperator::HandleType::CenterPoint:
		requested_handle_type[HandlesOperator::center_point_translation] = false;
		break;
	default:
		break;
	}
}
