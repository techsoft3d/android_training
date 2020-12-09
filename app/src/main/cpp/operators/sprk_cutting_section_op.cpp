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

HPS::CuttingSectionOperator::CuttingSectionOperator(MouseButtons in_mouse_trigger, ModifierKeys in_modifier_trigger)
	: SelectOperator(in_mouse_trigger, in_modifier_trigger)
	, sectioning(false)
	, tracked_touch_ID(-1)
	, op_state(OpState::Uninitialized)
	, indicator_scale(0.05f)
	, is_plane_highlighted(false)
	, enable_mouse_over_highlighting(true)
	, plane_normal_valid(false)
	, skip_mouse_overs(false)
{
	op_state = OpState::Uninitialized;
	selection_options.SetRelatedLimit(0).SetInternalLimit(1).SetLevel(HPS::Selection::Level::Subentity).SetProximity(0);
	mouse_over_selection_options.SetAlgorithm(HPS::Selection::Algorithm::Analytic).SetInternalLimit(0).SetRelatedLimit(0).SetLevel(HPS::Selection::Level::Entity);
	plane_material.SetFaceColor(HPS::RGBAColor(0.65f, 0.65f, 0.65f, 0.35f)).SetEdgeColor(HPS::RGBColor(0, 0, 0));
}

void HPS::CuttingSectionOperator::OnViewAttached(HPS::View const & in_attached_view)
{
	attached_view = in_attached_view;
	SetupOperatorSegment();

	HPS::Model model = attached_view.GetAttachedModel();
	HPS::SimpleSphere dummy;
	if (model.Type() != HPS::Type::None)
		model.GetSegmentKey().GetBoundingControl().ShowVolume(dummy, model_bounding);

	portfolio = in_attached_view.GetPortfolioKey();
	style_segment = HPS::Database::CreateRootSegment();
	HPS::NamedStyleDefinition def = portfolio.DefineNamedStyle("hps_cutting_section_highlight_style", style_segment);

	def.GetSource().GetMaterialMappingControl().SetEdgeColor(HPS::RGBColor(1, 1, 0));
	highlight_options.SetStyleName("hps_cutting_section_highlight_style")
		.SetOverlay(HPS::Drawing::Overlay::InPlace)
		.SetNotification(false);

	navigation_keys = attached_view.GetNavigationKeys();
}

void HPS::CuttingSectionOperator::OnViewDetached(HPS::View const & in_detached_view)
{
	if (is_plane_highlighted)
	{
		try 
		{
			HPS::LayoutArray layouts = in_detached_view.GetOwningLayouts();
			HPS::CanvasArray canvases;
			for (auto const & layout : layouts)
			{
				HPS::CanvasArray some_canvases = layout.GetOwningCanvases();
				canvases.insert(canvases.end(), some_canvases.begin(), some_canvases.end());
			}

			for (auto & canvas : canvases)
				canvas.GetWindowKey().GetHighlightControl().Unhighlight(highlight_options);
		
			in_detached_view.Update();
		}
		catch (HPS::InvalidObjectException const &)
		{}
	}

	operator_root_segment.Delete();
	sections.clear();

	style_segment.Delete();
	portfolio.UndefineNamedStyle("hps_cutting_section_highlight_style");
}

void HPS::CuttingSectionOperator::OnModelAttached()
{
	HPS::SimpleSphere dummy;
	GetAttachedView().GetAttachedModel().GetSegmentKey().GetBoundingControl().ShowVolume(dummy, model_bounding);
}

bool HPS::CuttingSectionOperator::OnMouseDown(MouseState const  & in_state)
{
	if(!IsMouseTriggered(in_state))
		return false;

    return HandleMouseAndTouchDown(in_state.GetEventSource(), in_state.GetActiveEvent().ClickCount, in_state.GetEventPath(), in_state.GetLocation());
}


