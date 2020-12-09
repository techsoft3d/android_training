// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_PARASOLID_H
#define SPRK_PARASOLID_H

#include "sprk.h"

#ifdef _MSC_VER
#ifndef STATIC_APP
#	ifdef SPRK_PARASOLID
#		define PARASOLID_API __declspec (dllexport)
#	else
#		define PARASOLID_API __declspec (dllimport)
#	endif
#endif
#else
#	include <stddef.h>
#	if defined(LINUX_SYSTEM) && defined(SPRK_PARASOLID)
#		ifndef STATIC_APP
#			define PARASOLID_API __attribute__ ((visibility ("default")))
#		endif
#	endif
#endif

#ifndef PARASOLID_API
#	define PARASOLID_API 
#endif

namespace HPS
{
	/*! The Parasolid class contains objects and enumerations used for importing and exporting CAD files via Parasolid. */
	class PARASOLID_API Parasolid
	{
	public:
		typedef int ParasolidEntity;		
		class Viewport;

		typedef HPS::IntArray													EntityArray;
		typedef std::vector<double, HPS::Allocator<double>>						DoubleArray;
		typedef std::vector<Viewport, HPS::Allocator<Viewport>>					ViewportArray;

		/*!	\enum TessellationLevel
		 *	Enumerates predefined tessellation levels used to import Parasolid files. */
		enum class TessellationLevel
		{
			ExtraLow,		//!< Extra Low tessellation setting
			Low,			//!< Low tessellation setting
			Medium,			//!< Medium tessellation setting
			High,			//!< High tessellation setting
			ExtraHigh,		//!< Extra High tessellation setting
			Custom,			//!< Custom tessellation setting. Chord and Angle values will need to be specified.
		};

		/*!	\enum Format
		 *	Enumerates the formats Parasolid can import. */
		enum class Format
		{
			Text,			//!< Text File
			Binary,			//!< Machine Dependent Binary File
			NeutralBinary,	//!< Machine Independent Binary File
		};

		/*!	\enum MismatchBehavior
			*	Enumerates the behaviors the Parasolid importer can assume when encountering attribute mismatches. */
		enum class MismatchBehavior
		{
			Ignore,			//!< Ignore attribute mismatches
			Fail,			//!< Fail on attribute mismatch
		};

		/*!	\enum CompoundBodyBehavior
			*	Enumerates the behaviors the Parasolid importer can assume when encountering compound bodies. */
		enum class CompoundBodyBehavior
		{
			Split,			//!< Split compound bodies into simple bodies
			Keep,			//!< Import compound bodies as-is
			Fail,			//!< Fail when encountering a compound body
		};

		class PARASOLID_API Viewport
		{
		public:
			Viewport();
			Viewport(DoubleArray in_box, DPoint in_location, DVector in_z_direction, DVector in_x_direction);
			void ShowViewport(DoubleArray & out_box, DPoint & out_location, DVector & out_z_direction, DVector & out_x_direction) const;

			bool						operator==(Viewport const & in_viewport) const;
			bool						operator!=(Viewport const & in_viewport) const;
			bool						Equals(Viewport const & in_viewport) const;

		private:
			DoubleArray box;				//min xyz, max xyz
			DPoint location;					
			DVector axis;					//local z-direction
			DVector reference_direction;	//local x-direction. must be orthogonal to axis.
		};

		/*!	\enum Ignore
		*  Enumerates the way the Parasolid importer chooses which facets or lines to ignore during tessellation.
		*  Corresponds to the PK_face_ignore_t struct. */
		enum class Ignore
		{
			Absolute,				//!< specify an absolute value
			FeatureToModelRatio,	//!< specify ratio of feature to model box
			FeatureToBodyRatio,		//!< specify ratio of feature to body box
		};

		class Facet
		{
		public:
			/*!	\enum Shape
			 *	Enumerates the way the Parasolid importer treats concave and convex facets during tessellation. 
			 *  Corresponds to the PK_face_shape_t struct. */
			enum class Shape
			{
				Any,		//!< Allow creation of holed or concaved facets
				Cut,		//!< Facets with holes are cut into concave facets
				Convex,		//!< Divide concave facets into convex faces
			};

			/*!	\enum Match
			 *	Enumerates the way the Parasolid importer treats facets near neighboring faces during tessellation. 
			 *  Corresponds to the PK_face_match_t struct. */
			enum class Match
			{
				Geometrical,	//!< Facets meet exactly but are topologically disjoint
				Topological,	//!< Facets meet exactly and have the same topology
				Trimmed,		//!< Facets may overlap or have gaps between them
			};

			/*!	\enum Density
			 *	Enumerates the way the Parasolid importer can alter facet density during tessellation.
			 *  Corresponds to the PK_face_density_t struct. */
			enum class Density
			{
				ViewIndependent,		//!< Density in independent of view
				Silhouette,				//!< Density increased around silhouettes
				Parallel,				//!< Density increased where facet normals are close to parallel with the view
				SilhouetteAndParallel,	//!< Density increased where facet normals are close to parallel with the view and near silhouettes
			};

			/*!	\enum Degeneracy
			 *	Enumerates the way the Parasolid importer treats degeneracies during tessellation.
			 *  Corresponds to the PK_face_degen_t struct. */
			enum class Degeneracy
			{
				MultipleVertices,		//!< output multiple vertices at degeneracies
				SingleVertex,			//!< output single vertex at degeneracies
				Average,				//!< output average parameters at degeneracies
			};

			/*!	\enum IncrementalFaceting
			 *	Enumerates the way the Parasolid importer handles incremental faceting during tessellation.
			 *  Corresponds to the PK_face_incr_t struct. */
			enum class IncrementalFaceting
			{
				Off,					//!< incremental faceting is not used
				OffNoAttributes,		//!< incremental faceting is not used, attributes are deleted
				OffRefreshAttributes,	//!< incremental faceting is not used, attributes are deleted and new ones are generated
				On,						//!< incremental faceting is used
			};

			class PARASOLID_API Tolerance
			{
			public:
				Tolerance();
				Tolerance(double curve_chord_tolerance, double curve_chord_maximum_length, double curve_chord_max_angle,
							double surface_tolerance, double surface_max_angle);

				void ShowTolerance(double & out_curve_chord_tolerance, double & out_curve_chord_maximum_length, double & out_curve_chord_max_angle,
									double & out_surface_tolerance, double & out_surface_max_angle) const;

				bool						operator==(Tolerance const & in_tolerance) const;
				bool						operator!=(Tolerance const & in_tolerance) const;
				bool						Equals(Tolerance const & in_tolerance) const;

			private:
				double curve_chord_tolerance;
				double curve_chord_maximum_length;
				double curve_chord_max_angle;
				double surface_tolerance;
				double surface_max_angle;
			};
		};

		class Line
		{
		public:
			class Hatching
			{
			public:
				enum class Planar
				{
					NoHatching,				//!< No hatching
					ByAttribute,			//!< Hatch faces which have planar hatch attributes
					ByVectorNoLocking,		//!< Hatch faces by vector. The plane position is not locked to a point
					ByVector,				//!< Hatch faces by vector.
				};

				enum class Radial
				{
					NoHatching,				//!< No hatching
					ByAttribute,			//!< Hatch faces which have radial hatch attributes
					ByVectorNoLocking,		//!< Hatch faces by vector. The radial position is not locked to a point
					ByVector,				//!< Hatch faces by vector.
				};

				enum class Parametric
				{
					NoHatching,				//!< No hatching
					ByAttribute,			//!< Hatch faces which have parametric hatch attributes
					ByVectorNoLocking,		//!< Hatch faces by vector. The parametric position is not locked to a point
					ByVector,				//!< Hatch faces by vector.
				};
			};

			enum class UnfixedBlends
			{
				Draw,						//!< No rendering of unfixed blends
				DoNotDraw,					//!< Unfixed blends are rendered
				ByAttribute,				//!< Unfixed blends are rendered as defined by the blend attribute
			};

			enum class Visibility
			{
				DoNotEvaluate,				//!< Visibility information is not evaluated
				NoHidden,					//!< Hidden lines are not drawn
				MarkHiddenAsInvisible,		//!< Hidden lines are drawn and marked as invisible
				Draft,						//!< Hidden lines and distinguish between those hidden by lines and those hidden by faces
				Custom,						//!< Evaluate visibility based on the options passed to LineTessellationKit::SetVisibilityEvaluation()
			};

			enum class Smooth
			{
				Indicate,					//!< Indicate whether edges are smooth.
				DoNotIndicate,				//!< Do not indicate whether edges are smooth.
				Draft,						//!< Indicate whether edges are smooth, and also whether they are coincident with other lines.
			};

			enum class Region
			{
				Output,						//!< Output regional data for all edges and silhouettes
				DoNotOutput,				//!< Do not output regional data
				ByAttribute,				//!< Output regional data for silhouettes and edges bounding faces with the regional attribute
			};

			enum class Hierarchical
			{
				On,							//!< Do not produce hierarchical output
				Off,						//!< Hierarchical output
				NoGeometry,					//!< Hierarchical output - no geometry
				Parameterized,				//!< Hierarchical output parameterised
			};

			enum class BCurveRendering
			{
				Polyline,					//!< Output bcurves as polylines
				Bezier,						//!< Output bcurves in Bezier form
				NURBS,						//!< Output bcurves in NURBS form
			};

