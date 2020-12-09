// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_HCA_H
#define SPRK_HCA_H

#include "sprk.h"

#ifdef _MSC_VER
#ifndef STATIC_APP
#	ifdef SPRK_HCA
#		define HPS_HCA_API __declspec (dllexport)
#	else
#		define HPS_HCA_API __declspec (dllimport)
#	endif
#endif
#else
#	include <stddef.h>
#endif

#ifndef HPS_HCA_API
#	define HPS_HCA_API
#endif

#include <mutex>
#include <vector>

namespace HPS
{
/*! The HCA class contains objects used for importing Stream Cache files. */
class HPS_HCA_API HCA
{
public:

	/*! The ImportNotifier class is a smart-pointer that is tied to a file import.  It is used to interact with an ongoing import or get the results from a completed import. */
	class HPS_HCA_API ImportNotifier : public IONotifier
	{
	public:
		/*! The default constructor creates an ImportNotifier object which is not tied to any file import. */
		ImportNotifier();

		/*! The copy constructor creates a new ImportNotifier object that is associated with the same file import as the source ImportNotifier.
		 * 	\param in_that The source ImportNotifier to copy. */
		ImportNotifier(ImportNotifier const & in_that);

		/*! The conversion constructor creates a new derived ImportNotifier object from a base IONotifier object.
		 * 	The copy will only be successful if the source notifier is really an upcast of this notifier type.
		 *	Otherwise the copy will fail and the resulting ImportNotifier will be invalid.
		 * 	\param in_that The source IONotifier to copy. */
		ImportNotifier(IONotifier const & in_that);

		/*! The move constructor creates an ImportNotifier by transferring the underlying impl of the rvalue reference to this ImportNotifier
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ImportNotifier to take the impl from. */
		ImportNotifier(ImportNotifier && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ImportNotifier thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ImportNotifier to take the impl from.
		 * 	\return A reference to this ImportNotfier. */
		ImportNotifier &		operator=(ImportNotifier && in_that);

		virtual ~ImportNotifier();

	static const HPS::Type staticType = HPS::Type::HCAImportNotifier;
		HPS::Type				ObjectType() const { return staticType; };

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment.
		 *	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier const & in_that);

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment. */
		void					Assign(ImportNotifier const & in_that);

		/*!	Get the the Model where the Stream Cache file was imported into. 
		 *  Throws an IOException if the import is not complete or was not successful.
		 *  \return The Model containing the Stream Cache file for a successful file import. */
		HPS::Model &			GetTarget() const;
	};

	/*! The HPS::HCA::ImportOptionsKit class is a user space object.  It contains settings controlling what and how Stream Cache data is imported via the HCA Sprocket. */
	class HPS_HCA_API ImportOptionsKit : public HPS::Sprocket
	{
	public:
		/*! The default constructor creates an empty ImportOptionsKit object. */
		ImportOptionsKit();

		/*! The copy constructor creates a new ImportOptionsKit object that contains the same settings as the source ImportOptionsKit.
		 * 	\param in_kit The source ImportOptionsKit to copy. */
		ImportOptionsKit(ImportOptionsKit const & in_kit);