bool HPS::CuttingSectionOperator::OnMouseUp(MouseState const & in_state)
{
	if(!IsMouseTriggered(in_state) && op_state != OpState::Uninitialized)
	{
		if (!sections.empty())
		{
			bool cut_geometry_visibility;
			HPS::VisibilityControl visibility_control = GetAttachedView().GetModelOverrideSegmentKey().GetVisibilityControl();
			if (visibility_control.ShowCutFaces(cut_geometry_visibility) && !cut_geometry_visibility)
			{
				visibility_control.SetCutGeometry(true);
				op_state = OpState::Initialized;
				indicator_seg.Flush(Search::Type::Geometry);
				GetAttachedView().Update();
			}
		}
		
		skip_mouse_overs = false;
	}

	return false;
}


bool HPS::CuttingSectionOperator::OnMouseMove(MouseState const  & in_state)
{
	if (skip_mouse_overs)
		return false;

	if (enable_mouse_over_highlighting)
		MouseOverHighlighting(in_state);

	switch(op_state)
	{
	case OpState::Uninitialized:
	case OpState::FacePicking:
		{
			HPS::SelectionResults new_selection;
			HPS::WindowKey window = in_state.GetEventSource();
			
			if(window.GetSelectionControl().SelectByPoint(in_state.GetLocation(), selection_options, new_selection))
			{
				//update normal indicator geometry
				SelectionItem item = new_selection.GetIterator().GetItem();
				HPS::Key sel_key;
				item.ShowSelectedItem(sel_key);

				if(sel_key.Type() == HPS::Type::ShellKey)
				{
					for (auto const & key : navigation_keys)
					{
						if (sel_key == key)
						{
							plane_normal_valid = false;
							return false;
						}
					}

					ShellKey shell(sel_key);
					SizeTArray faces;
					if(item.ShowFaces(faces))
					{
						IntArray face_list;
						PointArray points;
						shell.ShowFacelist(face_list);
						shell.ShowPoints(points);

						//find the faces[0]th face
						size_t counter = 0;
						size_t current_face = 0;
						while (counter != faces[0])
						{
							current_face += (face_list[current_face] + 1);
							counter++;
						}

						if (face_list[current_face] > 2)
						{
							PointArray significant_points;
							significant_points.push_back(points[face_list[current_face + 1]]);
							significant_points.push_back(points[face_list[current_face + 2]]);
							significant_points.push_back(points[face_list[current_face + 3]]);

							KeyPath sel_path;
							item.ShowPath(sel_path);
							MatrixKit mat;
							sel_path.ShowNetModellingMatrix(mat);
							significant_points = mat.Transform(significant_points);

							//assuming that each 
							HPS::Vector basis_one = (significant_points[1] - significant_points[0]);
							HPS::Vector basis_two = (significant_points[2] - significant_points[0]);
							plane_normal = basis_one.Cross(basis_two);
							plane_normal.Normalize();

							//we've got a useful normal
							plane_normal_valid = true;
							item.ShowSelectionPosition(anchor_point);

							//compute extents
							Point corner1, corner2;
							sel_path.ConvertCoordinate(Coordinate::Space::Window, Point(1,1,0), Coordinate::Space::World, corner1);
							sel_path.ConvertCoordinate(Coordinate::Space::Window, Point(-1,-1,0), Coordinate::Space::World, corner2);
							float size = static_cast<float>((corner1 - corner2).Length()) * indicator_scale;
						
							//insert indicator under the view with overlay on
							indicator_seg.Flush(Search::Type::Geometry);
							InsertNormalIndicator(size);
							GetAttachedView().UpdateWithNotifier().Wait();
						}
					}
					else
						plane_normal_valid = false;
				}
			}
			else
			{
				plane_normal_valid = false;
				indicator_seg.Flush(Search::Type::Geometry);
				GetAttachedView().Update();
			}

			return false;
		} break;

	case OpState::Translating:
		{
			if (IsMouseTriggered(in_state))
			{
                TranslateCuttingPlane(in_state.GetEventPath(), in_state.GetLocation());
				return true;
			}
		} break;

	default: break;

	}

	return false;
}