			enum class OverlappingBehavior
			{
				DoNotAllow,					//!< Do not allow for overlapping instances
				Allow,						//!< Allow for overlapping instances
				AllowAndIntersectAll,		//!< Allow for overlapping instances, and clash all curves
				AllowAndIntersectPairs,		//!< Allow for overlapping instances, and clash pairs of curves
			};
		};

		typedef std::vector<Facet::Tolerance, HPS::Allocator<Facet::Tolerance>> ToleranceArray;
		class FacetTessellationKit;
		class LineTessellationKit;

		/*! The Component class is a smart pointer.  It represents a variety of Parasolid components.  
		 *  The primary purpose of this class is simply to provide access to the underlying ParasolidEntity object
		 *	for use by Parasolid library functions. */
		class PARASOLID_API Component : public HPS::Component
		{
		public:
			/*! The default constructor creates an uninitialized Parasolid::Component object.  The Type() function will return Type::None. */
			Component();

			/*! This constructor creates an Parasolid::Component object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
			 *	an upcast of a Parasolid::Component object.  Otherwise the copy will fail and the resulting Parasolid::Component will be invalid.
			 *	\param in_that The source Component to copy. */
			Component(HPS::Component const & in_that);

			/*! The copy constructor creates an Parasolid::Component object that shares the underlying smart-pointer of the source Parasolid::Component.
			 *	\param in_that The source Parasolid::Component to copy. */
			Component(Parasolid::Component const & in_that);

			/*! The move constructor creates a Parasolid::Component by transferring the underlying object of the rvalue reference to this Parasolid::Component.
			 * 	\param in_that An rvalue reference to an Parasolid::Component to take the underlying object from. */
			Component(Parasolid::Component && in_that);

			virtual ~Component();

			static const HPS::Type staticType = HPS::Type::ParasolidComponent;
			HPS::Type				ObjectType() const { return staticType; }

#ifndef _MSC_VER
			Component &				operator=(Component const & in_that) = default;
#endif

			/*! The move assignment operator transfers the underlying object of the rvalue reference to this Parasolid::Component.
			 *	\param in_that An rvalue reference to a Parasolid::Component to take the underlying object from.
			 *	\return A reference to this Parasolid::Component. */
			Component &				operator=(Parasolid::Component && in_that);


			/*! Gets the ParasolidEntity object corresponding to this Parasolid Component.
			 *	\return The ParasolidEntity object corresponding to this Parasolid Component. */
			ParasolidEntity 			GetParasolidEntity() const;

			/*! Re-tessellate the given Component with the provided options.  
			 *  Tessellation can only occur at the body, instance or assembly level, so calling this function on
			 *	Component objects below that level will have no effect.  
			 *	\param in_facet_options The facet tessellation options to use to generate the new tessellation for this Component. 
			 *  \param in_line_options The line tessellation options to use to generate the new tessellation for this Component.*/
			void					Tessellate(Parasolid::FacetTessellationKit const & in_facet_options, Parasolid::LineTessellationKit const & in_line_options);

			/*!  Applies a transform to the underlying Parasolid data associated with this component, replacing any transform currently set on it.
			*   Only components of type ParasolidAssembly, ParasolidInstance and ParasolidTopoBody can have transforms applied to them. Additionally, only transformations which are rigid motions can be applied. Calling SetTransform with a transform which is not a rigid motion, such as a shear, will cause the function to fail.
			*	\param in_transform The transform to apply to this component.
			*	\return <span class='code'>true</span> if transform was applied correctly, <span class='code'>false</span> otherwise. */
			bool					SetTransform(MatrixKit const & in_transform);

			/*! Appends a transform to the underlying Parasolid data associated with this component.
			*   Only components of type ParasolidAssembly, ParasolidInstance and ParasolidTopoBody can have transforms applied to them. Additionally, only transformations which are rigid motions can be applied. Calling AddTransform with a transform which is not a rigid motion, such as a shear, will cause the function to fail.
			*	\param in_transform The transform to apply to this component.
			*	\return <span class='code'>true</span> if transform was applied correctly, <span class='code'>false</span> otherwise. */
			bool					AddTransform(MatrixKit const & in_transform);
		};

		typedef std::vector<Component, HPS::Allocator<Component>> ComponentArray;

		/*! The CADModel class is a smart pointer.  It is an abstract concept symbolizing the root of the Parasolid import.
		 *  It is not related to any Parasolid object. */
		class PARASOLID_API CADModel : public HPS::CADModel
		{
		public:
			/*! The default constructor creates an uninitialized CADModel object.  The Type() function will return Type::None. */
			CADModel();

			/*! This constructor creates a CADModel object that shares the underlying smart-pointer of the source Component.  
			 *  The copy will only be successful if the source component is really
			 *	an upcast of a Parasolid::CADModel object.  Otherwise the copy will fail and the resulting Parasolid::CADModel will be invalid.
			 *	\param in_that The source Component to copy. */
			CADModel(Component const & in_that);

			/*! This constructor creates a Parasolid::CADModel object that shares the underlying smart-pointer of the source HPS::CADModel.  
			 *  The copy will only be successful if the source component is really
			 *	an upcast of a Parasolid::CADModel object.  Otherwise the copy will fail and the resulting Parasolid::CADModel will be invalid.
			 *	\param in_that The source HPS::CADModel to copy. */
			CADModel(HPS::CADModel const & in_that);

			/*! The copy constructor creates a Parasolid::CADModel object that shares the underlying smart-pointer of the source Parasolid::CADModel.
			 *	\param in_that The source Parasolid::CADModel to copy. */
			CADModel(Parasolid::CADModel const & in_that);

			/*! The move constructor creates a Parasolid::CADModel by transferring the underlying object of the rvalue reference to this Parasolid::CADModel.
			 * 	\param in_that An rvalue reference to a Parasolid::CADModel to take the underlying object from. */
			CADModel(Parasolid::CADModel && in_that);

			virtual ~CADModel();

			static const HPS::Type staticType = HPS::Type::ParasolidCADModel;
			HPS::Type				ObjectType() const { return staticType; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
			CADModel &				operator=(Parasolid::CADModel const & in_that) = default;
#endif

			/*! The move assignment operator transfers the underlying object of the rvalue reference to this Parasolid::CADModel.
			 *	\param in_that An rvalue reference to a Parasolid::CADModel to take the underlying object from.
			 *	\return A reference to this Parasolid::CADModel. */
			CADModel &				operator=(Parasolid::CADModel && in_that);

			/*! Returns the Component associated with the provided entity. 
			 *	\param in_entity The Parasolid entity for which to return a Component.
			 *	\return The component associated with the passed in entity, or an empty component if no component is associated with in_entity. */
			Component				GetComponentFromEntity(ParasolidEntity in_entity);

			/*! Renders the entity specified, according to the tessellation options provided.
			 *	Only entities of type PK_CLASS_assembly and PK_CLASS_body can be used with this function.
			 *	\param in_entity_to_add The Parasolid entity to import into HPS.
			 *	\param in_facet_tessellation The facet tessellation parameters used during import.
			 *	\param in_line_tessellation The line tessellation parameters used during import.
			 *	\param in_owner The component owning which will own the imported entity
			 *	\return The component created as part of the import of the provided entity. */
			Component				AddEntity(ParasolidEntity in_entity_to_add, FacetTessellationKit const & in_facet_tessellation, LineTessellationKit const & in_line_tessellation, Component const & in_owner = Component());

			/*! Re-tessellate the given CADModel with the provided options.  
			 *	\param in_facet_options The facet tessellation options to use to generate the new tessellation for this CADModel. 
			 *  \param in_line_options The line tessellation options to use to generate the new tessellation for this CADModel.*/
			void					Tessellate(Parasolid::FacetTessellationKit const & in_facet_options, Parasolid::LineTessellationKit const & in_line_options);
		};

		/*! The Factory class is used to create Parasolid objects which inherit from HPS::Component or one of its subclasses. */
		class PARASOLID_API Factory : public Sprocket
		{
		public:
			/*! Creates a new Parasolid::Component as a subcomponent of a given Component.
			 *	\param in_owner The HPS::Component which owns the newly created Parasolid::Component.
			 *	\param in_type The type of Parasolid::Component to create.
			 *	\param in_entity The Parasolid entity associated with the newly created Parasolid::Component. */
			static Component	CreateComponent(HPS::Component const & in_owner, HPS::Component::ComponentType in_type, ParasolidEntity in_entity = 0);

			/*! Creates a new Parasolid::CADModel.
			 *	\param in_model The Model associated with the Parasolid::CADModel. */
			static CADModel		CreateCADModel(Model const & in_model = HPS::Factory::CreateModel());

			/*! Returns the component at whose level changes can be made without influencing other instances of the same component.
			 *  For Parasolid components this function is only meaningful when in_component_path contains at least one component
			 *  of type ParasolidInstance or ParasolidTopoBody. If none of these types are present in in_component_path
			 *  a warning will be logged and an empty component will be returned.
			 * \param in_component_path The ComponentPath to operate on.
			 * \return The newly de-instanced Component. */
			static Component	DeInstanceComponent(ComponentPath const & in_component_path);

		private:
			//! Private default constructor to prevent instantiation.
			Factory();
		};

		/*! The ImportNotifier class is a smart-pointer that is tied to a file import.  It is used to interact with an ongoing import or get the results from a completed import. */
		class PARASOLID_API ImportNotifier : public IONotifier
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

