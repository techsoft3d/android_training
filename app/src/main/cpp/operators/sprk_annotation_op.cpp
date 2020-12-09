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

HPS::AnnotationOperator::AnnotationOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger) 
	: Operator(in_mouse_trigger, in_modifier_trigger)
	, editing_text(false)
	, start_new_note(true)
	, annotation_user_data("hps_annotation")
	, annotation_shape("box")
	, current_text_row(0)
    , tracked_touch_id(-1)
    , keyboard_active(false)
    , always_show_keyboard_on_touch_up(false)
{
	selection_options.SetLevel(Selection::Level::Entity)
		.SetRelatedLimit(0)
		.SetAlgorithm(Selection::Algorithm::Analytic);

	annotation_selection_options.Set(selection_options);

	background_color = RGBColor(1, 1, 1);
	leader_line_color = RGBColor(1, 0, 0);
	highlight_color = RGBColor(1, 1, 0);
    
    HPS::Database::GetEventDispatcher().Subscribe(handler, ClassID<HPS::HideKeyboardEvent>());
    handler.SetOperator(this);
}

HPS::AnnotationOperator::AnnotationInsertedEvent::~AnnotationInsertedEvent()
{}

void HPS::AnnotationOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	last_attached_view = in_attached_view;
	annotation_segment = last_attached_view.GetSegmentKey().Subsegment("construction segments").Subsegment("annotations");
	annotation_segment.GetVisibilityControl().SetLeaderLines(true).SetEdges(true).SetText(true).SetFaces(true);
	annotation_segment.GetTextAttributeControl().SetBackground(true);
	annotation_segment.GetMaterialMappingControl().SetFaceColor(background_color).SetEdgeColor(leader_line_color).SetLineColor(leader_line_color);
	annotation_segment.GetDrawingAttributeControl().SetOverlay(Drawing::Overlay::InPlace);

	current_portfolio = last_attached_view.GetPortfolioKey();

	highlight_style_segment = Database::CreateRootSegment();
	highlight_style_segment.GetMaterialMappingControl().SetEdgeColor(highlight_color).SetLineColor(highlight_color);
	current_portfolio.DefineNamedStyle("annotation_highlight_style", highlight_style_segment);
}

void HPS::AnnotationOperator::OnViewDetached(HPS::View const &)
{
	current_portfolio.UndefineNamedStyle("annotation_highlight_style");
	highlight_style_segment.Delete();
}

bool HPS::AnnotationOperator::OnMouseDown(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state))
    {
        KeyPath path(in_state.GetEventPath());
        return InputDown(in_state.GetEventSource(), in_state.GetLocation(), path);
    }

	return false;
}

bool HPS::AnnotationOperator::OnMouseMove(MouseState const & in_state)
{
	if(IsMouseTriggered(in_state) && editing_text)
    {
        KeyPath path(in_state.GetEventPath());
        return InputMove(in_state.GetLocation(), path);
    }

	return false;
}

bool HPS::AnnotationOperator::OnTouchDown(TouchState const & in_state)
{
    if (tracked_touch_id != -1)
        return false;
    
    HPS::TouchArray touches = in_state.GetActiveEvent().Touches;
    if (touches.size() != 1 || touches[0].TapCount != 1)
        return false;
    
    touch_down_position = touches[0].Location;
    
    KeyPath path(in_state.GetEventPath());
    bool track_touch = InputDown(in_state.GetEventSource(), touches[0].Location, path);
    
    if (track_touch)
        tracked_touch_id = touches[0].ID;
    
    return track_touch;
}

bool HPS::AnnotationOperator::OnTouchMove(TouchState const & in_state)
{
    if (tracked_touch_id == -1 || !editing_text)
        return false;
    
    TouchArray touches = in_state.GetTouches();
    if (touches.size() != 1 || touches[0].TapCount != 1 || touches[0].ID != tracked_touch_id)
        return false;
    
    KeyPath path(in_state.GetEventPath());
    return InputMove(touches[0].Location, path);
    
	return true;
}

bool HPS::AnnotationOperator::OnTouchUp(TouchState const & in_state)
{
    TouchArray up_touches = in_state.GetActiveEvent().Touches;
    if (up_touches.empty())
        return false;
    
    if (tracked_touch_id == -1 || !editing_text)
        return false;
    
    if (up_touches.size() != 1 || up_touches[0].TapCount != 1 || up_touches[0].ID != tracked_touch_id)
        return false;
    
    if (always_show_keyboard_on_touch_up || up_touches[0].Location == touch_down_position)
    {
        Point main_camera_center;
        KeyPath event_path = in_state.GetEventPath();
        event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, up_touches[0].Location, HPS::Coordinate::Space::World, main_camera_center);
    
        CenterCamera(main_camera_center);
        HPS::ShowKeyboardEvent show_keyboard;
        HPS::Database::GetEventDispatcher().InjectEventWithNotifier(show_keyboard).Wait();
        keyboard_active = true;
        always_show_keyboard_on_touch_up = false;
    }
    
    tracked_touch_id = -1;

    return false;
}