bool HPS::CuttingSectionOperator::OnTouchDown(TouchState const & in_state)
{
    TouchArray touches = in_state.GetActiveEvent().Touches;
    
    if(tracked_touch_ID == -1)
    {
        if (op_state == OpState::Uninitialized)
        {
			plane_normal_valid = true;
            HPS::SelectionResults new_selection;
            HPS::WindowKey window = in_state.GetEventSource();
            
            if(window.GetSelectionControl().SelectByPoint(touches[0].Location, selection_options, new_selection))
            {
                SelectionItem item = new_selection.GetIterator().GetItem();
                HPS::Key sel_key;
                item.ShowSelectedItem(sel_key);
                
                if(sel_key.Type() == HPS::Type::ShellKey)
                {
                    ShellKey shell(sel_key);
                    SizeTArray faces;
                    VectorArray normals;
                    
                    if(item.ShowFaces(faces) && shell.ShowNetFaceNormalsByList(faces, normals))
                    {
                        //we've got a useful normal
                        plane_normal = normals[0];
                        item.ShowSelectionPosition(anchor_point);
                        return HandleMouseAndTouchDown(in_state.GetEventSource(), 1, in_state.GetEventPath(), touches[0].Location);
                    }
                }
            }
        }
        else
        {
			if (HandleMouseAndTouchDown(in_state.GetEventSource(), touches[0].TapCount, in_state.GetEventPath(), touches[0].Location))
			{
				tracked_touch_ID = touches[0].ID;
				return true;
			}
        }
    }
    
    return false;
}

bool HPS::CuttingSectionOperator::OnTouchMove(TouchState const & in_state)
{
    TouchArray touches = in_state.GetTouches();
    
    if(touches.size() == 1 && touches[0].ID == tracked_touch_ID && op_state == OpState::Translating)
    {
        TranslateCuttingPlane(in_state.GetEventPath(), touches[0].Location);
        return true;
    }
    
    return false;
}

bool HPS::CuttingSectionOperator::OnTouchUp(TouchState const & in_state)
{
    TouchArray up_touches = in_state.GetActiveEvent().Touches;
	bool handled = false;
    for(size_t i = 0, e = up_touches.size(); i < e && tracked_touch_ID != -1; ++i)
    {
        if(up_touches[i].ID == tracked_touch_ID)
        {
			handled = true;
            tracked_touch_ID = -1;
            if (op_state != OpState::Uninitialized)
            {
				if (!sections.empty())
				{
					bool cut_geometry_visibility;
					HPS::VisibilityControl visibility_control = GetAttachedView().GetModelOverrideSegmentKey().GetVisibilityControl();
					if (visibility_control.ShowCutFaces(cut_geometry_visibility) && !cut_geometry_visibility)
					{
						visibility_control.SetCutGeometry(true);
						op_state = OpState::Initialized;
						GetAttachedView().Update();
					}
				}
            }
        }
    }
	return handled;
}

namespace
{
	static const float arrow_head_length = 0.4f;
	static const float arrow_head_width = 0.16f;
	static const float arrow_stem_width = 0.07f;
}

bool HPS::CuttingSectionOperator::OnTimerTick(TimerTickEvent const & in_event)
{
	HPS_UNREFERENCED(in_event);
	if (last_skipped_highlight_state_valid &&
		last_highlight_notifier.Status() != HPS::Window::UpdateStatus::InProgress &&
		op_state != OpState::Uninitialized &&
		enable_mouse_over_highlighting)
	{
		last_skipped_highlight_state_valid = false;
		MouseOverHighlighting(last_skipped_highlight_state);
	}

	return false;
}

void HPS::CuttingSectionOperator::InsertNormalIndicator(float scale)
{
	PointArray pts(6);
	FloatArray radii(6);

	CameraKit current_camera;
	GetAttachedView().GetSegmentKey().ShowCamera(current_camera);
	Point position, target;
	current_camera.ShowPosition(position);
	current_camera.ShowTarget(target);
	Vector viewing_direction (target - position);
	Vector cone_direction = plane_normal;
	if (viewing_direction.Dot(plane_normal) > 0)
		cone_direction *= -1;

	//duplicating points to get better smooth shading
	pts[0] = anchor_point;
	pts[1] = anchor_point + scale * arrow_head_length * cone_direction;
	pts[2] = pts[1];
	pts[3] = pts[1];
	pts[4] = pts[1];
	pts[5] = anchor_point + scale * cone_direction;

	radii[0] = 0.0f;
	radii[1] = scale * arrow_head_width;
	radii[2] = radii[1];
	radii[3] = scale * arrow_stem_width;
	radii[4] = radii[3];
	radii[5] = radii[3];

	indicator_seg.InsertCylinder(pts, radii);
	indicator_seg.InsertMarker(anchor_point);
}