			static const HPS::Type staticType = HPS::Type::ParasolidImportNotifier;
			HPS::Type				ObjectType() const { return staticType; };

			/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
			 *	\param in_that The source ImportNotifier for the assignment.
			 *	\return A reference to this ImportNotifier. */
			ImportNotifier &		operator=(ImportNotifier const & in_that);

			/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
			 *	\param in_that The source ImportNotifier for the assignment. */
			void					Assign(ImportNotifier const & in_that);

			/*!	Get the CADModel for the file import.  Throws an IOException if the import is not complete, was not successful or was canceled.
			 *  \return The CADModel for a successful file import. */
			Parasolid::CADModel		GetCADModel() const;

			/*!	Get the number of milliseconds it took Parasolid to read the file into memory.
			 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
			Time					GetImportTime() const;

			/*!	Get the number of milliseconds it took Visualize to parse the Parasolid data and create the corresponding scene graph.
			 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
			Time					GetParseTime() const;
		};

		class ImportOptionsKit;
		class ExportOptionsKit;

		/*!	The File class provides functions to import and export CAD files via the Parasolid interface. */
		class PARASOLID_API File
		{
		public:

			/*!	Performs an asynchronous import of the specified CAD file with the provided options.  May throw an IOException prior to starting the asynchronous import.
			 * 	\param in_file_name The name of the CAD file to import.
			 *	\param in_options The options controlling the import of the CAD file.
			 *	\return An ImportNotfier object that can be used to query the import progress and status. */
			static ImportNotifier Import(char const * in_file_name, ImportOptionsKit const & in_options);

			/*!	Performs an asynchronous import of the specified entities.  May throw an IOException prior to starting the asynchronous import.
			 * 	\param in_parts An array of PK_PART_t entities to import.
			 *	\param in_facet_options The options controlling the import of facets in the CAD file.
			 *	\param in_line_options The options controlling the import of lines in the CAD file.
			 *	\return An ImportNotfier object that can be used to query the import progress and status. */
			static ImportNotifier Import(EntityArray const & in_parts, FacetTessellationKit const & in_facet_options, LineTessellationKit const & in_line_options);

			/*! Performs a synchronous export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
			 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Parasolid, if it did not, the export will not succeed.
			 * 	\param in_file_name The name of the file to write the Parasolid data to.
			 * 	\param in_options The options controlling the export of the Parasolid data. */
			static void Export(CADModel const & in_cad_model, char const * in_file_name, ExportOptionsKit const & in_options);

		private:
			//! Private default constructor to prevent instantiation.
			File();
		};

		/*! The ImportOptionsKit class is a user space object.  It contains settings controlling what and how data is imported via the Parasolid interface. Calling HPS::Parasolid::ImportOptionsKit::GetDefault() will return an options kit with values found in <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-io">here.</a> */
		class PARASOLID_API ImportOptionsKit : public SprocketKit
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

			static const HPS::Type staticType = HPS::Type::ParasolidImportOptionsKit;
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

			/*! Sets the format type to be imported.  
			 *  This corresponds to the value that will be passed to PK_PART_receive_o_t.transmit_format.
			 *	\param in_format The format of the file to be imported.
			 *	\return A reference to this ImportOptionsKit. 
			 *  \sa <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-import-format">Default value</a>
			 */
			ImportOptionsKit &			SetFormat(Parasolid::Format in_format);

			/*! Sets whether to import user fields from the CAD file.  
			 *  This corresponds to the value that will be passed to PK_PART_receive_o_t.receive_user_fields.
			 *	\param in_import_user_fields Whether to import user fields from the CAD file.
			 *	\return A reference to this ImportOptionsKit. 
			 *  \sa <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-import-user-fields">Default value</a>
			 */
			ImportOptionsKit &			SetUserFields(bool in_import_user_fields);

			/*! Sets the importer behavior for mismatched attributes.  
			 *  This corresponds to the value that will be passed to PK_PART_receive_o_t.attdef_mismatch.
			 *	\param in_behavior The behavior of the importer when mismatched attributes are found.
			 *	\return A reference to this ImportOptionsKit. 
			 *  \sa <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-import-attr-mismatch">Default value</a>
			 */
			ImportOptionsKit &			SetAttributeMismatchBehavior(Parasolid::MismatchBehavior in_behavior);

			/*! Sets the importer behavior for compound bodies.  
			 *  This corresponds to the value that will be passed to PK_PART_receive_o_t.receive_compound.
			 *	\param in_behavior The behavior of the importer when compound bodies are found.
			 *	\return A reference to this ImportOptionsKit. 
			 *  \sa <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-import-compound-body-beh">Default value</a>
			 */
			ImportOptionsKit &			SetCompoundBodyBehavior(Parasolid::CompoundBodyBehavior in_behavior);

			/*! Sets the facet tessellation settings used during import
			 *  This corresponds to the PK_TOPOL_render_facet_o_t struct.
			 *	\param in_facet_tessellation_kit The facet tessellation settings used during import
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			SetFacetTessellation(Parasolid::FacetTessellationKit const & in_facet_tessellation_kit);

			/*! Sets the line tessellation settings used during import
			 *  This corresponds to the PK_TOPOL_render_line_o_t struct.
			 *	\param in_facet_tessellation_kit The line tessellation settings used during import
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			SetLineTessellation(Parasolid::LineTessellationKit const & in_line_tessellation_kit);

			/*! Sets where to insert the imported file in an existing Component hierarchy.  If this option is not set, a new CADModel will be created
			*	for the imported file.  If this option is specified, the underlying Parasolid data of the existing CADModel will be modified to include the imported file
			*	and the data will be imported into the specified location in the existing Component hierarchy and its corresponding scene graph.
			*	The file will be imported in the same PK_PARTITION_t as the one to which entities in in_path belong.
			*	\param in_path The path describing the location to insert the imported file into.  This path must either terminate at a CADModel or a Component
			*		corresponding to a Parasolid Assembly.  Note that if the imported file is added along a ComponentPath with attributes specified in
			*		the Parasolid data, the components added to the existing Parasolid data will also inherit those attributes.
			*	\param in_transform A transform to apply to the components being added to the existing CADModel.  This transform will also be added to the underlying
			*		Parasolid data.  Valid transformations are limited to translations, rotations, uniform scaling and reflection. 
			*		If the transform does not conform to these limitations it will be ignored. Defaults to an identity transform.
			*	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			SetLocation(ComponentPath const & in_path, MatrixKit const & in_transform = MatrixKit());


			/*! Removes the format import state.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetFormat();

			/*! Removes the user fields import state.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetUserFields();

			/*! Removes the attribute mismatch behavior state.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetAttributeMismatchBehavior();

			/*! Removes the compound body behavior state.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetCompoundBodyBehavior();

			/*! Removes the facet tessellation state.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetFacetTessellation();

			/*! Removes the line tessellation state.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetLineTessellation();

			/*! Removes the location state.
			*	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetLocation();

			/*! Removes all settings from this ImportOptionsKit.
			 *	\return A reference to this ImportOptionsKit. */
			ImportOptionsKit &			UnsetEverything();

			
			/*! Shows the format import setting.
			 *	\param out_format The format setting for this import.
			 *	\return <span class='code'>true</span> if a format setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowFormat(Parasolid::Format & out_format) const;

			/*! Shows the user fields import setting.
			 *	\param out_user_fields The user fields setting for this import.
			 *	\return <span class='code'>true</span> if a user fields setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowUserFields(bool & out_user_fields) const;

			/*! Shows the attribute mismatch behavior import setting.
			 *	\param out_behavior The attribute mismatch behavior setting for this import.
			 *	\return <span class='code'>true</span> if an attribute mismatch behavior setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowAttributeMismatchBehavior(Parasolid::MismatchBehavior & out_behavior) const;

			/*! Shows the compound body behavior import setting.
			 *	\param out_behavior The compound body behavior setting for this import.
			 *	\return <span class='code'>true</span> if a compound body behavior setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowCompoundBodyBehavior(Parasolid::CompoundBodyBehavior & out_behavior) const;

			/*! Shows the facet tessellation import setting.
			 *	\param out_facet_tessellation The facet tessellation setting for this import.
			 *	\return <span class='code'>true</span> if a facet tessellation setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowFacetTessellation(Parasolid::FacetTessellationKit & out_facet_tessellation) const;

			/*! Shows the line tessellation import setting.
			 *	\param out_line_tessellation The line tessellation setting for this import.
			 *	\return <span class='code'>true</span> if a line tessellation setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowLineTessellation(Parasolid::LineTessellationKit & out_line_tessellation) const;

			/*! Shows the location setting.
			*	\param out_path The path describing the location to insert the imported file into.
			*	\param out_transform The transform to apply to the components being added to the existing CADModel.
			*	\return <span class='code'>true</span> if a location setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowLocation(ComponentPath & out_path, MatrixKit & out_transformation) const;

		};

		/*! The FacetTessellationKit class is a user space object.  It contains settings controlling facet tessellation for Parasolid.
		 *  It corresponds to the PK_TOPOL_render_facet_o_t struct. */
		class PARASOLID_API FacetTessellationKit : public SprocketKit
		{
		public:
			/*! The default constructor creates an empty FacetTessellationKit object. */
			FacetTessellationKit();

