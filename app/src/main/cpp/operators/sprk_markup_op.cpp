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

const HPS::RGBColor HPS::MarkupOperator::default_color = HPS::RGBColor(1, 0, 0);
const HPS::TextAttributeKit HPS::MarkupOperator::default_text_attributes = HPS::TextAttributeKit::GetDefault();
const HPS::LineAttributeKit HPS::MarkupOperator::default_line_attributes = HPS::LineAttributeKit::GetDefault();

HPS::MarkupOperator::MarkupOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger) 
	: Operator(in_mouse_trigger, in_modifier_trigger)
	, mouse_trigger(in_mouse_trigger)
	, modifier_trigger(in_modifier_trigger)
	, markup_type(MarkupOperator::MarkupType::Freehand)
	, start_new_line(false)
	, current_line_size(0)
	, start_new_note(true)
    , keyboard_active(false)
	, current_text_row(0)
	, operator_active(false)
	, tracked_touch_id(-1)
    , second_tracked_touch_id(-1)
{
	current_attributes.color = default_color;
	current_attributes.text_attributes = default_text_attributes;
	current_attributes.line_attributes = default_line_attributes;
    
    HPS::Database::GetEventDispatcher().Subscribe(handler, ClassID<HPS::HideKeyboardEvent>());
    handler.SetOperator(this);
}

HPS::MarkupOperator::MarkupInsertedEvent::~MarkupInsertedEvent()
{
}

void HPS::MarkupOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	last_attached_view = in_attached_view;

	SetupConstructionSegments();
	current_segment = markup_segment.Subsegment();
	current_segment.GetMaterialMappingControl().SetLineColor(current_attributes.color).SetTextColor(current_attributes.color).SetEdgeColor(current_attributes.color);
	current_segment.SetTextAttribute(current_attributes.text_attributes);
	current_segment.SetLineAttribute(current_attributes.line_attributes);

	attribute_map.insert(std::make_pair(current_attributes.color, current_segment));
}

void HPS::MarkupOperator::OnViewDetached(HPS::View const &)
{
	EndTextNote();
}

bool MarkupOperator::SetupConstructionSegments()
{
	if (!last_attached_view.Empty())
	{
		markup_segment = last_attached_view.GetSegmentKey().Subsegment("construction segments").Subsegment("markup");
		CameraKit camera = CameraKit::GetDefault();
		markup_segment.SetCamera(camera);
		markup_segment.GetDrawingAttributeControl().SetDepthRange(0, 0.01f).SetOverlay(Drawing::Overlay::Default).SetWorldHandedness(Drawing::Handedness::Left);
        markup_segment.GetVisibilityControl().SetText(true).SetLines(true).SetEdges(true).SetFaces(false).SetCuttingSections(false).SetCutGeometry(false);
		markup_segment.GetAttributeLockControl().SetLock(AttributeLock::Type::Visibility, true);

		return true;
	}

	return false;
}

void MarkupOperator::CreateNewMarkupSegment()
{
	current_segment = markup_segment.Subsegment();
	current_segment.GetMaterialMappingControl().SetLineColor(current_attributes.color).SetTextColor(current_attributes.color).SetEdgeColor(current_attributes.color);
	current_segment.SetLineAttribute(current_attributes.line_attributes);
	current_segment.SetTextAttribute(current_attributes.text_attributes);
	attribute_map.insert(std::make_pair(current_attributes.color, current_segment));
}

void MarkupOperator::LookupSegment()
{
	auto range = attribute_map.equal_range(current_attributes.color);
	if (range.first == range.second)
	{
		//no segment matches the required attributes
		CreateNewMarkupSegment();
	}
	else
	{
		//check all segments in range to see if they match the attributes
		for (auto it = range.first, e = range.second; it != e; ++it)
		{
			LineAttributeKit line_attributes;
			TextAttributeKit text_attributes;
			if (it->second.ShowLineAttribute(line_attributes) && 
				it->second.ShowTextAttribute(text_attributes) &&
				line_attributes == current_attributes.line_attributes &&
				text_attributes == current_attributes.text_attributes)
			{
				current_segment = it->second;
				return;
			}
		}

		//if we get here, we have not found a segment with matching attributes
		CreateNewMarkupSegment();
	}
}

