// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_EXCHANGE_PARASOLID_H
#define SPRK_EXCHANGE_PARASOLID_H

#include "sprk.h"
#include "sprk_exchange.h"
#include "sprk_parasolid.h"

#ifdef _MSC_VER
#ifndef STATIC_APP
#	ifdef SPRK_EXCHANGE_PARASOLID
#		define EXCHANGE_PARASOLID_API __declspec (dllexport)
#	else
#		define EXCHANGE_PARASOLID_API __declspec (dllimport)
#	endif
#endif
#else
#	include <stddef.h>
#	if defined(LINUX_SYSTEM) && defined(SPRK_EXCHANGE_PARASOLID)
#		ifndef STATIC_APP
#			define EXCHANGE_PARASOLID_API __attribute__ ((visibility ("default")))
#		endif
#	endif
#endif

#ifndef EXCHANGE_PARASOLID_API
#	define EXCHANGE_PARASOLID_API 
#endif

namespace HPS
{

class EXCHANGE_PARASOLID_API ExchangeParasolid
{
public:
	typedef int ParasolidEntity;

	/*! The ImportNotifier class is a smart-pointer that is tied to a file import.  It is used to interact with an ongoing import or get the results from a completed import. */
	class EXCHANGE_PARASOLID_API ImportNotifier : public IONotifier
	{
	public:
		/*! The default constructor creates an ImportNotifier object which is not tied to any file import. */
		ImportNotifier();

		/*! The copy constructor creates a new ImportNotifier object that is associated with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier to copy. */
		ImportNotifier(ImportNotifier const & in_that);

		/*! The conversion constructor creates a new derived ImportNotifier object from a base IONotifier object.
		 *	The copy will only be successful if the source notifier is really an upcast of this notifier type.
		 *	Otherwise the copy will fail and the resulting ImportNotifier will be invalid.
		 *	\param in_that The source IONotifier to copy. */
		ImportNotifier(IONotifier const & in_that);

		/*! The move constructor creates an ImportNotifier by transferring the underlying impl of the rvalue reference to this ImportNotifier
		 *	thereby avoiding a copy and allocation.
		 *	\param in_that An rvalue reference to an ImportNotifier to take the impl from. */
		ImportNotifier(ImportNotifier && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ImportNotifier thereby avoiding a copy.
		 *	\param in_that An rvalue reference to an ImportNotifier to take the impl from.
		 *	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier && in_that);

		virtual ~ImportNotifier();

	static const HPS::Type staticType = HPS::Type::ExchangeParasolidImportNotifier;
		HPS::Type				ObjectType() const { return staticType; };

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment.
		 *	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier const & in_that);

		/*! Associate this ImportNotifier with the same file import as the source ImportNotifier.
		 *	\param in_that The source ImportNotifier for the assignment. */
		void					Assign(ImportNotifier const & in_that);

		/*!	Get the CADModel for the file import.  Throws an IOException if the import is not complete, was not successful or was canceled.
		 *	\return The CADModel for a successful file import. */
		Exchange::CADModel		GetCADModel() const;

		/*!	Get the number of milliseconds it took Exchange to read the CAD file(s) and generate the corresponding PRC data.
		 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
		Time					GetImportTime() const;

		/*!	Get the number of milliseconds it took Exchange to translate the PRC data into Parasolid data.
		 *	Throws an IOException if the translation is not complete, was not successful or was canceled. */
		Time					GetTranslationTime() const;

		/*!	Get the number of milliseconds it took Visualize to parse the PRC data and create the corresponding scene graph.
		 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
		Time					GetParseTime() const;
	};

	/*!	The File class provides functions to import CAD files via Exchange and manage the BRep data with Parasolid. */
	class EXCHANGE_PARASOLID_API File
	{
	public:
		/*!	Performs an asynchronous import of the specified CAD file with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 *	Note that if the import is successful, the CADModel created from this import will own its associated A3DAsmModelFile (see Exchange::CADModel::GetEntityOwnership
		 *	for more details).
		 *	\param in_filename The name of the CAD file to import.
		 *	\param in_import_options The options controlling the import of the CAD file.
		 *	\param in_translation_options The options controlling the translation of the CAD data to Parasolid.
		 *	\param in_facet_tessellation The options controlling the facet tessellation for the Parasolid data.
		 *	\param in_line_tessellation The options controlling the line tessellation for the Parasolid data.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(
			char const * in_filename,
			Exchange::ImportOptionsKit const & in_import_options,
			Exchange::TranslationOptionsKit const & in_translation_options,
			Parasolid::FacetTessellationKit const & in_facet_tessellation,
			Parasolid::LineTessellationKit const & in_line_tessellation);

		/*! Performs an asynchronous reload of the specified Exchange::Component with the provided options.  May throw an IOException prior to starting the asynchronous reload.
		 *	This function differs from the Exchange::Component::Reload function in that it allows one to specify all Parasolid-related tessellation options, rather than
		 *	restricting those options to the subset supported by Exchange and Parasolid.
		 *	\param in_translation_options The options controlling the translation of the CAD data to Parasolid.
		 *	\param in_facet_tessellation The options controlling the facet tessellation for the Parasolid data.
		 *	\param in_line_tessellation The options controlling the line tessellation for the Parasolid data.
		 *	\return An Exchange::ReloadNotifier object that can be used to query the reload progress and status. */
		static Exchange::ReloadNotifier Reload(
			Exchange::Component const & in_component,
			Exchange::TranslationOptionsKit const & in_translation_options,
			Parasolid::FacetTessellationKit const & in_facet_tessellation,
			Parasolid::LineTessellationKit const & in_line_tessellation);