			/*! The copy constructor creates a new FacetTessellationKit object that contains the same settings as the source FacetTessellationKit.
			 * 	\param in_kit The source FacetTessellationKit to copy. */
			FacetTessellationKit(FacetTessellationKit const & in_kit);

			/*! The move constructor creates an FacetTessellationKit by transferring the underlying impl of the rvalue reference to this FacetTessellationKit
			 * 	thereby avoiding a copy and allocation.
			 * 	\param in_that An rvalue reference to an FacetTessellationKit to take the impl from. */
			FacetTessellationKit(FacetTessellationKit && in_that);

			/*! The move assignment operator transfers the underlying impl of the rvalue reference to this FacetTessellationKit thereby avoiding a copy.
			 * 	\param in_that An rvalue reference to an FacetTessellationKit to take the impl from.
			 * 	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit & operator=(FacetTessellationKit && in_that);

			virtual ~FacetTessellationKit();

			static const HPS::Type staticType = HPS::Type::ParasolidFacetTessellationKit;
			HPS::Type					ObjectType() const { return staticType; }

			/*! Creates an FacetTessellationKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
			 *	where it is reasonable to have a default.  These values will be used for facet tessellation unless an option is overridden by the options passed to File::Import.
			 *	\return An FacetTessellationKit with the default settings. */
			static FacetTessellationKit		GetDefault();

			/*! Copies the source FacetTessellationKit into this FacetTessellationKit.
			 * 	\param in_kit The source FacetTessellationKit to copy. */
			void						Set(FacetTessellationKit const & in_kit);

			/*! Copies this FacetTessellationKit into the given FacetTessellationKit.
			 * 	\param out_kit The FacetTessellationKit to populate with the contents of this FacetTessellationKit. */
			void						Show(FacetTessellationKit & out_kit) const;

			/*! Copies the source FacetTessellationKit into this FacetTessellationKit.
			 * 	\param in_kit The source FacetTessellationKit to copy.
			 * 	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &			operator=(FacetTessellationKit const & in_kit);

			/*! Indicates whether this FacetTessellationKit has any values set on it.
			 * 	\return <span class='code'>true</span> if no values are set on this FacetTessellationKit, <span class='code'>false</span> otherwise. */
			bool						Empty() const;

			/*!	Check if the source FacetTessellationKit is equivalent to this FacetTessellationKit. 
			 *	\param in_kit The source FacetTessellationKit to compare to this FacetTessellationKit.
			 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
			bool						Equals(FacetTessellationKit const & in_kit) const;

			/*!	Check if the source FacetTessellationKit is equivalent to this FacetTessellationKit. 
			 *	\param in_kit The source FacetTessellationKit to compare to this FacetTessellationKit.
			 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
			bool						operator==(FacetTessellationKit const & in_kit) const;

			/*!	Check if the source FacetTessellationKit is not equivalent to this FacetTessellationKit. 
			 *	\param in_kit The source FacetTessellationKit to compare to this FacetTessellationKit.
			 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
			bool						operator!=(FacetTessellationKit const & in_kit) const;


			/*! Sets facet tessellation level.  
			 *  This function can optionally compute the best values for line tessellation based on the values passed in.
			 *  Doing so is enabled by default and will result in lines which smoothly match their respective faces.
			 *	\param in_tessellation_level The tessellation level used for importing facets.
			 *	\param in_chord The chord tolerance. Only considered if in_tessellation_level is set to Custom.
			 *	\param in_angle The angle tolerance. Only considered if in_tessellation_level is set to Custom.
			 *	\param in_generate_line_tessellation Automatically figure out the best parameters for line tessellation based on facet tessellation levels. True by default.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetTessellationLevel(HPS::Parasolid::TessellationLevel in_tessellation_level, double in_chord = -1, double in_angle = -1, bool in_generate_line_tessellation = true);

			/*! Sets constraints on the facet generation.  
			 *  This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.shape.
			 *	\param in_shape The behavior used during tessellation for handling concave, convex and holed entities.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetShapeGeneration(Facet::Shape in_shape);

			/*! Sets the vertex matching strategy at edges
			 *  This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.match.
			 *	\param in_vertex_matching The behavior used during tessellation for handling facet meshes between neighboring faces.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetVertexMatching(Facet::Match in_vertex_matching);

			/*! Sets the density settings.
			 *	\param in_density The density settings. This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.density.
			 *	\param in_density_tolerance The density tolerance. Must be specified is in_density is not ViewIndependent. This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.local_density_tol.
			 *	\param in_density_max_angle The density tolerance maximum angle. This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.local_density_ang.
			 *	\param in_view_directions An array of view directions. Must be specified is in_density is not ViewIndependent. This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.view_directions.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetDensity(Facet::Density in_density, double in_density_tolerance = 0, double in_density_max_angle = 0, DVectorArray const & in_view_directions = DVectorArray());

			/*! Sets which loops should be ignored while faceting.
			 *  This corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.loops.
			 *	\param in_ignored_loops loops ignored during faceting. Each entry in the vector correspond to a PK_LOOP_t object
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetIgnoredLoops(IntArray & in_ignored_loops);

			/*! Sets face size constraints.
			 *	\param in_minimum_width minimum facet width. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.min_facet_width.
			 *	\param in_maximum_width minimum facet width. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.max_facet_width.
			 *	\param in_maximum_sides maximum facet sides. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.max_facet_sides.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetFacetSize(double in_minimum_width, double in_maximum_width, int in_maximum_sides);

			/*! Sets chord tolerance.
			 *	\param in_chord_tolerance curve chordal tolerance between a curve and its facet edges. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.curve_chord_tol.
			 *	\param in_maximum_chord_length maximum chord length. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.curve_chord_max.
			 *	\param in_maximum_chord_angle maximum chord angle, in radians. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.curve_chord_ang.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetChordTolerance(double in_chord_tolerance, double in_maximum_chord_length, double in_maximum_chord_angle);

			/*! Sets tolerance between surfaces and corresponding facets.
			 *	\param in_plane_tolerance distance tolerance between surface and its facet. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.surface_plane_tol.
			 *	\param in_max_plane_angle angular tolerance between surface and its facet, in radians. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.surface_plane_ang.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetSurfacePlaneTolerance(double in_plane_tolerance, double in_max_plane_angle);

			/*! Sets tolerance between facet and mid-plane.
			 *	\param in_plane_tolerance distance tolerance between facet and mid-plane. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.facet_plane_tol.
			 *	\param in_max_plane_angle angular tolerance between facet and mid-plane, in radians. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.facet_plane_ang.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetFacetPlaneTolerance(double in_plane_tolerance, double in_max_plane_angle);

			/*! Sets the behavior the import assumes during tessellation when encountering a degeneracy.
			 *	\param in_degeneracy_behavior describes how vertices are handled at degeneracies. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.degen.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetDegeneracyBehavior(Facet::Degeneracy in_degeneracy_behavior);

			/*! Sets specific tolerances for a set of topologies.
			 *	\param in_tolerance_array array of tolerance specifications. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.local_tols.
			 *	\param in_topology_array array of topologies for which a specific tolerance will be applied. Each entry in the vector correspond to a PK_TOPOL_t object. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.topols_with_local_tols.
			 *	\param in_tolerance_for_tolopoly indexes connecting each element of the topology array to a tolerance. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.local_tols_for_topols.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetLocalTolerances(ToleranceArray const & in_tolerance_array, IntArray const & in_topology_array, IntArray const & in_tolerance_for_tolopoly);

			/*! Sets criteria for ignoring facets.
			 *	\param in_ignore whether any facet should be ignored.
			 *	\param in_ignore_criteria specifies how to interpret the minimum_value parameter. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.ignore.
			 *	\param in_minimum_value minimum value for a facet not to be ignored. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.ignore_value.
			 *	\param in_treat_faces_individually whether facets should be considered individually or as part of the face's owning body for ignore calculations. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.ignore_scope.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetIgnoreCriteria(bool in_ignore, Ignore in_ignore_criteria = Ignore::Absolute, double in_minimum_value = 0, bool in_treat_faces_individually = false);

			/*! Sets whether wire edges should be taken into account while faceting.
			 *	\param in_wire_edgeswhether wire edges should be taken into account while faceting. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.wire_edges.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetWireEdges(bool in_wire_edges);

			/*! Sets whether incremental faceting should be used.
			 *	\param in_faceting sets the incremental faceting behavior. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.incremental_facetting.
			 *	\param refine_tessellation whether to refine facets. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.incremental_refinement.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetIncrementalFaceting(Facet::IncrementalFaceting in_faceting, bool refine_tessellation = false);

			/*! Sets whether facets around points of inflections may be further refined.
			 *	\param in_refine whether facets around points of inflections may be further refined. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.inflect.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetInflection(bool in_refine);

			/*! Sets whether extra checks should be performed to improve the quality of facets.
			 *	\param in_improved_quality whether extra checks should be performed to improve the quality of facets. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.quality.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetImprovedFacetQuality(bool in_improved_quality);

			/*! Sets whether vertices should be forced against tolerance edges.
			 *	\param in_flatten_vertices whether vertices should be forced against tolerance edges. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.vertices_on_planar.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetFlattenVertices(bool in_flatten_vertices);

			/*! Sets whether to take into account faces which are offsets of one another.
			 *	\param in_offset_faces whether to take into account faces which are offsets of one another. Corresponds to the value that will be passed to PK_TOPOL_facet_mesh_o_t.respect_offset.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetOffsetFaces(bool in_offset_faces);

			/*! Sets whether to output surface normals.
			 *	\param in_normals whether to output surface normals. Corresponds to the value that will be passed to PK_TOPOL_render_facet_go_o_t.go_normals.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetNormals(bool in_normals);

			/*! Sets whether to output surface parameters.
			 *	\param in_parameters whether to output surface parameters. Corresponds to the value that will be passed to PK_TOPOL_render_facet_go_o_t.go_parameters.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetParameters(bool in_parameters);

			/*! Sets whether to output edges.
			 *	\param in_edges whether to output edges. Corresponds to the value that will be passed to PK_TOPOL_render_facet_go_o_t.go_edges.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetEdges(bool in_edges);

			/*! Sets output settings for strips
			 *	\param in_strips whether to output facets in strips. Corresponds to the value that will be passed to PK_TOPOL_render_facet_go_o_t.go_strips.
			 *	\param in_max_facets_per_strip maximum number of facets per strips. Corresponds to the value that will be passed to PK_TOPOL_render_facet_go_o_t.go_max_facets_per_strip.
			 *	\param in_split_strips whether to split facet strips. Corresponds to the value that will be passed to PK_TOPOL_render_facet_go_o_t.split_strips.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		SetStrips(bool in_strips, int in_max_facets_per_strip, bool in_split_strips);

			/*! Removes the tessellation level state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetTessellationLevel();

			/*! Removes the shape generation tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetShapeGeneration();

			/*! Removes the vertex matching tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetVertexMatching();

			/*! Removes the density tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetDensity();

			/*! Removes the ignored loops tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetIgnoredLoops();

			/*! Removes the facet size tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetFacetSize();

			/*! Removes the chord tolerance tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetChordTolerance();

			/*! Removes the plane tolerance tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetSurfacePlaneTolerance();

			/*! Removes the facet plane tolerance tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetFacetPlaneTolerance();

			/*! Removes the degeneracy tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetDegeneracyBehavior();

			/*! Removes the local tolerances tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetLocalTolerances();

			/*! Removes the ignore criteria tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetIgnoreCriteria();

			/*! Removes the wire edges tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetWireEdges();

			/*! Removes the incremental faceting tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetIncrementalFaceting();

			/*! Removes the inflection tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetInflection();

			/*! Removes the facet quality tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetImprovedFacetQuality();

			/*! Removes the vertex flattening tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetFlattenVertices();

			/*! Removes the offset faces tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetOffsetFaces();

			/*! Removes the normals tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetNormals();

			/*! Removes the parameters tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetParameters();

			/*! Removes the edges tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetEdges();

			/*! Removes the strip tessellation state.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetStrips();

			/*! Removes all settings from this FacetTessellationKit.
			 *	\return A reference to this FacetTessellationKit. */
			FacetTessellationKit &		UnsetEverything();