bool HPS::MarkupOperator::OnMouseDown(MouseState const & in_state)
{
	if (in_state.GetActiveEvent().CurrentButton == mouse_trigger && in_state.GetActiveEvent().GetModifierKeys() == modifier_trigger)
	{
		HPS::KeyPath event_path(in_state.GetEventPath());
		event_path = markup_segment + event_path;

		HPS::Point world_point;
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::World, world_point);

		start_point = world_point;
		start_new_line = true;

		operator_active = true;

		current_line = HPS::LineKey();
		current_line_size = 0;
		current_rectangle = HPS::LineKey();
		current_circle = HPS::CircleKey();
		current_circle_line = HPS::LineKey();

		if (markup_type == MarkupType::Text && start_new_note)
		{
			start_new_note = false;
			LookupSegment();
			default_text = current_segment.InsertText(world_point, "Type your note. \nClick elsewhere when you are done");
			last_attached_view.Update();
		}
		else if (markup_type == MarkupType::Text)
			EndTextNote();


		return true;
	}

	return false;
}

bool HPS::MarkupOperator::OnMouseUp(MouseState const & in_state)
{
	if (!IsMouseTriggered(in_state) && markup_type != MarkupType::Text)
	{
		if (operator_active)
		{
			if (markup_type == MarkupType::Circle && !current_circle_line.Empty())
			{
				MarkupInsertedEvent event(current_circle_line, last_attached_view); 
				HPS::Database::GetEventDispatcher().InjectEvent(event); 
			}
			else if (markup_type == MarkupType::Rectangle && !current_rectangle.Empty())
			{
				MarkupInsertedEvent event(current_rectangle, last_attached_view);
				HPS::Database::GetEventDispatcher().InjectEvent(event);
			}
			else if (markup_type == MarkupType::Freehand && !current_line.Empty())
			{
				auto name = current_line.Owner().Name();
				MarkupInsertedEvent event(current_line, last_attached_view);
				HPS::Database::GetEventDispatcher().InjectEvent(event);
			}	
		}
		operator_active = false;
		return true;
	}

	return false;
}

bool HPS::MarkupOperator::OnMouseMove(HPS::MouseState const & in_state)
{
	if (IsMouseTriggered(in_state) && operator_active && markup_type != MarkupType::Text)
	{
		HPS::KeyPath event_path(in_state.GetEventPath());
		event_path = markup_segment + event_path;
		HPS::Point world_point;
		event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_state.GetLocation(), HPS::Coordinate::Space::World, world_point);

		if (markup_type == MarkupType::Freehand)
			DrawFreehand(world_point);
		else if (markup_type == MarkupType::Circle)
			DrawCircle(world_point);
		else if (markup_type == MarkupType::Rectangle)
			DrawRectangle(world_point);

		last_attached_view.Update();

		return true;
	}

	return false;
}

bool HPS::MarkupOperator::OnTouchDown(const HPS::TouchState &in_state)
{
    if (tracked_touch_id != -1 || keyboard_active)
		return false;

	HPS::TouchArray touches = in_state.GetActiveEvent().Touches;
    
    if (touches.size() == 2 &&
        (markup_type == MarkupType::Circle || markup_type == MarkupType::Rectangle))
    {
        HPS::KeyPath event_path(in_state.GetEventPath());
        event_path = markup_segment + event_path;
        HPS::Point world_point_one, world_point_two;
        event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, world_point_one);
        event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[1].Location, HPS::Coordinate::Space::World, world_point_two);
        
        start_new_line = true;
        operator_active = true;
        
        if (markup_type == MarkupType::Circle)
            DrawCircleFromTwoPoints(world_point_one, world_point_two);
        else if (markup_type == MarkupType::Rectangle)
            DrawRectangleFromTwoPoints(world_point_one, world_point_two);
        
        tracked_touch_id = touches[0].ID;
        second_tracked_touch_id = touches[1].ID;
        
        last_attached_view.Update();
    }
    else
    {
        if (touches.size() != 1)
            return false;
        if (touches[0].TapCount != 1)
            return false;

        HPS::KeyPath event_path(in_state.GetEventPath());
        event_path = markup_segment + event_path;
        HPS::Point world_point;
        event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, world_point);
        
        if (markup_type == MarkupType::Text)
        {
            HPS::Point main_camera_center;
            event_path = in_state.GetEventPath();
            event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, main_camera_center);
            
            CenterCameras(main_camera_center, world_point);
            HPS::ShowKeyboardEvent show_keyboard;
            HPS::Database::GetEventDispatcher().InjectEventWithNotifier(show_keyboard).Wait();
            keyboard_active = true;
        
            start_new_note = true;
            LookupSegment();
            default_text = current_segment.InsertText(world_point, "Type your note. \nHide the keyboard when you are done");
            last_attached_view.Update();
        }

        start_point = world_point;
        start_new_line = true;
        operator_active = true;
        tracked_touch_id = touches[0].ID;
    }

    return false;
}

