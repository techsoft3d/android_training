// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_EXCHANGE_H
#define SPRK_EXCHANGE_H

#include "sprk.h"

typedef void A3DEntity;
typedef void A3DAsmModelFile;
typedef void A3DRWParamsPrcReadHelper;

#ifdef _MSC_VER
#ifndef STATIC_APP
#	ifdef SPRK_EXCHANGE
#		define EXCHANGE_API __declspec (dllexport)
#	else
#		define EXCHANGE_API __declspec (dllimport)
#	endif
#endif
#else
#	include <stddef.h>
#	if defined(LINUX_SYSTEM) && defined(SPRK_EXCHANGE)
#		ifndef STATIC_APP
#			define EXCHANGE_API __attribute__ ((visibility ("default")))
#		endif
#	endif
#endif

#ifndef EXCHANGE_API
#	define EXCHANGE_API 
#endif

namespace HPS
{

/*! The Exchange class contains objects and enumerations used for importing and exporting CAD files via Exchange. */
class EXCHANGE_API Exchange
{
public:

	class Component;
	class Configuration;
	class Sheet;
	class TessellationOptionsKit;
	class TranslationNotifier;
	class TranslationOptionsKit; 
#if !defined(TARGET_OS_ANDROID)
	class ExportParasolidOptionsKit;
#endif
	class ReloadNotifier;

	typedef std::vector<Configuration, HPS::Allocator<Configuration> >							ConfigurationArray;
	typedef std::vector<Sheet, HPS::Allocator<Sheet> >											SheetArray;
	typedef HPS::IntArray																		ParasolidEntityArray;

	/*! \enum Units
	 *	Enumerates the units for files imported via Exchange. */
	enum class Units
	{
		Unknown,		//!< Unknown unit type (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999a0f569eb5bc27c70a33728e23c8ce369e">kA3DUnitUnknown</a>).
		Point,			//!< Point (1/72 of an inch) units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999ae96f899a4e3758dce0a7170ff88a78c9">kA3DUnitPoint</a>).
		Inch,			//!< Inch units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999a12f06c0e2ee078bc868a5fa569132de6">kA3DUnitInch</a>).
		Millimeter,		//!< Millimeter units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999ade3981c065da65d8daeec8e5804f94b9">kA3DUnitMillimeter</a>).
		Centimeter,		//!< Centimeter units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999ad76e469380b5017a219b9ac0f4547f43">kA3DUnitCentimeter</a>).
		Pica,			//!< Pica (1/72 of a foot) units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999af2b8a6046f67b96ad1451bed80492b37">kA3DUnitPicas</a>).
		Foot,			//!< Foot units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999aae962ce8ff2fad88b6bbadae25b1066d">kA3DUnitFoot</a>).
		Yard,			//!< Yard units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999ad5f2556d1a70238c27d15b263b7e4db1">kA3DUnitYard</a>).
		Meter,			//!< Meter units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999ac206335a311d131148be8ff44492ee80">kA3DUnitMeter</a>).
		Kilometer,		//!< Kilometer units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999ae4c6d259f422d5f1e7570970dd667b87">kA3DUnitKilometer</a>).
		Mile			//!< Mile units (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaf3d2feab4ca1327b679505072d134999a4da8e580244d4d1cfdde6bf9843822aa">kA3DUnitMile</a>).
	};

	/*! \enum UnloadMode
	 *	Enumerates the ways in which a ProductOccurrence can be unloaded. */
	enum class UnloadMode
	{
		/*! Exchange data will be unloaded for a ProductOccurrence via the <a href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__modelfile.html#gaa26665b6921e658742e5450d16c38022">A3DAsmModelFileUnloadParts</a>
		 *	and the Component objects corresponding to Exchange entities which were removed will be deleted. */
		ExchangeAndVisualization,

		/*! Exchange data will be unloaded for a ProductOccurrence via the <a href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__modelfile.html#gaa26665b6921e658742e5450d16c38022">A3DAsmModelFileUnloadParts</a>
		 *	while the corresponding Component objects will be kept around.  This will invalidate the entity pointers for any Component which no longer exists in the Exchange data,
		 *	but still allows use of the Component APIs to interact with the components.  Any such components will have a load status of LoadStatus::VisualizationOnly, and would
		 *	be reimported if they or a parent product occurrence get loaded again. */
		ExchangeOnly,
	};

	/*! \enum LoadStatus
	 *	Enumerates the various load statuses for a ProductOccurrence. */
	enum class LoadStatus
	{
		/*! Unknown state. */
		Unknown,
		/*! Product occurrence is loaded in Exchange and has visualization data. */
		Loaded,
		/*! Product occurrence has only visualization data, no Exchange data is available. */
		VisualizationOnly,
		/*! Some product occurrences under the product occurrence are loaded, some are not. */
		PartiallyLoaded,
		/*! No Exchange or visualization data is loaded for the product occurrence. */
		NotLoaded
	};

	/*! The Component class is a smart pointer.  It represents a variety of Exchange components.  The primary purpose of this class is simply to provide access to the underlying A3DEntity pointer
	 *	for use by Exchange library functions. */
	class EXCHANGE_API Component : public HPS::Component
	{
	public:
		/*! The default constructor creates an uninitialized Exchange::Component object.  The Type() function will return Type::None. */
		Component();

		/*! This constructor creates an Exchange::Component object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Component object.  Otherwise the copy will fail and the resulting Exchange::Component will be invalid.
		 *	\param in_that The source Component to copy. */
		Component(HPS::Component const & in_that);

		/*! The copy constructor creates an Exchange::Component object that shares the underlying smart-pointer of the source Exchange::Component.
		 *	\param in_that The source Exchange::Component to copy. */
		Component(Exchange::Component const & in_that);

		/*! The move constructor creates an Exchange::Component by transferring the underlying object of the rvalue reference to this Exchange::Component.
		 * 	\param in_that An rvalue reference to an Exchange::Component to take the underlying object from. */
		Component(Exchange::Component && in_that);

		virtual ~Component();