void HPS::CuttingSectionOperator::SetPlaneMaterial(HPS::MaterialMappingKit const & in_plane_material)
{
	plane_material = in_plane_material;

	try 
	{
		plane_representation_segment.SetMaterialMapping(plane_material);
		GetAttachedView().Update();
	}
	catch (HPS::InvalidObjectException const &)
	{ }
}

HPS::ShellKey HPS::CuttingSectionOperator::InsertCuttingPlaneGeometry()
{
	//if the plane is exactly perpendicular to the x, y or z axis,
	//determine the size of the cutting plane geometry based on the
	//size of the appropriate bounding box dimensions
	//otherwise insert a cutting plane centered around the point the user selected, 
	//with a side length equal to the bounding box diagonal / 1.5

	HPS::Point dimension(model_bounding.Diagonal().x, model_bounding.Diagonal().y, model_bounding.Diagonal().z);
	float percent_offset = 0.10f;
	float bbx_z = (float)((model_bounding.Diagonal().z / 2) + (model_bounding.Diagonal().z * percent_offset));
	float bbx_y = (float)((model_bounding.Diagonal().y / 2) + (model_bounding.Diagonal().y * percent_offset));
	float bbx_x = (float)((model_bounding.Diagonal().x / 2) + (model_bounding.Diagonal().x * percent_offset));
	HPS::Point model_center(model_bounding.min.x + dimension.x / 2,
							model_bounding.min.y + dimension.y / 2,
							model_bounding.min.z + dimension.z / 2);

	HPS::PointArray points;
	if (plane_normal == HPS::Vector(1, 0, 0) || plane_normal == HPS::Vector(-1, 0, 0))
	{
		points.push_back(HPS::Point(anchor_point.x, model_center.y - bbx_y, model_center.z - bbx_z));
		points.push_back(HPS::Point(anchor_point.x, model_center.y + bbx_y, model_center.z - bbx_z));
		points.push_back(HPS::Point(anchor_point.x, model_center.y + bbx_y, model_center.z + bbx_z));
		points.push_back(HPS::Point(anchor_point.x, model_center.y - bbx_y, model_center.z + bbx_z));
	}
	else if (plane_normal == HPS::Vector(0, 1, 0) || plane_normal == HPS::Vector(0, -1, 0))
	{
		points.push_back(HPS::Point(model_center.x - bbx_x, anchor_point.y, model_center.z - bbx_z));
		points.push_back(HPS::Point(model_center.x + bbx_x, anchor_point.y, model_center.z - bbx_z));
		points.push_back(HPS::Point(model_center.x + bbx_x, anchor_point.y, model_center.z + bbx_z));
		points.push_back(HPS::Point(model_center.x - bbx_x, anchor_point.y, model_center.z + bbx_z));
	}
	else if (plane_normal == HPS::Vector(0, 0, 1) || plane_normal == HPS::Vector(0, 0, -1))
	{
		points.push_back(HPS::Point(model_center.x - bbx_x, model_center.y - bbx_y, anchor_point.z));
		points.push_back(HPS::Point(model_center.x + bbx_x, model_center.y - bbx_y, anchor_point.z));
		points.push_back(HPS::Point(model_center.x + bbx_x, model_center.y + bbx_y, anchor_point.z));
		points.push_back(HPS::Point(model_center.x - bbx_x, model_center.y + bbx_y, anchor_point.z));
	}
	else
	{
		HPS::Vector basis_one(plane_normal.y, plane_normal.z, -plane_normal.x);
		HPS::Vector basis_two = basis_one.Cross(plane_normal);
		basis_one = basis_two.Cross(plane_normal);
		float length = (float)model_bounding.Diagonal().Length() / 1.5f;
		points.push_back(anchor_point + length * (basis_one + basis_two));
		points.push_back(anchor_point + length * (basis_one - basis_two));
		points.push_back(anchor_point + length * (-basis_one - basis_two));
		points.push_back(anchor_point + length * (-basis_one + basis_two));

		HPS::Vector center_anchor_distance(anchor_point - model_center);
		HPS::Vector projection_on_normal = center_anchor_distance.Dot(plane_normal) * plane_normal;
		center_anchor_distance = center_anchor_distance - projection_on_normal;

		HPS::MatrixKit xform;
		xform.Translate(-center_anchor_distance.x, -center_anchor_distance.y, -center_anchor_distance.z);
		points = xform.Transform(points);
	}

	HPS::IntArray facelist;
	facelist.push_back(4);
	facelist.push_back(0);
	facelist.push_back(1);
	facelist.push_back(2);
	facelist.push_back(3);

	return plane_representation_segment.Subsegment("").InsertShell(points, facelist);
}