bool HPS::MarkupOperator::OnTouchMove(const HPS::TouchState &in_state)
{
	if (tracked_touch_id == -1 || !operator_active)
		return false;

	TouchArray touches = in_state.GetTouches();
    
    if (touches.size() == 2 &&
        (markup_type == MarkupType::Circle || markup_type == MarkupType::Rectangle))
    {
        if ((touches[0].ID == tracked_touch_id && touches[1].ID == second_tracked_touch_id) ||
            (touches[0].ID == second_tracked_touch_id && touches[1].ID == tracked_touch_id))
        {
            HPS::KeyPath event_path(in_state.GetEventPath());
            event_path = markup_segment + event_path;
            HPS::Point world_point_one, world_point_two;
            event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, world_point_one);
            event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[1].Location, HPS::Coordinate::Space::World, world_point_two);
        
            if (markup_type == MarkupType::Circle)
                DrawCircleFromTwoPoints(world_point_one, world_point_two);
            else if (markup_type == MarkupType::Rectangle)
                DrawRectangleFromTwoPoints(world_point_one, world_point_two);
        }
    }
    else
    {
        if (touches.size() != 1)
            return false;
        if (touches[0].TapCount != 1 || touches[0].ID != tracked_touch_id)
            return false;

        HPS::KeyPath event_path(in_state.GetEventPath());
        event_path = markup_segment + event_path;
        HPS::Point world_point;
        event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, touches[0].Location, HPS::Coordinate::Space::World, world_point);

        if (markup_type == MarkupType::Freehand)
            DrawFreehand(world_point);
        else if (markup_type == MarkupType::Circle)
            DrawCircle(world_point);
        else if (markup_type == MarkupType::Rectangle)
            DrawRectangle(world_point);
    }

	last_attached_view.Update();
	return true;
}

bool HPS::MarkupOperator::OnTouchUp(const HPS::TouchState &in_state)
{
	TouchArray up_touches = in_state.GetActiveEvent().Touches;
	if (up_touches.empty())
		return false;

	for(size_t i = 0, e = up_touches.size(); i < e; ++i)
	{
		if(up_touches[i].ID == tracked_touch_id)
			tracked_touch_id = -1;
        else if (up_touches[i].ID == second_tracked_touch_id)
            second_tracked_touch_id = -1;
	}

	if (operator_active)
	{
		if (markup_type == MarkupType::Circle && !current_circle_line.Empty())
		{
			MarkupInsertedEvent event(current_circle_line, last_attached_view);
			HPS::Database::GetEventDispatcher().InjectEvent(event);
		}
		else if (markup_type == MarkupType::Rectangle && !current_rectangle.Empty())
		{
			MarkupInsertedEvent event(current_rectangle, last_attached_view);
			HPS::Database::GetEventDispatcher().InjectEvent(event);
		}
		else if (markup_type == MarkupType::Freehand && !current_line.Empty())
		{
			MarkupInsertedEvent event(current_line, last_attached_view);
			HPS::Database::GetEventDispatcher().InjectEvent(event);
		}	
	}
    
    if (markup_type != MarkupType::Text)
        operator_active = false;

	return true;
}