			/*! Shows the shape generation tessellation setting.
			 *	\param out_tessellation_level The tessellation level used for this import
			 *	\param out_chord The chord tolerance. Only relevant if out_tessellation_level is Custom.
			 *	\param out_angle The angle tolerance. Only relevant if out_tessellation_level is Custom.
			 *	\param out_generate_line_tessellation Whether line tessellation levels are automatically generated based on facet tessellation level.
			 *	\return <span class='code'>true</span> if a shape generation setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowTessellationLevel(TessellationLevel & out_tessellation_level, double & out_chord, double & out_angle, bool & out_generate_line_tessellation) const;

			/*! Shows the shape generation tessellation setting.
			 *	\param out_shape The shape generation setting for this kit.
			 *	\return <span class='code'>true</span> if a shape generation setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowShapeGeneration(Facet::Shape & out_shape) const;

			/*! Shows the vertex matching tessellation setting.
			 *	\param out_vertex_matching The vertex matching tessellation setting for this kit.
			 *	\return <span class='code'>true</span> if a vertex matching tessellation setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowVertexMatching(Facet::Match & out_vertex_matching) const;

			/*! Shows the density tessellation setting.
			 *	\param out_density The density setting for this kit.
			 *	\param out_density_tolerance The density tolerance.
			 *	\param out_density_max_angle The density tolerance maximum angle.
			 *	\param out_view_directions An array of view directions.
			 *	\return <span class='code'>true</span> if a density setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowDensity(Facet::Density & out_density, double & out_density_tolerance, double & out_density_max_angle, DVectorArray & out_view_directions) const;

			/*! Shows the ignored loops tessellation setting.
			 *	\param out_ignored_loops The ignored loops setting for this kit.
			 *	\return <span class='code'>true</span> if a ignored loops setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowIgnoredLoops(IntArray & out_ignored_loops) const;

			/*! Shows the facet size tessellation setting.
			 *	\param out_minimum_width The minimum facet width setting for this kit.
			 *	\param out_maximum_width The maximum facet width setting for this kit.
			 *	\param out_maximum_sides The maximum facet side setting for this kit.
			 *	\return <span class='code'>true</span> if a facet size setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowFacetSize(double & out_minimum_width, double & out_maximum_width, int & out_maximum_sides) const;

			/*! Shows the chord tolerance tessellation setting.
			 *	\param out_chord_tolerance The tolerance setting for this kit.
			 *	\param out_maximum_chord_length The maximum chord length setting for this kit.
			 *	\param out_maximum_chord_angle The maximum chord angle setting for this kit.
			 *	\return <span class='code'>true</span> if a chord tolerance setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowChordTolerance(double & out_chord_tolerance, double & out_maximum_chord_length, double & out_maximum_chord_angle) const;

			/*! Shows the surface plane tolerance tessellation setting.
			 *	\param out_plane_tolerance The tolerance setting for this kit.
			 *	\param out_max_plane_angle The maximum angle setting for this kit.
			 *	\return <span class='code'>true</span> if a surface plane tolerance setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowSurfacePlaneTolerance(double & out_plane_tolerance, double & out_max_plane_angle) const;

			/*! Shows the facet plane tolerance tessellation setting.
			 *	\param out_plane_tolerance The tolerance setting for this kit.
			 *	\param out_max_plane_angle The maximum angle setting for this kit.
			 *	\return <span class='code'>true</span> if a facet plane tolerance setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowFacetPlaneTolerance(double & out_plane_tolerance, double & out_max_plane_angle) const;

			/*! Shows the degeneracy tessellation setting.
			 *	\param out_degeneracy_behavior The degeneracy setting for this kit.
			 *	\return <span class='code'>true</span> if a degeneracy setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowDegeneracyBehavior(Facet::Degeneracy & out_degeneracy_behavior) const;

			/*! Shows the local tolerance tessellation setting.
			 *	\param out_tolerance_array The local tolerance array for this kit.
			 *	\param out_topology_array The topology array for this kit.
			 *	\param out_tolerance_for_tolopoly The index array for this kit.
			 *	\return <span class='code'>true</span> if a local tolerance setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowLocalTolerances(ToleranceArray & out_tolerance_array, IntArray & out_topology_array, IntArray & out_tolerance_for_tolopoly) const;

			/*! Shows the ignore criteria tessellation setting.
			 *	\param out_ignore Whether facets are ignored in this kit. If this is false the other parameters are meaningless.
			 *	\param out_ignore_criteria The ignore criteria setting for this kit. Meaningless if out_ignore is false.
			 *	\param out_minimum_value The minimum value for ignoring a facet for this kit. Meaningless if out_ignore is false.
			 *	\param out_treat_faces_individually Whether faces are considered individually when deciding if they should be ignored. Meaningless if out_ignore is false.
			 *	\return <span class='code'>true</span> if a ignore criteria were specified, <span class='code'>false</span> otherwise. */
			bool						ShowIgnoreCriteria(bool & out_ignore, Ignore & out_ignore_criteria, double & out_minimum_value, bool & out_treat_faces_individually) const;

