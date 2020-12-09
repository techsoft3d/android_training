// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "sprk_exchange.h"

#if (defined(_MSC_VER) && _MSC_VER >= 1900)
#	pragma warning( push )
#	pragma warning( disable: 4459 ) // declaration of <name> hides global declaration
#endif

#include "A3DSDKIncludes.h"

#if (defined(_MSC_VER) && _MSC_VER >= 1900)
#	pragma warning( pop )
#endif

#include <sstream>
#include <string>

#if defined(TARGET_OS_ANDROID)
#include <sstream>
namespace std
{
	// std::to_string is not available on android
	template < typename T > std::string to_string( const T& n )
	{
		std::ostringstream stm ;
		stm << n ;
		return stm.str() ;
	}
}
#endif

using namespace HPS;

Exchange::MeasurementOperator::MeasurementOperator()
	: Exchange::CommonMeasurementOperator()
	, measurement_type(MeasurementType::PointToPoint)
	, tracked_touch_id(-1)
    , current_touch_id(-1)
	, operator_active(false)
	, end_measurement(false)
	, use_explicit_direction(false)
	, anchors(0)
	, anchors_in_place(false)
	, edge_length(0.0)
	, edge_type(EdgeType::Generic)
	, inverted_measurement(false)
	, highlight_on_mouse_over(false)
	, disable_highlighting(false)
{

}

Exchange::MeasurementOperator::MeasurementOperator(Exchange::CADModel const & in_cad_model, MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Exchange::CommonMeasurementOperator(in_cad_model, in_mouse_trigger, in_modifier_trigger)
	, measurement_type(MeasurementType::PointToPoint)
	, tracked_touch_id(-1)
    , current_touch_id(-1)
	, operator_active(false)
	, end_measurement(false)
	, use_explicit_direction(false)
	, anchors(0)
	, anchors_in_place(false)
	, edge_length(0.0)
	, edge_type(EdgeType::Generic)
	, inverted_measurement(false)
	, highlight_on_mouse_over(false)
	, disable_highlighting(false)
{
	SelectabilityKit selectability;
	selectability.SetEverything(Selectability::Value::Off).SetFaces(Selectability::Value::ForcedOn);

	point_to_point_selection.SetLevel(HPS::Selection::Level::Entity)
		.SetRelatedLimit(6)
		.SetProximity(0.15f)
		.SetSorting(Selection::Sorting::Default)
		.SetAlgorithm(HPS::Selection::Algorithm::Visual)
		.SetSelectability(selectability);

	feature_to_feature_selection.SetLevel(HPS::Selection::Level::Entity)
		.SetRelatedLimit(0)
		.SetProximity(0.15f)
		.SetSorting(Selection::Sorting::Default)
		.SetAlgorithm(HPS::Selection::Algorithm::Visual);

	angle_selection = feature_to_feature_selection;

	selectability.SetEverything(Selectability::Value::Off).SetLines(Selectability::Value::On);

	edge_radius_selection.SetLevel(HPS::Selection::Level::Entity)
		.SetRelatedLimit(6)
		.SetProximity(0.15f)
		.SetSorting(Selection::Sorting::Default)
		.SetAlgorithm(HPS::Selection::Algorithm::Visual)
		.SetSelectability(selectability);

	mouse_over_highlight_options
		.SetStyleName("highlight_style")
		.SetOverlay(Drawing::Overlay::InPlace);

	edit_measurement_highlight_options
		.SetStyleName("highlight_style")
		.SetOverlay(Drawing::Overlay::Default);
}

Exchange::MeasurementOperator::Surface::Surface()
	: surface_type(SurfaceType::Unsupported)
{
}

void Exchange::MeasurementOperator::SetMeasurementType(MeasurementType in_measurement_type)
{
	if (in_measurement_type != measurement_type)
	{
		ResetMeasurement();
		measurement_type = in_measurement_type;
	}
}

Exchange::MeasurementOperator::MeasurementType Exchange::MeasurementOperator::GetMeasurementType()
{
	return measurement_type;
}

void Exchange::MeasurementOperator::SetMouseOverHighlighting(bool in_highlighting)
{
	highlight_on_mouse_over = in_highlighting;
}

void Exchange::MeasurementOperator::SetMouseOverHighlighting(bool in_highlighting, HighlightOptionsKit const & in_highlight_options_kit)
{
	highlight_on_mouse_over = in_highlighting;
	mouse_over_highlight_options = in_highlight_options_kit;
	edit_measurement_highlight_options = in_highlight_options_kit;
	edit_measurement_highlight_options.SetOverlay(Drawing::Overlay::Default);
}

bool Exchange::MeasurementOperator::GetMouseOverHighlighting()
{
	return highlight_on_mouse_over;
}

HPS::HighlightOptionsKit Exchange::MeasurementOperator::GetHighlightOptions()
{
	return mouse_over_highlight_options;
}

void Exchange::MeasurementOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	Exchange::CommonMeasurementOperator::OnViewAttached(in_attached_view);

	auto layouts = in_attached_view.GetOwningLayouts();
	for (auto const & layout : layouts)
	{
		auto c = layout.GetOwningCanvases();
		canvases.insert(canvases.end(), c.begin(), c.end());
	}
}

void Exchange::MeasurementOperator::OnViewDetached(HPS::View const &)
{
	ResetMeasurement();
	canvases.clear();
}

bool Exchange::MeasurementOperator::OnMouseDown(MouseState const & in_state)
{
	WindowKey window = in_state.GetEventSource();
	size_t click_count = in_state.GetActiveEvent().ClickCount;

	MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;

	if (measurement_to_edit == MeasurementType::PointToPoint ||
		measurement_to_edit == MeasurementType::FaceAngle)
	{
		if (IsMouseTriggered(in_state) && !manipulate_measurement && anchors < 2)
			return InputDown(in_state.GetLocation(), window, in_state.GetEventPath(), click_count);
		else if (IsMouseTriggered(in_state) && 
			measurement_to_edit == MeasurementType::FaceAngle &&
			click_count == 2)
			return InputDown(in_state.GetLocation(), window, in_state.GetEventPath(), click_count);
		else if (measurement_type == MeasurementType::PointToPoint && !manipulate_measurement && anchors == 2)
			return true;
	}

	else if (measurement_to_edit == MeasurementType::EdgeAndRadius ||
		measurement_to_edit == MeasurementType::FeatureToFeature)
	{
		if (IsMouseTriggered(in_state) && operator_active)
		{
			end_measurement = true;
			return true;
		}

		//this condition can probably be collapsed with the point-to-point case, adding !manipulate_measurement should have no consequences
		if (IsMouseTriggered(in_state))
			return InputDown(in_state.GetLocation(), window, in_state.GetEventPath(), click_count);
	}
	return false;
}

bool Exchange::MeasurementOperator::OnTouchDown(TouchState const & in_state)
{
    TouchArray touches = in_state.GetActiveEvent().Touches;
    
    if (touches.size() != 1)
        return false;
    
    WindowKey window = in_state.GetEventSource();
    size_t tap_count = touches[0].TapCount;
    current_touch_id = touches[0].ID;
    
    MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;
    
    if (measurement_to_edit == MeasurementType::PointToPoint ||
        measurement_to_edit == MeasurementType::FaceAngle)
    {
        if (!manipulate_measurement && anchors < 2)
            return InputDown(touches[0].Location, window, in_state.GetEventPath(), tap_count);
        else if (measurement_to_edit == MeasurementType::FaceAngle &&
                 tap_count == 2)
            return InputDown(touches[0].Location, window, in_state.GetEventPath(), tap_count);
        else if (measurement_type == MeasurementType::PointToPoint && !manipulate_measurement && anchors == 2)
            return true;
    }
    
    else if (measurement_to_edit == MeasurementType::EdgeAndRadius ||
             measurement_to_edit == MeasurementType::FeatureToFeature)
    {
        if (operator_active)
        {
            end_measurement = true;
            return true;
        }
        
        //this condition can probably be collapsed with the point-to-point case, adding !manipulate_measurement should have no consequences
        return InputDown(touches[0].Location, window, in_state.GetEventPath(), tap_count);
    }
    
    return false;
}

bool Exchange::MeasurementOperator::InputDown(WindowPoint const & in_location, WindowKey & in_window, KeyPath const & in_path, size_t number_of_clicks)
{
	if (!highlighted_path.Empty())
	{
		auto highlight_control = in_window.GetHighlightControl();
		highlight_control.Unhighlight(highlighted_path, mouse_over_highlight_options);
		highlight_control.Unhighlight(edit_measurement_highlight_options);
		highlighted_path = KeyPath();
	}

	SelectionOptionsKit specific_kit;
	switch (measurement_type)
	{
	case MeasurementType::PointToPoint:
		specific_kit = point_to_point_selection;
		break;
	case MeasurementType::EdgeAndRadius:
		specific_kit = edge_radius_selection;
		break;
	case MeasurementType::FeatureToFeature:
		specific_kit = feature_to_feature_selection;
		break;
	case MeasurementType::FaceAngle:
		specific_kit = angle_selection;
		break;
	}

	//first do a selection to see if we are hitting a navigation aid
	SelectionResults results;
	if (in_window.GetSelectionControl().SelectByPoint(in_location, selection_options, results) > 0)
	{
		auto navigation_keys = GetAttachedView().GetNavigationKeys();
		Key key;
		results.GetIterator().GetItem().ShowSelectedItem(key);
		if (std::find(navigation_keys.begin(), navigation_keys.end(), key) != navigation_keys.end())
			return false;
	}

	KeyPath event_path(in_path);
	event_path = GetAttachedView().GetAttachedModel().GetSegmentKey() 
		+ GetAttachedView().GetAttachedModelIncludeLink() 
		+ GetAttachedView().GetModelOverrideSegmentKey() + event_path;
	specific_kit.SetScope(event_path);
	selection_options.SetScope(event_path);

	if (in_window.GetSelectionControl().SelectByPoint(in_location, specific_kit, results) == 0)
	{
		if (measurement_type == MeasurementType::FaceAngle &&
			anchors != 0 &&
			number_of_clicks != 2)
			return false;

		//if nothing was selected, try again with the default selection options, see if we are selecting a previously entered measurement
		if (in_window.GetSelectionControl().SelectByPoint(in_location, selection_options, results) == 0)
		{
			disable_highlighting = true;
			return false;
		}
		else
		{
			//check if we indeed selected a measurement
			auto it = results.GetIterator();
			Key selected_key;
			WorldPoint world_point;
			it.GetItem().ShowSelectedItem(selected_key);
			it.GetItem().ShowSelectionPosition(world_point);

			//check if it has the user data which indicates it is a measurement
			SegmentKey selected_segment = (SegmentKey)selected_key.Owner();
			ByteArray data;
			if (selected_segment.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data) &&
				strcmp((const char *)data.data(), "hps_measurement_segment") == 0)
			{
				RestoreMeasurement(selected_segment);
                
                tracked_touch_id = current_touch_id;
                bool event_generated_from_touch = tracked_touch_id == -1 ? false : true;
                if (event_generated_from_touch)
                    anchors_in_place = true;

				if (measurement_type == MeasurementType::FaceAngle && number_of_clicks == 2)
				{
					InvertMeasuredAngle(in_window);
					manipulate_measurement = false;
					anchors = 0;
				}
				else if (!event_generated_from_touch)
				{
					in_window.GetHighlightControl().Highlight(current_measurement, edit_measurement_highlight_options);
					GetAttachedView().Update();
				}

				return true;
			}
		}
	}

	else
	{
		Key selected_key;
		WorldPoint world_point;
		WindowPoint window_point;
		KeyPath selection_path;

		if (measurement_type != MeasurementType::EdgeAndRadius)
		{
			SelectionItem selection_item = results.GetIterator().GetItem();
			selection_item.ShowSelectionPosition(world_point);
			selection_item.ShowSelectionPosition(window_point);
			selection_item.ShowSelectedItem(selected_key);
			selection_item.ShowPath(selection_path);
		}
		else
		{
			auto it = results.GetIterator();
			while (it.IsValid())
			{
				it.GetItem().ShowSelectedItem(selected_key);
				it.GetItem().ShowPath(selection_path);
				it.GetItem().ShowSelectionPosition(window_point);
				if (selected_key.Type() == HPS::Type::LineKey)
					break;

				it.Next();
			}
		}

		switch (measurement_type)
		{
		case MeasurementType::PointToPoint:
			InsertPointToPointMeasurement(world_point);
			break;
		case MeasurementType::EdgeAndRadius:
			return InsertEdgeRadiusMeasurement(selection_path, window_point, selected_key);
			break;
		case MeasurementType::FeatureToFeature:
			return InsertFeatureToFeatureMeasurement(selection_path, window_point, selected_key, in_window);
			break;
		case MeasurementType::FaceAngle:
			return InsertAngleMeasurement(selection_path, window_point, selected_key, in_window, results);
			break;
		}
		
	}
	return true;
}

void Exchange::MeasurementOperator::InsertPointToPointMeasurement(Point const & in_world_point)
{
	if (!anchors_in_place)
	{
		//each measurement has its own segment
		if (current_measurement.Empty())
			current_measurement = measurement_segment.Subsegment(CommonMeasurementOperator::GetNewMeasurementSegmentName("Distance"));

		++anchors;

		if (anchors == 1)
		{
			//first click: insert a marker to represent the first anchor
			anchor_one = current_measurement.InsertMarker(in_world_point);
			first_click_position = in_world_point;
		}
		else if (anchors == 2)
		{
			//second click: insert a second marker to represent the second anchor.
			//also insert the rest of the geometry representing the measurement.
			second_click_position = in_world_point;
			anchor_two = current_measurement.InsertMarker(in_world_point);
			line_to_cursor = current_measurement.InsertLine(first_click_position, second_click_position);

			SegmentKey patterned_line_segment = current_measurement.Subsegment("patterned_line");
			patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_in");
			distance_line = patterned_line_segment.InsertLine(first_click_position, second_click_position);

			leader_line_one = current_measurement.InsertLine(first_click_position, first_click_position);
			leader_line_two = current_measurement.InsertLine(second_click_position, second_click_position);
			distance_point_one = first_click_position;
			distance_point_two = second_click_position;

			Vector length_vector(second_click_position - first_click_position);
			double length = length_vector.Length();
			std::stringstream length_text;
			length_text.precision(measurement_precision);
			length_text << std::fixed << length;
			if (!units.Empty())
				length_text << " " << units;
			text_string = HPS::UTF8(length_text.str().data(), "utf8");
			text = current_measurement.InsertText(second_click_position, text_string);

			measurement_direction = Vector(second_click_position - first_click_position);
			measurement_direction = measurement_direction.Normalize();
            
            //If this measurement came from a touch event, record the touch ID of the touch which inserted the second anchor point
            //and allow the user to move the inserted measurement right away
            tracked_touch_id = current_touch_id;
            bool event_generated_from_touch = tracked_touch_id == -1 ? false : true;
            if (event_generated_from_touch)
                anchors_in_place = true;

			GetCameraDirection();
		}

		GetAttachedView().Update();
	}
	else
	{
		anchors_in_place = false;
		anchors = 0;
	}
}