bool HPS::MarkupOperator::OnKeyDown(KeyboardState const & in_state)
{
	if (operator_active)
	{
		KeyboardCodeArray keyboard_codes = in_state.GetActiveEvent().KeyboardCodes;
		if (std::find(keyboard_codes.begin(), keyboard_codes.end(), KeyboardCode::Escape) != keyboard_codes.end())
		{
			if (markup_type == MarkupType::Text)
			{
				current_text.Delete();
				EndTextNote();
			}
			else if(markup_type == MarkupType::Freehand)
				current_line.Delete();
			else if(markup_type == MarkupType::Circle)
				current_circle_line.Delete();
			else if(markup_type == MarkupType::Rectangle)
				current_rectangle.Delete();
			last_attached_view.Update();
			operator_active = false;
			return true;
		}

		if (markup_type == MarkupType::Text)
		{
			if (!default_text.Empty())
				default_text.Delete();

			for (auto const & code : keyboard_codes)
			{
				//Do not print characters that do not represent a letter
				if (code == KeyboardCode::None
					|| code == KeyboardCode::Shift
					|| code == KeyboardCode::LeftShift
					|| code == KeyboardCode::RightShift
					|| code == KeyboardCode::Alt
					|| code == KeyboardCode::LeftAlt
					|| code == KeyboardCode::RightAlt
					|| code == KeyboardCode::Control
					|| code == KeyboardCode::LeftCtrl
					|| code == KeyboardCode::RightCtrl
					|| code >= KeyboardCode::Insert)
					continue;

				HPS::UTF8 utf8_code;
				if (static_cast<int>(code) < 255)
				{
					char * char_code = new char[128];
					strcpy(char_code, (char *)(&code));
					utf8_code = HPS::UTF8(char_code);
					delete [] char_code;
				}
				else
					return false;
				
				if (code == KeyboardCode::Return)
				{
					if (!current_text.Empty())
					{
						if (text_columns.empty())
							text_columns.push_back(0);

						current_text = current_text.EditTextByInsertion(current_text_row, text_columns.back()++, 1, utf8_code);
						current_text_row++;
						text_columns.push_back(0);
						last_attached_view.Update();
					}
				}
#ifdef __APPLE__
				else if (code == KeyboardCode::Delete && !current_text.Empty())
#else
				else if (code == KeyboardCode::Backspace && !current_text.Empty())
#endif
				{
					if (current_text_row == 0 
						&& ((text_columns.size() == 1 && text_columns.back() == 0) 
						|| text_columns.empty()))
						return true;

					if (text_columns.back() == 0)
						current_text = current_text.EditTextByDeletion(current_text_row - 1, text_columns[text_columns.size() - 2] - 1, 1);
					else
						current_text = current_text.EditTextByDeletion(current_text_row, text_columns.back() - 1, 1);

					if (text_columns.back() == 0)
					{
						text_columns.pop_back();
						if (current_text_row > 0)
							current_text_row--;
					}

					text_columns.back()--;
					if (text_columns.back() == 0 && text_columns.size() > 1)
					{
						text_columns.pop_back();
						current_text_row--;
					}

					last_attached_view.Update();
				}
				else
				{
					if (current_text.Empty())
					{
						LookupSegment();
						current_text = current_segment.InsertText(start_point, utf8_code);
						text_columns.push_back(1);
					}
					else
					{
						if (text_columns.empty())
							text_columns.push_back(0);
						current_text = current_text.EditTextByInsertion(current_text_row, text_columns.back()++, 1, utf8_code);
					}
					last_attached_view.Update();
				}
			}
			return true;
		}
	}

	return false;
}

bool HPS::MarkupOperator::OnTextInput(const HPS::UTF8 & in_text)
{
    if (operator_active && markup_type == MarkupType::Text && keyboard_active)
    {
        if (!default_text.Empty())
            default_text.Delete();
            
        if (current_text.Empty() || start_new_note)
        {
            start_new_note = false;
            LookupSegment();
            current_text = current_segment.InsertText(start_point, in_text);
        }
        else
        {
            current_text.Delete();
            current_text = current_segment.InsertText(start_point, in_text);
        }
        
        last_attached_view.Update();

        return true;
    }
    return false;
}

void HPS::MarkupOperator::DrawFreehand(Point const & location)
{
	if (start_new_line)
	{
		LookupSegment();
		current_line = current_segment.InsertLine(start_point, location);
		start_new_line = false;
		start_point = location;
		current_line_size = 2;
	}
	else if (!current_line.Empty())
		current_line = current_line.EditPointsByInsertion(current_line_size++, 1, &location);
	else
	{
		//current_line got deleted someone, probably by the user. Reset the line
		start_new_line = true;
		current_line_size = 0;
	}
}

void HPS::MarkupOperator::DrawCircle(Point const & location)
{
	if (start_new_line)
		LookupSegment();
	else
	{
		current_circle.Delete();
		current_circle_line.Delete();
	}

	start_new_line = false;
	HPS::Vector distance(location - start_point);
	float radius = (float)distance.Length();

	current_circle = current_segment.InsertCircle(start_point, radius, HPS::Vector(0, 0, 1));
	current_circle_line = current_segment.InsertLineFromGeometry(current_circle);
	current_circle.Delete();
}