void HPS::CuttingSectionOperator::MouseOverHighlighting(HPS::MouseState const & in_state)
{
	bool updating = false;
	if (!sections.empty() &&
		last_highlight_notifier.Status() != HPS::Window::UpdateStatus::InProgress &&
		op_state != OpState::Uninitialized &&
		op_state != OpState::FacePicking)
	{
		HPS::WindowKey event_source = in_state.GetEventSource();

		//highlight the cutting plane when we mouse over it
		HPS::SelectionResults results;
		plane_representation_segment.GetSelectabilityControl().SetFaces(HPS::Selectability::Value::On);
		size_t count = in_state.GetEventSource().GetSelectionControl().SelectByPoint(in_state.GetLocation(), mouse_over_selection_options, results);
		plane_representation_segment.GetSelectabilityControl().SetEverything(HPS::Selectability::Value::Off);

		if (count > 0)
		{
			HPS::Key selected_key;
			results.GetIterator().GetItem().ShowSelectedItem(selected_key);
			if (selected_key.Type() == HPS::Type::ShellKey)
			{
				bool found = false;
				for (auto it = sections.begin(), e = sections.end(); it != e; ++it)
				{
					if (std::find(it->second.begin(), it->second.end(), (HPS::ShellKey)selected_key) != it->second.end())
					{
						found = true;
						break;
					}
				}

				if (found && !is_plane_highlighted)
				{
					in_state.GetEventSource().GetHighlightControl().Highlight(selected_key, highlight_options);
					is_plane_highlighted = true;
					last_highlight_notifier = event_source.UpdateWithNotifier();
					updating = true;
				}
				else if (!found && is_plane_highlighted)
				{
					in_state.GetEventSource().GetHighlightControl().Unhighlight(highlight_options);
					is_plane_highlighted = false;
					last_highlight_notifier = event_source.UpdateWithNotifier();
					updating = true;
				}
			}
		}
		else if (is_plane_highlighted)
		{
			in_state.GetEventSource().GetHighlightControl().Unhighlight(highlight_options);
			is_plane_highlighted = false;
			last_highlight_notifier = event_source.UpdateWithNotifier();
			updating = true;
		}

		last_skipped_highlight_state_valid = false;
	}

	if (!updating)
	{
		last_skipped_highlight_state = in_state;
		last_skipped_highlight_state_valid = true;
	}
}