bool Exchange::MeasurementOperator::InsertEdgeRadiusMeasurement(KeyPath const & in_selection_path, WindowPoint & in_selection_position, Key const & in_edge_key)
{
	WindowPoint window_selection_position = in_selection_position;
	in_selection_path.ConvertCoordinate(Coordinate::Space::Window, in_selection_position, Coordinate::Space::World, in_selection_position);

	//this edge key should belong to a component of type exchange topological edge.
	//if this is not the case if probably means the file was imported without b-rep, and there is nothing this operator can do.
	HPS::Exchange::Component edge_component = cad_model.GetComponentPath(HPS::KeyPath(in_edge_key + in_selection_path)).Front();
	if (edge_component.GetComponentType() != Component::ComponentType::ExchangeTopoEdge)
		return false;

	//calculate the length and type of the edge
	GetEdgeLengthAndType(edge_component);

	//get net modelling matrix from model segment to selection segment
	KeyArray selection_keys;
	in_selection_path.ShowKeys(selection_keys);
	KeyArray shortened_keys;
	bool start_copying_keys = false;
	SegmentKey model_key = GetAttachedView().GetAttachedModel().GetSegmentKey();
	for (auto rit = selection_keys.rbegin(), re = selection_keys.rend(); rit != re; ++rit)
	{
		if (*rit == model_key)
			start_copying_keys = true;

		if (start_copying_keys)
			shortened_keys.push_back(*rit);
	}
	std::reverse(shortened_keys.begin(), shortened_keys.end());
	KeyPath shortened_path(shortened_keys);
	MatrixKit net_modelling_matrix;
	shortened_path.ShowNetModellingMatrix(net_modelling_matrix);

	if (edge_type == Exchange::MeasurementOperator::EdgeType::Line)
	{
		current_measurement = measurement_segment.Subsegment(CommonMeasurementOperator::GetNewMeasurementSegmentName("Length"));

		auto get_face_normal = [&](HPS::Exchange::Component const & in_coedge, Vector & out_normal)
		{
			auto loop = in_coedge.GetOwners();
			auto face = loop[0].GetOwners();
			HPS::Exchange::Component topo_face(face[0]);
			if (IsPlane(topo_face))
			{
				ShellKey shell = (ShellKey)topo_face.GetKeys()[0];
				VectorArray normals;
				shell.ShowNetVertexNormalsByRange(0, 1, normals);
				out_normal = normals[0];
				out_normal = net_modelling_matrix.Transform(out_normal);
			}
		};

		GetCameraDirection();
		LineKey line_key(in_edge_key);
		PointArray points;
		line_key.ShowPoints(points);
		first_click_position = points[0];
		second_click_position = points.back();
		first_click_position = net_modelling_matrix.Transform(first_click_position);
		second_click_position = net_modelling_matrix.Transform(second_click_position);
		measurement_direction = Vector(second_click_position - first_click_position);
		measurement_direction = measurement_direction.Normalize();

		Vector normal_one = Vector::Zero();
		Vector normal_two = Vector::Zero();
		auto co_edges = edge_component.GetOwners();
		if (co_edges.size() >= 1)
			get_face_normal(co_edges[0], normal_one);
		if (co_edges.size() == 2)
			get_face_normal(co_edges[1], normal_two);

		if (normal_one != Vector::Zero() || normal_two != Vector::Zero())
		{
			use_explicit_direction = true;

			//we need to choose whether to use normal_one or normal two
			Vector best_normal;
			if (normal_one == Vector::Zero())
			{
				float dot = fabs(camera_direction.Dot(normal_two));
				if (HPS::Float::Equals(dot, 1.0f))
					explicit_direction = normal_two.Cross(measurement_direction);
				else
				{
					Vector temp = normal_two.Cross(measurement_direction);
					if (fabs(camera_direction.Dot(temp)) >= fabs(camera_direction.Dot(normal_two)))
						explicit_direction = normal_two;
					else
						explicit_direction = temp;
				}
				explicit_direction = explicit_direction.Normalize();
			}
			else if (normal_two == Vector::Zero())
			{
				float dot = fabs(camera_direction.Dot(normal_one));
				if (HPS::Float::Equals(dot, 1.0f))
					explicit_direction = normal_one.Cross(measurement_direction);
				else
				{
					Vector temp = normal_one.Cross(measurement_direction);
					if (fabs(camera_direction.Dot(temp)) >= fabs(camera_direction.Dot(normal_one)))
						explicit_direction = normal_one;
					else
						explicit_direction = temp;
				}
				explicit_direction = explicit_direction.Normalize();
			}
			else
			{
				if (fabs(camera_direction.Dot(normal_one)) <= fabs(camera_direction.Dot(normal_two)))
					best_normal = normal_two;
				else
					best_normal = normal_one;

				explicit_direction = measurement_direction.Cross(best_normal);
				explicit_direction = explicit_direction.Normalize();
			}
		}
		else if (co_edges.size() == 2)
		{
			//the edge is adjacent to two non-planar faces
			//check whether one is a cylindrical/conical one

			auto loop = co_edges[0].GetOwners();
			auto face = loop[0].GetOwners();
			Exchange::MeasurementOperator::Surface temp_surface_one;
			GetSurfaceType(Exchange::Component(face[0]), temp_surface_one);

			loop = co_edges[1].GetOwners();
			face = loop[0].GetOwners();
			Exchange::MeasurementOperator::Surface temp_surface_two;
			GetSurfaceType(Exchange::Component(face[0]), temp_surface_two);

			Point normal_points[2];
			if (temp_surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
				temp_surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder)
			{
				Exchange::MeasurementOperator::Surface temp_surface = temp_surface_one;
				BoundingKit bounding;
				shortened_path.ShowNetBounding(true, bounding);

				SimpleSphere sphere;
				SimpleCuboid cuboid;
				bounding.ShowVolume(sphere, cuboid);
				float half_diagonal = (float)(cuboid.Diagonal().Length() / 2);

				MatrixKit matrix;
				shortened_path.ShowNetModellingMatrix(matrix);
				temp_surface.center = matrix.Transform(temp_surface.center);
				temp_surface.normal = matrix.Transform(temp_surface.normal);
				temp_surface.normal = temp_surface.normal.Normalize();

				//recompute the surface center so that it corresponds to the face bbx center
				Vector center_to_bbx_center(sphere.center - temp_surface.center);
				temp_surface.center = temp_surface.center + temp_surface.normal * temp_surface.normal.Dot(center_to_bbx_center);

				normal_points[0] = temp_surface.center + temp_surface.normal * half_diagonal;
				normal_points[1] = temp_surface.center - temp_surface.normal * half_diagonal;
			}

			//orient the measurement based on the center-line of the conical/cylindrical surface associated with the measured edge
			Vector normal_direction(normal_points[1] - normal_points[0]);
			Vector unit_normal_direction = normal_direction.Normalize();

			Point point_one;
			Vector point_one_to_normal(first_click_position - normal_points[0]);
			Vector point_two_to_normal(second_click_position - normal_points[0]);
			Vector shortest_distance;
			if (point_one_to_normal.Length() < point_two_to_normal.Length())
			{
				point_one = first_click_position;
				shortest_distance = point_one_to_normal;
			}
			else
			{
				point_one = second_click_position;
				shortest_distance = point_two_to_normal;
			}
			Point center = normal_points[0] + unit_normal_direction * shortest_distance.Dot(normal_direction);

			use_explicit_direction = true;
			Vector right_normal = Vector(point_one - center).Cross(measurement_direction);
			right_normal = right_normal.Normalize();
			explicit_direction = right_normal.Cross(measurement_direction);

			//if we are looking exactly in the direction of the measurement, change the measurement direction
			float dot_p = fabs(explicit_direction.Dot(camera_direction));
			float epsilon = 0.0001f;
			if (fabs(dot_p - 1) < epsilon)
				explicit_direction = right_normal;
			else
			{
				//if we are looking down the center line of the conical/cylindrical surface associated with the measured edge, change the measurement direction
				dot_p = fabs(unit_normal_direction.Dot(camera_direction));
				if (fabs(dot_p - 1) < epsilon)
					explicit_direction = right_normal;
			}

			explicit_direction = explicit_direction.Normalize();
		}

		//setup geometry for a line measurement
		edge_line = current_measurement.InsertLine(first_click_position, second_click_position);
		leader_line_one = current_measurement.InsertLine(first_click_position, first_click_position);
		leader_line_two = current_measurement.InsertLine(second_click_position, second_click_position);
		line_to_cursor = current_measurement.InsertLine(second_click_position, second_click_position);

		SegmentKey patterned_line_segment = current_measurement.Subsegment("patterned_line");
		patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_in");
		distance_line = patterned_line_segment.InsertLine(first_click_position, second_click_position);

		std::stringstream length_text;
		length_text.precision(measurement_precision);
		length_text << std::fixed << edge_length;
		if (!units.Empty())
			length_text << " " << units;
		text_string = HPS::UTF8(length_text.str().data(), "utf8");
		text = current_measurement.InsertText(second_click_position, text_string);

        tracked_touch_id = current_touch_id;
		operator_active = true;
		GetAttachedView().Update();

		return true;
	}
	else if (edge_type == Exchange::MeasurementOperator::EdgeType::Circle)
	{
		current_measurement = measurement_segment.Subsegment(CommonMeasurementOperator::GetNewMeasurementSegmentName("Radius"));

		//setup geometry for a radius measurement
		LineKey line_key(in_edge_key);
		PointArray points;
		line_key.ShowPoints(points);
		points = net_modelling_matrix.Transform(points);
		edge_line = current_measurement.InsertLine(points);

		circle_center = net_modelling_matrix.Transform(circle_center);
		center_marker = current_measurement.InsertMarker(circle_center);

		SegmentKey patterned_line_segment = current_measurement.Subsegment("patterned_line");
		patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_in");
		distance_line = patterned_line_segment.InsertLine(circle_center, in_selection_position);
		line_to_cursor = current_measurement.InsertLine(in_selection_position, in_selection_position);

		std::stringstream radius_text;
		radius_text.precision(measurement_precision);
		radius_text << std::fixed << "R" << radius;
		if (!units.Empty())
			radius_text << " " << units;
		text_string = HPS::UTF8(radius_text.str().data(), "utf8");
		text = current_measurement.InsertText(in_selection_position, text_string);

        tracked_touch_id = current_touch_id;
		operator_active = true;
		GetCameraDirection();
		GetAttachedView().Update();

		return true;
	}
	else if (edge_type == Exchange::MeasurementOperator::EdgeType::Generic)
	{
		GetCameraDirection();
		current_measurement = measurement_segment.Subsegment(CommonMeasurementOperator::GetNewMeasurementSegmentName("Length"));

		Plane plane(camera_direction, first_click_position);
		Point intersection = GetPlaneIntersection(plane, in_selection_path, window_selection_position);

		LineKey line_key(in_edge_key);
		PointArray points;
		line_key.ShowPoints(points);
		points = net_modelling_matrix.Transform(points);
		edge_line = current_measurement.InsertLine(points);
		first_click_position = points[(size_t)HPS::Floor(points.size() / 2)];
		line_to_cursor = current_measurement.InsertLine(first_click_position, intersection);

		std::stringstream length_text;
		length_text.precision(measurement_precision);
		length_text << std::fixed << edge_length;
		if (!units.Empty())
			length_text << " " << units;
		text_string = HPS::UTF8(length_text.str().data(), "utf8");
		text = current_measurement.InsertText(intersection, text_string);

        tracked_touch_id = current_touch_id;
		operator_active = true;
		GetAttachedView().Update();

		return true;
	}

	return false;
}

bool Exchange::MeasurementOperator::InsertFeatureToFeatureMeasurement(KeyPath const & in_selection_path, WindowPoint & in_selection_position, Key const & in_face_key, WindowKey & in_window)
{
	in_selection_path.ConvertCoordinate(Coordinate::Space::Window, in_selection_position, Coordinate::Space::World, in_selection_position);

	//this face key should belong to a component of type exchange topological face.
	//if this is not the case if probably means the file was imported without b-rep, and there is nothing this operator can do.
	ComponentPath current_path = cad_model.GetComponentPath(HPS::KeyPath(in_face_key + in_selection_path));
	HPS::Exchange::Component const & face_component = current_path.Front();
	if (face_component.GetComponentType() != Component::ComponentType::ExchangeTopoFace)
		return false;

	//find the simplified surface type used by this component
	if (anchors == 0)
	{
		GetSurfaceType(face_component, surface_one);
		if (surface_one.surface_type == Surface::SurfaceType::Unsupported)
			return false;
		surface_one.path = current_path;
	}
	else
	{
		if (current_path == surface_one.path)
			return false;

		GetSurfaceType(face_component, surface_two);
		if (surface_two.surface_type == Surface::SurfaceType::Unsupported)
			return false;

		surface_two.path = current_path;
	}
	++anchors;

	//highlight the face we selected
	KeyPath complete_path = in_face_key + in_selection_path;
	in_window.GetHighlightControl().Highlight(complete_path, highlight_options);

	auto draw_normal = [](Surface & surface, SegmentKey & insert_here, LineKey & normal)
	{
		if (surface.surface_type == Surface::SurfaceType::ConeOrCylinder)
		{
			KeyPath path = surface.path.GetKeyPaths()[0];
			BoundingKit bounding;
			path.ShowNetBounding(true, bounding);

			SimpleSphere sphere;
			SimpleCuboid cuboid;
			bounding.ShowVolume(sphere, cuboid);
			float half_diagonal = (float)(cuboid.Diagonal().Length() / 2);

			MatrixKit matrix;
			path.ShowNetModellingMatrix(matrix);
			surface.center = matrix.Transform(surface.center);
			surface.normal = matrix.Transform(surface.normal);
			surface.normal = surface.normal.Normalize();

			//recompute the surface center so that it corresponds to the face bbx center
			Vector center_to_bbx_center(sphere.center - surface.center);
			surface.center = surface.center + surface.normal * surface.normal.Dot(center_to_bbx_center);

			Point point_one = surface.center + surface.normal * half_diagonal;
			Point point_two = surface.center - surface.normal * half_diagonal;
			surface.normal_points = insert_here.InsertLine(point_one, point_two);
			normal = surface.normal_points;
		}
	};

	if (current_measurement.Empty())
		current_measurement = measurement_segment.Subsegment(CommonMeasurementOperator::GetNewMeasurementSegmentName("Distance"));

	if (anchors == 1)
		draw_normal(surface_one, current_measurement, current_normal);

	if (anchors == 2)
	{
		draw_normal(surface_two, current_measurement, current_normal);
		operator_active = true;
        tracked_touch_id = current_touch_id;
        bool event_generated_from_touch = tracked_touch_id == -1 ? false : true;
        if (event_generated_from_touch)
            anchors_in_place = true;

		GetCameraDirection();

		//draw the measurement
		if ((surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
			surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::Plane) ||
			(surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
			surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::Plane))
		{
			PlaneToCenterLineDistance();
		}
		else if (surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::Plane &&
			surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::Plane)
		{
			PlaneToPlaneDistance();
		}
		else if (surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
			surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder)
		{
			LineToLineDistance();
		}

		if (surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
			surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder)
		{
			float dot_p = fabs(surface_one.normal.Dot(surface_two.normal));
			if (HPS::Float::Equals(dot_p, 1))
			{
				//the two normals are parallel
				explicit_direction = surface_one.normal;
				use_explicit_direction = true;
			}
			else
			{
				float length = (float)Vector(second_click_position - first_click_position).Length();
				if (length > 0)
				{
					Vector direction_of_measurement(first_click_position - second_click_position);
					direction_of_measurement = direction_of_measurement.Normalize();

					if (surface_one.normal.Dot(camera_direction) >= surface_two.normal.Dot(camera_direction))
						explicit_direction = surface_one.normal.Cross(direction_of_measurement);
					else
						explicit_direction = surface_two.normal.Cross(direction_of_measurement);

					use_explicit_direction = true;
				}
			}
		}
	}

	GetAttachedView().Update();
	return true;
}

