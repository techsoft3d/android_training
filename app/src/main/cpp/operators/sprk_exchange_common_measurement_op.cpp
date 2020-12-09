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
#	pragma warning ( pop )
#endif

#include <sstream>

using namespace HPS;

size_t Exchange::CommonMeasurementOperator::length_measurement_index	= 1;
size_t Exchange::CommonMeasurementOperator::radius_measurement_index	= 1;
size_t Exchange::CommonMeasurementOperator::distance_measurement_index	= 1;
size_t Exchange::CommonMeasurementOperator::angle_measurement_index		= 1;

Exchange::CommonMeasurementOperator::CommonMeasurementOperator()
	: Operator()
	, measurement_precision(2)
	, manipulate_measurement(false)
{

}

Exchange::CommonMeasurementOperator::CommonMeasurementOperator(Exchange::CADModel const & in_cad_model, MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: Operator(in_mouse_trigger, in_modifier_trigger)
	, cad_model(in_cad_model)
	, measurement_precision(2)
	, manipulate_measurement(false)
{
	selection_options.SetAlgorithm(Selection::Algorithm::Analytic).SetLevel(Selection::Level::Entity).SetProximity(0.01).SetSorting(Selection::Sorting::Default);
	text_attributes = TextAttributeKit::GetDefault();
	text_attributes.SetSize(0.015, Text::SizeUnits::WindowRelative);
	materials = MaterialMappingKit::GetDefault();
	materials.SetTextColor(RGBColor(1, 0, 0)).SetLineColor(RGBColor(1, 0, 0)).SetMarkerColor(RGBColor(1, 0, 0)).SetFaceColor(RGBColor(1, 0, 0));
	GetUnits();
}

Exchange::CommonMeasurementOperator::MeasurementInsertedEvent::~MeasurementInsertedEvent()
{
}

Exchange::CommonMeasurementOperator::MeasurementDeletedEvent::~MeasurementDeletedEvent()
{
}

void Exchange::CommonMeasurementOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	SetupConstructionSegment();
	View view = in_attached_view;
	portfolio = view.GetPortfolioKey();
	left_arrow = portfolio.DefineGlyph("left_arrow", HPS::GlyphKit::GetDefault(Glyph::Default::SolidTriangleLeft));
	right_arrow = portfolio.DefineGlyph("right_arrow", HPS::GlyphKit::GetDefault(Glyph::Default::SolidTriangleRight));
	SetGlyphColor();

	GlyphLinePatternElement start_cap_glyph;
	start_cap_glyph.SetSource("left_arrow");
	GlyphLinePatternElement end_cap_glyph;
	end_cap_glyph.SetSource("right_arrow");
	LinePatternParallelKit line_parallel;
	line_parallel.SetStartCap(start_cap_glyph).SetEndCap(end_cap_glyph);
	LinePatternKit line_pattern;
	line_pattern.SetParallels(LinePatternParallelKitArray(1, line_parallel));
	portfolio.DefineLinePattern("HPS_measurement_pointing_out", line_pattern);

	line_parallel.SetStartCap(end_cap_glyph).SetEndCap(start_cap_glyph);
	line_pattern.UnsetEverything();
	line_pattern.SetParallels(LinePatternParallelKitArray(1, line_parallel));
	portfolio.DefineLinePattern("HPS_measurement_pointing_in", line_pattern);

	style_segment = Database::CreateRootSegment();
	style_segment.SetMaterialMapping(materials);
	portfolio.DefineNamedStyle("measurement_highlight", style_segment);
	highlight_options.SetStyleName("measurement_highlight").SetOverlay(Drawing::Overlay::InPlace);
}

void Exchange::CommonMeasurementOperator::SetupConstructionSegment()
{
	SegmentKey model_segment = cad_model.GetModel().GetSegmentKey();
	measurement_segment = model_segment.Subsegment("construction segments").Subsegment("measurement");
	measurement_segment.GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::Default);
	measurement_segment.GetVisibilityControl().SetText(true).SetLines(true).SetEdges(true).SetMarkers(true);
	measurement_segment.GetPerformanceControl().SetStaticModel(Performance::StaticModel::None);

	measurement_segment.GetTextAttributeControl().SetPreference(Text::Preference::Vector);

	measurement_segment.SetTextAttribute(text_attributes);
	SetMaterialsOnMeasurementSegment(measurement_segment, materials);
	measurement_segment.GetAttributeLockControl().SetLock(AttributeLock::Type::Material);
}