bool HPS::CuttingSectionOperator::HandleMouseAndTouchDown(WindowKey const & in_event_source, size_t in_number_of_clicks,
                                                          KeyPath const & in_event_path, WindowPoint const & in_event_location)
{
    if((op_state == OpState::Uninitialized || op_state == OpState::FacePicking) && plane_normal_valid)
    {
        //insert section and plane
        float d = -(plane_normal.x * anchor_point.x + plane_normal.y * anchor_point.y + plane_normal.z * anchor_point.z);
        indicator_seg.Flush(Search::Type::Geometry);

		HPS::CuttingSectionKey section_key = cutting_sections_segment.InsertCuttingSection(Plane(plane_normal, d));
		section_key.SetVisualization(CuttingSection::Mode::None, RGBAColor(0.5f, 0.5f, 0.5f, 0.5f));
		HPS::ShellKey plane_representation = InsertCuttingPlaneGeometry();

		if (!sectioning || sections.empty())
		{
			std::vector<ShellKey> planes(1, plane_representation);
			sections.push_back(std::make_pair(section_key, planes));
		}
		else if (sectioning && sections.size() == 1)
		{
			HPS::PlaneArray active_sections;
			sections[0].first.ShowPlanes(active_sections);
			active_sections.insert(active_sections.end(), Plane(plane_normal, d));
			sections[0].first.EditPlanesByReplacement(0, active_sections);
			sections[0].second.push_back(plane_representation);
		}
        
        op_state = OpState::Initialized;
        GetAttachedView().Update();
    }
    else
    {
		skip_mouse_overs = true;
        HPS::SelectionResults results;
		plane_representation_segment.GetSelectabilityControl().SetFaces(HPS::Selectability::Value::On);
        size_t count = in_event_source.GetSelectionControl().SelectByPoint(in_event_location, mouse_over_selection_options, results);
		plane_representation_segment.GetSelectabilityControl().SetEverything(HPS::Selectability::Value::Off);
        
        if (count > 0)
        {
            HPS::Key selected_key;
            results.GetIterator().GetItem().ShowSelectedItem(selected_key);

			for (auto it = sections.begin(), e = sections.end(); it != e; ++it)
			{
				auto found_shell = it->second.end();
				if ((found_shell = std::find(it->second.begin(), it->second.end(), (HPS::ShellKey)selected_key)) != it->second.end())
				{
					translating_plane_offset = found_shell - it->second.begin();
					if (in_number_of_clicks == 2)
					{
						if (!sectioning)
						{
							//we selected a valid plane.
							//flip all of them and move on
							HPS::PlaneArray planes;
							it->first.ShowPlanes(planes);
							for (auto & plane : planes)
								plane = -plane;
							it->first.EditPlanesByReplacement(0, planes);
							skip_mouse_overs = false;
							break;
						}
					}
					else
					{
						op_state = OpState::Translating;
						translating_plane_representation = (HPS::ShellKey)selected_key;
						translating_section = it->first;
						
						in_event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_event_location, HPS::Coordinate::Space::World, start_world_point);
						skip_mouse_overs = false;
						return true;
					}
				}
			}
        }
        return false;
    }
    
    return false;
}

void HPS::CuttingSectionOperator::TranslateCuttingPlane(HPS::KeyPath const & in_event_path, HPS::WindowPoint const & in_event_location)
{
    GetAttachedView().GetModelOverrideSegmentKey().GetVisibilityControl().SetCutGeometry(false);
    
    HPS::WorldPoint current_point;
    in_event_path.ConvertCoordinate(HPS::Coordinate::Space::Window, in_event_location, HPS::Coordinate::Space::World, current_point);
    
    HPS::Vector delta = current_point - start_world_point;
    
    HPS::PlaneArray cutting_planes;
    translating_section.ShowPlanes(cutting_planes);
    HPS::Vector cutting_plane_normal(cutting_planes[translating_plane_offset].a, cutting_planes[translating_plane_offset].b, cutting_planes[translating_plane_offset].c);
    HPS::Vector adjusted_delta = delta.Dot(cutting_plane_normal) * cutting_plane_normal;
    HPS::MatrixKit transform;
    transform.Translate(adjusted_delta.x, adjusted_delta.y, adjusted_delta.z);
    
    if (delta.Dot(cutting_plane_normal) < 0)
        cutting_planes[translating_plane_offset].d += (float)adjusted_delta.Length();
    else
        cutting_planes[translating_plane_offset].d -= (float)adjusted_delta.Length();
    
    HPS::PointArray plane_points;
    translating_plane_representation.ShowPoints(plane_points);
    plane_points = transform.Transform(plane_points);
    translating_plane_representation.EditPointsByReplacement(0, plane_points);
	if (sectioning)
		ViewAlignSectionPlanes(cutting_planes);

    translating_section.EditPlanesByReplacement(0, cutting_planes);
    
    start_world_point = current_point;
    GetAttachedView().Update();
}