void Exchange::MeasurementOperator::InsertFeatureToFeatureGeometry(Point const & point_one, Point const & point_two, float distance)
{
	SegmentKey patterned_line_segment = current_measurement.Subsegment("patterned_line");
	patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_in");
	distance_line = patterned_line_segment.InsertLine(point_one, point_two);
	first_click_position = point_one;
	second_click_position = point_two;
	leader_line_one = current_measurement.InsertLine(first_click_position, first_click_position);
	leader_line_two = current_measurement.InsertLine(second_click_position, second_click_position);
	anchor_one = current_measurement.InsertMarker(first_click_position);
	anchor_two = current_measurement.InsertMarker(second_click_position);
	line_to_cursor = current_measurement.InsertLine(first_click_position, first_click_position);

	distance_point_one = first_click_position;
	distance_point_two = second_click_position;

	std::stringstream length_text;
	length_text.precision(measurement_precision);
	length_text << std::fixed << distance;
	if (!units.Empty())
		length_text << " " << units;
	text_string = HPS::UTF8(length_text.str().data(), "utf8");

	text = current_measurement.InsertText(point_two, text_string);
}

bool Exchange::MeasurementOperator::InsertAngleMeasurement(KeyPath const & in_selection_path, WindowPoint & in_selection_position, Key const & in_face_key, WindowKey & in_window, SelectionResults const & in_selection_results)
{
	inverted_measurement = false;
	in_selection_path.ConvertCoordinate(Coordinate::Space::Window, in_selection_position, Coordinate::Space::World, in_selection_position);

	//this edge key should belong to a component of type exchange topological edge.
	//if this is not the case if probably means the file was imported without b-rep, and there is nothing this operator can do.
	HPS::Exchange::Component const & face_component = cad_model.GetComponentPath(HPS::KeyPath(in_face_key + in_selection_path)).Front();
	if (face_component.GetComponentType() != Component::ComponentType::ExchangeTopoFace)
		return false;

	//this operator only accepts faces whose underlying surface is planar
	if (!IsPlane(face_component))
		return false;

	//get the normals
	ShellKey shell(face_component.GetKeys()[0]);
	VectorArray normals;
	shell.ShowNetVertexNormalsByRange(0, 1, normals);
	HPS::KeyPath shell_path(in_face_key + in_selection_path);
	MatrixKit net_matrix;
	shell_path.ShowNetModellingMatrix(net_matrix);
	normals = net_matrix.Transform(normals);
	normals[0] = normals[0].Normalize();

	if (anchors == 0)
		first_face_normal = normals[0];
	else
		second_face_normal = normals[0];

	if (current_measurement.Empty())
		current_measurement = measurement_segment.Subsegment(CommonMeasurementOperator::GetNewMeasurementSegmentName("Angle"));

	++anchors;

	//if this is the second anchor, check that it is not parallel to the first one
	if (anchors == 2)
	{
		float dot_product = fabs(first_face_normal.Dot(second_face_normal));
		if (Float::Equals(dot_product, 1))
		{
			--anchors;
			return false;
		}
	}

	//highlight the face we selected
	in_window.GetHighlightControl().Highlight(in_selection_results, highlight_options);

	//if they are not parallel, insert a marker to and record their positions
	MarkerKey temporary;
	if (anchors < 3)
		temporary = current_measurement.InsertMarker(in_selection_position);
	if (anchors == 1)
	{
		first_click_position = in_selection_position;
		anchor_one = temporary;
	}
	else
	{
		second_click_position = in_selection_position;
		anchor_two = temporary;
	}

	if (anchors == 2)
	{
        tracked_touch_id = current_touch_id;
        bool event_generated_from_touch = tracked_touch_id == -1 ? false : true;
        if (event_generated_from_touch)
            anchors_in_place = true;
        
		GetCameraDirection();

		//the line_to_leader_line is a line obtained by crossing the two face normals
		//we draw this line starting from the first point and continuing in its direction
		//by the distance between the two points, projected on the unit vector describing the direction of line_to_leader_line
		Vector line_to_leader_line_direction = second_face_normal.Cross(first_face_normal);
		Vector point_two_to_point_one(second_click_position - first_click_position);
		float point_distance = point_two_to_point_one.Dot(line_to_leader_line_direction.Normalize());
		Point leader_line_one_start = first_click_position + line_to_leader_line_direction.Normalize() * point_distance;
		line_to_leader_line = current_measurement.InsertLine(first_click_position, leader_line_one_start);

		//the second leader line is just a dot, since it starts and ends at the location where the second point was inserted
		//this will become a proper line in InputMove, as the user moves the mouse/finger
		//its direction is obtained by crossing the second face normal with the direction of line_to_leader_line
		leader_line_two_direction = second_face_normal.Cross(line_to_leader_line_direction);
		leader_line_two_direction = leader_line_two_direction.Normalize();
		leader_line_two = current_measurement.InsertLine(second_click_position, second_click_position);

		//the first leader line direction is obtained by crossing the first face normal with the direction of line_to_leader_line
		leader_line_one_direction = line_to_leader_line_direction.Cross(first_face_normal);
		leader_line_one_direction = leader_line_one_direction.Normalize();

		HPS::BoundingKit model_bounding;
		float scene_size = 1000.0f;
		if (GetAttachedView().GetAttachedModel().GetSegmentKey().ShowBounding(model_bounding))
		{
			HPS::SimpleSphere sphere;
			HPS::SimpleCuboid cuboid;
			model_bounding.ShowVolume(sphere, cuboid);
			scene_size = sphere.radius * 2.5f;
		}

		//get the circle center
		Plane plane(second_click_position, second_face_normal);
		Point one = leader_line_one_start + leader_line_one_direction * scene_size;
		Point two = leader_line_one_start - leader_line_one_direction * scene_size;
		circle_center = plane.IntersectLineSegment(one, two);

		//calculate angle 
		//always select the smaller of the two possible angles
		Vector direction_one(circle_center - leader_line_one_start);
		direction_one = direction_one.Normalize();
		Vector direction_two(circle_center - second_click_position);
		direction_two = direction_two.Normalize();
		float dot_p = direction_one.Dot(direction_two);
		float angle = HPS::ACos(dot_p);
		if (angle > 180)
			angle = 360 - angle;

		std::stringstream angle_text;
		angle_text.precision(measurement_precision);
		angle_text << std::fixed << angle << "\xC2\xB0";
		text_string = HPS::UTF8(angle_text.str().data(), "utf8");
		text = current_measurement.InsertText(second_click_position, text_string);

		//if the smaller of the two possible angles between normals is not the one we are drawing,
		//change the direction of one of the normals
		Vector center_to_point_two = (second_click_position - circle_center);
		center_to_point_two = center_to_point_two.Normalize();

		dot_p = center_to_point_two.Dot(leader_line_one_direction);
		float drawn_angle = HPS::ACos(dot_p);
		if (dot_p > 0)
			drawn_angle = 180 - drawn_angle;
		bool no_leader_line = false;
		if (!HPS::Float::Equals(drawn_angle, angle))
		{
			leader_line_two_direction *= -1;
			no_leader_line = true;
		}
		else
		{
			dot_p = direction_two.Dot(leader_line_two_direction);
			if (dot_p > 0)
			{
				leader_line_two_direction *= -1;
				no_leader_line = true;
			}
		}

		//the radius of the circle is given by the distance between the second point and the circle center
		float circle_radius = float(Vector(second_click_position - circle_center).Length());

		//we find the point where leader_line_one ends, by starting at the circle center, and walking down 
		//the direction of leader_line_one by the radius
		Point leader_line_one_end;
		if (no_leader_line)
			leader_line_one_end = leader_line_one_start;
		else
		{
			Vector start_to_circle_center(circle_center - leader_line_one_start);
			start_to_circle_center = start_to_circle_center.Normalize();
			dot_p = leader_line_one_direction.Dot(start_to_circle_center);
			float radius_difference = float(Vector(circle_center - leader_line_one_start).Length());

			if (dot_p < 0)
				leader_line_one_end = leader_line_one_start + leader_line_one_direction * (circle_radius - radius_difference);
			else
				leader_line_one_end = leader_line_one_start - leader_line_one_direction * (circle_radius - radius_difference);
		}

		leader_line_one = current_measurement.InsertLine(leader_line_one_start, leader_line_one_end);

		//in order to insert the circular arc which represents our measurement, we need three points on the arc
		//the start point if the end of leader_line_one
		//the end point is the second point
		CircularArcKit circular_arc_measurement_kit;
		circular_arc_measurement_kit.SetStart(leader_line_one_end).SetEnd(second_click_position);

		//to find the third point we create a line between start and finish, then pick a point half way through it
		//from the circle center, we move in the direction of this point by the length of the radius.
		//the resulting point should be the middle point we are looking for
		Vector secant(second_click_position - leader_line_one_end);
		float length = (float)secant.Length();
		secant = secant.Normalize();
		Point point_on_line = leader_line_one_end + secant * length / 2;

		Vector middle_point_direction(point_on_line - circle_center);
		middle_point_direction = middle_point_direction.Normalize();

		Point point_on_circle = circle_center + middle_point_direction * circle_radius;
		circular_arc_measurement_kit.SetMiddle(point_on_circle);

		mid_point_direction = Vector(point_on_circle - circle_center);
		mid_point_direction = mid_point_direction.Normalize();
		measurement_arc = current_measurement.InsertCircularArc(circular_arc_measurement_kit);

		//add a line going from the end of leader_line_two to the cursor. Here this line is just a dot
		line_to_cursor = current_measurement.InsertLine(second_click_position, second_click_position);
	}

	GetAttachedView().Update();
	return true;
}

void Exchange::MeasurementOperator::InvertMeasuredAngle(WindowKey & in_window)
{
	inverted_measurement = !inverted_measurement;
	mid_point_direction = -mid_point_direction;

	Point middle_point;
	measurement_arc.ShowMiddle(middle_point);

	Vector center_to_middle_point_direction(middle_point - circle_center);
	float measure_radius = (float)center_to_middle_point_direction.Length();
	center_to_middle_point_direction = center_to_middle_point_direction.Normalize();
	center_to_middle_point_direction *= -1;

	Point new_middle_point = circle_center + center_to_middle_point_direction * measure_radius;
	measurement_arc.SetMiddle(new_middle_point);

	Point text_position;
	text.ShowPosition(text_position);

	PointArray leader_line_points;
	leader_line_one.ShowPoints(leader_line_points);
	Vector direction_one(circle_center - leader_line_points[0]);
	direction_one = direction_one.Normalize();
	Vector direction_two(circle_center - second_click_position);
	direction_two = direction_two.Normalize();
	float dot_p = direction_one.Dot(direction_two);
	float angle = HPS::ACos(dot_p);
	if (angle > 180)
		angle = 360 - angle;
	if (inverted_measurement)
		angle = 360 - angle;

	std::stringstream angle_text;
	angle_text.precision(measurement_precision);
	angle_text << std::fixed << angle << "\xC2\xB0";
	text_string = HPS::UTF8(angle_text.str().c_str(), "utf8");
	text.Delete();
	text = current_measurement.InsertText(text_position, text_string);

	AdjustLineToCursor(text_position);

	if (inverted_measurement)
		CommonMeasurementOperator::Tag(measurement_arc, "inverted", CommonMeasurementOperator::Tags::Inverted);
	else
		measurement_arc.UnsetUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Inverted));

	InputUp(in_window);
	GetAttachedView().Update();
}

void Exchange::MeasurementOperator::AdjustLineToCursor(Point const & cursor_position)
{
	PointArray leader_line_one_points;
	leader_line_one.ShowPoints(leader_line_one_points);
	PointArray leader_line_two_points;
	leader_line_two.ShowPoints(leader_line_two_points);

	//calculate the measured angle
	Vector direction_one(circle_center - leader_line_one_points[0]);
	direction_one = direction_one.Normalize();
	Vector direction_two(circle_center - second_click_position);
	direction_two = direction_two.Normalize();
	float dot_p = direction_one.Dot(direction_two);
	float measurement_angle = HPS::ACos(dot_p);
	if (measurement_angle > 180)
		measurement_angle = 360 - measurement_angle;

	//calculate the angle on screen
	Vector direction_three(circle_center - cursor_position);
	direction_three = direction_three.Normalize();
	dot_p = direction_one.Dot(direction_three);
	float screen_angle = HPS::ACos(dot_p);
	if (screen_angle > 180)
		screen_angle = 360 - screen_angle;

	line_to_cursor.Delete();
	dot_p = direction_two.Dot(direction_three);
	float other_screen_angle = HPS::ACos(dot_p);
	if (other_screen_angle > 180)
		other_screen_angle = 360 - other_screen_angle;

	auto draw_line_to_cursor = [&]()
	{
		line_to_cursor.Delete();
		Point line_to_cursor_points[2];
		if (Vector(cursor_position - leader_line_two_points[1]).Length() < Vector(cursor_position - leader_line_one_points[1]).Length())
			line_to_cursor_points[0] = leader_line_two_points[1];
		else
			line_to_cursor_points[0] = leader_line_one_points[1];
		line_to_cursor_points[1] = cursor_position;
		line_to_cursor = current_measurement.InsertLine(2, line_to_cursor_points);
	};

	if (!manipulate_measurement)
	{
		if (screen_angle < measurement_angle && other_screen_angle < measurement_angle)
		{
			//do nothing
		}
		else
			draw_line_to_cursor();
	}
	else
	{
		if (screen_angle < measurement_angle && other_screen_angle < measurement_angle)
			draw_line_to_cursor();
	}
}

bool Exchange::MeasurementOperator::OnMouseMove(MouseState const & in_state)
{
	MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;
	WindowKey window = in_state.GetEventSource();

	if (highlight_on_mouse_over && !manipulate_measurement)
	{
		if (Highlight(measurement_to_edit, in_state.GetLocation(), window, in_state.GetEventPath()))
				highlight_update_notifier = GetAttachedView().UpdateWithNotifier();
	}

	if (measurement_to_edit == MeasurementType::PointToPoint ||
		measurement_to_edit == MeasurementType::FaceAngle)
	{
		if (anchors_in_place)
			return InputMove(in_state.GetLocation(), in_state.GetEventPath());
	}
	else if (measurement_to_edit == MeasurementType::EdgeAndRadius ||
		measurement_to_edit == MeasurementType::FeatureToFeature)
	{
		if (!operator_active)
			return false;

		return InputMove(in_state.GetLocation(), in_state.GetEventPath());
	}

	return false;
}