void HPS::MarkupOperator::DrawCircleFromTwoPoints(HPS::Point const & point_one, HPS::Point const & point_two)
{
    HPS::Point center = HPS::Midpoint(point_one, point_two);
    
    HPS::Vector distance(point_two - point_one);
    float radius = (float)distance.Length() / 2.0f;
    
    if (start_new_line)
        LookupSegment();
	else
	{
		current_circle.Delete();
		current_circle_line.Delete();
	}
    
    current_circle = current_segment.InsertCircle(center, radius, HPS::Vector(0, 0, 1));
	current_circle_line = current_segment.InsertLineFromGeometry(current_circle);
	current_circle.Delete();

    start_new_line = false;
}

void HPS::MarkupOperator::DrawRectangle(Point const & location)
{
	if (start_new_line)
		LookupSegment();
	else
		current_rectangle.Delete();

	start_new_line = false;

	HPS::PointArray points;
	points.push_back(HPS::Point(start_point.x, start_point.y, 0));
	points.push_back(HPS::Point(start_point.x, location.y, 0));
	points.push_back(HPS::Point(location.x, location.y, 0));
	points.push_back(HPS::Point(location.x, start_point.y, 0));
	points.push_back(HPS::Point(start_point.x, start_point.y, 0));
	current_rectangle = current_segment.InsertLine(points);
}

void HPS::MarkupOperator::DrawRectangleFromTwoPoints(Point const & point_one, Point const & point_two)
{
    if (start_new_line)
        LookupSegment();
    else if (!current_rectangle.Empty())
        current_rectangle.Delete();
    
    HPS::PointArray points;
    points.push_back(HPS::Point(point_one.x, point_one.y, 0));
    points.push_back(HPS::Point(point_one.x, point_two.y, 0));
    points.push_back(HPS::Point(point_two.x, point_two.y, 0));
    points.push_back(HPS::Point(point_two.x, point_one.y, 0));
    points.push_back(HPS::Point(point_one.x, point_one.y, 0));
    current_rectangle = current_segment.InsertLine(points);
    
    start_new_line = false;
}

void HPS::MarkupOperator::CenterCameras(Point const & main_camera_center, Point const & markup_camera_center)
{
    last_attached_view.GetSegmentKey().ShowCamera(original_camera);
    markup_segment.ShowCamera(original_markup_camera);
    
    CameraKit centered_camera;
    centered_camera.Set(original_camera);
    centered_camera.SetTarget(main_camera_center);
    
    Point original_target;
    original_camera.ShowTarget(original_target);
    
    Vector camera_movement(main_camera_center - original_target);
    Point original_position;
    original_camera.ShowPosition(original_position);
    
    MatrixKit translation;
    translation.Translate(camera_movement.x, camera_movement.y, camera_movement.z);
    centered_camera.SetPosition(translation.Transform(original_position));
    
    last_attached_view.SmoothTransition(centered_camera, 0.2);
    
    
    
    centered_camera.Set(original_markup_camera);
    centered_camera.SetTarget(markup_camera_center);
    original_markup_camera.ShowTarget(original_target);
    Vector markup_camera_movement(markup_camera_center - original_target);
    original_markup_camera.ShowPosition(original_position);
    MatrixKit markup_translation;
    markup_translation.Translate(markup_camera_movement.x, markup_camera_movement.y, markup_camera_movement.z);
    centered_camera.SetPosition(markup_translation.Transform(original_position));
    markup_segment.SetCamera(centered_camera);
}

void HPS::MarkupOperator::ResetCameras()
{
    markup_segment.SetCamera(original_markup_camera);
    last_attached_view.SmoothTransition(original_camera, 0.2);
}

void HPS::MarkupOperator::DeleteMarkups()
{
	attribute_map.clear();

	if (!markup_segment.Empty())
		markup_segment.Flush();
	OnViewAttached(last_attached_view);
}

void HPS::MarkupOperator::EndTextNote()
{
	if (operator_active && !current_text.Empty())
	{
		MarkupInsertedEvent event(current_text, last_attached_view);
		HPS::Database::GetEventDispatcher().InjectEvent(event);
	}

	start_new_note = true;
	text_columns.clear();
	current_text_row = 0;
	operator_active = false;
	current_text = HPS::TextKey();
	default_text.Delete();
}