void HPS::CuttingSectionOperator::ViewAlignSectionPlanes(PlaneArray & in_out_planes) const
{
	HPS::Point pos;
	HPS::Point target;

	GetAttachedView().GetSegmentKey().GetCameraControl().ShowPosition(pos);
	GetAttachedView().GetSegmentKey().GetCameraControl().ShowTarget(target);

	HPS::Vector cam_dir = HPS::Vector(target - pos).Normalize();

	for (HPS::Plane & plane : in_out_planes)
	{
		HPS::Vector pnormal(plane.a, plane.b, plane.c);
		if (pnormal.Dot(cam_dir) > 0)
			plane = -plane;
	}
}
void HPS::CuttingSectionOperator::SetPlanes(PlaneArray const & in_planes)
{
	sections.clear();
	indicator_seg.Flush(Search::Type::Geometry);
	cutting_sections_segment.Flush(Search::Type::Geometry, HPS::Search::Space::Subsegments);
	plane_representation_segment.Flush(Search::Type::Geometry, HPS::Search::Space::Subsegments);

	std::vector<ShellKey> all_the_planes_representations;
	for (auto const & plane : in_planes)
	{
		plane_normal = HPS::Vector(plane.a, plane.b, plane.c);
		plane_normal.Normalize();
		plane_normal_valid = true;
		anchor_point = HPS::Point(0, 0, 0) - plane_normal * plane.d;

		HPS::ShellKey plane_representation = InsertCuttingPlaneGeometry();
		if (sectioning)
			all_the_planes_representations.push_back(plane_representation);
		else
		{
			HPS::CuttingSectionKey section_key = cutting_sections_segment.InsertCuttingSection(Plane(plane_normal, plane.d));
			section_key.SetVisualization(CuttingSection::Mode::None, RGBAColor(0.5f, 0.5f, 0.5f, 0.5f));
			std::vector<ShellKey> planes(1, plane_representation);
			sections.push_back(std::make_pair(section_key, planes));
		}
	}

	if (sectioning)
	{
		PlaneArray view_aligned_planes = in_planes;
		ViewAlignSectionPlanes(view_aligned_planes);
		HPS::CuttingSectionKey section_key = cutting_sections_segment.InsertCuttingSection(view_aligned_planes);
		section_key.SetVisualization(CuttingSection::Mode::None, RGBAColor(0.5f, 0.5f, 0.5f, 0.5f));
		sections.push_back(std::make_pair(section_key, all_the_planes_representations));
	}

	if (op_state != OpState::FacePicking)
		op_state = OpState::Initialized;

	GetAttachedView().Update();
}

HPS::PlaneArray HPS::CuttingSectionOperator::GetPlanes()
{
	HPS::PlaneArray out_planes;
	for (auto it = sections.begin(), e = sections.end(); it != e; ++it)
	{
		HPS::PlaneArray some_planes;
		it->first.ShowPlanes(some_planes);
		out_planes.insert(out_planes.end(), some_planes.begin(), some_planes.end());
	}

	return out_planes;
}

void HPS::CuttingSectionOperator::SetSectioning(bool in_sectioning)
{
	if (in_sectioning == sectioning)
		return;

	sectioning = in_sectioning;
	if (sections.empty())
		return;

	indicator_seg.Flush(Search::Type::Geometry);

	if (in_sectioning && sections.size() > 1)
	{
		//make everything into one section
		HPS::PlaneArray all_the_planes = GetPlanes();

		sections.clear();
		cutting_sections_segment.Flush(Search::Type::Geometry, HPS::Search::Space::Subsegments);
		plane_representation_segment.Flush(Search::Type::Geometry, HPS::Search::Space::Subsegments);

		std::vector<ShellKey> all_the_planes_representations;
		for (auto & plane : all_the_planes)
		{
			plane_normal = HPS::Vector(plane.a, plane.b, plane.c);
			anchor_point = HPS::Point(0, 0, 0) - plane_normal * plane.d;
			all_the_planes_representations.push_back(InsertCuttingPlaneGeometry());
		}

		ViewAlignSectionPlanes(all_the_planes);
		HPS::CuttingSectionKey section_key = cutting_sections_segment.InsertCuttingSection(all_the_planes);
		section_key.SetVisualization(CuttingSection::Mode::None, RGBAColor(0.5f, 0.5f, 0.5f, 0.5f));
		sections.push_back(std::make_pair(section_key, all_the_planes_representations));
	}
	else if (sections.size() == 1)
	{
		auto cutting_key = sections[0].first;
		auto representation_planes = sections[0].second;

		HPS::PlaneArray planes;
		cutting_key.ShowPlanes(planes);
		if (planes.size() == 1)
			return;

		sections.clear();
		cutting_sections_segment.Flush(Search::Type::Geometry, HPS::Search::Space::Subsegments);
		plane_representation_segment.Flush(Search::Type::Geometry, HPS::Search::Space::Subsegments);

		for (auto & plane : planes)
		{
			HPS::CuttingSectionKey section_key = cutting_sections_segment.InsertCuttingSection(plane);
			section_key.SetVisualization(CuttingSection::Mode::None, RGBAColor(0.5f, 0.5f, 0.5f, 0.5f));
			plane_normal = HPS::Vector(plane.a, plane.b, plane.c);
			anchor_point = HPS::Point(0, 0, 0) - plane_normal * plane.d;
			HPS::ShellKey plane_representation = InsertCuttingPlaneGeometry();
			std::vector<ShellKey> this_one_plane(1, plane_representation);
			sections.push_back(std::make_pair(section_key, this_one_plane));
		}
	}
}