bool HPS::AnnotationOperator::InputDown(HPS::WindowKey const & window, HPS::Point const & location, HPS::KeyPath & path)
{
    Model attached_model = last_attached_view.GetAttachedModel();
    KeyPath scoped_path;
    try
    {
        scoped_path = attached_model.GetSegmentKey()
        + last_attached_view.GetAttachedModelIncludeLink()
        + last_attached_view.GetModelOverrideSegmentKey()
        + path;
    }
    catch (InvalidObjectException const &)
    {
        return false;
    }
    
    //check if we have selected a previously inserted annotation
    SelectionResults results;
    if (window.GetSelectionControl().SelectByPoint(location, annotation_selection_options, results) > 0)
    {
        SelectionResultsIterator it = results.GetIterator();
        Key selected_key;
        it.GetItem().ShowSelectedItem(selected_key);
        
        try
        {
            GeometryKey selected_geometry(selected_key);
            ByteArray data;
            if (selected_geometry.ShowUserData(0, data)
                && strcmp((const char *)data.data(), annotation_user_data) == 0)
            {
                StartNewNote();
                
                text_to_edit = TextKey(selected_key);
                editing_text = true;
                text_to_edit.Owner().GetTextAttributeControl().SetBackgroundStyle("annotation_highlight_style");
                last_attached_view.Update();
                
                return true;
            }
        }
        catch (InvalidObjectException const &)
        { }
    }
    
    //check to see if we have selected geometry
    //this time scope the selection to everything under the model segment to avoid selecting navigation aids
    selection_options.SetScope(scoped_path);
    if (window.GetSelectionControl().SelectByPoint(location, selection_options, results) > 0)
    {
        SelectionResultsIterator it = results.GetIterator();
        Key selected_key;
        it.GetItem().ShowSelectedItem(selected_key);
        
        StartNewNote();
        
        //insert new annotation
        SegmentKey new_annotation = annotation_segment.Subsegment();
        always_show_keyboard_on_touch_up = true;
        
        editing_text = true;
        
        WorldPoint selection_position;
        it.GetItem().ShowSelectionPosition(selection_position);
        text_to_edit = new_annotation.InsertText(selection_position, "Note");
        new_annotation.GetTextAttributeControl().SetBackground(annotation_shape);
        text_to_edit.SetLeaderLine(selection_position, Text::LeaderLineSpace::World);
        AddAnnotationUserData();
        new_annotation.GetTextAttributeControl().SetBackgroundStyle("annotation_highlight_style");

		AnnotationInsertedEvent event(new_annotation, last_attached_view); 
		HPS::Database::GetEventDispatcher().InjectEvent(event); 
        
        last_attached_view.Update();
        
        return true;
    }
    else
    {
        StartNewNote();
        editing_text = false;
        text_to_edit = TextKey();
        last_attached_view.Update();
        
        if (keyboard_active)
        {
            HideKeyboardEvent hide_keyboard;
            Database::GetEventDispatcher().InjectEvent(hide_keyboard);
        }
        
        return false;
    }
    
    return false;
}

bool HPS::AnnotationOperator::InputMove(HPS::Point const & location, HPS::KeyPath & path)
{
    SegmentKey owner_segment = text_to_edit.Owner();
    
    UTF8 text_string;
    text_to_edit.ShowText(text_string);
    
    PointArray leader_line_positions;
    Text::LeaderLineSpace leader_line_space;
    text_to_edit.ShowLeaderLines(leader_line_positions, leader_line_space);
    
    Point world_location;
    path.ConvertCoordinate(Coordinate::Space::Window, location, Coordinate::Space::World, world_location);
    
    text_to_edit.Delete();
    
    text_to_edit = owner_segment.InsertText(world_location, text_string);
    text_to_edit.SetLeaderLines(leader_line_positions, leader_line_space);
    AddAnnotationUserData();
    
    last_attached_view.Update();
    return true;
}