bool Exchange::MeasurementOperator::OnTouchMove(HPS::TouchState const & in_state)
{
    TouchArray touches = in_state.GetTouches();
    if (touches.size() != 1 || touches[0].ID != tracked_touch_id)
        return false;
    
    MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;
    WindowKey window = in_state.GetEventSource();
    
    Touch the_touch = touches[0];
    if (measurement_to_edit == MeasurementType::PointToPoint ||
        measurement_to_edit == MeasurementType::FaceAngle)
    {
        if (anchors_in_place)
            return InputMove(the_touch.Location, in_state.GetEventPath());
    }
    else if (measurement_to_edit == MeasurementType::EdgeAndRadius ||
             measurement_to_edit == MeasurementType::FeatureToFeature)
    {
        if (!operator_active)
            return false;
        
        return InputMove(the_touch.Location, in_state.GetEventPath());
    }
    
    return false;
}

bool Exchange::MeasurementOperator::InputMove(WindowPoint const & in_location, KeyPath const & in_path)
{
	MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;

	if (measurement_to_edit == MeasurementType::PointToPoint ||
		(measurement_to_edit == MeasurementType::EdgeAndRadius && edge_type == EdgeType::Line) ||
		measurement_to_edit == MeasurementType::FeatureToFeature)
	{
		Vector zero = Vector::Zero();
		Exchange::CommonMeasurementOperator::PositionLinearMeasurementGeometry(in_location, in_path, leader_line_one, leader_line_two, distance_line,
			line_to_cursor, first_click_position, second_click_position, text, text_string, distance_point_one, distance_point_two, current_measurement,
			measurement_to_edit == MeasurementType::FeatureToFeature ? measurement_plane : HPS::Plane::Zero(), 
			use_explicit_direction ? explicit_direction : zero);

		GetAttachedView().Update();
		return false;
	}
	else if (measurement_to_edit == MeasurementType::EdgeAndRadius && edge_type == EdgeType::Circle)
	{
		//only two things change: the measurement line, and the line to cursor.
		//get the circle normal
		PointArray measurement_line_points;
		distance_line.ShowPoints(measurement_line_points);
		measurement_direction = Vector(measurement_line_points[1] - measurement_line_points[0]);
		measurement_direction = measurement_direction.Normalize();

		PointArray edge_points;
		edge_line.ShowPoints(edge_points);
		Vector first(edge_points[0] - circle_center);
		first = first.Normalize();
		Vector second(edge_points[(size_t)(edge_points.size() / 2)] - circle_center);
		second = second.Normalize();

		float epsilon = 1e-5;
		float dot = fabs(first.Dot(measurement_direction));
		Vector circle_normal;
		if (dot - 1 > epsilon)
			circle_normal = first.Cross(measurement_direction);
		else
			circle_normal = second.Cross(measurement_direction);

		circle_normal = circle_normal.Normalize();
		Plane circle_plane(circle_center, circle_normal);
		Point intersection = GetPlaneIntersection(circle_plane, in_path, in_location);

		Vector center_to_intersection(intersection - circle_center);
		center_to_intersection = center_to_intersection.Normalize();
		Point point_on_line = circle_center + center_to_intersection * radius;
		measurement_line_points.clear();
		measurement_line_points.push_back(circle_center);
		measurement_line_points.push_back(point_on_line);
		distance_line.EditPointsByReplacement(0, measurement_line_points);

		Point line_to_cursor_points[] = { point_on_line, intersection };
		line_to_cursor.EditPointsByReplacement(0, 2, line_to_cursor_points);

		SegmentKey patterned_line_segment = current_measurement.Subsegment("patterned_line");
		Vector center_to_cursor_durection(intersection - circle_center);
		double center_to_cursor_length = center_to_cursor_durection.Length();
		if (center_to_cursor_length < radius)
			patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_out");
		else
			patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_in");

		text.Delete();
		text = current_measurement.InsertText(intersection, text_string);

		GetAttachedView().Update();
		return true;
	}
	else if (measurement_to_edit == MeasurementType::EdgeAndRadius && edge_type == EdgeType::Generic)
	{
		Plane plane(camera_direction, first_click_position);
		Point intersection = GetPlaneIntersection(plane, in_path, in_location);

		line_to_cursor.EditPointsByReplacement(1, PointArray(1, intersection));
		text.Delete();
		text = current_measurement.InsertText(intersection, text_string);

		GetAttachedView().Update();
		return true;
	}
	else if (measurement_to_edit == MeasurementType::FaceAngle)
	{
		//find where the position of the cursor fits on the measurement plane
		Point arc_start, arc_end, arc_middle;
		measurement_arc.ShowStart(arc_start);
		measurement_arc.ShowEnd(arc_end);
		measurement_arc.ShowMiddle(arc_middle);
		Vector one(arc_middle - arc_start);
		Vector two(arc_end - arc_middle);
		Vector normal = one.Normalize().Cross(two.Normalize());
		normal = normal.Normalize();

		if (HPS::Float::Equals(fabs(normal.Dot(camera_direction)), 0))
		{
			//the camera direction and the measurement plane are orthogonal,
			//making it impossible to move the measurement meaningfully.
			return true;
		}

		Plane circle_plane(normal, arc_start);
		Point intersection = GetPlaneIntersection(circle_plane, in_path, in_location);

		Vector cursor_to_center(intersection - circle_center);
		float face_angle_radius = (float)cursor_to_center.Length();

		//leader_line_one is modified in length, while maintaining its direction
		PointArray leader_line_one_points;
		leader_line_one.ShowPoints(leader_line_one_points);

		if (!inverted_measurement)
		{
			if (leader_line_one_direction.Dot(mid_point_direction) > 0)
				leader_line_one_points[1] = circle_center + leader_line_one_direction * face_angle_radius;
			else
				leader_line_one_points[1] = circle_center - leader_line_one_direction * face_angle_radius;
		}
		else
		{
			if (leader_line_one_direction.Dot(mid_point_direction) < 0)
				leader_line_one_points[1] = circle_center + leader_line_one_direction * face_angle_radius;
			else
				leader_line_one_points[1] = circle_center - leader_line_one_direction * face_angle_radius;
		}
		leader_line_one.EditPointsByReplacement(0, leader_line_one_points);

		//the same goes for leader_line_two
		PointArray leader_line_two_points;
		leader_line_two.ShowPoints(leader_line_two_points);
		leader_line_two_points[1] = circle_center + leader_line_two_direction * face_angle_radius;
		leader_line_two.EditPointsByReplacement(0, leader_line_two_points);

		//the line_to_cursor will only be visible if the cursor falls outside of the measurement arc
		AdjustLineToCursor(intersection);

		//recompute the measurement arc
		measurement_arc.Delete();
		CircularArcKit circular_arc_kit;
		circular_arc_kit.SetStart(leader_line_one_points[1]).SetEnd(leader_line_two_points[1]);

		Vector center_to_point_two = (second_click_position - circle_center);
		center_to_point_two = center_to_point_two.Normalize();

		float dot_p = center_to_point_two.Dot(leader_line_one_direction);
		float drawn_angle = HPS::ACos(dot_p);
		if (dot_p > 0)
			drawn_angle = 180 - drawn_angle;

		dot_p = leader_line_two_direction.Dot(leader_line_one_direction);
		float measured_angle = HPS::ACos(dot_p);
		if (dot_p > 0)
			measured_angle = 180 - measured_angle;

		Point point_on_circle = circle_center + mid_point_direction * face_angle_radius;
		circular_arc_kit.SetMiddle(point_on_circle);
		measurement_arc = current_measurement.InsertCircularArc(circular_arc_kit);

		//reposition the text
		text.Delete();
		text = current_measurement.InsertText(intersection, text_string);

		GetAttachedView().Update();
		return true;
	}

	return false;
}

bool Exchange::MeasurementOperator::OnMouseUp(MouseState const & in_state)
{
	bool mouse_triggered = false;
	if (!IsMouseTriggered(in_state))
	{
		mouse_triggered = true;
		disable_highlighting = false;
	}

	MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;
	WindowKey window = in_state.GetEventSource();

	if (measurement_to_edit == MeasurementType::PointToPoint ||
		measurement_to_edit == MeasurementType::FaceAngle)
	{
		if (mouse_triggered && anchors_in_place)
			return InputUp(window);

		if (mouse_triggered && anchors == 2)
			anchors_in_place = true;
	}
	else if (measurement_to_edit == MeasurementType::EdgeAndRadius)
	{
		if (mouse_triggered && operator_active)
			return InputUp(window);
	}
	else if (measurement_to_edit == MeasurementType::FeatureToFeature)
	{
		if (mouse_triggered && operator_active)
			return InputUp(window);
	}

	return false;
}

bool Exchange::MeasurementOperator::OnTouchUp(HPS::TouchState const & in_state)
{
    disable_highlighting = false;
    
    MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;
    WindowKey window = in_state.GetEventSource();
    
    if (measurement_to_edit == MeasurementType::PointToPoint ||
        measurement_to_edit == MeasurementType::FaceAngle)
    {
        if (anchors_in_place)
            return InputUp(window);
        
        if (anchors == 2)
            anchors_in_place = true;
    }
    else if (measurement_to_edit == MeasurementType::EdgeAndRadius)
    {
		if (operator_active)
		{
			operator_active = false;
			return InputUp(window);
		}
    }
    else if (measurement_to_edit == MeasurementType::FeatureToFeature)
    {
		if (operator_active)
		{
			if (anchors == 2)
			{
				//remove the highlight from the faces
				window.GetHighlightControl().Unhighlight(highlight_options);
				GetAttachedView().Update();
			}

			operator_active = false;
			anchors = 0;
			return InputUp(window);
		}
    }
    
    return false;
}

bool Exchange::MeasurementOperator::InputUp(WindowKey & in_window)
{
	MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;

	if ((measurement_to_edit == MeasurementType::EdgeAndRadius || measurement_to_edit == MeasurementType::FeatureToFeature) && !end_measurement)
		return true;

	if (measurement_to_edit == MeasurementType::FeatureToFeature ||
		measurement_to_edit == MeasurementType::FaceAngle)
	{
		//remove the highlight from the faces
		in_window.GetHighlightControl().Unhighlight(highlight_options);
		GetAttachedView().Update();
	}

	//tag the measurements with user data so that they can be restored when manipulating the measurement
	TagMeasurement();

	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::MeasurementInsertedEvent event(current_measurement, GetAttachedView());
		HPS::Database::GetEventDispatcher().InjectEvent(event);
	}

	if (manipulate_measurement)
	{
		in_window.GetHighlightControl().Unhighlight(current_measurement, edit_measurement_highlight_options);
		GetAttachedView().Update();
	}

	current_measurement = SegmentKey();

	if (measurement_to_edit == MeasurementType::PointToPoint ||
		measurement_to_edit == MeasurementType::FaceAngle)
	{
		anchors_in_place = false;
		anchors = 0;
	}
	else if (measurement_to_edit == MeasurementType::EdgeAndRadius)
	{
		operator_active = false;
		edge_type = EdgeType::Generic;
		end_measurement = false;
	}
	else if (measurement_to_edit == MeasurementType::FeatureToFeature)
	{
		operator_active = false;
		anchors = 0;
		surface_one = Surface();
		surface_two = Surface();
		end_measurement = false;
	}

	manipulate_measurement = false;
	use_explicit_direction = false;
	explicit_direction = Vector();
	highlighted_path = KeyPath();

	return true;
}

void Exchange::MeasurementOperator::RestoreMeasurement(SegmentKey const & measurement_segment_key)
{
	ByteArray data;
	measurement_segment_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::MeasurementType), data);

	if (strcmp((const char *)data.data(), "point_to_point") == 0)
	{
		RestorePointToPointMeasurement(measurement_segment_key);
		temporary_measurement_type = MeasurementType::PointToPoint;
	}
	else if (strcmp((const char *)data.data(), "edge_line") == 0)
	{
		RestoreEdgeMeasurement(measurement_segment_key);
		temporary_measurement_type = MeasurementType::EdgeAndRadius;
	}
	else if (strcmp((const char *)data.data(), "edge_circle") == 0)
	{
		RestoreRadiusMeasurement(measurement_segment_key);
		temporary_measurement_type = MeasurementType::EdgeAndRadius;
	}
	else if (strcmp((const char *)data.data(), "edge_generic") == 0)
	{
		RestoreGenericEdgeMeasurement(measurement_segment_key);
		temporary_measurement_type = MeasurementType::EdgeAndRadius;
	}
	else if (strcmp((const char *)data.data(), "face_distance") == 0)
	{
		RestoreFeatureToFeatureMeasurement(measurement_segment_key);
		temporary_measurement_type = MeasurementType::FeatureToFeature;
	}
	else if (strcmp((const char *)data.data(), "face_angle") == 0)
	{
		RestoreAngleMeasurement(measurement_segment_key);
		temporary_measurement_type = MeasurementType::FaceAngle;
	}
	else
		return;

	measurement_segment_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorX), data);
	camera_direction.x = (float)atof((const char *)data.data());
	measurement_segment_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorY), data);
	camera_direction.y = (float)atof((const char *)data.data());
	measurement_segment_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorZ), data);
	camera_direction.z = (float)atof((const char *)data.data());

	manipulate_measurement = true;

	if (temporary_measurement_type == MeasurementType::EdgeAndRadius ||
		temporary_measurement_type == MeasurementType::FeatureToFeature)
		operator_active = true;
}

void Exchange::MeasurementOperator::RestorePointToPointMeasurement(SegmentKey const & measurement_segment_key)
{
	SearchOptionsKit search_options;
	SearchTypeArray search_types;
	search_types.push_back(Search::Type::Line);
	search_types.push_back(Search::Type::Marker);
	search_types.push_back(Search::Type::Text);
	search_options.SetCriteria(search_types).SetSearchSpace(Search::Space::Subsegments);
	SearchResults search_results;
	measurement_segment_key.Find(search_options, search_results);

	ByteArray data;
	MarkerKey distance_marker_one, distance_marker_two;
	auto it = search_results.GetIterator();
	while (it.IsValid())
	{
		Key item = it.GetItem();
		HPS::Type item_type = item.Type();
		if (item_type == HPS::Type::MarkerKey)
		{
			MarkerKey marker_key(item);
			marker_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 marker_data((const char *)data.data(), "utf8");

			if (marker_data == "distance_marker_one")
				distance_marker_one = marker_key;
			else if (marker_data == "distance_marker_two")
				distance_marker_two = marker_key;
			else if (marker_data == "anchor_one")
				anchor_one = marker_key;
			else if (marker_data == "anchor_two")
				anchor_two = marker_key;
		}
		else if (item_type == HPS::Type::LineKey)
		{
			LineKey line_key(item);
			line_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 line_data((const char *)data.data(), "utf8");

			if (line_data == "distance_line")
				distance_line = line_key;
			else if (line_data == "leader_line_one")
				leader_line_one = line_key;
			else if (line_data == "leader_line_two")
				leader_line_two = line_key;
			else if (line_data == "line_to_cursor")
				line_to_cursor = line_key;
		}
		else if (item_type == HPS::Type::TextKey)
		{
			TextKey text_key(item);
			text_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 text_data((const char *)data.data(), "utf8");

			if (text_data == "text")
				text = text_key;
		}
		it.Next();
	}

	anchor_one.ShowPoint(first_click_position);
	anchor_two.ShowPoint(second_click_position);
	distance_marker_one.ShowPoint(distance_point_one);
	distance_marker_two.ShowPoint(distance_point_two);
	text.ShowText(text_string);

	anchors_in_place = false;
	anchors = 2;
	measurement_direction = Vector(second_click_position - first_click_position);
	measurement_direction = measurement_direction.Normalize();
	current_measurement = measurement_segment_key;
}