			/*! Shows the wire edges tessellation setting.
			 *	\param out_wire_edges The wire edge setting for this kit.
			 *	\return <span class='code'>true</span> if a wire edge setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowWireEdges(bool & out_wire_edges) const;

			/*! Shows the incremental faceting tessellation setting.
			 *	\param out_faceting The incremental faceting setting for this kit.
			 *	\param out_refine_tessellation Whether facets are refined setting for this kit.
			 *	\return <span class='code'>true</span> if an incremental faceting setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowIncrementalFaceting(Facet::IncrementalFaceting & out_faceting, bool & out_refine_tessellation) const;

			/*! Shows the inflection tessellation setting.
			 *	\param out_refine The inflection setting for this kit.
			 *	\return <span class='code'>true</span> if a inflection setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowInflection(bool & out_refine) const;

			/*! Shows the improved facet quality tessellation setting.
			 *	\param out_improved_quality The out_improved_quality setting for this kit.
			 *	\return <span class='code'>true</span> if a improved facet quality setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowImprovedFacetQuality(bool & out_improved_quality) const;

			/*! Shows the flatten faces tessellation setting.
			 *	\param out_flatten_vertices The flatten faces setting for this kit.
			 *	\return <span class='code'>true</span> if a flatten faces setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowFlattenVertices(bool & out_flatten_vertices) const;

			/*! Shows the offset faces tessellation setting.
			 *	\param out_offset_faces The offset faces setting for this kit.
			 *	\return <span class='code'>true</span> if a offset faces setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowOffsetFaces(bool & out_offset_faces) const;

			/*! Shows the normals tessellation setting.
			 *	\param out_normals The normals setting for this kit.
			 *	\return <span class='code'>true</span> if a normals setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowNormals(bool & out_normals) const;

			/*! Shows the parameters tessellation setting.
			 *	\param out_parameters The parameters setting for this kit.
			 *	\return <span class='code'>true</span> if a parameters setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowParameters(bool & out_parameters) const;

			/*! Shows the edges tessellation setting.
			 *	\param out_edges The edges setting for this kit.
			 *	\return <span class='code'>true</span> if a edges setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowEdges(bool & out_edges) const;

			/*! Shows the strips tessellation setting.
			 *	\param out_strips The strips setting for this kit.
			 *	\param out_max_facets_per_strip The maximum number of facet per strip setting for this kit.
			 *	\param out_split_strips Whether strips can be split for this kit.
			 *	\return <span class='code'>true</span> if a strips setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowStrips(bool & out_strips, int & out_max_facets_per_strip, bool & out_split_strips) const;

		};

		/*! The LineTessellationKit class is a user space object.  It contains settings controlling what and how data is imported via Parasolid. */
		class PARASOLID_API LineTessellationKit : public SprocketKit
		{
		public:
			/*! The default constructor creates an empty LineTessellationKit object. */
			LineTessellationKit();

			/*! The copy constructor creates a new LineTessellationKit object that contains the same settings as the source LineTessellationKit.
			 * 	\param in_kit The source LineTessellationKit to copy. */
			LineTessellationKit(LineTessellationKit const & in_kit);

			/*! The move constructor creates an LineTessellationKit by transferring the underlying impl of the rvalue reference to this LineTessellationKit
			 * 	thereby avoiding a copy and allocation.
			 * 	\param in_that An rvalue reference to an LineTessellationKit to take the impl from. */
			LineTessellationKit(LineTessellationKit && in_that);

			/*! The move assignment operator transfers the underlying impl of the rvalue reference to this LineTessellationKit thereby avoiding a copy.
			 * 	\param in_that An rvalue reference to an LineTessellationKit to take the impl from.
			 * 	\return A reference to this LineTessellationKit. */
			LineTessellationKit & operator=(LineTessellationKit && in_that);

			virtual ~LineTessellationKit();

			static const HPS::Type staticType = HPS::Type::ParasolidLineTessellationKit;
			HPS::Type					ObjectType() const { return staticType; }

			/*! Creates an LineTessellationKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
			 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Import.
			 *	\return An LineTessellationKit with the default settings. */
			static LineTessellationKit		GetDefault();

			/*! Copies the source LineTessellationKit into this LineTessellationKit.
			 * 	\param in_kit The source LineTessellationKit to copy. */
			void						Set(LineTessellationKit const & in_kit);

			/*! Copies this LineTessellationKit into the given LineTessellationKit.
			 * 	\param out_kit The LineTessellationKit to populate with the contents of this LineTessellationKit. */
			void						Show(LineTessellationKit & out_kit) const;

			/*! Copies the source LineTessellationKit into this LineTessellationKit.
			 * 	\param in_kit The source LineTessellationKit to copy.
			 * 	\return A reference to this LineTessellationKit. */
			LineTessellationKit &			operator=(LineTessellationKit const & in_kit);

			/*! Indicates whether this LineTessellationKit has any values set on it.
			 * 	\return <span class='code'>true</span> if no values are set on this LineTessellationKit, <span class='code'>false</span> otherwise. */
			bool						Empty() const;

			/*!	Check if the source LineTessellationKit is equivalent to this LineTessellationKit. 
			 *	\param in_kit The source LineTessellationKit to compare to this LineTessellationKit.
			 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
			bool						Equals(LineTessellationKit const & in_kit) const;

			/*!	Check if the source LineTessellationKit is equivalent to this LineTessellationKit. 
			 *	\param in_kit The source LineTessellationKit to compare to this LineTessellationKit.
			 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
			bool						operator==(LineTessellationKit const & in_kit) const;

			/*!	Check if the source LineTessellationKit is not equivalent to this LineTessellationKit. 
			 *	\param in_kit The source LineTessellationKit to compare to this LineTessellationKit.
			 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
			bool						operator!=(LineTessellationKit const & in_kit) const;



			/*! Sets whether edges are drawn.
			 *	\param in_draw_edges whether edges are drawn. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.edge.
			 *	\param in_internal_edges whether edges are drawn. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.internal.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetEdges(bool in_draw_edges, bool in_internal_edges);

			/*! Sets whether silhouette edges are drawn.
			 *  This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.silhouette.
			 *	\param in_draw_silhouette_edges whether silhouette edges are drawn. 
			 *	\param in_draw_circles_as_arcs whether near circular analytic silhouettes can be drawn as arcs
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetSilhouetteEdges(bool in_draw_silhouette_edges, bool in_draw_circles_as_arcs);

			/*! Sets planar hatching tessellation options
			 *	\param in_planar_haching the type of planar hatching desired. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.planar.
			 *	\param in_planar_spacing distance between panes. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.planar_spacing.
			 *	\param in_point_through used with in_direction to determine the axis for planar hatching. Only relevant if in_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_direction used with in_point_through to determine the axis for planar hatching. Only relevant if in_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetPlanarHatching(Line::Hatching::Planar in_planar_haching, double in_planar_spacing = 0, DPoint in_point_through = DPoint(0, 0, 0), DVector in_direction = DVector(0, 0, 0));

			/*! Sets radial hatching tessellation options
			 *	\param in_radial_hatching the type of radial hatching desired. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial.
			 *	\param in_around angular spacing around spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_around. Only relevant if in_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_around_start starting value for angular spacing around spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_around_start. Only relevant if in_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_along spacing along spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_along. Only relevant if in_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_along_start starting value for angular spacing along spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_along_start. Only relevant if in_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_about spacing about spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_about. Only relevant if in_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_about_start starting value for angular spacing about spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_about_start. Only relevant if in_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetRadialHatching(Line::Hatching::Radial in_radial_hatching, double in_around = 0, double in_around_start = 0, double in_along = 0, double in_along_start = 0, double in_about = 0, double in_about_start = 0);

			/*! Sets parametric hatching tessellation options
			 *	\param in_parametric_hatching the type of parametric hatching desired. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param.
			 *	\param in_u hatch spacing in u direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_u. Only relevant if in_parametric_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_u_start start value for parametric hatching in the u direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_u_start. Only relevant if in_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_v hatch spacing in v direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_v. Only relevant if in_parametric_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param in_v_start start value for parametric hatching in the v direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_v_start. Only relevant if in_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetParametricHatching(Line::Hatching::Parametric in_parametric_hatching, double in_u = 0, double in_u_start = 0, double in_v = 0, double in_v_start = 0);

			/*! Sets whether unfixed blends are drawn
			 *	\param in_unfixed_blends whether unfixed blends are drawn. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.unfix.
			 *	\param in_spacing default rib spacing. Only relevant if in_unfixed_blends equals Draw. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.unfix_spacing.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetUnfixedBlends(Line::UnfixedBlends in_unfixed_blends, double in_spacing = 0);

			/*! Sets whether visibility is evaluated
			 *	\param in_visibility setting for visibility evaluation. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.visibility.
			 *	\param in_draw_invisible whether to output invisible lines. Only relevant if in_visibility equals Custom. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.invisible.
			 *	\param in_drafting whether to distinguish between lines hidden by other lines and lines hidden by faces. Only relevant if in_visibility equals Custom. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.drafting.
			 *	\param in_self_hidden whether to distinguish lines that are self-hidden or hidden by another occurrence of themselves. Only relevant if in_visibility equals Custom. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.self_hidden.
			 *	\param in_suppressed_indices array of indices for which invisible lines are suppressed. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.suppressed_indices.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetVisibilityEvaluation(Line::Visibility in_visibility, bool in_draw_invisible = false, bool in_drafting = false, bool in_self_hidden = false, IntArray in_suppressed_indices = IntArray());

			/*! Sets whether to indicate smooth edges
			 *	\param in_smooth whether to indicate smooth edges. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.smooth.
			 *	\param in_tolerance maximum angle allowed between faces for edges to be smooth. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.edge_smooth_tol.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetSmoothEdges(Line::Smooth in_smooth, double in_tolerance = 0);

			/*! Sets whether regional data is produced.
			 *	\param in_region whether regional data is produced. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.region.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetRegionalData(Line::Region in_region);

			/*! Sets whether a hierarchical output is produced
			 *	\param in_hierarchical_output whether a hierarchical output is produced. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.hierarch.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetHierarchicalOutput(Line::Hierarchical in_hierarchical_output);

			/*! Sets the method used to render bcurves
			 *	\param in_bcurve_rendering_method the method used to render bcurves. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.bcurve.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetBCurveRenderingMethod(Line::BCurveRendering in_bcurve_rendering_method);

			/*! Sets whether the importer uses viewports and how.
			 *	\param in_use_viewports whether the importer uses viewports. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.viewport
			 *	\param in_is_3d whether the viewports used are three dimensional or not. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.viewport_type.
			 *	\param viewport_clipping whether viewport clipping should be used. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.viewport_clipping.
			 *	\param in_viewports the viewports used.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetViewports(bool in_use_viewports, bool in_is_3d = true, bool viewport_clipping = false, ViewportArray in_viewports = ViewportArray());

			/*! Sets chord tolerance.
			 *	\param in_chord_tolerance curve chordal tolerance between a curve and chord edges. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.curve_chord_tol.
			 *	\param in_maximum_chord_length maximum chord length. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.curve_chord_max.
			 *	\param in_maximum_chord_angle maximum chord angle, in radians. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.curve_chord_ang.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetChordTolerance(double in_chord_tolerance, double in_maximum_chord_length, double in_maximum_chord_angle);

			/*! Sets criteria for ignoring lines.
			 *	\param in_ignore whether any line should be ignored.
			 *	\param in_ignore_criteria specifies how to interpret the minimum_value parameter. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.ignore.
			 *	\param in_minimum_value minimum value for a facet not to be ignored. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.ignore_value.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetIgnoreCriteria(bool in_ignore, Ignore in_ignore_criteria = Ignore::Absolute, double in_minimum_value = 0);

			/*! Sets whether a memory limit should be used for rendering lines
			 *	\param in_use_memory_limit whether a memory limit should be used for rendering lines. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.memory_target.
			 *	\param in_limit the memory limits, in bytes. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.memory_target_value.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetMemoryLimit(bool in_use_memory_limit, size_t in_limit = 0);

			/*! Sets the transparency settings for line tessellation
			 *	\param in_allow_transparency whether transparent bodies are drawn. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.transparent.
			 *	\param in_transparent_hide whether transparent bodies can hide other transparent bodies. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.transparent_hid.
			 *	\param in_transparent_bodies an array of body occurrences to be rendered transparent. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.transparent_indices.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetTransparentBodies(bool in_allow_transparency, bool in_transparent_hide, IntArray in_transparent_bodies = IntArray());

			/*! Sets the behavior the import assumes when it encounters missing geometry
			 *	\param in_fail_on_missing_geometry whether the importer should fail when encountering missing geometry. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.ske_missing.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetMissingGeometryFailure(bool in_fail_on_missing_geometry);

			/*! Sets the behavior the import assumes when it encounters overlapping curves
			 *	\param in_overlapping_behavior the behavior the import assumes when it encounters overlapping curves. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.overlap.
			 *	\param in_indices_one array of body occurrences used for clashing. Only relevant if in_overlapping_behavior equals AllowAndIntersectAll or AllowAndIntersectPairs. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.overlap_indices1.
			 *	\param in_indices_two array of body occurrences used for clashing. Only relevant if in_overlapping_behavior equals AllowAndIntersectAll or AllowAndIntersectPairs. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.overlap_indices2.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		SetOverlappingBehavior(Line::OverlappingBehavior in_overlapping_behavior, IntArray in_indices_one = IntArray(), IntArray in_indices_two = IntArray());



			/*! Removes the edge tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetEdges();

			/*! Removes the silhouette edges tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetSilhouetteEdges();

			/*! Removes the planar hatching tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetPlanarHatching();

			/*! Removes the radial hatching tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetRadialHatching();

			/*! Removes the parametric hatching tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetParametericHatching();

			/*! Removes the unfixed blends tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetUnfixedBlends();

			/*! Removes the visibility evaluation tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetVisibilityEvaluation();

			/*! Removes the smooth edges tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetSmoothEdges();

			/*! Removes the regional data tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetRegionalData();

			/*! Removes the hierarchical output tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetHierarchicalOutput();

			/*! Removes the bcurve rendering method tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetBCurveRenderingMethod();

			/*! Removes the viewports tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetViewports();

			/*! Removes the chord tolerance tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetChordTolerance();

			/*! Removes the ignore criteria tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetIgnoreCriteria();

			/*! Removes the memory limit tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetMemoryLimit();

			/*! Removes the transparent bodies tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetTransparentBodies();

			/*! Removes the missing geometry tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetMissingGeometryFailure();

			/*! Removes the overlapping geometry tessellation state.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetOverlappingBehavior();

			/*! Removes all settings from this LineTessellationKit.
			 *	\return A reference to this LineTessellationKit. */
			LineTessellationKit &		UnsetEverything();