void Exchange::CommonMeasurementOperator::OnViewDetached(HPS::View const &)
{
	style_segment.Delete();
	portfolio.UndefineNamedStyle("measurement_highlight");
}

Exchange::CADModel Exchange::CommonMeasurementOperator::GetCADModel() const
{
	return cad_model;
}

void Exchange::CommonMeasurementOperator::SetCADModel(Exchange::CADModel const & in_cad_model)
{
	cad_model = in_cad_model;
	GetUnits();
}

size_t Exchange::CommonMeasurementOperator::GetPrecision() const
{
	return measurement_precision;
}

void Exchange::CommonMeasurementOperator::SetPrecision(size_t in_precision)
{
	measurement_precision = in_precision;
}

void Exchange::CommonMeasurementOperator::GetUnits()
{
	StringMetadata string_metadata = cad_model.GetMetadata("Units");
	UTF8 string_metadata_value = string_metadata.GetValue();
	if (string_metadata_value != "Unknown")
	{
		if (string_metadata_value == "Millimeter")
			units = HPS::UTF8("mm", "utf8");
		else if (string_metadata_value == "Centimeter")
			units = HPS::UTF8("cm", "utf8");
		else if (string_metadata_value == "Meter")
			units = HPS::UTF8("m", "utf8");
		else if (string_metadata_value == "Kilometer")
			units = HPS::UTF8("Km", "utf8");
		else if (string_metadata_value == "Point")
			units = HPS::UTF8("pts", "utf8");
		else if (string_metadata_value == "Pica")
			units = HPS::UTF8("pc", "utf8");
		else if (string_metadata_value == "Inch")
			units = HPS::UTF8("in", "utf8");
		else if (string_metadata_value == "Foot")
			units = HPS::UTF8("ft", "utf8");
		else if (string_metadata_value == "Yard")
			units = HPS::UTF8("yd", "utf8");
		else if (string_metadata_value == "Mile")
			units = HPS::UTF8("mi", "utf8");
	}
}

MaterialMappingKit Exchange::CommonMeasurementOperator::GetMaterial() const
{
	return materials;
}

void Exchange::CommonMeasurementOperator::SetMaterialsOnMeasurementSegment(HPS::SegmentKey const & set_materials_here, HPS::MaterialMappingKit const & materials_to_apply)
{
	HPS::Material::Type material_type;
	HPS::RGBAColor rgba_color;
	float value;

	HPS::MaterialMappingControl material_mapping_control = set_materials_here.GetMaterialMappingControl();
	if (materials_to_apply.ShowLineColor(material_type, rgba_color, value))
	{
		if (material_type == Material::Type::MaterialIndex)
			material_mapping_control.SetLineMaterialByIndex(value);
		else
			material_mapping_control.SetLineColor(rgba_color);
	}

	if (materials_to_apply.ShowTextColor(material_type, rgba_color, value))
	{
		if (material_type == Material::Type::MaterialIndex)
			material_mapping_control.SetTextMaterialByIndex(value);
		else
			material_mapping_control.SetTextColor(rgba_color);
	}

	if (materials_to_apply.ShowMarkerColor(material_type, rgba_color, value))
	{
		if (material_type == Material::Type::MaterialIndex)
			material_mapping_control.SetMarkerMaterialByIndex(value);
		else
			material_mapping_control.SetMarkerColor(rgba_color);
	}
}

void Exchange::CommonMeasurementOperator::SetMaterial(MaterialMappingKit const & in_material_mapping)
{
	materials = in_material_mapping;
	if (!measurement_segment.Empty())
	{
		SetMaterialsOnMeasurementSegment(measurement_segment, materials);
		SetGlyphColor();
		style_segment.SetMaterialMapping(materials);
	}
}

TextAttributeKit Exchange::CommonMeasurementOperator::GetTextAttributes() const
{
	return text_attributes;
}

void Exchange::CommonMeasurementOperator::SetTextAttribute(TextAttributeKit const & in_text_attributes)
{
	text_attributes = in_text_attributes;
	if (!measurement_segment.Empty())
		measurement_segment.SetTextAttribute(text_attributes);
}