void Exchange::MeasurementOperator::RestoreEdgeMeasurement(SegmentKey const & measurement_segment_key)
{
	SearchOptionsKit search_options;
	SearchTypeArray search_types;
	search_types.push_back(Search::Type::Line);
	search_types.push_back(Search::Type::Marker);
	search_types.push_back(Search::Type::Text);
	search_options.SetCriteria(search_types).SetSearchSpace(Search::Space::Subsegments);
	SearchResults search_results;
	measurement_segment_key.Find(search_options, search_results);

	ByteArray data;
	MarkerKey distance_marker_one, distance_marker_two;
	auto it = search_results.GetIterator();
	while (it.IsValid())
	{
		Key item = it.GetItem();
		HPS::Type item_type = item.Type();
		if (item_type == HPS::Type::MarkerKey)
		{
			MarkerKey marker_key(item);
			marker_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 marker_data((const char *)data.data(), "utf8");

			if (marker_data == "distance_marker_one")
				distance_marker_one = marker_key;
			else if (marker_data == "distance_marker_two")
				distance_marker_two = marker_key;
		}
		else if (item_type == HPS::Type::LineKey)
		{
			LineKey line_key(item);
			line_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 line_data((const char *)data.data(), "utf8");

			if (line_data == "distance_line")
				distance_line = line_key;
			else if (line_data == "leader_line_one")
				leader_line_one = line_key;
			else if (line_data == "leader_line_two")
				leader_line_two = line_key;
			else if (line_data == "line_to_cursor")
				line_to_cursor = line_key;
			else if (line_data == "edge_line")
			{
				edge_line = line_key;
				if (edge_line.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorX), data))
				{
					use_explicit_direction = true;
					explicit_direction.x = (float)atof((const char *)data.data());
					edge_line.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorY), data);
					explicit_direction.y = (float)atof((const char *)data.data());
					edge_line.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorZ), data);
					explicit_direction.z = (float)atof((const char *)data.data());
				}
			}
		}
		else if (item_type == HPS::Type::TextKey)
		{
			TextKey text_key(item);
			text_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 text_data((const char *)data.data(), "utf8");

			if (text_data == "text")
				text = text_key;
		}
		it.Next();
	}

	PointArray edge_points;
	edge_line.ShowPoints(edge_points);
	first_click_position = edge_points[0];
	second_click_position = edge_points.back();
	distance_marker_one.ShowPoint(distance_point_one);
	distance_marker_two.ShowPoint(distance_point_two);
	text.ShowText(text_string);

	measurement_direction = Vector(second_click_position - first_click_position);
	measurement_direction = measurement_direction.Normalize();
	current_measurement = measurement_segment_key;

	edge_type = Exchange::MeasurementOperator::EdgeType::Line;
}

void Exchange::MeasurementOperator::RestoreGenericEdgeMeasurement(SegmentKey const & measurement_segment_key)
{
	SearchOptionsKit search_options;
	SearchTypeArray search_types;
	search_types.push_back(Search::Type::Line);
	search_types.push_back(Search::Type::Text);
	search_options.SetCriteria(search_types).SetSearchSpace(Search::Space::Subsegments);
	SearchResults search_results;
	measurement_segment_key.Find(search_options, search_results);

	ByteArray data;
	auto it = search_results.GetIterator();
	while (it.IsValid())
	{
		Key item = it.GetItem();
		HPS::Type item_type = item.Type();
		if (item_type == HPS::Type::LineKey)
		{
			LineKey line_key(item);
			if (line_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data))
			{
				UTF8 line_data((const char *)data.data(), "utf8");

				if (line_data == "line_to_cursor")
					line_to_cursor = line_key;
			}
		}
		else if (item_type == HPS::Type::TextKey)
		{
			TextKey text_key(item);
			text_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 text_data((const char *)data.data(), "utf8");

			if (text_data == "text")
				text = text_key;
		}
		it.Next();
	}

	text.ShowText(text_string);
	current_measurement = measurement_segment_key;

	edge_type = Exchange::MeasurementOperator::EdgeType::Generic;
}

void Exchange::MeasurementOperator::RestoreRadiusMeasurement(SegmentKey const & measurement_segment_key)
{
	SearchOptionsKit search_options;
	SearchTypeArray search_types;
	search_types.push_back(Search::Type::Line);
	search_types.push_back(Search::Type::Marker);
	search_types.push_back(Search::Type::Text);
	search_options.SetCriteria(search_types).SetSearchSpace(Search::Space::Subsegments);
	SearchResults search_results;
	measurement_segment_key.Find(search_options, search_results);

	ByteArray data;
	MarkerKey distance_marker_one, distance_marker_two;
	auto it = search_results.GetIterator();
	while (it.IsValid())
	{
		Key item = it.GetItem();
		HPS::Type item_type = item.Type();
		if (item_type == HPS::Type::MarkerKey)
		{
			MarkerKey marker_key(item);
			marker_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 marker_data((const char *)data.data(), "utf8");

			if (marker_data == "center_marker")
			{
				center_marker = marker_key;
				marker_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Radius), data);
				radius = (float)atof((const char *)data.data());
				center_marker.ShowPoint(circle_center);
			}
		}
		else if (item_type == HPS::Type::LineKey)
		{
			LineKey line_key(item);
			line_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 line_data((const char *)data.data(), "utf8");

			if (line_data == "distance_line")
				distance_line = line_key;
			else if (line_data == "line_to_cursor")
				line_to_cursor = line_key;
			else if (line_data == "edge_line")
				edge_line = line_key;
		}
		else if (item_type == HPS::Type::TextKey)
		{
			TextKey text_key(item);
			text_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 text_data((const char *)data.data(), "utf8");

			if (text_data == "text")
				text = text_key;
		}
		it.Next();
	}

	text.ShowText(text_string);

	current_measurement = measurement_segment_key;
	edge_type = Exchange::MeasurementOperator::EdgeType::Circle;
}

void Exchange::MeasurementOperator::RestoreFeatureToFeatureMeasurement(SegmentKey const & measurement_segment_key)
{
	SearchOptionsKit search_options;
	SearchTypeArray search_types;
	search_types.push_back(Search::Type::Line);
	search_types.push_back(Search::Type::Marker);
	search_types.push_back(Search::Type::Text);
	search_options.SetCriteria(search_types).SetSearchSpace(Search::Space::Subsegments);
	SearchResults search_results;
	measurement_segment_key.Find(search_options, search_results);

	ByteArray data;
	MarkerKey distance_marker_one, distance_marker_two;
	auto it = search_results.GetIterator();
	while (it.IsValid())
	{
		Key item = it.GetItem();
		HPS::Type item_type = item.Type();
		if (item_type == HPS::Type::MarkerKey)
		{
			MarkerKey marker_key(item);
			marker_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 marker_data((const char *)data.data(), "utf8");

			if (marker_data == "distance_marker_one")
				distance_marker_one = marker_key;
			else if (marker_data == "distance_marker_two")
				distance_marker_two = marker_key;
			else if (marker_data == "anchor_one")
				anchor_one = marker_key;
			else if (marker_data == "anchor_two")
				anchor_two = marker_key;
		}
		else if (item_type == HPS::Type::LineKey)
		{
			LineKey line_key(item);
			if (line_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data))
			{
				UTF8 line_data((const char *)data.data(), "utf8");

				if (line_data == "distance_line")
				{
					distance_line = line_key;

					if (distance_line.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorX), data))
					{
						use_explicit_direction = true;
						explicit_direction.x = (float)atof((const char *)data.data());
						distance_line.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorY), data);
						explicit_direction.y = (float)atof((const char *)data.data());
						distance_line.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorZ), data);
						explicit_direction.z = (float)atof((const char *)data.data());
					}
				}
				else if (line_data == "leader_line_one")
					leader_line_one = line_key;
				else if (line_data == "leader_line_two")
					leader_line_two = line_key;
				else if (line_data == "line_to_cursor")
					line_to_cursor = line_key;
			}
		}
		else if (item_type == HPS::Type::TextKey)
		{
			TextKey text_key(item);
			text_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 text_data((const char *)data.data(), "utf8");

			if (text_data == "text")
				text = text_key;
		}
		it.Next();
	}

	anchor_one.ShowPoint(first_click_position);
	anchor_two.ShowPoint(second_click_position);
	distance_marker_one.ShowPoint(distance_point_one);
	distance_marker_two.ShowPoint(distance_point_two);
	text.ShowText(text_string);

	operator_active = true;
	measurement_direction = Vector(second_click_position - first_click_position);
	measurement_direction = measurement_direction.Normalize();
	current_measurement = measurement_segment_key;
}

void Exchange::MeasurementOperator::RestoreAngleMeasurement(SegmentKey const & measurement_segment_key)
{
	SearchOptionsKit search_options;
	SearchTypeArray search_types;
	search_types.push_back(Search::Type::Line);
	search_types.push_back(Search::Type::Text);
	search_types.push_back(Search::Type::CircularArc);
	search_types.push_back(Search::Type::Marker);
	search_options.SetCriteria(search_types).SetSearchSpace(Search::Space::Subsegments);
	SearchResults search_results;
	measurement_segment_key.Find(search_options, search_results);

	ByteArray data;
	MarkerKey center_marker_key;
	LineKey distance_line_one, distance_line_two;
	auto it = search_results.GetIterator();
	while (it.IsValid())
	{
		Key item = it.GetItem();
		HPS::Type item_type = item.Type();
		if (item_type == HPS::Type::LineKey)
		{
			LineKey line_key(item);
			line_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 line_data((const char *)data.data(), "utf8");

			if (line_data == "line_to_leader_line")
				line_to_leader_line = line_key;
			else if (line_data == "leader_line_one")
				leader_line_one = line_key;
			else if (line_data == "leader_line_two")
				leader_line_two = line_key;
			else if (line_data == "direction_one")
				distance_line_one = line_key;
			else if (line_data == "direction_two")
				distance_line_two = line_key;
			else if (line_data == "line_to_cursor")
				line_to_cursor = line_key;
		}
		else if (item_type == HPS::Type::TextKey)
		{
			TextKey text_key(item);
			text_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 text_data((const char *)data.data(), "utf8");

			if (text_data == "text")
				text = text_key;
		}
		else if (item_type == HPS::Type::MarkerKey)
		{
			MarkerKey marker_key(item);
			marker_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 marker_data((const char *)data.data(), "utf8");

			if (marker_data == "center")
				center_marker_key = marker_key;
		}
		else if (item_type == HPS::Type::CircularArcKey)
		{
			CircularArcKey circular_arc_key(item);
			circular_arc_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Name), data);
			UTF8 circular_arc_data((const char *)data.data(), "utf8");

			if (circular_arc_data == "measurement_arc")
			{
				measurement_arc = circular_arc_key;
				if (circular_arc_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::Inverted), data) &&
					strcmp((const char *)data.data(), "inverted") == 0)
					inverted_measurement = true;
				circular_arc_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorX), data);
				mid_point_direction.x = (float)atof((const char *)data.data());
				circular_arc_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorY), data);
				mid_point_direction.y = (float)atof((const char *)data.data());
				circular_arc_key.ShowUserData(static_cast<size_t>(CommonMeasurementOperator::Tags::VectorZ), data);
				mid_point_direction.z = (float)atof((const char *)data.data());
			}
		}
		it.Next();
	}

	center_marker_key.ShowPoint(circle_center);
	PointArray points;
	distance_line_one.ShowPoints(points);
	leader_line_one_direction = Vector(points[1]);
	distance_line_two.ShowPoints(points);
	leader_line_two_direction = Vector(points[1]);
	text.ShowText(text_string);
	anchors = 2;
	anchors_in_place = false;
	current_measurement = measurement_segment_key;
}

void Exchange::MeasurementOperator::TagMeasurement()
{
	MeasurementType what_to_tag;
	if (manipulate_measurement)
		what_to_tag = temporary_measurement_type;
	else
		what_to_tag = measurement_type;

	switch (what_to_tag)
	{
	case MeasurementType::PointToPoint:
		TagPointToPointMeasurement();
		break;
	case MeasurementType::EdgeAndRadius:
	{
		if (edge_type == EdgeType::Line)
			TagEdgeMeasurement();
		else if (edge_type == EdgeType::Circle)
			TagRadiusMeasurement();
		else if (edge_type == EdgeType::Generic)
			TagGenericEdgeMeasurement();
	} break;
	case MeasurementType::FeatureToFeature:
		TagFeatureToFeatureMeasurement();
		break;
	case MeasurementType::FaceAngle:
		TagAngleMeasurement();
		break;
	}
}