			/*! Shows whether edges are drawn.
			 *	\param & out_draw_edges whether edges are drawn. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.edge.
			 *	\param & out_internal_edges whether edges are drawn. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.internal.
			 *	\return <span class='code'>true</span> if an edge setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowEdges(bool & out_draw_edges, bool & out_internal_edges) const;

			/*! Shows whether silhouette edges are drawn.
			 *  This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.silhouette.
			 *	\param & out_draw_silhouette_edges whether silhouette edges are drawn. 
			 *	\param & out_draw_circles_as_arcs whether near circular analytic silhouettes can be drawn as arcs
			 *	\return <span class='code'>true</span> if a silhouette edges setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowSilhouetteEdges(bool & out_draw_silhouette_edges, bool & out_draw_circles_as_arcs) const;

			/*! Shows planar hatching tessellation options
			 *	\param & out_planar_haching the type of planar hatching desired. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.planar.
			 *	\param & out_planar_spacing distance between panes. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.planar_spacing.
			 *	\param & out_point_through used with & out_direction to determine the axis for planar hatching. Only relevant if & out_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_direction used with & out_point_through to determine the axis for planar hatching. Only relevant if & out_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\return <span class='code'>true</span> if a planar hatching setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowPlanarHatching(Line::Hatching::Planar & out_planar_haching, double & out_planar_spacing, DPoint & out_point_through, DVector & out_direction) const;

			/*! Shows radial hatching tessellation options
			 *	\param & out_radial_hatching the type of radial hatching desired. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial.
			 *	\param & out_around angular spacing around spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_around. Only relevant if & out_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_around_start starting value for angular spacing around spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_around_start. Only relevant if & out_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_along spacing along spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_along. Only relevant if & out_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_along_start starting value for angular spacing along spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_along_start. Only relevant if & out_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_about spacing about spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_about. Only relevant if & out_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_about_start starting value for angular spacing about spine. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.radial_about_start. Only relevant if & out_radial_hatching equals to either ByVectorNoLocking or ByVector
			 *	\return <span class='code'>true</span> if a radial hatching setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowRadialHatching(Line::Hatching::Radial & out_radial_hatching, double & out_around, double & out_around_start, double & out_along, double & out_along_start, double & out_about, double & out_about_start) const;

			/*! Shows parametric hatching tessellation options
			 *	\param & out_parametric_hatching the type of parametric hatching desired. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param.
			 *	\param & out_u hatch spacing in u direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_u. Only relevant if & out_parametric_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_u_start start value for parametric hatching in the u direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_u_start. Only relevant if & out_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_v hatch spacing in v direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_v. Only relevant if & out_parametric_hatching equals to either ByVectorNoLocking or ByVector
			 *	\param & out_v_start start value for parametric hatching in the v direction. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.param_v_start. Only relevant if & out_planar_hatching equals to either ByVectorNoLocking or ByVector
			 *	\return <span class='code'>true</span> if a parametric hatching setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowParametericHatching(Line::Hatching::Parametric & out_parametric_hatching, double & out_u, double & out_u_start, double & out_v, double & out_v_start) const;

			/*! Shows whether unfixed blends are drawn
			 *	\param & out_unfixed_blends whether unfixed blends are drawn. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.unfix.
			 *	\param & out_spacing default rib spacing. Only relevant if & out_unfixed_blends equals ByAttribute. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.unfix_spacing.
			 *	\return <span class='code'>true</span> if an unfixed blends setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowUnfixedBlends(Line::UnfixedBlends & out_unfixed_blends, double & out_spacing) const;

			/*! Shows whether visibility is evaluated
			 *	\param & out_visibility Setting for visibility evaluation. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.visibility.
			 *	\param & out_draw_invisible whether to output invisible lines. Only relevant if & out_visibility equals Custom. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.invisible.
			 *	\param & out_drafting whether to distinguish between lines hidden by other lines and lines hidden by faces. Only relevant if & out_visibility equals Custom. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.drafting.
			 *	\param & out_self_hidden whether to distinguish lines that are self-hidden or hidden by another occurrence of themselves. Only relevant if & out_visibility equals Custom. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.self_hidden.
			 *	\param & out_suppressed_indices array of indices for which invisible lines are suppressed. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.suppressed_indices.
			 *	\return <span class='code'>true</span> if a visibility setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowVisibilityEvaluation(Line::Visibility & out_visibility, bool & out_draw_invisible, bool & out_drafting, bool & out_self_hidden, IntArray & out_suppressed_indices) const;

			/*! Shows whether to indicate smooth edges
			 *	\param & out_smooth whether to indicate smooth edges. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.smooth.
			 *	\param & out_tolerance maximum angle allowed between faces for edges to be smooth. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.edge_smooth_tol.
			 *	\return <span class='code'>true</span> if a smooth edge setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowSmoothEdges(Line::Smooth & out_smooth, double & out_tolerance) const;

			/*! Shows whether regional data is produced.
			 *	\param & out_region whether regional data is produced. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.region.
			 *	\return <span class='code'>true</span> if a region setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowRegionalData(Line::Region & out_region) const;

			/*! Shows whether a hierarchical output is produced
			 *	\param & out_hierarchical_output whether a hierarchical output is produced. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.hierarch.
			 *	\return <span class='code'>true</span> if a hierarchical setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowHierarchicalOutput(Line::Hierarchical & out_hierarchical_output) const;

			/*! Shows the method used to render bcurves
			 *	\param & out_bcurve_rendering_method the method used to render bcurves. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.bcurve.
			 *	\return <span class='code'>true</span> if a bcurve setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowBCurveRenderingMethod(Line::BCurveRendering & out_bcurve_rendering_method) const;

			/*! Shows whether the importer uses viewports and how.
			 *	\param & out_use_viewports whether the importer uses viewports. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.viewport
			 *	\param & out_is_3d whether the viewports used are three dimensional or not. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.viewport_type.
			 *	\param viewport_clipping whether viewport clipping should be used. This corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.viewport_clipping.
			 *	\param & out_viewports the viewports used.
			 *	\return <span class='code'>true</span> if a viewport setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowViewports(bool & out_use_viewports, bool & out_is_3d, bool & out_viewport_clipping, ViewportArray & out_viewports) const;

			/*! Shows chord tolerance.
			 *	\param & out_chord_tolerance curve chordal tolerance between a curve and chord edges. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.curve_chord_tol.
			 *	\param & out_maximum_chord_length maximum chord length. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.curve_chord_max.
			 *	\param & out_maximum_chord_angle maximum chord angle, in radians. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.curve_chord_ang.
			 *	\return <span class='code'>true</span> if a tolerance setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowChordTolerance(double & out_chord_tolerance, double & out_maximum_chord_length, double & out_maximum_chord_angle) const;

			/*! Shows criteria for ignoring lines.
			 *	\param & out_ignore whether any line should be ignored.
			 *	\param & out_ignore_criteria specifies how to interpret the minimum_value parameter. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.ignore.
			 *	\param & out_minimum_value minimum value for a facet not to be ignored. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.ignore_value.
			 *	\return <span class='code'>true</span> if an ignore criteria setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowIgnoreCriteria(bool & out_ignore, Ignore & out_ignore_criteria, double & out_minimum_value) const;

			/*! Shows whether a memory limit should be used for rendering lines
			 *	\param & out_use_memory_limit whether a memory limit should be used for rendering lines. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.memory_target.
			 *	\param & out_limit the memory limits, in bytes. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.memory_target_value.
			 *	\return <span class='code'>true</span> if an memory limit setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowMemoryLimit(bool & out_use_memory_limit, size_t & out_limit) const;

			/*! Shows the transparency setting for line tessellation
			 *	\param & out_allow_transparency whether transparent bodies are drawn. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.transparent.
			 *	\param & out_transparent_hide whether transparent bodies can hide other transparent bodies. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.transparent_hid.
			 *	\param & out_force_transparent an array of body occurrences to be rendered transparent. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.transparent_indices.
			 *	\return <span class='code'>true</span> if a transparency setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowTransparentBodies(bool & out_allow_transparency, bool & out_transparent_hide, IntArray & out_force_transparent) const;

			/*! Shows the behavior the import assumes when it encounters missing geometry
			 *	\param & out_fail_on_missing_geometry whether the importer should fail when encountering missing geometry. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.ske_missing.
			 *	\return <span class='code'>true</span> if a missing geometry setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowMissingGeometryFailure(bool & out_fail_on_missing_geometry) const;

			/*! Shows the behavior the import assumes when it encounters overlapping curves
			 *	\param & out_overlapping_behavior the behavior the import assumes when it encounters overlapping curves. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.overlap.
			 *	\param & out_indices_one array of body occurrences used for clashing. Only relevant if & out_overlapping_behavior equals AllowAndIntersectAll or AllowAndIntersectPairs. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.overlap_indices1.
			 *	\param & out_indices_two array of body occurrences used for clashing. Only relevant if & out_overlapping_behavior equals AllowAndIntersectAll or AllowAndIntersectPairs. Corresponds to the value that will be passed to PK_TOPOL_render_line_o_t.overlap_indices2.
			 *	\return <span class='code'>true</span> if an overlapping behavior setting was specified, <span class='code'>false</span> otherwise. */
			bool		ShowOverlappingBehavior(Line::OverlappingBehavior & out_overlapping_behavior, IntArray & out_indices_one, IntArray & out_indices_two) const;
		};