void Exchange::CommonMeasurementOperator::SetGlyphColor()
{
	if (!left_arrow.Empty() && !right_arrow.Empty())
	{
		RGBAColor glyph_color;
		Material::Type material_type;
		float index;
		materials.ShowMarkerColor(material_type, glyph_color, index);

		GlyphKit glyph;
		auto set_glyph_color = [&] ()
		{
			GlyphElementArray elements;
			glyph.ShowElements(elements);

			for (auto & element : elements)
			{
				if (material_type == Material::Type::MaterialIndex)
					element.SetIndexedColor((byte)index);
				else
					element.SetExplicitColor(glyph_color);
			}
			glyph.SetElements(elements);
		};

		left_arrow.Show(glyph);
		set_glyph_color();
		left_arrow.Set(glyph);

		right_arrow.Show(glyph);
		set_glyph_color();
		right_arrow.Set(glyph);
	}
}

void Exchange::CommonMeasurementOperator::GetCameraDirection()
{
	CameraKit camera;
	GetAttachedView().GetSegmentKey().ShowCamera(camera);
	Point position, target;
	camera.ShowPosition(position);
	camera.ShowTarget(target);
	camera_direction = Vector(target - position);
	camera_direction = camera_direction.Normalize();
}

void Exchange::CommonMeasurementOperator::PositionLinearMeasurementGeometry(
	WindowPoint const & window_cursor_location,	// mouse cursor location in window coordinates
	KeyPath const & event_path,					// event path
	LineKey & leader_line_one,					// line extending from one end of the original measured geometry to the measurement_line
	LineKey & leader_line_two,					// line extending from the other end of the original measured geometry to the measurement_line
	LineKey & measurement_line,					// line between leader lines
	LineKey & line_to_cursor,					// line extending to the cursor
	Point & original_point_one,					// one point at one end of the geometry being measured
	Point & original_point_two,					// one point at the other end of the geometry being measured
	TextKey & text,								// the measurement label
	UTF8 const & text_string,					// what the label says
	Point & distance_point_one,					// intersection of leader_line_one and measurement_line
	Point & distance_point_two,					// intersection of leader_line_two and measurement_line
	SegmentKey & current_measurement_segment,
	Plane const & measurement_plane,
	Vector const & explicit_direction)
{
	Vector measurement_direction(original_point_two - original_point_one);
	float measure_distance = 0;
	Vector direction;

	HPS::Point world_point;
	event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, window_cursor_location, HPS::Coordinate::Space::World, world_point);

	auto get_plane_intersection = [](KeyPath const & in_key_path, WindowPoint const & in_window_location, Plane const & in_measurement_plane)
	{
		Point window_cursor2 = in_window_location;
		window_cursor2.z += 100;

		Point world_cursor;
		Point world_cursor2;
		in_key_path.ConvertCoordinate(Coordinate::Space::Window, in_window_location, Coordinate::Space::World, world_cursor);
		in_key_path.ConvertCoordinate(Coordinate::Space::Window, window_cursor2, Coordinate::Space::World, world_cursor2);
		return in_measurement_plane.IntersectLineSegment2(world_cursor, world_cursor2);
	};

	if (explicit_direction == Vector::Zero())
	{
		Plane plane(camera_direction, original_point_one);
		Point intersection = get_plane_intersection(event_path, window_cursor_location, plane);
		Vector anchor_to_cursor(intersection - original_point_one);
		float anchor_to_cursor_distance = (float)anchor_to_cursor.Length();
		anchor_to_cursor = anchor_to_cursor.Normalize();

		direction = camera_direction.Cross(measurement_direction);
		direction = direction.Normalize();
		float angle = HPS::ACos(anchor_to_cursor.Dot(direction));
		measure_distance = anchor_to_cursor_distance * HPS::Cos(angle);
	}
	else
	{
		direction = explicit_direction;
		measurement_direction = measurement_direction.Normalize();
		Plane explicit_measurement_plane(original_point_one, direction.Cross(measurement_direction));
		Point intersection = get_plane_intersection(event_path, window_cursor_location, explicit_measurement_plane);

		Vector anchor_to_cursor(intersection - original_point_one);
		float anchor_to_cursor_distance = (float)anchor_to_cursor.Length();
		anchor_to_cursor = anchor_to_cursor.Normalize();

		direction = explicit_direction;
		float angle = HPS::ACos(anchor_to_cursor.Dot(direction));
		measure_distance = anchor_to_cursor_distance * HPS::Cos(angle);
	}

	Point measure_point_one = original_point_one + direction * measure_distance;
	Point measure_point_two = original_point_two + direction * measure_distance;

	float const epsilon = 0.00001;
	if (fabs(measure_point_one.x - measure_point_two.x) <= epsilon &&
		fabs(measure_point_one.y - measure_point_two.y) <= epsilon &&
		fabs(measure_point_one.z - measure_point_two.z) <= epsilon)
	{
		//measurement of size 0
		Point window_cursor2 = window_cursor_location;
		window_cursor2.z += 100;

		Point world_cursor;
		Point world_cursor2;
		event_path.ConvertCoordinate(Coordinate::Space::Window, window_cursor_location, Coordinate::Space::World, world_cursor);
		event_path.ConvertCoordinate(Coordinate::Space::Window, window_cursor2, Coordinate::Space::World, world_cursor2);
		world_point = measurement_plane.IntersectLineSegment2(world_cursor, world_cursor2);

		Point line_to_cursor_points [] = {world_point, world_point};
		line_to_cursor.EditPointsByReplacement(0, 2, line_to_cursor_points);

		Point connector_one_points [] = {original_point_one, world_point};
		leader_line_one.EditPointsByReplacement(0, 2, connector_one_points);
		Point connector_two_points [] = {original_point_two, world_point};
		leader_line_two.EditPointsByReplacement(0, 2, connector_two_points);
		distance_point_one = world_point;
		distance_point_two = world_point;
	}
	else
	{
		Point measurement_line_points [] = {measure_point_one, measure_point_two};
		measurement_line.EditPointsByReplacement(0, 2, measurement_line_points);

		//in order to track the cursor correctly, operate in camera space and restrain the line to the vector defined by the measurement points
		Point measure_point_one_camera;
		Point measure_point_two_camera;
		Point cursor_camera;
		event_path.ConvertCoordinate(Coordinate::Space::Window, window_cursor_location, Coordinate::Space::Camera, cursor_camera);
		event_path.ConvertCoordinate(Coordinate::Space::World, measure_point_one, Coordinate::Space::Camera, measure_point_one_camera);
		event_path.ConvertCoordinate(Coordinate::Space::World, measure_point_two, Coordinate::Space::Camera, measure_point_two_camera);

		float t = 0;
		if (Float::Equals(measure_point_two_camera.x - measure_point_one_camera.x, 0))
			t = (cursor_camera.y - measure_point_one_camera.y) / (measure_point_two_camera.y - measure_point_one_camera.y);
		else
			t = (cursor_camera.x - measure_point_one_camera.x) / (measure_point_two_camera.x - measure_point_one_camera.x);

		Point cursor_camera_point = measure_point_one_camera + t * (measure_point_two_camera - measure_point_one_camera);
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Camera, cursor_camera_point, HPS::Coordinate::Space::World, world_point);

		Point line_to_cursor_points [] = {measure_point_one, world_point};
		line_to_cursor.EditPointsByReplacement(0, 2, line_to_cursor_points);

		//change the line pattern used if needed:
		//have the arrows point out if the cursor is within the measurement
		//have the arrows point in otherwise
		Vector line_to_cursor_vector(world_point - measure_point_one);
		Vector distance_vector(measure_point_two - measure_point_one);
		double distance_vector_length = distance_vector.Length();
		double line_to_cursor_vector_length = line_to_cursor_vector.Length();
		float dot_p = distance_vector.Dot(line_to_cursor_vector);

		SegmentKey patterned_line_segment = current_measurement_segment.Subsegment("patterned_line");
		if (dot_p < 0 ||
			(dot_p > 0 && line_to_cursor_vector_length > distance_vector_length))
			patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_in");
		else
			patterned_line_segment.GetLineAttributeControl().SetPattern("HPS_measurement_pointing_out");

		Point connector_one_points [] = {original_point_one, measure_point_one};
		leader_line_one.EditPointsByReplacement(0, 2, connector_one_points);
		Point connector_two_points [] = {original_point_two, measure_point_two};
		leader_line_two.EditPointsByReplacement(0, 2, connector_two_points);
		distance_point_one = measure_point_one;
		distance_point_two = measure_point_two;
	}

	text.Delete();
	text = current_measurement_segment.InsertText(world_point, text_string);
}