		/*! The move constructor creates an ImportOptionsKit by transferring the underlying impl of the rvalue reference to this ImportOptionsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ImportOptionsKit to take the impl from. */
		ImportOptionsKit(ImportOptionsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ImportOptionsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ImportOptionsKit to take the impl from.
		 * 	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit & operator=(ImportOptionsKit && in_that);

		virtual ~ImportOptionsKit();

	static const HPS::Type staticType = HPS::Type::HCAImportOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ImportOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Import.
		 *	\return An ImportOptionsKit with the default settings. */
		static ImportOptionsKit		GetDefault();

		/*! Copies the source ImportOptionsKit into this ImportOptionsKit.
		 * 	\param in_kit The source ImportOptionsKit to copy. */
		void						Set(ImportOptionsKit const & in_kit);

		/*! Copies this ImportOptionsKit into the given ImportOptionsKit.
		 * 	\param out_kit The ImportOptionsKit to populate with the contents of this ImportOptionsKit. */
		void						Show(ImportOptionsKit & out_kit) const;

		/*! Copies the source ImportOptionsKit into this ImportOptionsKit.
		 * 	\param in_kit The source ImportOptionsKit to copy.
		 * 	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			operator=(ImportOptionsKit const & in_kit);

		/*! Indicates whether this ImportOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ImportOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ImportOptionsKit is equivalent to this ImportOptionsKit.
		 *	\param in_kit The source ImportOptionsKit to compare to this ImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ImportOptionsKit const & in_kit) const;

		/*!	Check if the source ImportOptionsKit is equivalent to this ImportOptionsKit.
		 *	\param in_kit The source ImportOptionsKit to compare to this ImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ImportOptionsKit const & in_kit) const;

		/*!	Check if the source ImportOptionsKit is not equivalent to this ImportOptionsKit.
		 *	\param in_kit The source ImportOptionsKit to compare to this ImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ImportOptionsKit const & in_kit) const;

		/*! Sets a Model to be used as part of the import. Geometry will be imported in the model Segment, and definitions will be added to
		*	the Portfolio associated with the Model provided.
		*	Setting a target is required for the import to succeed.
		*	\param in_model Model to use.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTarget(HPS::Model const & in_target);

		/*! Sets a View to be used as part of the import.
		*	Setting a View is required for the import to succeed.
		*	\param in_view View to use for camera tracking.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetView(HPS::View const & in_view);

		/*! Sets the path to hc_access.js which will be used during the import.
		*	Setting a Javascript Path is required for the import to succeed.
		*	The hc_access.js file is provided with HOOPS Visualize.
		*	\param javascript_path Path of the javascript file to use.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetJavascriptPath(HPS::UTF8 const & javascript_path);

		/*! Shows the target import setting.
		*	\param out_model Model to use to implicitly set Segment, Library and Portfolio into which the data will be imported.
		*	\return Returns <span class='code'>false</span> if a target setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowTarget(HPS::Model & out_model) const;

		/*! Shows the view import setting.
		*	\param out_view View to use.
		*	\return Returns <span class='code'>false</span> if a view setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowView(HPS::View & out_view) const;

		/*! Shows the path that will be used to find hc_access.js during import.
		*	\param out_javascript_path The path used to reach hc_access.js.
		*	\return Returns <span class='code'>false</span> if a javascript path setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowJavascriptPath(HPS::UTF8 & out_javascript_path) const;

		/*! Removes the target import setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTarget();

		/*! Removes the view import setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetView();

		/*!Removes javascript path import setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetJavascriptPath();

		/*! Removes all settings from this ImportOptionsKit.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetEverything();
	};

	/*! The HPS::HCA::NetworkImportOptionsKit class is a user space object.  It contains settings controlling what and how Stream Cache data is imported over a network via the HCA Sprocket. */
	class HPS_HCA_API NetworkImportOptionsKit : public HPS::Sprocket
	{
	public:
		/*! The default constructor creates an empty NetworkImportOptionsKit object. */
		NetworkImportOptionsKit();

		/*! The copy constructor creates a new NetworkImportOptionsKit object that contains the same settings as the source NetworkImportOptionsKit.
		 * 	\param in_kit The source NetworkImportOptionsKit to copy. */
		NetworkImportOptionsKit(NetworkImportOptionsKit const & in_kit);

		/*! The move constructor creates an NetworkImportOptionsKit by transferring the underlying impl of the rvalue reference to this NetworkImportOptionsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an NetworkImportOptionsKit to take the impl from. */
		NetworkImportOptionsKit(NetworkImportOptionsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this NetworkImportOptionsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an NetworkImportOptionsKit to take the impl from.
		 * 	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit & operator=(NetworkImportOptionsKit && in_that);

		virtual ~NetworkImportOptionsKit();

		static const HPS::Type staticType = HPS::Type::HCANetworkImportOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an NetworkImportOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Import.
		 *	\return An NetworkImportOptionsKit with the default settings. */
		static NetworkImportOptionsKit		GetDefault();

		/*! Copies the source NetworkImportOptionsKit into this NetworkImportOptionsKit.
		 * 	\param in_kit The source NetworkImportOptionsKit to copy. */
		void						Set(NetworkImportOptionsKit const & in_kit);

		/*! Copies this NetworkImportOptionsKit into the given NetworkImportOptionsKit.
		 * 	\param out_kit The NetworkImportOptionsKit to populate with the contents of this NetworkImportOptionsKit. */
		void						Show(NetworkImportOptionsKit & out_kit) const;

		/*! Copies the source NetworkImportOptionsKit into this NetworkImportOptionsKit.
		 * 	\param in_kit The source NetworkImportOptionsKit to copy.
		 * 	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			operator=(NetworkImportOptionsKit const & in_kit);

		/*! Indicates whether this NetworkImportOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this NetworkImportOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source NetworkImportOptionsKit is equivalent to this NetworkImportOptionsKit.
		 *	\param in_kit The source NetworkImportOptionsKit to compare to this NetworkImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(NetworkImportOptionsKit const & in_kit) const;

		/*!	Check if the source NetworkImportOptionsKit is equivalent to this NetworkImportOptionsKit.
		 *	\param in_kit The source NetworkImportOptionsKit to compare to this NetworkImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(NetworkImportOptionsKit const & in_kit) const;

		/*!	Check if the source NetworkImportOptionsKit is not equivalent to this NetworkImportOptionsKit.
		 *	\param in_kit The source NetworkImportOptionsKit to compare to this NetworkImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(NetworkImportOptionsKit const & in_kit) const;

		/*! Sets a Model to be used as part of the import. Geometry will be imported in the model Segment, and definitions will be added to
		*	the Portfolio associated with the Model provided.
		*	Setting a target is required for the import to succeed.
		*	\param in_model Model to use.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			SetTarget(HPS::Model const & in_target);

		/*! Sets a View to be used as part of the import.
		*	Setting a View is required for the import to succeed.
		*	\param in_view View to use for camera tracking.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			SetView(HPS::View const & in_view);

		/*! Sets the path to hc_access.js which will be used during the import.
		*	Setting a Javascript Path is required for the import to succeed.
		*	The hc_access.js file is provided with HOOPS Visualize.
		*	\param javascript_path Path of the javascript file to use.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			SetJavascriptPath(HPS::UTF8 const & javascript_path);

		/*! Sets the URL that will be used to connect to the Communicator server.
		*	The Network Path is required when importing a file over the network.
		*	\param network_path URL to use.
		*	\return A reference to this ImportOptionsKit. */
		NetworkImportOptionsKit &			SetNetworkPath(HPS::UTF8 const & network_path);

		/*! Whether to load and display previews of the model bounding when streaming starts.
		*	As actual geometry starts streaming in, it will replace its bounding.
		*	\param model_bounding_previews Whether model boundings will be streamed.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			SetModelBoundingPreviews(bool model_bounding_previews);

		/*! Whether to display a bounding mesh to replace geometry that was ejected because of the memory limit being hit.
		*	Additionally this option will only be honored if a memory limit is set.
		*	\param ejected_geometry_bounding_previews Whether ejected geometry should be replaced with boundings.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			SetEjectedGeometryBoundingPreviews(bool ejected_geometry_bounding_previews);

		/*! Sets the memory amount to devote to geometry data, specified in Megabytes.
		*	Passing a value of zero to this function is equivalent to calling UnsetLimitMib()
		*	Reaching the memory limit will cause some geometry to be ejected from the scene so that more can stream in.
		*	\param limit_MiB Memory in Megabytes to use for geometry data.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			SetLimitMiB(uint32_t limit_MiB);


		/*! Shows the target import setting.
		*	\param out_model Model to use to implicitly set Segment, Library and Portfolio into which the data will be imported.
		*	\return Returns <span class='code'>false</span> if a target setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowTarget(HPS::Model & out_model) const;

		/*! Shows the view import setting.
		*	\param out_view View to use.
		*	\return Returns <span class='code'>false</span> if a view setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowView(HPS::View & out_view) const;

		/*! Shows the path that will be used to find hc_access.js during import.
		*	\param out_javascript_path The path used to reach hc_access.js.
		*	\return Returns <span class='code'>false</span> if a javascript path setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowJavascriptPath(HPS::UTF8 & out_javascript_path) const;

		/*! Shows the network path import setting.
		*	\param out_network_path The network path to use during the import.
		*	\return Returns <span class='code'>false</span> if a network path setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowNetworkPath(HPS::UTF8 & out_network_path) const;

		/*! Shows the model bounding preview import setting.
		*	\param out_model_bounding_previews Whether boundings should be visualized while waiting for geometry to stream in.
		*	\return Returns <span class='code'>false</span> if a model bounding preview setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowModelBoundingPreviews(bool & out_model_bounding_previews) const;

		/*! Shows the ejected bounding previews import setting.
		*	\param out_ejected_geometry_bounding_previews Whether ejected geometry should be replaced with a visualization of its boundings.
		*	\return Returns <span class='code'>false</span> if an ejected bounding preview setting was not specified <span class='code'>false</span> otherwise. */
		bool						ShowEjectedGeometryBoundingPreviews(bool & out_ejected_geometry_bounding_previews) const;

		/*! Shows the memory limit import setting.
		*	\param out_limit_MiB The amount of memory, specified in Megabytes, to use for geometry data.
		*	\return Returns <span class='code'>false</span> if a memory limit setting was not specified, <span class='code'>false</span> otherwise. */
		bool						ShowLimitMiB(uint32_t & out_limit_MiB) const;


		/*! Removes the target import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetTarget();

		/*! Removes the view import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetView();

		/*!Removes javascript path import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetJavascriptPath();

		/*!Removes network path import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetNetworkPath();

		/*! Removes model bounding previews setting import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetModelBoundingPreviews();

		/*! Removes ejected bounding previews setting import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetEjectedGeometryBoundingPreviews();

		/*! Removes the geometry data memory limit import setting.
		*	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetLimitMiB();

		/*! Removes all settings from this NetworkImportOptionsKit.
		 *	\return A reference to this NetworkImportOptionsKit. */
		NetworkImportOptionsKit &			UnsetEverything();
	};


	/*!	The File class provides functions to import Stream Cache files. */
	class HPS_HCA_API File
	{
	public:
		/*!	Performs an asynchronous local import of the specified Stream Cache file with the provided options. May throw an IOException prior to starting the asynchronous import.
		 *	After the import has started it will take place on a separate thread. 
		 *	The status of the import can be checked using the returned ImportNotifier.
		 *	It is possible to wait for the import to complete by using the Wait method from the returned ImportNotifier.
		 *	While it is possible to interact with the scene's camera even before the model has fully finished loading, attempting to select the model, highlight it, 
		 *	hide, isolate or show parts of it while it is being loaded will result in an exception being thrown.
		 * 	\param in_file_name The name of the file to import.
		 *	\param in_options The options controlling the import of the Stream Cache file.
		 *	\return An ImportNotifier object that can be used to query the import progress and status. */
		static ImportNotifier Import(char const * in_file_name, ImportOptionsKit const & in_options);

		/*!	Performs an asynchronous import of the specified Stream Cache file with the provided options over the network. May throw an IOException prior to starting the asynchronous import.
		*	After the import has started it will take place on a separate thread. 
		*	The Wait method from the returned ImportNotifier can be used to wait for the minimum necessary model data is imported. Once Wait returns, it is safe
		*	to interact with the model, even as geometry continues to stream in.
		*	Attempting to select, highlight, isolate, hide, or show parts of the model before Wait returns can result in an exception being thrown.
		* 	\param in_file_name The name of the file to import.
		*	\param in_options The options controlling the import of the Stream Cache file.
		*	\return An ImportNotifier object that can be used to query the import progress and status. */
		static ImportNotifier Import(char const * in_file_name, NetworkImportOptionsKit const & in_options);

	private:
		//! Private default constructor to prevent instantiation.
		File();
	};

	/*! The RequestIdEvent class is the event that will be triggered for each HCA Model Request call. */
	class HPS_HCA_API RequestIdEvent : public Event
	{
	public:

		/*! The default constructor creates an empty RequestIdEvent object. */
		RequestIdEvent() : Event()
		{
			channel = GetClassID();
			consumable = false;
		}

		RequestIdEvent(uint32_t in_request_id) : Event(), request_id(in_request_id)
		{
			channel = GetClassID();
			consumable = false;
		}

		/*! This constructor converts an Event Object to a RequestIdEvent object.
		 * 	\param in_event The Event Object to be converted. */
		RequestIdEvent(Event const & in_event) : Event(in_event)
		{
			if (in_event.GetChannel() == Object::ClassID<RequestIdEvent>())
			{
				auto that = static_cast<RequestIdEvent const &>(in_event);
				request_id = that.request_id;
			}
			else
				throw HPS::InvalidSpecificationException("Invalid Event Type to Cast From.");
		}

		virtual ~RequestIdEvent();

		/*! Allocates and returns a copy of this RequestIdEvent.
		 * 	\return A copy of this RequestIdEvent. */
		Event * Clone() const override
		{
			RequestIdEvent * new_event = new RequestIdEvent(*this);
			return new_event;
		}

		uint32_t request_id;
	};

	class ModelTreeItem;
	class ModelTree;

	typedef ::std::shared_ptr<ModelTreeItem> ModelTreeItemPtr;
	typedef ::std::shared_ptr<ModelTree> ModelTreePtr;


	/*! The Model class provides interactions with a HCA Model */
	class HPS_HCA_API Model {

	public:

		/*! Constructs an uninitialized HCA::Model. */
		Model();

		/*! Constructs a HCA::Model under an existing HPS::Model.
		\param in_model the HPS::Model under which to create an HCA::Model. */
		Model(HPS::Model const & in_model);

		/*! Copy constructor. This constructor creates an HCA::Model which shares the same resources of the source HCA::Model.
		*	\param in_that The source HCA::Model to copy. */
		Model(Model const & that);

		/*! The move constructor creates an HCA::Model by transferring the underlying object of the rvalue reference to this HCA::Model.
		* 	\param in_that An rvalue reference to an HCA::Model to take the underlying object from. */
		Model(Model&& that);

		~Model();

		/*!	Check if the source Model is equivalent to this Model.
		*	\param other The source Model to compare to this Model.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool Equals(Model const & other) const;

		/*!	Check if the source Model is equivalent to this Model.
		*	\param other The source Model to compare to this Model.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool operator== (Model const & other) const;

		/*!	Check if the source Model is not equivalent to this Model.
		*	\param other The source Model to compare to this Model.
		*	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool operator!= (Model const & other) const;

		/*! Copies the source Model into this Model.
		* 	\param other The source Model to copy.
		* 	\return A reference to this Model. */
		Model& operator=(Model&& other);

		/*! Checks the state of the Model.
		* 	\return <span class='code'>true</span> if this Model is uninitialized, <span class='code'>false</span> otherwise. */
		bool Empty();

		/*! Shows the node ID for the root node of the Model.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param out_node_id the node ID for the root node of the Model.
		*	\return <span class='code'>true</span> if the root node ID was found, <span class='code'>false</span> otherwise. */
		bool ShowRootNode(int64_t & out_node_id);

		/*! Shows the name for the requested node.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_node_id the ID for the node whose name should be shown
		*	\param out_name the name of in_node_id, in the UTF8 locale. Might be an empty UTF8 object.
		*	\return <span class='code'>true</span> if the node name was found successfully, <span class='code'>false</span> otherwise. */
		bool ShowNodeName(int64_t in_node_id, HPS::UTF8 & out_name);

		/*! Shows the IDs of the children of the requested node.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_node_id the ID for the node whose children should be shown
		*	\param out_ids an array of IDs representing the children of in_node_id. 
		*	\return <span class='code'>true</span> if the node's children were found successfully, <span class='code'>false</span> otherwise. */
		bool ShowNodeChildren(int64_t in_node_id, HPS::Int64Array & out_ids);

		/*! Shows the ID of the parent of the requested node.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_node_id the ID for the node whose parent should be shown
		*	\param out_parent_id the ID representing the parent of in_node_id. Might be an empty array
		*	\return <span class='code'>true</span> if the node's parent was found successfully, <span class='code'>false</span> otherwise. */
		bool ShowNodeParent(int64_t in_node_id, int64_t & out_parent_id);

		/*! Requests properties for the specified node.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	When the request has been fulfilled, an event of type HCA::RequestIdEvent will be injected to the Database's Event Dispatcher.
		*	Registering a handler to HCA::RequestIdEvent will allow the user to know when this request has completed.
		*	The value of HCA::RequestIdEvent::request_id object will match out_requested_id, returned by this function.
		*	\param in_node_id the ID for the node whose properties should be collected.
		*	\param out_request_id an ID which uniquely identifies this request
		*	\return <span class='code'>true</span> if the node's properties were gathered successfully, <span class='code'>false</span> otherwise. */
		bool RequestNodeProperties(int64_t in_node_id, uint32_t & out_request_id);

		/*! Retrieves previously requested properties
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	Before retrieving properties, you should request them, using RequestNodeProperties.
		*	This function waits for the requested property to become available
		*	If your model's properties are not changing, it is sufficient to call RequestNodeProperties just once for each node ID of interest.
		*	Each property is returned as a pair or key and value. The out_keys and out_values parameters are two parallel arrays containing
		*	respectively the key and value of each property.
		*	\param in_request_id a unique ID representing a request for properties. This value is obtained through a call to RequestNodeProperties.
		*	\param in_timeout_ms the maximum amount of time to wait while trying to retrieve the node properties, specified in milliseconds. Passing 0 means that the function will wait indefinitely until it can retrieve the properties.
		*	\param out_node_id the ID of the node for which properties are being returned.
		*	\param out_valid whether the returned properties are valid.
		*	\param out_keys the property keys. Each entry in this array corresponds to the entry at the same index in the out_values array.
		*	\param out_values the property values. Each entry in this array corresponds to the entry at the same index in the out_keys array.
		*	\return <span class='code'>true</span> if the node's properties were retrieved successfully, <span class='code'>false</span> otherwise. */
		bool RetrieveNodeProperties(uint32_t in_request_id, uint32_t in_timeout_ms, int64_t & out_node_id, bool & out_valid, HPS::UTF8Array & out_keys, HPS::UTF8Array & out_values);

		/*! Given a KeyPath, the associated node ID is returned. 
		*	\param in_key_path the key path for which a node ID is being requested
		*	\param out_node_id the node ID that corresponds to in_key_path
		*	\return <span class='code'>true</span> if the node ID is associated to in_key_path, <span class='code'>false</span> otherwise.*/
		bool ShowNodeFromKeyPath(HPS::KeyPath const & in_key_path, int64_t & out_node_id);

		/*! Shows the specified node under the given Canvas. This makes the selected node visible.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to show the node.
		*	\param in_node_id The ID of the node to be shown */
		void Show(HPS::Canvas const & in_canvas, int64_t in_node_id);

		/*! Hides the specified node under the given Canvas. This makes the selected node invisible.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to hide the node.
		*	\param in_node_id The ID of the node to be hidden */
		void Hide(HPS::Canvas const & in_canvas, int64_t in_node_id);

		/*! Unhighlights the specified node under the given Canvas.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to unhighlight the node.
		*	\param in_node_id The ID of the node to be unhighlighted */
		void Unhighlight(HPS::Canvas const & in_canvas, int64_t in_node_id);

		/*! Highlights the specified node under the given Canvas, using the options provided.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to highlight the node.
		*	\param in_highlight_options The options to be used when highlighting in_node_id.
		*	\param in_node_id The ID of the node to be highlighted */
		void Highlight(HPS::Canvas const & in_canvas, HighlightOptionsKit const & in_highlight_options, int64_t in_node_id);

		/*! Highlights the specified nodes under the given Canvas, using the options provided.
		*	All specified nodes will be highlighted using the same options.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to highlight the nodes.
		*	\param in_highlight_options The options to be used when highlighting in_node_ids.
		*	\param in_node_ids an array of node IDs to be highlighted */
		void Highlight(HPS::Canvas const & in_canvas, HighlightOptionsKit const & in_highlight_options, HPS::Int64Array const & in_node_ids);

		/*! Shows only the specified node and hides the rest.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to isolate the node.
		*	\param in_node_id The ID of the node to isolate. */
		void Isolate(HPS::Canvas const & in_canvas, int64_t in_node_id);

		/*! Smoothly zooms the camera to focus on the specified node. The transition lasts half a second.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to zoom to the node.
		*	\param in_node_id The ID of the node to zoom to. */
		void Zoom(HPS::Canvas const & in_canvas, int64_t in_node_id);

		/*! Shows all nodes in the model, under the specified Canvas.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to show the nodes. */
		void ShowAll(HPS::Canvas const & in_canvas);

		/*! Checks whether the given node is currently Hidden as a result of a Hide or Isolate operation.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_node_id The ID of the node to examine. 
		*	\return <span class='code'>true</span> if the node is hidden, <span class='code'>false</span> otherwise. */
		bool IsHidden(int64_t in_node_id);

		/*! Checks whether the given node is currently highlighted.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_node_id The ID of the node to examine.
		*	\return <span class='code'>true</span> if the node is highlighted, <span class='code'>false</span> otherwise. */
		bool IsHighlighted(int64_t in_node_id);

		/*! This convenience function performs the HCA::ModelTreeItem::OnHighlightUnhighlight function on the root item of the Model Tree
		*	See the description of HCA::ModelTreeItem::OnHighlightUnhighlight for more information. */
		void OnHighlightUnhighlight();

		/*! Unhighlights all nodes in the model, under the specified Canvas.
		*	throws HPS::InvalidObjectException is the Model is uninitialized.
		*	\param in_canvas The Canvas in which to unhighlight the nodes. */
		void UnhighlightAll(HPS::Canvas const & in_canvas);

	private:

		friend class ModelTreeItem;
		//! Private default constructor to prevent instantiation.
		Model(void * in_opaque);
		void * opaque;
	};

	/*! The ModelTree class is a smart pointer which corresponds to a tree object in a GUI toolkit, e.g., a TreeView in WPF or a CTreeCtrl in MFC.
	 *	This class can be derived from to facilitate tree creation in the various GUI toolkits.  It is used to create a model browser. */
	class HPS_HCA_API ModelTree : public Sprocket
	{
	public:
		/*! \enum ItemType
		 *	Enumerates the types of items that can be contained in a ModelTree. */
		enum class ItemType
		{
			None,			//!< No type.
			Model,			//!< Item is an HCA::Model
			Node,			//!< Item is an HCA::Node
		};

		/*! The default constructor creates an uninitialized ModelTree object.  The Type() function will return Type::None. */
		ModelTree();

		/*! This constructor creates a ModelTree object associated with a Canvas and the specified HCA::Model. 
		*	This constructor should be used to create a new ModelTree object.
		 *	\param in_canvas The Canvas to associate this ModelTree with.
		 *	\param hca_model The model to associate this ModelTree with. */
		ModelTree(Canvas const & in_canvas, HCA::Model & hca_model);

		/*! The copy constructor creates a ModelTree object that shares the underlying smart-pointer of the source ModelTree.
		 *	\param in_that The source ModelTree to copy. */
		ModelTree(ModelTree const & in_that);

		/*! The move constructor creates a ModelTree by transferring the underlying object of the rvalue reference to this ModelTree.
		 * 	\param in_that An rvalue reference to a ModelTree to take the underlying object from. */
		ModelTree(ModelTree && in_that);

		virtual ~ModelTree();

		static const HPS::Type staticType = HPS::Type::HCAModelTree;
		virtual HPS::Type ObjectType() const { return staticType; }

		/*! Share the underlying smart-pointer of the ModelTree source.
		 *	\param in_that The ModelTree source of the assignment.
		 *	\return A reference to this ModelTree. */
		ModelTree &			operator=(ModelTree const & in_that);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ModelTree.
		 *	\param in_that An rvalue reference to a ModelTree to take the underlying object from.
		 *	\return A reference to this ModelTree. */
		ModelTree &			operator=(ModelTree && in_that);

		/*! Share the underlying smart-pointer of the ModelTree source.
		 *	\param in_that The ModelTree source of the assignment.
		 *	\return A reference to this ModelTree. */
		virtual void			Assign(ModelTree const & in_that);

		/*!	Check if the source ModelTree points to the same underlying impl as this ModelTree.
		 *	\param in_that The source ModelTree to compare to this ModelTree.
		 *	\return <span class='code'>true</span> if the objects reference the same underlying data, <span class='code'>false</span> otherwise. */
		bool					Equals(ModelTree const & in_that) const;

		/*!	Check if the source ModelTree points to a different impl than this ModelTree.
		 *	\param in_that The source ModelTree to compare to this ModelTree.
		 *	\return <span class='code'>true</span> if the objects reference different underlying data, <span class='code'>false</span> otherwise. */
		bool					operator!=(ModelTree const & in_that) const;

		/*!	Check if the source ModelTree points to the same underlying impl as this ModelTree.
		 *	\param in_that The source ModelTree to compare to this ModelTree.
		 *	\return <span class='code'>true</span> if the objects reference the same underlying data, <span class='code'>false</span> otherwise. */
		bool					operator==(ModelTree const & in_that) const;


		/*!	Sets the root ModelTreeItem for this ModelTree.  This object should be created on the heap and it should have been created with a ModelTreeItem constructor which
		 *	took an HCA::Model, otherwise an exception will be thrown.  Additionally, if there was already an existing root for this ModelTree, the Flush function
		 *	will be invoked prior to setting this as the new root, and ModelTreeItem::Expand will be invoked on the new root.
		 *	\param in_root The root ModelTreeItem for this ModelTree.
		 *	\sa Flush
		 *	\sa ModelTreeItem::Expand */
		void					SetRoot(ModelTreeItemPtr const & in_root);

		/*!	Gets the root ModelTreeItem for this ModelTree.
		 *	\return The root ModelTreeItem for this ModelTree. */
		ModelTreeItemPtr	GetRoot() const;

		/*!	Gets the HCA Model associated with this ModelTree.
		*	\return The HCA Model associated with this ModelTree. */
		HCA::Model & GetHCAModel() const;

		/*!	Gets the Canvas associated with this ModelTree.
		*	\return The Canvas associated with this ModelTree. */
		HPS::Canvas & GetCanvas() const;

		/*!	Resets the root ModelTreeItem for this ModelTree.  
		*	This will release the root ModelTreeItem by this ModelTree and set the root to a null pointer.  
		*	If this function is overridden, the overriding function should invoke this base function. */
		virtual void			Flush();
	};


	/*! The ModelTreeItem class is a smart pointer which corresponds to an item or node in a tree object in a GUI toolkit, e.g., a TreeViewItem in WPF or an HTREEITEM in MFC.
	 *	This class must be derived from to facilitate tree creation in the various GUI toolkits.  It is used to represent items in a model browser. */
	class HPS_HCA_API ModelTreeItem : public Sprocket
	{
	public:
		/*! The default constructor creates an uninitialized ModelTreeItem object.  The Type() function will return Type::None. */
		ModelTreeItem();

		/*! The copy constructor creates a ModelTreeItem object that shares the underlying smart-pointer of the source ModelTreeItem.
		 *	\param in_that The source ModelTreeItem to copy. */
		ModelTreeItem(ModelTreeItem const & in_that);

		/*! The move constructor creates a ModelTreeItem by transferring the underlying object of the rvalue reference to this ModelTreeItem.
		 * 	\param in_that An rvalue reference to a ModelTreeItem to take the underlying object from. */
		ModelTreeItem(ModelTreeItem && in_that);

		/*! This constructor creates a ModelTreeItem suitable for acting as a root in a ModelTree. 
		*	\param in_tree A pointer to the ModelTree which will contain this ModelTreeItem
		*	\param in_hca_model The HCA::Model which will be assigned to this ModelTreeItem. */
		ModelTreeItem(ModelTreePtr const & in_tree, HCA::Model const & in_hca_model);

		/*! This constructor creates a ModelTreeItem which is not suitable for acting as a root in a ModelTree, but is suitable to be a ModelTreeItem under the root of the ModelTree.
		 *	This constructor should be the one invoked in the overridden AddChild function. 
		 *	\param in_tree A pointer to the ModelTree which will contain this ModelTreeItem.
		 *	\param in_node_id The node ID to be associated with this ModelTreeItem. */
		ModelTreeItem(ModelTreePtr const & in_tree, int64_t in_node_id);

		virtual ~ModelTreeItem();

		static const HPS::Type staticType = HPS::Type::HCAModelTreeItem;
		HPS::Type ObjectType() const { return staticType; }

		/*! Share the underlying smart-pointer of the ModelTreeItem source.
		 *	\param in_that The ModelTreeItem source of the assignment.
		 *	\return A reference to this ModelTreeItem. */
		ModelTreeItem &				operator=(ModelTreeItem const & in_that);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ModelTreeItem.
		 *	\param in_that An rvalue reference to a ModelTreeItem to take the underlying object from.
		 *	\return A reference to this ModelTreeItem. */
		ModelTreeItem &				operator=(ModelTreeItem && in_that);

		/*! Share the underlying smart-pointer of the ModelTreeItem source.
		 *	\param in_that The ModelTreeItem source of the assignment.
		 *	\return A reference to this ModelTreeItem. */
		virtual void					Assign(ModelTreeItem const & in_that);

		/*!	Check if the source ModelTreeItem points to the same underlying impl as this ModelTreeItem.
		 *	\param in_that The source ModelTreeItem to compare to this ModelTreeItem.
		 *	\return <span class='code'>true</span> if the objects reference the same underlying data, <span class='code'>false</span> otherwise. */
		bool							Equals(ModelTreeItem const & in_that) const;

		/*!	Check if the source ModelTreeItem points to a different impl than this ModelTreeItem.
		 *	\param in_that The source ModelTreeItem to compare to this ModelTreeItem.
		 *	\return <span class='code'>true</span> if the objects reference different underlying data, <span class='code'>false</span> otherwise. */
		bool							operator!=(ModelTreeItem const & in_that) const;

		/*!	Check if the source ModelTreeItem points to the same underlying impl as this ModelTreeItem.
		 *	\param in_that The source ModelTreeItem to compare to this ModelTreeItem.
		 *	\return <span class='code'>true</span> if the objects reference the same underlying data, <span class='code'>false</span> otherwise. */
		bool							operator==(ModelTreeItem const & in_that) const;


		/*!	Gets the ModelTree associated with this ModelTreeItem.  This provides a means of accessing the derived ModelTree by this ModelTreeItem if necessary.
		 *	\return The ModelTree associated with this ModelTreeItem. */
		ModelTreePtr					GetTree() const;

		/*!	Gets the title for this ModelTreeItem.  This is the text that should be displayed for this ModelTreeItem in the ModelTree.
		 *	\return The title for this ModelTreeItem. */
		UTF8							GetTitle() const;

		/*!	Gets the item type for this ModelTreeItem.  This is used to determine what type of data this ModelTreeItem represents in the ModelTree.
		 *	\return The item type for this ModelTreeItem. */
		ModelTree::ItemType				GetItemType() const;

		/*!	Indicates whether this ModelTreeItem has any children.
		 *	\return <span class='code'>true</span> if this ModelTreeItem has children, <span class='code'>false</span> otherwise. */
		bool							HasChildren() const;


		/*!	This function is used to create a new ModelTreeItem to be the child of this ModelTreeItem. 
		*	You will need to implement this function in your class which derives from HCA::ModelTreeItem.
		*	This function will be invoked if this ComponentTreeItem has children and Expand has been invoked on it.
		*	This function should contain any necessary calls to GUI toolkits to create the appropriate GUI elements.
		*	Additionally, since this function requires the creation of a derived ModelTreeItem, that derived ModelTreeItem should use the base
		*	constructor which is appropriate for creating non-root items.
		*	\param in_node_id The node ID to be associated with the ModelTreeItem being added to the ModelTree. */
		virtual ModelTreeItemPtr		AddChild(int64_t in_node_id) = 0;


		/*!	This function should be invoked when this ModelTreeItem needs to expand.  If this function is overridden, the overriding function should invoke this base function.
		 *	Additionally, this function will always repopulate the children for this ModelTreeItem when invoked. */
		virtual void					Expand();

		/*!	This function should be invoked when this ModelTreeItem needs to collapse.  If this function is overridden, the overriding function should invoke this base function.
		 *	Additionally, this function will always release all children for this ModelTreeItem when invoked. */
		virtual void					Collapse();


		/*! Returns the HCA node ID associated with this ModelTreeItem 
		*	\return The HCA node ID associated with this ModelTreeItem . */
		uint64_t						GetNodeId() const;

		/*! Indicates whether this ModelTreeItem is currently hidden. 
		*	\return <span class='code'>true</span> if this ModelTreeItem is hidden, <span class='code'>false</span> otherwise. */
		bool							IsHidden() const;

		/*!	This function will be invoked when this item becomes shown or hidden. 
		*	If you want something to happen when an item becomes shown or hidden you should provide an implementation for this function
		*	in your class which derives from HCA::ModelTreeItem.
		*	A common usage for this function is to change the icon associated with the node to indicate to users whether the item is
		*	shown or hidden at a glance. */
		virtual void					OnShowHide() {};

		/*!	Hides this ModelTreeItem it isn't already. 
		*	Geometry associated with a hidden ModelTreeItem becomes invisible. 
		*	Calling this function will result in a call to OnShowHide after the hide operation has taken place. */
		void							Hide();

		/*!	Shows this ModelTreeItem it isn't already visible.
		*	Geometry associated with a shown ModelTreeItem becomes visible.
		*	Calling this function will result in a call to OnShowHide after the show operation has taken place. */
		void							Show();

		/*!	Shows this ModelTreeItem and hides every other nodes in the tree, causing this item to become the only one visible.
		*	The camera will zoom to the isolated item.
		*	Calling this function will result in a call to OnShowHide for the item which is isolated, after the isolate operation has taken place. */
		void							Isolate();

		/*! Smoothly zooms the camera to focus on this item. The transition lasts half a second. */
		void							Zoom();

		/*! Indicates whether this ModelTreeItem is currently expanded.
		*	\return <span class='code'>true</span> if this ModelTreeItem is expanded, <span class='code'>false</span> otherwise. */
		bool							IsExpanded() const;

		/*! Indicates whether this ModelTreeItem is currently highlighted.
		*	\return <span class='code'>true</span> if this ModelTreeItem is highlighted, <span class='code'>false</span> otherwise. */
		bool							IsHighlighted() const;

		/*!	This function will be invoked when this item becomes highlighted or unhighlighted. 
		*	If you want something to happen when an item's highlight state changes you should provide an implementation for this function
		*	in your class which derives from HCA::ModelTreeItem.
		*	A common usage for this function is to make the text associated with highlighted items bold to indicate to users whether the item is
		*	highlighted or not at a glance. */
		virtual void					OnHighlightUnhighlight();

		/*!	Highlights this ModelTreeItem using the options specified.
		*	Calling this function will result in a call to OnHighlightUnhighlight after the highlight operation has taken place. */
		void							Highlight(HighlightOptionsKit const & in_highlight_options);

		/*!	Removes all highlights from this ModelTreeItem.
		*	Calling this function will result in a call to OnHighlightUnhighlight after the unhighlight operation has taken place. */
		void							Unhighlight();

		/*	Requests properties for this ModelTreeItem
		*	When the request has been fulfilled, an event of type HCA::RequestIdEvent will be injected to the Database's Event Dispatcher.
		*	Registering a handler to HCA::RequestIdEvent will allow the user to know when this request has completed.
		*	The value of HCA::RequestIdEvent::request_id object will match out_requested_id, returned by this function.
		*	Properties requested with this function can be inspected through a call to HCA::Model::RetrieveNodeProperties after the request
		*	associated with out_request_id completes.
		*	\param out_request_id an ID which uniquely identifies this request
		*	\return <span class='code'>true</span> if the node's properties were gathered successfully, <span class='code'>false</span> otherwise. */
		bool							RequestProperties(uint32_t & out_request_id);
	};

private:
	//! Private default constructor to prevent instantiation.
	HCA();
};

}

#endif