void Exchange::MeasurementOperator::TagPointToPointMeasurement()
{
	SegmentKey invisible_geometry = current_measurement.Subsegment("invisible");
	invisible_geometry.Flush();
	invisible_geometry.GetVisibilityControl().SetEverything(false);
	invisible_geometry.GetAttributeLockControl().SetLock(AttributeLock::Type::Visibility, true);
	MarkerKey distance_marker_one = invisible_geometry.InsertMarker(distance_point_one);
	MarkerKey distance_marker_two = invisible_geometry.InsertMarker(distance_point_two);

	CommonMeasurementOperator::Tag(distance_marker_one, "distance_marker_one", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(distance_marker_two, "distance_marker_two", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(text, "text", CommonMeasurementOperator::Tags::Name);

	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::Tag(current_measurement, "hps_measurement_segment", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(current_measurement, "point_to_point", CommonMeasurementOperator::Tags::MeasurementType);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
		CommonMeasurementOperator::Tag(distance_line, "distance_line", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_one, "leader_line_one", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_two, "leader_line_two", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(line_to_cursor, "line_to_cursor", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(anchor_one, "anchor_one", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(anchor_two, "anchor_two", CommonMeasurementOperator::Tags::Name);
	}
}

void Exchange::MeasurementOperator::TagEdgeMeasurement()
{
	SegmentKey invisible_geometry = current_measurement.Subsegment("invisible");
	invisible_geometry.Flush();
	invisible_geometry.GetVisibilityControl().SetEverything(false);
	invisible_geometry.GetAttributeLockControl().SetLock(AttributeLock::Type::Visibility, true);
	MarkerKey distance_marker_one = invisible_geometry.InsertMarker(distance_point_one);
	MarkerKey distance_marker_two = invisible_geometry.InsertMarker(distance_point_two);

	CommonMeasurementOperator::Tag(distance_marker_one, "distance_marker_one", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(distance_marker_two, "distance_marker_two", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(text, "text", CommonMeasurementOperator::Tags::Name);

	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::Tag(current_measurement, "hps_measurement_segment", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(current_measurement, "edge_line", CommonMeasurementOperator::Tags::MeasurementType);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);

		if (use_explicit_direction)
		{
			CommonMeasurementOperator::Tag(edge_line, std::to_string(explicit_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
			CommonMeasurementOperator::Tag(edge_line, std::to_string(explicit_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
			CommonMeasurementOperator::Tag(edge_line, std::to_string(explicit_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
		}

		CommonMeasurementOperator::Tag(distance_line, "distance_line", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_one, "leader_line_one", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_two, "leader_line_two", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(line_to_cursor, "line_to_cursor", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(edge_line, "edge_line", CommonMeasurementOperator::Tags::Name);
	}
}

void Exchange::MeasurementOperator::TagGenericEdgeMeasurement()
{
	CommonMeasurementOperator::Tag(text, "text", CommonMeasurementOperator::Tags::Name);

	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::Tag(current_measurement, "hps_measurement_segment", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(current_measurement, "edge_generic", CommonMeasurementOperator::Tags::MeasurementType);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
		CommonMeasurementOperator::Tag(line_to_cursor, "line_to_cursor", CommonMeasurementOperator::Tags::Name);
	}
}

void Exchange::MeasurementOperator::TagRadiusMeasurement()
{
	CommonMeasurementOperator::Tag(text, "text", CommonMeasurementOperator::Tags::Name);

	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::Tag(current_measurement, "hps_measurement_segment", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(current_measurement, "edge_circle", CommonMeasurementOperator::Tags::MeasurementType);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
		CommonMeasurementOperator::Tag(distance_line, "distance_line", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(line_to_cursor, "line_to_cursor", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(edge_line, "edge_line", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(center_marker, "center_marker", CommonMeasurementOperator::Tags::Name);

		std::stringstream radius_string;
		radius_string.precision(measurement_precision);
		radius_string << std::fixed << radius;
		CommonMeasurementOperator::Tag(center_marker, radius_string.str().data(), CommonMeasurementOperator::Tags::Radius);
	}
}

void Exchange::MeasurementOperator::TagFeatureToFeatureMeasurement()
{
	SegmentKey invisible_geometry = current_measurement.Subsegment("invisible");
	invisible_geometry.Flush();
	invisible_geometry.GetVisibilityControl().SetEverything(false);
	invisible_geometry.GetAttributeLockControl().SetLock(AttributeLock::Type::Visibility, true);
	MarkerKey distance_marker_one = invisible_geometry.InsertMarker(distance_point_one);
	MarkerKey distance_marker_two = invisible_geometry.InsertMarker(distance_point_two);

	CommonMeasurementOperator::Tag(distance_marker_one, "distance_marker_one", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(distance_marker_two, "distance_marker_two", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(text, "text", CommonMeasurementOperator::Tags::Name);

	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::Tag(current_measurement, "hps_measurement_segment", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(current_measurement, "face_distance", CommonMeasurementOperator::Tags::MeasurementType);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);

		if (use_explicit_direction)
		{
			CommonMeasurementOperator::Tag(distance_line, std::to_string(explicit_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
			CommonMeasurementOperator::Tag(distance_line, std::to_string(explicit_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
			CommonMeasurementOperator::Tag(distance_line, std::to_string(explicit_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
		}

		CommonMeasurementOperator::Tag(distance_line, "distance_line", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_one, "leader_line_one", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_two, "leader_line_two", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(line_to_cursor, "line_to_cursor", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(anchor_one, "anchor_one", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(anchor_two, "anchor_two", CommonMeasurementOperator::Tags::Name);
	}
}

void Exchange::MeasurementOperator::TagAngleMeasurement()
{
	SegmentKey invisible_geometry = current_measurement.Subsegment("invisible");
	invisible_geometry.Flush();
	invisible_geometry.GetVisibilityControl().SetEverything(false);
	invisible_geometry.GetAttributeLockControl().SetLock(AttributeLock::Type::Visibility, true);
	MarkerKey center = invisible_geometry.InsertMarker(circle_center);
	Point origin(0, 0, 0);
	LineKey direction_one = invisible_geometry.InsertLine(origin, Point(leader_line_one_direction));
	LineKey direction_two = invisible_geometry.InsertLine(origin, Point(leader_line_two_direction));

	CommonMeasurementOperator::Tag(center, "center", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(direction_one, "direction_one", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(direction_two, "direction_two", CommonMeasurementOperator::Tags::Name);
	CommonMeasurementOperator::Tag(text, "text", CommonMeasurementOperator::Tags::Name);

	CommonMeasurementOperator::Tag(measurement_arc, std::to_string(mid_point_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
	CommonMeasurementOperator::Tag(measurement_arc, std::to_string(mid_point_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
	CommonMeasurementOperator::Tag(measurement_arc, std::to_string(mid_point_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
	if (!manipulate_measurement)
	{
		CommonMeasurementOperator::Tag(current_measurement, "hps_measurement_segment", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(current_measurement, "face_angle", CommonMeasurementOperator::Tags::MeasurementType);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.x).data(), CommonMeasurementOperator::Tags::VectorX);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.y).data(), CommonMeasurementOperator::Tags::VectorY);
		CommonMeasurementOperator::Tag(current_measurement, std::to_string(camera_direction.z).data(), CommonMeasurementOperator::Tags::VectorZ);
		CommonMeasurementOperator::Tag(measurement_arc, "measurement_arc", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_one, "leader_line_one", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(leader_line_two, "leader_line_two", CommonMeasurementOperator::Tags::Name);
		CommonMeasurementOperator::Tag(line_to_leader_line, "line_to_leader_line", CommonMeasurementOperator::Tags::Name);
		if (!line_to_cursor.Empty())
			CommonMeasurementOperator::Tag(line_to_cursor, "line_to_cursor", CommonMeasurementOperator::Tags::Name);
	}
}

bool Exchange::MeasurementOperator::OnKeyDown(KeyboardState const & in_state)
{
	if (current_measurement.Empty())
		return false;

	KeyboardCodeArray keyboard_codes = in_state.GetActiveEvent().KeyboardCodes;
	if (std::find(keyboard_codes.begin(), keyboard_codes.end(), KeyboardCode::Escape) != keyboard_codes.end())
	{
		if (manipulate_measurement)
		{
			CommonMeasurementOperator::MeasurementDeletedEvent event(current_measurement.Name(), GetAttachedView());
			HPS::Database::GetEventDispatcher().InjectEvent(event);
		}

		ResetMeasurement();
		return true;
	}

	return false;
}

void HPS::Exchange::MeasurementOperator::ResetMeasurement()
{
	View view = GetAttachedView();
	auto view_type = view.Type();

	current_measurement.Delete();
	tracked_touch_id = -1;
    current_touch_id = -1;

	if (measurement_type == MeasurementType::PointToPoint ||
		measurement_type == MeasurementType::FaceAngle)
	{
		anchors = 0;
		anchors_in_place = false;
	}
	else if (measurement_type == MeasurementType::EdgeAndRadius)
	{
		operator_active = false;
		end_measurement = false;
	}
	else if (measurement_type == MeasurementType::FeatureToFeature)
	{
		operator_active = false;
		end_measurement = false;
		anchors = 0;
		surface_one = Surface();
		surface_two = Surface();
	}


	if (measurement_type == MeasurementType::FeatureToFeature ||
		measurement_type == MeasurementType::FaceAngle)
	{
		for (auto & canvas : canvases)
			canvas.GetWindowKey().GetHighlightControl().Unhighlight(highlight_options);
	}

	manipulate_measurement = false;
	
	if (view_type != HPS::Type::None)
		view.Update();
}

void Exchange::MeasurementOperator::GetEdgeLengthAndType(Exchange::Component const & edge_component)
{
	auto get_analytic_curve = [&](A3DCrvBase * base_curve, double tolerance, double context_scale, double ri_scale, double product_occurrence_scale)
	{
		A3DEEntityType recognized_types[] = { kA3DTypeCrvLine, kA3DTypeCrvCircle };
		A3DEAnalyticType analytic_type;
		A3DCrvBase * dummy = nullptr;
		edge_type = Exchange::MeasurementOperator::EdgeType::Generic;
		if (A3DSimplifyCurveWithAnalytics(base_curve, tolerance, 2, recognized_types, &dummy, &analytic_type) == A3D_SUCCESS)
		{
			if (analytic_type == kA3DAnalyticAlreadyLine || analytic_type == kA3DAnalyticLine)
				edge_type = Exchange::MeasurementOperator::EdgeType::Line;
			else if (analytic_type == kA3DAnalyticAlreadyCircle || analytic_type == kA3DAnalyticCircle)
			{
				A3DCrvCircle * circle = nullptr;
				if (analytic_type == kA3DAnalyticAlreadyCircle)
					circle = reinterpret_cast<A3DCrvCircle *>(base_curve);
				else
					circle = reinterpret_cast<A3DCrvCircle *>(dummy);
				A3DCrvCircleData circle_data;
				A3D_INITIALIZE_DATA(A3DCrvCircleData, circle_data);
				A3DCrvCircleGet(circle, &circle_data);
				A3DVector3dData origin = circle_data.m_sTrsf.m_sOrigin;

				radius = (float)(circle_data.m_dRadius * context_scale * ri_scale * product_occurrence_scale);

				float model_scale = GetModelScale(edge_component);
				circle_center = Point((float)origin.m_dX * model_scale, (float)origin.m_dY * model_scale, (float)origin.m_dZ * model_scale);
				A3DCrvCircleGet(nullptr, &circle_data);

				edge_type = Exchange::MeasurementOperator::EdgeType::Circle;
			}


			A3DEntityDelete(dummy);
		}
	};

	A3DTopoEdge * edge = edge_component.GetExchangeEntity();
	A3DTopoEdgeData edge_data;
	A3D_INITIALIZE_DATA(A3DTopoEdgeData, edge_data);
	if (A3DTopoEdgeGet(edge, &edge_data) == A3D_SUCCESS)
	{
		//get the product occurrence scale, the model scale and the context scale
		HPS::ComponentArray co_edges = edge_component.GetOwners();
		HPS::ComponentArray loops = co_edges[0].GetOwners();
		HPS::ComponentArray faces = loops[0].GetOwners();
		HPS::ComponentArray shells = faces[0].GetOwners();
		HPS::ComponentArray connexes = shells[0].GetOwners();
		HPS::ComponentArray bodies = connexes[0].GetOwners();
		HPS::Exchange::Component body = bodies[0];

		HPS::ComponentArray models = bodies[0].GetOwners();
		HPS::Exchange::Component model = models[0];

		HPS::ComponentArray part_definitions = models[0].GetOwners();
		HPS::ComponentArray product_occurrences = part_definitions[0].GetOwners();
		HPS::Exchange::Component product_occurrence = product_occurrences[0];

		std::vector<A3DAsmProductOccurrence *> product_occurrences_with_location_info;
		double product_occurrence_scale = 1;
		while (!product_occurrence.Empty() && product_occurrence.GetComponentType() == Component::ComponentType::ExchangeProductOccurrence)
		{
			A3DAsmProductOccurrence * a3d_product_occurrence = product_occurrence.GetExchangeEntity();
			A3DAsmProductOccurrenceData product_occurrence_data;
			A3D_INITIALIZE_DATA(A3DAsmProductOccurrenceData, product_occurrence_data);
			A3DAsmProductOccurrenceGet(a3d_product_occurrence, &product_occurrence_data);

			if (product_occurrence_data.m_pLocation != nullptr)
				product_occurrences_with_location_info.push_back(a3d_product_occurrence);

			product_occurrence = product_occurrence.GetOwners()[0];

			A3DAsmProductOccurrenceGet(nullptr, &product_occurrence_data);
		}

		for (auto const & one_product_occurrence : product_occurrences_with_location_info)
		{
			//we found a product occurrence which has a scale
			A3DAsmProductOccurrenceData product_occurrence_data;
			A3D_INITIALIZE_DATA(A3DAsmProductOccurrenceData, product_occurrence_data);
			A3DAsmProductOccurrenceGet(one_product_occurrence, &product_occurrence_data);

			if (product_occurrence_data.m_pLocation != nullptr)
			{
				A3DMiscCartesianTransformationData transformation_data;
				A3D_INITIALIZE_DATA(A3DMiscCartesianTransformationData, transformation_data);
				if (A3DMiscCartesianTransformationGet(product_occurrence_data.m_pLocation, &transformation_data) == A3D_SUCCESS)
				{
					product_occurrence_scale *= transformation_data.m_sScale.m_dX;
					A3DMiscCartesianTransformationGet(nullptr, &transformation_data);
				}
			}

			A3DAsmProductOccurrenceGet(nullptr, &product_occurrence_data);
		}

		double ri_scale = 1;
		if (model.HasComponentType(Component::ComponentType::ExchangeRepresentationItemMask))
		{
			A3DRiRepresentationItem * rep_item = model.GetExchangeEntity();
			A3DRiRepresentationItemData rep_item_data;
			A3D_INITIALIZE_DATA(A3DRiRepresentationItemData, rep_item_data);
			if (A3DRiRepresentationItemGet(rep_item, &rep_item_data) == A3D_SUCCESS)
			{
				if (rep_item_data.m_pCoordinateSystem != nullptr)
				{
					A3DRiCoordinateSystemData coordinate_data;
					A3D_INITIALIZE_DATA(A3DRiCoordinateSystemData, coordinate_data);
					if (A3DRiCoordinateSystemGet(rep_item_data.m_pCoordinateSystem, &coordinate_data) == A3D_SUCCESS)
					{
						if (coordinate_data.m_pTransformation != nullptr)
						{
							A3DMiscCartesianTransformationData transformation_data;
							A3D_INITIALIZE_DATA(A3DMiscCartesianTransformationData, transformation_data);
							if (A3DMiscCartesianTransformationGet(coordinate_data.m_pTransformation, &transformation_data) == A3D_SUCCESS)
							{
								ri_scale = transformation_data.m_sScale.m_dX;
								A3DMiscCartesianTransformationGet(nullptr, &transformation_data);
							}
							A3DRiCoordinateSystemGet(nullptr, &coordinate_data);
						}
					}
				}
				A3DRiRepresentationItemGet(nullptr, &rep_item_data);
			}
		}

		double context_scale = 1;
		A3DTopoBodyData topo_body_data;
		A3D_INITIALIZE_DATA(A3DTopoBodyData, topo_body_data);

		if (A3DTopoBodyGet(body.GetExchangeEntity(), &topo_body_data) == A3D_SUCCESS)
		{
			A3DTopoContextData context_data;
			A3D_INITIALIZE_DATA(A3DTopoContextData, context_data);

			if (A3DTopoContextGet(topo_body_data.m_pContext, &context_data) == A3D_SUCCESS &&
				context_data.m_bHaveScale)
				context_scale = context_data.m_dScale;

			A3DTopoContextGet(nullptr, &context_data);
		}
		A3DTopoBodyGet(nullptr, &topo_body_data);

		//if the edge has a curve associated with it, this is easy
		A3DCrvBase * base_curve = edge_data.m_p3dCurve;
		double length;
		if (base_curve != nullptr && A3DCurveLength(base_curve, edge_data.m_bHasTrimDomain ? &edge_data.m_sInterval : NULL, &length) == A3D_SUCCESS)
		{
			edge_length = length * context_scale * ri_scale * product_occurrence_scale;
			get_analytic_curve(base_curve, 0.00001, context_scale, ri_scale, product_occurrence_scale);
		}

		//otherwise we need to do some more work
		else if (base_curve == nullptr)
		{
			A3DTopoCoEdgeData co_edge_data;
			A3D_INITIALIZE_DATA(A3DTopoCoEdgeData, co_edge_data);
			A3DTopoFaceData face_data;
			A3D_INITIALIZE_DATA(A3DTopoFaceData, face_data);

			if (co_edges[0].GetComponentType() == HPS::Component::ComponentType::ExchangeTopoCoEdge)
			{
				HPS::Exchange::Component co_edge_component = co_edges[0];
				if (A3DTopoCoEdgeGet(co_edge_component.GetExchangeEntity(), &co_edge_data) == A3D_SUCCESS)
				{
					HPS::Exchange::Component face_component = faces[0];
					if (A3DTopoFaceGet(face_component.GetExchangeEntity(), &face_data) == A3D_SUCCESS)
					{
						A3DMiscCartesianTransformationData transformation_data;
						A3D_INITIALIZE_DATA(A3DMiscCartesianTransformationData, transformation_data);

						transformation_data.m_sOrigin.m_dX = 0;
						transformation_data.m_sOrigin.m_dY = 0;
						transformation_data.m_sOrigin.m_dZ = 0;
						transformation_data.m_sXVector.m_dX = 1;
						transformation_data.m_sXVector.m_dY = 0;
						transformation_data.m_sXVector.m_dZ = 0;
						transformation_data.m_sYVector.m_dX = 0;
						transformation_data.m_sYVector.m_dY = 1;
						transformation_data.m_sYVector.m_dZ = 0;
						transformation_data.m_sScale.m_dX = 1;
						transformation_data.m_sScale.m_dY = 1;
						transformation_data.m_sScale.m_dZ = 1;
						transformation_data.m_ucBehaviour = kA3DTransformationTranslate & kA3DTransformationRotate & kA3DTransformationScale;

						A3DIntervalData interval_data;
						A3D_INITIALIZE_DATA(A3DIntervalData, interval_data);
						A3DCrvGetInterval(co_edge_data.m_pUVCurve, &interval_data);

						A3DParameterizationData parameterization_data;
						A3D_INITIALIZE_DATA(A3DParameterizationData, parameterization_data);
						parameterization_data.m_sInterval = interval_data;
						parameterization_data.m_dCoeffA = 1.0;

						A3DCrvOnSurf * curve_on_surface;
						A3DCrvOnSurfData curve_on_surface_data;
						A3D_INITIALIZE_DATA(A3DCrvOnSurfData, curve_on_surface_data);
						curve_on_surface_data.m_pUVCurve = co_edge_data.m_pUVCurve;
						curve_on_surface_data.m_bIs2D = false;
						curve_on_surface_data.m_sTrsf = transformation_data;
						curve_on_surface_data.m_sParam = parameterization_data;
						curve_on_surface_data.m_pSurface = face_data.m_pSurface;
						A3DCrvOnSurfCreate(&curve_on_surface_data, &curve_on_surface);

						double curve_length;
						// TUNG: do we need the domain trim here? I don't know when we will get into this case
						if (A3DCurveLength(curve_on_surface, 0, &curve_length) == A3D_SUCCESS)
						{
							edge_length = curve_length * context_scale * ri_scale * product_occurrence_scale;
							get_analytic_curve(curve_on_surface, 0.00001, context_scale, ri_scale, product_occurrence_scale);
						}

						A3DCrvGetInterval(nullptr, &interval_data);
						A3DCrvOnSurfCreate(nullptr, &curve_on_surface);
						A3DTopoFaceGet(nullptr, &face_data);
					}
					A3DTopoCoEdgeGet(nullptr, &co_edge_data);
				}
			}
		}
	}

	A3DTopoEdgeGet(nullptr, &edge_data);
}

void Exchange::MeasurementOperator::GetSurfaceType(Exchange::Component const & face_component, Surface & surface)
{
	auto get_cone_info = [](A3DSurfBase * a3d_surface, Surface & surface, float scale)
	{
		A3DSurfCone * cone = reinterpret_cast<A3DSurfCone *>(a3d_surface);
		A3DSurfConeData cone_data;
		A3D_INITIALIZE_DATA(A3DSurfConeData, cone_data);

		if (A3DSurfConeGet(cone, &cone_data) == A3D_SUCCESS)
		{
			A3DMiscCartesianTransformationData transform = cone_data.m_sTrsf;
			Vector x((float)transform.m_sXVector.m_dX, (float)transform.m_sXVector.m_dY, (float)transform.m_sXVector.m_dZ);
			Vector y((float)transform.m_sYVector.m_dX, (float)transform.m_sYVector.m_dY, (float)transform.m_sYVector.m_dZ);
			Vector normal = y.Cross(x);

			surface.center = Point((float)(transform.m_sOrigin.m_dX * scale), (float)(transform.m_sOrigin.m_dY * scale), (float)(transform.m_sOrigin.m_dZ * scale));
			surface.normal = normal.Normalize();
			surface.surface_type = Surface::SurfaceType::ConeOrCylinder;

			A3DSurfConeGet(nullptr, &cone_data);
		}
	};

	auto get_cylinder_info = [](A3DSurfBase * a3d_surface, Surface & surface, float scale)
	{
		A3DSurfCylinder * cylinder = reinterpret_cast<A3DSurfCylinder *>(a3d_surface);
		A3DSurfCylinderData cylinder_data;
		A3D_INITIALIZE_DATA(A3DSurfCylinderData, cylinder_data);

		if (A3DSurfCylinderGet(cylinder, &cylinder_data) == A3D_SUCCESS)
		{
			A3DMiscCartesianTransformationData transform = cylinder_data.m_sTrsf;
			Vector x((float)transform.m_sXVector.m_dX, (float)transform.m_sXVector.m_dY, (float)transform.m_sXVector.m_dZ);
			Vector y((float)transform.m_sYVector.m_dX, (float)transform.m_sYVector.m_dY, (float)transform.m_sYVector.m_dZ);
			Vector normal = y.Cross(x);

			surface.center = Point((float)(transform.m_sOrigin.m_dX * scale), (float)(transform.m_sOrigin.m_dY * scale), (float)(transform.m_sOrigin.m_dZ * scale));
			surface.normal = normal.Normalize();
			surface.surface_type = Surface::SurfaceType::ConeOrCylinder;

			A3DSurfCylinderGet(nullptr, &cylinder_data);
		}
	};



	A3DTopoFaceData face_data;
	A3D_INITIALIZE_DATA(A3DTopoFaceData, face_data);
	if (A3DTopoFaceGet(face_component.GetExchangeEntity(), &face_data) == A3D_SUCCESS)
	{
		if (face_data.m_pSurface != nullptr)
		{
			float scale = GetModelScale(face_component);

			A3DEEntityType a3d_surface_type;
			if (A3DEntityGetType(face_data.m_pSurface, &a3d_surface_type) == A3D_SUCCESS)
			{
				if (a3d_surface_type == kA3DTypeSurfCone)
					get_cone_info(face_data.m_pSurface, surface, scale);
				else if (a3d_surface_type == kA3DTypeSurfCylinder)
					get_cylinder_info(face_data.m_pSurface, surface, scale);
				else if (a3d_surface_type == kA3DTypeSurfPlane)
					surface.surface_type = Surface::SurfaceType::Plane;
				else
				{
					A3DEEntityType recognized_surfaces[] = { kA3DTypeSurfCone, kA3DTypeSurfCylinder, kA3DTypeSurfPlane };
					A3DSurfBase * simplified_surface;
					A3DEAnalyticType simplified_type;
					if (A3DSimplifySurfaceWithAnalytics(face_data.m_pSurface, 0.0001, 3, recognized_surfaces, &simplified_surface, &simplified_type) == A3D_SUCCESS)
					{
						if (simplified_type == kA3DAnalyticCone)
							get_cone_info(face_data.m_pSurface, surface, scale);
						else if (simplified_type == kA3DAnalyticCylinder)
							get_cylinder_info(face_data.m_pSurface, surface, scale);
						else if (simplified_type == kA3DAnalyticPlane)
							surface.surface_type = Surface::SurfaceType::Plane;

						A3DEntityDelete(simplified_surface);
					}
				}
			}
		}

		A3DTopoFaceGet(nullptr, &face_data);
	}
}

void Exchange::MeasurementOperator::PlaneToCenterLineDistance()
{
	//measure the shortest distance between the center line of the conical/cylindrical face and the planar face
	Exchange::Component plane_component;
	ComponentPath component_path;

	LineKey center_line;
	if (surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
		surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::Plane)
	{
		plane_component = surface_two.path.Front();
		component_path = surface_two.path;
		center_line = surface_one.normal_points;
	}
	else if (surface_two.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::ConeOrCylinder &&
		surface_one.surface_type == Exchange::MeasurementOperator::Surface::SurfaceType::Plane)
	{
		center_line = surface_two.normal_points;
		plane_component = surface_one.path.Front();
		component_path = surface_one.path;
	}

	PointArray center_line_points;
	center_line.ShowPoints(center_line_points);

	float minimum_distance = (std::numeric_limits<float>::max)();
	Point center_line_point;
	Point edge_point;

	HPS::ComponentArray edges = plane_component.GetAllSubcomponents(Component::ComponentType::ExchangeTopoEdge);
	for (auto const & edge : edges)
	{
		LineKey edge_line_key = LineKey(edge.GetKeys()[0]);

		PointArray edge_line_points;
		edge_line_key.ShowPoints(edge_line_points);

		MatrixKit net_matrix;
		component_path.GetKeyPaths()[0].ShowNetModellingMatrix(net_matrix);
		edge_line_points = net_matrix.Transform(edge_line_points);

		size_t number_of_points = edge_line_points.size();
		for (size_t i = 0; i < (number_of_points - 1); ++i)
		{
			Point point_on_center_line;
			Point point_on_edge;
			LineKey dummy_normal;
			float distance = LineSegmentDistance(center_line_points[0], center_line_points[1], edge_line_points[i], edge_line_points[i + 1], current_normal, dummy_normal, point_on_edge, point_on_center_line);
			if (distance < minimum_distance)
			{
				minimum_distance = distance;
				center_line_point = point_on_center_line;
				edge_point = point_on_edge;
			}
		}
	}

	InsertFeatureToFeatureGeometry(edge_point, center_line_point, minimum_distance);
}

void Exchange::MeasurementOperator::PlaneToPlaneDistance()
{
	//measure the shortest distance between two planar faces
	ShellKey shell_one = (ShellKey)surface_one.path.Front().GetKeys()[0];
	ShellKey shell_two = (ShellKey)surface_two.path.Front().GetKeys()[0];

	PointArray shell_one_points, shell_two_points;
	shell_one.ShowPoints(shell_one_points);
	shell_two.ShowPoints(shell_two_points);

	MatrixKit net_matrix;
	surface_one.path.GetKeyPaths()[0].ShowNetModellingMatrix(net_matrix);
	shell_one_points = net_matrix.Transform(shell_one_points);

	surface_two.path.GetKeyPaths()[0].ShowNetModellingMatrix(net_matrix);
	shell_two_points = net_matrix.Transform(shell_two_points);

	IntArray facelist_one, facelist_two;
	shell_one.ShowFacelist(facelist_one);
	shell_two.ShowFacelist(facelist_two);

	size_t facelist_one_size = facelist_one.size();
	size_t facelist_two_size = facelist_two.size();

	float minimum_distance_squared = (std::numeric_limits<float>::max)();
	Point point_one;
	Point point_two;
	bool early_out = false;

	//brute-force comparison of the triangle pairs making up the two faces
	//we know that Exchange inserts shells which are made-up of triangles
	for (size_t one = 0; one < facelist_one_size && !early_out; one += 4)
	{
		//get the points making up the first triangle
		Point t11 = shell_one_points[facelist_one[one + 1]];
		Point t12 = shell_one_points[facelist_one[one + 2]];
		Point t13 = shell_one_points[facelist_one[one + 3]];

		for (size_t two = 0; two < facelist_two_size; two += 4)
		{
			//get the points making up the second triangle
			Point t21 = shell_two_points[facelist_two[two + 1]];
			Point t22 = shell_two_points[facelist_two[two + 2]];
			Point t23 = shell_two_points[facelist_two[two + 3]];

			auto check_distance = [&](Point const & comparison_point, Point const & closest_point)
			{
				double distance_squared = Vector(comparison_point - closest_point).LengthSquared();
				if (distance_squared < minimum_distance_squared)
				{
					minimum_distance_squared = (float)distance_squared;
					point_one = comparison_point;
					point_two = closest_point;
				}

				if (HPS::Float::Equals(distance_squared, 0.0))
				{
					early_out = true;
					return true;
				}
				else
					return false;

			};

			auto check_distance_2 = [&](float distance, Point const & c1, Point const & c2)
			{
				if (distance < minimum_distance_squared)
				{
					minimum_distance_squared = distance;
					point_one = c1;
					point_two = c2;
				}

				if (HPS::Float::Equals(distance, 0.0f))
				{
					early_out = true;
					return true;
				}
				else
					return false;
			};

			//compute 6 vertex-triangle tests
			Point closest_point = ClosestPointOnTriangleToPoint(t11, t21, t22, t23);
			if (check_distance(t11, closest_point))
				break;
			closest_point = ClosestPointOnTriangleToPoint(t12, t21, t22, t23);
			if (check_distance(t12, closest_point))
				break;
			closest_point = ClosestPointOnTriangleToPoint(t13, t21, t22, t23);
			if (check_distance(t13, closest_point))
				break;
			closest_point = ClosestPointOnTriangleToPoint(t21, t11, t12, t13);
			if (check_distance(t21, closest_point))
				break;
			closest_point = ClosestPointOnTriangleToPoint(t22, t11, t12, t13);
			if (check_distance(t22, closest_point))
				break;
			closest_point = ClosestPointOnTriangleToPoint(t23, t11, t12, t13);
			if (check_distance(t23, closest_point))
				break;

			//compute 9 edge-edge tests
			Point out_point_one, out_point_two;
			float squared_distance = (std::numeric_limits<float>::max)();

			squared_distance = ClosestPointSegmentSegment(t11, t12, t21, t22, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
			squared_distance = ClosestPointSegmentSegment(t12, t13, t21, t22, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
			squared_distance = ClosestPointSegmentSegment(t13, t11, t21, t22, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;

			squared_distance = ClosestPointSegmentSegment(t11, t12, t22, t23, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
			squared_distance = ClosestPointSegmentSegment(t12, t13, t22, t23, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
			squared_distance = ClosestPointSegmentSegment(t13, t11, t22, t23, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;

			squared_distance = ClosestPointSegmentSegment(t11, t12, t23, t21, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
			squared_distance = ClosestPointSegmentSegment(t12, t13, t23, t21, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
			squared_distance = ClosestPointSegmentSegment(t13, t11, t23, t21, out_point_one, out_point_two);
			if (check_distance_2(squared_distance, out_point_one, out_point_two))
				break;
		}
	}

	InsertFeatureToFeatureGeometry(point_one, point_two, sqrt(minimum_distance_squared));
}

void Exchange::MeasurementOperator::LineToLineDistance()
{
	//measure the minimum distance between two infinite lines
	LineKey normal_one = surface_one.normal_points;
	LineKey normal_two = surface_two.normal_points;

	PointArray normal_one_points;
	normal_one.ShowPoints(normal_one_points);
	PointArray normal_two_points;
	normal_two.ShowPoints(normal_two_points);

	Point point_one;
	Point point_two;
	float distance = LineSegmentDistance(normal_one_points[0], normal_one_points[1], normal_two_points[0], normal_two_points[1], normal_one, normal_two, point_one, point_two, false);

	Vector one(normal_one_points[1] - normal_one_points[0]);
	Vector two(normal_two_points[1] - normal_two_points[0]);
	if (one.Normalize() == two.Normalize())
	{
		if (two == Vector(1, 0, 0))
			two = Vector(0, 1, 0);
		else
			two = Vector(1, 0, 0);
	}

	Vector normal = one.Cross(two);
	measurement_plane = Plane(normal, normal_one_points[0]);

	InsertFeatureToFeatureGeometry(point_one, point_two, distance);
}

float Exchange::MeasurementOperator::LineSegmentDistance(Point & p0, Point & p1, Point & q0, Point & q1, LineKey & normal_one, LineKey & normal_two, Point & out_point_on_edge, Point & out_point_on_center_line, bool clamp)
{
	//this is a fork of the algorithm found here http://geomalgorithms.com/a07-_distance.html
	//calculate minimum distance between line from p0 to p1 and line from q0 to q1
	Vector u(p1 - p0);
	Vector v(q1 - q0);

	const float epsilon = 0.000000001f;
	if (!clamp)
	{
		//check that the two segments are not part of the same line
		Vector ac(q0 - p0);
		Vector cross_one = u.Cross(ac);
		Vector ad(q1 - p0);
		Vector cross_two = u.Cross(ad);
		if (cross_one.Length() <= epsilon && cross_two.Length() <= epsilon)
		{
			out_point_on_center_line = HPS::Midpoint(p1, p0);
			out_point_on_edge = out_point_on_center_line;
			return 0;
		}
	}

	Vector w(p0 - q0);

	float a = u.Dot(u);
	float b = u.Dot(v);
	float c = v.Dot(v);
	float d = u.Dot(w);
	float e = v.Dot(w);
	float D = a * c - b * b;
	float sc = 0.0f;
	float tc = 0.0f;

	bool recompute_sc = false;
	if (D < epsilon)
	{
		//if the lines are basically parallel, the distance is the same everywhere
		//choose the mid-point of a line, and compute the parameter of the other one
		//accordingly
		tc = 0.5f;
		recompute_sc = true;
	}
	else
	{
		sc = (b * e - c * d) / D;
		tc = (a * e - b * d) / D;
	}

	if ((tc < 0 || tc > 1) && clamp || recompute_sc)
	{
		//in the case of comparing a segment to an infinite line
		//(the planar surface vs. conical/cylindrical surface case)
		//the parameter for the line representing the segment needs to be clamped.
		//the other parameter needs to be recalculated based on the clamped value
		tc = HPS::Clamp(tc, 0.0f, 1.0f);
		Point point_on_line = q0 + tc * v;
		Vector diagonal(point_on_line - p0);
		sc = (float)(u.Dot(diagonal) / a);
	}

	Vector p = w + (sc * u) - (tc * v);

	out_point_on_edge = q0 + v * tc;
	out_point_on_center_line = p0 + u * sc;

	//resize the normal line if needed
	if (sc < 0)
	{
		Point normal_points[] = { out_point_on_center_line, p1 };
		normal_one.EditPointsByReplacement(0, 2, normal_points);
		p0 = out_point_on_center_line;
	}
	else if (sc > 1)
	{
		Point normal_points[] = { p0, out_point_on_center_line };
		normal_one.EditPointsByReplacement(0, 2, normal_points);
		p1 = out_point_on_center_line;
	}

	if (!clamp)
	{
		//resize the other line if we are doing a comparison between two infinite lines
		if (tc < 0)
		{
			Point normal_points[] = { out_point_on_edge, q1 };
			normal_two.EditPointsByReplacement(0, 2, normal_points);
			q0 = out_point_on_edge;
		}
		else if (sc > 1)
		{
			Point normal_points[] = { q0, out_point_on_edge };
			normal_two.EditPointsByReplacement(0, 2, normal_points);
			q1 = out_point_on_edge;
		}
	}

	return (float)p.Length();
}

Point Exchange::MeasurementOperator::ClosestPointOnTriangleToPoint(Point const & p, Point const & a, Point const & b, Point const & c)
{
	//Implementation taken from Real-Time Collision Detection

	//Check if P in vertex region outside A
	Vector ab(b - a);
	Vector ac(c - a);
	Vector ap(p - a);
	float d1 = ab.Dot(ap);
	float d2 = ac.Dot(ap);
	if (d1 <= 0.0f && d2 <= 0.0f)
		return a;

	//Check if P in vertex region outside B
	Vector bp(p - b);
	float d3 = ab.Dot(bp);
	float d4 = ac.Dot(bp);
	if (d3 >= 0.0f && d4 <= d3)
		return b;

	//Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return Point(a + v * ab);
	}

	//Check if P in vertex region outside C
	Vector cp(p - c);
	float d5 = ab.Dot(cp);
	float d6 = ac.Dot(cp);
	if (d6 >= 0.0f && d5 <= d6)
		return c;

	//Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return Point(a + w * ac);
	}

	//Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return Point(b + w * Vector(c - b));
	}

	//P inside face region. Compute Q through its barycentric coordinates (u, v, w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return Point(a + ab * v + ac * w);
}

float Exchange::MeasurementOperator::ClosestPointSegmentSegment(Point const & p1, Point const & q1, Point const & p2, Point const & q2, Point & c1, Point & c2)
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
		return Vector(c1 - c2).Dot(Vector(c1 - c2));
	}

	if (a <= epsilon)
	{
		//First segment degenerates into a point
		s = 0.0f;
		t = f / e;
		t = HPS::Clamp(t, 0.0f, 1.0f);
	}
	else
	{
		float c = d1.Dot(r);
		if (e <= epsilon)
		{
			//Second segment degenerates into a point
			t = 0.0f;
			s = HPS::Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			//General case
			float b = d1.Dot(d2);
			float denom = a * e - b * b;

			//if segments are non parallel compute closest point on L1 to L2 and
			//clamp to segment S1. Else pick arbitrary s (here 0)
			if (denom != 0.0f)
				s = HPS::Clamp((b * f - c* e) / denom, 0.0f, 1.0f);
			else
				s = 0.0f;

			float tnom = b * s + f;
			if (tnom < 0.0f)
			{
				t = 0.0f;
				s = HPS::Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (tnom > e)
			{
				t = 1.0f;
				s = HPS::Clamp((b - c) / a, 0.0f, 1.0f);
			}
			else
				t = tnom / e;
		}
	}

	c1 = p1 + d1 * s;
	c2 = p2 + d2 * t;
	return Vector(c1 - c2).Dot(Vector(c1 - c2));
}

bool Exchange::MeasurementOperator::IsPlane(Exchange::Component const & face_component)
{
	bool is_plane = false;
	A3DTopoFaceData face_data;
	A3D_INITIALIZE_DATA(A3DTopoFaceData, face_data);
	if (A3DTopoFaceGet(face_component.GetExchangeEntity(), &face_data) == A3D_SUCCESS)
	{
		if (face_data.m_pSurface != nullptr)
		{
			A3DEEntityType a3d_surface_type;
			if (A3DEntityGetType(face_data.m_pSurface, &a3d_surface_type) == A3D_SUCCESS)
			{
				if (a3d_surface_type == kA3DTypeSurfPlane)
					is_plane = true;
				else
				{
					A3DEEntityType recognized_surfaces[] = { kA3DTypeSurfCone, kA3DTypeSurfCylinder, kA3DTypeSurfPlane };
					A3DSurfBase * simplified_surface;
					A3DEAnalyticType simplified_type;
					if (A3DSimplifySurfaceWithAnalytics(face_data.m_pSurface, 0.0001, 3, recognized_surfaces, &simplified_surface, &simplified_type) == A3D_SUCCESS)
					{
						if (simplified_type == kA3DAnalyticPlane)
							is_plane = true;

						A3DEntityDelete(simplified_surface);
					}
				}
			}
		}

		A3DTopoFaceGet(nullptr, &face_data);
	}

	return is_plane;
}

Point Exchange::MeasurementOperator::GetPlaneIntersection(Plane const & in_plane, KeyPath const & in_key_path, WindowPoint const & in_window_point)
{
	//calculate the projection of the vector made by the world cursor position and the center on the circle plane
	Point window_cursor2 = in_window_point;
	window_cursor2.z += 100;

	Point world_cursor;
	Point world_cursor2;
	in_key_path.ConvertCoordinate(Coordinate::Space::Window, in_window_point, Coordinate::Space::World, world_cursor);
	in_key_path.ConvertCoordinate(Coordinate::Space::Window, window_cursor2, Coordinate::Space::World, world_cursor2);
	Plane plane(camera_direction, first_click_position);
	return in_plane.IntersectLineSegment2(world_cursor, world_cursor2);
}

bool Exchange::MeasurementOperator::Highlight(MeasurementType in_measurement_type, WindowPoint const & in_location, WindowKey & in_window, KeyPath const & in_path)
{
	KeyPath event_path = in_path;
	event_path = GetAttachedView().GetAttachedModel().GetSegmentKey() 
		+ GetAttachedView().GetAttachedModelIncludeLink() 
		+ GetAttachedView().GetModelOverrideSegmentKey() + event_path;

	bool update_required = false;

	if (highlight_update_notifier.Status() == HPS::Window::UpdateStatus::InProgress || disable_highlighting)
		return update_required;

	auto highlight_control = in_window.GetHighlightControl();
	HPS::KeyPath complete_path;
	SelectionResults results;
	Key selected_key;
	KeyPath selection_path;

	auto check_selection = [&]()
	{
		complete_path = KeyPath(selected_key + selection_path);
		if (!highlighted_path.Empty() && complete_path == highlighted_path)
			return false;
		else if (!highlighted_path.Empty())
		{
			highlight_control.Unhighlight(highlighted_path, mouse_over_highlight_options);
			highlight_control.Unhighlight(edit_measurement_highlight_options);
			highlighted_path = KeyPath();
			update_required = true;
		}
		return true;
	};

	if (in_measurement_type == Exchange::MeasurementOperator::MeasurementType::EdgeAndRadius && operator_active == false)
	{
		edge_radius_selection.SetScope(event_path);
		size_t selection_count = in_window.GetSelectionControl().SelectByPoint(in_location, edge_radius_selection, results);
		if (selection_count == 0)
		{
			if (!highlighted_path.Empty())
			{
				highlight_control.Unhighlight(highlighted_path, mouse_over_highlight_options);
				highlight_control.Unhighlight(edit_measurement_highlight_options);
				highlighted_path = KeyPath();
				update_required = true;
			}
			
			return update_required;
		}

		auto it = results.GetIterator();
		while (it.IsValid())
		{
			auto item = it.GetItem();
			item.ShowSelectedItem(selected_key);
			item.ShowPath(selection_path);
			if (selected_key.Type() == HPS::Type::LineKey)
				break;

			it.Next();
		}

		if (!check_selection())
			return update_required;

		HPS::Exchange::Component edge_component = cad_model.GetComponentPath(complete_path).Front();
		if (edge_component.Empty() || edge_component.GetComponentType() != Component::ComponentType::ExchangeTopoEdge)
			return update_required;
	}
	else if ((in_measurement_type == Exchange::MeasurementOperator::MeasurementType::FeatureToFeature ||
		in_measurement_type == Exchange::MeasurementOperator::MeasurementType::FaceAngle) 
		&& anchors < 2)
	{
		feature_to_feature_selection.SetScope(event_path);
		size_t selection_count = in_window.GetSelectionControl().SelectByPoint(in_location, feature_to_feature_selection, results);
		if (selection_count == 0)
		{
			if (!highlighted_path.Empty())
			{
				highlight_control.Unhighlight(highlighted_path, mouse_over_highlight_options);
				highlight_control.Unhighlight(edit_measurement_highlight_options);
				highlighted_path = KeyPath();
				update_required = true;
			}

			return update_required;
		}

		SelectionItem selection_item = results.GetIterator().GetItem();
		selection_item.ShowSelectedItem(selected_key);
		selection_item.ShowPath(selection_path);

		if (!check_selection())
			return update_required;

		auto component_path = cad_model.GetComponentPath(complete_path);
		HPS::Exchange::Component face_component = component_path.Front();
		if (face_component.Empty() || face_component.GetComponentType() != Component::ComponentType::ExchangeTopoFace)
			return update_required;
		
		if (component_path == surface_one.path || component_path == surface_two.path)
			return update_required;

		if (in_measurement_type == Exchange::MeasurementOperator::MeasurementType::FeatureToFeature)
		{
			Surface temp_surface;
			GetSurfaceType(face_component, temp_surface);
			if (temp_surface.surface_type == Surface::SurfaceType::Unsupported)
				return update_required;
		}
		else
		{
			if (!IsPlane(face_component))
				return update_required;

			if (anchors == 1)
			{
				ShellKey shell = (ShellKey)face_component.GetKeys()[0];
				VectorArray normals;
				shell.ShowNetVertexNormalsByRange(0, 1, normals);
				HPS::KeyPath shell_path(selected_key + selection_path);
				MatrixKit net_matrix;
				shell_path.ShowNetModellingMatrix(net_matrix);
				normals = net_matrix.Transform(normals);
				float dot_product = fabs(first_face_normal.Dot(normals[0]));
				if (Float::Equals(dot_product, 1))
					return update_required;
			}
		}
	}

	if (in_measurement_type == Exchange::MeasurementOperator::MeasurementType::FeatureToFeature)
	{
		ComponentPath component_path(cad_model.GetComponentPath(complete_path));
		if (!component_path.Empty())
		{
			HPS::ComponentPath edge_path = component_path;
			for (auto const & loop : component_path.Front().GetSubcomponents())
			{
				edge_path.PushFront(loop);
				for (auto const & coedge : loop.GetSubcomponents())
				{
					edge_path.PushFront(coedge);
					for (auto const & edge : coedge.GetSubcomponents())
					{
						edge_path.PushFront(edge);
						highlight_control.Highlight(edge_path.GetKeyPaths()[0], edit_measurement_highlight_options);
					}
				}
			}
		}
	}

	highlighted_path = complete_path;
	highlight_control.Highlight(complete_path, mouse_over_highlight_options);
	update_required = true;
	return update_required;
}

bool HPS::Exchange::MeasurementOperator::IsMeasurementActive()
{
	if (manipulate_measurement)
		return true;
	else
	{
		MeasurementType measurement_to_edit = manipulate_measurement ? temporary_measurement_type : measurement_type;
		if ((measurement_to_edit == MeasurementType::EdgeAndRadius || measurement_to_edit == MeasurementType::FeatureToFeature) && operator_active)
			return true;
		else if (anchors == 2)
			return true;
	}

	return false;
}

void HPS::Exchange::MeasurementOperator::DeleteLastMeasurement()
{
	ResetMeasurement();
	highlighted_path.Reset();
}