void HPS::CuttingSectionOperator::SetIndicatorVisibility(bool in_use_indicator)
{
	if (in_use_indicator)
		op_state = OpState::FacePicking;
	else
		op_state = OpState::Initialized;
}

void HPS::CuttingSectionOperator::SetupOperatorSegment()
{
	if (operator_root_segment.Type() == HPS::Type::None)
	{
		operator_root_segment = attached_view.GetSegmentKey().Subsegment("");
		indicator_seg = operator_root_segment.Subsegment("indicator");
		indicator_seg.GetDrawingAttributeControl().SetOverlay(HPS::Drawing::Overlay::Default);
		indicator_seg.GetSelectabilityControl().SetEverything(Selectability::Value::Off);
		indicator_seg.GetLightingAttributeControl().SetInterpolationAlgorithm(Lighting::InterpolationAlgorithm::Phong);
		indicator_seg.GetBoundingControl().SetExclusion(true);
		indicator_seg.GetMaterialMappingControl().SetFaceColor(RGBAColor(1.0f, 0, 0)).SetMarkerColor(RGBAColor(0,0,0));
		indicator_seg.GetVisibilityControl().SetEverything(false).SetFaces(true).SetFaceLights(true).SetMarkers(true);
		indicator_seg.GetCylinderAttributeControl().SetTessellation(20);

		cutting_sections_segment = operator_root_segment.Subsegment("cutting_sections");
		cutting_sections_segment.GetVisibilityControl().SetShadows(false);
		cutting_sections_segment.GetVisualEffectsControl().SetSimpleReflection(false).SetSimpleShadow(false).SetPostProcessEffectsEnabled(false);
		plane_representation_segment = operator_root_segment.Subsegment("plane_geometry");
		plane_representation_segment.SetMaterialMapping(plane_material);
		plane_representation_segment.GetVisibilityControl().SetCuttingSections(false).SetEdges(true).SetPerimeterEdges(true).SetFaces(true);
		plane_representation_segment.GetDrawingAttributeControl().SetFaceDisplacement(-128);
		plane_representation_segment.GetBoundingControl().SetExclusion(true);
		plane_representation_segment.GetSelectabilityControl().SetEverything(HPS::Selectability::Value::Off);

		operator_root_segment.GetVisualEffectsControl().SetPostProcessEffectsEnabled(false);
	}
}

void HPS::CuttingSectionOperator::SetPlaneVisibility(bool in_visibility)
{
	SetupOperatorSegment();
	plane_representation_segment.GetVisibilityControl().SetFaces(in_visibility).SetEdges(in_visibility).SetPerimeterEdges(in_visibility);
}

bool HPS::CuttingSectionOperator::GetPlaneVisibility()
{
	bool out_visibility = false;
	SetupOperatorSegment();
	plane_representation_segment.GetVisibilityControl().SetFaces(out_visibility);

	return out_visibility;
}