void Exchange::CommonMeasurementOperator::Tag(HPS::Key & tag, const char * message, Tags tag_index)
{
	auto to_bytes = [](const char * message)
	{
		HPS::ByteArray data;
		size_t len = strlen(message);
		for (size_t i = 0; i < len; ++i) 
			data.push_back(message[i]);
		data.push_back('\0');
		return data;
	};

	size_t index = static_cast<size_t>(tag_index);
	HPS::Type tag_type = tag.Type();
	switch (tag_type)
	{
	case HPS::Type::SegmentKey:
		{
			SegmentKey tag_me(tag);
			HPS::ByteArray data = to_bytes(message);
			tag_me.SetUserData(index, data);
		} break;
	case HPS::Type::LineKey:
		{
			LineKey tag_me(tag);
			HPS::ByteArray data = to_bytes(message);
			tag_me.SetUserData(index, data);
		} break;
	case HPS::Type::MarkerKey:
		{
			MarkerKey tag_me(tag);
			HPS::ByteArray data = to_bytes(message);
			tag_me.SetUserData(index, data);
		} break;
	case HPS::Type::TextKey:
		{
			TextKey tag_me(tag);
			HPS::ByteArray data = to_bytes(message);
			tag_me.SetUserData(index, data);
		} break;
	case HPS::Type::CircularArcKey:
		{
			CircularArcKey tag_me(tag);
			HPS::ByteArray data = to_bytes(message);
			tag_me.SetUserData(index, data);
		} break;
		default: break;
	}
}