	static const HPS::Type staticType = HPS::Type::ExchangeComponent;
		HPS::Type				ObjectType() const { return staticType; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		Component &				operator=(Component const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this Exchange::Component.
		 *	\param in_that An rvalue reference to an Exchange::Component to take the underlying object from.
		 *	\return A reference to this Exchange::Component. */
		Component &				operator=(Exchange::Component && in_that);


		/*! Gets the A3DEntity pointer corresponding to this Exchange Component.
		 *	\return The A3DEntity pointer corresponding to this Exchange Component. */
		A3DEntity *				GetExchangeEntity() const;


		/*! Re-tessellate the given Component with the provided options.  Tessellation occurs at the representation item level in Exchange, so calling this function on
		 *	Component objects below that level will have no effect.  If this function is invoked on Component objects above that level, e.g., at the product
		 *	occurrence level, all representation items underneath that Component will be re-tessellated with the provided options.  Additionally, only components
		 *	for which there is BRep data can the tessellation be regenerated.
		 *	\param in_options The options to use to generate the new tessellation for this Component. */
		void					Tessellate(TessellationOptionsKit const & in_options);

		/*!	Asynchronously translates this component into an array of Parasolid parts, using the options provided.
		 *  A valid Parasolid session is required for this function to succeed.
		 *  The component has to be a Representation Item for this function to succeed.
		 * 	\param in_units The units for the representation item
		 * 	\param in_translation_options Options used during the translation.
		 *	\return A TranslationNotifier object that can be used to query the translation progress and status. */
		TranslationNotifier		Translate(Units in_units, TranslationOptionsKit const & in_translation_options);

		/*!	Asynchronously reloads this component.  This will delete all underlying subcomponents and then reparse the underlying Exchange data.
		 *	This function should be invoked on the Component corresponding to an entity which was edited or a Component above such a Component.
		 *	\param in_tessellation_options The options to use to generate tessellations for new Components (if necessary).
		 *	\param in_translation_options The options to use for translation from Exchange BRep to Parasolid.  This option only applies if a file was loaded
		 *		via the ExchangeParasolid module and there are components underlying this component that need to be translated to Parasolid.
		 *	\return A ReloadNotifier object that can be used to query the reload progress and status. */
		ReloadNotifier			Reload(TessellationOptionsKit const & in_tessellation_options = TessellationOptionsKit(), TranslationOptionsKit const & in_translation_options = TranslationOptionsKit());

		/*!  Applies a transform to the underlying PRC data associated with this component, replacing any transform currently set on it.
		*   Only components of type ExchangeProductOccurrence, or components that have the type ExchangeRepresentationItemMask can have transforms applied to them. Additionally, SetTransform does not work on Exchange models which have a Drawing structure to them. This would include CATDrawing files and 2D DWG files loaded through Exchange. Any attempt to use this function on those models will produce a warning.
		*	\param in_transform The transform to apply to this component.
		*	\return <span class='code'>true</span> if transform was applied correctly, <span class='code'>false</span> otherwise. */
		bool					SetTransform(MatrixKit const & in_transform);

		/*! Appends a transform to the underlying PRC data associated with this component.
		*   Only components of type ExchangeProductOccurrence, or components that have the type ExchangeRepresentationItemMask can have transforms applied to them. Additionally, AddTransform does not work on Exchange models which have a Drawing structure to them. This would include CATDrawing files and 2D DWG files loaded through Exchange. Any attempt to use this function on those models will produce a warning.
		*	\param in_transform The transform to apply to this component.
		*	\return <span class='code'>true</span> if transform was applied correctly, <span class='code'>false</span> otherwise. */
		bool					AddTransform(MatrixKit const & in_transform);

		/*!	Create a copy of this component under another component.
		*	This function will create a copy of the A3DEntity underlying this component and put it under the Exchange entity associated
		*	with the new owner.
		*	It will also create a new component, the keys associated to it and include those keys under the new owner.
		*	You are required to sync the HPS model tree with the PRC structures after calling this via Reload.
		*	\param in_new_owner The component which will be the owner of the newly created component.
		*	\return The newly created component.
		*	\sa Reload */
		HPS::Component			CopyTo(HPS::Component const & in_new_owner);

		/*! Rename this component in the PRC.
		*	\param in_new_name The new name of the A3DEntity.
		*	\return <span class='code'>true</span> if the name was set successfully, <span class='code'>false</span> otherwise. */
		bool					SetName(char const * in_new_name);
	};

	class EXCHANGE_API ProductOccurrence : public Exchange::Component
	{
	public:
		/*! The default constructor creates an empty ProductOccurrence object. */
		ProductOccurrence();

		/*! This constructor creates a ProductOccurrence object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::ProductOccurrence object.  Otherwise the copy will fail and the resulting Exchange::ProductOccurrence will be invalid.
		 *	\param in_that The source Component to copy. */
		ProductOccurrence(HPS::Component const & in_that);

		/*! This constructor creates a ProductOccurrence object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::ProductOccurrence object.  Otherwise the copy will fail and the resulting Exchange::ProductOccurrence will be invalid.
		 *	\param in_that The source Component to copy. */
		ProductOccurrence(Component const & in_that);

		/*! The copy constructor creates a new ProductOccurrence object that contains the same settings as the source ProductOccurrence.
		 * 	\param in_sheet The source ProductOccurrence to copy. */
		ProductOccurrence(ProductOccurrence const & in_sheet);

		/*! The move constructor creates an ProductOccurrence by transferring the underlying impl of the rvalue reference to this ProductOccurrence
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to a ProductOccurrence to take the impl from. */
		ProductOccurrence(ProductOccurrence && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ProductOccurrence thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to a ProductOccurrence to take the impl from.
		 * 	\return A reference to this ProductOccurrence. */
		ProductOccurrence & operator=(ProductOccurrence && in_that);

		virtual ~ProductOccurrence();

	static const HPS::Type staticType = HPS::Type::ExchangeProductOccurrence;
		HPS::Type				ObjectType() const { return staticType; }

		/*!	Check if the source ProductOccurrence is equivalent to this ProductOccurrence.
		 *	\param in_kit The source ProductOccurrence to compare to this ProductOccurrence.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(ProductOccurrence const & in_kit) const;

		/*!	Check if the source ProductOccurrence is equivalent to this ProductOccurrence.
		 *	\param in_kit The source ProductOccurrence to compare to this ProductOccurrence.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(ProductOccurrence const & in_kit) const;

		/*!	Check if the source ProductOccurrence is not equivalent to this ProductOccurrence.
		 *	\param in_kit The source ProductOccurrence to compare to this ProductOccurrence.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(ProductOccurrence const & in_kit) const;

		/*! Unloads this product occurrence.  This will remove the underlying data from Exchange for this component and potentially also the corresponding
		 *	component objects depending on the specified UnloadMode.  This operation can only be performed for product occurrence components coming from file
		 *	formats that support incremental loading (see ImportMode::Incremental).  If you try to unload a product occurrence coming from a format that does 
		 *	not support incremental loading, an exception will be thrown.
		 *	\param in_mode The unload mode to use for this product occurrence. */
		void					Unload(UnloadMode in_mode = UnloadMode::ExchangeAndVisualization);

		/*! Gets the load status for this product occurrence.  This is primarily relevant for product occurrences coming from formats that support incremental loading.
		 *	For product occurrences coming from formats that don't support incremental loading, it can provide information about which product occurrences contain no
		 *	data or could not be loaded (e.g., if there are missing files in an assembly), however, such product occurrences are still not capable of being
		 *	incrementally loaded.
		 *	\return The load status for this product occurrence. */
		LoadStatus				GetLoadStatus() const;

		/*! This function will check if the A3DEntity under the HPS::Exchange::Component from where we call it is an External Data Product
		*	Occurrence or not.
		*	\return A boolean which indicates if the product occurrence is an external data or not. */
		bool					IsExternalData() const;
	};

	/*! The Sheet class is a user space object.  It allows users to activate drawing sheets imported via Exchange. */
	class EXCHANGE_API Sheet : public Exchange::Component
	{
	public:
		/*! The default constructor creates an empty Sheet object. */
		Sheet();

		/*! This constructor creates a Sheet object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Sheet object.  Otherwise the copy will fail and the resulting Exchange::Sheet will be invalid.
		 *	\param in_that The source Component to copy. */
		Sheet(HPS::Component const & in_that);

		/*! This constructor creates a Sheet object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Sheet object.  Otherwise the copy will fail and the resulting Exchange::Sheet will be invalid.
		 *	\param in_that The source Component to copy. */
		Sheet(Component const & in_that);

		/*! The copy constructor creates a new Sheet object that contains the same settings as the source Sheet.
		 * 	\param in_sheet The source Sheet to copy. */
		Sheet(Sheet const & in_sheet);

		/*! The move constructor creates an Sheet by transferring the underlying impl of the rvalue reference to this Sheet
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to a Sheet to take the impl from. */
		Sheet(Sheet && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this Sheet thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to a Sheet to take the impl from.
		 * 	\return A reference to this Sheet. */
		Sheet & operator=(Sheet && in_that);

		virtual ~Sheet();

	static const HPS::Type staticType = HPS::Type::ExchangeSheet;
		HPS::Type				ObjectType() const { return staticType; }

		/*!	Check if the source Sheet is equivalent to this Sheet.
		 *	\param in_kit The source Sheet to compare to this Sheet.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(Sheet const & in_kit) const;

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		Sheet &				operator=(Sheet const & in_that) = default;
#endif

		/*!	Check if the source Sheet is equivalent to this Sheet.
		 *	\param in_kit The source Sheet to compare to this Sheet.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(Sheet const & in_kit) const;

		/*!	Check if the source Sheet is not equivalent to this Sheet.
		 *	\param in_kit The source Sheet to compare to this Sheet.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(Sheet const & in_kit) const;

		/* Activates this sheet */
		HPS::View				Activate();
	};

	/*! The CADModel class is a smart pointer.  It represents an A3DAsmModelFile in Exchange.  The primary purpose of this class is simply to provide access to the underlying A3DAsmModelFile pointer
	 *	for use by Exchange library functions. */
	class EXCHANGE_API CADModel : public HPS::CADModel
	{
	public:
		/*! The default constructor creates an uninitialized CADModel object.  The Type() function will return Type::None. */
		CADModel();

		/*! This constructor creates a CADModel object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::CADModel object.  Otherwise the copy will fail and the resulting Exchange::CADModel will be invalid.
		 *	\param in_that The source Component to copy. */
		CADModel(Component const & in_that);

		/*! This constructor creates an Exchange::CADModel object that shares the underlying smart-pointer of the source HPS::CADModel.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::CADModel object.  Otherwise the copy will fail and the resulting Exchange::CADModel will be invalid.
		 *	\param in_that The source HPS::CADModel to copy. */
		CADModel(HPS::CADModel const & in_that);

		/*! The copy constructor creates an Exchange::CADModel object that shares the underlying smart-pointer of the source Exchange::CADModel.
		 *	\param in_that The source Exchange::CADModel to copy. */
		CADModel(Exchange::CADModel const & in_that);

		/*! The move constructor creates an Exchange::CADModel by transferring the underlying object of the rvalue reference to this Exchange::CADModel.
		 * 	\param in_that An rvalue reference to an Exchange::CADModel to take the underlying object from. */
		CADModel(Exchange::CADModel && in_that);

		virtual ~CADModel();

	static const HPS::Type staticType = HPS::Type::ExchangeCADModel;
		HPS::Type				ObjectType() const { return staticType; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		CADModel &				operator=(Exchange::CADModel const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this Exchange::CADModel.
		 *	\param in_that An rvalue reference to an Exchange::CADModel to take the underlying object from.
		 *	\return A reference to this Exchange::CADModel. */
		CADModel &				operator=(Exchange::CADModel && in_that);


		/*! Gets the A3DEntity pointer corresponding to this Exchange::CADModel.
		 *	\return The A3DEntity pointer corresponding to this Exchange::CADModel. */
		A3DEntity *				GetExchangeEntity() const;

		/*! Gets the Component associated with the given A3DEntity pointer (if any).
		*	\param in_entity The A3DEntity pointer from which to find the associated Component.
		*	\return The Component associated with the given A3DEntity pointer, or an empty component if there is no such Component. */
		HPS::Component			GetComponentFromEntity(A3DEntity * in_entity) const;

		/*! Whether this CADModel owns the associated A3DAsmModelFile or not.  If this CADModel owns the associated A3DAsmModelFile, when it is deleted (via Exchange::CADModel::Delete), it will delete
		 *	not only the associated Visualize components, segments and geometry but also the A3DAsmModelFile via <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__modelfile.html#ga147b1bb7013909e3eacea23bb5bf80c2">A3DAsmModelFileDelete</a>.
		 *	If this CADModel does not own the associated A3DAsmModelFile, when it is deleted, it will only delete the associated Visualize components, segments and geometry, but
		 *	not the A3DAsmModelFile.  It will be up to the user to ensure the A3DAsmModelFile is deleted to reclaim any memory it has allocated in this case.
		 *	\return Whether this CADModel owns the associated A3DAsmModelFile. */
		bool					GetEntityOwnership() const;


		/*! Gets the current configuration for this CADModel if it exists.  Only formats which support the concept of configurations can have a current configuration, and only
		 *	files of those formats which contain configurations will have a current configuration.
		 *	\return The current configuration for this CADModel. */
		UTF8Array				GetCurrentConfiguration() const;

		/*! Gets a list of configurations for this CADModel (if any).  Only formats which support the concept of configurations can return a non-empty array, and only
		 *	files of those formats which contain configurations will return a non-empty array.
		 *	\return The list of configurations for this CADModel. */
		ConfigurationArray		GetConfigurations() const;

		/* Gets a list of sheets for this CADModel (if any). Only formats which support the concept of sheets can return a non-empty array.
		 * \return The list of sheets for this CADModel */
		SheetArray				GetSheets() const;

		/*!	Asynchronously translates this CADModel into an array of Parasolid parts, using the options provided.
		 *  A valid Parasolid session is required for this function to succeed.
		 *  If this CADModel was imported in tessellation only mode an empty Parasolid assembly will be returned.
		 *	\param in_translation_options Options used during the translation.
		 *	\return An TranslationNotifier object that can be used to query the import progress and status. */
		TranslationNotifier		Translate(TranslationOptionsKit const & in_translation_options);

		/*!	Asynchronously reloads this CADModel.  This will delete all underlying subcomponents and then reparse the underlying Exchange data.
		 *	\param in_options The options to use to generate tessellations for new Components (if necessary).
		 *	\param in_translation_options The options to use for translation from Exchange BRep to Parasolid.  This option only applies if a file was loaded
		 *		via the ExchangeParasolid module and there are components underlying this component that need to be translated to Parasolid. */
		ReloadNotifier			Reload(TessellationOptionsKit const & in_options = TessellationOptionsKit(), TranslationOptionsKit const & in_translation_options = TranslationOptionsKit());

		/*! Forces the visibility of all PMI on in the given Canvas and layer. 
		 *  This will undo any hides, shows, isolates, or other highlights performed on PMI components
		 *	contained within this CADModel via ComponentPath calls.
		 *	\param in_canvas The Canvas in which to perform the the operation
		 *	\param in_layer The layer in the Canvas in which to perform the operation. */
		void					ShowAllPMI(Canvas & in_canvas, size_t in_layer = 0);

		/*! Forces all of the PMI in this CADModel to be rendered in front of the other geometry.
		*	contained within this CADModel via ComponentPath calls.
		*	\param in_enable Dictates whether the pmi will be forced in front or not.
		*	\param in_canvas The Canvas in which to perform the the operation
		*	\param in_layer The layer in the Canvas in which to perform the operation. */
		void					SetAllPMIInFront(bool in_enable, Canvas & in_canvas, size_t in_layer = 0);

		/*! Forces the visibility of all PMI off in the given Canvas and layer. 
		 *  This will undo any hides, shows, isolates, or other highlights performed on PMI components
		 *	contained within this CADModel via ComponentPath calls.
		 *	\param in_canvas The Canvas in which to perform the the operation
		 *	\param in_layer The layer in the Canvas in which to perform the operation. */
		void					HideAllPMI(Canvas & in_canvas, size_t in_layer = 0);

		/*! Re-tessellate the given CADModel with the provided options.  Tessellation occurs at the representation item level in Exchange.
		 *  All representation items underneath this CADModel will be re-tessellated with the provided options.  Additionally, only components
		 *	for which there is BRep data can the tessellation be regenerated.
		 *	\param in_options The options to use to generate the new tessellation for this CADModel */
		void					Tessellate(TessellationOptionsKit const & in_options);
		
		/*! Replace the Exchange entity associated with the provided component.
		 *	You are required to sync the HPS model tree with the PRC structures after calling this via Reload.
		 *	\param in_component The component whose entity is to be replaced.
		 *	\param in_a3d_entity The new entity that will be associated with in_component.
		 *	\return <span class='code'>true</span> if the new entity was associated successfully, <span class='code'>false</span> otherwise.
		 *	\sa Reload */
		bool					ReplaceExchangeEntity(Component const & in_component, A3DEntity* in_a3d_entity);
	};

	/*! The Capture class is a smart pointer.  It represents an A3DMkpView in Exchange.  The primary purpose of this class is simply to provide access to the underlying A3DMkpView pointer
	 *	for use by Exchange library functions. */
	class EXCHANGE_API Capture : public HPS::Capture
	{
	public:
		/*! The default constructor creates an uninitialized Capture object.  The Type() function will return Type::None. */
		Capture();

		/*! This constructor creates a Capture object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Capture object.  Otherwise the copy will fail and the resulting Exchange::Capture will be invalid.
		 *	\param in_that The source Component to copy. */
		Capture(Component const & in_that);

		/*! This constructor creates an Exchange::Capture object that shares the underlying smart-pointer of the source HPS::Capture.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Capture object.  Otherwise the copy will fail and the resulting Exchange::Capture will be invalid.
		 *	\param in_that The source HPS::Capture to copy. */
		Capture(HPS::Capture const & in_that);

		/*! The copy constructor creates an Exchange::Capture object that shares the underlying smart-pointer of the source Exchange::Capture.
		 *	\param in_that The source Exchange::Capture to copy. */
		Capture(Exchange::Capture const & in_that);

		/*! The move constructor creates an Exchange::Capture by transferring the underlying object of the rvalue reference to this Exchange::Capture.
		 * 	\param in_that An rvalue reference to an Exchange::Capture to take the underlying object from. */
		Capture(Exchange::Capture && in_that);

		virtual ~Capture();

	static const HPS::Type staticType = HPS::Type::ExchangeCapture;
		HPS::Type				ObjectType() const { return staticType; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		Capture &				operator=(Exchange::Capture const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this Exchange::Capture.
		 *	\param in_that An rvalue reference to an Exchange::Capture to take the underlying object from.
		 *	\return A reference to this Exchange::Capture. */
		Capture &				operator=(Exchange::Capture && in_that);


		/*! Gets the A3DEntity pointer corresponding to this Exchange::Capture.
		 *	\return The A3DEntity pointer corresponding to this Exchange::Capture. */
		A3DEntity *				GetExchangeEntity() const;

		/* Activates this capture */
		HPS::View				Activate(HPS::ComponentPath const & in_capture_path = HPS::ComponentPath());
	};

	/*! The Filter class is a smart pointer.  It represents an A3DAsmFilter in Exchange.  The primary purpose of this class is simply to provide access to the underlying A3DAsmFilter pointer
	 *	for use by Exchange library functions. */
	class EXCHANGE_API Filter : public HPS::Filter
	{
	public:
		/*! The default constructor creates an uninitialized Filter object.  The Type() function will return Type::None. */
		Filter();

		/*! This constructor creates a Filter object that shares the underlying smart-pointer of the source Component.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Filter object.  Otherwise the copy will fail and the resulting Exchange::Filter will be invalid.
		 *	\param in_that The source Component to copy. */
		Filter(Component const & in_that);

		/*! This constructor creates an Exchange::Filter object that shares the underlying smart-pointer of the source HPS::Filter.  The copy will only be successful if the source component is really
		 *	an upcast of an Exchange::Filter object.  Otherwise the copy will fail and the resulting Exchange::Filter will be invalid.
		 *	\param in_that The source HPS::Filter to copy. */
		Filter(HPS::Filter const & in_that);

		/*! The copy constructor creates an Exchange::Filter object that shares the underlying smart-pointer of the source Exchange::Filter.
		 *	\param in_that The source Exchange::Filter to copy. */
		Filter(Exchange::Filter const & in_that);

		/*! The move constructor creates an Exchange::Filter by transferring the underlying object of the rvalue reference to this Exchange::Filter.
		 * 	\param in_that An rvalue reference to an Exchange::Filter to take the underlying object from. */
		Filter(Exchange::Filter && in_that);

		virtual ~Filter();

	static const HPS::Type staticType = HPS::Type::ExchangeFilter;
		HPS::Type				ObjectType() const { return staticType; }

#if !defined(_MSC_VER) || _MSC_VER >= 1900
		Filter &				operator=(Exchange::Filter const & in_that) = default;
#endif

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this Exchange::Filter.
		 *	\param in_that An rvalue reference to an Exchange::Filter to take the underlying object from.
		 *	\return A reference to this Exchange::Filter. */
		Filter &				operator=(Exchange::Filter && in_that);


		/*! Gets the A3DEntity pointer corresponding to this Exchange::Filter.
		 *	\return The A3DEntity pointer corresponding to this Exchange::Filter. */
		A3DEntity *				GetExchangeEntity() const;
	};

	/*! The Factory class is used to create and Exchange objects which inherit from HPS::Component or one of its subclasses. */
	class EXCHANGE_API Factory : public Sprocket
	{
	public:
		/*! Creates a new Exchange::Component as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created Exchange::Component.
		 *	\param in_type The type of Exchange::Component to create.
		 *	\param in_entity The Exchange entity associated with the newly created Exchange::Component. */
		static Component	CreateComponent(HPS::Component const & in_owner, HPS::Component::ComponentType in_type, A3DEntity * in_entity = nullptr);

		/*! Creates a new Exchange::CADModel.
		 *	\param in_model The Model associated with the newly created Exchange::CADModel.
		 *	\param in_entity The Exchange entity associated with the newly created Exchange::CADModel.
		 *	\param in_owns_entity Whether the newly created Exchange::CADModel owns the associated A3DAsmModelFile (see the documentation for
		 *		Exchange::CADModel::GetEntityOwnership for more details).*/
		static CADModel		CreateCADModel(Model const & in_model = HPS::Factory::CreateModel(), A3DAsmModelFile * in_entity = nullptr, bool in_owns_entity = false);

		/*! Creates a new Exchange::Filter as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created Exchange::Filter.
		 *	\param in_entity The Exchange entity associated with the newly created Exchange::Filter. */
		static Filter		CreateFilter(HPS::Component const & in_owner, A3DEntity * in_entity = nullptr);

		/*! Creates a new Exchange::Capture as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created Exchange::Capture.
		 *	\param in_entity The Exchange entity associated with the newly created Exchange::Capture.
		 *	\param in_is_default Whether the newly created Exchange::Capture is the default capture for the CADModel, i.e., the capture that will be activated
		 *		when CADModel::ActivateDefaultCapture is invoked.  Note that if one is creating a default capture, the owner must be a CADModel (or subclass thereof)
		 *		and this will delete the old default capture on that CADModel if one was present. */
		static Capture		CreateCapture(HPS::Component const & in_owner, A3DEntity * in_entity = nullptr, bool in_is_default = false);

		/*! Creates a new Exchange::Sheet as a subcomponent of a given Component.
		 *	\param in_owner The HPS::Component which owns the newly created Exchange::Sheet.
		 *	\param in_entity The Exchange entity associated with the newly created Exchange::Sheet. */
		static Sheet		CreateSheet(HPS::Component const & in_owner, A3DEntity * in_entity = nullptr);

		/*! Returns the component at whose level changes can be made without influencing other instances of the same component.
		 *  The ComponentPath passed to this function needs to include a component of type Representation Item.
		 * \param in_component_path The ComponentPath to operate on.
		 * \return The newly de-instanced Component. */
		static Component	DeInstanceComponent(HPS::ComponentPath const & in_component_path);

	private:
		//! Private default constructor to prevent instantiation.
		Factory();
	};

	/*! The ImportNotifier class is a smart-pointer that is tied to a file import.  It is used to interact with an ongoing import or get the results from a completed import. */
	class EXCHANGE_API ImportNotifier : public IONotifier
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
		 * 	\return A reference to this ImportNotifier. */
		ImportNotifier &		operator=(ImportNotifier && in_that);

		virtual ~ImportNotifier();

	static const HPS::Type staticType = HPS::Type::ExchangeImportNotifier;
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
		Exchange::CADModel		GetCADModel() const;

		/*!	Get the PRC Read Helper for the file import.  Throws an IOException if the import is not complete, was not successful or was canceled.
		 *	The returned value will only be non-null if the Exchange::ImportOptionsKit::SetPRCReadHelper option was set to <span class='code'>true</span>
		 *	and the file that was imported was a PRC file or a PRC stream.
		 *	\return The <span class='code'>A3DRWParamsPrcReadHelper *</span> created during a successful file import.
		 *	\sa Exchange::ImportOptionsKit::SetPRCReadHelper */
		A3DRWParamsPrcReadHelper * GetPRCReadHelper() const;

		/*!	Get the number of milliseconds it took Exchange to read the CAD file(s) and generate the corresponding PRC data.
		 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
		Time					GetImportTime() const;

		/*!	Get the number of milliseconds it took Visualize to parse the PRC data and create the corresponding scene graph.
		 *	Throws an IOException if the import is not complete, was not successful or was canceled. */
		Time					GetParseTime() const;
	};

	/*! The TranslationNotifier class is a smart-pointer that is tied to a file translation.  It is used to interact with an ongoing translation or get the results from a completed translation. */
	class EXCHANGE_API TranslationNotifier : public IONotifier
	{
	public:
		/*! The default constructor creates an TranslationNotifier object which is not tied to any file translation. */
		TranslationNotifier();

		/*! The copy constructor creates a new TranslationNotifier object that is associated with the same file translation as the source TranslationNotifier.
		 * 	\param in_that The source TranslationNotifier to copy. */
		TranslationNotifier(TranslationNotifier const & in_that);

		/*! The conversion constructor creates a new derived TranslationNotifier object from a base IONotifier object.
		 * 	The copy will only be successful if the source notifier is really an upcast of this notifier type.
		 *	Otherwise the copy will fail and the resulting TranslationNotifier will be invalid.
		 * 	\param in_that The source IONotifier to copy. */
		TranslationNotifier(IONotifier const & in_that);

		/*! The move constructor creates an TranslationNotifier by transferring the underlying impl of the rvalue reference to this TranslationNotifier
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an TranslationNotifier to take the impl from. */
		TranslationNotifier(TranslationNotifier && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this TranslationNotifier thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an TranslationNotifier to take the impl from.
		 * 	\return A reference to this TranslationNotifier. */
		TranslationNotifier &		operator=(TranslationNotifier && in_that);

		virtual ~TranslationNotifier();

	static const HPS::Type staticType = HPS::Type::ExchangeTranslationNotifier;
		HPS::Type							ObjectType() const { return staticType; };

		/*! Associate this TranslationNotifier with the same file translation as the source TranslationNotifier.
		 *	\param in_that The source TranslationNotifier for the assignment.
		 *	\return A reference to this TranslationNotifier. */
		TranslationNotifier &				operator=(TranslationNotifier const & in_that);

		/*! Associate this TranslationNotifier with the same file translation as the source TranslationNotifier.
		 *	\param in_that The source TranslationNotifier for the assignment. */
		void								Assign(TranslationNotifier const & in_that);

		/*!	Get the Parasolid parts obtained from the translation.  Throws an IOException if the translation is not complete, was not successful or was canceled.
		 *  \return The Parasolid Parts for a successful file translation. */
		Exchange::ParasolidEntityArray		GetParasolidParts() const;

		/*!	Get the number of milliseconds it took Exchange to read the CAD file(s) and generate the corresponding PRC data.
		 *	Throws an IOException if the translation is not complete, was not successful or was canceled. */
		HPS::Time							GetImportTime() const;

		/*!	Get the number of milliseconds it took Exchange to translate the PRC data into Parasolid data.
		 *	Throws an IOException if the translation is not complete, was not successful or was canceled. */
		HPS::Time							GetTranslationTime() const;
	};

	/*! The ExportNotifier class is a smart-pointer that is tied to a file export.  It is used to interact with an ongoing export or get the results from a completed export. */
	class EXCHANGE_API ExportNotifier : public IONotifier
	{
	public:
		/*! The default constructor creates an ExportNotifier object which is not tied to any file export. */
		ExportNotifier();

		/*! The copy constructor creates a new ExportNotifier object that is associated with the same file export as the source ExportNotifier.
		 * 	\param in_that The source ExportNotifier to copy. */
		ExportNotifier(ExportNotifier const & in_that);

		/*! The conversion constructor creates a new derived ExportNotifier object from a base IONotifier object.
		 * 	The copy will only be successful if the source notifier is really an upcast of this notifier type.
		 *	Otherwise the copy will fail and the resulting ExportNotifier will be invalid.
		 * 	\param in_that The source IONotifier to copy. */
		ExportNotifier(IONotifier const & in_that);

		/*! The move constructor creates an ExportNotifier by transferring the underlying object of the rvalue reference to this ExportNotifier
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ExportNotifier to take the underlying object from. */
		ExportNotifier(ExportNotifier && in_that);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportNotifier thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ExportNotifier to take the underlying object from.
		 * 	\return A reference to this ExportNotifier. */
		ExportNotifier &		operator=(ExportNotifier && in_that);

		virtual ~ExportNotifier();

	static const HPS::Type staticType = HPS::Type::ExchangeExportNotifier;
		HPS::Type				ObjectType() const { return staticType; };

		/*! Associate this ExportNotifier with the same file export as the source ExportNotifier.
		 *	\param in_that The source ExportNotifier for the assignment.
		 *	\return A reference to this ExportNotifier. */
		ExportNotifier &		operator=(ExportNotifier const & in_that);

		/*! Associate this ExportNotifier with the same file export as the source ExportNotifier.
		 *	\param in_that The source ExportNotifier for the assignment. */
		void					Assign(ExportNotifier const & in_that);

		/*!	Get the A3DAsmModelFile created during the export.  Throws an IOException if the export is not complete, was not successful or was canceled.
		 *	It will be up to the user to delete this A3DAsmModelFile via <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__modelfile.html#ga147b1bb7013909e3eacea23bb5bf80c2">A3DAsmModelFileDelete</a>.
		 *	\return The A3DAsmModelFile for a successful file export. */
		A3DAsmModelFile *		GetModelFile() const;
	};

	/*! The ReloadNotifier class is a smart-pointer that is tied to a Component::Reload or a CADModel::Reload.  It is used to interact with an ongoing reload. */
	class EXCHANGE_API ReloadNotifier : public IONotifier
	{
	public:
		/*! The default constructor creates an ReloadNotifier object which is not tied to any component reload. */
		ReloadNotifier();

		/*! The copy constructor creates a new ReloadNotifier object that is associated with the same component reload as the source ReloadNotifier.
		 * 	\param in_that The source ReloadNotifier to copy. */
		ReloadNotifier(ReloadNotifier const & in_that);

		/*! The conversion constructor creates a new derived ReloadNotifier object from a base IONotifier object.
		 * 	The copy will only be successful if the source notifier is really an upcast of this notifier type.
		 *	Otherwise the copy will fail and the resulting ReloadNotifier will be invalid.
		 * 	\param in_that The source IONotifier to copy. */
		ReloadNotifier(IONotifier const & in_that);

		/*! The move constructor creates an ReloadNotifier by transferring the underlying object of the rvalue reference to this ReloadNotifier
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ReloadNotifier to take the underlying object from. */
		ReloadNotifier(ReloadNotifier && in_that);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ReloadNotifier thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ReloadNotifier to take the underlying object from.
		 * 	\return A reference to this ReloadNotifier. */
		ReloadNotifier &		operator=(ReloadNotifier && in_that);

		virtual ~ReloadNotifier();

	static const HPS::Type staticType = HPS::Type::ExchangeReloadNotifier;
		HPS::Type				ObjectType() const { return staticType; };

		/*! Associate this ReloadNotifier with the same component reload as the source ReloadNotifier.
		 *	\param in_that The source ReloadNotifier for the assignment.
		 *	\return A reference to this ReloadNotifier. */
		ReloadNotifier &		operator=(ReloadNotifier const & in_that);

		/*! Associate this ReloadNotifier with the same component reload as the source ReloadNotifier.
		 *	\param in_that The source ReloadNotifier for the assignment. */
		void					Assign(ReloadNotifier const & in_that);
	};


	class ImportOptionsKit;
	class ModelFileImportOptionsKit;

#if TARGET_OS_ANDROID == 0 && TARGET_OS_IPHONE == 0
	class Export3MFOptionsKit;
	class ExportFBXOptionsKit;
	class ExportACISOptionsKit;
	class ExportJTOptionsKit;
	class ExportPRCOptionsKit;
	class ExportSTLOptionsKit;
	class ExportOBJOptionsKit;
	class ExportU3DOptionsKit;
	class ExportXMLOptionsKit;
#endif

#if !defined(TARGET_OS_ANDROID)
	class ExportSTEPOptionsKit;
	class ExportIGESOptionsKit;
#endif

	/*! \enum BRepMode
	 *	Enumerates the ways in which to import BRep and tessellation data for files imported via Exchange. */
	enum class BRepMode
	{
		/*! Only tessellation data will be requested from Exchange, which will tessellate items as necessary or extract existing tessellations as appropriate.
		 *	No BRep data will be available for the file if this mode is used. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga224bb061827a61351f3c4df119e1b54fa46a6ed59e94171abe6280e61541e7825">kA3DReadTessOnly</a>) */
		TessellationOnly,
		/*! Only BRep data will be requested from Exchange, and each representation item will be tessellated individually as necessary.  This can potentially lead to a lower
		 *	peak memory level at the expense of a potentially longer import. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga224bb061827a61351f3c4df119e1b54fadce0e3caf9bb3001dbd634134d986b08">kA3DReadGeomOnly</a>) */
		BRepOnly,
		/*! BRep and tessellation data will be requested from Exchange, which will perform the tessellations in one shot rather than on demand.  This can potentially lead to
		 *	a higher peak memory level with the benefit of a potentially faster import. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga224bb061827a61351f3c4df119e1b54faff06369f761ac6b0ecbef7eba076fcdc">kA3DReadGeomAndTess</a>) */
		BRepAndTessellation,
	};

	/*! \enum ImportMode
	 *	Enumerates the ways in which to perform file import via Exchange. */
	enum class ImportMode
	{
		/*! The standard form of import.  Import options will be respected as to what should be loaded (solids, surfaces, PMI, etc.), but all specified data will be imported
		 *	for all product occurrences within a file. */
		Complete,
		/*! Only the structure of the CAD file will be imported.  Product occurrences within that structure can then be loaded on demand.
		 *	This type of import can only be performed for certain formats, in particular, SolidWorks, NX (Unigraphics), Creo (Pro/E), and CATIA V5 files. */
		Incremental,
	};

	/*! \enum SurfaceType
	 *	Enumerates the types of surfaces supported by Exchange */
	enum class SurfaceType
	{
		Blend01,	//!< Blend Type 1 surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa6d78028a580b839d078d1f6eaabac2f8">kA3DTypeSurfBlend01</a>).
		Blend02,	//!< Blend Type 2 surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa9d5537ab59f3478daf5ae6cf540887b2">kA3DTypeSurfBlend02</a>).
		Blend03,	//!< Blend Type 3 surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fad696ed2eac7ceb31f9c628f3eafaaf6d">kA3DTypeSurfBlend03</a>).
		Blend04,	//!< Blend Type 4 surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faa3c3daf1e7aed1ea0113b45101a174b9">kA3DTypeSurfBlend04</a>).
		NURBS,		//!< NURBS surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa2428b6005638a044784ab1cda332486e">kA3DTypeSurfNurbs </a>).
		Cone,		//!< Conical surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fab8a1bb7e057a49370de9ab3b8d73562b">kA3DTypeSurfCone </a>).
		Cylinder,	//!< Cylindrical surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fae14c03c86421b3b8222b3e8626c7ccb1">kA3DTypeSurfCylinder </a>).
		Offset,		//!< Offset surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fab3bc669513d40ccc4830c8491b5b92e5">kA3DTypeSurfOffset </a>).
		Pipe,		//!< Pipe surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faf2aee065bf5a080f71cd08a7130cd375">kA3DTypeSurfPipe </a>).
		Plane,		//!< Planar surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fae7b5cf7965871b64567a3a038b48ce71">kA3DTypeSurfPlane </a>).
		Ruled,		//!< Ruled surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa418abf5b880963767309cda209d0d851">kA3DTypeSurfRuled </a>).
		Sphere,		//!< Spherical surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faa4bcfb2a27a6397fc1b483708e9211b0">kA3DTypeSurfSphere </a>).
		Revolution,	//!< Revolution surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70facc553d3f52442d5a8d904f60e1b6137d">kA3DTypeSurfRevolution </a>).
		Extrusion,	//!< An Extrusion (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fab7a9d9fe15f73d9a3dfba0426b2fefdd">kA3DTypeSurfExtrusion </a>).
		FromCurve,	//!< Surface defined from a curve (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa2c1e680be58c71b13b28c9b797f4674d">kA3DTypeSurfFromCurves</a>).
		Torus,		//!< Torus surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fabb93f0ee0640edb3d785bd27c58ac9be">kA3DTypeSurfTorus</a>).
		Transform,	//!< Transform (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa775ddce62e6056444e9d0b9b55b38603">kA3DTypeSurfTransform </a>).
	};

	/*! \enum CurveType
	 *	Enumerates the types of curves supported by Exchange */
	enum class CurveType
	{
		Blend,			//!< Blend boundary curve (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fafa98e46014b1cf776a9182ee0e21c83a">kA3DTypeCrvBlend02Boundary </a>).
		NURBS,			//!< NURBS curve (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa05c231917815ee81b9c5669003cd7f4a">kA3DTypeCrvNurbs </a>).
		Circle,			//!< Circle (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faf736c4f75c8441c7e1abc0c99adc334c">kA3DTypeCrvCircle </a>).
		Composite,		//!< Array of oriented curves (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa9656f7fb58da472779493b3f17d79efd">kA3DTypeCrvComposite </a>).
		CurveOnSurface,	//!< Curve defined by a UV curve on a surface (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fac2812a3380dca8c6f51c89b4ccab2c5f">kA3DTypeCrvOnSurf </a>).
		Ellipse,		//!< Ellipse (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa72aeea6c362abd4eaa0d755632a37a55">kA3DTypeCrvEllipse </a>).
		Equation,		//!< Curve described by an equation (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70facf41f99af6690f9d7132fc8180cf3f75">kA3DTypeCrvEquation </a>).
		Helix,			//!< Helix (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faeee69df5472dc470838e8471a82de0fc">kA3DTypeCrvHelix </a>).
		Hyperbola,		//!< Hyperbola (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa7a354675b19e229c18030340fc4bb2a0">kA3DTypeCrvHyperbola </a>).
		Intersection,	//!< Intersection between two surfaces (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faf104910ed894582f5c94250900a43f2a">kA3DTypeCrvIntersection </a>).
		Line,			//!< Line (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa89e2246bca62ddf013006c8d161afb7a">kA3DTypeCrvLine </a>).
		Offset,			//!< Offset curve (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70faa8b51eb76e8c1b180a16a8b54100cbcc">kA3DTypeCrvOffset </a>).
		Parabola,		//!< Parabola (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fab4cee3824af772abf253c526b37c053f">kA3DTypeCrvParabola </a>).
		Polyline,		//!< Polyhedric curve (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa1dafbfc6d38db0d928843f3215d6d054">kA3DTypeCrvPolyLine </a>).
		Transform,		//!< Transformed curve (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__types__enum.html#gga4e3c6bc1e67a6f663cd1ee85ef7dc70fa6a2cc9b0a88a80edd801b1e4caf60089">kA3DTypeCrvTransform </a>).
	};

	typedef std::vector<SurfaceType, HPS::Allocator<SurfaceType> >	SurfaceTypeArray; 
	typedef std::vector<CurveType, HPS::Allocator<CurveType> >		CurveTypeArray; 

	/*! \enum AnnotationCaptureFitting
	 *	Enumerates the type of data in the scene to use to generate cameras for annotation captures. */
	enum class AnnotationCaptureFitting
	{
		PMIOnly,		//!< Use only the bounds of the PMI referenced by the annotation capture to compute the camera.
		ModelAndPMI		//!< Use the bounds of the model and the PMI referenced by the annotation capture to compute the camera.
	};

	/*! The Exchange::Tessellation class is a concept class for tessellation-related enums. */
	class EXCHANGE_API Tessellation
	{
	public:
		/*! \enum Type
		 *	Enumerates the types of tessellation settings that can be used by Exchange to generate tessellation data. */
		enum class Type
		{
			Standard,		//!< Tessellation data will be generated using a standard predefined level.
			Custom			//!< Tessellation data will be generated using custom user-defined settings.
		};


		/*! \enum Level
		 *	Enumerates the standard tessellation levels for files imported or exported via Exchange. */
		enum class Level
		{
			ExtraLow,		//!< Pre-defined extra low tessellation level (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4a7d56f0e70c3e2577034c5da08e505dd7">kA3DTessLODExtraLow</a>).
			Low,			//!< Pre-defined low tessellation level (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4aed58273421d2cf223dfe115dfd7ec38a">kA3DTessLODLow</a>).
			Medium,			//!< Pre-defined medium tessellation level (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4a477350749f0dce5e32ac2e00b28cb0ff">kA3DTessLODMedium</a>).
			High,			//!< Pre-defined high tessellation level (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4a035a3610c9c8a9c5297abca994f68aaa">kA3DTessLODHigh</a>).
			ExtraHigh,		//!< Pre-defined extra high tessellation level (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4a2fd5f670623679794d0c9d547025db39">kA3DTessLODExtraHigh</a>).
		};

		/*! The Exchange::Tessellation::Chord class is a concept class for chord-tessellation-related enums. */
		class EXCHANGE_API Chord
		{
		public:
			/*! \enum Limit
			 *	Enumerates the type of chord limit that will be used to generate tessellation. */
			enum class Limit
			{
				/*! The limit will correspond to the chord height ratio (either <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a5920a88f4fd5b6eda162b6766a307a74">A3DRWParamsTessellationData::m_dChordHeightRatio</a>
				 *	or <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a8cad7ca4cdadfc03fd122f4b9523c012">A3DRWParamsExportStlData::m_dChordHeightRatio</a>).*/
				Ratio,
				/*! The limit will correspond to the maximum chord height (either <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a3ec6ccd454c6a79432e08b2b1da0331d">A3DRWParamsTessellationData::m_dMaxChordHeight</a>
				 *	or <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#af550c79671cc6e8ca577ec7ba0e7af14">A3DRWParamsExportStlData::m_dMaxChordHeight</a>).*/
				Height
			};

		private:
			//! Private default constructor to prevent instantiation.
			Chord();
		};

		/*! \enum Accuracy
		 *	Enumerates the tessellation accuracy modes for files imported or exported via HOOPS Exchange. */
		enum class Accuracy
		{
			/*! Tessellation suitable for visualization (either <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a004de12aa32476c3139ba8f87f0c62e8">A3DRWParamsTessellationData::m_bAccurateTessellation</a>
			 *	or <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a8a0da6bf835e5ad144ab35a4a006ad08">A3DRWParamsExportStlData::m_bAccurateTessellation</a> set to <span class='code'>false</span>)*/
			Standard,
			/*! Tessellation suitable for analysis (either <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a004de12aa32476c3139ba8f87f0c62e8">A3DRWParamsTessellationData::m_bAccurateTessellation</a>
			 *	or <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a8a0da6bf835e5ad144ab35a4a006ad08">A3DRWParamsExportStlData::m_bAccurateTessellation</a> set to <span class='code'>true</span>)*/
			Accurate,
			/*! Tessellation suitable for analysis (either <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a004de12aa32476c3139ba8f87f0c62e8">A3DRWParamsTessellationData::m_bAccurateTessellationWithGrid </a>*/
			GridAccurate
		};

	private:
		//! Private default constructor to prevent instantiation.
		Tessellation();
	};

	/*! The Exchange::JT class is a concept class for JT-related enums. */
	class EXCHANGE_API JT
	{
	public:
		/*! \enum Content
		 *	Enumerates the type of content to export to a JT file. */
		enum class Content
		{
			Geometry,					//!< Export only B-rep geometry data (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#ggae0889438e401da5d62d47dde87fa4490a089a64395fa6747220094faccea5f255">kA3DWriteGeomOnly</a>).
			Tessellation,				//!< Export only tessellation data (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#ggae0889438e401da5d62d47dde87fa4490a5d93a71c0f9755aa22f6f3285f06cbc3">kA3DWriteTessOnly</a>).
			GeometryAndTessellation		//!< Export BRep geometry and tessellation data (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#ggae0889438e401da5d62d47dde87fa4490af2a197b32a2657f015a3f88b3452e0b6">kA3DWriteGeomAndTess</a>).
		};

		/*! \enum TessellationLevel
		 *	Enumerates the tessellation levels used when importing JT files. */
		enum class TessellationLevel
		{
			Low,							//!< Load the lowest level of tessellation available in the JT file. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_j_t_data.html#a8c3bc6faa67639dd110bd3d1655b22e6">A3DRWParamsJTData::m_eReadTessellationLevelOfDetail = JTTessLODLow</a>).
			Medium,							//!< Load the medium level of tessellation available in the JT file. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_j_t_data.html#a8c3bc6faa67639dd110bd3d1655b22e6">A3DRWParamsJTData::m_eReadTessellationLevelOfDetail = JTTessLODMedium</a>).
			High,							//!< Load the highest level of tessellation available in the JT file. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_j_t_data.html#a8c3bc6faa67639dd110bd3d1655b22e6">A3DRWParamsJTData::m_eReadTessellationLevelOfDetail = JTTessLODHigh</a>).
		};

		/*! \enum Version
		 *	Enumerates the version of the JT format to export to a JT file. */
		enum class Version
		{
			JT81,		//!< Version 8.1 of the JT format. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__write.html#gga1f509439985212ac69c6231e5f141cbda0245eb6a3278c29be7e527235f515c87">kA3DE_JT81</a>).
			JT95,		//!< Version 9.5 of the JT format. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__write.html#gga1f509439985212ac69c6231e5f141cbdaacd6800f6092e4a69975406a95e73020">kA3DE_JT95</a>).
			JT100,		//!< Version 10.0 of the JT format. (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d__write.html#gga1f509439985212ac69c6231e5f141cbdaad06da40040c09a7e531be2ae3fd68a4">kA3DE_JT100</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		JT();
	};

	/*! The Exchange::STEP class is a concept class for STEP-related enums. */
	class EXCHANGE_API STEP
	{
	public:
		/*! \enum Field
		 *	Enumerates the field from which to get the name for a product occurrence on import of a STEP file. */
		enum class Field
		{
			ProductName,								//!< Get the name from from the PRODUCT field (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a4bc8af17dc3072ff46e83feb1dfe874f">A3DRWParamsStepData::m_bPreferProductName = true</a>).
			NextAssemblyUsageOccurrenceID,				//!< Get the name from the first field of NEXT_ASSEMBLY_USAGE_OCCURRENCE (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga2d1f4178578f7301c0253053c1d4bb1ea7d570fb3c28088f2ccc295bfcc37646d">kA3DStepNameFromNAUO_ID</a>).
			NextAssemblyUsageOccurrenceName,			//!< Get the name from the second field of NEXT_ASSEMBLY_USAGE_OCCURRENCE (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga2d1f4178578f7301c0253053c1d4bb1eadb9050d353e2b9bbcc79a2176ec1f8e3">kA3DStepNameFromNAUO_NAME</a>).
			NextAssemblyUsageOccurrenceDescription		//!< Get the name from the third field of NEXT_ASSEMBLY_USAGE_OCCURRENCE (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga2d1f4178578f7301c0253053c1d4bb1ea904c71c2337bff853003eae0d8dc200d">kA3DStepNameFromNAUO_DESCRIPTION</a>).
		};

		/*! \enum Format
		 *	Enumerates the format to use when exporting a STEP file. */
		enum class Format
		{
			AP203,		//!< The AP 203 STEP format (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga64358e83bace29e1f9ba47dc14817194a9ee65bcdb4888d9da54e835da01fc67e">kA3DStepAP203</a>).
			AP214,		//!< The AP 214 STEP format (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga64358e83bace29e1f9ba47dc14817194ab9322a2755751a0685af3901d558e477">kA3DStepAP214</a>).
			AP242,		//!< The AP 242 STEP format (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga64358e83bace29e1f9ba47dc14817194a74d5053523555add169b131e9b2ac325">kA3DStepAP242</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		STEP();
	};

	/*! The Exchange::PRC class is a concept class for PRC-related enums. */
	class EXCHANGE_API PRC
	{
	public:
		/*! \enum BRepCompression
		 *	Enumerates the compression to apply to BRep data when exporting a PRC file. */
		enum class BRepCompression
		{
			None,		//!< No compression (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#afbbe41721d6878768a624ea8d0894531">A3DRWParamsExportPrcData::m_bCompressBrep = false</a>).
			Low,		//!< Low compression, high accurracy (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga34066fd7757e486fd13543922ea12df8a2d7e4562fd9b672689989fd175e48689">kA3DCompressionLow</a>).
			Medium,		//!< Medium compression, medium accurracy (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga34066fd7757e486fd13543922ea12df8abef195d08d1adc08395ef6429a9d18a3">kA3DCompressionMedium</a>).
			High		//!< High compression, low accurracy (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga34066fd7757e486fd13543922ea12df8a005a7f0fc8cc13745541de441010b844">kA3DCompressionHigh</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		PRC();
	};

	/*! The Exchange::ProE class is a concept class for ProE-related enums. */
	class EXCHANGE_API ProE
	{
	public:
		/*! \enum SessionColor
		 *	Enumerates what colors should be used for PMI in Wildfire files which have no color. */
		enum class SessionColor
		{
			/*! Use the default session color for the latest version of Creo supported by Exchange (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga1d4b5720237bd57a0c768eb4601f1f2eac3dce52cc5295b50bd828dcf9ac24ec6">kA3DLastCreoVersionSessionColor</a>). */
			LastCreoVersion,
			/*! Use the default session color for Exchange (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga1d4b5720237bd57a0c768eb4601f1f2eaf9d996efd1533f8657f927c08a942dc0">kA3DHExchangeSessionColor</a>). */
			ExchangeDefault,
			/*! Use the default PMI color specified in the import options (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#gga1d4b5720237bd57a0c768eb4601f1f2ea1cc10af424fe8b1d7d3332e4901f7125">kA3DFileVersionSessionColor</a>).
			 *	\sa Exchange::ImportOptionsKit::SetPMIDefaultColor */
			UserDefined
		};

		/*! \enum FamilyTable
		 *	Enumerates how to import Family Tables. */
		enum class FamilyTable
		{
			/*! Use BRep available in Accelerator files (.xpr and .xas), if available.(<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ga98a08928de9f931c0dcfd7b93e45bad7">A3DProEFamTabAcceleratorFileOnly</a>). */
			BRepOnly,
			/*! Use BRep available in Accelerator files (.xpr and .xas), if available. If BRep is not available use tessellation, when available(<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ga98a08928de9f931c0dcfd7b93e45bad7">A3DProEFamTabOrUseTessellation</a>). */
			BRepOrTessellation,
			/*! Always import, even if no tessellation and no BRep is available, in which case a wire generic entity will be imported.(<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ga98a08928de9f931c0dcfd7b93e45bad7">A3DProEFamTabOrUseWireAndGeneric</a>). */
			BRepOrTessellationOrGeneric
		};

		/*! \enum ConstructionEntities
		 *	Enumerates how to import Construction Entities. */
		enum class ConstructionEntities
		{
			/*! Import construction entities.(<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaceade8ef4d8365b15a85dfb80739330eadd5e7c57acc1e297783c97237deb5ee7">A3DProEReadConstructEntities_Yes</a>). */
			On,
			/*! Do not import construction entities.(<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaceade8ef4d8365b15a85dfb80739330ea44fd5c60c4a65e3864012e12e43b6d5d">A3DProEReadConstructEntities_No</a>). */
			Off,
			/*! Use the datum setting to determine whether to import construction entities.(<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggaceade8ef4d8365b15a85dfb80739330ea9c3136bd17ab403d9e523d10bf4e3623">A3DProEReadConstructEntities_AsDatum</a>). */
			AsDatum,
		}; 

	private:
		//! Private default constructor to prevent instantiation.
		ProE();
	};

	/*! The Exchange::U3D class is a concept class for U3D-related enums. */
	class EXCHANGE_API U3D
	{
	public:
		/*!	\enum Version
		 *	Enumerates the version to use when exporting a U3D file. */
		enum class Version
		{
			ECMA1,		//!< ECMA-363, version 1 (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga0e9211137820530ecc855326c34ede9da2c87e9374ce89144bbd3aa9d35fd8b53">kA3DECMA1</a>).
			ECMA3		//!< ECMA-363, version 3 (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__write.html#gga0e9211137820530ecc855326c34ede9da6c4f24519bde58271432c798c78ff5af">kA3DECMA3</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		U3D();
	};

	/*! The Exchange::Revit class is a concept class for Revit-related enums. */
	class EXCHANGE_API Revit
	{
	public:
		enum class PhysicalProperties
		{
			/*! Physical properties are not read. */
			NotComputed,
			/*! Physical properties are computed during reading process and stored as metadata. */
			Computed
		};

	private:
		//! Private default constructor to prevent instantiation
		Revit();
	};

	/*! The Configuration class is a user space object.  It is a used to get configuration information for a CAD file. */
	class EXCHANGE_API Configuration : public HPS::Sprocket
	{
	public:
		/*! The default constructor creates an empty Configuration object. */
		Configuration();

		/*! This constructor creates a Configuration with the given name and no subconfigurations.
		 *	\param in_name UTF8-encoded string with the name for this Configuration. */
		Configuration(char const * in_name);

		/*! This constructor creates a Configuration with the given name and the given subconfigurations
		 *	\param in_name UTF8-encoded string with the name for this Configuration. 
		 *	\param in_count Size of the following array.
		 *	\param in_subconfigurations The subconfigurations for this Configuration. */
		Configuration(char const * in_name, size_t in_count, Configuration const in_subconfigurations[]);

		/*! This constructor creates a Configuration with the given name and the given subconfigurations
		 *	\param in_name UTF8-encoded string with the name for this Configuration. 
		 *	\param in_subconfigurations The subconfigurations for this Configuration. */
		Configuration(char const * in_name, ConfigurationArray const & in_subconfigurations);

		/*! The copy constructor creates a new Configuration object that contains the same settings as the source Configuration.
		 * 	\param in_configuration The source Configuration to copy. */
		Configuration(Configuration const & in_configuration);

		/*! The move constructor creates an Configuration by transferring the underlying impl of the rvalue reference to this Configuration
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an Configuration to take the impl from. */
		Configuration(Configuration && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this Configuration thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an Configuration to take the impl from.
		 * 	\return A reference to this Configuration. */
		Configuration & operator=(Configuration && in_that);

		virtual ~Configuration();

	static const HPS::Type staticType = HPS::Type::ExchangeConfiguration;
		HPS::Type				ObjectType() const { return staticType; }


		/*! Copies the source Configuration into this Configuration.
		 * 	\param in_kit The source Configuration to copy. */
		void					Set(Configuration const & in_kit);

		/*! Copies this Configuration into the given Configuration.
		 * 	\param out_kit The Configuration to populate with the contents of this Configuration. */
		void					Show(Configuration & out_kit) const;

		/*! Copies the source Configuration into this Configuration.
		 * 	\param in_kit The source Configuration to copy.
		 * 	\return A reference to this Configuration. */
		Configuration &			operator=(Configuration const & in_kit);

		/*! Indicates whether this Configuration has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this Configuration, <span class='code'>false</span> otherwise. */
		bool					Empty() const;

		/*!	Check if the source Configuration is equivalent to this Configuration.
		 *	\param in_kit The source Configuration to compare to this Configuration.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(Configuration const & in_kit) const;

		/*!	Check if the source Configuration is equivalent to this Configuration.
		 *	\param in_kit The source Configuration to compare to this Configuration.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(Configuration const & in_kit) const;

		/*!	Check if the source Configuration is not equivalent to this Configuration.
		 *	\param in_kit The source Configuration to compare to this Configuration.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(Configuration const & in_kit) const;


		/*! Gets the name for this Configuration.
		 *	\return The name for this Configuration. */
		HPS::UTF8				GetName() const;

		/*! Gets the subconfigurations for this Configuration.
		 *	\return The subconfigurations for this Configuration. */
		ConfigurationArray		GetSubconfigurations() const;
	};

	/*!	The File class provides functions to import and export CAD files via Exchange. */
	class EXCHANGE_API File
	{
	public:
		/*!	\enum Format
		 *	Enumerates the formats Exchange can recognize. */
		enum class Format
		{
			Unsupported,			//!< Format cannot be imported via Exchange.
			ACIS,					//!< ACIS file (<span class='code'>A3DEModellerType::kA3DModellerAcis</span>).
			CADDS,					//!< CADDS file (<span class='code'>A3DEModellerType::kA3DModellerCadds</span>).  This format requires the "From The Vault" libraries to import.
			CATIAV4,				//!< CATIA V4 file (<span class='code'>A3DEModellerType::kA3DModellerCatia</span>).
			CATIAV5,				//!< CATIA V5 file (<span class='code'>A3DEModellerType::kA3DModellerCatiaV5</span>).
			CGR,					//!< CATIA Graphical Represenation file (<span class='code'>A3DEModellerType::kA3DModellerCgr</span>).
			COLLADA,				//!< COLLADA (DAE) file (<span class='code'>A3DEModellerType::kA3DModellerDAE</span>).
			CreoProE,				//!< Creo (Pro/E) file (<span class='code'>A3DEModellerType::kA3DModellerProE</span>).
			DWG,					//!< DWG file (<span class='code'>A3DEModellerType::kA3DModellerDWG</span>).
			DXF,					//!< Dxf file (<span class='code'>A3DEModellerType::kA3DModellerDxf</span>).
			IDEAS,					//!< I-DEAS file (<span class='code'>A3DEModellerType::kA3DModellerIdeas</span>).
			IFC,					//!< Industry Foundation Classes (IFC) file (<span class='code'>A3DEModellerType::kA3DModellerIFC</span>).
			IGES,					//!< IGES file (<span class='code'>A3DEModellerType::kA3DModellerIges</span>).
			Inventor,				//!< Autodesk Inventor file (<span class='code'>A3DEModellerType::kA3DModellerInventor</span>).
			JT,						//!< JT file (<span class='code'>A3DEModellerType::kA3DModellerJt</span>).
			KeyholeMarkupLanguage,	//!< Keyhole Markup Language (KMZ) file (<span class='code'>A3DEModellerType::kA3DModellerKMZ</span>).  This format requires the "From The Vault" libraries.
			LatticeXVL,				//!< Lattice XVL file (<span class='code'>A3DEModellerType::kA3DModellerXvl</span>).  This format requires the "From The Vault" libraries to import.
			OneSpaceDesigner,		//!< OneSpace Designer file (<span class='code'>A3DEModellerType::kA3DModellerOneSpaceDesigner</span>).  This format requires the "From The Vault" libraries to import.
			Parasolid,				//!< Parasolid file (<span class='code'>A3DEModellerType::kA3DModellerParasolid</span>).
			PDF,					//!< PDF file (<span class='code'>A3DEModellerType::kA3DModellerPDF</span>).  PRC or U3D data stored inside the PDF can be imported.
			PRC,					//!< Product Representation Compact (PRC) file (<span class='code'>A3DEModellerType::kA3DModellerPrc</span>).
			Rhino,					//!< Rhinocerous 3D file (<span class='code'>A3DEModellerType::kA3DModellerRhino</span>).
			NXUnigraphics,			//!< NX (Unigraphics) file (<span class='code'>A3DEModellerType::kA3DModellerUnigraphics</span>).
			SolidEdge,				//!< Solid Edge file (<span class='code'>A3DEModellerType::kA3DModellerSE</span>).
			SolidWorks,				//!< SolidWorks file (<span class='code'>A3DEModellerType::kA3DModellerSlw</span>).
			STEP,					//!< STEP file (<span class='code'>A3DEModellerType::kA3DModellerStep</span>).
			StereoLithography,		//!< Stereo Lithography (STL) file (<span class='code'>A3DEModellerType::kA3DModellerStl</span>).
			ThreeDStudioMax,		//!< Autodesk 3D Studio Max file (<span class='code'>A3DEModellerType::kA3DModeller3DS</span>).
			ThreeDXML,				//!< 3DXML file (<span class='code'>A3DEModellerType::kA3DModeller3dxml</span>).
			Universal3D,			//!< Universal 3D (U3D) file (<span class='code'>A3DEModellerType::kA3DModellerU3D</span>).
			VDAFS,					//!< VDA-FS file (<span class='code'>A3DEModellerType::kA3DModellerVda</span>).
			VRML,					//!< VRML file (<span class='code'>A3DEModellerType::kA3DModellerWrl</span>).
			WavefrontObject,		//!< Wavefront Object (OBJ) file (<span class='code'>A3DEModellerType::kA3DModellerOBJ</span>).
			Revit,					//!< Revit file (<span class='code'>A3DEModellerType::kA3DModellerRevit</span>).  This format is only available on 64bit compilers.
			GLTF,					//!< GLTF file (<span class='code'>A3DEModellerType::kA3DModellerGLTF</span>).
			DWF,					//!< Autodesk DWF file (<span class='code'>A3DEModellerType::kA3DModellerDwf</span>).
			FBX,					//!< FBX file (<span class='code'>A3DEModellerType::kA3DModellerFbx</span>).
			STEPXML,				//!< STEP XML file (<span class='code'>A3DEModellerType::kA3DModellerStepXml</span>)
		};

		/*! Gets the format for the given file as determined by Exchange.  May throw an IOException if a problem is encountered when querying the format. */
		static Format GetFormat(char const * in_file_name);

		/*! Gets a list of configurations for the given file (if any).  It should be called prior to Import if a file has configurations and the user wants or needs to specify the configuration to import
		 *	and requires a list of the available configurations.  If there are configurations, the function will return an array which has been populated with the list of configurations.   If there are no
		 *	configurations, the function will return an empty array.  May throw an IOException if a problem is encountered when querying the configurations. */
		static ConfigurationArray GetConfigurations(char const * in_file_name);

		/*!	Returns information associated with the given file as determined by Exchange, without performing a full conversion.
		 * CatiaV5 and Solidworks are the only formats currently supported.
		 * May thrown an IOException if a problem is encountered.
		 * Some of the fields returned may be empty.
		 *	\param in_file_name The file to query
		 *	\param out_format The format of the file
		 *	\param out_model_name The name of the model contained in the file
		 *	\param out_version The software release version
		 *	\param out_author The file author
		 *	\param out_organization The organization associated with the file
		 *	\param out_time_stamp The time stamp */
		static void GetInformation(const char * in_file_name, Format & out_format, UTF8 & out_model_name, UTF8 & out_version, UTF8 & out_author, UTF8 & out_organization, UTF8 & out_time_stamp);

		/*!	Returns the thumbnail image associated with the given file as determined by Exchange, if any.
		 * CatiaV5 and Solidworks are the only formats currently supported.
		 * May thrown an IOException if a problem is encountered.
		 *	\param in_file_name The file to query
		 *	\return An ImageKit containing the thumbnail */
		static HPS::ImageKit GetThumbnailImage(const char * in_file_name);

		/*!	Performs an asynchronous import of the specified CAD file with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 *	Note that if the import is successful, the CADModel created from this import will own its associated A3DAsmModelFile (see Exchange::CADModel::GetEntityOwnership
		 *	for more details).
		 *	\param in_file_name The name of the CAD file to import.
		 *	\param in_options The options controlling the import of the CAD file.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(char const * in_file_name, ImportOptionsKit const & in_options);

		/*!	Performs an asynchronous import of the specified A3DAsmModelFile with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 *	Note that if the import is successful, the CADModel created from this import will not own its associated A3DAsmModelFile (see Exchange::CADModel::GetEntityOwnership
		 *	for more details).
		 *	\param in_model_file The A3DAsmModelFile to import.
		 *	\param in_options The options controlling the import of the CAD file.  Whether these options will have any effect depends on how the A3DAsmModelFile was created.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(A3DAsmModelFile const * in_model_file, ModelFileImportOptionsKit const & in_options);

		/*!	Performs an asynchronous import of the specified PRC data with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 *	Note that if the import is successful, the CADModel created from this import will own its associated A3DAsmModelFile (see Exchange::CADModel::GetEntityOwnership
		 *	for more details).
		 *	\param in_byte_count Size of the following array.
		 *	\param in_prc_data Buffer of PRC data to import.
		 *	\param in_options The options controlling the import of the CAD file.  Whether these options will have any effect depends on what is contained in the PRC data.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(size_t in_byte_count, byte const in_prc_data[], ModelFileImportOptionsKit const & in_options);

		/*!	Performs an asynchronous import of the specified PRC data with the provided options.  May throw an IOException prior to starting the asynchronous import.
		 *	Note that if the import is successful, the CADModel created from this import will own its associated A3DAsmModelFile (see Exchange::CADModel::GetEntityOwnership
		 *	for more details).
		 *	\param in_prc_data Buffer of PRC data to import.
		 *	\param in_options The options controlling the import of the CAD file.  Whether these options will have any effect depends on what is contained in the PRC data.
		 *	\return An ImportNotfier object that can be used to query the import progress and status. */
		static ImportNotifier Import(ByteArray const & in_prc_data, ModelFileImportOptionsKit const & in_options);


		/*!	Asynchronously translates the specified CAD files into an array of Parasolid parts, using the options provided.
		 *  A valid Parasolid session is required for this function to succeed.
		 * 	\param in_file_name The name of the CAD file to translate.
		 *	\param in_options The options controlling the translation of the CAD file.
		 *	\param in_translation_options Options used during the translation.
		 *	\return An TranslationNotifier object that can be used to query the import progress and status. */
		static TranslationNotifier Translate(char const * in_file_name, ImportOptionsKit const & in_options, TranslationOptionsKit const & in_translation_options);

#if TARGET_OS_ANDROID == 0 && TARGET_OS_IPHONE == 0
		/*! Performs an asynchronous 3MF export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		* 	\param in_file_name The name of the file to write the 3MF data to.
		* 	\param in_options The options controlling the export of the 3MF data. */
		static ExportNotifier Export3MF(CADModel const & in_cad_model, char const * in_file_name, Export3MFOptionsKit const & in_options);

		/*! Performs an asynchronous 3MF export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile to export as 3MF.
		* 	\param in_file_name The name of the file to write the 3MF data to.
		* 	\param in_options The options controlling the export of the 3MF data.
		*	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier Export3MF(KeyPathArray const & in_source, char const * in_file_name, Export3MFOptionsKit const & in_options);

		/*! Performs an asynchronous 3MF export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_count The number of key paths stored in in_source.
		* 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile to export as 3MF.
		* 	\param in_file_name The name of the file to write the 3MF data to.
		* 	\param in_options The options controlling the export of the 3MF data.
		*	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier Export3MF(size_t in_count, KeyPath const in_source[], char const * in_file_name, Export3MFOptionsKit const & in_options);

		/*! Performs an asynchronous 3MF export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_source A key path containing the data to export.  The geometry and segments under this key path will be used to create the A3DAsmModelFile to export as 3MF.
		* 	\param in_file_name The name of the file to write the 3MF data to.
		* 	\param in_options The options controlling the export of the 3MF data.
		*	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier Export3MF(KeyPath in_source, char const * in_file_name, Export3MFOptionsKit const & in_options);

		/*! Performs a synchronous FBX export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the FBX data to.
		 * 	\param in_options The options controlling the export of the FBX data. */
		static void ExportFBX(CADModel const & in_cad_model, char const * in_file_name, ExportFBXOptionsKit const & in_options);

		/*! Performs a synchronous ACIS export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the ACIS data to.
		 * 	\param in_options The options controlling the export of the ACIS data. */
		static void ExportACIS(CADModel const & in_cad_model, char const * in_file_name, ExportACISOptionsKit const & in_options);

		/*! Performs a synchronous JT export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the JT data to.
		 * 	\param in_options The options controlling the export of the JT data. */
		static void ExportJT(CADModel const & in_cad_model, char const * in_file_name, ExportJTOptionsKit const & in_options);

		/*! Performs an asynchronous PRC export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the PRC data to.
		 * 	\param in_options The options controlling the export of the PRC data.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(CADModel const & in_cad_model, char const * in_file_name, ExportPRCOptionsKit const & in_options);

		/*! Performs an asynchronous PRC export from an A3DAsmModelFile created from a KeyPathArray.
		 * 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile to export as PRC.
		 * 	\param in_file_name The name of the file to write the PRC data to.
		 * 	\param in_compress_tessellation Whether to compress the tessellation data in the PRC data.  Defaults to <span class='code'>false</span>.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(KeyPathArray const & in_source, char const * in_file_name, bool in_compress_tessellation = false);

		/*! Performs an asynchronous PRC export from an A3DAsmModelFile created from a KeyPathArray.
		* 	\param in_count The size of in_source
		 * 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile to export as PRC.
		 * 	\param in_file_name The name of the file to write the PRC data to.
		 * 	\param in_compress_tessellation Whether to compress the tessellation data in the PRC data.  Defaults to <span class='code'>false</span>.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(size_t in_count, KeyPath const in_source [], char const * in_file_name, bool in_compress_tessellation = false);

		/*! Performs an asynchronous PRC export from an A3DAsmModelFile created from a KeyPath.
		 * 	\param in_count The size of in_source
		 * 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile to export as PRC.
		 * 	\param in_file_name The name of the file to write the PRC data to.
		 * 	\param in_compress_tessellation Whether to compress the tessellation data in the PRC data.  Defaults to <span class='code'>false</span>.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(KeyPath const & in_source, char const * in_file_name, bool in_compress_tessellation = false);

		/*! Performs an asynchronous creation of an A3DAsmModelFile from a KeyPathArray.
		 * 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(KeyPathArray const & in_source);

		/*! Performs an asynchronous creation of an A3DAsmModelFile from a KeyPathArray.
		 * 	\param in_count The size of in_source
		 * 	\param in_source An array of key paths containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(size_t in_count, KeyPath const in_source []);

		/*! Performs an asynchronous creation of an A3DAsmModelFile from a KeyPath.
		 * 	\param in_source A key path containing the data to export.  The geometry and segments under these key paths will be used to create the A3DAsmModelFile.
		 *	\return An ExportNotfier object that can be used to query the export progress and status. */
		static ExportNotifier ExportPRC(KeyPath const & in_source);

		/*! Performs a synchronous STL export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the STL data to.
		 * 	\param in_options The options controlling the export of the STL data. */
		static void ExportSTL(CADModel const & in_cad_model, char const * in_file_name, ExportSTLOptionsKit const & in_options);

		/*! Performs a synchronous OBJ export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		* 	\param in_file_name The name of the file to write the OBJ data to.
		* 	\param in_options The options controlling the export of the OBJ data. */
		static void ExportOBJ(CADModel const & in_cad_model, char const * in_file_name, ExportOBJOptionsKit const & in_options);

		/*! Performs a synchronous U3D export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the U3D data to.
		 * 	\param in_options The options controlling the export of the U3D data. */
		static void ExportU3D(CADModel const & in_cad_model, char const * in_file_name, ExportU3DOptionsKit const & in_options);

		/*! Performs a synchronous VRML export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the VRML data to. */
		static void ExportVRML(CADModel const & in_cad_model, char const * in_file_name);

		/*! Performs a synchronous XML export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		 * 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		 * 	\param in_file_name The name of the file to write the XML data to.
		 * 	\param in_options The options controlling the export of the XML data. */
		static void ExportXML(CADModel const & in_cad_model, char const * in_file_name, ExportXMLOptionsKit const & in_options);
#endif

#if TARGET_OS_ANDROID == 0
		/*! Performs a synchronous IGES export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		* 	\param in_file_name The name of the file to write the IGES data to.
		* 	\param in_options The options controlling the export of the IGES data. */
		static void ExportIGES(CADModel const & in_cad_model, char const * in_file_name, ExportIGESOptionsKit const & in_options);

		/*! Performs a synchronous Parasolid export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		* 	\param in_file_name The name of the file to write the Parasolid data to.
		* 	\param in_options The options controlling the export of the Parasolid data. */
		static void ExportParasolid(CADModel const & in_cad_model, char const * in_file_name, ExportParasolidOptionsKit const & in_options);

		/*! Performs a synchronous STEP export of the given CADModel with the provided options to a given filename.  An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.  This CADModel must have come from a CAD file imported via Exchange, if it did not, the export will not succeed.
		* 	\param in_file_name The name of the file to write the STEP data to.
		* 	\param in_options The options controlling the export of the STEP data. */
		static void ExportSTEP(CADModel const & in_cad_model, char const * in_file_name, ExportSTEPOptionsKit const & in_options);
#endif

	private:
		//! Private default constructor to prevent instantiation.
		File();
	};

	/*! The HPS::Exchange::NURBSConversionOptionsKit class is a user space object.  It contains settings controlling how data is converted to NURBS curves and surfaces during import. */
	class EXCHANGE_API NURBSConversionOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty NURBSConversionOptionsKit object. */
		NURBSConversionOptionsKit();

		/*! The copy constructor creates a new NURBSConversionOptionsKit object that contains the same settings as the source NURBSConversionOptionsKit.
		 * 	\param in_kit The source NURBSConversionOptionsKit to copy. */
		NURBSConversionOptionsKit(NURBSConversionOptionsKit const & in_kit);

		/*! The move constructor creates an NURBSConversionOptionsKit by transferring the underlying impl of the rvalue reference to this NURBSConversionOptionsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an NURBSConversionOptionsKit to take the impl from. */
		NURBSConversionOptionsKit(NURBSConversionOptionsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this NURBSConversionOptionsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an NURBSConversionOptionsKit to take the impl from.
		 * 	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & operator=(NURBSConversionOptionsKit && in_that);

		virtual ~NURBSConversionOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeNURBSConversionOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Copies the source NURBSConversionOptionsKit into this NURBSConversionOptionsKit.
		 * 	\param in_kit The source NURBSConversionOptionsKit to copy. */
		void						Set(NURBSConversionOptionsKit const & in_kit);

		/*! Copies this NURBSConversionOptionsKit into the given NURBSConversionOptionsKit.
		 * 	\param out_kit The NURBSConversionOptionsKit to populate with the contents of this NURBSConversionOptionsKit. */
		void						Show(NURBSConversionOptionsKit & out_kit) const;

		/*! Copies the source NURBSConversionOptionsKit into this NURBSConversionOptionsKit.
		 * 	\param in_kit The source NURBSConversionOptionsKit to copy.
		 * 	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit &			operator=(NURBSConversionOptionsKit const & in_kit);

		/*! Indicates whether this NURBSConversionOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this NURBSConversionOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source NURBSConversionOptionsKit is equivalent to this NURBSConversionOptionsKit. 
		 *	\param in_kit The source NURBSConversionOptionsKit to compare to this NURBSConversionOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(NURBSConversionOptionsKit const & in_kit) const;

		/*!	Check if the source NURBSConversionOptionsKit is equivalent to this NURBSConversionOptionsKit. 
		 *	\param in_kit The source NURBSConversionOptionsKit to compare to this NURBSConversionOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(NURBSConversionOptionsKit const & in_kit) const;

		/*!	Check if the source NURBSConversionOptionsKit is not equivalent to this NURBSConversionOptionsKit. 
		 *	\param in_kit The source NURBSConversionOptionsKit to compare to this NURBSConversionOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(NURBSConversionOptionsKit const & in_kit) const;

		/*! Creates a NURBSConversionOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.
		 *	\return A NURBSConversionOptionsKit with the default settings. */
		static NURBSConversionOptionsKit		GetDefault();

		/*! Sets whether to replace UV curves which cross seams of periodic surfaces with 3D curves.  
		*  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a6f0e7092293f45ec534bae13b48f77c4">A3DCopyAndAdaptBrepModelData::m_bDeleteCrossingUV</a>.
		 *	\param in_state Whether to replace UV curves which cross seams of periodic surfaces.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetCrossSeamCurveReplacement(bool in_state);

		/*! Sets whether to compute 3D curves from BRep data.
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a4024e5da20487738977e04dd16a64397">A3DCopyAndAdaptBrepModelData::m_bForceCompute3D</a>.
		 *	\param in_state Whether to compute 3D curves from BRep data.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & Set3DCurvesComputation(bool in_state);

		/*! Sets whether to compute UV curves from BRep data.
		 *	\param in_state Whether to compute UV curves from BRep data. This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a5232771d15d64ccc82656921f0c68fe7">A3DCopyAndAdaptBrepModelData::m_bForceComputeUV</a>.
		 *	\param in_allow_cross_seam_curves Whether to allow UV curves to cross seams. This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a9c058810222bd0ef9c31c5a8437c66dc">A3DCopyAndAdaptBrepModelData::m_bAllowUVCrossingSeams</a>.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetUVCurvesComputation(bool in_state, bool in_allow_cross_seam_curves);

		/*! Sets whether to split closed faces.
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#aa7b8501191631658ea3c3bb2e5d574d9">A3DCopyAndAdaptBrepModelData::m_bSplitClosedFaces</a>.
		 *	\param in_state Whether to split closed faces at the parametric seam and mid-parameter.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetClosedFaceSplitting(bool in_state);

		/*! Sets whether to split periodic faces.
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a3429ac0a29df01bdcd471a40137160a4">A3DCopyAndAdaptBrepModelData::m_bSplitFaces</a>.
		 *	\param in_state Whether to split periodic faces on parametric seams.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetPeriodicFaceSplitting(bool in_state);

		/*! Sets whether to keep or recompute the parametrization of surfaces
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a855f3f05a8f2db6c172e6a410cbfb1b8">A3DCopyAndAdaptBrepModelData::m_bUseSameParam</a>.
		 *	\param in_state If true, the parametrization information for surfaces will be retained, otherwise it will be recalculated.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetParameterization(bool in_state);

		/*! Sets the tolerance for the conversion.
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a12cd764f4e22771a36116e024e509f54">A3DCopyAndAdaptBrepModelData::m_dTol</a>.
		 *	\param in_tolerance The tolerance of the resulting BRep data. The value is relative to the scale of the model.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetTolerance(double in_tolerance);

		/*! Sets a list of surface types which are allowed in the resulting conversion
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a56fcd4126ba55957bbb2bd0cecb892b7">A3DCopyAndAdaptBrepModelData::m_puiAcceptableSurfaces</a>.
		 *  The array passed to this function will be sorted.
		 *	\param in_allowed_surfaces An array of surfaces which the conversion can produce.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetAllowedSurfaces(SurfaceTypeArray const & in_allowed_surfaces);

		/*! Sets a list of surface types which are allowed in the resulting conversion
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#a56fcd4126ba55957bbb2bd0cecb892b7">A3DCopyAndAdaptBrepModelData::m_puiAcceptableSurfaces</a>.
		 *  The array passed to this function will be sorted.
		 *	\param in_count The size of in_allowed_surfaces.
		 *	\param in_allowed_surfaces An array of surfaces which the conversion can produce.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetAllowedSurfaces(size_t in_count, SurfaceType const in_allowed_surfaces []);

		/*! Sets a list of curves types which are allowed in the resulting conversion
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#aa93f295ec1ec6cd881e0e7bff8a23aa8">A3DCopyAndAdaptBrepModelData::m_puiAcceptableCurves</a>.
		 *  The array passed to this function will be sorted.
		 *	\param in_allowed_curves An array of curves which the conversion can produce.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetAllowedCurves(CurveTypeArray const & in_allowed_curves);

		/*! Sets a list of curves types which are allowed in the resulting conversion
   		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_copy_and_adapt_brep_model_data.html#aa93f295ec1ec6cd881e0e7bff8a23aa8">A3DCopyAndAdaptBrepModelData::m_puiAcceptableCurves</a>.
		 *  The array passed to this function will be sorted.
		 *	\param in_count The size of in_allowed_curves.
		 *	\param in_allowed_curves An array of curves which the conversion can produce.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & SetAllowedCurves(size_t in_count, CurveType const in_allowed_curves []);


		/*! Removes the cross seam replacement conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetCrossSeamCurveReplacement();

		/*! Removes the 3D curves computation conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & Unset3DCurvesComputation();

		/*! Removes the UV curves computation conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetUVCurvesComputation();

		/*! Removes the closed face splitting conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetClosedFaceSplitting();

		/*! Removes the periodic face splitting conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetPeriodicFaceSplitting();

		/*! Removes the parameterization conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetParameterization();

		/*! Removes the tolerance conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetTolerance();

		/*! Removes the allowed surfaces conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetAllowedSurfaces();

		/*! Removes the allowed curves conversion state.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetAllowedCurves();

		/*! Removes all settings.
		 *	\return A reference to this NURBSConversionOptionsKit. */
		NURBSConversionOptionsKit & UnsetEverything();



		/*! Shows the cross seam curve replacement conversion setting.
		 *	\param out_state Whether cross seam curves are going to be replaced by the conversion
		 *	\return <span class='code'>true</span> if a cross seam replacement conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowCrossSeamCurveReplacement(bool & out_state) const;

		/*! Shows the 3D curve computation conversion setting.
		 *	\param out_state Whether 3D curves will be computed by the conversion
		 *	\return <span class='code'>true</span> if a 3D curve computation conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool Show3DCurvesComputation(bool & out_state) const;

		/*! Shows the UV curve computation conversion setting.
		 *	\param out_state Whether UV curves will be computed by the conversion
		 *	\param out_allow_cross_seam_curves Whether UV curves will be allowed to cross seams
		 *	\return <span class='code'>true</span> if a UV curve computation conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowUVCurvesComputation(bool & out_state, bool & out_allow_cross_seam_curves) const;

		/*! Shows the face splitting conversion setting.
		 *	\param out_state Whether faces will be split by the conversion
		 *	\return <span class='code'>true</span> if a closed face splitting conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowClosedFaceSplitting(bool & out_state) const;

		/*! Shows the periodic face splitting conversion setting.
		 *	\param out_state Whether periodic faces will be split by the conversion
		 *	\return <span class='code'>true</span> if a periodic face splitting conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowPeriodicFaceSplitting(bool & out_state) const;

		/*! Shows the parameterization conversion setting.
		 *	\param out_state Whether existing parameterization will be maintained by the conversion
		 *	\return <span class='code'>true</span> if a parameterization conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowParameterization(bool & out_state) const;

		/*! Shows the tolerance conversion setting.
		 *	\param out_tolerance The tolerance that will be used for the conversion
		 *	\return <span class='code'>true</span> if a tolerance conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowTolerance(double & out_tolerance) const;

		/*! Shows the allowed surfaces conversion setting.
		 *	\param out_allowed_surfaces The types of surfaces the conversion is allowed to produce.
		 *	\return <span class='code'>true</span> if an allowed surfaces conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowAllowedSurfaces(SurfaceTypeArray & out_allowed_surfaces) const;
		
		/*! Shows the allowed curves conversion setting.
		 *	\param out_allowed_curves The types of curves the conversion is allowed to produce.
		 *	\return <span class='code'>true</span> if an allowed curves conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowAllowedCurves(CurveTypeArray & out_allowed_curves) const;
	};

	/*! The HPS::Exchange::ImportOptionsKit class is a user space object.  It contains settings controlling what and how data is imported via Exchange. Calling HPS::Exchange::ImportOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options">here.</a> */
	class EXCHANGE_API ImportOptionsKit : public SprocketKit
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

		static const HPS::Type staticType = HPS::Type::ExchangeImportOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ImportOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Import(const char*,const ImportOptionsKit&) or File::Translate.
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


		/*! Sets the import mode for BRep data in the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#abdbdcc84388eec9befad25ca29424452">A3DRWParamsGeneralData::m_eReadGeomTessMode</a>.
		 *	\param in_mode The import mode for BRep data in the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-brep-mode">Default value</a>
		 */
		ImportOptionsKit &			SetBRepMode(BRepMode in_mode);

		/*! Sets whether to import solids from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#a02404f7977d712941ccd54de3e6248a9">A3DRWParamsGeneralData::m_bReadSolids</a>.
		 *	\param in_state Whether to import solids from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-solids">Default value</a>
		 */
		ImportOptionsKit &			SetSolids(bool in_state);

		/*! Sets whether to import surfaces from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#a6c2407fa19dc1d71f9dcc6c8d895224c">A3DRWParamsGeneralData::m_bReadSurfaces</a>.
		 *	\param in_state Whether to import surfaces from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-surfaces">Default value</a>
		 */
		ImportOptionsKit &			SetSurfaces(bool in_state);

		/*! Sets whether to import wireframes from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#a849a8059a1b22fa36284aa97b54e6638">A3DRWParamsGeneralData::m_bReadWireframes</a>.
		 *	\param in_state Whether to import wireframes from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-wireframes">Default value</a>
		 */
		ImportOptionsKit &			SetWireframes(bool in_state);

		/*! Sets whether to import PMI from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#abf96d02b9d0cc24dad58a61fa608bd01">A3DRWParamsGeneralData::m_bReadPmis</a>.
		 *	\param in_state Whether to import PMI from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-pmi">Default value</a>
		 */
		ImportOptionsKit &			SetPMI(bool in_state);

		/*! Sets whether to import attributes (or metadata) from the CAD file.  Even if attributes are not imported, some things like names and certain IDs will be available, though all other attributes will not be.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#acf1542d1301f1bcf9f7945f154c28213">A3DRWParamsGeneralData::m_bReadAttributes</a>.
		 *	\param in_state Whether to import attributes from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-attributes">Default value</a>
		 */
		ImportOptionsKit &			SetAttributes(bool in_state);

		/*! Sets whether to import hidden objects from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#ac81e2ae6208ef5df9a0e0b42b726a281">A3DRWParamsGeneralData::m_bReadHiddenObjects</a>.
		 *	\param in_state Whether to import hidden objects from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-hidden-objects">Default value</a>
		 */
		ImportOptionsKit &			SetHiddenObjects(bool in_state);

		/*! Sets whether to import construction and references from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#afadd1647af886a121f56fdd5e7c6d73e">A3DRWParamsGeneralData::m_bReadConstructionAndReferences</a>.
		 *	\param in_state Whether to import construction and references from the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-construction-and-references">Default value</a>
		 */
		ImportOptionsKit &			SetConstructionAndReferences(bool in_state);

		/*! Sets whether to only import data from the active filter in the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#ade701ca16d71148b26cb3f2f52d286dd">A3DRWParamsGeneralData::m_bReadActiveFilter</a>.
		 *	\param in_state Whether to only import data from the active filter in the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-active-filter">Default value</a>
		 */
		ImportOptionsKit &			SetActiveFilter(bool in_state);

		/*! Sets whether to import feature trees from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#ae4ab0848f2d31a68d62ea5132c1dcaba">A3DRWParamsGeneralData::m_bReadFeature</a>.
		*   The following Import Options are incompatible with Feature Trees: sewing, importing a file using multiple processes, and using the Incremental import Mode.
		*	Furthermore, features are only supported in files of the following formats: CatiaV5, NX (also known as Unigraphics) and ProE (also known as Creo).
		*	\param in_state Whether to import feature trees from the CAD file.
		*	\return A reference to this ImportOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-feature-trees">Default value</a>
		*/
		ImportOptionsKit &			SetFeatureTrees(bool in_state);

		/*! Sets whether to import drawings from the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#ae4ab0848f2d31a68d62ea5132c1dcaba">A3DRWParamsGeneralData::m_eReadDrawings</a>.
		*	\param in_state Whether to import 2d drawings from the CAD file.
		*	\return A reference to this ImportOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-drawings">Default value</a>
		*/
		ImportOptionsKit &			SetDrawings(bool in_state);

		/*! Sets the units to use for a CAD file if it is not well defined.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#a317145ca6e3a78eddc66c252ab5bef52">A3DRWParamsGeneralData::m_eDefaultUnit</a>.
		 *	\param in_units The units to use for the CAD file if it is not well defined.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-default-units">Default value</a>
		 */
		ImportOptionsKit &			SetDefaultUnits(Units in_units);

		/*! Sets the number of processes to use when performing the import.  This should be a value between 1 and the number of cores on your system.
		 *	\note Values greater than 1 can only be used for imports of certain formats, in particular, %JT, SolidWorks, NX (Unigraphics), Creo (Pro/E), and CATIA V5 files.
		 *	\param in_count The number of processes to use when performing the import.
		 *	\return A reference to this ImportOptionsKit. 
         *  \deprecated This function has been deprecated.        */
		ImportOptionsKit &			SetMultiProcessCount(unsigned int in_count);

		/*! The sewing tolerance to use on the A3DAsmModelFile after importing a file.  If this option is set, after a file is successfully imported,
		 *	the A3DAsmModelFile will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d___healing.html#ga7e1b188709dbfd7353ae86798360a67a">A3DAsmModelFileSew</a>
		 *	with this tolerance.
		 *	\param in_tolerance The sewing tolerance in millimeters to use on the A3DAsmModelFile after importing a file.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetSewingTolerance(double in_tolerance);

		/*! This option lets you redirect the normal of non-solid bodies.
		 *   eg: when a body is not able to be sewed closed it still has the notion of an inside and outside.
		 *	the in_state will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_sew_options_data.html">A3DSewOptionsData</a>
		 *	With this option, you are asking Exchange to use a predictive algorithm to correctly identify inside vs outside.
		 *	\param in_state The state of the option used when importing a file
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetComputePreferredOpenShellOrientation(bool in_state);

		/*! Sets the font to use to replace those referenced by the CAD file but which are not present on the system.
		 *	\param in_font_name UTF8-encoded name of the font to use to replaced those referenced by the CAD file but which are not present on the system.
		 *		This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pmi_data.html#af2f44be4d2925808a5e0c4e3620f6fd0">A3DRWParamsPmiData::m_pcSubstitutionFont</a>.
		 *	\param in_always_substitute Whether to always use this font even in place of those which are present on the system.
		 *		This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pmi_data.html#a106b365169cc6643303d7eaf6c01ef5c">A3DRWParamsPmiData::m_bAlwaysSubstituteFont</a>.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetPMISubstitutionFont(char const * in_font_name, bool in_always_substitute = false);

		/*! Sets the precision to use for PMI which has no precision information in the CAD file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pmi_data.html#a2865e14c2af7f9a6d3e45e11d876f9e3">A3DRWParamsPmiData::m_iNumberOfDigitsAfterDot</a>.
		 *	\param in_precision The precision to use for PMI which has no precision information in the CAD file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-pmi-precision">Default value</a>
		 */
		ImportOptionsKit &			SetPMIPrecision(size_t in_precision);

		/*! Sets the units to use for the PMI in the CAD file if it is not well defined.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pmi_data.html#a195fb611b3266d34c45b01bd7e1d451a">A3DRWParamsPmiData::m_eDefaultUnit</a>.
		 *	\param in_units The units to use for the PMI in the CAD file if it is not well defined.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-pmi-default-units">Default value</a>
		 */
		ImportOptionsKit &			SetPMIDefaultUnits(Units in_units);

		/*! Dictates whether Visualize will flip pmi when rotating so that text / symbols remain readable.
		*   \note If exchange reports that a markup does not contain leader lines, but the tessellation does contain lines that markup will not be 
		*	flipped. This is a precaution for models where the leader line geometry was included as part of the markup tessellation and 
		*	flipping the geometry would cause any present leader lines to point in the wrong direction. 
		*	\param in_flip Whether or not to have Visualize flip pmi when rotating.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit &			SetPMIFlipping(bool in_flip);

		/*! Dictates whether Visualize will read the views in the PRC that have kA3DGraphicsRemoved bit set.
		*   \note In the PRC sometime views in subassemblies or Part of the model file are marked with kA3DGraphicsRemoved bit.
		*	These views still exist in the PRC, however normally are ignored by viewer.
		*	\param read_removed_views Whether or not to have Visualize read the removed views in PRC.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit &			SetRemovedViews(bool read_removed_views);

		/*! Dictates whether Visualize will read the external data Product Occurrence in the PRC.
		*   \note In the PRC sometime Product Occurrence contains another Product Occurrence in its ExternalData.
		*	Normally Visualize will not include this level of Product Occurrence in its model tree, but by activate this
		*	we will have this level in the model tree.
		*	\param read_external_po Whether or not to have Visualize read the Product occurrence of the ExternalData.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit &			SetExternalProductOccurrence(bool read_external_po);

		/*! Sets the color to use for the PMI in the CAD file if none is specified.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pmi_data.html#a1722062dec9eb83696c2dd8774761932">A3DRWParamsPmiData::m_sDefaultColor</a>.
		 *  Optionally, it is possible to force all PMI to appear in the specified color. Thie corresponds to the value what will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pmi_data.html#abe908e6a0140d07b1f476543f1504810">A3DRWParamsPmiData::m_bAlwaysUseDefaultColor</a>.
		 *	\param in_color The color to use for the PMI in the CAD file if none is specified.
		 *	\param in_override_color Whether to force all PMIs to use the color specified by in_color.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-pmi-default-color">Default value</a>
		 */
		ImportOptionsKit &			SetPMIDefaultColor(RGBColor const & in_color, bool in_override_color = false);

		/*! Dictates whether Exchange tesselation data will be released from their parent representation items during import.
		*	\warning Setting this option to true will cause future exports to output without tessellation data.  
		*	\param in_cleanup Whether to clean up tessellation data from representation items.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit &			SetTessellationCleanup(bool in_cleanup);

		/*! Sets the predefined quality of the tessellation that will be generated for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a7949f7f08c25a321ce73843676072eb1">A3DRWParamsTessellationData::m_eTessellationLevelOfDetail</a>.
		 *	\param in_level The predefined quality of the tessellation that will be generated for BRep data.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-tessellation-level">Default value</a>
		 */
		ImportOptionsKit &			SetTessellationLevel(Tessellation::Level in_level);

		/*! Sets the user defined quality of the tessellation that will be generated for BRep data.  This implicitly sets <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a7949f7f08c25a321ce73843676072eb1">A3DRWParamsTessellationData::m_eTessellationLevel</a>
		 *		to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4ab5aec16efd29ca9df5969341c5c0a245">kA3DTessLODUserDefined</a>.
		 *	\param in_limit_type The type of the chord limit to be specified.
		 *	\param in_limit Either the ratio of the tessellation chord height to the bounding box height which should be in the range <span class='code'>[50, 10000]</span>, or the maximum distance between the surface definition and the tessellation approximation, depending on the preceding argument.
		 *	\param in_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation which should be in the range <span class='code'>[10, 40]</a>.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a58e1c697c1fd5243e338485edfaf4e8d">A3DRWParamsTessellationData::m_dAngleToleranceDeg</a>.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTessellationLevel(Tessellation::Chord::Limit in_limit_type, double in_limit, double in_angle_tolerance);

		/*! Sets the accuracy of the tessellation that will be generated for BRep data.
		 *	\param in_type The accuracy to use for tessellation generation.
		 *	\param in_accurate_normals Whether to compute surface normals when performing accurate tessellation.  This only applies if <span class='code'>in_type</span> is set to Tessellation::Accuracy::Accurate or Tessellation::Accuracy::GridAccurate.
		 *		This correspond to the (negated) value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a51d14d591da50a1d28fd6a61d7ed7eab">A3DRWParamsTessellationData::m_bDoNotComputeNormalsInAccurateTessellation</a>.
		 *	\param in_accurate_surface_curvatures Whether to take into account surfaces curvature in accurate tessellation to controle triangles elongation directions.  This only applies if <span class='code'>in_type</span> is set to Tessellation::Accuracy::Accurate or Tessellation::Accuracy::GridAccurate.
		 *		This correspond to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a965186e2951e1cc735ee12311cb17698">A3DRWParamsTessellationData::m_bAccurateSurfaceCurvatures</a>.
		 *	\param in_accurate_grid_maximum_stitch_length Maximal grid stitch length. Disabled if value is 0. Be careful, a too small value can generate a huge tessellation.  This only applies if <span class='code'>in_type</span> is set to Tessellation::Accuracy::GridAccurate.
		 *		This correspond to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a965186e2951e1cc735ee12311cb17698">A3DRWParamsTessellationData::m_dAccurateTessellationWithGridMaximumStitchLength</a>.
		 *	\return A reference to this ImportOptionsKit.
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-tessellation-accuracy">Default value</a>
		 */
		ImportOptionsKit &			SetTessellationAccuracy(Tessellation::Accuracy in_type, bool in_accurate_normals = true, bool in_accurate_surface_curvatures = false, double in_accurate_grid_maximum_stitch_length = 0.0);

		/*! Sets whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a13ff3862be1e4ff11a00fcf6c9373e29">A3DRWParamsTessellationData::m_bKeepUVPoints</a>.
		 *	\param in_state Whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-tessellation-preserve-uv-points">Default value</a>
		 */
		ImportOptionsKit &			SetTessellationPreservesUVPoints(bool in_state);

		/*! Sets the maximum length of triangle edges in the tessellation generated for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a124a4c06b502d452312a70c1cd92eb09">A3DRWParamsTessellationData::m_dMaximalTriangleEdgeLength</a>.
		 *	\param in_length The maximum length of triangle edges in the tessellation generated for BRep data.  The value must be non-negative, and if set to <span class='code'>0.0</span>, the length will not be used when generating the tessellation.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-tessellation-maximum-edge-length">Default value</a>
		 */
		ImportOptionsKit &			SetTessellationMaximumEdgeLength(double in_length);

		/*! Sets the directories to search to find textures referenced by a CAD assembly.  If no search directories are set, the directory containing the CAD assembly will be searched recursively.
		 *	These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_general_data.html#ae8e904435a06c9030044981c4d4a56a2">A3DRWParamsGeneralData::m_ppcSearchTextureDirectories</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_directories The directories to search for textures referenced by a CAD assembly.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTextureDirectories(size_t in_count, UTF8 const in_directories[]);

		/*! Sets the directories to search to find textures referenced by a CAD assembly.  If no search directories are set, the directory containing the CAD assembly will be searched recursively.
		 *	These correspond to the values that will be passed to <a target="_blank" href="">A3DRWParamsGeneralData::m_ppcSearchTextureDirectories</a>.
		 *	\param in_directories The directories to search for textures referenced by a CAD assembly.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetTextureDirectories(UTF8Array const & in_directories);

		/*! Sets the directories to search to find files referenced by a CAD assembly.  If no search directories are set, the directory containing the CAD assembly will be searched recursively.
		 *	These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_assembly_data.html#aaba5f7e1f9121d7c7b75c60b98bb3599">A3DRWParamsAssemblyData::m_ppcSearchDirectories</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_directories The directories to search for files references by a CAD assembly.
		 *	\param in_recurse_flags Whether to search recursively in each of the corresponding directories.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetSearchDirectories(size_t in_count, UTF8 const in_directories[], bool const in_recurse_flags[]);

		/*! Sets the directories to search to find files referenced by a CAD assembly.  If no search directories are set, the directory containing the CAD assembly will be searched recursively.  If the size of the directory and flag arrays are different, the smaller size will be used.
		 *	These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_assembly_data.html#aaba5f7e1f9121d7c7b75c60b98bb3599">A3DRWParamsAssemblyData::m_ppcSearchDirectories</a>.
		 *	\param in_directories The directories to search for files references by a CAD assembly.
		 *	\param in_recurse_flags Whether to search recursively in each of the corresponding directories.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetSearchDirectories(UTF8Array const & in_directories, BoolArray const & in_recurse_flags);

		/*! Sets the directories to search to find files referenced by a CAD assembly.  This variant additionally allows the specification of logical names for each search directory for use by Catia V4 files.
		 *	If no search directories are set, the directory containing the CAD assembly will be searched recursively.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_assembly_data.html#aaba5f7e1f9121d7c7b75c60b98bb3599">A3DRWParamsAssemblyData::m_ppcSearchDirectories</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_directories The directories to search for files references by a CAD assembly.
		 *	\param in_logical_names The logical names for each corresponding directory.  If there are no logical names for the directories, this value can be null.
		 *	\param in_recurse_flags Whether to search recursively in each of the corresponding directories.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetCatiaV4LogicalNameSearching */
		ImportOptionsKit &			SetSearchDirectories(size_t in_count, UTF8 const in_directories[], UTF8 const in_logical_names[], bool const in_recurse_flags[]);

		/*! Sets the directories to search to find files referenced by a CAD assembly.  This variant additionally allows the specification of logical names for each search directory for use by Catia V4 files.
		 *	If no search directories are set, the directory containing the CAD assembly will be searched recursively.  If the size of the directory, (non-empty) logical name, or flag arrays are different, the smallest size will be used.
		 *	These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_assembly_data.html#aaba5f7e1f9121d7c7b75c60b98bb3599">A3DRWParamsAssemblyData::m_ppcSearchDirectories</a>.
		 *	\param in_directories The directories to search for files references by a CAD assembly.
		 *	\param in_logical_names The logical names for each corresponding directory.  If there are no logical names for the directories, this array can be empty.
		 *	\param in_recurse_flags Whether to search recursively in each of the corresponding directories.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetCatiaV4LogicalNameSearching */
		ImportOptionsKit &			SetSearchDirectories(UTF8Array const & in_directories, UTF8Array const & in_logical_names, BoolArray const & in_recurse_flags);

		/*! Sets the directories to search to find files referenced by a CAD assembly.  This is an alternative manner to specify search directories by using text files which contain lists of directories
		 *	to search.  These correspond to the values will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_assembly_data.html#a5991db32cefee257907997a67f0760d6">A3DRWParamsAssemblyData::m_ppcPathDefinitions</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_file_names The names of text files containing the lists of directories to search to find files referenced by a CAD assembly.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetSearchDirectoriesByFile(size_t in_count, UTF8 const in_file_names[]);

		/*! Sets the directories to search to find files referenced by a CAD assembly.  This is an alternative manner to specify search directories by using text files which contain lists of directories
		 *	to search.  These correspond to the values will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_assembly_data.html#a5991db32cefee257907997a67f0760d6">A3DRWParamsAssemblyData::m_ppcPathDefinitions</a>.
		 *	\param in_file_names The names of text files containing the lists of directories to search to find files referenced by a CAD assembly.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetSearchDirectoriesByFile(UTF8Array const & in_file_names);

		/*! Sets whether to search the CAD file root directory for sub-parts.
		* The values passed to this function correspond to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_assembly_data.html#a33a1ddb44e44cc8b2fae6ea952d5b24d">A3DRWParamsAssemblyData::m_bUseRootDirectory</a> and
		* <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_assembly_data.html#ac0255bd9e56386a94e0b49ecf8fb5d5b">A3DRWParamsAssemblyData::m_bRootDirRecursive</a> respectively.
		* \param in_state Whether to search the root CAD file directory for sub-parts
		* \param in_recursive Whether to search subdirectories of the root directory for sub-parts. This option is only considered if in_state is <span class='code'>true</span>, and the subdirectories are only searched if the sub-parts are not found in the root directory.
		* \return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetSearchRootDirectory(bool in_state, bool in_recursive = true);

		/*! Sets the configuration to import from a CAD file.  This is only applicable to Catia V4, Solidworks and I-deas file formats.  If no configuration is specified, the default configuration will be loaded if possible.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_multi_entries_data.html#a3bb99a84a6242a8dff42048744fa9dc2">A3DRWParamsMultiEntriesData::m_ppcEntries</a>.
		 *	\param in_configuration UTF8-encoded configuration name to import from a CAD file.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetConfiguration(char const * in_configuration);

		/*! Sets a nested configuration to import from a CAD file.  This is only applicable to Catia V4, Solidworks and I-deas file formats.  If no configuration is specified, the default configuration will be loaded if possible.
		 *	These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_multi_entries_data.html#a3bb99a84a6242a8dff42048744fa9dc2">A3DRWParamsMultiEntriesData::m_ppcEntries</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_configuration List of nested configuration names to import from a CAD file.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetConfiguration(size_t in_count, UTF8 const in_configuration[]);

		/*! Sets a nested configuration to import from a CAD file.  This is only applicable to Catia V4, Solidworks and I-deas file formats.  If no configuration is specified, the default configuration will be loaded if possible.
		 *	These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_multi_entries_data.html#a3bb99a84a6242a8dff42048744fa9dc2">A3DRWParamsMultiEntriesData::m_ppcEntries</a>.
		 *	\param in_configuration List of nested configuration names to import from a CAD file.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetConfiguration(UTF8Array const & in_configuration);

		/*! Sets the logical name for the root directory for a Catia V4 file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_catia_v4_data.html#a28f95238b76a7381ae99caf2798275d3">A3DRWParamsCatiaV4Data::m_pcRootDirLogicalName</a>.
		 *	\param in_name UTF8-encoded logical name for the root directory for a Catia V4 file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-catia-v4-logical-root-dir-name">Default value</a>
		 */
		ImportOptionsKit &			SetCatiaV4LogicalNameForRootDirectory(char const * in_name);

		/*! Sets the whether to allow searching in additional logical names for a Catia V4 file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_catia_v4_data.html#ac7df8bc70dec98b9b5dd3e98fcc55aec">A3DRWParamsCatiaV4Data::m_bAllowSearchInOtherLogicalNames</a>.
		 *	\param in_state Whether to allow searching in addtional logical names for a Catia V4 file.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetSearchDirectories 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-catia-v4-logical-name-search">Default value</a>
		 */
		ImportOptionsKit &			SetCatiaV4LogicalNameSearching(bool in_state);

		/*! Sets whether to use the a cache for a Catia V5 file.
		 *	\param in_active Whether the cache should be activated for a Catia V5 file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_catia_v5_data.html#ac6d8b0636fd1ed716de5caed7c1edfdc">A3DRWParamsCatiaV5Data::m_bCacheActivation</a>.
		 *	\param in_path UTF8-encoded path to the folder to be used for the Catia V5 cache.  This corresponds to the value that will be passed to and <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_catia_v5_data.html#ace536350152e7f36170bd6b7210a78b4">A3DRWParamsCatiaV5Data::m_pcCachePath</a>.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-catia-v5-cache">Default value</a>
		 */
		ImportOptionsKit &			SetCatiaV5Cache(bool in_active, char const * in_path);

		/*! Sets whether to use material for rendering for a Catia V5 file. The path to search for the materials needs to be specified through ImportOptionsKit::SetTextureDirectories.	
		 *	\param in_active Whether to use materials for rendering for a Catia V5 file.  This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_catia_v5_data.html#a454de3ffb730913fe8a46ffb883350bc">A3DRWParamsCatiaV5Data::m_bUseMaterialRendering</a>.
		 *	\return A reference to this ImportOptionsKit.
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-catia-v5-cache">Default value</a>
		 */
		ImportOptionsKit &			SetCatiaV5MaterialRendering(bool in_active);

		/*! Sets the preferred reference sets for a Unigraphics file.
		 *	\param in_count Size of the following array.
		 *	\param in_reference_sets List of preferred references sets for a Unigraphics file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_unigraphics_data.html#a1b8135c0678c9320213771a56deeb425">A3DRWParamsUnigraphicsData::m_ppcPreferredReferenceSets</a>.
		 *	\param in_apply_to_all_levels Whether the reference sets will apply to subassemblies for a Unigraphics file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_unigraphics_data.html#a98d1972c70c2591b15676d8da61fb9cc">A3DRWParamsUnigraphicsData::m_bApplyToAllLevels</a>.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-unig-refset-count">Default value</a>
		 */
		ImportOptionsKit &			SetUnigraphicsPreferredReferenceSets(size_t in_count, UTF8 const in_reference_sets[], bool in_apply_to_all_levels);

		/*! Sets the preferred reference sets for a Unigraphics file.
		 *	\param in_reference_sets List of preferred references sets for a Unigraphics file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_unigraphics_data.html#a1b8135c0678c9320213771a56deeb425">A3DRWParamsUnigraphicsData::m_ppcPreferredReferenceSets</a>.
		 *	\param in_apply_to_all_levels Whether the reference sets will apply to subassemblies for a Unigraphics file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_unigraphics_data.html#a98d1972c70c2591b15676d8da61fb9cc">A3DRWParamsUnigraphicsData::m_bApplyToAllLevels</a>.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-unig-refset-count">Default value</a>
		 */
		ImportOptionsKit &			SetUnigraphicsPreferredReferenceSets(UTF8Array const & in_reference_sets, bool in_apply_to_all_levels);

		/*! Sets whether to make view cameras fitted for a Unigraphics file.
		 *	\param in_state Whether to make view cameras fit all visible geometry for a Unigraphics file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_unigraphics_data.html#abb94fa02dfff4869dc52bc2a7ac02f4e">A3DRWParamsUnigraphicsData::m_bFitAllToUpdateViewCameras</a>.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-unig-fitted-camera">Default value</a>
		 */
		ImportOptionsKit &			SetUnigraphicsFittedCamera(bool in_state);

		/*! Sets the code page name for a Pro/E file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#aee49fc6bcb11dcec12776a355483850c">A3DRWParamsProEData::m_pcCodePageName</a>.
		 *	\param in_name UTF8-encoded code page name for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-code-page-name">Default value</a>
		 */
		ImportOptionsKit &			SetProECodePageName(char const * in_name);

		/*! Sets whether to import dimension tolerances for a Pro/E file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a020b92422765c998b946cfeddcbdaa45">A3DRWParamsProEData::m_bDisplayTolerance</a>.
		 *	\param in_state Whether to import dimension tolerances for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-dim-tol">Default value</a>
		 */
		ImportOptionsKit &			SetProEDimensionTolerance(bool in_state);

		/*! Sets whether to import subpart PMI for a Pro/E file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a51fd278203bc00ac696982dd10daaa7c">A3DRWParamsProEData::m_bDisplaySubpartAnnotations</a>.
		 *	\param in_state Whether to import subpart PMI for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProESubpartPMI(bool in_state);

		/*! Sets the session color for a Pro/E file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a958a4ff570248f1952195a8dcf633480">A3DRWParamsProEData::m_eSessionColorType</a>.
		 *	\param in_type The session color for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-session-color">Default value</a>
		 */
		ImportOptionsKit &			SetProESessionColor(ProE::SessionColor in_type);

		/*! Sets whether to import the entities making up a boolean operation for a Pro/E file, in the case where the tessellation for the result of this boolean operation cannot be found.  
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a2d7a5127cfca9898abf42be42f97969f">A3DRWParamsProEData::m_bBoolOpUseGenericIfNoTess</a>.
		 *	\param in_state Whether to import the entities of a boolean operation for which there is no available tessellation for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEMissingBoolean(bool in_state);

		/*! Sets whether to try to import a flexible component for a Pro/E file, even in the case where the tessellation for the component cannot be found.  
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#aa8ddbcd35e13e26aa44f7670de4ca014">A3DRWParamsProEData::m_bFlexCompUseGenericIfNoTess</a>.
		 *	\param in_state Whether to always try to import a flexible component, even in the event of missing tessellation, for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEMissingFlexibleComponent(bool in_state);

		/*! Sets the source for Family Tables for a Pro/E file
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a0d0d8ac3bc0330b6d761469c8ab4be10">A3DRWParamsProEData::m_eFamilyTables</a>.
		 *	\param in_source Sets the family tables source for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEFamilyTableSource(ProE::FamilyTable in_source);

		/*! Sets whether to create an additional Home View for a Pro/E file. This view will reflect the state in which the file was saved.
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#aa6d01b0a2f46b00070cea99108561583">A3DRWParamsProEData::m_bComputeHomeView</a>.
		 *	\param in_source Whether to create a home view for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEHomeView(bool in_state);

		/*! Sets whether to create additional Views for a Pro/E file, with each view representing an explode state.
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a956bca96cfefa7b15a5adab3eb8aa41d">A3DRWParamsProEData::m_bReadExplodeStateAsView</a>.
		 *	\param in_source Whether to create a views for exploded states, for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEExplodedViews(bool in_state);

		/*! Sets whether datums are visible, for a Pro/E file
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a47db1716f0ef2ca325549e64564862e8">A3DRWParamsProEData::m_bDisplayVisibleDatum</a>.
		 *	\param in_source Whether visible datums are visible for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEDatum(bool in_state);


		/*! Sets whether to import construction entities for a Pro/E file
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a627eed00c67fe7308329f279c6894111">A3DRWParamsProEData::m_eReadConstrucEntities</a>.
		 *	\param in_state Whether to import construction entities for a Pro/E file
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProEConstructionEntities(ProE::ConstructionEntities in_state);

		/*! Sets whether to import skeleton entities for a Pro/E file.
		 *  This corresponds to the inverse value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_pro_e_data.html#a2849896ee6971fbd5de52cc70dc0aee6">A3DRWParamsProEData::m_bHideSkeletons</a>.
		 *	\param in_state Whether import skeleton entities for a Pro/E file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-proe-subpart-name">Default value</a>
		 */
		ImportOptionsKit &			SetProESkeletons(bool in_state);

		/*! Sets the field to get the occurrence name from for a STEP file.  These correspond to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a4bc8af17dc3072ff46e83feb1dfe874f">A3DRWParamsStepData::m_bPreferProductName</a>
		 *	and <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#ad98223a794d9e17d81fc6ab60ed03bfc">A3DRWParamsStepData::m_eNameFromNAUO</a>.
		 *	\param in_field The field to get the occurrence name from for a STEP file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-step-name-preference">Default value</a>
		 */
		ImportOptionsKit &			SetSTEPNamePreference(STEP::Field in_field);

		/*! Sets the code page name for a STEP file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a02503308b5619b1fbb416b21c5f38a48">A3DRWParamsStepData::m_pcCodePageName</a>
		 *	\param in_name UTF8-encoded code page name for a STEP file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-step-code-page-name">Default value</a>
		 */
		ImportOptionsKit &			SetSTEPCodePageName(char const * in_name);

		/*! Sets whether to prefer the first color if geometry has several colors for a STEP file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a59240fd61b4af5b3ce88a8230f491c1d">A3DRWParamsStepData::m_bPreferFirstColor</a>.
		 *	\param in_state Whether to prefer the first color if geometry has several colors for a STEP file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-step-first-color-preference">Default value</a>
		 */
		ImportOptionsKit &			SetSTEPFirstColorPreference(bool in_state);

		/*! Sets whether to split SHELL_BASED_SURFACE_MODEL with several OPEN_SHELLs into several bodies for a STEP file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a2a4a3914b3b487bf4150df0db8bf978a">A3DRWParamsStepData::m_bSplitSHELL_BASED_SURFACE_MODEL</a>.
		 *	\param in_state Whether to split SHELL_BASED_SURFACE_MODEL with several OPEN_SHELLs into several bodies for a STEP file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-step-sbsms">Default value</a>
		 */
		ImportOptionsKit &			SetSTEPShellBasedSurfaceModelSplitting(bool in_state);

		/*! Sets whether to heal orientations for a STEP file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a25b52930d8cf2bb4696b590de781edf3">A3DRWParamsStepData::m_bHealOrientations</a>.
		 *	\param in_state Whether to heal orientations for a STEP file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-step-orient-heal">Default value</a>
		 */
		ImportOptionsKit &			SetSTEPOrientationHealing(bool in_state);

		/*! Sets whether to import validation properties for a STEP file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_step_data.html#a8dc946eb5d5dd56472f9c217208b209e">A3DRWParamsStepData::m_bReadValidationProperties</a>.
		 *  This option is valid only if Attributes are being imported.
		 *	\param in_state Whether to import validation properties for a STEP file.
		 *	\return A reference to this ImportOptionsKit.
		 */
		ImportOptionsKit &			SetSTEPValidationProperties(bool in_state);

		/*! Sets the code page name for an IFC file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_i_f_c_data.html#a5bd1b53dc0d3910f0a4ec81b1595441d">A3DRWParamsIFCData::m_pcCodePageName</a>.
		 *	\param in_name UTF8-encoded code page name for an IFC file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-ifc-code-page-name">Default value</a>
		 */
		ImportOptionsKit &			SetIFCCodePageName(char const * in_name);

		/*! Sets the file to write attribute data to in XML format for an IFC file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_i_f_c_data.html#a911d62d50ee46f0b5d11d9a1ed7450e6">A3DRWParamsIFCData::m_pcXMLFilePathForAttributes</a>.
		 *	\param in_filename UTF8-encoded filename for attribute data.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetAttributes
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-ifc-attr-xml-file">Default value</a>
		 */
		ImportOptionsKit &			SetIFCAttributeXMLFile(char const * in_filename);

		/*! Sets whether to optimize the setting of IFCOWNERHISTORY for an IFC file.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_i_f_c_data.html#ae7e9998e00cd10af7c564298696fba5d">A3DRWParamsIFCData::m_bIFCOWNERHISTORYOptimized</a>.
		 *	\param in_state Whether to optimize the setting of IFCOWNERHISTORY for an IFC file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-ifc-oho">Default value</a>
		 */
		ImportOptionsKit &			SetIFCOwnerHistoryOptimization(bool in_state);

		/*! Sets whether to optimize importing of faces. Setting this to true will reduce the number of polyhedra generated during the import. This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_i_f_c_data.html#a7c65f34c3a7c2e8d531b6f9143a8c0cf">A3DRWParamsIFCData::m_bFACETED_BREPAsOnFace</a>.
		 *	\param in_state Whether to optimize face import for an IFC file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-ifc-face-opt">Default value</a>
		 */
		ImportOptionsKit &			SetIFCFaceOptimization(bool in_state);

		/*! Sets whether to import edges for IFC files
		 *  When this is set to false the import time will decrease. It is still possible to achieve visual fidelity by turning on hard edge visibility.
		 *	\param in_state Whether to import edges for an IFC file.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-ifc-edges">Default value</a>
		 */
		ImportOptionsKit &			SetIFCEdges(bool in_state);

		/*! Sets whether to import all IFC metadata or to import only each part's GloablID. . This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_i_f_c_data.html#a7f66d34bd33d39e0e381d6bbc64446c9">A3DRWParamsIFCData::m_bAttributesOnlyGlobalId</a>.
		 *	\param in_state Whether to import all IFC metadata or to import only each part's GlobalID
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-face-metadata">Default value</a>
		 */
		ImportOptionsKit &			SetIFCMetadata(bool in_state);

		/*! Sets the index into the list 3D streams contained in a PDF to load.
		 *	\param in_index The index into the list of 3D stream contained in a PDF to load.
		 *	\return A reference to this ImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-3dpdf-stream-index">Default value</a>
		 */
		ImportOptionsKit &			SetPDF3DStreamIndex(size_t in_index);

		/*! Sets the tessellation level to load for JT files.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_j_t_data.html#a8c3bc6faa67639dd110bd3d1655b22e6">A3DRWParamsJTData::m_eReadTessellationLevelOfDetail</a>.
		 *	\param in_jt_tessellation_level The tessellation level to load.
		 *	\return A reference to this ImportOptionsKit. 
		 *	\sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-import-options-jt-tess-level">Default value</a>
		 */
		ImportOptionsKit &			SetJTTessellationLevel(JT::TessellationLevel in_jt_tessellation_level);

		/*! Manage the visibility of datum entities: if set to 0, visibility is hidden if set to 1, visibility is as in SLW feature tree manager
		*  This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_solidworks_data.html#a5639f3e4cf3de53e9fc426a2ba3244ba">A3DRWParamsSolidworksData::m_usDisplayVisibleDatum</a>.
		*	\param in_state Whether to use the visibility defined in the file.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit&			SetSolidworksDisplayVisibleDatum(bool in_state);

		/*! Whether to use the tessellation embedded in the file, for Inventor files.  
		 *  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_inventor_data.html#a8f09e2f69f7509db1c0beff156643bcc">A3DRWParamsInventorData::m_bUseTessForFile</a>.
		 *	\param in_state Whether to use the tessellation embedded in the file.
		 *	\return A reference to this ImportOptionsKit. 
		 */
		ImportOptionsKit &			SetInventorEmbeddedTessellation(bool in_state);

		/*! Sets whether to create an <span class='code'>A3DRWParamsPrcReadHelper</span> when importing a PRC file or a PRC stream.
		 *	The resulting <span class='code'>A3DRWParamsPrcReadHelper</span> can be accessed by Exchange::ImportNotifier::GetPRCReadHelper.
		 *	\param in_use_helper Whether to create an <span class='code'>A3DRWParamsPrcReadHelper</span> during import.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa Exchange::ImportNotifier::GetPRCReadHelper */
		ImportOptionsKit &			SetPRCReadHelper(bool in_use_helper);

		/*! Rhino files can be viewed in either 'shaded' mode or 'render' mode.
		*	By default the shaded mode colors will be imported. This function allows you to import render model colors instead.
		*	This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_rhino_data.html#a2c565fdfd6f4493e9a4ce36ccdee11e0">A3DRWParamsRhinoData::m_bForceRenderedModeColors</a>.
		*	\param in_state Whether to force rendered mode colors instead of shading mode colors.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit &			SetRhinoForceRenderedModeColors(bool in_state);

		/*! Sets how physical properties are imported.
		*	By default, no physical property is imported. This setting also will only be taken into account only if we choose to import attributes from the model.
		*	This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_revit_data.html#a2c565fdfd6f4493e9a4ce36ccdee11e0">A3DRWParamsRevitData::m_ePhysicalProperties</a>.
		*	\param in_physical_properties Physical properties setting.
		*	\return A reference to this ImportOptionsKit.
		*/
		ImportOptionsKit &			SetRevitPhysicalProperties(Revit::PhysicalProperties in_physical_properties);

		/*! Sets the type of data that should be used to generate the camera for annotation captures.
		 *	\param in_fitting The type of fitting to use for annotation captures.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetAnnotationCaptureFitting(AnnotationCaptureFitting in_fitting);

		/*! Sets where to insert the imported file in an existing Component hierarchy.  If this option is not set, a new CADModel will be created
		 *	for the imported file.  If this option is specified, the underlying PRC data of the existing CADModel will be modified to include the imported file
		 *	and the data will be imported into the specified location in the existing Component hierarchy and its corresponding scene graph.
		 *	\param in_path The path describing the location to insert the imported file into.  This path must either terminate at a CADModel or a Component
		 *		corresponding to an A3DAsmProductOccurrence.  Note that if the imported file is added along a ComponentPath with attributes specified in
		 *		the PRC data, the components added to the existing PRC data will also inherit those attributes.  Note also that if the path is just a CADModel,
		 *		a new A3DAsmModelFile underyling the CADModel must be created to include both the existing product occurrences and the new product occurrences,
		 *		and the existing A3DAsmModelFile will be deleted.
		 *	\param in_transform A transform to apply to the components being added to the existing CADModel.  This transform will also be added to the underlying
		 *		PRC data.  Defaults to an identity transform.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetLocation(ComponentPath const & in_path, MatrixKit const & in_transform = MatrixKit());

		/*! Sets the type of import to perform.  If set to ImportMode::Incremental, only the structural components of the CAD file will
		 *	be created, but the underlying representation items will not be loaded.  These empty product occurrence objects can then be loaded by using the
		 *	SetIncrementalComponentPath or SetIncrementalComponentPaths functions.  If set to ImportMode::Complete, the CAD file will be imported respecting
		 *	the other specified import options.  It is important to note that incremental import can only be performed for certain formats (see ImportMode::Incremental
		 *	for the specific formats).  Trying to use incremental import with an unsupported format will cause the import to fail and an exception to be thrown by the
		 *	ImportNotifier.  Also it is important to note that this option is mutually exclusive with the incremental ComponentPath option, i.e., if one is set,
		 *	the other will be unset.
		 *	\param in_mode The type of import to perform.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetIncrementalComponentPath
		 *	\sa SetIncrementalComponentPaths */
		ImportOptionsKit &			SetMode(ImportMode in_mode);

		/*! The path to the product occurrence to load.  This option should only be used if a file was imported incrementally (i.e., with the SetMode option set to
		 *	ImportMode::Incremental) and the specified path has not already been loaded, otherwise an exception will be thrown from the ImportNotifier.  Note that you
		 *	can check the load status of a product occurrence by using ProductOccurrence::GetLoadStatus.  The leaf of the path should be a product occurrence, though
		 *	if it is not, the closest product occurrence in the path will be loaded.  It is important to note that this option is mutually exclusive with the mode option,
		 *	i.e., if one is set, the other will be unset.
		 *	\param in_path The path to the product occurrence to load.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetMode
		 *	\sa SetIncrementalComponentPaths */
		ImportOptionsKit &			SetIncrementalComponentPath(HPS::ComponentPath const & in_path);

		/*! The paths to the product occurrences to load.  This option has the same behavior and limitations as SetIncrementalPath, but allows one to specify multiple paths
		 *	to load.
		 *	\param in_count Size of the following array.
		 *	\param in_paths The paths to the product occurrences to load.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetMode
		 *	\sa SetIncrementalComponentPath */
		ImportOptionsKit &			SetIncrementalComponentPaths(size_t in_count, HPS::ComponentPath const in_paths[]);

		/*! The paths to the product occurrences to load.  This option has the same behavior and limitations as SetIncrementalPath, but allows one to specify multiple paths
		 *	to load.
		 *	\param in_paths The paths to the product occurrences to load.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetMode
		 *	\sa SetIncrementalComponentPath */
		ImportOptionsKit &			SetIncrementalComponentPaths(HPS::ComponentPathArray const & in_paths);

		/*! Converts the curves and surfaces of the model file according to the options specified.
		 *	\param in_nurbs_conversion_options The options to use during conversion
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetNURBSConversion(HPS::Exchange::NURBSConversionOptionsKit const & in_nurbs_conversion_options);

		/*! Sets the color to use for non-PMI geometry in the CAD file if none is specified.
		 *	\param in_color The color to use for non-PMI geometry in the CAD file if none is specified.
		 *	\return A reference to this ImportOptionsKit.
		 *	\sa SetPMIDefaultColor */
		ImportOptionsKit &			SetGeometryDefaultColor(RGBAColor const & in_color);

		/* When this option is enabled, translations that are deemed to be especially large compared to the model size are removed from the
		* scene graph and instead imported as user data, at the index specified, on the segment where the translation would have been.
		* Removing very large translation is useful to avoid visual artifacts due to loss in precision.
		* Note that only part of a translation might be extracted.
		* If you are adding a model to an existing scene through the SetLocation feature, and the original model was loaded using the
		* SetLargeTranslationExtraction setting, the same data_index value should be used for the model being added to the existing scene,
		* so that any translations previously extracted from the model can be taken into account during import.
		*	\param in_state Whether to enable large translation extraction
		*	\param in_data_index The index at which the user data representing the extracted translation will be stored
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			SetLargeTranslationExtraction(bool in_state, intptr_t in_data_index = 1);

		/*! Removes the BRep mode import setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetBRepMode();

		/*! Removes the solid import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSolids();

		/*! Removes the surface import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSurfaces();

		/*! Removes the wireframe import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetWireframes();

		/*! Removes the PMI import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPMI();

		/*! Removes the attribute import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetAttributes();

		/*! Removes the hidden objects import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetHiddenObjects();

		/*! Removes the construction and references import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetConstructionAndReferences();

		/*! Removes the active filter import state.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetActiveFilter();

		/*! Removes the drawing state.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetDrawings();

		/*! Removes the feature trees import state.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetFeatureTrees();

		/*! Removes the sewing tolerance setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSewingTolerance();

		/*! Removes the compute preferred open shell orientation setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetComputePreferredOpenShellOrientation();

		/*! Removes the default units setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetDefaultUnits();

		/*! Removes the multi-process count setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetMultiProcessCount();

		/*! Removes the PMI substitution font setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPMISubstitutionFont();

		/*! Removes the PMI precision setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPMIPrecision();

		/*! Removes the PMI default units setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPMIDefaultUnits();

		/*! Removes the PMI default color setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPMIDefaultColor();

		/*! Removes the tessellation level setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTessellationLevel();

		/*! Removes the tessellation accuracy setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTessellationAccuracy();

		/*! Removes the tessellation cleanup setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTessellationCleanup();

		/*! Removes the pmi flipping setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPMIFlipping();

		/*! Removes the read removed views setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetRemovedViews();

		/*! Removes the read external data Product Occurrence setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetExternalProductOccurrence();

		/*! Removes the tessellation UV preservation setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTessellationPreservesUVPoints();

		/*! Removes the tessellation maximum edge length setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTessellationMaximumEdgeLength();

		/*! Removes the texture search directories setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetTextureDirectories();

		/*! Removes the search directories setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSearchDirectories();

		/*! Removes the search directories by file setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSearchDirectoriesByFile();

		/*! Removes the search root directory setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSearchRootDirectory();

		/*! Removes the configuration setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetConfiguration();

		/*! Removes the Catia V4 logical name for the root directory setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetCatiaV4LogicalNameForRootDirectory();

		/*! Removes the Catia V4 logical name searching setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetCatiaV4LogicalNameSearching();

		/*! Removes the Catia V5 cache setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetCatiaV5Cache();

		/*! Removes the Catia V5 material rendering setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetCatiaV5MaterialRendering();

		/*! Removes the Unigraphics preferred reference sets setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetUnigraphicsPreferredReferenceSets();

		/*! Removes the Unigraphics fitted camera setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetUnigraphicsFittedCamera();

		/*! Removes the Pro/E code page name setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProECodePageName();

		/*! Removes the Pro/E dimension tolerance setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEDimensionTolerance();

		/*! Removes the Pro/E subpart PMI setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProESubpartPMI();

		/*! Removes the Pro/E session color setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProESessionColor();

		/*! Removes the Pro/E datum setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEDatum();

		/*! Removes the Pro/E home view setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEHomeView();

		/*! Removes the Pro/E exploded views setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEExplodedViews();

		/*! Removes the Pro/E missing boolean setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEMissingBoolean();

		/*! Removes the Pro/E missing flexible setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEMissingFlexibleComponent();

		/*! Removes the Pro/E family table setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEFamilyTreeSource();

		/*! Removes the Pro/E construction entities setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProEConstructionEntities();

		/*! Removes the Pro/E skeletons setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetProESkeletons();

		/*! Removes the STEP name preference setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSTEPNamePreference();

		/*! Removes the STEP first color preference setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSTEPFirstColorPreference();

		/*! Removes the STEP code page name setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSTEPCodePageName();

		/*! Removes the STEP shell based surface model splitting setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSTEPShellBasedSurfaceModelSplitting();

		/*! Removes the STEP orientation healing setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSTEPOrientationHealing();

		/*! Removes the STEP validation properties setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetSTEPValidationProperties();

		/*! Removes the IFC code page name setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIFCCodePageName();

		/*! Removes the IFC attribute XML file setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIFCAttributeXMLFile();

		/*! Removes the IFC owner history optimization setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIFCOwnerHistoryOptimization();

		/*! Removes the IFC face optimization setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIFCFaceOptimization();

		/*! Removes the IFC edges setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIFCEdges();

		/*! Removes the IFC metadata setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIFCMetadata();

		/*! Removes the PDF 3D stream index setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPDF3DStreamIndex();

		/*! Removes the JT tessellation level setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetJTTessellationLevel();

		/*! Removes the Solidworks display visible datum setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit&			UnsetSolidworksDisplayVisibleDatum();

		/*! Removes the Inventor embedded tessellation setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetInventorEmbeddedTessellation();

		/*! Removes the PRC read helper setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetPRCReadHelper();

		/*! Removes the Rhino force rendered mode color color setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetRhinoForceRenderedModeColors();

		/*! Removes the Revit physical property setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetRevitPhysicalProperties();

		/*! Removes the annotation capture fitting setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetAnnotationCaptureFitting();

		/*! Removes the location setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetLocation();

		/*! Removes the mode setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetMode();

		/*! Removes the incremental ComponentPaths setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetIncrementalComponentPaths();

		/*! Removes the NURBS conversion setting.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetNURBSConversion();

		/*! Removes the geometry default color setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetGeometryDefaultColor();

		/* Removes the large translation extraction setting.
		*	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetLargeTranslationExtraction();

		/*! Removes all settings from this ImportOptionsKit.
		 *	\return A reference to this ImportOptionsKit. */
		ImportOptionsKit &			UnsetEverything();

		/*! Shows the BRep mode import setting.
		 *	\param out_mode The import mode for BRep data in the CAD file.
		 *	\return <span class='code'>true</span> if a BRep mode import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowBRepMode(BRepMode & out_mode) const;

		/*! Shows the solid import setting.
		 *	\param out_state Whether to import solids from the CAD file.
		 *	\return <span class='code'>true</span> if a solid import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSolids(bool & out_state) const;

		/*! Shows the surface import setting.
		 *	\param out_state Whether to import surfaces from the CAD file.
		 *	\return <span class='code'>true</span> if a surface import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSurfaces(bool & out_state) const;

		/*! Shows the wireframe import setting.
		 *	\param out_state Whether to import wireframes from the CAD file.
		 *	\return <span class='code'>true</span> if a wireframe import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowWireframes(bool & out_state) const;

		/*! Shows the PMI import setting.
		 *	\param out_state Whether to import PMI from the CAD file.
		 *	\return <span class='code'>true</span> if a PMI import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPMI(bool & out_state) const;

		/*! Shows the attribute import setting.
		 *	\param out_state Whether to import attributes from the CAD file.
		 *	\return <span class='code'>true</span> if an attribute import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowAttributes(bool & out_state) const;

		/*! Shows the hidden objects import setting.
		 *	\param out_state Whether to import hidden objects from the CAD file.
		 *	\return <span class='code'>true</span> if a hidden objects import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowHiddenObjects(bool & out_state) const;

		/*! Shows the construction and references import setting.
		 *	\param out_state Whether to import construction and references from the CAD file.
		 *	\return <span class='code'>true</span> if a construction and references import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowConstructionAndReferences(bool & out_state) const;

		/*! Shows the active filter import setting.
		 *	\param out_state Whether to only import data from the active filter in the CAD file.
		 *	\return <span class='code'>true</span> if an active filter import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowActiveFilter(bool & out_state) const;

		/*! Shows the drawings import setting.
		*	\param out_state Whether to import 2d drawings from the CAD file.
		*	\return <span class='code'>true</span> if a drawings import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowDrawings(bool & out_state) const;

		/*! Shows the feature trees import setting.
		*	\param out_state Whether to import feature trees from the CAD file.
		*	\return <span class='code'>true</span> if a feature trees import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowFeatureTrees(bool & out_state) const;

		/*! Shows the sewing tolerance setting.
		 *	\param out_tolerance The sewing tolerance to use on the A3DAsmModelFile after importing a file.
		 *	\return <span class='code'>true</span> if a sewing tolerance setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSewingTolerance(double & out_tolerance) const;

		/*! Shows the compute preferred open shell orientation setting.
		 *	\param out_state The compute preferred open shell orientation to use in the A3DSewOptionsData structure when importing a file.
		 *	\return <span class='code'>true</span> if compute preferred open shell orientation setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowComputePreferredOpenShellOrientation(bool & out_state) const;

		/*! Shows the default units setting.
		 *	\param out_units The default units setting.
		 *	\return <span class='code'>true</span> if a default units setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowDefaultUnits(Units & out_units) const;

		/*! Shows the multi-process count setting.
		*	\param out_count The number of processes to use when performing the import.
		*	\return <span class='code'>true</span> if a multi-process count setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowMultiProcessCount(unsigned int & out_count) const;

		/*! Shows the PMI substitution font setting.
		 *	\param out_font_name UTF8-encoded name of the font to use to replaced those referenced by the CAD file but which are not present on the system.
		 *	\param out_always_substitute Whether to always use this font even in place of those which are present on the system.
		 *	\return <span class='code'>true</span> if a PMI substitution font setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPMISubstitutionFont(UTF8 & out_font_name, bool & out_always_substitute) const;

		/*! Shows the PMI precision setting.
		 *	\param out_precision The precision to use for PMI which has no precision information in the CAD file.
		 *	\return <span class='code'>true</span> if a PMI precision setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPMIPrecision(size_t & out_precision) const;

		/*! Shows the PMI default units setting.
		 *	\param out_units The PMI default units setting.
		 *	\return <span class='code'>true</span> if a PMI default units setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPMIDefaultUnits(Units & out_units) const;

		/*! Shows the PMI default color setting.
		 *	\param out_color The PMI default color setting.
		 *	\param out_override Whether out_color overrides all PMI color
		 *	\return <span class='code'>true</span> if a PMI default color setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPMIDefaultColor(RGBColor & out_color, bool & out_override) const;

		/*! Shows the tessellation level setting.
		 *	\param out_type The type of the tessellation level setting which was specified.
		 *	\param out_level The predefined quality of the tessellation that will be generated for BRep data.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Standard.
		 *	\param out_limit_type The type of chord limit which was specified.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_limit The chord limit to use to generate tessellation for BRep data.  Its interpretation depends on the preceding argument.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\return <span class='code'>true</span> if a tessellation level setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationLevel(Tessellation::Type & out_type, Tessellation::Level & out_level, Tessellation::Chord::Limit & out_limit_type, double & out_limit, double & out_angle_tolerance) const;

		/*! Shows the tessellation accuracy setting.
		 *	\param out_type The type of the tessellation accuracy which was specified.
		 *	\param out_accurate_normals Whether to compute surface normals when performing accurate tessellation.  This only applies if <span class='code'>out_type</span> is different than Tessellation::Accuracy::Standard.
		 *	\param out_accurate_surface_curvatures Whether to take into account surfaces curvature in accurate tessellation to controle triangles elongation directions.  This only applies if <span class='code'>out_type</span> is different than Tessellation::Accuracy::Standard.
		 *	\param out_accurate_grid_maximum_stitch_length Maximal grid stitch length. Disabled if value is 0. Be careful, a too small value can generate a huge tessellation.  This only applies if <span class='code'>out_type</span> is Tessellation::Accuracy::GridAccurate.
		 *	\return <span class='code'>true</span> if a tessellation accuracy setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationAccuracy(Tessellation::Accuracy & out_type, bool & out_accurate_normals, bool & out_accurate_surface_curvatures, double & out_accurate_grid_maximum_stitch_length) const;

		/*! Shows the tessellation cleanup setting.
		*	\param out_cleanup The tesselation cleanup setting that was specified.
		*	\return <span class='code'>true</span> if a tessellation cleanup setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationCleanup(bool & out_cleanup) const;

		/*! Shows the pmi flipping setting.
		*	\param out_pmi_flipping The pmi flipping setting that was specified.
		*	\return <span class='code'>true</span> if a pmi flipping setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPMIFlipping(bool & out_pmi_flipping) const;

		/*! Shows the read removed views setting.
		*	\param out_read_removed_views The read removed views setting that was specified.
		*	\return <span class='code'>true</span> if a read removed views setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowRemovedViews(bool & out_read_removed_views) const;

		/*! Shows the read external data Product Occurrence setting.
		*	\param out_read_external_po The read external data Product Occurrence setting that was specified.
		*	\return <span class='code'>true</span> if a read external data Product Occurrence setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowExternalProductOccurrence(bool & out_read_external_po) const;

		/*! Shows the tessellation UV preservation setting.
		 *	\param out_state Whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.
		 *	\return <span class='code'>true</span> if a tessellation UV preservation setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationPreservesUVPoints(bool & out_state) const;

		/*! Shows the tessellation maximum edge length setting.
		 *	\param out_length The tessellation maximum edge length setting.
		 *	\return <span class='code'>true</span> if a tessellation maximum edge length setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationMaximumEdgeLength(double & out_length) const;

		/*! Shows the texture search directories setting.
		 *	\param out_directories The directories to search for textures referenced by a CAD assembly.
		 *	\return <span class='code'>true</span> if a texture search directories setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTextureDirectories(UTF8Array & out_directories) const;

		/*! Shows the search directories setting.
		 *	\param out_directories The directories to search for files referenced by a CAD assembly.
		 *	\param out_logical_names The logical names for each corresponding directory.  If there are no logical names for the directories, this array can be empty.
		 *	\param out_recurse_flags Whether to search recursively in each of the corresponding directories.
		 *	\return <span class='code'>true</span> if a search directories setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSearchDirectories(UTF8Array & out_directories, UTF8Array & out_logical_names, BoolArray & out_recurse_flags) const;

		/*! Shows the search directories by file setting.
		 *	\param out_file_names The names of text files containing the lists of directories to search to find files referenced by a CAD assembly.
		 *	\return <span class='code'>true</span> if a search directories by file setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSearchDirectoriesByFile(UTF8Array & out_file_names) const;

		/*! Shows the search root directory setting.
		*	\param out_state Whether to search the CAD file root directory for sub-parts
		*   \param out_recursive Whether to search subdirectories of the CAD file root directory for sub-parts, if they are not found in the root directory.
		*	\return <span class='code'>true</span> if a root search setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSearchRootDirectory(bool & out_state, bool & out_recursive) const;

		/*! Shows the configuration settting.
		 *	\param out_configuration List of nested configuration names to import from a CAD file.
		 *	\return <span class='code'>true</span> if a configuation setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowConfiguration(UTF8Array & out_configuration) const;

		/*! Shows the Catia V4 logical name for the root directory.
		 *	\param out_name The Catia V4 logical name for the root directory.
		 *	\return <span class='code'>true</span> if a Catia V4 logical name for the root directory setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowCatiaV4LogicalNameForRootDirectory(UTF8 & out_name) const;

		/*! Shows the Catia V4 logical name searching setting.
		 *	\param out_state Whether to allow searching in addtional logical names for a Catia V4 file.
		 *	\return <span class='code'>true</span> if a Catia V4 logical name searching setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowCatiaV4LogicalNameSearching(bool & out_state) const;

		/*! Shows the Catia V5 cache setting.
		 *	\param out_active Whether the cache should be activated for a Catia V5 file.
		 *	\param out_path The path to the folder to be used for the Catia V5 cache.
		 *	\return <span class='code'>true</span> if a Catia V5 cache setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowCatiaV5Cache(bool & out_active, UTF8 & out_path) const;

		/*! Shows the Catia V5 material rendering setting.
		 *	\param out_active Whether the option material rendering should be activated for a Catia V5 file.
		 *	\return <span class='code'>true</span> if a Catia V5 material rendering setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowCatiaV5MaterialRendering(bool & out_active) const;

		/*! Shows the Unigraphics preferred reference sets setting.
		 *	\param out_reference_sets List of preferred references sets for a Unigraphics file.
		 *	\param out_apply_to_all_levels Whether the reference sets will apply to subassemblies for a Unigraphics file.
		 *	\return <span class='code'>true</span> if a Unigraphics preferred reference sets setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowUnigraphicsPreferredReferenceSets(UTF8Array & out_reference_sets, bool & out_apply_to_all_levels) const;

		/*! Shows the Unigraphics fitted camera setting.
		 *	\param out_state Whether to make view cameras fit all visible geometry for a Unigraphics file.
		 *	\return <span class='code'>true</span> if a Unigraphics fitted camera setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowUnigraphicsFittedCamera(bool & out_state) const;

		/*! Shows the Pro/E code page name setting.
		 *	\param out_name The code page name for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E code page name setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProECodePageName(UTF8 & out_name) const;

		/*! Shows the Pro/E dimension tolerance setting.
		 *	\param out_state Whether to import dimension tolerances for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E dimension tolerance setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEDimensionTolerance(bool & out_state) const;

		/*! Shows the Pro/E subpart PMI setting.
		 *	\param out_state Whether to import subpart PMI for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E subpart PMI setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProESubpartPMI(bool & out_state) const;

		/*! Shows the Pro/E session color setting.
		 *	\param out_type The session color for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E session color setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProESessionColor(ProE::SessionColor & out_type) const;

		/*! Shows the Pro/E datum setting.
		 *	\param out_state Whether datums are visible, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E datum setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEDatum(bool & out_state) const;

		/*! Shows the Pro/E home view setting.
		 *	\param out_state Whether to create a home view, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E home view setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEHomeView(bool & out_state) const;

		/*! Shows the Pro/E exploded views setting.
		 *	\param out_state Whether to create one view for each exploded state, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E exploded views setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEExplodedViews(bool & out_state) const;

		/*! Shows the Pro/E missing boolean setting.
		 *	\param out_state Whether to import boolean entities when their tessellation is missing, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E missing boolean setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEMissingBoolean(bool & out_state) const;

		/*! Shows the Pro/E missing flexible setting.
		 *	\param out_state Whether to import flexible entities when their tessellation is missing, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E missing flexible setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEMissingFlexibleComponent(bool & out_state) const;

		/*! Shows the Pro/E family table setting.
		 *	\param out_source How to import family tables, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E family table setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEFamilyTableSource(ProE::FamilyTable & out_source) const;

		/*! Shows the Pro/E construction entities setting.
		 *	\param out_state How to import construction entities, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E construction entities setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProEConstructionEntities(ProE::ConstructionEntities & out_state) const;

		/*! Shows the Pro/E skeletons setting.
		 *	\param out_state Whether to import skeletons, for a Pro/E file.
		 *	\return <span class='code'>true</span> if a Pro/E skeletons setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowProESkeletons(bool & out_state) const;

		/*! Shows the STEP name preference setting.
		 *	\param out_field The field to get the occurrence name from for a STEP file.
		 *	\return <span class='code'>true</span> if a STEP name preference setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSTEPNamePreference(STEP::Field & out_field) const;

		/*! Shows the STEP first color preference setting.
		 *	\param out_state Whether to prefer the first color if geometry has several colors for a STEP file.
		 *	\return <span class='code'>true</span> if a STEP first color preference setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSTEPFirstColorPreference(bool & out_state) const;

		/*! Shows the STEP code page name setting.
		 *	\param out_name The code page name for a STEP file.
		 *	\return <span class='code'>true</span> if a STEP code page name setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSTEPCodePageName(UTF8 & out_name) const;

		/*! Shows the STEP shell based surface model splitting setting.
		 *	\param out_state Whether to split SHELL_BASED_SURFACE_MODEL with several OPEN_SHELLs into several bodies for a STEP file.
		 *	\return <span class='code'>true</span> if a STEP shell based surface model splitting setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSTEPShellBasedSurfaceModelSplitting(bool & out_state) const;

		/*! Shows the STEP orientation healing setting.
		 *	\param out_state The orientation healing setting for a STEP file.
		 *	\return <span class='code'>true</span> if a STEP orientation healing setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSTEPOrientationHealing(bool & out_state) const;

		/*! Shows the STEP validation properties setting.
		 *	\param out_state The validation properties setting for a STEP file.
		 *	\return <span class='code'>true</span> if a STEP validation property setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSTEPValidationProperties(bool & out_state) const;

		/*! Shows the IFC code page name setting.
		 *	\param out_name The code page name for an IFC file.
		 *	\return <span class='code'>true</span> if an IFC code page name setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIFCCodePageName(UTF8 & out_name) const;

		/*! Shows the IFC attribute XML file setting.
		 *	\param out_filename The filename for attribute data for an IFC file.
		 *	\return <span class='code'>true</span> if an IFC attribute XML file setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIFCAttributeXMLFile(UTF8 & out_filename) const;

		/*! Shows the IFC owner history optimization setting.
		 *	\param out_state Whether to optimize the setting of IFCOWNERHISTORY for an IFC file.
		 *	\return <span class='code'>true</span> if an IFC owner history optimization setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIFCOwnerHistoryOptimization(bool & out_state) const;

		/*! Shows the IFC face optimization setting.
		 *	\param out_state Whether to optimize face import for an IFC file.
		 *	\return <span class='code'>true</span> if an IFC face optimization setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIFCFaceOptimization(bool & out_state) const;

		/*! Shows the IFC edges setting.
		 *	\param out_state Whether to import edges for an IFC file.
		 *	\return <span class='code'>true</span> if an IFC edge setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIFCEdges(bool & out_state) const;

		/*! Shows the IFC metadata setting.
		 *	\param out_state Whether to import all IFC metadata or just each part's GloablID
		 *	\return <span class='code'>true</span> if an IFC metadata setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIFCMetadata(bool & out_state) const;

		/*! Shows the PDF 3D stream index setting.
		 *	\param out_index The index into the list of 3D stream contained in a PDF to load.
		 *	\return <span class='code'>true</span> if a PDF 3D stream index setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPDF3DStreamIndex(size_t & out_index) const;

		/*! Shows the JT tessellation level setting.
		 *	\param out_tessellation_level The tessellation level used when loading the JT file
		 *	\return <span class='code'>true</span> if a JT tessellation level setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowJTTessellationLevel(JT::TessellationLevel & out_tessellation_level) const;

		/*! Shows the Solidworks display visible datum setting.
		*	\param out_state The display visible datum setting for Solidworks files.
		*	\return <span class='code'>true</span> if an Solidworks display visible datum setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowSolidworksDisplayVisibleDatum(bool& out_state) const;
		
		/*! Shows the Inventor embedded tessellation setting.
		 *	\param out_state The embedded tessellation setting for Inventor files.
		 *	\return <span class='code'>true</span> if an Inventor embedded tessellation setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowInventorEmbeddedTessellation(bool & out_state) const;

		/*! Shows the PRC read helper setting.
		 *	\param out_use_helper Whether to create an <span class='code'>A3DRWParamsPrcReadHelper</span> during import.
		 *	\return <span class='code'>true</span> if a PRC read helper setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowPRCReadHelper(bool & out_use_helper) const;

		/*! Shows the Rhino force rendered mode colors setting.
		*	\param out_state The force rendered mode colors setting for Rhino files.
		*	\return <span class='code'>true</span> if an Rhino embedded force rendered mode colors setting setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowRhinoForceRenderedModeColors(bool & out_state) const;

		/*! Shows the Revit physical properties import setting.
		*	\param	out_physical_properties	The physical properties import setting for Revit files.
		*	\return	<span class='code'>true</span> if a physical properties import setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowRevitPhysicalProperties(Revit::PhysicalProperties & out_physical_properties) const;

		/*! Shows the annotation capture fitting setting.
		 *	\param out_fitting The type of fitting to use for annotation captures.
		 *	\return <span class='code'>true</span> if an annotation capture fitting setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowAnnotationCaptureFitting(AnnotationCaptureFitting & out_fitting) const;

		/*! Shows the location setting.
		 *	\param out_path The path describing the location to insert the imported file into.
		 *	\param out_transform The transform to apply to the components being added to the existing CADModel.
		 *	\return <span class='code'>true</span> if a location setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowLocation(ComponentPath & out_path, MatrixKit & out_transform) const;

		/*! Shows the mode setting.
		 *	\param out_mode The type of import to perform.
		 *	\return <span class='code'>true</span> if a mode setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowMode(ImportMode & out_mode) const;

		/*! Shows the incremental ComponentPaths setting.
		 *	\param out_paths The paths to the product occurrences to load.
		 *	\return <span class='code'>true</span> if an incremental ComponentPaths setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowIncrementalComponentPaths(HPS::ComponentPathArray & out_paths) const;

		/*! Shows the NURBS conversion setting.
		 *	\param out_nurbs_conversion_options The options to be used during the conversion.
		 *	\return <span class='code'>true</span> if NURBS conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowNURBSConversion(HPS::Exchange::NURBSConversionOptionsKit & out_nurbs_conversion_options) const;

		/*! Shows the geometry default color setting.
		 *	\param out_color The geometry default color setting.
		 *	\return <span class='code'>true</span> if a geometry default color setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowGeometryDefaultColor(RGBAColor & out_color) const;

		/* Shows the large translation extraction setting.
		*	\param out_state Whether to extract large translations during an import.
		*	\param out_data_index The index at which the user data representing the extracted translation will be stored
		*	\return <span class='code'>true</span> if a large translation extraction setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowLargeTranslationExtraction(bool & out_state, intptr_t & out_data_index) const;
	};

	/*! The Exchange::Translation class is a concept class for translation-related enums. */
	class EXCHANGE_API Translation
	{
	public:
		/*! \enum Healing
		 *	Enumerates the healing options used when translating files. */
		enum class Healing
		{
			Off,					//!< Do not perform healing.
			On,						//!< Perform healing.
			OnlyIfNotParasolid,		//!< Only perform healing if the data comes from neither Parasolid nor a format which embeds Parasolid data (NX, Solidwords, Solid Edge and JT)
		};

		/*! \enum AccurateEdges
		 *	Enumerates the options related to computing accurate edges used when translating files. */
		enum class AccurateEdges
		{
			Off,					//!< Do not compute accurate edges.
			On,						//!< Compute accurate edges.
			OnlyIfNotParasolid,		//!< Only compute accurate edges if the data comes from neither Parasolid nor a format which embeds Parasolid data (NX, Solidwords, Solid Edge and JT)
		};

		/*! \enum Simplification
		 *	Enumerates the options related to simplifying geometry used when translating files. */
		enum class Simplification
		{
			Off,					//!< Do not simplify geometry.
			On,						//!< Simplify geometry.
			OnlyIfNotParasolid,		//!< Only simplify geometry if the data comes from neither Parasolid nor a format which embeds Parasolid data (NX, Solidwords, Solid Edge and JT)
		};

		/*! \enum Merging
		 *	Enumerates the options related to merging entities used when translating files. */
		enum class Merging
		{
			Off,					//!< Do not merge entities.
			On,						//!< Merge entities.
			OnlyIfNotParasolid,		//!< Only merge entities if the data comes from neither Parasolid nor a format which embeds Parasolid data (NX, Solidwords, Solid Edge and JT)
		};

	private:
		//! Private default constructor to prevent instantiation.
		Translation();
	};

	/*! The HPS::Exchange::TranslationOptionsKit class is a user space object.  It contains settings controlling what and how data is translated to Parasolid by Exchange. Calling HPS::Exchange::TranslationOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-translation-options">here.</a> */
	class EXCHANGE_API TranslationOptionsKit : public SprocketKit
	{
		public:
		/*! The default constructor creates an empty TranslationOptionsKit object. */
		TranslationOptionsKit();

		/*! The copy constructor creates a new TranslationOptionsKit object that contains the same settings as the source TranslationOptionsKit.
		 * 	\param in_kit The source TranslationOptionsKit to copy. */
		TranslationOptionsKit(TranslationOptionsKit const & in_kit);

		/*! The move constructor creates a TranslationOptionsKit by transferring the underlying impl of the rvalue reference to this TranslationOptionsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an TranslationOptionsKit to take the impl from. */
		TranslationOptionsKit(TranslationOptionsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this TranslationOptionsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an TranslationOptionsKit to take the impl from.
		 * 	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit & operator=(TranslationOptionsKit && in_that);

		virtual ~TranslationOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeTranslationOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates a TranslationOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for translation unless an option is overridden by the options passed to one of the Translate functions.
		 *	\return A TranslationOptionsKit with the default settings. */
		static TranslationOptionsKit	GetDefault();

		/*! Copies the source TranslationOptionsKit into this TranslationOptionsKit.
		 * 	\param in_kit The source TranslationOptionsKit to copy. */
		void							Set(TranslationOptionsKit const & in_kit);

		/*! Copies this TranslationOptionsKit into the given TranslationOptionsKit.
		 * 	\param out_kit The TranslationOptionsKit to populate with the contents of this TranslationOptionsKit. */
		void							Show(TranslationOptionsKit & out_kit) const;

		/*! Copies the source TranslationOptionsKit into this TranslationOptionsKit.
		 * 	\param in_kit The source TranslationOptionsKit to copy.
		 * 	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			operator=(TranslationOptionsKit const & in_kit);

		/*! Indicates whether this TranslationOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this TranslationOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source TranslationOptionsKit is equivalent to this TranslationOptionsKit. 
		 *	\param in_kit The source TranslationOptionsKit to compare to this TranslationOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(TranslationOptionsKit const & in_kit) const;

		/*!	Check if the source TranslationOptionsKit is equivalent to this TranslationOptionsKit. 
		 *	\param in_kit The source TranslationOptionsKit to compare to this TranslationOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(TranslationOptionsKit const & in_kit) const;

		/*!	Check if the source TranslationOptionsKit is not equivalent to this TranslationOptionsKit. 
		 *	\param in_kit The source TranslationOptionsKit to compare to this TranslationOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(TranslationOptionsKit const & in_kit) const;

		/*! Sets whether to convert solids to a set of independent faces for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#a67ec36338dcfaa92d331c294d63ac94d">A3DRWParamsExportIgesData::m_bSaveSolidsAsFaces</a>.
		 *	\param in_state Whether to convert solids to a set of independent faces for export.
		 *	\return A reference to this TranslationOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-solids">Default value</a>
		 */
		TranslationOptionsKit &			SetSolidsAsFaces(bool in_state);

		/*! Sets whether to export tessellation data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#a9fc3ea289436ec57dcdbce78ddc57a97">A3DRWParamsExportParasolidData::m_bWriteTessellation</a>.
		 *	\param in_state Whether to export tessellation data.
		 *	\return A reference to this TranslationOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-tess">Default value</a>
		 */
		TranslationOptionsKit &			SetTessellation(bool in_state);

		/*! Sets whether to export hidden objects.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#a435f5a402a4e769a90105c312e75af3f">A3DRWParamsExportParasolidData::m_bWriteBlankedEntities</a>.
		 *	\param in_state Whether to export hidden objects.
		 *	\return A reference to this TranslationOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-hidden-obj">Default value</a>
		 */
		TranslationOptionsKit &			SetHiddenObjects(bool in_state);

		/*! Sets the application name to put in the Parasolid data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#aaf34910e2c76bc91ffce06955671597d">A3DRWParamsExportParasolidData::m_pcApplication</a>.
		 *	\param in_name UTF8-encoded application name to put in the Parasolid data.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetApplication(char const * in_name);

		/*! Sets the version string to put in the Parasolid data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#ae2cbff6b7cde472c983810da1d9ea0a2">A3DRWParamsExportParasolidData::m_pcVersion</a>.
		 *	\param in_version UTF8-encoded version string to put in the Parasolid data.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetVersion(char const * in_version);

		/*! Sets whether to explode multi-connexes B-reps into multiple bodies.  This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_export_parasolid_data.html#a6ab80906d9944339d1c6d0d22d1a3bb6">A3DRWParamsExportParasolidData::m_bExplodeMultiBodies</a>.
		*	\param in_state Whether to explode multi-connexes B-reps into multiple bodies.
		*	\return A reference to this TranslationOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-explode">Default value</a>
		*/
		TranslationOptionsKit&			SetExplodeMultiBodies(bool in_state);

		/*! Sets whether healing will be performed as part of the translation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/index.html#build/api_ref/struct_a3_d_r_w_params_translate_to_pk_parts_data.html#a6c3c03d071137f85b1f993cb223129fb">A3DRWParamsTranslateToPkPartsData::m_eHealing</a>.
		 *	\param in_healing Whether to perform healing.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetHealing(Translation::Healing in_healing);

		/*! Sets whether accurate edges will be computed as part of the translation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/index.html#build/api_ref/struct_a3_d_r_w_params_translate_to_pk_parts_data.html#a28d5d381ab7d0f2d16861e2e567cb72a">A3DRWParamsTranslateToPkPartsData::m_eComputeAccurateEdges</a>.
		 *	\param in_accurate_edges Whether to compute accurate edges.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetAccurateEdges(Translation::AccurateEdges in_accurate_edges);

		/*! Sets whether geometry can be simplified as part of the translation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/index.html#build/api_ref/struct_a3_d_r_w_params_translate_to_pk_parts_data.html#ga968b12c118770ae30f80fd2cc6d71f22">A3DRWParamsTranslateToPkPartsData::m_eSimplifyGeometry</a>.
		 *	\param in_simplification Whether to simplify geometry.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetSimplification(Translation::Simplification in_simplification);

		/*! Sets whether entities can be merged as part of the translation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/index.html#build/api_ref/struct_a3_d_r_w_params_translate_to_pk_parts_data.html#a4e280982be9046d0a499d4d028f30e48">A3DRWParamsTranslateToPkPartsData::m_eMergeEntities</a>.
		 *	\param in_merging Whether to merge entities.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetEntityMerging(Translation::Merging in_merging);

		/*! Sets whether perform sewing as part of the translation.
		 *	\param in_state Whether to perform sewing.
		 *	\param in_tolerance The sewing tolerance in millimeters.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetSewing(bool in_state, double in_tolerance = 0.0);

		/*! Sets whether erroneous faces can be disjointed from their solids as part of the translation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/index.html#build/api_ref/struct_a3_d_r_w_params_translate_to_pk_parts_data.html#ga45fd82ccead86ee8770d926596016c4b">A3DRWParamsTranslateToPkPartsData::m_bDisjoinIfFaceFaceError</a>.
		 *	\param in_state Whether to disjoin erroneous faces from their owning solids.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetDisjointFaces(bool in_state);

		/*! Removes the solids to faces conversion setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetSolidsAsFaces();

		/*! Removes the tessellation export setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetTessellation();

		/*! Removes the hidden objects export setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetHiddenObjects();

		/*! Removes the application name setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetApplication();

		/*! Removes the version string setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetVersion();

		/*! Removes the healing setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetHealing();

		/*! Removes the accurate edges setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetAccurateEdges();

		/*! Removes the simplification setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetSimplification();

		/*! Removes the merging setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetEntityMerging();

		/*! Removes the sewing setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetSewing();

		/*! Removes the disjoin faces setting.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetDisjointFaces();

		/*! Removes the explode multi bodies setting.
		*	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit&			UnsetExplodeMultiBodies();

		/*! Removes all settings from this TranslationOptionsKit.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetEverything();


		/*! Shows the solids to faces conversion setting.
		 *	\param out_state Whether to convert solids to a set of independent faces for translation.
		 *	\return <span class='code'>true</span> if a solids to faces conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowSolidsAsFaces(bool & out_state) const;

		/*! Shows the tessellation translation setting.
		 *	\param out_state Whether to export tessellation data.
		 *	\return <span class='code'>true</span> if a tessellation export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowTessellation(bool & out_state) const;

		/*! Shows the hidden object translation setting.
		 *	\param out_state Whether to translate hidden objects.
		 *	\return <span class='code'>true</span> if a hidden object export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowHiddenObjects(bool & out_state) const;

		/*! Shows the application name setting.
		 *	\param out_name The application name to put in the Parasolid data.
		 *	\return <span class='code'>true</span> if an application name setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowApplication(UTF8 & out_name) const;

		/*! Shows the version string setting.
		 *	\param out_version The version string to put in the Parasolid data.
		 *	\return <span class='code'>true</span> if a version string setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowVersion(UTF8 & out_version) const;

		/*! Shows the explode multi bodies setting.
		*	\param out_state The explode multi bodies setting to put in the Parasolid data.
		*	\return <span class='code'>true</span> if a explode multi bodies setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowExplodeMultiBodies(bool& out_state) const;

		/*! Shows the healing setting.
		 *	\param out_healing Whether to heal data for translation.
		 *	\return <span class='code'>true</span> if a healing setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowHealing(Translation::Healing & out_healing) const;

		/*! Shows the accurate edges setting.
		 *	\param out_accurate_edges Whether to compute accurate edges for translation.
		 *	\return <span class='code'>true</span> if an accurate edges setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowAccurateEdges(Translation::AccurateEdges & out_accurate_edges) const;

		/*! Shows the simplification setting.
		 *	\param out_simplification Whether to simplify geometry for translation.
		 *	\return <span class='code'>true</span> if a simplification setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowSimplification(Translation::Simplification & out_simplification) const;

		/*! Shows the merging setting.
		 *	\param out_merging Whether to merge entities for translation.
		 *	\return <span class='code'>true</span> if a merging setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowEntityMerging(Translation::Merging & out_merging) const;

		/*! Shows the sewing setting.
		 *	\param out_state Whether to perform sewing for translation.
		 *	\param out_tolerance The sewing tolerance. Only valid if out_state is true.
		 *	\return <span class='code'>true</span> if a sewing setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowSewing(bool & out_state, double & out_tolerance) const;

		/*! Shows the disjoin faces setting.
		 *	\param out_state Whether to disjoin erroneous faces from their owning solids.
		 *	\return <span class='code'>true</span> if a disjoin faces setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowDisjointFaces(bool & out_state) const;

		/*! Sets the number of processes to use when performing translation.  This should be a value between 1 and the number of cores on your system.
		 *	\note Presently this option is only used for translations performed when using HPS::ExchangeParsolid::File::Import and HPS::ExchangeParasolid::File::Reload.
		 *	\param in_count The number of processes to use when performing translation.
		 *	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			SetMultiProcessCount(unsigned int in_count);

		/*! Removes the multi-process count setting.
		*	\return A reference to this TranslationOptionsKit. */
		TranslationOptionsKit &			UnsetMultiProcessCount();

		/*! Shows the multi-process count setting.
		 *	\param out_count The number of processes to use when performing translation.
		 *	\return <span class='code'>true</span> if a multi-process count setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowMultiProcessCount(unsigned int & out_count) const;
	};



	/*! The HPS::Exchange::ModelFileImportOptionsKit class is a user space object.  It contains settings (potentially) controlling how data is imported via Exchange.
	 *	This particular options kit is used when importing an A3DAsmModelFile directly (see Exchange::File::Import(void*,const ModelFileImportOptionsKit&)) and as such
	 *	the options will only be used if the A3DAsmModelFile permits (e.g., tessellation options will only be used if tessellation needs to be generated and the A3DAsmModelFile
	 *	has BRep data).  Calling HPS::Exchange::ModelFileImportOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-model-file-import-options">here.</a> */
	class EXCHANGE_API ModelFileImportOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ModelFileImportOptionsKit object. */
		ModelFileImportOptionsKit();

		/*! The copy constructor creates a new ModelFileImportOptionsKit object that contains the same settings as the source ModelFileImportOptionsKit.
		 * 	\param in_kit The source ModelFileImportOptionsKit to copy. */
		ModelFileImportOptionsKit(ModelFileImportOptionsKit const & in_kit);

		/*! The move constructor creates an ModelFileImportOptionsKit by transferring the underlying impl of the rvalue reference to this ModelFileImportOptionsKit
		 * 	thereby avoiding a copy and allocation.
		 * 	\param in_that An rvalue reference to an ModelFileImportOptionsKit to take the impl from. */
		ModelFileImportOptionsKit(ModelFileImportOptionsKit && in_that);

		/*! The move assignment operator transfers the underlying impl of the rvalue reference to this ModelFileImportOptionsKit thereby avoiding a copy.
		 * 	\param in_that An rvalue reference to an ModelFileImportOptionsKit to take the impl from.
		 * 	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit & operator=(ModelFileImportOptionsKit && in_that);

		virtual ~ModelFileImportOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeModelFileImportOptionsKit;
		HPS::Type							ObjectType() const { return staticType; }

		/*! Creates an ModelFileImportOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Import(void*,const ModelFileImportOptionsKit&).
		 *	\return A ModelFileImportOptionsKit with the default settings. */
		static ModelFileImportOptionsKit	GetDefault();

		/*! Copies the source ModelFileImportOptionsKit into this ModelFileImportOptionsKit.
		 * 	\param in_kit The source ModelFileImportOptionsKit to copy. */
		void								Set(ModelFileImportOptionsKit const & in_kit);

		/*! Copies this ModelFileImportOptionsKit into the given ModelFileImportOptionsKit.
		 * 	\param out_kit The ModelFileImportOptionsKit to populate with the contents of this ModelFileImportOptionsKit. */
		void								Show(ModelFileImportOptionsKit & out_kit) const;

		/*! Copies the source ModelFileImportOptionsKit into this ModelFileImportOptionsKit.
		 * 	\param in_kit The source ModelFileImportOptionsKit to copy.
		 * 	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			operator=(ModelFileImportOptionsKit const & in_kit);

		/*! Indicates whether this ModelFileImportOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ModelFileImportOptionsKit, <span class='code'>false</span> otherwise. */
		bool								Empty() const;

		/*!	Check if the source ModelFileImportOptionsKit is equivalent to this ModelFileImportOptionsKit. 
		 *	\param in_kit The source ModelFileImportOptionsKit to compare to this ModelFileImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool								Equals(ModelFileImportOptionsKit const & in_kit) const;

		/*!	Check if the source ModelFileImportOptionsKit is equivalent to this ModelFileImportOptionsKit. 
		 *	\param in_kit The source ModelFileImportOptionsKit to compare to this ModelFileImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool								operator==(ModelFileImportOptionsKit const & in_kit) const;

		/*!	Check if the source ModelFileImportOptionsKit is not equivalent to this ModelFileImportOptionsKit. 
		 *	\param in_kit The source ModelFileImportOptionsKit to compare to this ModelFileImportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool								operator!=(ModelFileImportOptionsKit const & in_kit) const;


		/* Dictates whether Visualize will flip pmi when rotating so that text / symbols remain readable.
		*   \note If exchange reports that a markup does not contain leader lines, but the tessellation does contain lines that markup will not be 
		*	flipped. This is a precaution for models where the leader line geometry was included as part of the markup tessellation and 
		*	flipping the geometry would cause any present leader lines to point in the wrong direction. 
		*	\param in_cleanup Whether or not to have visualize flip pmi when rotating.
		*	\return A reference to this ModelFileImportOptionsKit.
		*/
		ModelFileImportOptionsKit &			SetPMIFlipping(bool in_flip);

		/*! Sets the predefined quality of the tessellation that will be generated for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a7949f7f08c25a321ce73843676072eb1">A3DRWParamsTessellationData::m_eTessellationLevelOfDetail</a>.
		 *	\param in_level The predefined quality of the tessellation that will be generated for BRep data.
		 *	\return A reference to this ModelFileImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-model-file-import-options-tessellation-level">Default value</a>
		 */
		ModelFileImportOptionsKit &			SetTessellationLevel(Tessellation::Level in_level);

		/*! Sets the user defined quality of the tessellation that will be generated for BRep data.  This implicitly sets <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a7949f7f08c25a321ce73843676072eb1">A3DRWParamsTessellationData::m_eTessellationLevel</a>
		 *		to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4ab5aec16efd29ca9df5969341c5c0a245">kA3DTessLODUserDefined</a>.
		 *	\param in_limit_type The type of the chord limit to be specified.
		 *	\param in_limit Either the ratio of the tessellation chord height to the bounding box height which should be in the range <span class='code'>[50, 10000]</span>, or the maximum distance between the surface definition and the tessellation approximation, depending on the preceding argument.
		 *	\param in_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation which should be in the range <span class='code'>[10, 40]</a>.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a58e1c697c1fd5243e338485edfaf4e8d">A3DRWParamsTessellationData::m_dAngleToleranceDeg</a>.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetTessellationLevel(Tessellation::Chord::Limit in_limit_type, double in_limit, double in_angle_tolerance);

		/*! Sets the accuracy of the tessellation that will be generated for BRep data.
		 *	\param in_type The accuracy to use for tessellation generation.
		 *	\param in_accurate_normals Whether to compute surface normals when performing accurate tessellation.  This only applies if <span class='code'>in_type</span> is set different than Tessellation::Accuracy::Standard.
		 *		This correspond to the (negated) value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a51d14d591da50a1d28fd6a61d7ed7eab">A3DRWParamsTessellationData::m_bDoNotComputeNormalsInAccurateTessellation</a>.
		 *	\param in_accurate_surface_curvatures Whether to take into account surfaces curvature in accurate tessellation to controle triangles elongation directions.  This only applies if <span class='code'>in_type</span> is different than Tessellation::Accuracy::Standard.
		 *		This correspond to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a965186e2951e1cc735ee12311cb17698">A3DRWParamsTessellationData::m_bAccurateSurfaceCurvatures</a>.
		 *	\param accurate_grid_maximum_stitch_length Maximal grid stitch length. Disabled if value is 0. Be careful, a too small value can generate a huge tessellation.  This only applies if <span class='code'>in_type</span> is set to Tessellation::Accuracy::GridAccurate.
		 *		This correspond to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a965186e2951e1cc735ee12311cb17698">A3DRWParamsTessellationData::m_bAccurateTessellationWithGrid </a>.
		 *	\return A reference to this ModelFileImportOptionsKit.
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-model-file-import-options-tessellation-accuracy">Default value</a>
		 */
		ModelFileImportOptionsKit &			SetTessellationAccuracy(Tessellation::Accuracy in_type, bool in_accurate_normals = true, bool in_accurate_surface_curvatures = false, double accurate_grid_maximum_stitch_length = 0.0);

		/* Dictates whether Exchange tesselation data will be released from their parent representation items during import.
		*	\warning Setting this option to true will cause future exports to output without tessellation data.  
		*	\param in_cleanup Whether to clean up tessellation data from representation items.
		*	\return A reference to this ImportOptionsKit.
		*/
		ModelFileImportOptionsKit &			SetTessellationCleanup(bool in_cleanup);

		/*! Sets whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a13ff3862be1e4ff11a00fcf6c9373e29">A3DRWParamsTessellationData::m_bKeepUVPoints</a>.
		 *	\param in_state Whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.
		 *	\return A reference to this ModelFileImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-model-file-import-options-tessellation-preserve-uv-points">Default value</a>
		 */
		ModelFileImportOptionsKit &			SetTessellationPreservesUVPoints(bool in_state);

		/*! Sets the maximum length of triangle edges in the tessellation generated for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a124a4c06b502d452312a70c1cd92eb09">A3DRWParamsTessellationData::m_dMaximalTriangleEdgeLength</a>.
		 *	\param in_length The maximum length of triangle edges in the tessellation generated for BRep data.  The value must be non-negative, and if set to <span class='code'>0.0</span>, the length will not be used when generating the tessellation.
		 *	\return A reference to this ModelFileImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-model-file-import-options-tessellation-maximum-edge-length">Default value</a>
		 */
		ModelFileImportOptionsKit &			SetTessellationMaximumEdgeLength(double in_length);

		/*! Sets whether to import edges for IFC files
		 *  When this is set to false the import time will decrease. It is still possible to achieve visual fidelity by turning on hard edge visibility.
		 *	\param in_state Whether to import edges for an IFC file.
		 *	\return A reference to this ModelFileImportOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-model-file-import-options-ifc-edges">Default value</a>
		 */
		ModelFileImportOptionsKit &			SetIFCEdges(bool in_state);

		/*! Sets the type of data that should be used to generate the camera for annotation captures.
		 *	\param in_fitting The type of fitting to use for annotation captures.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetAnnotationCaptureFitting(AnnotationCaptureFitting in_fitting);

		/*! Sets where to insert the imported file in an existing Component hieararchy.  If this option is not set, a new CADModel will be created
		 *	for the imported file.  If this option is specified, the underlying PRC data of the existing CADModel will be modified to include the imported file
		 *	and the data will be imported into the specified location in the existing Component hierarchy and its corresponding scene graph.
		 *	\param in_path The path describing the location to insert the imported file into.  This path must either terminate at a CADModel or a Component
		 *		corresponding to an A3DAsmProductOccurrence.  Note that if the imported file is added along a ComponentPath with attributes specified in
		 *		the PRC data, the components added to the existing PRC data will also inherit those attributes.  Note also that if the path is just a CADModel,
		 *		a new A3DAsmModelFile underyling the CADModel must be created to include both the existing product occurrences and the new product occurrences.
		 *		This new A3DAsmModelFile will be owned by the CADModel being imported into (see CADModel::GetEntityOwnership), but the existing A3DAsmModelFile
		 *		will still need to be cleaned up via A3DAsmModelFileDelete since it was not owned by the CADModel.
		 *	\param in_transform A transform to apply to the components being added to the existing CADModel.  This transform will also be added to the underlying
		 *		PRC data.  Defaults to an identity transform.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetLocation(ComponentPath const & in_path, MatrixKit const & in_transform = MatrixKit());

		/*! The sewing tolerance to use on the A3DAsmModelFile prior to generating the scene graph.  If this option is set, prior to generating the scene graph,
		 *	the A3DAsmModelFile will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/group__a3d___healing.html#ga7e1b188709dbfd7353ae86798360a67a">A3DAsmModelFileSew</a>
		 *	with this tolerance.
		 *	\param in_tolerance The sewing tolerance to use on the A3DAsmModelFile.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetSewingTolerance(double in_tolerance);

		/*! This option lets you redirect the normal of non-solid bodies.
		 *   eg: when a body is not able to be sewed closed it still has the notion of an inside and outside.
		 *	the in_state will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_sew_options_data.html">A3DSewOptionsData</a>
		 *	With this option, you are asking Exchange to use a predictive algorithm to correctly identify inside vs outside.
		 *	\param in_state The state of the option used when importing a file
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetComputePreferredOpenShellOrientation(bool in_state);

		/*! Converts the curves and surfaces of the model file according to the options specified.
		 *	\param in_nurbs_conversion_options The options to use during conversion
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetNURBSConversion(HPS::Exchange::NURBSConversionOptionsKit const & in_nurbs_conversion_options);

		/*! Sets the color to use for non-PMI geometry in the CAD file if none is specified.
		*	\param in_color The color to use for non-PMI geometry in the CAD file if none is specified.
		*	\return A reference to this ModelFileImportOptionsKit.
		*	\sa SetPMIDefaultColor */
		ModelFileImportOptionsKit &			SetGeometryDefaultColor(RGBAColor const & in_color);

		/*! Sets whether we import the hidden objects from the CAD Model or not.
		 *	The in_state will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_general_data.html#ac81e2ae6208ef5df9a0e0b42b726a281">A3DRWParamsGeneralData::m_bReadHiddenObjects</a>
		 *	With this option, you are asking Exchange read the objects that are marked as hidden or graphics removed.
		 *	\param in_state The state of the option used when importing a file
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			SetHiddenObjects(bool in_state);

		/*! Removes the tessellation level setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetTessellationLevel();

		/*! Removes the tessellation accuracy setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetTessellationAccuracy();

		/*! Removes the tessellation cleanup setting.
		*	\return A reference to this ImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetTessellationCleanup();

		/*! Removes the pmi flipping setting.
		*	\return A reference to this ImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetPMIFlipping();

		/*! Removes the tessellation UV preservation setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetTessellationPreservesUVPoints();

		/*! Removes the tessellation maximum edge length setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetTessellationMaximumEdgeLength();

		/*! Removes the IFC edges setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetIFCEdges();

		/*! Removes the annotation capture fitting setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetAnnotationCaptureFitting();

		/*! Removes the location setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetLocation();

		/*! Removes the sewing tolerance setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetSewingTolerance();

		/*! Removes the compute preferred open shell orientation setting.
		*	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetComputePreferredOpenShellOrientation();

		/*! Removes the NURBS conversion setting.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetNURBSConversion();

		/*! Removes the geometry default color setting.
		*	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetGeometryDefaultColor();

		/*! Removes the read hidden objects setting.
		*	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetHiddenObjects();

		/*! Removes all settings from this ModelFileImportOptionsKit.
		 *	\return A reference to this ModelFileImportOptionsKit. */
		ModelFileImportOptionsKit &			UnsetEverything();


		/*! Shows the tessellation level setting.
		 *	\param out_type The type of the tessellation level setting which was specified.
		 *	\param out_level The predefined quality of the tessellation that will be generated for BRep data.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Standard.
		 *	\param out_limit_type The type of chord limit which was specified.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_limit The chord limit to use to generate tessellation for BRep data.  Its interpretation depends on the preceding argument.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\return <span class='code'>true</span> if a tessellation level setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowTessellationLevel(Tessellation::Type & out_type, Tessellation::Level & out_level, Tessellation::Chord::Limit & out_limit_type, double & out_limit, double & out_angle_tolerance) const;

		/*! Shows the tessellation accuracy setting.
		 *	\param out_type The type of the tessellation accuracy which was specified.
		 *	\param out_accurate_normals Whether to compute surface normals when performing accurate tessellation.  This only applies if <span class='code'>out_type</span> is different than Tessellation::Accuracy::Standard.
		 *	\param out_accurate_surface_curvatures Whether to take into account surfaces curvature in accurate tessellation to controle triangles elongation directions.  This only applies if <span class='code'>out_type</span> is different than Tessellation::Accuracy::Standard.
		 *	\param out_accurate_grid_maximum_stitch_length Maximal grid stitch length. Disabled if value is 0. Be careful, a too small value can generate a huge tessellation.  This only applies if <span class='code'>out_type</span> is Tessellation::Accuracy::GridAccurate.
		 *	\return <span class='code'>true</span> if a tessellation accuracy setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationAccuracy(Tessellation::Accuracy & out_type, bool & out_accurate_normals, bool & out_accurate_surface_curvatures, double & out_accurate_grid_maximum_stitch_length) const;

		/*! Shows the tessellation cleanup setting.
		*	\param out_cleanup The tesselation cleanup setting that was specified.
		*	\return <span class='code'>true</span> if a tessellation cleanup setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowTessellationCleanup(bool & out_cleanup) const;

		/*! Shows the pmi flipping setting.
		*	\param out_pmi_flipping The pmi flipping setting that was specified.
		*	\return <span class='code'>true</span> if a pmi flipping setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowPMIFlipping(bool & out_pmi_flipping) const;

		/*! Shows the tessellation UV preservation setting.
		 *	\param out_state Whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.
		 *	\return <span class='code'>true</span> if a tessellation UV preservation setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowTessellationPreservesUVPoints(bool & out_state) const;

		/*! Shows the tessellation maximum edge length setting.
		 *	\param out_length The tessellation maximum edge length setting.
		 *	\return <span class='code'>true</span> if a tessellation maximum edge length setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowTessellationMaximumEdgeLength(double & out_length) const;

		/*! Shows the IFC edges setting.
		 *	\param out_state Whether to import edges for an IFC file.
		 *	\return <span class='code'>true</span> if an IFC edge setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowIFCEdges(bool & out_state) const;

		/*! Shows the annotation capture fitting setting.
		 *	\param out_fitting The type of fitting to use for annotation captures.
		 *	\return <span class='code'>true</span> if an annotation capture fitting setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowAnnotationCaptureFitting(AnnotationCaptureFitting & out_fitting) const;

		/*! Shows the location setting.
		 *	\param out_path The path describing the location to insert the imported file into.
		 *	\param out_transform The transform to apply to the components being added to the existing CADModel.
		 *	\return <span class='code'>true</span> if a location setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowLocation(ComponentPath & out_path, MatrixKit & out_transform) const;

		/*! Shows the sewing tolerance setting.
		 *	\param out_tolerance The sewing tolerance to use on the A3DAsmModelFile prior to generating the scene graph.
		 *	\return <span class='code'>true</span> if a sewing tolerance setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowSewingTolerance(double & out_tolerance) const;


		/*! Shows the compute preferred open shell orientation setting.
		*	\param out_state The compute preferred open shell orientation to use in the A3DSewOptionsData structure when importing a file.
		*	\return <span class='code'>true</span> if compute preferred open shell orientation setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowComputePreferredOpenShellOrientation(bool & out_state) const;

		/*! Shows the NURBS conversion setting.
		 *	\param out_nurbs_conversion_options The options to be used during the conversion.
		 *	\return <span class='code'>true</span> if NURBS conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowNURBSConversion(HPS::Exchange::NURBSConversionOptionsKit & out_nurbs_conversion_options) const;

		/*! Shows the geometry default color setting.
		*	\param out_color The geometry default color setting.
		*	\return <span class='code'>true</span> if a geometry default color setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowGeometryDefaultColor(RGBAColor & out_color) const;

		/*! Shows the read hidden objects setting.
		*	\param out_state Whether to import hidden objects from the CAD model or not.
		*	\return <span class='code'>true</span> if the read hidden objects setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowHiddenObjects(bool & out_state) const;
	};

#if TARGET_OS_ANDROID == 0 && TARGET_OS_IPHONE == 0
	/*! The HPS::Exchange::Export3MFOptionsKit class is a user space object.  It contains settings controlling how 3MF data is exported via the Exchange interface. */
	class EXCHANGE_API Export3MFOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty Export3MFOptionsKit object. */
		Export3MFOptionsKit();

		/*! The copy constructor creates a new Export3MFOptionsKit object that contains the same settings as the source Export3MFOptionsKit.
		* \param in_kit The source Export3MFOptionsKit to copy. */
		Export3MFOptionsKit(Export3MFOptionsKit const & in_kit);

		/*! The move constructor creates an Export3MFOptionsKit by transferring the underlying object of the rvalue reference to this Export3MFOptionsKit.
		* \param in_that An rvalue reference to an Export3MFOptionsKit to take the underlying object from. */
		Export3MFOptionsKit(Export3MFOptionsKit && in_that);

		virtual ~Export3MFOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExport3MFOptionsKit;
		HPS::Type ObjectType() const { return staticType; }

		/*! Creates an Export3MFOptionsKit which contains the default settings. The returned object will not necessarily have values set for every option, but it will have settings for those options
		* where it is reasonable to have defaults. These values will be used for export unless an option in overridden by the options passed to File::Export3MF.
		* \return An Export3MFOptionsKit with the default settings. */
		static Export3MFOptionsKit GetDefault();

		/*! Copies the source Export3MFOptionsKit into this Export3MFOptionsKit.
		* 	\param in_kit The source Export3MFOptionsKit to copy. */
		void Set(Export3MFOptionsKit const & in_kit);

		/*! Copies this Export3MFOptionsKit into the given Export3MFOptionsKit.
		* 	\param out_kit The Export3MFOptionsKit to populate with the contents of this Export3MFOptionsKit. */
		void Show(Export3MFOptionsKit& out_kit) const;

		/*! Copies the source Export3MFOptionsKit into this Export3MFOptionsKit.
		* 	\param in_kit The source Export3MFOptionsKit to copy.
		* 	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & operator=(Export3MFOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this Export3MFOptionsKit.
		*	\param in_that An rvalue reference to an Export3MFOptionsKit to take the underlying object from.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & operator=(Export3MFOptionsKit && in_that);

		/*! Indicates whether this Export3MFOptionsKit has any values set on it.
		* 	\return <span class='code'>true</span> if no values are set on this Export3MFOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source Export3MFOptionsKit is equivalent to this Export3MFOptionsKit.
		*	\param in_kit The source Export3MFOptionsKit to compare to this Export3MFOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(Export3MFOptionsKit const & in_kit) const;

		/*!	Check if the source Export3MFOptionsKit is equivalent to this Export3MFOptionsKit.
		*	\param in_kit The source Export3MFOptionsKit to compare to this Export3MFOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(Export3MFOptionsKit const & in_kit) const;

		/*!	Check if the source Export3MFOptionsKit is not equivalent to this Export3MFOptionsKit.
		*	\param in_kit The source Export3MFOptionsKit to compare to this Export3MFOptionsKit.
		*	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(Export3MFOptionsKit const & in_kit) const;

		/*! Sets an array of key paths which will be exported in addition to the model.
		*  Use this setting to export Visualize geometry to 3MF.
		*	\param in_additional_key_paths The key paths to be exported.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit &		SetAdditionalKeyPaths(KeyPathArray const & in_additional_key_paths);

		/*! Sets an array of key paths which will be exported in addition to the model.
		*  Use this setting to export Visualize geometry to 3MF.
		*	\param in-count The number of key paths contained in in_additional_key_paths.
		*	\param in_additional_key_paths The key paths to be exported.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit &		SetAdditionalKeyPaths(size_t in_count, KeyPath const in_additional_key_paths[]);

		/*! Removes the additional key paths setting.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit &		UnsetAdditionalKeyPaths();

		/*! Shows the additional key paths setting.
		*	\param out_additional_key_paths The key paths which will be added to the model
		*	\return <span class='code'>true</span> if an additional key paths setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowAdditionalKeyPaths(KeyPathArray & out_additional_key_paths) const;

		/*! Sets whether to retain the current tessellation during export.
		*	\param in_state Whether to retain the current tessellation during export.
		*   \param in_tessellation_kit The tessellation to use during export. Only used if <span class='code'>in_state</span> is <span class='code'>false</span>, or if no tessellation is present.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & SetCurrentTessellationRetention(bool in_state, TessellationOptionsKit const & in_tessellation_kit = TessellationOptionsKit());

		/*! Removes the KeepCurrentTessellation setting.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & UnsetCurrentTessellationRetention();

		/*! Shows the KeepCurrentTessellation setting.
		*	\param out_state Whether to retain the current tessellation during export.
		*   \param out_tessellation_kit The tessellation options to use during export. Only valid if <span class='code'>out_state</span> is <span class='code'>false</span>.
		*	\return <span class='code'>true</span> if a tessellation retention setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowCurrentTessellationRetention(bool & out_state, Exchange::TessellationOptionsKit & out_tessellation_kit) const;

		/*! Sets a description. This will be reported in the output file as metadata.
		*	\param in_description The description.
		*	\return A reference to this Export3MFOptionsKit .*/
		Export3MFOptionsKit & SetDescription(char const * in_description);

		/*! Removes the description setting.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & UnsetDescription();

		/*! Shows the description setting.
		*	\param out_description The description.
		*	\return <span class='code'>true</span> if a description setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowDescription(HPS::UTF8 & out_state) const;

		/*! Sets the copyright metadata.
		*	\param in_copyright The copyright metadata.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & SetCopyright(char const * in_state);

		/*! Removes the Copyright setting.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & UnsetCopyright();

		/*! Shows the copyright setting.
		*	\param out_copyright The copyright metadata.
		*	\return <span class='code'>true</span> if a copyright setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowCopyright(HPS::UTF8 & out_state) const;

		/*! Sets license terms metadata.
		*	\param in_license The license terms metadata.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & SetLicenseTerms(char const * in_license);

		/*! Removes the license setting.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & UnsetLicenseTerms();

		/*! Shows the license setting.
		*	\param out_license The license metadata
		*	\return <span class='code'>true</span> if a license setting was specified, <span class='code'>false</span> otherwise. */
		bool ShowLicenseTerms(HPS::UTF8 & out_license) const;

		/*! Removes all settings from this Export3MFOptionsKit.
		*	\return A reference to this Export3MFOptionsKit. */
		Export3MFOptionsKit & UnsetEverything();
	};	

	/*! The HPS::Exchange::ExportFBXOptionsKit class is a user space object.  It contains settings controlling how FBX data is exported via the Exchange interface. Calling HPS::Exchange::ExportFBXOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportFBXOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportFBXOptionsKit object. */
		ExportFBXOptionsKit();

		/*! The copy constructor creates a new ExportFBXOptionsKit object that contains the same settings as the source ExportFBXOptionsKit.
		 * 	\param in_kit The source ExportFBXOptionsKit to copy. */
		ExportFBXOptionsKit(ExportFBXOptionsKit const & in_kit);

		/*! The move constructor creates an ExportFBXOptionsKit by transferring the underlying object of the rvalue reference to this ExportFBXOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportFBXOptionsKit to take the underlying object from. */
		ExportFBXOptionsKit(ExportFBXOptionsKit && in_that);

		virtual ~ExportFBXOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportFBXOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates an ExportFBXOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportFBX.
		 *	\return An ExportFBXOptionsKit with the default settings. */
		static ExportFBXOptionsKit		GetDefault();

		/*! Copies the source ExportFBXOptionsKit into this ExportFBXOptionsKit.
		 * 	\param in_kit The source ExportFBXOptionsKit to copy. */
		void							Set(ExportFBXOptionsKit const & in_kit);

		/*! Copies this ExportFBXOptionsKit into the given ExportFBXOptionsKit.
		 * 	\param out_kit The ExportFBXOptionsKit to populate with the contents of this ExportFBXOptionsKit. */
		void							Show(ExportFBXOptionsKit & out_kit) const;

		/*! Copies the source ExportFBXOptionsKit into this ExportFBXOptionsKit.
		 * 	\param in_kit The source ExportFBXOptionsKit to copy.
		 * 	\return A reference to this ExportFBXOptionsKit. */
		ExportFBXOptionsKit &			operator=(ExportFBXOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportFBXOptionsKit.
		 *	\param in_that An rvalue reference to an ExportFBXOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportFBXOptionsKit. */
		ExportFBXOptionsKit &			operator=(ExportFBXOptionsKit && in_that);

		/*! Indicates whether this ExportFBXOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportFBXOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source ExportFBXOptionsKit is equivalent to this ExportFBXOptionsKit. 
		 *	\param in_kit The source ExportFBXOptionsKit to compare to this ExportFBXOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(ExportFBXOptionsKit const & in_kit) const;

		/*!	Check if the source ExportFBXOptionsKit is equivalent to this ExportFBXOptionsKit. 
		 *	\param in_kit The source ExportFBXOptionsKit to compare to this ExportFBXOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(ExportFBXOptionsKit const & in_kit) const;

		/*!	Check if the source ExportFBXOptionsKit is not equivalent to this ExportFBXOptionsKit. 
		 *	\param in_kit The source ExportFBXOptionsKit to compare to this ExportFBXOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(ExportFBXOptionsKit const & in_kit) const;


		/*! Sets folder where texture will be exported (if any). If NULL, then texture are written in the same directory as the .fbx. Symbol @ is expanded as Filename of current file (without extension)
		 *  Example: If the output file is 'C:/out/myfile.obj' and the texture folder is set to 'C:/out/myfile.obj', the exoport process will create a directory called
		 *  'C:/out/myfile' and place the textures there
		 *	\param in_name UTF8-encoded path to the folder where texture will be exported.  This corresponds to the value that will be passed to and <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_fbx_data.html#a72edcbc1d9d4b3e6e9c320a7797784b6">A3DRWParamsExportFbxData::m_pcTextureFolder</a>.
		 *	\return A reference to this ExportFBXOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-fbx-texture-folder">Default value</a>
		 */
		ExportFBXOptionsKit &			SetTextureFolder(char const * in_name);


		/*! Sets whether to write FBX data in ascii form.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_fbx_data.html#a1461a12555be8519707bc658d9c7cdcd">A3DRWParamsExportFBXData::m_bAscii</a>.
		 *	\param in_state Whether to write FBX data in ascii form.
		 *	\return A reference to this ExportFBXOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-fbx-ascii">Default value</a>
		 */
		ExportFBXOptionsKit &			SetAscii(bool in_state);


		/*! Removes the texture folder setting.
		 *	\return A reference to this ExportFBXOptionsKit. */
		ExportFBXOptionsKit &			UnsetTextureFolder();

		/*! Removes the ascii export setting.
		 *	\return A reference to this ExportFBXOptionsKit. */
		ExportFBXOptionsKit &			UnsetAscii();

		/*! Removes all settings from this ExportFBXOptionsKit.
		 *	\return A reference to this ExportFBXOptionsKit. */
		ExportFBXOptionsKit &			UnsetEverything();


		/*! Shows the texture folder setting.
		 *	\param out_path The path to the folder where texture will be exported.
		 *	\return <span class='code'>true</span> if a texture folder setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowTextureFolder(UTF8 & out_path) const;

		/*! Shows the ascii export setting.
		 *	\param out_state Whether to write FBX data in ascii form.
		 *	\return <span class='code'>true</span> if a ascii export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowAscii(bool & out_state) const;
	};

	/*! The HPS::Exchange::ExportACISOptionsKit class is a user space object.  It contains settings controlling how ACIS data is exported via the Exchange interface. Calling HPS::Exchange::ExportACISOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportACISOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportACISOptionsKit object. */
		ExportACISOptionsKit();

		/*! The copy constructor creates a new ExportACISOptionsKit object that contains the same settings as the source ExportACISOptionsKit.
		 * 	\param in_kit The source ExportACISOptionsKit to copy. */
		ExportACISOptionsKit(ExportACISOptionsKit const & in_kit);

		/*! The move constructor creates an ExportACISOptionsKit by transferring the underlying object of the rvalue reference to this ExportACISOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportACISOptionsKit to take the underlying object from. */
		ExportACISOptionsKit(ExportACISOptionsKit && in_that);

		virtual ~ExportACISOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportACISOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates an ExportACISOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportACIS.
		 *	\return An ExportACISOptionsKit with the default settings. */
		static ExportACISOptionsKit		GetDefault();

		/*! Copies the source ExportACISOptionsKit into this ExportACISOptionsKit.
		 * 	\param in_kit The source ExportACISOptionsKit to copy. */
		void							Set(ExportACISOptionsKit const & in_kit);

		/*! Copies this ExportACISOptionsKit into the given ExportACISOptionsKit.
		 * 	\param out_kit The ExportACISOptionsKit to populate with the contents of this ExportACISOptionsKit. */
		void							Show(ExportACISOptionsKit & out_kit) const;

		/*! Copies the source ExportACISOptionsKit into this ExportACISOptionsKit.
		 * 	\param in_kit The source ExportACISOptionsKit to copy.
		 * 	\return A reference to this ExportACISOptionsKit. */
		ExportACISOptionsKit &			operator=(ExportACISOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportACISOptionsKit.
		 *	\param in_that An rvalue reference to an ExportACISOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportACISOptionsKit. */
		ExportACISOptionsKit &			operator=(ExportACISOptionsKit && in_that);

		/*! Indicates whether this ExportACISOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportACISOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source ExportACISOptionsKit is equivalent to this ExportACISOptionsKit. 
		 *	\param in_kit The source ExportACISOptionsKit to compare to this ExportACISOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(ExportACISOptionsKit const & in_kit) const;

		/*!	Check if the source ExportACISOptionsKit is equivalent to this ExportACISOptionsKit. 
		 *	\param in_kit The source ExportACISOptionsKit to compare to this ExportACISOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(ExportACISOptionsKit const & in_kit) const;

		/*!	Check if the source ExportACISOptionsKit is not equivalent to this ExportACISOptionsKit. 
		 *	\param in_kit The source ExportACISOptionsKit to compare to this ExportACISOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(ExportACISOptionsKit const & in_kit) const;


		/*! Sets whether to write ACIS data in millimeter units instead of the native units of the CADModel.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_acis_data.html#ac5e79d97a322f3b0ed1737f7a7d470a2">A3DRWParamsExportAcisData::m_bSaveAsMillimeter</a>.
		 *	\param in_state Whether to write ACIS data in millimeter units instead of the native units of the CADModel.
		 *	\return A reference to this ExportACISOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-acis-mm">Default value</a>
		 */
		ExportACISOptionsKit &			SetMillimeterUnits(bool in_state);

		/*! Sets whether to write ACIS data in binary form.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_acis_data.html#ad1c0225f67ae2be437fb22859cd06f24">A3DRWParamsExportAcisData::m_bSaveAsBinary</a>.
		 *	\param in_state Whether to write ACIS data in binary form.
		 *	\return A reference to this ExportACISOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-acis-binary">Default value</a>
		 */
		ExportACISOptionsKit &			SetBinary(bool in_state);


		/*! Removes the millimeter unit export setting.
		 *	\return A reference to this ExportACISOptionsKit. */
		ExportACISOptionsKit &			UnsetMillimeterUnits();

		/*! Removes the binary export setting.
		 *	\return A reference to this ExportACISOptionsKit. */
		ExportACISOptionsKit &			UnsetBinary();

		/*! Removes all settings from this ExportACISOptionsKit.
		 *	\return A reference to this ExportACISOptionsKit. */
		ExportACISOptionsKit &			UnsetEverything();


		/*! Shows the millimeter unit export setting.
		 *	\param out_state Whether to write ACIS data in millimeter units instead of the native units of the CADModel.
		 *	\return <span class='code'>true</span> if a millimeter unit export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowMillimeterUnits(bool & out_state) const;

		/*! Shows the binary export setting.
		 *	\param out_state Whether to write ACIS data in binary form.
		 *	\return <span class='code'>true</span> if a binary export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowBinary(bool & out_state) const;
	};

	/*! The HPS::Exchange::ExportJTOptionsKit class is a user space object.  It contains settings controlling how JT data is exported via Exchange. Calling HPS::Exchange::ExportJTOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportJTOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportJTOptionsKit object. */
		ExportJTOptionsKit();

		/*! The copy constructor creates a new ExportJTOptionsKit object that contains the same settings as the source ExportJTOptionsKit.
		 * 	\param in_kit The source ExportJTOptionsKit to copy. */
		ExportJTOptionsKit(ExportJTOptionsKit const & in_kit);

		/*! The move constructor creates an ExportJTOptionsKit by transferring the underlying object of the rvalue reference to this ExportJTOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportJTOptionsKit to take the underlying object from. */
		ExportJTOptionsKit(ExportJTOptionsKit && in_that);

		virtual ~ExportJTOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportJTOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates an ExportJTOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportJT.
		 *	\return An ExportJTOptionsKit with the default settings. */
		static ExportJTOptionsKit		GetDefault();

		/*! Copies the source ExportJTOptionsKit into this ExportJTOptionsKit.
		 * 	\param in_kit The source ExportJTOptionsKit to copy. */
		void							Set(ExportJTOptionsKit const & in_kit);

		/*! Copies this ExportJTOptionsKit into the given ExportJTOptionsKit.
		 * 	\param out_kit The ExportJTOptionsKit to populate with the contents of this ExportJTOptionsKit. */
		void							Show(ExportJTOptionsKit & out_kit) const;

		/*! Copies the source ExportJTOptionsKit into this ExportJTOptionsKit.
		 * 	\param in_kit The source ExportJTOptionsKit to copy.
		 * 	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			operator=(ExportJTOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportJTOptionsKit.
		 *	\param in_that An rvalue reference to an ExportJTOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			operator=(ExportJTOptionsKit && in_that);

		/*! Indicates whether this ExportJTOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportJTOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source ExportJTOptionsKit is equivalent to this ExportJTOptionsKit. 
		 *	\param in_kit The source ExportJTOptionsKit to compare to this ExportJTOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(ExportJTOptionsKit const & in_kit) const;

		/*!	Check if the source ExportJTOptionsKit is equivalent to this ExportJTOptionsKit. 
		 *	\param in_kit The source ExportJTOptionsKit to compare to this ExportJTOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(ExportJTOptionsKit const & in_kit) const;

		/*!	Check if the source ExportJTOptionsKit is not equivalent to this ExportJTOptionsKit. 
		 *	\param in_kit The source ExportJTOptionsKit to compare to this ExportJTOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(ExportJTOptionsKit const & in_kit) const;


		/*! Sets the type of data to export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_j_t_data.html#add11ead0aff256e907a0d932dd620c19">A3DRWParamsExportJTData::m_eWriteGeomTessMode</a>.
		 *	\param in_content The type of data to export.
		 *	\return A reference to this ExportJTOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-jt-content">Default value</a>
		 */
		ExportJTOptionsKit &			SetContent(JT::Content in_content);

		/*! Sets whether to export hidden objects.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_j_t_data.html#a08988cd540914a2637ca5e83fe0dfebb">A3DRWParamsExportJTData::m_bWriteHiddenObjects</a>.
		 *	\param in_state Whether to export hidden objects.
		 *	\return A reference to this ExportJTOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-jt-hidden-objects">Default value</a>
		 */
		ExportJTOptionsKit &			SetHiddenObjects(bool in_state);

		/*! Sets whether to export PMI.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_j_t_data.html#a722eab9279f0e9a8d21d583e07ebff92">A3DRWParamsExportJTData::m_bWritePMI</a>.
		 *	\param in_state Whether to export PMI.
		 *	\return A reference to this ExportJTOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-jt-pmi">Default value</a>
		 */
		ExportJTOptionsKit &			SetPMI(bool in_state);

		/*! Sets the version of the JT format to export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_export_j_t_data.html#a1c2fbdc0d4d5db2fbdd45b56f046a8d9">A3DRWParamsExportJTData::m_eJTVersion</a>.
		 *	\param in_version The version of the JT format to export.
		 *	\return A reference to this ExportJTOptionsKit.
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-jt-version">Default value</a>
		 */
		ExportJTOptionsKit &			SetVersion(JT::Version in_version);


		/*! Removes the content export setting.
		 *	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			UnsetContent();

		/*! Removes the hidden objects export setting.
		 *	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			UnsetHiddenObjects();

		/*! Removes the PMI export setting.
		 *	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			UnsetPMI();

		/*! Removes the version export setting.
		 *	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			UnsetVersion();

		/*! Removes all settings from this ExportJTOptionsKit.
		 *	\return A reference to this ExportJTOptionsKit. */
		ExportJTOptionsKit &			UnsetEverything();


		/*! Shows the content export setting.
		 *	\param out_content The type of data to export.
		 *	\return <span class='code'>true</span> if a content export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowContent(JT::Content & out_content) const;

		/*! Shows the hidden object export setting.
		 *	\param out_state Whether to export hidden objects.
		 *	\return <span class='code'>true</span> if a hidden object export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowHiddenObjects(bool & out_state) const;

		/*! Shows the PMI export setting.
		 *	\param out_state Whether to export PMI.
		 *	\return <span class='code'>true</span> if a PMI export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowPMI(bool & out_state) const;

		/*! Shows the version export setting.
		 *	\param out_version The version of the JT format to export.
		 *	\return <span class='code'>true</span> if a version export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowVersion(JT::Version & out_version) const;
	};

	/*! The HPS::Exchange::ExportPRCOptionsKit class is a user space object. It contains settings controlling how PRC data is exported via Exchange. Calling HPS::Exchange::ExportPRCOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportPRCOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportPRCOptionsKit object. */
		ExportPRCOptionsKit();

		/*! The copy constructor creates a new ExportPRCOptionsKit object that contains the same settings as the source ExportPRCOptionsKit.
		 * 	\param in_kit The source ExportPRCOptionsKit to copy. */
		ExportPRCOptionsKit(ExportPRCOptionsKit const & in_kit);

		/*! The move constructor creates an ExportPRCOptionsKit by transferring the underlying object of the rvalue reference to this ExportPRCOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportPRCOptionsKit to take the underlying object from. */
		ExportPRCOptionsKit(ExportPRCOptionsKit && in_that);

		virtual ~ExportPRCOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportPRCOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ExportPRCOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportPRC.
		 *	\return An ExportPRCOptionsKit with the default settings. */
		static ExportPRCOptionsKit	GetDefault();

		/*! Copies the source ExportPRCOptionsKit into this ExportPRCOptionsKit.
		 * 	\param in_kit The source ExportPRCOptionsKit to copy. */
		void						Set(ExportPRCOptionsKit const & in_kit);

		/*! Copies this ExportPRCOptionsKit into the given ExportPRCOptionsKit.
		 * 	\param out_kit The ExportPRCOptionsKit to populate with the contents of this ExportPRCOptionsKit. */
		void						Show(ExportPRCOptionsKit & out_kit) const;

		/*! Copies the source ExportPRCOptionsKit into this ExportPRCOptionsKit.
		 * 	\param in_kit The source ExportPRCOptionsKit to copy.
		 * 	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		operator=(ExportPRCOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportPRCOptionsKit.
		 *	\param in_that An rvalue reference to an ExportPRCOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		operator=(ExportPRCOptionsKit && in_that);

		/*! Indicates whether this ExportPRCOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportPRCOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ExportPRCOptionsKit is equivalent to this ExportPRCOptionsKit. 
		 *	\param in_kit The source ExportPRCOptionsKit to compare to this ExportPRCOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ExportPRCOptionsKit const & in_kit) const;

		/*!	Check if the source ExportPRCOptionsKit is equivalent to this ExportPRCOptionsKit. 
		 *	\param in_kit The source ExportPRCOptionsKit to compare to this ExportPRCOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ExportPRCOptionsKit const & in_kit) const;

		/*!	Check if the source ExportPRCOptionsKit is not equivalent to this ExportPRCOptionsKit. 
		 *	\param in_kit The source ExportPRCOptionsKit to compare to this ExportPRCOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ExportPRCOptionsKit const & in_kit) const;


		/*! Sets whether to compress tessellation for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#ae1cf4104a5eda56ce958e94ee1c1c42c">A3DRWParamsExportPrcData::m_bCompressTessellation</a>.
		 *	\param in_state Whether to compress tessellation for export.
		 *	\return A reference to this ExportPRCOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-prc-tess">Default value</a>
		 */
		ExportPRCOptionsKit &		SetTessellationCompression(bool in_state);

		/*! Sets the level of compression to apply to BRep data for export.  This corresponds to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#afbbe41721d6878768a624ea8d0894531">A3DRWParamsExportPrcData::m_bCompressBrep</a>
		 *	and <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#afd35a9e2bea37b388227b76e14f49daa">A3DRWParamsExportPrcData::m_eCompressBrepType</a>.
		 *	\param in_level The level of compression to apply to BRep data for export.
		 *	\return A reference to this ExportPRCOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-prc-brep">Default value</a>
		 */
		ExportPRCOptionsKit &		SetBRepCompression(PRC::BRepCompression in_level);

		/*! Sets whether to remove the BRep information for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#a7a61340a38925e02b30cb67f39b1fcd4">A3DRWParamsExportPrcData::m_bRemoveBRep</a>.
		 *	\param in_state Whether to remove BRep information in the PRC data for this annotation.
		 *	\return A reference to this AnnotationKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-prc-brep-removal">Default value</a>
		 */
		ExportPRCOptionsKit &		SetBRepRemoval(bool in_state);

		/*! Sets whether to remove the attribute information for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#aa8435b8348b8d6f1c11f1bc15313ed47">A3DRWParamsExportPrcData::m_bRemoveAttributes</a>.
		 *	\param in_state Whether to remove the attribute information in the PRC data for this annotation.
		 *	\return A reference to this AnnotationKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-prc-attr-removal">Default value</a>
		 */
		ExportPRCOptionsKit &		SetAttributeRemoval(bool in_state);

		/*! Sets an array of key paths which will be exported to PRC in addition to the model.
		 *  Use this setting to export Visualize geometry to PRC.
		 *	\param in_additional_key_paths The key paths to be exported.
		 *	\return A reference to this AnnotationKit. */
		ExportPRCOptionsKit &		SetAdditionalKeyPaths(KeyPathArray const & in_additional_key_paths);

		/*! Sets an array of key paths which will be exported to PRC in addition to the model.
		 *  Use this setting to export Visualize geometry to PRC.
		 *	\param in-count The number of key paths contained in in_additional_key_paths.
		 *	\param in_additional_key_paths The key paths to be exported.
		 *	\return A reference to this AnnotationKit. */
		ExportPRCOptionsKit &		SetAdditionalKeyPaths(size_t in_count, KeyPath const in_additional_key_paths []);


		/*! Removes the tessellation compression setting.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		UnsetTessellationCompression();

		/*! Removes the BRep compression level setting.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		UnsetBRepCompression();

		/*! Removes the BRep removal setting.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		UnsetBRepRemoval();

		/*! Removes the attribute removal setting.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		UnsetAttributeRemoval();

		/*! Removes the additional key paths setting.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		UnsetAdditionalKeyPaths();

		/*! Removes all settings from this ExportPRCOptionsKit.
		 *	\return A reference to this ExportPRCOptionsKit. */
		ExportPRCOptionsKit &		UnsetEverything();


		/*! Shows the tessellation compression setting.
		 *	\param out_state Whether to compress tessellation for export.
		 *	\return <span class='code'>true</span> if a tessellation compression setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationCompression(bool & out_state) const;

		/*! Shows the BRep compression level setting.
		 *	\param out_level The level of compression to apply to BRep data for export.
		 *	\return <span class='code'>true</span> if a BRep compression level setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowBRepCompression(PRC::BRepCompression & out_level) const;

		/*! Shows the BRep removal setting.
		 *	\param out_state Whether to remove BRep information for export.
		 *	\return <span class='code'>true</span> if a BRep removal setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowBRepRemoval(bool & out_state) const;

		/*! Shows the attribute removal setting.
		 *	\param out_state Whether to remove attribute information for export.
		 *	\return <span class='code'>true</span> if an attribute removal setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowAttributeRemoval(bool & out_state) const;

		/*! Shows the additional key paths setting.
		 *	\param out_additional_key_paths The key paths which will be added to the model
		 *	\return <span class='code'>true</span> if an additional key paths setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowAdditionalKeyPaths(KeyPathArray & out_additional_key_paths) const;
	};

	/*! The HPS::Exchange::ExportSTLOptionsKit class is a user space object. It contains settings controlling how STL data is exported via Exchange. Calling HPS::Exchange::ExportSTLOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportSTLOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportSTLOptionsKit object. */
		ExportSTLOptionsKit();

		/*! The copy constructor creates a new ExportSTLOptionsKit object that contains the same settings as the source ExportSTLOptionsKit.
		 * 	\param in_kit The source ExportSTLOptionsKit to copy. */
		ExportSTLOptionsKit(ExportSTLOptionsKit const & in_kit);

		/*! The move constructor creates an ExportSTLOptionsKit by transferring the underlying object of the rvalue reference to this ExportSTLOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportSTLOptionsKit to take the underlying object from. */
		ExportSTLOptionsKit(ExportSTLOptionsKit && in_that);

		virtual ~ExportSTLOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportSTLOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ExportSTLOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportSTL.
		 *	\return An ExportSTLOptionsKit with the default settings. */
		static ExportSTLOptionsKit	GetDefault();

		/*! Copies the source ExportSTLOptionsKit into this ExportSTLOptionsKit.
		 * 	\param in_kit The source ExportSTLOptionsKit to copy. */
		void						Set(ExportSTLOptionsKit const & in_kit);

		/*! Copies this ExportSTLOptionsKit into the given ExportSTLOptionsKit.
		 * 	\param out_kit The ExportSTLOptionsKit to populate with the contents of this ExportSTLOptionsKit. */
		void						Show(ExportSTLOptionsKit & out_kit) const;

		/*! Copies the source ExportSTLOptionsKit into this ExportSTLOptionsKit.
		 * 	\param in_kit The source ExportSTLOptionsKit to copy.
		 * 	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		operator=(ExportSTLOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportSTLOptionsKit.
		 *	\param in_that An rvalue reference to an ExportSTLOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		operator=(ExportSTLOptionsKit && in_that);

		/*! Indicates whether this ExportSTLOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportSTLOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ExportSTLOptionsKit is equivalent to this ExportSTLOptionsKit. 
		 *	\param in_kit The source ExportSTLOptionsKit to compare to this ExportSTLOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ExportSTLOptionsKit const & in_kit) const;

		/*!	Check if the source ExportSTLOptionsKit is equivalent to this ExportSTLOptionsKit. 
		 *	\param in_kit The source ExportSTLOptionsKit to compare to this ExportSTLOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ExportSTLOptionsKit const & in_kit) const;

		/*!	Check if the source ExportSTLOptionsKit is not equivalent to this ExportSTLOptionsKit. 
		 *	\param in_kit The source ExportSTLOptionsKit to compare to this ExportSTLOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ExportSTLOptionsKit const & in_kit) const;


		/*! Sets whether to write STL data in binary form.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#ae50c4e70d9faa7dd7fd1c7948bff60f6">A3DRWParamsExportStlData::m_bBinaryFile</a>.
		 *	\param in_state Whether to write STL data in binary form.
		 *	\return A reference to this ExportSTLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-stl-binary">Default value</a>
		 */
		ExportSTLOptionsKit &		SetBinary(bool in_state);

		/*! Sets the predefined quality of the tessellation that will be generated for BRep data using the standard tessellation algorithm.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a46e17ddb4aefca9ebc45db1092c58229">A3DRWParamsExportStlData::m_eTessellationLevelOfDetail</a>.
		 *	\param in_level The predefined quality of the tessellation that will be generated for BRep data using the standard tessellation algorithm.
		 *	\return A reference to this ExportSTLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-stl-tess">Default value</a>
		 */
		ExportSTLOptionsKit &		SetTessellationLevel(Tessellation::Level in_level);

		/*! Sets the user defined quality of the tessellation that will be generated for BRep data using the standard or accurate tessellation algorithm.  This implicitly sets <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a46e17ddb4aefca9ebc45db1092c58229">A3DRWParamsExportStlData::m_eTessellationLevel</a>
		 *		to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4ab5aec16efd29ca9df5969341c5c0a245">kA3DTessLODUserDefined</a>.
		 *	\param in_limit_type The type of the chord limit to be specified.
		 *	\param in_limit Either the ratio of the tessellation chord height to the bounding box height which should be in the range <span class='code'>[50, 10000]</span>, or the maximum distance between the surface definition and the tessellation approximation, depending on the preceding argument.
		 *	\param in_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation which should be in the range <span class='code'>[10, 40]</a>.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a35aaea20089e9b5f377262eee7c230c3">A3DRWParamsExportStlData::m_dAngleToleranceDeg</a>.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		SetTessellationLevel(Tessellation::Chord::Limit in_limit_type, double in_limit, double in_angle_tolerance);

		/*! Sets the accuracy of the tessellation that will be generated for BRep data.
		 *	\param in_type The accuracy to use for tessellation generation.
		 *	\return A reference to this ExportSTLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-stl-tess-accuracy">Default value</a>
		 */
		ExportSTLOptionsKit &		SetTessellationAccuracy(Tessellation::Accuracy in_type);

		/*! Sets whether to keep the current tessellation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#ac9abb807d07a3e1cd510c3255d8f28eb">A3DRWParamsExportStlData::m_bKeepCurrentTessellation</a>.
		 *	\param in_state Whether to keep the current tessellation.
		 *	\return A reference to this ExportSTLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-stl-tess-retention">Default value</a>
		 */
		ExportSTLOptionsKit &		SetCurrentTessellationRetention(bool in_state);

		/*! Sets the maximum length of triangle edges in the tessellation generated for BRep data.  This is only relevant if the tessellation level is set to Tessellation::Level::UserDefined.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#a970a7e29cdfe377c7e9a7b5b769e87ab">A3DRWParamsExportStlData::m_dMaximalTriangleEdgeLength</a>.
		 *	\param in_length The maximum length of triangle edges in the tessellation generated for BRep data.  The value must be non-negative.
		 *	\return A reference to this ExportSTLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-stl-max-edge-length">Default value</a>
		 */
		ExportSTLOptionsKit &		SetMaximumEdgeLength(double in_length);

		/*! Sets the units to associate with the STL export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_stl_data.html#ae50c4e70d9faa7dd7fd1c7948bff60f6">A3DRWParamsExportStlData::m_eExportUnit</a>.
		*   This function will result in a scale being applied to the exported model. The CADModel will NOT have a unit associated with it, since STL is a unit-less format.
		*   For example: exporting a cube with a 1mm side with Units set to Meter, will cause the side of the cube to shrink by a factor of 1000, since every unit in the file, which used
		*	to correspond to one mm, now corresponds to a meter.
		*   This is useful in the case where the STL file needs to be loaded in a scene alongside another model, so that the units of the two models can match.
		*	\param in_units The units to associate with the export. Passing Units::Unknown will preserve the original units.
		*	\return A reference to this ExportSTLOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-stl-units">Default value</a>
		*/
		ExportSTLOptionsKit &		SetUnits(Units in_units);


		/*! Removes the binary export setting.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetBinary();

		/*! Removes the tessellation level setting.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetTessellationLevel();

		/*! Removes the tessellation accuracy setting.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetTessellationAccuracy();

		/*! Removes the current tessellation retention setting.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetCurrentTessellationRetention();

		/*! Removes the tessellation maximum edge length setting.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetMaximumEdgeLength();

		/*! Removes the units export setting.
		*	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetUnits();

		/*! Removes all settings from this ExportSTLOptionsKit.
		 *	\return A reference to this ExportSTLOptionsKit. */
		ExportSTLOptionsKit &		UnsetEverything();


		/*! Shows the binary export setting.
		 *	\param out_state Whether to write STL data in binary form.
		 *	\return <span class='code'>true</span> if a binary export setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowBinary(bool & out_state) const;

		/*! Shows the tessellation level setting.
		 *	\param out_type The type of the tessellation level setting which was specified.
		 *	\param out_level The predefined quality of the tessellation that will be generated for BRep data using the standard tessellaiton algorithm.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Standard.
		 *	\param out_limit_type The type of chord limit which was specified.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_limit The chord limit to use to generate tessellation for BRep data.  Its interpretation depends on the preceding argument.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\return <span class='code'>true</span> if a tessellation level setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationLevel(Tessellation::Type & out_type, Tessellation::Level & out_level, Tessellation::Chord::Limit & out_limit_type, double & out_limit, double & out_angle_tolerance) const;

		/*! Shows the tessellation accuracy setting.
		 *	\param out_type The type of the tessellation accuracy which was specified.
		 *	\return <span class='code'>true</span> if a tessellation accuracy setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTessellationAccuracy(Tessellation::Accuracy & out_type) const;

		/*! Shows the current tessellation retention setting.
		 *	\param out_state Whether to keep the current tessellation.
		 *	\return <span class='code'>true</span> if a current tessellation retention setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowCurrentTessellationRetention(bool & out_state) const;

		/*! Shows the tessellation maximum edge length setting.
		 *	\param out_length The tessellation maximum edge length setting.
		 *	\return <span class='code'>true</span> if a tessellation maximum edge length setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowMaximumEdgeLength(double & out_length) const;

		/*! Shows the binary export setting.
		*	\param out_state The units that will be associated with this export.
		*	\return <span class='code'>true</span> if a units export setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowUnits(Units & out_units) const;
	};

	/*! The HPS::Exchange::ExportOBJOptionsKit class is a user space object. It contains settings controlling how OBJ data is exported via Exchange. Calling HPS::Exchange::ExportOBJOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportOBJOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportOBJOptionsKit object. */
		ExportOBJOptionsKit();

		/*! The copy constructor creates a new ExportOBJOptionsKit object that contains the same settings as the source ExportOBJOptionsKit.
		* 	\param in_kit The source ExportOBJOptionsKit to copy. */
		ExportOBJOptionsKit(ExportOBJOptionsKit const & in_kit);

		/*! The move constructor creates an ExportOBJOptionsKit by transferring the underlying object of the rvalue reference to this ExportOBJOptionsKit.
		* 	\param in_that An rvalue reference to an ExportOBJOptionsKit to take the underlying object from. */
		ExportOBJOptionsKit(ExportOBJOptionsKit && in_that);

		virtual ~ExportOBJOptionsKit();

		static const HPS::Type staticType = HPS::Type::ExchangeExportOBJOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ExportOBJOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		*	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportOBJ.
		*	\return An ExportOBJOptionsKit with the default settings. */
		static ExportOBJOptionsKit	GetDefault();

		/*! Copies the source ExportOBJOptionsKit into this ExportOBJOptionsKit.
		* 	\param in_kit The source ExportOBJOptionsKit to copy. */
		void						Set(ExportOBJOptionsKit const & in_kit);

		/*! Copies this ExportOBJOptionsKit into the given ExportOBJOptionsKit.
		* 	\param out_kit The ExportOBJOptionsKit to populate with the contents of this ExportOBJOptionsKit. */
		void						Show(ExportOBJOptionsKit & out_kit) const;

		/*! Copies the source ExportOBJOptionsKit into this ExportOBJOptionsKit.
		* 	\param in_kit The source ExportOBJOptionsKit to copy.
		* 	\return A reference to this ExportOBJOptionsKit. */
		ExportOBJOptionsKit &		operator=(ExportOBJOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportOBJOptionsKit.
		*	\param in_that An rvalue reference to an ExportOBJOptionsKit to take the underlying object from.
		*	\return A reference to this ExportOBJOptionsKit. */
		ExportOBJOptionsKit &		operator=(ExportOBJOptionsKit && in_that);

		/*! Indicates whether this ExportOBJOptionsKit has any values set on it.
		* 	\return <span class='code'>true</span> if no values are set on this ExportOBJOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ExportOBJOptionsKit is equivalent to this ExportOBJOptionsKit.
		*	\param in_kit The source ExportOBJOptionsKit to compare to this ExportOBJOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ExportOBJOptionsKit const & in_kit) const;

		/*!	Check if the source ExportOBJOptionsKit is equivalent to this ExportOBJOptionsKit.
		*	\param in_kit The source ExportOBJOptionsKit to compare to this ExportOBJOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ExportOBJOptionsKit const & in_kit) const;

		/*!	Check if the source ExportOBJOptionsKit is not equivalent to this ExportOBJOptionsKit.
		*	\param in_kit The source ExportOBJOptionsKit to compare to this ExportOBJOptionsKit.
		*	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ExportOBJOptionsKit const & in_kit) const;

		/*! Sets folder where texture will be exported (if any). If NULL, then texture are written in the same directory as the .fbx. Symbol @ is expanded as Filename of current file (without extension)
		*	\param in_name UTF8-encoded path to the folder where texture will be exported.  This corresponds to the value that will be passed to and <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_obj_data.html">A3DRWParamsExportObjData::m_pcTextureFolder</a>.
		*	\return A reference to this ExportOBJOptionsKit.
		*/
		ExportOBJOptionsKit &			SetTextureFolder(char const * in_name);

		/*! Shows the texture folder setting.
		*	\param out_path The path to the folder where texture will be exported.
		*	\return <span class='code'>true</span> if a texture folder setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowTextureFolder(UTF8 & out_path) const;

		/*! Removes the texture folder setting.
		*	\return A reference to this ExportOBJOptionsKit. */
		ExportOBJOptionsKit &			UnsetTextureFolder();

		/*! Removes all settings from this ExportOBJOptionsKit.
		*	\return A reference to this ExportOBJOptionsKit. */
		ExportOBJOptionsKit &			UnsetEverything();
	};

	/*! The HPS::Exchange::ExportU3DOptionsKit class is a user space object. It contains settings controlling how U3D data is exported via Exchange. Calling HPS::Exchange::ExportU3DOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportU3DOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportU3DOptionsKit object. */
		ExportU3DOptionsKit();

		/*! The copy constructor creates a new ExportU3DOptionsKit object that contains the same settings as the source ExportU3DOptionsKit.
		 * 	\param in_kit The source ExportU3DOptionsKit to copy. */
		ExportU3DOptionsKit(ExportU3DOptionsKit const & in_kit);

		/*! The move constructor creates an ExportU3DOptionsKit by transferring the underlying object of the rvalue reference to this ExportU3DOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportU3DOptionsKit to take the underlying object from. */
		ExportU3DOptionsKit(ExportU3DOptionsKit && in_that);

		virtual ~ExportU3DOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportU3DOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ExportU3DOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportU3D.
		 *	\return An ExportU3DOptionsKit with the default settings. */
		static ExportU3DOptionsKit	GetDefault();

		/*! Copies the source ExportU3DOptionsKit into this ExportU3DOptionsKit.
		 * 	\param in_kit The source ExportU3DOptionsKit to copy. */
		void						Set(ExportU3DOptionsKit const & in_kit);

		/*! Copies this ExportU3DOptionsKit into the given ExportU3DOptionsKit.
		 * 	\param out_kit The ExportU3DOptionsKit to populate with the contents of this ExportU3DOptionsKit. */
		void						Show(ExportU3DOptionsKit & out_kit) const;

		/*! Copies the source ExportU3DOptionsKit into this ExportU3DOptionsKit.
		 * 	\param in_kit The source ExportU3DOptionsKit to copy.
		 * 	\return A reference to this ExportU3DOptionsKit. */
		ExportU3DOptionsKit &		operator=(ExportU3DOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportU3DOptionsKit.
		 *	\param in_that An rvalue reference to an ExportU3DOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportU3DOptionsKit. */
		ExportU3DOptionsKit &		operator=(ExportU3DOptionsKit && in_that);

		/*! Indicates whether this ExportU3DOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportU3DOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ExportU3DOptionsKit is equivalent to this ExportU3DOptionsKit. 
		 *	\param in_kit The source ExportU3DOptionsKit to compare to this ExportU3DOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ExportU3DOptionsKit const & in_kit) const;

		/*!	Check if the source ExportU3DOptionsKit is equivalent to this ExportU3DOptionsKit. 
		 *	\param in_kit The source ExportU3DOptionsKit to compare to this ExportU3DOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ExportU3DOptionsKit const & in_kit) const;

		/*!	Check if the source ExportU3DOptionsKit is not equivalent to this ExportU3DOptionsKit. 
		 *	\param in_kit The source ExportU3DOptionsKit to compare to this ExportU3DOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ExportU3DOptionsKit const & in_kit) const;


		/*! Sets the version of the U3D data to export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_u3_d_data.html#ae7d0446b90139bee7ccf7d0eef4a7204">A3DRWParamsExportU3DData::m_eU3DVersion</a>.
		 *	\param in_version The version of the U3D data to export.
		 *	\return A reference to this ExportU3DOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-u3d-version">Default value</a>
		 */
		ExportU3DOptionsKit &		SetVersion(U3D::Version in_version);

		/*! Sets the level of compression to apply to tessellations in the U3D data for export.  If no compression level is set, no compression will be applied.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_u3_d_data.html#ae9321a865749efe613aa1d141dff426b">A3DRWParamsExportU3DData::m_ucMeshQualityValue</a>.
		 *	\param in_level The level of compression to apply to tessellations in the U3D data for export.  This value should be in the range <span class='code'>[0, 100]</span>.
		 *	\return A reference to this ExportU3DOptionsKit. */
		ExportU3DOptionsKit &		SetCompression(unsigned char in_level);


		/*! Removes the version setting.
		 *	\return A reference to this ExportU3DOptionsKit. */
		ExportU3DOptionsKit &		UnsetVersion();

		/*! Removes the compression setting.
		 *	\return A reference to this ExportU3DOptionsKit. */
		ExportU3DOptionsKit &		UnsetCompression();

		/*! Removes all settings from this ExportU3DOptionsKit.
		 *	\return A reference to this ExportU3DOptionsKit. */
		ExportU3DOptionsKit &		UnsetEverything();


		/*! Shows the version setting.
		 *	\param out_version The version of the U3D data to export.
		 *	\return <span class='code'>true</span> if a version setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowVersion(U3D::Version & out_version) const;

		/*! Shows the compression setting.
		 *	\param out_level The level of compression to apply to tessellations in the U3D data for export.
		 *	\return <span class='code'>true</span> if a compression setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowCompression(unsigned char & out_level) const;
	};

	/*! The HPS::Exchange::ExportXMLOptionsKit class is a user space object. It contains settings controlling how XML data is exported via Exchange. Calling HPS::Exchange::ExportXMLOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportXMLOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportXMLOptionsKit object. */
		ExportXMLOptionsKit();

		/*! The copy constructor creates a new ExportXMLOptionsKit object that contains the same settings as the source ExportXMLOptionsKit.
		 * 	\param in_kit The source ExportXMLOptionsKit to copy. */
		ExportXMLOptionsKit(ExportXMLOptionsKit const & in_kit);

		/*! The move constructor creates an ExportXMLOptionsKit by transferring the underlying object of the rvalue reference to this ExportXMLOptionsKit.
		 * 	\param in_that An rvalue reference to an ExportXMLOptionsKit to take the underlying object from. */
		ExportXMLOptionsKit(ExportXMLOptionsKit && in_that);

		virtual ~ExportXMLOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportXMLOptionsKit;
		HPS::Type					ObjectType() const { return staticType; }

		/*! Creates an ExportXMLOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportXML.
		 *	\return An ExportXMLOptionsKit with the default settings. */
		static ExportXMLOptionsKit	GetDefault();

		/*! Copies the source ExportXMLOptionsKit into this ExportXMLOptionsKit.
		 * 	\param in_kit The source ExportXMLOptionsKit to copy. */
		void						Set(ExportXMLOptionsKit const & in_kit);

		/*! Copies this ExportXMLOptionsKit into the given ExportXMLOptionsKit.
		 * 	\param out_kit The ExportXMLOptionsKit to populate with the contents of this ExportXMLOptionsKit. */
		void						Show(ExportXMLOptionsKit & out_kit) const;

		/*! Copies the source ExportXMLOptionsKit into this ExportXMLOptionsKit.
		 * 	\param in_kit The source ExportXMLOptionsKit to copy.
		 * 	\return A reference to this ExportXMLOptionsKit. */
		ExportXMLOptionsKit &		operator=(ExportXMLOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportXMLOptionsKit.
		 *	\param in_that An rvalue reference to an ExportXMLOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportXMLOptionsKit. */
		ExportXMLOptionsKit &		operator=(ExportXMLOptionsKit && in_that);

		/*! Indicates whether this ExportXMLOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportXMLOptionsKit, <span class='code'>false</span> otherwise. */
		bool						Empty() const;

		/*!	Check if the source ExportXMLOptionsKit is equivalent to this ExportXMLOptionsKit. 
		 *	\param in_kit The source ExportXMLOptionsKit to compare to this ExportXMLOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						Equals(ExportXMLOptionsKit const & in_kit) const;

		/*!	Check if the source ExportXMLOptionsKit is equivalent to this ExportXMLOptionsKit. 
		 *	\param in_kit The source ExportXMLOptionsKit to compare to this ExportXMLOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool						operator==(ExportXMLOptionsKit const & in_kit) const;

		/*!	Check if the source ExportXMLOptionsKit is not equivalent to this ExportXMLOptionsKit. 
		 *	\param in_kit The source ExportXMLOptionsKit to compare to this ExportXMLOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool						operator!=(ExportXMLOptionsKit const & in_kit) const;


		/*! Sets whether to export metadata.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_x_m_l_data.html#a4cb6a30be9204c7b4ba1f328277b698f">A3DRWParamsExportXMLData::m_bExportMetadata</a>.
		 *	\param in_state Whether to export metadata.
		 *	\return A reference to this ExportXMLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-xml-metadata">Default value</a>
		 */
		ExportXMLOptionsKit &		SetMetadata(bool in_state);

		/*! Sets whether to export transformations.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_x_m_l_data.html#acc79c32566fa8af73cead09de81097fd">A3DRWParamsExportXMLData::m_bExportTransformations</a>.
		 *	\param in_state Whether to export transformations.
		 *	\return A reference to this ExportXMLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-xml-transformations">Default value</a>
		 */
		ExportXMLOptionsKit &		SetTransformations(bool in_state);

		/*! Sets whether to export materials.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_x_m_l_data.html#a5e041e21b0e91ca6cbd2d955bbab8ce3">A3DRWParamsExportXMLData::m_bExportColorMaterial</a>.
		 *	\param in_state Whether to export materials.
		 *	\return A reference to this ExportXMLOptionsKit. 
		 *  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-xml-materials">Default value</a>
		 */
		ExportXMLOptionsKit &		SetMaterials(bool in_state);


		/*! Removes the metadata export setting.
		 *	\return A reference to this ExportXMLOptionsKit. */
		ExportXMLOptionsKit &		UnsetMetadata();

		/*! Removes the transformation export setting.
		 *	\return A reference to this ExportXMLOptionsKit. */
		ExportXMLOptionsKit &		UnsetTransformations();

		/*! Removes the material export setting.
		 *	\return A reference to this ExportXMLOptionsKit. */
		ExportXMLOptionsKit &		UnsetMaterials();

		/*! Removes all settings from this ExportXMLOptionsKit.
		 *	\return A reference to this ExportXMLOptionsKit. */
		ExportXMLOptionsKit &		UnsetEverything();


		/*! Shows the metadata export setting.
		 *	\param out_state Whether to export metadata.
		 *	\return <span class='code'>true</span> if a metadata export setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowMetadata(bool & out_state) const;

		/*! Shows the transformation export setting.
		 *	\param out_state Whether to export transformations.
		 *	\return <span class='code'>true</span> if a transformation export setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowTransformations(bool & out_state) const;

		/*! Shows the material export setting.
		 *	\param out_state Whether to export materials.
		 *	\return <span class='code'>true</span> if a material export setting was specified, <span class='code'>false</span> otherwise. */
		bool						ShowMaterials(bool & out_state) const;
	};
#endif

#if !defined(TARGET_OS_ANDROID)
	/*! The HPS::Exchange::ExportIGESOptionsKit class is a user space object.  It contains settings controlling how IGES data is exported via Exchange. Calling HPS::Exchange::ExportIGESOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportIGESOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportIGESOptionsKit object. */
		ExportIGESOptionsKit();

		/*! The copy constructor creates a new ExportIGESOptionsKit object that contains the same settings as the source ExportIGESOptionsKit.
		* 	\param in_kit The source ExportIGESOptionsKit to copy. */
		ExportIGESOptionsKit(ExportIGESOptionsKit const & in_kit);

		/*! The move constructor creates an ExportIGESOptionsKit by transferring the underlying object of the rvalue reference to this ExportIGESOptionsKit.
		* 	\param in_that An rvalue reference to an ExportIGESOptionsKit to take the underlying object from. */
		ExportIGESOptionsKit(ExportIGESOptionsKit && in_that);

		virtual ~ExportIGESOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportIGESOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates an ExportIGESOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		*	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportIGES.
		*	\return An ExportIGESOptionsKit with the default settings. */
		static ExportIGESOptionsKit		GetDefault();

		/*! Copies the source ExportIGESOptionsKit into this ExportIGESOptionsKit.
		* 	\param in_kit The source ExportIGESOptionsKit to copy. */
		void							Set(ExportIGESOptionsKit const & in_kit);

		/*! Copies this ExportIGESOptionsKit into the given ExportIGESOptionsKit.
		* 	\param out_kit The ExportIGESOptionsKit to populate with the contents of this ExportIGESOptionsKit. */
		void							Show(ExportIGESOptionsKit & out_kit) const;

		/*! Copies the source ExportIGESOptionsKit into this ExportIGESOptionsKit.
		* 	\param in_kit The source ExportIGESOptionsKit to copy.
		* 	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			operator=(ExportIGESOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportIGESOptionsKit.
		*	\param in_that An rvalue reference to an ExportIGESOptionsKit to take the underlying object from.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			operator=(ExportIGESOptionsKit && in_that);

		/*! Indicates whether this ExportIGESOptionsKit has any values set on it.
		* 	\return <span class='code'>true</span> if no values are set on this ExportIGESOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source ExportIGESOptionsKit is equivalent to this ExportIGESOptionsKit.
		*	\param in_kit The source ExportIGESOptionsKit to compare to this ExportIGESOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(ExportIGESOptionsKit const & in_kit) const;

		/*!	Check if the source ExportIGESOptionsKit is equivalent to this ExportIGESOptionsKit.
		*	\param in_kit The source ExportIGESOptionsKit to compare to this ExportIGESOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(ExportIGESOptionsKit const & in_kit) const;

		/*!	Check if the source ExportIGESOptionsKit is not equivalent to this ExportIGESOptionsKit.
		*	\param in_kit The source ExportIGESOptionsKit to compare to this ExportIGESOptionsKit.
		*	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(ExportIGESOptionsKit const & in_kit) const;


		/*! Sets whether to convert analytic surfaces (planes, cones, cylinder, etc.) to NURBS surfaces for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#a5f9a62b6128465c5d853bdca396fff82">A3DRWParamsExportIgesData::m_bSaveAnalyticsToNurbs</a>.
		*	\param in_state Whether to convert analytic surfaces to NURBS surfaces for export.
		*	\return A reference to this ExportIGESOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-iges-analytics-as-nurbs">Default value</a>
		*/
		ExportIGESOptionsKit &			SetAnalyticsAsNURBS(bool in_state);

		/*! Sets whether to convert polyhedrics to wireframe data for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#a56ff19520b1333904f49f4c0d094b84b">A3DRWParamsExportIgesData::m_bSaveFacetedToWireframe</a>.
		*	\param in_state Whether to convert polyhedrics to wireframe data for export.
		*	\return A reference to this ExportIGESOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-iges-polyhedrics-to-wire">Default value</a>
		*/
		ExportIGESOptionsKit &			SetFacetedAsWireframe(bool in_state);

		/*! Sets whether to convert solids to a set of independent faces for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#a06aaaa16ca40eebf1990c5f35fe1121d">A3DRWParamsExportIgesData::m_bSaveSolidsAsFaces</a>.
		*	\param in_state Whether to convert solids to a set of independent faces for export.
		*	\return A reference to this ExportIGESOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-iges-solids-as-faces">Default value</a>
		*/
		ExportIGESOptionsKit &			SetSolidsAsFaces(bool in_state);

		/*! Sets whether to export hidden objects.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#a6b42c907aa1748e6ff9913f4661c96fb">A3DRWParamsExportIgesData::m_bWriteHiddenObjects</a>.
		*	\param in_state Whether to export hidden objects.
		*	\return A reference to this ExportIGESOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-iges-hidden-objects">Default value</a>
		*/
		ExportIGESOptionsKit &			SetHiddenObjects(bool in_state);

		/*! Sets whether to export tessellation data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#aea1df80f24cd02b53e156283bfa131bc">A3DRWParamsExportIgesData::m_bWriteTessellation</a>.
		*	\param in_state Whether to export tessellation data.
		*	\return A reference to this ExportIGESOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-iges-tessellation">Default value</a>
		*/
		ExportIGESOptionsKit &			SetTessellation(bool in_state);

		/*! Sets the application name to put in the IGES data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#a6601a88d622a11cb3b1d07200df53cbe">A3DRWParamsExportIgesData::m_pcApplication</a>.
		*	\param in_name UTF8-encoded application name to put in the IGES data.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			SetApplication(char const * in_name);

		/*! Sets the version string to put in the IGES data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_iges_data.html#a26359fe0cd96f0e0913f5f355acf5866">A3DRWParamsExportIgesData::m_pcVersion</a>.
		*	\param in_version UTF8-encoded version string to put in the IGES data.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			SetVersion(char const * in_version);


		/*! Removes the analytics to NURBS conversion setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetAnalyticsAsNURBS();

		/*! Removes the faceted to wireframe conversion setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetFacetedAsWireframe();

		/*! Removes the solids to faces conversion setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetSolidsAsFaces();

		/*! Removes the hidden objects export setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetHiddenObjects();

		/*! Removes the tessellation export setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetTessellation();

		/*! Removes the application name setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetApplication();

		/*! Removes the version string setting.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetVersion();

		/*! Removes all settings from this ExportIGESOptionsKit.
		*	\return A reference to this ExportIGESOptionsKit. */
		ExportIGESOptionsKit &			UnsetEverything();


		/*! Shows the analytics to NURBS conversion setting.
		*	\param out_state Whether to convert analytic surfaces to NURBS surfaces for export.
		*	\return <span class='code'>true</span> if an analytics to NURBS conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowAnalyticsAsNURBS(bool & out_state) const;

		/*! Shows the faceted to wireframe conversion setting.
		*	\param out_state Whether to convert polyhedrics to wireframe data for export.
		*	\return <span class='code'>true</span> if a faceted to wireframe conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowFacetedAsWireframe(bool & out_state) const;

		/*! Shows the solids to faces conversion setting.
		*	\param out_state Whether to convert solids to a set of independent faces for export.
		*	\return <span class='code'>true</span> if a solids to faces conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowSolidsAsFaces(bool & out_state) const;

		/*! Shows the hidden object export setting.
		*	\param out_state Whether to export hidden objects.
		*	\return <span class='code'>true</span> if a hidden object export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowHiddenObjects(bool & out_state) const;

		/*! Shows the tessellation export setting.
		*	\param out_state Whether to export tessellation data.
		*	\return <span class='code'>true</span> if a tessellation export setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowTessellation(bool & out_state) const;

		/*! Shows the application name setting.
		*	\param out_name The application name to put in the IGES data.
		*	\return <span class='code'>true</span> if an application name setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowApplication(UTF8 & out_name) const;

		/*! Shows the version string setting.
		*	\param out_version The version string to put in the IGES data.
		*	\return <span class='code'>true</span> if a version string setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowVersion(UTF8 & out_version) const;
	};

	/*! The HPS::Exchange::ExportParasolidOptionsKit class is a user space object.  It contains settings controlling how Parasolid data is exported via Exchange. Calling HPS::Exchange::ExportParasolidOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportParasolidOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportParasolidOptionsKit object. */
		ExportParasolidOptionsKit();

		/*! The copy constructor creates a new ExportParasolidOptionsKit object that contains the same settings as the source ExportParasolidOptionsKit.
		* 	\param in_kit The source ExportParasolidOptionsKit to copy. */
		ExportParasolidOptionsKit(ExportParasolidOptionsKit const & in_kit);

		/*! The move constructor creates an ExportParasolidOptionsKit by transferring the underlying object of the rvalue reference to this ExportParasolidOptionsKit.
		* 	\param in_that An rvalue reference to an ExportParasolidOptionsKit to take the underlying object from. */
		ExportParasolidOptionsKit(ExportParasolidOptionsKit && in_that);

		virtual ~ExportParasolidOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportParasolidOptionsKit;
		HPS::Type							ObjectType() const { return staticType; }

		/*! Creates an ExportParasolidOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		*	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportParasolid.
		*	\return An ExportParasolidOptionsKit with the default settings. */
		static ExportParasolidOptionsKit	GetDefault();

		/*! Copies the source ExportParasolidOptionsKit into this ExportParasolidOptionsKit.
		* 	\param in_kit The source ExportParasolidOptionsKit to copy. */
		void								Set(ExportParasolidOptionsKit const & in_kit);

		/*! Copies this ExportParasolidOptionsKit into the given ExportParasolidOptionsKit.
		* 	\param out_kit The ExportParasolidOptionsKit to populate with the contents of this ExportParasolidOptionsKit. */
		void								Show(ExportParasolidOptionsKit & out_kit) const;

		/*! Copies the source ExportParasolidOptionsKit into this ExportParasolidOptionsKit.
		* 	\param in_kit The source ExportParasolidOptionsKit to copy.
		* 	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			operator=(ExportParasolidOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportParasolidOptionsKit.
		*	\param in_that An rvalue reference to an ExportParasolidOptionsKit to take the underlying object from.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			operator=(ExportParasolidOptionsKit && in_that);

		/*! Indicates whether this ExportParasolidOptionsKit has any values set on it.
		* 	\return <span class='code'>true</span> if no values are set on this ExportParasolidOptionsKit, <span class='code'>false</span> otherwise. */
		bool								Empty() const;

		/*!	Check if the source ExportParasolidOptionsKit is equivalent to this ExportParasolidOptionsKit.
		*	\param in_kit The source ExportParasolidOptionsKit to compare to this ExportParasolidOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool								Equals(ExportParasolidOptionsKit const & in_kit) const;

		/*!	Check if the source ExportParasolidOptionsKit is equivalent to this ExportParasolidOptionsKit.
		*	\param in_kit The source ExportParasolidOptionsKit to compare to this ExportParasolidOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool								operator==(ExportParasolidOptionsKit const & in_kit) const;

		/*!	Check if the source ExportParasolidOptionsKit is not equivalent to this ExportParasolidOptionsKit.
		*	\param in_kit The source ExportParasolidOptionsKit to compare to this ExportParasolidOptionsKit.
		*	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool								operator!=(ExportParasolidOptionsKit const & in_kit) const;


		/*! Sets whether to convert solids to a set of independent faces for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#a67ec36338dcfaa92d331c294d63ac94d">A3DRWParamsExportIgesData::m_bSaveSolidsAsFaces</a>.
		*	\param in_state Whether to convert solids to a set of independent faces for export.
		*	\return A reference to this ExportParasolidOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-solids">Default value</a>
		*/
		ExportParasolidOptionsKit &			SetSolidsAsFaces(bool in_state);

		/*! Sets whether to export tessellation data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#a9fc3ea289436ec57dcdbce78ddc57a97">A3DRWParamsExportParasolidData::m_bWriteTessellation</a>.
		*	\param in_state Whether to export tessellation data.
		*	\return A reference to this ExportParasolidOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-tess">Default value</a>
		*/
		ExportParasolidOptionsKit &			SetTessellation(bool in_state);

		/*! Sets whether to export hidden objects.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#a435f5a402a4e769a90105c312e75af3f">A3DRWParamsExportParasolidData::m_bWriteBlankedEntities</a>.
		*	\param in_state Whether to export hidden objects.
		*	\return A reference to this ExportParasolidOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-hidden-obj">Default value</a>
		*/
		ExportParasolidOptionsKit &			SetHiddenObjects(bool in_state);

		/*! Sets the application name to put in the Parasolid data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#aaf34910e2c76bc91ffce06955671597d">A3DRWParamsExportParasolidData::m_pcApplication</a>.
		*	\param in_name UTF8-encoded application name to put in the Parasolid data.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			SetApplication(char const * in_name);

		/*! Sets the version string to put in the Parasolid data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_parasolid_data.html#ae2cbff6b7cde472c983810da1d9ea0a2">A3DRWParamsExportParasolidData::m_pcVersion</a>.
		*	\param in_version UTF8-encoded version string to put in the Parasolid data.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			SetVersion(char const * in_version);

		/*! Sets whether to explode multi-connexes B-reps into multiple bodies.  This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_export_parasolid_data.html#a6ab80906d9944339d1c6d0d22d1a3bb6">A3DRWParamsExportParasolidData::m_bExplodeMultiBodies</a>.
		*	\param in_state Whether to explode multi-connexes B-reps into multiple bodies.
		*	\return A reference to this ExportParasolidOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-parasolid-explode">Default value</a>
		*/
		ExportParasolidOptionsKit&			SetExplodeMultiBodies(bool in_state);

		/*! Removes the solids to faces conversion setting.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			UnsetSolidsAsFaces();

		/*! Removes the tessellation export setting.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			UnsetTessellation();

		/*! Removes the hidden objects export setting.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			UnsetHiddenObjects();

		/*! Removes the application name setting.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			UnsetApplication();

		/*! Removes the version string setting.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			UnsetVersion();

		/*! Removes the explode multi bodies setting.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit&			UnsetExplodeMultiBodies();

		/*! Removes all settings from this ExportParasolidOptionsKit.
		*	\return A reference to this ExportParasolidOptionsKit. */
		ExportParasolidOptionsKit &			UnsetEverything();
		

		/*! Shows the solids to faces conversion setting.
		*	\param out_state Whether to convert solids to a set of independent faces for export.
		*	\return <span class='code'>true</span> if a solids to faces conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowSolidsAsFaces(bool & out_state) const;

		/*! Shows the tessellation export setting.
		*	\param out_state Whether to export tessellation data.
		*	\return <span class='code'>true</span> if a tessellation export setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowTessellation(bool & out_state) const;

		/*! Shows the hidden object export setting.
		*	\param out_state Whether to export hidden objects.
		*	\return <span class='code'>true</span> if a hidden object export setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowHiddenObjects(bool & out_state) const;

		/*! Shows the application name setting.
		*	\param out_name The application name to put in the Parasolid data.
		*	\return <span class='code'>true</span> if an application name setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowApplication(UTF8 & out_name) const;

		/*! Shows the version string setting.
		*	\param out_version The version string to put in the Parasolid data.
		*	\return <span class='code'>true</span> if a version string setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowVersion(UTF8 & out_version) const;

		/*! Shows the explode multi bodies setting.
		*	\param out_state The explode multi bodies setting to put in the Parasolid data.
		*	\return <span class='code'>true</span> if a explode multi bodies setting was specified, <span class='code'>false</span> otherwise. */
		bool								ShowExplodeMultiBodies(bool& out_state) const;
	};

	/*! The HPS::Exchange::ExportSTEPOptionsKit class is a user space object.  It contains settings controlling how STEP data is exported via Exchange. Calling HPS::Exchange::ExportSTEPOptionsKit::GetDefault() will return an options kit with values found in <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options">here.</a> */
	class EXCHANGE_API ExportSTEPOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportSTEPOptionsKit object. */
		ExportSTEPOptionsKit();

		/*! The copy constructor creates a new ExportSTEPOptionsKit object that contains the same settings as the source ExportSTEPOptionsKit.
		* 	\param in_kit The source ExportSTEPOptionsKit to copy. */
		ExportSTEPOptionsKit(ExportSTEPOptionsKit const & in_kit);

		/*! The move constructor creates an ExportSTEPOptionsKit by transferring the underlying object of the rvalue reference to this ExportSTEPOptionsKit.
		* 	\param in_that An rvalue reference to an ExportSTEPOptionsKit to take the underlying object from. */
		ExportSTEPOptionsKit(ExportSTEPOptionsKit && in_that);

		virtual ~ExportSTEPOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeExportSTEPOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates an ExportSTEPOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		*	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::ExportSTEP.
		*	\return An ExportSTEPOptionsKit with the default settings. */
		static ExportSTEPOptionsKit		GetDefault();

		/*! Copies the source ExportSTEPOptionsKit into this ExportSTEPOptionsKit.
		* 	\param in_kit The source ExportSTEPOptionsKit to copy. */
		void							Set(ExportSTEPOptionsKit const & in_kit);

		/*! Copies this ExportSTEPOptionsKit into the given ExportSTEPOptionsKit.
		* 	\param out_kit The ExportSTEPOptionsKit to populate with the contents of this ExportSTEPOptionsKit. */
		void							Show(ExportSTEPOptionsKit & out_kit) const;

		/*! Copies the source ExportSTEPOptionsKit into this ExportSTEPOptionsKit.
		* 	\param in_kit The source ExportSTEPOptionsKit to copy.
		* 	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			operator=(ExportSTEPOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportSTEPOptionsKit.
		*	\param in_that An rvalue reference to an ExportSTEPOptionsKit to take the underlying object from.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			operator=(ExportSTEPOptionsKit && in_that);

		/*! Indicates whether this ExportSTEPOptionsKit has any values set on it.
		* 	\return <span class='code'>true</span> if no values are set on this ExportSTEPOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source ExportSTEPOptionsKit is equivalent to this ExportSTEPOptionsKit.
		*	\param in_kit The source ExportSTEPOptionsKit to compare to this ExportSTEPOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(ExportSTEPOptionsKit const & in_kit) const;

		/*!	Check if the source ExportSTEPOptionsKit is equivalent to this ExportSTEPOptionsKit.
		*	\param in_kit The source ExportSTEPOptionsKit to compare to this ExportSTEPOptionsKit.
		*	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(ExportSTEPOptionsKit const & in_kit) const;

		/*!	Check if the source ExportSTEPOptionsKit is not equivalent to this ExportSTEPOptionsKit.
		*	\param in_kit The source ExportSTEPOptionsKit to compare to this ExportSTEPOptionsKit.
		*	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(ExportSTEPOptionsKit const & in_kit) const;


		/*! Sets the format of the STEP data to export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a71882c615bbe67ead32b1113825d0f5b">A3DRWParamsExportStepData::m_eStepFormat</a>.
		*	\param in_format The format of the STEP data to export.
		*	\return A reference to this ExportSTEPOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-step-format">Default value</a>
		*/
		ExportSTEPOptionsKit &			SetFormat(STEP::Format in_format);

		/*! Sets whether to convert analytic surfaces (planes, cones, cylinder, etc.) to NURBS surfaces for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a89d690650e250f2bef9dbc80a21e3d9b">A3DRWParamsExportStepData::m_bSaveAnalyticsToNurbs</a>.
		*	\param in_state Whether to convert analytic surfaces to NURBS surfaces for export.
		*	\return A reference to this ExportSTEPOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-step-analytics-nurbs">Default value</a>
		*/
		ExportSTEPOptionsKit &			SetAnalyticsAsNURBS(bool in_state);

		/*! Sets whether to convert polyhedrics to wireframe data for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#ab17ce385c872e4af49241096a0cf1c41">A3DRWParamsExportStepData::m_bSaveFacetedToWireframe</a>.
		*	\param in_state Whether to convert polyhedrics to wireframe data for export.
		*	\return A reference to this ExportSTEPOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-step-faceted-as-wireframe">Default value</a>
		*/
		ExportSTEPOptionsKit &			SetFacetedAsWireframe(bool in_state);

		/*! Sets whether to shorten names in the STEP data for export.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a44c272717ab0bbec16f8f399e3b148a3">A3DRWParamsExportStepData::m_bUseShortNames</a>.
		*	\param in_state Whether to shorten names in the STEP data for export.
		*	\return A reference to this ExportSTEPOptionsKit.
		*  \sa <a target="_blank" href="../../prog_guide/appendix_default_attribute_values.html#exchange-export-options-step-name-shortening">Default value</a>
		*/
		ExportSTEPOptionsKit &			SetNameShortening(bool in_state);

		/*! Sets the application name to put in the STEP data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#ab15765cdb5e1b82955aeb9c63a2aaf83">A3DRWParamsExportStepData::m_pcApplication</a>.
		*	\param in_name UTF8-encoded application name to put in the STEP data.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetApplication(char const * in_name);

		/*! Sets the version string to put in the STEP data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a80898bb5538eadc8429c0e52ef562ae2">A3DRWParamsExportStepData::m_pcVersion</a>.
		*	\param in_version UTF8-encoded version string to put in the STEP data.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetVersion(char const * in_version);

		/*! Sets whether to export PMI. This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a8b6d96a5391631c7b9aca2e5657b981b">A3DRWParamsExportStepData::m_bWritePMI</a>.
		*	\param in_state Whether to export PMI.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetPMI(bool in_state);

		/*! Sets whether to export curves. This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a560723534b6f345df6a6c1f64520fd47">A3DRWParamsExportStepData::m_bWriteUVCurves</a>.
		*	\param in_state Whether to export curves.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetCurves(bool in_state);

		/*! Sets whether to export attributes.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_export_step_data.html#a1ac6d05a47500fcc84c73e6ff2aeb6fd">A3DRWParamsExportStepData::m_bWriteAttributes</a>.
		*	\param in_state Whether to export attributes.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetAttributes(bool in_state);

		/*! Sets whether to export PMI with semantic information. This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_export_step_data.html#a08786e3a6599b9dcf9f2fe825f612809">A3DRWParamsExportStepData::m_bWritePMIWithSemantic</a>.
		*	\param in_state Whether to export PMI semantic information.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetPMIWithSemantic(bool in_state);

		/*! Sets whether to export PMI as tessellated rather than polyline. This corresponds to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/struct_a3_d_r_w_params_export_step_data.html#a8a2afa5d377c8a2b29a6ac2894f9847b">A3DRWParamsExportStepData::m_bWritePMIAsTessellated</a>.
		*	\param in_state Whether to export PMI as tessellated.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			SetPMIAsTessellated(bool in_state);

		/*! Removes the format setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetFormat();

		/*! Removes the analytics to NURBS conversion setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetAnalyticsAsNURBS();

		/*! Removes the faceted to wireframe conversion setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetFacetedAsWireframe();

		/*! Removes the name shortening setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetNameShortening();

		/*! Removes the application name setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetApplication();

		/*! Removes the version string setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetVersion();

		/*! Removes the PMI setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetPMI();

		/*! Removes the curves setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetCurves();

		/*! Removes the attributes setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetAttributes();

		/*! Removes the PMI with semantic setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetPMIWithSemantic();

		/*! Removes the PMI as tessellated setting.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetPMIAsTessellated();

		/*! Removes all settings from this ExportSTEPOptionsKit.
		*	\return A reference to this ExportSTEPOptionsKit. */
		ExportSTEPOptionsKit &			UnsetEverything();


		/*! Shows the format setting.
		*	\param out_format The format of the STEP data to export.
		*	\return <span class='code'>true</span> if a format setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowFormat(STEP::Format & out_format) const;

		/*! Shows the analytics to NURBS conversion setting.
		*	\param out_state Whether to convert analytic surfaces to NURBS surfaces for export.
		*	\return <span class='code'>true</span> if an analytics to NURBS conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowAnalyticsAsNURBS(bool & out_state) const;

		/*! Shows the faceted to wireframe conversion setting.
		*	\param out_state Whether to convert polyhedrics to wireframe data for export.
		*	\return <span class='code'>true</span> if a faceted to wireframe conversion setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowFacetedAsWireframe(bool & out_state) const;

		/*! Shows the name shortening setting.
		*	\param out_state Whether to shorten names in the STEP data for export.
		*	\return <span class='code'>true</span> if a name shortening setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowNameShortening(bool & out_state) const;

		/*! Shows the application name setting.
		*	\param out_name The application name to put in the STEP data.
		*	\return <span class='code'>true</span> if an application name setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowApplication(UTF8 & out_name) const;

		/*! Shows the version string setting.
		*	\param out_version The version string to put in the STEP data.
		*	\return <span class='code'>true</span> if a version string setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowVersion(UTF8 & out_version) const;

		/*! Shows the pmi setting.
		*	\param out_state Whether to export PMI.
		*	\return <span class='code'>true</span> if a PMI setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowPMI(bool & out_state) const;

		/*! Shows the curves setting.
		*	\param out_state Whether to export curves.
		*	\return <span class='code'>true</span> if a curves setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowCurves(bool & out_state) const;

		/*! Shows the attributes setting.
		*	\param out_state Whether to export attributes.
		*	\return <span class='code'>true</span> if an attributes setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowAttributes(bool & out_state) const;

		/*! Shows the pmi with semantic setting.
		*	\param out_state Whether to export PMI with semantic information.
		*	\return <span class='code'>true</span> if a PMI with semantic setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowPMIWithSemantic(bool & out_state) const;

		/*! Shows the pmi as tessellated setting.
		*	\param out_state Whether to export PMI as tessellated information.
		*	\return <span class='code'>true</span> if a PMI as tessellated setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowPMIAsTessellated(bool & out_state) const;
	};
#endif

	/*! The TessellationOptionsKit class is a user space object.  It contains settings controlling how STL data is exported via Exchange. */
	class EXCHANGE_API TessellationOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty TessellationOptionsKit object. */
		TessellationOptionsKit();

		/*! The copy constructor creates a new TessellationOptionsKit object that contains the same settings as the source TessellationOptionsKit.
		 * 	\param in_kit The source TessellationOptionsKit to copy. */
		TessellationOptionsKit(TessellationOptionsKit const & in_kit);

		/*! The move constructor creates an TessellationOptionsKit by transferring the underlying object of the rvalue reference to this TessellationOptionsKit.
		 * 	\param in_that An rvalue reference to an TessellationOptionsKit to take the underlying object from. */
		TessellationOptionsKit(TessellationOptionsKit && in_that);

		virtual ~TessellationOptionsKit();

	static const HPS::Type staticType = HPS::Type::ExchangeTessellationOptionsKit;
		HPS::Type						ObjectType() const { return staticType; }

		/*! Creates an TessellationOptionsKit which contains the default settings.  The returned object will not necessarily have values set for every option, but it will have settings for those options
		 *	where it is reasonable to have a default.  These values will be used for import unless an option is overridden by the options passed to File::Tessellation.
		 *	\return An TessellationOptionsKit with the default settings. */
		static TessellationOptionsKit	GetDefault();

		/*! Copies the source TessellationOptionsKit into this TessellationOptionsKit.
		 * 	\param in_kit The source TessellationOptionsKit to copy. */
		void							Set(TessellationOptionsKit const & in_kit);

		/*! Copies this TessellationOptionsKit into the given TessellationOptionsKit.
		 * 	\param out_kit The TessellationOptionsKit to populate with the contents of this TessellationOptionsKit. */
		void							Show(TessellationOptionsKit & out_kit) const;

		/*! Copies the source TessellationOptionsKit into this TessellationOptionsKit.
		 * 	\param in_kit The source TessellationOptionsKit to copy.
		 * 	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		operator=(TessellationOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this TessellationOptionsKit.
		 *	\param in_that An rvalue reference to an TessellationOptionsKit to take the underlying object from.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		operator=(TessellationOptionsKit && in_that);

		/*! Indicates whether this TessellationOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this TessellationOptionsKit, <span class='code'>false</span> otherwise. */
		bool							Empty() const;

		/*!	Check if the source TessellationOptionsKit is equivalent to this TessellationOptionsKit. 
		 *	\param in_kit The source TessellationOptionsKit to compare to this TessellationOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							Equals(TessellationOptionsKit const & in_kit) const;

		/*!	Check if the source TessellationOptionsKit is equivalent to this TessellationOptionsKit. 
		 *	\param in_kit The source TessellationOptionsKit to compare to this TessellationOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool							operator==(TessellationOptionsKit const & in_kit) const;

		/*!	Check if the source TessellationOptionsKit is not equivalent to this TessellationOptionsKit. 
		 *	\param in_kit The source TessellationOptionsKit to compare to this TessellationOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool							operator!=(TessellationOptionsKit const & in_kit) const;


		/*! Sets the predefined quality of the tessellation that will be generated for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a7949f7f08c25a321ce73843676072eb1">A3DRWParamsTessellationData::m_eTessellationLevelOfDetail</a>.
		 *	\param in_level The predefined quality of the tessellation that will be generated for BRep data.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		SetLevel(Tessellation::Level in_level);

		/*! Sets the user defined quality of the tessellation that will be generated for BRep data.  This implicitly sets <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a7949f7f08c25a321ce73843676072eb1">A3DRWParamsTessellationData::m_eTessellationLevel</a>
		 *		to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/group__a3d__read.html#ggac1fe96a5b9772683a41182babe1559f4ab5aec16efd29ca9df5969341c5c0a245">kA3DTessLODUserDefined</a>.
		 *	\param in_limit_type The type of the chord limit to be specified.
		 *	\param in_limit Either the ratio of the tessellation chord height to the bounding box height which should be in the range <span class='code'>[50, 10000]</span>, or the maximum distance between the surface definition and the tessellation approximation, depending on the preceding argument.
		 *	\param in_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation which should be in the range <span class='code'>[10, 40]</a>.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a58e1c697c1fd5243e338485edfaf4e8d">A3DRWParamsTessellationData::m_dAngleToleranceDeg</a>.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		SetLevel(Tessellation::Chord::Limit in_limit_type, double in_limit, double in_angle_tolerance);

		/*! Sets the accuracy of the tessellation that will be generated for BRep data.
		 *	\param in_type The accuracy to use for tessellation generation.
		 *	\param in_accurate_normals Whether to compute surface normals when performing accurate tessellation.  This only applies if <span class='code'>in_type</span> is set to Tessellation::Accuracy::Accurate or Tessellation::GridAccuracy::GridAccurate.
		 *		This correspond to the (negated) value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a51d14d591da50a1d28fd6a61d7ed7eab">A3DRWParamsTessellationData::m_bDoNotComputeNormalsInAccurateTessellation</a>.
		 *	\param in_accurate_surface_curvatures Whether to take into account surfaces curvature in accurate tessellation to controle triangles elongation directions.  This only applies if <span class='code'>in_type</span> is set to Tessellation::Accuracy::Accurate or Tessellation::GridAccuracy::GridAccurate.
		 *		This correspond to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a965186e2951e1cc735ee12311cb17698">A3DRWParamsTessellationData::m_bAccurateSurfaceCurvatures</a>.
		 *	\param in_accurate_grid_maximum_stitch_length Maximal grid stitch length. Disabled if value is 0. Be careful, a too small value can generate a huge tessellation.  This only applies if <span class='code'>in_type</span> is set to Tessellation::GridAccuracy::GridAccurate.
		 *		This correspond to the value that will be passed to <a target="_blank" href="https://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a965186e2951e1cc735ee12311cb17698">A3DRWParamsTessellationData::m_dAccurateTessellationWithGridMaximumStitchLength</a>.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		SetAccuracy(Tessellation::Accuracy in_type, bool in_accurate_normals = true, bool in_accurate_surface_curvatures = false, double in_accurate_grid_maximum_stitch_length = 0.0);

		/*! Sets whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a13ff3862be1e4ff11a00fcf6c9373e29">A3DRWParamsTessellationData::m_bKeepUVPoints</a>.
		 *	\param in_state Whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		SetUVPointPreservation(bool in_state);

		/*! Sets the maximum length of triangle edges in the tessellation generated for BRep data.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/build/api_ref/struct_a3_d_r_w_params_tessellation_data.html#a124a4c06b502d452312a70c1cd92eb09">A3DRWParamsTessellationData::m_dMaximalTriangleEdgeLength</a>.
		 *	\param in_length The maximum length of triangle edges in the tessellation generated for BRep data.  The value must be non-negative, and if set to <span class='code'>0.0</span>, the length will not be used when generating the tessellation.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		SetMaximumEdgeLength(double in_length);


		/*! Removes the level setting.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		UnsetLevel();

		/*! Removes the accuracy setting.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		UnsetAccuracy();

		/*! Removes the UV preservation setting.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		UnsetUVPointPreservation();

		/*! Removes the maximum edge length setting.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		UnsetMaximumEdgeLength();

		/*! Removes all settings from this TessellationOptionsKit.
		 *	\return A reference to this TessellationOptionsKit. */
		TessellationOptionsKit &		UnsetEverything();


		/*! Shows the level setting.
		 *	\param out_type The type of the level setting which was specified.
		 *	\param out_level The predefined quality of the tessellation that will be generated for BRep data.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Standard.
		 *	\param out_limit_type The type of chord limit which was specified.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_limit The chord limit to use to generate tessellation for BRep data.  Its interpretation depends on the preceding argument.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\param out_angle_tolerance The maximum angle in degrees between two consecutive wire elements in the generated tessellation.  This is only valid if <span class='code'>out_type</span> is Tessellation::Type::Custom.
		 *	\return <span class='code'>true</span> if a level setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowLevel(Tessellation::Type & out_type, Tessellation::Level & out_level, Tessellation::Chord::Limit & out_limit_type, double & out_limit, double & out_angle_tolerance) const;

		/*! Shows the accuracy setting.
		 *	\param out_type The type of the accuracy which was specified.
		 *	\param out_accurate_normals Whether to compute surface normals when performing accurate tessellation.  This only applies if <span class='code'>out_type</span> is different than Tessellation::Accuracy::Standard.
		 *	\param out_accurate_surface_curvatures Whether to take into account surfaces curvature in accurate tessellation to controle triangles elongation directions. This only applies if <span class='code'>out_type</span> is different than Tessellation::Accuracy::Standard.
		 *	\param out_accurate_grid_maximum_stitch_length Maximal grid stitch length. Disabled if value is 0. Be careful, a too small value can generate a huge tessellation. This only applies if <span class='code'>out_type</span> is Tessellation::Accuracy::GridAccurate.
		 *	\return <span class='code'>true</span> if an accuracy setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowAccuracy(Tessellation::Accuracy & out_type, bool & out_accurate_normals, bool & out_accurate_surface_curvatures, double & out_accurate_grid_maximum_stitch_length) const;

		/*! Shows the UV preservation setting.
		 *	\param out_state Whether to keep parametric points as texture coordinates when generating the tessellation for BRep data.
		 *	\return <span class='code'>true</span> if a UV preservation setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowUVPointPreservation(bool & out_state) const;

		/*! Shows the maximum edge length setting.
		 *	\param out_length The maximum edge length setting.
		 *	\return <span class='code'>true</span> if a maximum edge length setting was specified, <span class='code'>false</span> otherwise. */
		bool							ShowMaximumEdgeLength(double & out_length) const;
	};

	/*!
	The CommonMeasurementOperator class defines an operator which contains many often used functions when inserting measurements.
	Users can build a custom measurement operator by deriving from this class.
	This operator requires the model be loaded using the Exchange bridge, and the model must contain B-rep.
	*/
	class EXCHANGE_API CommonMeasurementOperator : public Operator
	{
	public:
		enum class Tags
		{
			Name = 0,
			MeasurementType,
			Radius,
			Inverted,
			VectorX,
			VectorY,
			VectorZ,
		};

		CommonMeasurementOperator();

		CommonMeasurementOperator(Exchange::CADModel const & in_cad_model, MouseButtons in_mouse_trigger = MouseButtons::ButtonLeft(), ModifierKeys in_modifier_trigger = ModifierKeys());

		/*! Returns the name of the operator. */
		virtual HPS::UTF8		GetName() const	override { return "HPS_ExchangeCommonMeasurementOperator"; }

		virtual void			OnViewAttached(HPS::View const & in_attached_view) override;
		virtual void			OnViewDetached(HPS::View const & in_detached_view) override;

		Exchange::CADModel GetCADModel() const;
		void SetCADModel(Exchange::CADModel const & in_cad_model);

		/*! Returns the precision used in the measurement (number of digits after the decimal point) */
		size_t				GetPrecision() const;

		/*! Changes the precision used in the measurement (number of digits after the decimal point) 
		 * Only affects future measurements. */
		void				SetPrecision(size_t in_precision);
		
		/*! Returns the material used for the measurements */
		MaterialMappingKit	GetMaterial() const;

		/*! Changes the material used for the measurements.
		 * Affects all measurements, even those already inserted. */
		void				SetMaterial(MaterialMappingKit const & in_material_mapping);

		/*! Returns the text attributes used for the measurements */
		TextAttributeKit	GetTextAttributes() const;

		/*! Changes the text attributes used for the measurements.
		 * Affects all measurements, even those already inserted. */
		void				SetTextAttribute(TextAttributeKit const & in_text_attributes);

		/*! Returns the top measurement segment containing all measurements*/
		SegmentKey			GetMeasurementSegment() const;

		/*! Returns the name to be used for the new measurement segment name*/
		static UTF8			GetNewMeasurementSegmentName(HPS::UTF8 const & in_prefix);

		/* Deletes all measurements */
		void				DeleteMeasurements();

		class EXCHANGE_API MeasurementInsertedEvent : public HPS::Event
		{
		public:
			/*! The default constructor creates an empty MeasurementInsertedEvent object. */
			MeasurementInsertedEvent() : Event()
			{
				channel = GetClassID();
				consumable = false;
			}

			MeasurementInsertedEvent(HPS::Key const & in_measurement_key, HPS::View const & in_view) : Event()
			{
				channel = GetClassID();
				consumable = false;
				measurement_key = in_measurement_key;
				view = in_view;
			}

			/*! This constructor converts an Event Object to a MeasurementInsertedEvent object.
			 * 	\param in_event The Event Object to be converted. */
			MeasurementInsertedEvent(Event const & in_event) : Event(in_event)
			{
				if (in_event.GetChannel() == Object::ClassID<MeasurementInsertedEvent>())
				{
					auto that = static_cast<MeasurementInsertedEvent const &>(in_event);
					measurement_key = that.measurement_key;
					view = that.view;
				}
				else
					throw HPS::InvalidSpecificationException("Invalid Event Type to Cast From.");
			}

			~MeasurementInsertedEvent();

			/*! Allocates and returns a copy of this MeasurementInsertedEvent.
			 * 	\return A copy of this MeasurementInsertedEvent. */
			Event * Clone() const
			{
				MeasurementInsertedEvent * new_event = new MeasurementInsertedEvent(*this);
				return new_event;
			}

			Key measurement_key;
			View view;
		};

		class EXCHANGE_API MeasurementDeletedEvent : public HPS::Event
		{
		public:
			/*! The default constructor creates an empty MeasurementDeletedEvent object. */
			MeasurementDeletedEvent() : Event()
			{
				channel = GetClassID();
				consumable = false;
			}

			MeasurementDeletedEvent(HPS::UTF8 const & in_measurement_name, HPS::View const & in_view) : Event()
			{
				channel = GetClassID();
				consumable = false;
				measurement_name = in_measurement_name;
				view = in_view;
			}

			/*! This constructor converts an Event Object to a MeasurementDeletedEvent object.
			 * 	\param in_event The Event Object to be converted. */
			MeasurementDeletedEvent(Event const & in_event) : Event(in_event)
			{
				if (in_event.GetChannel() == Object::ClassID<MeasurementDeletedEvent>())
				{
					auto that = static_cast<MeasurementDeletedEvent const &>(in_event);
					measurement_name = that.measurement_name;
					view = that.view;
				}
				else
					throw HPS::InvalidSpecificationException("Invalid Event Type to Cast From.");
			}

			~MeasurementDeletedEvent();

			/*! Allocates and returns a copy of this MeasurementDeletedEvent.
			 * 	\return A copy of this MeasurementDeletedEvent. */
			Event * Clone() const
			{
				MeasurementDeletedEvent * new_event = new MeasurementDeletedEvent(*this);
				return new_event;
			}

			UTF8 measurement_name;
			View view;
		};

	protected:
		Exchange::CADModel	cad_model;
		size_t				measurement_precision;
		UTF8				units;
		MaterialMappingKit	materials;
		TextAttributeKit	text_attributes;
		SegmentKey			measurement_segment;
		GlyphDefinition		left_arrow;
		GlyphDefinition		right_arrow;
		SelectionOptionsKit	selection_options;
		bool				manipulate_measurement;				//whether we are manipulating a previously entered measurement
		Vector				camera_direction;
		PortfolioKey		portfolio;
		SegmentKey			style_segment;
		HighlightOptionsKit	highlight_options;

		static size_t		length_measurement_index;
		static size_t		radius_measurement_index;
		static size_t		distance_measurement_index;
		static size_t		angle_measurement_index;

		void Tag(HPS::Key & tag, const char * message, Tags tag_index);
		void GetUnits();
		void SetGlyphColor();
		void GetCameraDirection();
		void SetupConstructionSegment();
		void PositionLinearMeasurementGeometry(WindowPoint const & window_cursor_location, KeyPath const & event_path, LineKey & leader_line_one, LineKey & leader_line_two, 
			LineKey & measurement_line, LineKey & line_to_cursor, Point & original_point_one, Point & original_point_two, TextKey & text, UTF8 const & text_string,
			Point & distance_point_one, Point & distance_point_two, SegmentKey & current_measurement_segment, Plane const & measurement_plane = Plane(), Vector const & explicit_direction = Vector());
		float GetModelScale(Exchange::Component const & component);
		void SetMaterialsOnMeasurementSegment(HPS::SegmentKey const & set_materials_here, HPS::MaterialMappingKit const & materials_to_apply);
	};

	/*!
	The MeasurementOperator class defines an operator which allows the user to insert measurements into the scene
	This operator requires the model be loaded using the Exchange bridge, and the model must contain B-rep.

	The behavior of the operator, and its usage, vary based on the type of measurement the user wishes to insert.

	- Point to Point measurement:
	The Point to Point measurement mode allows the user to measure the distance between two arbitrary points.
	Usage:
	- Click or tap where you want to insert the first measurement point.
	- Click or tap where you want to insert the second measurement point
	- Move the mouse or drag your finger to move the measurement geometry around.
	- Click or lift your finger to position the measurement.

	- Edge and Radius measurement:
	The Edge and Radius measurement mode allows the user to measure the length of an edge, or the radius of a circle,
	based on whether the edge clicked on is a line or a circle.
	Usage:
	- Click or tap on the edge you want to measure
	- Move the mouse or drag your finger to move the measurement geometry around.
	- Click again or lift your finger to position the measurement.
	- If the edge you selected is part of a circle, the circle radius will be measured, otherwise the edge length will be measured.

	- Feature to Feature measurement:
	The Feature to Feature measurement mode allows the user to measure the shortest distance between two features.
	Usage:
	- Click or tap on a face
	- Click or tap on a different face
	- Move the mouse or drag your finger to move the measurement geometry around.
	- Click again or lift your finger to position the measurement.

	- If both faces are planar, the shortest distance between them is measured
	- If one face is planar and the other is id conical or cylindrical, the shortest distance between the center line and the face is measured
	- If both faces are conical or cylindrical, the shortest distance between the two center lines is measured

	- Face Angle measurement:
	The Face Angle measurement mode allows the user to measure the angle between two planar, non-parallel faces.
	Usage:
	- Click or tap on a face
	- Click or tap on a different face, which is not parallel to the first face
	- Move the mouse or drag your finger to move the measurement geometry around.
	- Click again or lift your finger to position the measurement.

	Once a measurement has been inserted, its position can be modified by clicking on it, moving the mouse, and clicking once more once it is in the desired position.
    On touch-enabled devices measurements can be repositioned by dragging them into a new position.
	Pressing Escape will delete the measurement currently being inserted.

	The operator will inject an Event of type MeasurementInsertedEvent every time a new measurement is inserted.
	The user can handle this event to obtain the segment key associated with every inserted measurement.

	A valid CADModel needs to be passed to the constructor of this class.
	*/
	class EXCHANGE_API MeasurementOperator : public CommonMeasurementOperator
	{
	public:
		enum class MeasurementType
		{
			PointToPoint,
			EdgeAndRadius,
			FeatureToFeature,
			FaceAngle,
		};

		MeasurementOperator();

		MeasurementOperator(Exchange::CADModel const & in_cad_model, MouseButtons in_mouse_trigger = MouseButtons::ButtonLeft(), ModifierKeys in_modifier_trigger = ModifierKeys());

		/*! Returns the name of the operator. */
		virtual HPS::UTF8		GetName() const	override{ return "HPS_ExchangeMeasurementOperator"; }

		virtual void OnViewAttached(HPS::View const & in_attached_view) override;
		virtual void OnViewDetached(HPS::View const & in_detached_view) override;

		/*! This function is called whenever HPS receives a MouseEvent that signals a mouse button was pressed.
		*  This function inserts the measuring points.
		* \param in_state A MouseState object describing the current mouse state.
		* \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
		virtual bool OnMouseDown(MouseState const & in_state) override;

		/*! This function is called whenever HPS receives a MouseEvent that signals a mouse button was released.
		* \param in_state A MouseState object describing the current mouse state.
		* \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
		virtual bool OnMouseUp(MouseState const & in_state) override;

		/*! This function is called whenever HPS receives a MouseEvent that signals the mouse moved
        * When the user has just inserted the second measurement point, this function allows the user to move the measurement
        * If the user has selected an already existing measurement, this function allows the user to reposition the measurement
		* \param in_state A MouseState object describing the current mouse state.
		* \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
		virtual bool OnMouseMove(MouseState const & in_state) override;
    
        /*! This function is called whenever HPS receives a TouchEvent that signals the device was touched.
        *  This function inserts the measuring points.
        * \param in_state A TouchState object describing the current touch state.
        * \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
        virtual bool OnTouchDown(TouchState const & in_state) override;
    
        /*! This function is called whenever HPS receives a TouchEvent that signals a point of contact has been released.
        * \param in_state A TouchState object describing the current touch state.
        * \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
        virtual bool OnTouchUp(TouchState const & in_state) override;
    
        /*! This function is called whenever HPS receives a TouchEvent that signals a point of contact has moved.
        * When the user has just inserted the second measurement point, this function allows the user to move the measurement
        * If the user has selected an already existing measurement, this function allows the user to reposition the measurement
        * \param in_state A TouchState object describing the current touch state.
        * \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
        virtual bool OnTouchMove(TouchState const & in_state) override;

		/*! This function is called whenever HPS receives a KeyDownEvent that signals a key was pressed.
		* Pressing Escape while in the process of inserting a measurement deletes that measurement.
		* \return <span class='code'>true</span> if the input event was handled, <span class='code'>false</span> otherwise. */
		virtual bool OnKeyDown(KeyboardState const & in_state) override;

		/*! Sets the type of measurement to insert.
		* \param in_measurement_type The type of measurement to insert.*/
		void SetMeasurementType(MeasurementType in_measurement_type);

		/*! Returns the type of measurement the operator is currently set up to insert.
		* \return  The type of measurement the operator is currently set up to insert. */
		MeasurementType GetMeasurementType();

		/*! Whether measurable geometry should be highlighted when mousing over it
		* \param in_highlighting Whether measurable geometry should be highlighted when mousing over it. */
		void SetMouseOverHighlighting(bool in_highlighting);

		/*! Whether measurable geometry should be highlighted when mousing over it
		* \param in_highlighting Whether measurable geometry should be highlighted when mousing over it.
		* \param in_highlight_options_kit The highlight kit used for mouse over highlights */
		void SetMouseOverHighlighting(bool in_highlighting, HighlightOptionsKit const & in_highlight_options_kit);

		/*! Whether measurable geometry is highlighted when mousing over it
		* \return <span class='code'>true</span> if measurable geometry is highlighted on mouse over, <span class='code'>false</span> otherwise. */
		bool GetMouseOverHighlighting();

		/*! Returns the highlight option kit currently used for mouse over highlighting.
		* \return the highlight option kit currently used for mouse over highlighting. */
		HighlightOptionsKit GetHighlightOptions();

		/*! Whether a measurement is currently being inserted or edited.
		* \return <span class='code'>true</span> if a measurement is being inserted or manipulated, <span class='code'>false</span> otherwise. */
		bool IsMeasurementActive();

		/*! Delete the current measurement and brings the operator back to a state to start a new measurement */
		void DeleteLastMeasurement();

	private:
		enum class EdgeType											//used to determine the type of edge measured when using the EdgeAndRadius measurement type
		{
			Circle,
			Line,
			Generic,
		};

		class Surface												//helper class containing surfaces properties. Used for the FeatureToFeature measurement type
		{
		public:
			Surface();

			enum class SurfaceType
			{
				Plane,
				ConeOrCylinder,
				Unsupported,
			};

			SurfaceType		surface_type;							//the type of surface being measured
			Point			center;									//the center point of the surface
			Vector			normal;									//the center line of surfaces of type Cylinder or Cone
			LineKey			normal_points;							//the line representing the center line of surfaces of type Cylinder or Cone
			ComponentPath	path;									//the ComponentPath to this surface
		};

		//bookkeeping
		MeasurementType		measurement_type;						//the type of measurement to be inserted
		MeasurementType		temporary_measurement_type;				//the type of the measurement to be edited
		TouchID             tracked_touch_id;                       //the ID of the touch to track for OnTouchMove operations
        TouchID             current_touch_id;                       //the ID of the touch being processed
		SegmentKey			current_measurement;					//segment of the measurement being inserted / edited
		bool				operator_active;						//whether a measurement is in progress
		bool				end_measurement;						//whether we should end the current measurement
		CanvasArray			canvases;								//canvases related to the view where this operator is attached

		//measurement anchors
		size_t				anchors;								//valid for point-to-point and face-angle measurements
		bool				anchors_in_place;						//true if all the anchors have been placed
		Point				first_click_position;					//position of the first anchor
		Point				second_click_position;					//position of the second anchor

		//geometry for linear measurements
		MarkerKey			anchor_one;								//marker corresponding to the start of the measurement
		MarkerKey			anchor_two;								//marker corresponding to the end of the measurement
		LineKey				distance_line;							//a line representing the distance measured
		LineKey				leader_line_one;						//line connecting the first anchor point to the distance line
		LineKey				leader_line_two;						//line connecting the second anchor point to the distance line
		Point				distance_point_one;						//intersection of leader_line_one and distance_line
		Point				distance_point_two;						//intersection of leader_line_two and distance_line
		LineKey				line_to_cursor;							//line extending from distance_point_one to the cursor
		TextKey				text;									//text representing the measurement and units
		UTF8				text_string;							//the contents of the text
		Vector				measurement_direction;					//the direction of the measurement
		bool				use_explicit_direction;					//if true, we are moving the measurement along a specific vector, called explicit_direction
		Vector				explicit_direction;						//used if use_explicit_direction is true

		//geometry for radius measurement
		MarkerKey			center_marker;							//marker representing the center of the circle
		Point				circle_center;							//circle center
		float				radius;									//circle radius

		//edge specific data
		LineKey				edge_line;								//the edge being measured
		double				edge_length;							//length of the measured edge
		EdgeType			edge_type;								//the type of edge being measured

		//feature-to-feature specific data
		Surface				surface_one;							//data related to first selected surface
		Surface				surface_two;							//data related to second selected surface
		Plane				measurement_plane;						//the measurement plane
		LineKey				current_normal;							//the center line of surfaces of type Cone and Cylinder

		//angle specific data
		Vector				leader_line_one_direction;				//the direction of the first leader line
		Vector				leader_line_two_direction;				//the direction of the second leader line
		Vector				first_face_normal;						//the normal of the first selected face
		Vector				second_face_normal;						//the normal of the second selected face
		bool				inverted_measurement;					//the smallest of the two possible angles is always chosen. If the user wants the other angle, the measurement is marked as 'inverted'
		CircularArcKey		measurement_arc;						//an arc representing the measured angle
		LineKey				line_to_leader_line;					//line extending from one anchor to a leader line
		Vector				mid_point_direction;

		//selection kits
		SelectionOptionsKit point_to_point_selection;
		SelectionOptionsKit edge_radius_selection;
		SelectionOptionsKit	feature_to_feature_selection;
		SelectionOptionsKit	angle_selection;

		//highlighting
		bool				highlight_on_mouse_over;				//whether measurable geometry should be highlighted on mouse over
		HighlightOptionsKit mouse_over_highlight_options;			//the options used for mouse over highlighting
		HighlightOptionsKit edit_measurement_highlight_options;		//the options used for highlighting measurements while editing them
		UpdateNotifier		highlight_update_notifier;				//notifier used to know if the last highlight update has completed
		KeyPath				highlighted_path;						//highlighted_path
		bool				disable_highlighting;					//in some situation we temporarily disable highlighting to improve performance
		bool Highlight(MeasurementType in_measurement_type, WindowPoint const & in_location, WindowKey & in_window, KeyPath const & in_path);

		//input handling
		bool InputDown(WindowPoint const & in_location, WindowKey & in_window, KeyPath const & in_path, size_t number_of_clicks);
		bool InputMove(WindowPoint const & in_location, KeyPath const & in_path);
		bool InputUp(WindowKey & in_window);
		void ResetMeasurement();

		//inserting measurements
		void InsertPointToPointMeasurement(Point const & in_world_point);
		bool InsertEdgeRadiusMeasurement(KeyPath const & in_selection_path, WindowPoint & in_selection_position, Key const & in_edge_key);
		bool InsertFeatureToFeatureMeasurement(KeyPath const & in_selection_path, WindowPoint & in_selection_position, Key const & in_face_key, WindowKey & in_window);
		void InsertFeatureToFeatureGeometry(Point const & point_one, Point const & point_two, float distance);
		bool InsertAngleMeasurement(KeyPath const & in_selection_path, WindowPoint & in_selection_position, Key const & in_face_key, WindowKey & in_window, SelectionResults const & in_selection_results);
		void InvertMeasuredAngle(WindowKey & in_window);
		void AdjustLineToCursor(Point const & cursor_position);

		//saving measurements
		void TagMeasurement();
		void TagPointToPointMeasurement();
		void TagEdgeMeasurement();
		void TagRadiusMeasurement();
		void TagGenericEdgeMeasurement();
		void TagFeatureToFeatureMeasurement();
		void TagAngleMeasurement();

		//restoring measurements
		void RestoreMeasurement(SegmentKey const & measurement_segment);
		void RestorePointToPointMeasurement(SegmentKey const & measurement_segment);
		void RestoreEdgeMeasurement(SegmentKey const & measurement_segment);
		void RestoreRadiusMeasurement(SegmentKey const & measurement_segment);
		void RestoreGenericEdgeMeasurement(SegmentKey const & measurement_segment);
		void RestoreFeatureToFeatureMeasurement(SegmentKey const & measurement_segment);
		void RestoreAngleMeasurement(SegmentKey const & measurement_segment);

		//topology functions
		void GetEdgeLengthAndType(Exchange::Component const & edge_component);
		void GetSurfaceType(Exchange::Component const & face_component, Surface & surface);
		void PlaneToCenterLineDistance();
		void PlaneToPlaneDistance();
		void LineToLineDistance();
		float LineSegmentDistance(Point & p0, Point & p1, Point & q0, Point & q1, LineKey & normal_one, LineKey & normal_two, Point & out_point_on_edge, Point & out_point_on_center_line, bool clamp = true);
		Point ClosestPointOnTriangleToPoint(Point const & p, Point const & a, Point const & b, Point const & c);
		float ClosestPointSegmentSegment(Point const & p1, Point const & q1, Point const & p2, Point const & q2, Point & c1, Point & c2);
		bool IsPlane(Exchange::Component const & face_component);
		Point GetPlaneIntersection(Plane const & in_plane, KeyPath const & in_key_path, WindowPoint const & in_window_point);
	};

private:
	//! Private default constructor to prevent instantiation.
	Exchange();
};

}

#endif