		/*! Performs an asynchronous reload of the specified Exchange::CADModel with the provided options.  May throw an IOException prior to starting the asynchronous reload.
		 *	This function differs from the Exchange::CADModel::Reload function in that it allows one to specify all Parasolid-related tessellation options, rather than
		 *	restricting those options to the subset supported by Exchange and Parasolid.
		 *	\param in_translation_options The options controlling the translation of the CAD data to Parasolid.
		 *	\param in_facet_tessellation The options controlling the facet tessellation for the Parasolid data.
		 *	\param in_line_tessellation The options controlling the line tessellation for the Parasolid data.
		 *	\return An Exchange::ReloadNotifier object that can be used to query the reload progress and status. */
		static Exchange::ReloadNotifier Reload(
			Exchange::CADModel const & in_cad_model,
			Exchange::TranslationOptionsKit const & in_translation_options,
			Parasolid::FacetTessellationKit const & in_facet_tessellation,
			Parasolid::LineTessellationKit const & in_line_tessellation);

		/*! Re-tessellate the given Exchange::Component with the provided options.  Tessellation will occur for Parasolid bodies under the Exchange component, so calling this
		 *	function on Component objects below that level will have no effect.  If this function is invoked on Component objects above that level, e.g., at the product
		 *	occurrence level, all Parasolid bodies underneath that Component will be re-tessellated with the provided options.  This function differs from the
		 *	Exchange::Component::Tessellate function in that it allows one to specify all Parasolid-related tessellation options, rather than restricting those options
		 *	to the subset supported by Exchange and Parasolid.
		 *	\param in_facet_tessellation The options controlling the facet tessellation for the Parasolid data.
		 *	\param in_line_tessellation The options controlling the line tessellation for the Parasolid data. */
		static void Tessellate(
			Exchange::Component const & in_component,
			Parasolid::FacetTessellationKit const & in_facet_tessellation,
			Parasolid::LineTessellationKit const & in_line_tessellation);

		/*! Re-tessellate the given Exchange::CADModel with the provided options.  Tessellation will occur for Parasolid bodies.
		 *  This function differs from the Exchange::Component::Tessellate function in that it allows one to specify all Parasolid-related tessellation options, 
		 *  rather than restricting those options to the subset supported by Exchange and Parasolid.
		 *	\param in_facet_tessellation The options controlling the facet tessellation for the Parasolid data.
		 *	\param in_line_tessellation The options controlling the line tessellation for the Parasolid data. */
		static void Tessellate(
			Exchange::CADModel const & in_cad_model,
			Parasolid::FacetTessellationKit const & in_facet_tessellation,
			Parasolid::LineTessellationKit const & in_line_tessellation);

		/*! Renders the entity specified, according to the tessellation options provided.
		*	Only entities of type PK_CLASS_body can be used with this function.
		*	This function is intended to be used with Exchange CADModels. 
		*	If you wish to add a Parasolid entity to a Parasolid CADModel you should use the Parasolid::CADModel::AddEntity function.
		*	\param in_path_to_owner The component path to the location where the entity should be added. \nin_path_to_owner[0] should be a PartDefiniton component and in_path_to_owner[n] should be a CADModel component
		*	\param in_entity The Parasolid entity to add.
		*	\param in_transform A matrix to apply to the created Representation Item
		*	\param in_facet_tessellation The facet tessellation parameters used during import.
		*	\param in_line_tessellation The line tessellation parameters used during import.
		*	\return The component created as part of the import of the provided entity. 
		*	As part of the process of adding an entity, a new Representation Item component will be created under in_path_to_owner[0]. 
		*	The component referring to the added Parasolid part will be located under this newly created Representation Item component. */
		static Parasolid::Component AddEntity(
			ComponentPath const & in_path_to_owner,
			ParasolidEntity in_entity,
			MatrixKit const & in_transform = MatrixKit(),
			Parasolid::FacetTessellationKit const & in_facet_tessellation = Parasolid::FacetTessellationKit::GetDefault(),
			Parasolid::LineTessellationKit const & in_line_tessellation = Parasolid::LineTessellationKit::GetDefault());

		/*! Renders the entity specified, according to the tessellation options provided.
		*	Only entities of type PK_CLASS_body can be used with this function.
		*	This function is intended to be used with Exchange CADModels.
		*	If you wish to add a Parasolid entity to a Parasolid CADModel you should use the Parasolid::CADModel::AddEntity function.
		*	If you have not imported a file prior to calling this function, you will need to create a CADModel object through Factory::CreateCADModel and pass it to this function.
		*	\param in_owner The CADModel which will own the entity you wish to add.
		*	\param in_entity The Parasolid entity to add.
		*	\param in_transform A matrix to apply to the created Representation Item
		*	\param in_facet_tessellation The facet tessellation parameters used during import.
		*	\param in_line_tessellation The line tessellation parameters used during import.
		*	\return The component created as part of the import of the provided entity.
		*	As part of the process of adding an entity, a new Product Occurrence, Part Definition and Representation Item component will be created under the CADModel
		*	The component referring to the added Parasolid part will be located under this newly created Representation Item component. */
		static Parasolid::Component AddEntity(
			Exchange::CADModel const & in_owner,
			ParasolidEntity in_entity,
			MatrixKit const & in_transform = MatrixKit(),
			Parasolid::FacetTessellationKit const & in_facet_tessellation = Parasolid::FacetTessellationKit::GetDefault(),
			Parasolid::LineTessellationKit const & in_line_tessellation = Parasolid::LineTessellationKit::GetDefault());

	private:
		//! Private default constructor to prevent instantiation.
		File();
	};

private:
	//! Private default constructor to prevent instantiation.
	ExchangeParasolid();
};

}

#endif