float Exchange::CommonMeasurementOperator::GetModelScale(Exchange::Component const & component)
{
	//get the scale from the model
	double scale = 1;
	HPS::Exchange::Component body;

	auto component_type = component.GetComponentType();
	if (component_type == Component::ComponentType::ExchangeTopoEdge)
	{
		HPS::ComponentArray co_edges = component.GetOwners();
		HPS::ComponentArray loops = co_edges[0].GetOwners();
		HPS::ComponentArray faces = loops[0].GetOwners();
		HPS::ComponentArray shells = faces[0].GetOwners();
		HPS::ComponentArray connexes = shells[0].GetOwners();
		HPS::ComponentArray bodies = connexes[0].GetOwners();
		body = bodies[0];
	}
	else if (component_type == Component::ComponentType::ExchangeTopoFace)
	{
		HPS::ComponentArray shells = component.GetOwners();
		HPS::ComponentArray connexes = shells[0].GetOwners();
		HPS::ComponentArray bodies = connexes[0].GetOwners();
		body = bodies[0];
	}
	else
		return 0;

	A3DTopoBodyData topo_body_data;
	A3D_INITIALIZE_DATA(A3DTopoBodyData, topo_body_data);

	if (A3DTopoBodyGet(body.GetExchangeEntity(), &topo_body_data) == A3D_SUCCESS)
	{
		A3DTopoContextData context_data;
		A3D_INITIALIZE_DATA(A3DTopoContextData, context_data);

		if (A3DTopoContextGet(topo_body_data.m_pContext, &context_data) == A3D_SUCCESS &&
			context_data.m_bHaveScale)
			scale = context_data.m_dScale;

		A3DTopoContextGet(nullptr, &context_data);
	}
	A3DTopoBodyGet(nullptr, &topo_body_data);

	return (float)scale;
}

void Exchange::CommonMeasurementOperator::DeleteMeasurements()
{
	measurement_segment.Delete();
	SetupConstructionSegment();
}

SegmentKey Exchange::CommonMeasurementOperator::GetMeasurementSegment() const
{
	return measurement_segment;
}

UTF8 Exchange::CommonMeasurementOperator::GetNewMeasurementSegmentName(HPS::UTF8 const & in_prefix)
{
	std::stringstream segment_name;
	if (in_prefix == HPS::UTF8("Angle"))
		segment_name << in_prefix << "." << angle_measurement_index++;
	else if (in_prefix == HPS::UTF8("Distance"))
		segment_name << in_prefix << "." << distance_measurement_index++;
	else if (in_prefix == HPS::UTF8("Radius"))
		segment_name << in_prefix << "." << radius_measurement_index++;
	else if (in_prefix == HPS::UTF8("Length"))
		segment_name << in_prefix << "." << length_measurement_index++;

	return HPS::UTF8(segment_name.str().data());
}