		/*! The ExportOptionsKit class is a user space object. It contains settings controlling what and how data is exported via Parasolid. Default values for the various fields of this class can be found <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-io">here.</a> */
		class PARASOLID_API ExportOptionsKit : public SprocketKit
		{
		public:
			/*! The default constructor creates an empty ExportOptionsKit object. */
			ExportOptionsKit();

			/*! The copy constructor creates a new ExportOptionsKit object that contains the same settings as the source ExportOptionsKit.
			 * 	\param in_kit The source ExportOptionsKit to copy. */
			ExportOptionsKit(ExportOptionsKit const & in_kit);

			/*! The move constructor creates an ExportOptionsKit by transferring the underlying impl of the rvalue reference to this ExportOptionsKit
			 * 	thereby avoiding a copy and allocation.
			 * 	\param in_that An rvalue reference to an ExportOptionsKit to take the impl from. */
			ExportOptionsKit(ExportOptionsKit && in_that);

			/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ExportOptionsKit thereby avoiding a copy.
			 * 	\param in_that An rvalue reference to an ExportOptionsKit to take the impl from.
			 * 	\return A reference to this ExportOptionsKit. */
			ExportOptionsKit & operator=(ExportOptionsKit && in_that);

			virtual ~ExportOptionsKit();

			static const HPS::Type staticType = HPS::Type::ParasolidExportOptionsKit;
			HPS::Type					ObjectType() const { return staticType; }

			/*! Creates an ExportOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
			 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Export.
			 *	\return An ExportOptionsKit with the default settings. */
			static ExportOptionsKit		GetDefault();

			/*! Copies the source ExportOptionsKit into this ExportOptionsKit.
			 * 	\param in_kit The source ExportOptionsKit to copy. */
			void						Set(ExportOptionsKit const & in_kit);

			/*! Copies this ExportOptionsKit into the given ExportOptionsKit.
			 * 	\param out_kit The ExportOptionsKit to populate with the contents of this ExportOptionsKit. */
			void						Show(ExportOptionsKit & out_kit) const;

			/*! Copies the source ExportOptionsKit into this ExportOptionsKit.
			 * 	\param in_kit The source ExportOptionsKit to copy.
			 * 	\return A reference to this ExportOptionsKit. */
			ExportOptionsKit &			operator=(ExportOptionsKit const & in_kit);

			/*! Indicates whether this ExportOptionsKit has any values set on it.
			 * 	\return <span class='code'>true</span> if no values are set on this ExportOptionsKit, <span class='code'>false</span> otherwise. */
			bool						Empty() const;

			/*!	Check if the source ExportOptionsKit is equivalent to this ExportOptionsKit. 
			 *	\param in_kit The source ExportOptionsKit to compare to this ExportOptionsKit.
			 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
			bool						Equals(ExportOptionsKit const & in_kit) const;

			/*!	Check if the source ExportOptionsKit is equivalent to this ExportOptionsKit. 
			 *	\param in_kit The source ExportOptionsKit to compare to this ExportOptionsKit.
			 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
			bool						operator==(ExportOptionsKit const & in_kit) const;

			/*!	Check if the source ExportOptionsKit is not equivalent to this ExportOptionsKit. 
			 *	\param in_kit The source ExportOptionsKit to compare to this ExportOptionsKit.
			 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
			bool						operator!=(ExportOptionsKit const & in_kit) const;

			/*! Sets the format type to be exported.  
			 *  This corresponds to the value that will be passed to PK_PART_transmit_o_t.transmit_format.
			 *	\param in_format The format of the file to be exported.
			 *	\return A reference to this ExportOptionsKit. 
			 *  \sa <a href="../../prog_guide/appendix_default_attribute_values.html#parasolid-export-format">Default value</a>
			 */
			ExportOptionsKit &			SetFormat(Parasolid::Format in_format);

			/*! Sets whether to export user fields from the CAD file.  
			 *  This corresponds to the value that will be passed to PK_PART_transmit_o_t.receive_user_fields.
			 *	\param in_export_user_fields Whether to export user fields from the CAD file.
			 *	\return A reference to this ExportOptionsKit. */
			ExportOptionsKit &			SetUserFields(bool in_export_user_fields);


			/*! Removes the format export state.
			 *	\return A reference to this ExportOptionsKit. */
			ExportOptionsKit &			UnsetFormat();

			/*! Removes the user fields export state.
			 *	\return A reference to this ExportOptionsKit. */
			ExportOptionsKit &			UnsetUserFields();

			/*! Removes all settings from this ExportOptionsKit.
			 *	\return A reference to this ExportOptionsKit. */
			ExportOptionsKit &			UnsetEverything();

			
			/*! Shows the format export setting.
			 *	\param out_format The format setting for this export.
			 *	\return <span class='code'>true</span> if a format setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowFormat(Parasolid::Format & out_format) const;

			/*! Shows the user fields export setting.
			 *	\param out_user_fields The user fields setting for this export.
			 *	\return <span class='code'>true</span> if a user fields setting was specified, <span class='code'>false</span> otherwise. */
			bool						ShowUserFields(bool & out_user_fields) const;
		};
	};
}

#endif //SPRK_PARASOLID_H