bool HPS::AnnotationOperator::OnKeyDown(KeyboardState const & in_state)
{
	if (editing_text)
	{
		UTF8 text_string;
		text_to_edit.ShowText(text_string);

		PointArray leader_line_positions;
		Text::LeaderLineSpace leader_line_space;
		text_to_edit.ShowLeaderLines(leader_line_positions, leader_line_space);

		Point text_position;
		text_to_edit.ShowPosition(text_position);

		if (start_new_note)
		{
			SegmentKey owner = text_to_edit.Owner();
			text_to_edit.Delete();
			text_to_edit = owner.InsertText(text_position, "");
			text_to_edit.SetLeaderLines(leader_line_positions, leader_line_space);
			AddAnnotationUserData();
			start_new_note = false;
		}

		KeyboardCodeArray keyboard_codes = in_state.GetActiveEvent().KeyboardCodes;

		for (auto const & code : keyboard_codes)
		{
			if (code == KeyboardCode::None)
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
				if (text_columns.empty())
					text_columns.push_back(0);

				text_to_edit = text_to_edit.EditTextByInsertion(current_text_row, text_columns.back()++, 1, utf8_code);
				current_text_row++;
				text_columns.push_back(0);
				last_attached_view.Update();
			}
#ifdef __APPLE__
			else if (code == KeyboardCode::Delete && !text_to_edit.Empty())
#else
			else if (code == KeyboardCode::Backspace && !text_to_edit.Empty())
#endif
			{
				if (current_text_row == 0 
					&& ((text_columns.size() == 1 && text_columns.back() == 0) 
					|| text_columns.empty()))
					return true;

				if (text_columns.back() == 0)
					text_to_edit = text_to_edit.EditTextByDeletion(current_text_row - 1, text_columns[text_columns.size() - 2] - 1, 1);
				else
					text_to_edit = text_to_edit.EditTextByDeletion(current_text_row, text_columns.back() - 1, 1);

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
				if (text_to_edit.Empty())
				{
					text_to_edit = text_to_edit.Owner().InsertText(text_position, utf8_code);
					AddAnnotationUserData();
					text_to_edit.SetLeaderLines(leader_line_positions, leader_line_space);
					text_columns.push_back(1);
				}
				else
				{
					if (text_columns.empty())
						text_columns.push_back(0);
					text_to_edit = text_to_edit.EditTextByInsertion(current_text_row, text_columns.back()++, 1, utf8_code);
				}
			}
		}
		last_attached_view.Update();
		return true;
	}

	return false;
}

bool HPS::AnnotationOperator::OnTextInput(HPS::UTF8 const & in_text)
{
    if (editing_text)
    {
        UTF8 text_string;
        text_to_edit.ShowText(text_string);
        
        PointArray leader_line_positions;
        Text::LeaderLineSpace leader_line_space;
        text_to_edit.ShowLeaderLines(leader_line_positions, leader_line_space);
        
        Point text_position;
        text_to_edit.ShowPosition(text_position);
        
        SegmentKey owner = text_to_edit.Owner();
        text_to_edit.Delete();
        text_to_edit = owner.InsertText(text_position, in_text);
        text_to_edit.SetLeaderLines(leader_line_positions, leader_line_space);
        AddAnnotationUserData();
        
        last_attached_view.Update();
        return true;
    }

	return false;
}

void HPS::AnnotationOperator::AddAnnotationUserData()
{
	ByteArray data;
	size_t len = strlen(annotation_user_data);
	for (size_t i = 0; i < len; ++i) 
		data.push_back(annotation_user_data[i]);
	data.push_back('\0');

	text_to_edit.SetUserData(0, data);
}

void HPS::AnnotationOperator::StartNewNote()
{
	if (editing_text)
	{
		text_to_edit.Owner().GetTextAttributeControl().UnsetBackgroundStyle();
		start_new_note = true;
		text_columns.clear();
		current_text_row = 0;
	}
}

void HPS::AnnotationOperator::SetBackgroundShape(const char * in_shape)
{
	annotation_shape = UTF8(in_shape);
}

HPS::UTF8 HPS::AnnotationOperator::GetBackgroundShape() const
{
	return annotation_shape;
}

void HPS::AnnotationOperator::CenterCamera(Point const & camera_center)
{
    last_attached_view.GetSegmentKey().ShowCamera(original_camera);
    
    CameraKit centered_camera;
    centered_camera.Set(original_camera);
    centered_camera.SetTarget(camera_center);
    
    Point original_target;
    original_camera.ShowTarget(original_target);
    
    Vector camera_movement(camera_center - original_target);
    Point original_position;
    original_camera.ShowPosition(original_position);
    
    MatrixKit translation;
    translation.Translate(camera_movement.x, camera_movement.y, camera_movement.z);
    centered_camera.SetPosition(translation.Transform(original_position));
    
    last_attached_view.SmoothTransition(centered_camera, 0.2);
}

void HPS::AnnotationOperator::ResetCamera()
{
    last_attached_view.SmoothTransition(original_camera, 0.2);
}