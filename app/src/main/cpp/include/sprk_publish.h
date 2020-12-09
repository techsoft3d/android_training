// Copyright (c) Tech Soft 3D, Inc.
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPRK_PUBLISH_H
#define SPRK_PUBLISH_H

#include "sprk.h"

typedef void A3DEntity;

#ifdef _MSC_VER
#  ifndef STATIC_APP
#    ifdef SPRK_PUBLISH
#      define PUBLISH_API __declspec (dllexport)
#    else
#      define PUBLISH_API __declspec (dllimport)
#    endif
#  endif
#else
#  include <stddef.h>
#  if defined(SPRK_PUBLISH) and defined(LINUX_SYSTEM)
#    ifndef STATIC_APP
#      define PUBLISH_API __attribute__ ((visibility ("default")))
#    endif
#  endif
#endif

#ifndef PUBLISH_API
#  define PUBLISH_API
#endif

namespace HPS
{

/*! The Publish class contains objects and enumerations used for exporting 3D PDF files via Publish. */
class PUBLISH_API Publish
{
public:
	class DocumentKit;
	class PageKit;
	class TemplateKit;
	class AnnotationKit;
	class ArtworkKit;
	class ViewKit;
	class TextKit;
	class ImageKit;
	class TableKit;
	class LinkKit;
	class ButtonKit;
	class CheckBoxKit;
	class RadioButtonKit;
	class ListBoxKit;
	class DropDownListKit;
	class SignatureFieldKit;
	class TextFieldKit;
	class SlideTableKit;
	class DocumentKey;
	class PageControl;

	typedef std::vector<PageKit, HPS::Allocator<PageKit>>						PageKitArray;
	typedef std::vector<AnnotationKit, HPS::Allocator<AnnotationKit>>			AnnotationKitArray;
	typedef std::vector<ViewKit, HPS::Allocator<ViewKit>>						ViewKitArray;
	typedef std::vector<TextKit, HPS::Allocator<TextKit>>						TextKitArray;
	typedef std::vector<ImageKit, HPS::Allocator<ImageKit>>						ImageKitArray;
	typedef std::vector<TableKit, HPS::Allocator<TableKit>>						TableKitArray;
	typedef std::vector<LinkKit, HPS::Allocator<LinkKit>>						LinkKitArray;
	typedef std::vector<ButtonKit, HPS::Allocator<ButtonKit>>					ButtonKitArray;
	typedef std::vector<CheckBoxKit, HPS::Allocator<CheckBoxKit>>				CheckBoxKitArray;
	typedef std::vector<RadioButtonKit, HPS::Allocator<RadioButtonKit>>			RadioButtonKitArray;
	typedef std::vector<ListBoxKit, HPS::Allocator<ListBoxKit>>					ListBoxKitArray;
	typedef std::vector<DropDownListKit, HPS::Allocator<DropDownListKit>>		DropDownListKitArray;
	typedef std::vector<SignatureFieldKit, HPS::Allocator<SignatureFieldKit>>	SignatureFieldKitArray;
	typedef std::vector<TextFieldKit, HPS::Allocator<TextFieldKit>>				TextFieldKitArray;
	typedef std::vector<SlideTableKit, HPS::Allocator<SlideTableKit>>			SlideTableKitArray;



	/*! The Publish::Source class is a concept class for source-string-related enums. */
	class PUBLISH_API Source
	{
	public:
		/*! \enum Type
		 *	Enumerates the types of source strings which can be passed to Publish. */
		enum class Type
		{
			Code,	//!< String contains source code, e.g., actual JavaScript or HTML code.
			File	//!< String contains a file name.
		};

	private:
		//! Private default constructor to prevent instantiation.
		Source();
	};

	typedef std::vector<Source::Type, HPS::Allocator<Source::Type> >		SourceTypeArray;

	/*! The Publish::Page class is a concept class for page-related enums. */
	class PUBLISH_API Page
	{
	public:
		/*! \enum Format
		 *	Enumerates the page formats to use when generating a PDF via Publish. */
		enum class Format
		{
			P11x17,		//!< Standard 11" x 17" format, 792 x 1124 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPage11x17</a>).
			A3,			//!< Standard A3 format, 842 x 1190 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPageA3</a>).
			A4,			//!< Standard A4 format, 595 x 842 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPageA4</a>).
			A5,			//!< Standard A5 format, 420 x 595 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPageA5</a>).
			B4JIS,		//!< Standard B4 JIS format, 728 x 1031 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPageB4JIS</a>).
			B5JIS,		//!< Standard B5 JIS format, 515 x 728 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPageB5JIS</a>).
			Executive,	//!< Standard Executive format, 522 x 756 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63ad8452bac68dc03824a11f42d79828f85">kA3DPDFPageExecutive</a>).
			Legal,		//!< Standard Legal format, 612 x 1008 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63a16dc81c7a1a43d531ba06c9ba1514716">kA3DPDFPageLegal</a>).
			Letter,		//!< Standard Letter format, 612 x 792 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63a74ec96bf6e295145e74f767b47e4d9ac">kA3DPDFPageLetter</a>).
			Tabloid,	//!< Standard Tabloid format, 792 x 1224 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63a2baa4572b23b0cd733cf7dc6ab52bf67">kA3DPDFPageTabloid</a>).
			B4ISO,		//!< Standard B4 ISO format, 709 x 1001 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63a90c47569ead41cb5e67af86bb0e2bba2">kA3DPDFPageB4ISO</a>).
			B5ISO		//!< Standard B5 ISO format, 499 x 709 points (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga25e335aa40067b53e07b74a7713a9c63af347da2f5d15a9ea71fe4d1e1c56a8e6">kA3DPDFPageB5ISO</a>).
		};

		/*! \enum Orientation
		 *	Enumerates the page orientations to use when generating a PDF via Publish. */
		enum class Orientation
		{
			Portrait,	//!< Self-explanatory (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggaf5a607cdeb86115ec80f667f8c5aca33a4e541a992c2fedb52d55c99fcf7a0a4a">kA3DPDFPagePortrait</a>).
			Landscape	//!< Self-explanatory (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggaf5a607cdeb86115ec80f667f8c5aca33a185c7b3a2cd68cc2e3b82d571515fafc">kA3DPDFPageLandscape</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Page();
	};

	/*! The Publish::Annotation class is a concept class for annotation-related enums. */
	class PUBLISH_API Annotation
	{
	public:
		/*!	\enum Format
		 *	Enumerates the format of the data to put in an annotation. */
		enum class Format
		{
			PRC,	//!< PRC data will be generated for the annotation.
			U3D		//!< U3D data will be generated for the annotation.
		};

		enum class SourceType
		{
			CADModel,			//!< The source was set using a CADModel object.
			KeyPath,			//!< The source was set using a KeyPath array.
			CADModelAndKeyPath	//!< The source was set using both a CADModel and a KeyPath array.
		};

	private:
		//! Private default constructor to prevent instantiation.
		Annotation();
	};

	/*! The Publish::PRC class is a concept class for PRC-related enums. */
	class PUBLISH_API PRC
	{
	public:
		/*! \enum BRepCompression
		 *	Enumerates the compression to apply to BRep data when exporting a PRC file. */
		enum class BRepCompression
		{
			None,		//!< No compression (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/exchange_ref_manual/struct_a3_d_r_w_params_export_prc_data.html#afbbe41721d6878768a624ea8d0894531">A3DRWParamsExportPrcData::m_bCompressBrep = false</a>).
			Low,		//!< Low compression, high accurracy (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/exchange_ref_manual/group__a3d__write.html#gga34066fd7757e486fd13543922ea12df8a2d7e4562fd9b672689989fd175e48689">kA3DCompressionLow</a>).
			Medium,		//!< Medium compression, medium accurracy (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/exchange_ref_manual/group__a3d__write.html#gga34066fd7757e486fd13543922ea12df8abef195d08d1adc08395ef6429a9d18a3">kA3DCompressionMedium</a>).
			High		//!< High compression, low accurracy (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/exchange_ref_manual/group__a3d__write.html#gga34066fd7757e486fd13543922ea12df8a005a7f0fc8cc13745541de441010b844">kA3DCompressionHigh</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		PRC();
	};

	/*! The Publish::U3D class is a concept class for U3D-related enums. */
	class PUBLISH_API U3D
	{
	public:
		/*!	\enum Version
		 	Enumerates the version to use when exporting a U3D file. */
		enum class Version
		{
			ECMA1,		//!< ECMA-363, version 1 (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/exchange_ref_manual/group__a3d__write.html#gga0e9211137820530ecc855326c34ede9da2c87e9374ce89144bbd3aa9d35fd8b53">kA3DECMA1</a>).
			ECMA3		//!< ECMA-363, version 3 (<a target="_blank" href="http://docs.techsoft3d.com/exchange/latest/exchange_ref_manual/group__a3d__write.html#gga0e9211137820530ecc855326c34ede9da6c4f24519bde58271432c798c78ff5af">kA3DECMA3</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		U3D();
	};

	/*! The Publish::Animation class is a concept class for animation-related enumerations. */
	class PUBLISH_API Animation
	{
	public:
		/*!	\enum Style
		 *	Enumerates the animation styles for an ArtworkKit. */
		enum class Style
		{
			None,	//!< Reader does not control the animation allowing it to be driven through other means, e.g., JavaScript (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga435820c6ba4cc0bb85216ccfd0ef2da8a539c3acca5674ae4f4dc740fae09b77d">kA3DPDFAnimStyleNoAnimation</a>).
			Loop,	//!< Reader loops through the animation continuously (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga435820c6ba4cc0bb85216ccfd0ef2da8a95bea60c14a580b006a85a6db135509b">kA3DPDFAnimStyleLoop</a>).
			Bounce	//!< Reader plays the animation forwards and backward continuously (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga435820c6ba4cc0bb85216ccfd0ef2da8a8cc0a1cf7f857d76deae53e0c683717c">kA3DPDFAnimStyleBounce</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Animation();
	};

	/*! The Publish::Activation class is a concept class for activation-related enumerations. */
	class PUBLISH_API Activation
	{
	public:
		/*! \enum When
		 *	Enumerates when an annotation is activated. */
		enum class When
		{
			Explicit,	//!< Annotation is not activated until a script or the user explicitly activates it, e.g., by clicking on a poster image (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga01c0130727cc2edd88f427a6414ef667a884b6d595ee956c28cc5da695f7e9591">kA3DPDFActivExplicitActivation</a>).
			Opened,		//!< Annotation is activated when the page containing it is opened (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga01c0130727cc2edd88f427a6414ef667aec9abb0a4ded19f427b7e79cbbfa2156">kA3DPDFActivPageOpened</a>).
			Visible		//!< Annotation is activated when the page containing it is visible (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga01c0130727cc2edd88f427a6414ef667a73f2794a0760420f36fcde5f08f81103">kA3DPDFActivPageVisible</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Activation();
	};

	/*! The Publish::Deactivation class is a concept class for deactivation-related enumeration. */
	class PUBLISH_API Deactivation
	{
	public:
		/*! \enum When
		 *	Enumerates when an annotation is deactivated. */
		enum class When
		{
			Explicit,	//!< Annotation is not deactivated until a script or the user explcitly deactivate it (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga12a051a9c6ee6515992318eebb748258a5b8fff14386e34bb6c8f2693005c5b68">kA3DPDFActivExplicitDesactivation</a>).
			Closed,		//!< Annotation is deactivated as soon as the page containing it is closed (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga12a051a9c6ee6515992318eebb748258a1a553f3f9a5b468b6eec5170b81e4199">kA3DPDFActivPageClosed</a>).
			NotVisible	//!< Annotation is deactivated as soon as the page containing it is no longer visible (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga12a051a9c6ee6515992318eebb748258ab7532605357c7360b28f357c91055c25">kA3DPDFActivPageNotVisible</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Deactivation();
	};

	/*! The Publish::Lighting class is a concept class for lighting-related enumerations. */
	class PUBLISH_API Lighting
	{
	public:
		/*! /enum Mode
		 *	Enumerates the lighting modes for ViewKit and AnnotationKit objects. */
		enum class Mode
		{
			Artwork,		//!< Inerhit the light from the annotation (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfad5c785c61f136f44d6e6513c352ff5ce">kA3DPDFLightArtworkCurrent</a>).
			None,			//!< No light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfae1927c8bff5aa627695bd8916dfda5bf">kA3DPDFLightNone</a>).
			White,			//!< White light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa8b6b6ab4b4e3cb86d1c6ba2319cecde9">kA3DPDFLightWhite</a>).
			Day,			//!< Day light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa0ddada5c4e633287b2e51c99d3554a3a">kA3DPDFLightDay</a>).
			Bright,			//!< Bright light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa7a69d46596bac227683d7013e83fa2c8">kA3DPDFLightBright</a>).
			PrimaryColor,	//!< Primary color light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfac9b3db174ba930b42a9ffda125b69ab4">kA3DPDFLightPrimaryColor</a>).
			Night,			//!< Night light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa8cfdadaffa83b66b1bb0a8f64e1cf937">kA3DPDFLightNight</a>).
			Blue,			//!< Blue light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa3619f77b8f8ff7a28e025604a755e578">kA3DPDFLightBlue</a>).
			Red,			//!< Red light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfafdee1db1fcf56fec167eb3a259915a25">kA3DPDFLightRed</a>).
			Cube,			//!< Cube light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa11e3b3c984658f952d6f932f97c6617c">kA3DPDFLightCube</a>).
			CADOptimized,	//!< CAD Optimized light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfa7de3e8c065fcae103964c459396fb4b8">kA3DPDFLightCADOptimized</a>).
			Headlamp		//!< Headlamp light (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#ggac8d3efa36af3abbd13a34d024058b8bfaf00c821f5a5f166664ce1c47264432b4">kA3DPDFLightHeadlamp</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Lighting();
	};

	/*! The Publish::Rendering class is a concept class for rendering-related enumerations. */
	class PUBLISH_API Rendering
	{
	public:
		/*! \enum Mode
		 *	Enumerates the rendering modes for ViewKit and AnnotationKit objects. */
		enum class Mode
		{
			Solid,							//!< Displays textured and lit faces (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957ab429bb60f1cb55ca45dae4af0c4042ab">kA3DPDFRenderingSolid</a>).
			SolidWireframe,					//!< Displays textured and lit faces and monochromatic tessellation edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a2356f362dd13cde3aa54be012fd0f0ad">kA3DPDFRenderingSolidWireframe</a>).
			SolidOutline,					//!< Displays textured and lit faces and monochromatic silhouette edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957abfac049e469262b0e65ca14dc0436149">kA3DPDFRenderingSolidOutline</a>).
			BoundingBox,					//!< Displays monochromatic edges of bounding boxes for each node (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957ab35a838667a5c4c5af10201e98019839">kA3DPDFRenderingBoundingBox</a>).
			Transparent,					//!< Displays textured and lit transparent faces (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a1d24cba69ea954520199349f04eb6dfb">kA3DPDFRenderingTransparent</a>).
			TransparentWireframe,			//!< Displays textured and lit transparent faces and monochromatic tessellation edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a2537d2c24e365883006cdbd651ef5ea5">kA3DPDFRenderingTransparentWireframe</a>).
			TransparentBoundingBox,			//!< Displays monochromatic transparent faces of bounding boxes for each node (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957af73a6b4edc4877602d81200301aa57fe">kA3DPDFRenderingTransparentBoundingBox</a>).
			TransparentBoundingBoxOutline,	//!< Displays monochromatic edges and transparent faces of bounding boxes for each node (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a0936fa550dd3747011e4a108cdfc61bb">kA3DPDFRenderingTransparentBoundingBoxOutline</a>).
			Illustration,					//!< Displays monochromatic unlit faces and silhouette edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957ab63806514d090e16b5afc963204231fb">kA3DPDFRenderingIllustration</a>).
			ShadedIllustration,				//!< Displays textured and lit faces with increased emissivity and monochromatic silhouette edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a8df37091cef9be5929209a99c691efc2">kA3DPDFRenderingShadedIllustration</a>).
			Wireframe,						//!< Displays monochromatic tessellation edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a3bc65ddc59108fd4d979ce830b2f2bbf">kA3DPDFRenderingWireframe</a>).
			ShadedWireframe,				//!< Displays color interpolated and lit tessellation edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a5a7d09ffbeeb16532340e615553f7dbb">kA3DPDFRenderingShadedWireframe</a>).
			HiddenWireframe,				//!< Displays monochromatic faces and tessellation edges (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a6debf020de48bfef77979f6e75e643b9">kA3DPDFRenderingHiddenWireframe</a>).
			Vertices,						//!< Displays monochromatic vertices (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a7a59f96d08435a6b076bf589b801da86">kA3DPDFRenderingVertices</a>).
			ShadedVertices					//!< Displays colored and lit vertices (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dannot__module.html#gga5caefd555ca862b895072467e243e957a0abb939986cf861d64e68635a0f3cd7a">kA3DPDFRenderingShadedVertices</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Rendering();
	};

	/*! The Publish::Text class is a concept class for text-related enumerations. */
	class PUBLISH_API Text
	{
	public:
		/*! The Publish::Text::Font class is a concept class for text-font-related enumerations. */
		class PUBLISH_API Font
		{
		public:
			/*! \enum Type
			 * 	Enumerates the types of fonts to use for text strings. */
			enum class Type
			{
				BuiltIn,	//!< Use one of the enumerated fonts.
				Explicit	//!< Use an explicitly specified font.
			};

			/*! \enum Name
			 *	Enumerates the built-in fonts available for text strings. */
			enum class Name
			{
				TimesRoman,				//!< Standard Times Roman font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714a21bbf01f195c77e536c1f33c9ab1cd01">kA3DPDFFontTimesRoman</a>).
				TimesItalic,			//!< Italic Times Roman font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714ab85900d6a627331288fbd63cc1185b32">kA3DPDFFontTimesItalic</a>).
				TimesBold,				//!< Bold Times Roman font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714ac0e133c9cbe370cb05182e1ed3535791">kA3DPDFFontTimesBold</a>).
				TimesBoldItalic,		//!< Bold Italic Times Roman font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714a7946740ab05014c54cd4c93262d33b77">kA3DPDFFontTimesBoldItalic</a>).
				Helvetica,				//!< Standard Helvetica font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714aa57ace3ab1e09b88d1243b4c4a3aaffc">kA3DPDFFontHelvetica</a>).
				HelveticaOblique,		//!< Oblique Helvetica font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714a13e9ff214043342654656e47860e05b8">kA3DPDFFontHelveticaOblique</a>).
				HelveticaBold,			//!< Bold Helvetica font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714a78f41039d3bde26e7be565bf62f10101">kA3DPDFFontHelveticaBold</a>).
				HelveticaBoldOblique,	//!< Bold Oblique Helvetica font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714a60e56bb3646aa07da037558d4f28eb48">kA3DPDFFontHelveticaBoldOblique</a>).
				Courier,				//!< Standard Courier font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714ac9f020faaee3207378b26621843379a4">kA3DPDFFontCourier</a>).
				CourierOblique,			//!< Oblique Courier font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714abb9a872f785edafe6f60dabe32b3a3af">kA3DPDFFontCourierOblique</a>).
				CourierBold,			//!< Bold Courier font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714a112ee573ada9c5047582116e72526071">kA3DPDFFontCourierBold</a>).
				CourierBoldOblique		//!< Bold Oblique Courier font (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__text__module.html#gga12674029e90312cf9f2f5e6c9335d714aa48d0b687e3a41538c24845834b7a9c7">kA3DPDFFontCourierBoldOblique</a>).
			};

			/*!	\enum Style
			 *	Enumerates the font styles to use for text strings. */
			enum class Style
			{
				Regular,		//!< Regular font style.
				Bold,			//!< Bold font style.
				Italic,			//!< Italic font style.
				BoldItalic,		//!< Bold and italic font style.
			};

		private:
			//! Private default constructor to prevent instantiation.
			Font();
		};

		/*! \enum Language
		 *	Enumerates the supported languages for explicitly-specified fonts. */
		enum class Language
		{
			ASCII,					//!< Self-explanatory.
			EastEuropeanRoman,		//!< Self-explanatory.
			Cyrillic,				//!< Self-explanatory.
			Greek,					//!< Self-explanatory.
			Turkish,				//!< Self-explanatory.
			Hebrew,					//!< Self-explanatory.
			Arabic,					//!< Self-explanatory.
			Baltic,					//!< Self-explanatory.
			ChineseTraditional,		//!< Self-explanatory.
			ChineseSimplified,		//!< Self-explanatory.
			Japanese,				//!< Self-explanatory.
			Korean					//!< Self-explanatory.
		};

		/*!	\enum Rotation
		 *	Enumerates the rotation to use for text strings. */
		enum class Rotation
		{
			None,							//!< No rotation, text will be horizontal (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggaecd5562467e55481e319fa901d1cb65eab924d8c16c2894b5d5c6dbe9378803b6">kA3DPDFNormal</a>).
			CounterClockwise90Degrees,		//!< Text will be rotated 90 degrees counter-clockwise from the horizontal (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggaecd5562467e55481e319fa901d1cb65ea28b6ecebefe6af9c9e0751031b5a3e67">kA3DPDF90</a>).
			CounterClockwise180Degrees,		//!< Text will be rotated 180 degrees counter-clockwise from the horizontal (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggaecd5562467e55481e319fa901d1cb65ea8db4b54045ff4cf62741fc6ff35efb4b">kA3DPDF180</a>).
			CounterClockwise270Degrees,		//!< Text will be rotated 270 degrees counter-clockwise from the horizontal (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggaecd5562467e55481e319fa901d1cb65ea6595a146630cb905d24a77aa66c9d264">kA3DPDF270</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Text();
	};

	/*! The Publish::Image class is a concept class for image-related enumerations. */
	class PUBLISH_API Image
	{
	public:
		/*!	\enum Format
		 *	Enumerates the image formats that Publish can insert into a PDF. */
		enum class Format
		{
			Unknown,	//!< Unknown format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a2a6d182c4a53640e42b7719172a49ec8">kA3DPDFImageFormatUnknown</a>).
			BMP,		//!< Bitmap format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a4091db8534a312548c202ac3f219e55f">kA3DPDFImageFormatBmp</a>).
			PNG,		//!< PNG format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a90a93415584edf3dd8e869f6554e4721">kA3DPDFImageFormatPng</a>).
			JPEG,		//!< JPEG format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a9d3182571a2cd1c17d0e14dd9eaeb212">kA3DPDFImageFormatJpg</a>).
			RawRGB,		//!< RGB format, 3 bytes per pixel (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274aacc06d17c87fdc567e5fefeff8465bd9">kA3DPDFImageFormatBitmapRgbByte</a>).
			RawRGBA,	//!< RGBA format, 4 bytes per pixel (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274aa5077512c37411f950ab2ae6459821e4">kA3DPDFImageFormatBitmapRgbaByte</a>).
			RawGrey,	//!< Greyscale format, 1 byte per pixel (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274ab93b8c5e7b1928f0072be5f7277d0d69">kA3DPDFImageFormatBitmapGreyByte</a>).
			RawGreyA,	//!< Greyscale + alpha format, 2 bytes per pixel (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a5a4bfc8a2cc971fedd6b2a13d5323238">kA3DPDFImageFormatBitmapGreyaByte</a>).
			EMF,		//!< EMF format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274ac6480d635ba7f4b3b56baa3ab60fa755">kA3DPDFImageFormatEmf</a>).
			URL,		//!< URL format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a87d84076f2863b1b038791c86d6514fc">kA3DPDFImageFormatUrl</a>).
			GIF,		//!< GIF format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274aee3f2e9d7227b48fdc7e2908cc42cc2d">kA3DPDFImageFormatGif</a>).
			TIFF,		//!< TIFF format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274ac85187f900c7b7cd21865551472f714e">kA3DPDFImageFormatTif</a>).
			PCX,		//!< PCX format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a0ac7eefbe03075b17d526a1d0452b790">kA3DPDFImageFormatPcx</a>).
			Targa,		//!< Targa format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a2a12802abe8b9ac8914be49c7934d8c2">kA3DPDFImageFormatTga</a>).
			PPM,		//!< PPM format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a3fa7a0f649acfdb242e9f38560b3a867">kA3DPDFImageFormatPpm</a>).
			ILBM,		//!< ILBM format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274ad0ce672646a5e2bea2d20f813b0066dc">kA3DPDFImageFormatIlbm</a>).
			CEL,		//!< Cel format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274ac738607a295581804cf0c8bf575dd327">kA3DPDFImageFormatCel</a>).
			RGB,		//!< RGB format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a58a4746565da7a7a77e82c8fb968eb0e">kA3DPDFImageFormatRgb</a>).
			PSD,		//!< Photoshop PSD format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a27b40bc4e46d1f5003912be60e09d1ac">kA3DPDFImageFormatPsd</a>).
			PIC			//!< Softimage PIC format (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__pdf__image__module.html#ggac6b35fd71ab095070868b523f9842274a57f901a12c35c024cb9fd61dabcd9f0e">kA3DPDFImageFormatSoftimagepic</a>).
		};

	private:
		//! Private default constructor to prevent instantiation.
		Image();
	};

	/*! The Publish::Field class is a concept class for field-related enumerations. */
	class PUBLISH_API Field
	{
	public:
		/*!	\enum Type
		 *	Enumerates the field types that Publish can identify in a PDF. */
		enum class Type
		{
			Text,				//!< Text field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga5e1c7fe688d1b2f34a92f47e08ec7332">kA3DPDFText</a>)
			Button,				//!< Button field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga5e1c7fe688d1b2f34a92f47e08ec7332">kA3DPDFButton</a>)
			DropDown,			//!< Drop-down field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga5e1c7fe688d1b2f34a92f47e08ec7332">kA3DPDFDropDown</a>)
			ListBox,			//!< List box field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga5e1c7fe688d1b2f34a92f47e08ec7332">kA3DPDFListBox</a>)
			CheckBox,			//!< Check box field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga5e1c7fe688d1b2f34a92f47e08ec7332ad27ce4be4b793f5684eef9eeb9ea5360">kA3DPDFCheckBox</a>)
			RadioButton,		//!< Radio button field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga5e1c7fe688d1b2f34a92f47e08ec7332a24460a6b3838656a9fe313274dce742a">kA3DPDFRadioButton</a>)
			DigitalSignature,	//!< Digital signature field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga5e1c7fe688d1b2f34a92f47e08ec7332a9f2a9d68d1aec229ebd72ac4f793673b">kA3DPDFDigitalSignature</a>)
			BarCode				//!< Bar code field (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gga5e1c7fe688d1b2f34a92f47e08ec7332af1002cf957db0a03a2503ba2bdb5223e">kA3DPDFBarCode</a>)
		};

	private:
		//! Private default constructor to prevent instantiation.
		Field();
	};

	typedef std::vector<Field::Type, Allocator<Field::Type>>	FieldTypeArray;

	/*! The Publish::Highlighting class is a concept class for highlighting-related enumerations. */
	class PUBLISH_API Highlighting
	{
	public:
		/*! \enum Mode
		 *	Enumerates the highlighting modes for LinkKit objects. */
		enum class Mode
		{
			None,		//!< No highlighting (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__link__module.html#ggac1e9c538a7903cb14419c9fbcc5462e8a3d39c3c5875e86ea0cc158b1040615c1">kA3DPDFLinkHighlightNone</a>)
			Invert,		//!< Invert the colors in the content area (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__link__module.html#ggac1e9c538a7903cb14419c9fbcc5462e8a35dc0e01805bc7551225cc17a6dab3a4">kA3DPDFLinkHighlightInvert</a>)
			Outline,	//!< Invert the color of the outline of the content area (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__link__module.html#ggac1e9c538a7903cb14419c9fbcc5462e8a4e934e14f3d3e590455aca95da43972f">kA3DPDFLinkHighlightOutline</a>)
			Push		//!< Content area will appear to depress into the page (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__link__module.html#ggac1e9c538a7903cb14419c9fbcc5462e8af0e3b7f74abce906f785caa5f3382985">kA3DPDFLinkHighlightPush</a>)
		};

	private:
		//! Private default constructor to prevent instantiation.
		Highlighting();
	};

	/*! The Publish::Artwork class is a concept class for artwork-related enumerations. */
	class PUBLISH_API Artwork
	{
	public:
		/*! The Publish::Artwork::View class is a concept class for view-related artwork enumerations. */
		class PUBLISH_API View
		{
		public:
			/*! \enum Preference
			 *	Enumerates the preferences for default views in ArtworkKit objects. */
			enum class Preference
			{
				Native,		//!< Prefer the native default view in the PRC data (if one is present).
				PDF			//!< Prefer the default view defined in the PDF (if one is present).
			};

		private:
			//! Private default constructor to prevent instantiation.
			View();
		};

		/*! The Publish::Artwork::Display class is a concept class for display-related artwork enumerations. */
		class PUBLISH_API Display
		{
		public:
			/*! \enum Preference
			 *	Enumerates the preferences for the display of views in ArtworkKit objects. */
			enum class Preference
			{
				Defined,	//!< Prefer the display settings (rendering mode, lighting mode, and background color) set in a view when activating it.
				Runtime		//!< Prefer the runtime display settings (rendering mode, lighting mode, and background color) in the application when activating a view (except for the default view).
			};

		private:
			//! Private default constructor to prevent instantiation.
			Display();
		};

	private:
		//! Private default constructor to prevent instantiation.
		Artwork();
	};

	/*! The Publish::Border class is a concept class for border-related enumerations. */
	class PUBLISH_API Border
	{
	public:
		/*!	\enum Thickness
		 *	Enumerates the thicknesses for borders. */
		enum class Thickness
		{
			Thin,		//!< Thin border (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggad6067e024ccdc132a12822b2c252d824a840f59006c5efbd1eb92b3ddd0bba7bd">kA3DPDFThin</a>)
			Medium,		//!< Medium border (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggad6067e024ccdc132a12822b2c252d824aac6e333e72b0a1e34d2ed4a0d2fa5a74">kA3DPDFMedium</a>)
			Wide,		//!< Wide border (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ggad6067e024ccdc132a12822b2c252d824a0ad0557760c11b0dd5951ee91d082b07">kA3DPDFThick</a>)
		};

		/*!	\enum Style
		 *	Enumerates the border styles. */
		enum class Style
		{
			Solid,			//!< Solid (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga42f02d2d4f6546012a510a8eb7fb8c3e">kA3DPDFSolid</a>)
			Dashed,			//!< Dashed (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga42f02d2d4f6546012a510a8eb7fb8c3e">kA3DPDFDashed</a>)
			Beveled,		//!< Beveled (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga42f02d2d4f6546012a510a8eb7fb8c3e">kA3DPDFBeveled</a>)
			Inset,			//!< Inset (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga42f02d2d4f6546012a510a8eb7fb8c3e">kA3DPDFInset</a>)
			Underlined,		//!< Underlined (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga42f02d2d4f6546012a510a8eb7fb8c3e">kA3DPDFUnderlined</a>)
		};

	private:
		//! Private default constructor to prevent instantiation.
		Border();
	};

	/*! The Publish::Label class is a concept class for label-related enumerations. */
	class PUBLISH_API Label
	{
	public:
		/*!	\enum Position
		 *	Enumerates the label positions. */
		enum class Position
		{
			LabelOnly,		//!< Only the label will be displayed (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFLabelOnly</a>)
			IconOnly,		//!< Only the icon will be displayed (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFIconOnly</a>)
			Top,			//!< The label will be displayed above the icon (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFLabelTopIconBottom</a>)
			Bottom,			//!< The label will be displayed below the icon (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFIconTopLabelBottom</a>)
			Left,			//!< The label will be displayed to the left of the icon (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFLabelLeftIconRight</a>)
			Right,			//!< The label will be displayed to the right of the icon (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFIconTopLabelBottom</a>)
			OnTop,			//!< The label will be displayed on top of the icon (<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1ecc1b1bda7680246dae62ecefa65a3d">kA3DPDFLabelOverIcon</a>)
		};

	private:
		//! Private default constructor to prevent instantiation.
		Label();
	};


	/*! The Publish::Permission class is a concept class for permission-related enumerations. */
	class PUBLISH_API Permission
	{
	public:
		enum class Type
		{
			Open,				//!< User can open and decrypt the document.
			Secure,				//!< User can change the document security settings.
			Print,				//!< User can print document.
			Edit,				//!< User can add or modify text notes.
			Copy,				//!< User can copy information from the document.
			EditNotes,			//!< User can add, modify and delete text notes.
			SaveAs,				//!< User can perform a 'Save As...' operation.
			FillAndSign,		//!< User can fill in or sign existing forms or signature fields.
			Accessible,			//!< The Accessibility API is enabled.
			Assembly,			//!< User can insert, delete and rotate pages, create bookmarks and thumbnails.
			HighQualityPrint,	//!< User can produce high quality prints.
			Owner,				//!< All operations are permitted.
			FormSubmit,			//!< Allows the user to submit forms outside the browser.
			SpawnTemplate,		//!< User can spawn template pages.
			User,				//!< All permissions are active, except for Open and Secure
			All,				//!< All permissions are active
		};

	private:
		//! Private default constructor to prevent instantiation.
		Permission();
	};

	typedef std::vector<Permission::Type, HPS::Allocator<Permission::Type> >		PermissionTypeArray;


	/*! The DocumentKit class is a user space object.  It acts as the root container for all pages in a Publish PDF. */
	class PUBLISH_API DocumentKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty DocumentKit object. */
		DocumentKit();

		/*! The copy constructor creates a new DocumentKit object that contains the same settings as the source DocumentKit.
		 * 	\param in_kit The source DocumentKit to copy. */
		DocumentKit(DocumentKit const & in_kit);

		/*! The move constructor creates a DocumentKit by transferring the underlying object of the rvalue reference to this DocumentKit.
		 * 	\param in_kit An rvalue reference to a DocumentKit to take the underlying object from. */
		DocumentKit(DocumentKit && in_kit);

		virtual ~DocumentKit();

	static const HPS::Type staticType = HPS::Type::PublishDocumentKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a DocumentKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for export unless a setting is overridden by the DocumentKit passed to File::Export.
		 *	\return A DocumentKit with the default settings. */
		static DocumentKit	GetDefault();

		/*! Copies the source DocumentKit into this DocumentKit.
		 * 	\param in_kit The source DocumentKit to copy. */
		void				Set(DocumentKit const & in_kit);

		/*! Copies this DocumentKit into the given DocumentKit.
		 * 	\param out_kit The DocumentKit to populate with the contents of this DocumentKit. */
		void				Show(DocumentKit & out_kit) const;

		/*! Copies the source DocumentKit into this DocumentKit.
		 * 	\param in_kit The source DocumentKit to copy.
		 * 	\return A reference to this DocumentKit. */
		DocumentKit &		operator=(DocumentKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this DocumentKit.
		 *	\param in_kit An rvalue reference to a DocumentKit to take the underlying object from.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		operator=(DocumentKit && in_kit);

		/*! Indicates whether this DocumentKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this DocumentKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source DocumentKit is equivalent to this DocumentKit. 
		 *	\param in_kit The source DocumentKit to compare to this DocumentKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(DocumentKit const & in_kit) const;

		/*!	Check if the source DocumentKit is equivalent to this DocumentKit. 
		 *	\param in_kit The source DocumentKit to compare to this DocumentKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(DocumentKit const & in_kit) const;

		/*!	Check if the source DocumentKit is not equivalent to this DocumentKit. 
		 *	\param in_kit The source DocumentKit to compare to this DocumentKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(DocumentKit const & in_kit) const;


		/*! Sets the title, author, subject and creator for this document.  These corresponds to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a23a95fae20138d1c2b41e72d1e023f65">A3DPDFDocumentInformationData::m_pcTitle</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a80251b9b9e2a5943f44841cb9c996c36">A3DPDFDocumentInformationData::m_pcAuthor</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a7a2428427055c10dd5b5b625e1a3ddf1">A3DPDFDocumentInformationData::m_pcSubject</a>,
		 *	and <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a233e35e235385a13f5f26153223aed1a">A3DPDFDocumentInformationData::m_pcCreator</a> (respectively).
		 *	\param in_title UTF8-encoded title string to set on the document.
		 *	\param in_author UTF8-encoded author string to set on the document.
		 *	\param in_subject UTF8-encoded subject string to set on the document.
		 *	\param in_creator UTF8-encoded creator string to set on the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetInformation(char const * in_title, char const * in_author, char const * in_subject, char const * in_creator);

		/*! Sets the passwords for this document.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3caa56b7a947ac2b42f2e9c66ede5aa4">A3DPDFDocumentSetPassword</a>.
		 *	\param in_user_password UTF8-encoded password string required when opening, modifying or printing the document.  If an empty string is specified, no password will be required.
		 *	\param in_owner_password UTF8-encoded password string required when changing security features of document.  If an empty string is specified, no password will be required.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetPasswords(char const * in_user_password, char const * in_owner_password);

		/*! Adds a named JavaScript source or file to the document.  If there is an existing script on the document with the given name, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga4e6020563c56827aa43faf9566f043df">A3DPDFDocumentAddJavascriptFromString</a>.
		 *	\param in_script_name UTF8-encoded script name to be added to the document.
		 *	\param in_source UTF8-encoded JavaScript source or file (depending on the following argument) to be added to the document.
		 *	\param in_type The types of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddJavaScript(char const * in_script_name, char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Adds a list of named JavaScript sources or files to the document.  If there is an existing script on the document with a name which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga4e6020563c56827aa43faf9566f043df">A3DPDFDocumentAddJavascriptFromString</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_script_names Script names to be added to the document.
		 *	\param in_sources JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be added to the document.
		 *	\param in_types Types for each item in the preceding argument array.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddJavaScript(size_t in_count, UTF8 const in_script_names[], UTF8 const in_sources[], Source::Type const in_types[]);

		/*! Adds a list of named JavaScript sources or files to the document.  If there is an existing script on the document with a name which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga4e6020563c56827aa43faf9566f043df">A3DPDFDocumentAddJavascriptFromString</a>.
		 *	\param in_script_names Script names to be added to the document.
		 *	\param in_sources JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be added to the document.
		 *	\param in_types Types for each item in the preceding argument array.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddJavaScript(UTF8Array const & in_script_names, UTF8Array const & in_sources, SourceTypeArray const & in_types);

		/*! Adds an attachment to the document.  If there is an existing attachment on the document with the given filename, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3f4bcf1bf99759ba44543b5ab7462b99">A3DPDFDocumentAddFileAttachment</a>.
		 *	\param in_filename UTF8-encoded filename of the file to be attached.
		 *	\param in_description UTF8-encoded description of the file to be attached.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddAttachment(char const * in_filename, char const * in_description);

		/*! Adds a list of attachments to the document.  If there is an existing attachment on the document with a filename which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3f4bcf1bf99759ba44543b5ab7462b99">A3DPDFDocumentAddFileAttachment</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_filenames Filenames of the files to be attached.
		 *	\param in_descriptions Descriptions of the files to be attached.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddAttachments(size_t in_count, UTF8 const in_filenames[], UTF8 const in_descriptions[]);

		/*! Adds a list of attachments to the document.  If there is an existing attachment on the document with a filename which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3f4bcf1bf99759ba44543b5ab7462b99">A3DPDFDocumentAddFileAttachment</a>.
		 *	\param in_filenames Filenames of the files to be attached.
		 *	\param in_descriptions Descriptions of the files to be attached.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddAttachments(UTF8Array const & in_filenames, UTF8Array const & in_descriptions);

		/*! Adds an icon image to the document.  If there is an existing icon image on the document with the given name, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga7acb308303e1a5f8092e2fa368999048">A3DPDFDocumentAddImageAsIcon</a>.
		 *	\param in_name UTF8-encoded name of the icon image to be attached.
		 *	\param in_image Image to be attached as an icon.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddIconImage(char const * in_name, ImageKit const & in_image);

		/*! Adds icon images to the document.  If there is an existing icon image on the document with a name which is in the given list, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga7acb308303e1a5f8092e2fa368999048">A3DPDFDocumentAddImageAsIcon</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_names Names of the icon images to be attached.
		 *	\param in_images Images to be attached as icons.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddIconImages(size_t in_count, UTF8 const in_names[], ImageKit const in_images[]);

		/*! Adds icon images to the document.  If there is an existing icon image on the document with a name which is in the given list, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga7acb308303e1a5f8092e2fa368999048">A3DPDFDocumentAddImageAsIcon</a>.
		 *	\param in_names Names of the icon images to be attached.
		 *	\param in_images Images to be attached as icons.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddIconImages(UTF8Array const & in_names, ImageKitArray const & in_images);

		/*! Adds the given page to the document.  This will replace all pages (if any) which are already on the document.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_page Page to add to the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetPage(PageKit const & in_page);

		/*! Adds the given page to the document.  This will append the page after any existing pages on the document.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_page Page to add to the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddPage(PageKit const & in_page);

		/*! Adds the given pages to the document.  This will replace all pages (if any) which are already on the document.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_pages Pages to add to the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetPages(size_t in_count, PageKit const in_pages[]);

		/*! Adds the given pages to the document.  This will replace all pages (if any) which are already on the document.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_pages Pages to add to the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetPages(PageKitArray const & in_pages);

		/*! Adds the given pages to the document.  This will append the pages after any existing pages on the document.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_pages Pages to add to the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddPages(size_t in_count, PageKit const in_pages[]);

		/*! Adds the given pages to the document.  This will append the pages after any existing pages on the document.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_pages Pages to add to the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		AddPages(PageKitArray const & in_pages);

		/*! Sets document permissions. This correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaa3cd3a52f56a9f817801f8b021afa64a">A3DPDFDocumentSetDocumentPermissions</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_permissions The permissions to be set on the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetPermissions(size_t in_count, Permission::Type const in_permissions[]);

		/*! Sets document permissions. This correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaa3cd3a52f56a9f817801f8b021afa64a">A3DPDFDocumentSetDocumentPermissions</a>.
		 *	\param in_permissions The permissions to be set on the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		SetPermissions(PermissionTypeArray const & in_permissions);


		/*! Removes the document title, author, subject and creator.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetInformation();

		/*! Removes the document passwords.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetPasswords();

		/*! Removes the script with a given name from the document.
		 *	\param in_script_name UTF8-encoded script name to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetJavaScript(char const * in_script_name);

		/*! Removes the scripts with the given names from the document.
		 *	\param in_count Size of the following array.
		 *	\param in_script_names Script names to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetJavaScript(size_t in_count, UTF8 const in_script_names[]);

		/*! Removes the scripts with the given names from the document.
		 *	\param in_script_names Script names to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetJavaScript(UTF8Array const & in_script_names);

		/*! Removes all scripts from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetJavaScript();

		/*! Removes the attachment with a given filename from the document.
		 *	\param in_filename UTF8-encoded filename of attachment to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetAttachment(char const * in_filename);

		/*! Removes the attachments with the given filenames from the document.
		 *	\param in_count Size of the following array.
		 *	\param in_filenames Filenames of attachments to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetAttachments(size_t in_count, UTF8 const in_filenames[]);

		/*! Removes the attachments with the given filenames from the document.
		 *	\param in_filenames Filenames of attachments to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetAttachments(UTF8Array const & in_filenames);

		/*! Removes all attachments from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetAttachments();

		/*! Removes the icon image with a given name from the document.
		 *	\param in_name UTF8-encoded name of icon image to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetIconImage(char const * in_name);

		/*! Removes the icon images with the given names from the document.
		 *	\param in_count Size of the following array.
		 *	\param in_names Names of icon images to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetIconImages(size_t in_count, UTF8 const in_names[]);

		/*! Removes the icon images with the given names from the document.
		 *	\param in_names Names of icon images to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetIconImages(UTF8Array const & in_names);

		/*! Removes all icon images from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetIconImages();

		/*! Removes the page at the given index from the document.
		 *	\param in_index Index of the page to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetPage(size_t in_index);

		/*! Removes the pages at the given indices from the document.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the pages to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetPages(size_t in_count, size_t const in_indices[]);

		/*! Removes the pages at the given indices from the document.
		 *	\param in_indices Indices of the pages to remove from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetPages(SizeTArray const & in_indices);

		/*! Removes all pages from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetPages();

		/*! Removes permission settings from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetPermissions();

		/*! Removes all data from the document.
		 *	\return A reference to this DocumentKit. */
		DocumentKit &		UnsetEverything();


		/*! Shows the document title, author, subject and creator.
		 *	\param out_title Title of the document
		 *	\param out_author Author of the document
		 *	\param out_subject Subject of the document
		 *	\param out_creator Creator of the document
		 *	\return <span class='code'>true</span> if a title, author, subject and creator were specified, <span class='code'>false</span> otherwise. */
		bool				ShowInformation(UTF8 & out_title, UTF8 & out_author, UTF8 & out_subject, UTF8 & out_creator) const;

		/*! Shows the document passwords.
		 *	\param out_user_password User password of the document.
		 *	\param out_owner_password Owner password of the document.
		 *	\return <span class='code'>true</span> if passwords were specified, <span class='code'>false</span> otherwise. */
		bool				ShowPasswords(UTF8 & out_user_password, UTF8 & out_owner_password) const;

		/*! Shows the JavaScript sources and/or files on the document.
		 *	\param out_script_names Script names on the document.
		 *	\param out_sources JavaScript sources and/or files on the document.
		 *	\param out_types Types for each item in the preceding array.
		 *	\return True if this setting was specified, otherwise false.
		 *	\return <span class='code'>true</span> if scripts were specified, <span class='code'>false</span> otherwise. */
		bool				ShowJavaScript(UTF8Array & out_script_names, UTF8Array & out_sources, SourceTypeArray & out_types) const;

		/*! Shows the attachments on the document.
		 *	\param out_filenames Filenames of the attachments on the document.
		 *	\param out_descriptions Descriptions of the attachments on the document.
		 *	\return <span class='code'>true</span> if attachments were specified, <span class='code'>false</span> otherwise. */
		bool				ShowAttachments(UTF8Array & out_filenames, UTF8Array & out_descriptions) const;

		/*! Shows the icon images on the document.
		 *	\param out_names Names of the icon images on the document.
		 *	\param out_images Icon images on the document.
		 *	\return <span class='code'>true</span> if icon images were specified, <span class='code'>false</span> otherwise. */
		bool				ShowIconImages(UTF8Array & out_names, ImageKitArray & out_images) const;

		/*! Shows the pages on the document.
		 *	\param out_pages Pages on the document.
		 *	\return <span class='code'>true</span> if pages were specified, <span class='code'>false</span> otherwise. */
		bool				ShowPages(PageKitArray & out_pages) const;

		/*! Shows permissions on the document
		 *	\param out_permissions Permissions on the document.
		 *	\return <span class='code'>true</span> if permissions were specified, <span class='code'>false</span> otherwise. */
		bool				ShowPermissions(PermissionTypeArray & out_permissions) const;
	};

	/*! The PageKit class is a user space object.  It acts as the container for all data that can be on a page in a Publish PDF. */
	class PUBLISH_API PageKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty PageKit object. */
		PageKit();

		/*! This constructor creates a PageKit with the given format and orientation.
		 *	\param in_format Format of the page.
		 *	\param in_orientation Orientation of the page. */
		PageKit(Page::Format in_format, Page::Orientation in_orientation);

		/*! This constructor creates a PageKit from the given PDF template file.
		 *	\param in_filename UTF8-encoded filename for the PDF file to act as a template. */
		PageKit(char const * in_filename);

		/*! The copy constructor creates a new PageKit object that contains the same settings as the source PageKit.
		 * 	\param in_kit The source PageKit to copy. */
		PageKit(PageKit const & in_kit);

		/*! The move constructor creates a PageKit by transferring the underlying object of the rvalue reference to this PageKit.
		 * 	\param in_kit An rvalue reference to a PageKit to take the underlying object from. */
		PageKit(PageKit && in_kit);

		virtual ~PageKit();

	static const HPS::Type staticType = HPS::Type::PublishPageKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a PageKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for export unless a setting is overridden by the PageKit passed to File::Export.
		 *	\return A PageKit with the default settings. */
		static PageKit		GetDefault();

		/*! Copies the source PageKit into this PageKit.
		 * 	\param in_kit The source PageKit to copy. */
		void				Set(PageKit const & in_kit);

		/*! Copies this PageKit into the given PageKit.
		 * 	\param out_kit The PageKit to populate with the contents of this PageKit. */
		void				Show(PageKit & out_kit) const;

		/*! Copies the source PageKit into this PageKit.
		 * 	\param in_kit The source PageKit to copy.
		 * 	\return A reference to this PageKit. */
		PageKit &			operator=(PageKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this PageKit.
		 *	\param in_kit An rvalue reference to a PageKit to take the underlying object from.
		 *	\return A reference to this PageKit. */
		PageKit &			operator=(PageKit && in_kit);

		/*! Indicates whether this PageKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this PageKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source PageKit is equivalent to this PageKit. 
		 *	\param in_kit The source PageKit to compare to this PageKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(PageKit const & in_kit) const;

		/*!	Check if the source PageKit is equivalent to this PageKit. 
		 *	\param in_kit The source PageKit to compare to this PageKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(PageKit const & in_kit) const;

		/*!	Check if the source PageKit is not equivalent to this PageKit. 
		 *	\param in_kit The source PageKit to compare to this PageKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(PageKit const & in_kit) const;


		/*! Sets the format of the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_page_data.html#a365806d4aaa8d9fd2c24ab1335dbc549">A3DPDFPageData::m_ePageSize</a>.
		 *	This has no effect if this page has a template file.
		 *	\param in_format Format of the page.
		 *	\return A reference to this PageKit. */
		PageKit &			SetFormat(Page::Format in_format);

		/*! Sets the orientation of the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_page_data.html#a5df6f02cb70127783ed0b1d5869bc99d">A3DPDFPageData::m_ePageOrientation</a>.
		 *	This has no effect if this page has a template file.
		 *	\param in_orientation Orientation of the page.
		 *	\return A reference to this PageKit. */
		PageKit &			SetOrientation(Page::Orientation in_orientation);

		/*! Adds an annotation to the page.  This will replace all annotations (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_annotation Annotation to add to the page.
		 *	\param in_location Annotation location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetAnnotation(AnnotationKit const & in_annotation, IntRectangle const & in_location);

		/*! Adds an annotation to the page.  This will not replace any existing annotations on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_annotation Annotation to add to the page.
		 *	\param in_location Annotation location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddAnnotation(AnnotationKit const & in_annotation, IntRectangle const & in_location);

		/*! Adds a list of annotations to the page.  This will replace all annotations (if any) on the page.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_annotations Annotations to add to the page.
		 *	\param in_locations Annotation locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetAnnotations(size_t in_count, AnnotationKit const in_annotations[], IntRectangle const in_locations[]);

		/*! Adds a list of annotations to the page.  This will replace all annotations (if any) on the page.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_annotations Annotations to add to the page.
		 *	\param in_locations Annotation locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetAnnotations(AnnotationKitArray const & in_annotations, IntRectangleArray const & in_locations);

		/*! Adds a list of annotations to the page.  This will not replace any existing annotations on the page.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_annotations Annotations to add to the page.
		 *	\param in_locations Annotation locations in points relative to bottom left of the page.
		 *	\return A reference to this PageKit. */
		PageKit &			AddAnnotations(size_t in_count, AnnotationKit const in_annotations[], IntRectangle const in_locations[]);

		/*! Adds a list of annotations to the page.  This will not replace any existing annotations on the page.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_annotations Annotations to add to the page.
		 *	\param in_locations Annotation locations in points relative to bottom left of the page.
		 *	\return A reference to this PageKit. */
		PageKit &			AddAnnotations(AnnotationKitArray const & in_annotations, IntRectangleArray const & in_locations);

		/*! Adds a text string to the page.  This will replace all text (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_text Text to add to the page.
		 *	\param in_location Text location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetText(TextKit const & in_text, IntRectangle const & in_location);

		/*! Adds a text string to the page.  This will not replace any text on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_text Text to add to the page.
		 *	\param in_location Text location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddText(TextKit const & in_text, IntRectangle const & in_location);

		/*! Adds text strings to the page.  This will replace all text (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_text Text to add to the page.
		 *	\param in_locations Text locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetText(size_t in_count, TextKit const in_text[], IntRectangle const in_locations[]);

		/*! Adds text strings to the page.  This will replace all text (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_text Text to add to the page.
		 *	\param in_locations Text locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetText(TextKitArray const & in_text, IntRectangleArray const & in_locations);

		/*! Adds text strings to the page.  This will not replace any text on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_text Text to add to the page.
		 *	\param in_locations Text locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddText(size_t in_count, TextKit const in_text[], IntRectangle const in_locations[]);

		/*! Adds text strings to the page.  This will not replace any text on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_text Text to add to the page.
		 *	\param in_locations Text locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddText(TextKitArray const & in_text, IntRectangleArray const & in_locations);

		/*! Adds an image to the page.  This will replace all images (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_image Image to add to the page.
		 *	\param in_location Image location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetImage(ImageKit const & in_image, IntRectangle const & in_location);

		/*! Adds an image to the page.  This will not replace any images on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_image Image to add to the page.
		 *	\param in_location Image location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddImage(ImageKit const & in_image, IntRectangle const & in_location);

		/*! Adds a list of images to the page.  This will replace all images (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_images Images to add to the page.
		 *	\param in_locations Image locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetImages(size_t in_count, ImageKit const in_images[], IntRectangle const in_locations[]);

		/*! Adds a list of images to the page.  This will replace all images (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_images Images to add to the page.
		 *	\param in_locations Image locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetImages(ImageKitArray const & in_images, IntRectangleArray const & in_locations);

		/*! Adds a list of images to the page.  This will not replace any images on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_images Images to add to the page.
		 *	\param in_locations Image locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddImages(size_t in_count, ImageKit const in_images[], IntRectangle const in_locations[]);

		/*! Adds a list of images to the page.  This will not replace any images on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_images Images to add to the page.
		 *	\param in_locations Image locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddImages(ImageKitArray const & in_images, IntRectangleArray const & in_locations);

		/*! Adds a table to the page.  This will replace all tables (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_table Table to add to the page.
		 *	\param in_location Table location in points relative to upper left of the page.  Only the left and top fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTable(TableKit const & in_table, IntRectangle const & in_location);

		/*! Adds a table to the page.  This will not replace any tables on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_table Table to add to the page.
		 *	\param in_location Table location in points relative to upper left of the page.  Only the left and top fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddTable(TableKit const & in_table, IntRectangle const & in_location);

		/*! Adds a list of tables to the page.  This will replace all tables (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_tables Tables to add to the page.
		 *	\param in_locations Table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTables(size_t in_count, TableKit const in_tables[], IntRectangle const in_locations[]);

		/*! Adds a list of tables to the page.  This will replace all tables (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_tables Tables to add to the page.
		 *	\param in_locations Table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTables(TableKitArray const & in_tables, IntRectangleArray const & in_locations);

		/*! Adds a list of tables to the page.  This will not replace any tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_tables Tables to add to the page.
		 *	\param in_locations Table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddTables(size_t in_count, TableKit const in_tables[], IntRectangle const in_locations[]);

		/*! Adds a list of tables to the page.  This will not replace any tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_tables Tables to add to the page.
		 *	\param in_locations Table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddTables(TableKitArray const & in_tables, IntRectangleArray const & in_locations);

		/*! Adds a link to the page.  This will replace all links (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_link Link to add to the page.
		 *	\param in_location Link location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetLink(LinkKit const & in_link, IntRectangle const & in_location);

		/*! Adds a link to the page.  This will not replace any links on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_link Link to add to the page.
		 *	\param in_location Link location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddLink(LinkKit const & in_link, IntRectangle const & in_location);

		/*! Adds a list of links to the page.  This will replace all links (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_links Links to add to the page.
		 *	\param in_locations Link locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetLinks(size_t in_count, LinkKit const in_links[], IntRectangle const in_locations[]);

		/*! Adds a list of links to the page.  This will replace all links (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_links Links to add to the page.
		 *	\param in_locations Link locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetLinks(LinkKitArray const & in_links, IntRectangleArray const & in_locations);

		/*! Adds a list of links to the page.  This will not replace any links on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_links Links to add to the page.
		 *	\param in_locations Link locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddLinks(size_t in_count, LinkKit const in_links[], IntRectangle const in_locations[]);

		/*! Adds a list of links to the page.  This will not replace any links on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_links Links to add to the page.
		 *	\param in_locations Link locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddLinks(LinkKitArray const & in_links, IntRectangleArray const & in_locations);

		/*! Adds a button to the page.  This will replace all buttons (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_button Button to add to the page.
		 *	\param in_location Button location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButton(ButtonKit const & in_button, IntRectangle const & in_location);

		/*! Adds a button to the page.  This will not replace any buttons on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_button Button to add to the page.
		 *	\param in_location Button location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddButton(ButtonKit const & in_button, IntRectangle const & in_location);

		/*! Adds a list of buttons to the page.  This will replace all buttons (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_buttons Buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtons(size_t in_count, ButtonKit const in_buttons[], IntRectangle const in_locations[]);

		/*! Adds a list of buttons to the page.  This will replace all buttons (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_buttons Buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtons(ButtonKitArray const & in_buttons, IntRectangleArray const & in_locations);

		/*! Adds a list of buttons to the page.  This will not replace any buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_buttons Buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddButtons(size_t in_count, ButtonKit const in_buttons[], IntRectangle const in_locations[]);

		/*! Adds a list of buttons to the page.  This will not replace any buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_buttons Buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddButtons(ButtonKitArray const & in_buttons, IntRectangleArray const & in_locations);

		/*! Adds a text field to the page.  This will replace all text fields (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_text_field Text field to add to the page.
		 *	\param in_location Text field location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTextField(TextFieldKit const & in_text_field, IntRectangle const & in_location);

		/*! Adds a text field to the page.  This will not replace any text fields on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_text_field Text field to add to the page.
		 *	\param in_location Text field location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddTextField(TextFieldKit const & in_text_field, IntRectangle const & in_location);

		/*! Adds a list of text fields to the page.  This will replace all text fields (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_text_fields Text fields to add to the page.
		 *	\param in_locations Text field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTextFields(size_t in_count, TextFieldKit const in_text_fields[], IntRectangle const in_locations[]);

		/*! Adds a list of text fields to the page.  This will replace all text fields (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_text_fields Text fields to add to the page.
		 *	\param in_locations Text field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTextFields(TextFieldKitArray const & in_text_fields, IntRectangleArray const & in_locations);

		/*! Adds a list of text fields to the page.  This will not replace any text fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_text_fields Text fields to add to the page.
		 *	\param in_locations Text field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddTextFields(size_t in_count, TextFieldKit const in_text_fields[], IntRectangle const in_locations[]);

		/*! Adds a list of text fields to the page.  This will not replace any text fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_text_fields Text fields to add to the page.
		 *	\param in_locations Text field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddTextFields(TextFieldKitArray const & in_text_fields, IntRectangleArray const & in_locations);

		/*! Adds a slide table to the page.  This will replace all slide tables (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_slide_table Slide table to add to the page.
		 *	\param in_location Slide table location in points relative to upper left of the page.  Only the left and top fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetSlideTable(SlideTableKit const & in_slide_table, IntRectangle const & in_location);

		/*! Adds a slide table to the page.  This will not replace any slide tables on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_slide_table Slide table to add to the page.
		 *	\param in_location Slide table location in points relative to upper left of the page.  Only the left and top fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddSlideTable(SlideTableKit const & in_slide_table, IntRectangle const & in_location);

		/*! Adds a list of slide tables to the page.  This will replace all slide tables (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_slide_tables Slide tables to add to the page.
		 *	\param in_locations Slide table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetSlideTables(size_t in_count, SlideTableKit const in_slide_tables[], IntRectangle const in_locations[]);

		/*! Adds a list of slide tables to the page.  This will replace all slide tables (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_slide_tables Slide tables to add to the page.
		 *	\param in_locations Slide table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetSlideTables(SlideTableKitArray const & in_slide_tables, IntRectangleArray const & in_locations);

		/*! Adds a list of slide tables to the page.  This will not replace any slide tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_slide_tables Slide tables to add to the page.
		 *	\param in_locations Slide table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddSlideTables(size_t in_count, SlideTableKit const in_slide_tables[], IntRectangle const in_locations[]);

		/*! Adds a list of slide tables to the page.  This will not replace any slide tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_slide_tables Slide tables to add to the page.
		 *	\param in_locations Slide table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddSlideTables(SlideTableKitArray const & in_slide_tables, IntRectangleArray const & in_locations);

		/*! Adds a check box to the page.  This will replace all check box (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7433812feb725392327a6266e38d85a4">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_checkbox Check Box to add to the page.
		 *	\param in_location Check Box location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetCheckBox(CheckBoxKit const & in_checkbox, IntRectangle const & in_location);

		/*! Adds a check box to the page.  This will not replace any check boxes on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7433812feb725392327a6266e38d85a4">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_checkbox Check Box to add to the page.
		 *	\param in_location Check Box location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddCheckBox(CheckBoxKit const & in_checkbox, IntRectangle const & in_location);

		/*! Adds check boxes to the page.  This will replace all check boxes (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7433812feb725392327a6266e38d85a4">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_checkbox Check Box to add to the page.
		 *	\param in_locations Check Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetCheckBoxes(size_t in_count, CheckBoxKit const in_checkbox[], IntRectangle const in_locations[]);

		/*! Adds check boxes to the page.  This will replace all check boxes (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7433812feb725392327a6266e38d85a4">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_checkbox Check Box to add to the page.
		 *	\param in_locations Check Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetCheckBoxes(CheckBoxKitArray const & in_checkbox, IntRectangleArray const & in_locations);

		/*! Adds check boxes to the page.  This will not replace any check boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7433812feb725392327a6266e38d85a4">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_checkbox Check Box to add to the page.
		 *	\param in_locations Check Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddCheckBoxes(size_t in_count, CheckBoxKit const in_checkbox[], IntRectangle const in_locations[]);

		/*! Adds check boxes to the page.  This will not replace any check boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7433812feb725392327a6266e38d85a4">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_checkbox Check Box to add to the page.
		 *	\param in_locations Check Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddCheckBoxes(CheckBoxKitArray const & in_checkbox, IntRectangleArray const & in_locations);

		/*! Adds a radio button to the page.  This will replace all radio buttons (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac24f87cbf1a247c8516ebfa6f7cf7f89">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_radio_button Radio button to add to the page.
		 *	\param in_location Button location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetRadioButton(RadioButtonKit const & in_radio_button, IntRectangle const & in_location);

		/*! Adds a radio button to the page.  This will not replace any radio buttons on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac24f87cbf1a247c8516ebfa6f7cf7f89">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_radio_button Radio button to add to the page.
		 *	\param in_location Button location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddRadioButton(RadioButtonKit const & in_radio_button, IntRectangle const & in_location);

		/*! Adds a list of buttons to the page.  This will replace all radio buttons (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac24f87cbf1a247c8516ebfa6f7cf7f89">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_radio_buttons Radio buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetRadioButtons(size_t in_count, RadioButtonKit const in_radio_buttons[], IntRectangle const in_locations[]);

		/*! Adds a list of buttons to the page.  This will replace all radio buttons (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac24f87cbf1a247c8516ebfa6f7cf7f89">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_radio_buttons Radio buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetRadioButtons(RadioButtonKitArray const & in_radio_buttons, IntRectangleArray const & in_locations);

		/*! Adds a list of buttons to the page.  This will not replace any radio buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac24f87cbf1a247c8516ebfa6f7cf7f89">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_radio_buttons Radio buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddRadioButtons(size_t in_count, RadioButtonKit const in_radio_buttons[], IntRectangle const in_locations[]);

		/*! Adds a list of buttons to the page.  This will not replace any radio buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac24f87cbf1a247c8516ebfa6f7cf7f89">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_radio_buttons Radio buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddRadioButtons(RadioButtonKitArray const & in_radio_buttons, IntRectangleArray const & in_locations);

		/*! Adds a list box to the page.  This will replace all list box (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga2f971ef3bf91640243af5beb32d79ad2">A3DPDFPageInsertListBox</a>.
		 *	\param in_list_box List Box to add to the page.
		 *	\param in_location List Box location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetListBox(ListBoxKit const & in_list_box, IntRectangle const & in_location);

		/*! Adds a list box to the page.  This will not replace any list boxes on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga2f971ef3bf91640243af5beb32d79ad2">A3DPDFPageInsertListBox</a>.
		 *	\param in_list_box List Box to add to the page.
		 *	\param in_location List Box location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddListBox(ListBoxKit const & in_list_box, IntRectangle const & in_location);

		/*! Adds list boxes to the page.  This will replace all list boxes (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga2f971ef3bf91640243af5beb32d79ad2">A3DPDFPageInsertListBox</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_list_box List Box to add to the page.
		 *	\param in_locations List Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetListBoxes(size_t in_count, ListBoxKit const in_list_box[], IntRectangle const in_locations[]);

		/*! Adds list boxes to the page.  This will replace all list boxes (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga2f971ef3bf91640243af5beb32d79ad2">A3DPDFPageInsertListBox</a>.
		 *	\param in_list_box List Box to add to the page.
		 *	\param in_locations List Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetListBoxes(ListBoxKitArray const & in_list_box, IntRectangleArray const & in_locations);

		/*! Adds list boxes to the page.  This will not replace any list boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga2f971ef3bf91640243af5beb32d79ad2">A3DPDFPageInsertListBox</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_list_box List Box to add to the page.
		 *	\param in_locations List Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddListBoxes(size_t in_count, ListBoxKit const in_list_box[], IntRectangle const in_locations[]);

		/*! Adds list boxes to the page.  This will not replace any list boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga2f971ef3bf91640243af5beb32d79ad2">A3DPDFPageInsertListBox</a>.
		 *	\param in_list_box List Box to add to the page.
		 *	\param in_locations List Box locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddListBoxes(ListBoxKitArray const & in_list_box, IntRectangleArray const & in_locations);

		/*! Adds a drop down list to the page.  This will replace all drop down list (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gab9b0df08811a3e7005540ad1114ad818">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_drop_down_list Drop Down List to add to the page.
		 *	\param in_location Drop Down List location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetDropDownList(DropDownListKit const & in_drop_down_list, IntRectangle const & in_location);

		/*! Adds a drop down list to the page.  This will not replace any drop down lists on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gab9b0df08811a3e7005540ad1114ad818">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_drop_down_list Drop Down List to add to the page.
		 *	\param in_location Drop Down List location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddDropDownList(DropDownListKit const & in_drop_down_list, IntRectangle const & in_location);

		/*! Adds drop down lists to the page.  This will replace all drop down lists (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gab9b0df08811a3e7005540ad1114ad818">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_drop_down_list Drop Down List to add to the page.
		 *	\param in_locations Drop Down List locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetDropDownLists(size_t in_count, DropDownListKit const in_drop_down_list[], IntRectangle const in_locations[]);

		/*! Adds drop down lists to the page.  This will replace all drop down lists (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gab9b0df08811a3e7005540ad1114ad818">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_drop_down_list Drop Down List to add to the page.
		 *	\param in_locations Drop Down List locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetDropDownLists(DropDownListKitArray const & in_drop_down_list, IntRectangleArray const & in_locations);

		/*! Adds drop down lists to the page.  This will not replace any drop down lists on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gab9b0df08811a3e7005540ad1114ad818">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_drop_down_list Drop Down List to add to the page.
		 *	\param in_locations Drop Down List locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddDropDownLists(size_t in_count, DropDownListKit const in_drop_down_list[], IntRectangle const in_locations[]);

		/*! Adds drop down lists to the page.  This will not replace any drop down lists on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gab9b0df08811a3e7005540ad1114ad818">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_drop_down_list Drop Down List to add to the page.
		 *	\param in_locations Drop Down List locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddDropDownLists(DropDownListKitArray const & in_drop_down_list, IntRectangleArray const & in_locations);

		/*! Adds a signature field to the page.  This will replace all signature field (if any) on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga3f2f1980667a3a840923320736264203">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_signature_field Signature Field to add to the page.
		 *	\param in_location Signature Field location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetSignatureField(SignatureFieldKit const & in_signature_field, IntRectangle const & in_location);

		/*! Adds a signature field to the page.  This will not replace any signature fields on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga3f2f1980667a3a840923320736264203">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_signature_field Signature Field to add to the page.
		 *	\param in_location Signature Field location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddSignatureField(SignatureFieldKit const & in_signature_field, IntRectangle const & in_location);

		/*! Adds signature fields to the page.  This will replace all signature fields (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga3f2f1980667a3a840923320736264203">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_signature_field Signature Field to add to the page.
		 *	\param in_locations Signature Field locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetSignatureFields(size_t in_count, SignatureFieldKit const in_signature_field[], IntRectangle const in_locations[]);

		/*! Adds signature fields to the page.  This will replace all signature fields (if any) on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga3f2f1980667a3a840923320736264203">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_signature_field Signature Field to add to the page.
		 *	\param in_locations Signature Field locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			SetSignatureFields(SignatureFieldKitArray const & in_signature_field, IntRectangleArray const & in_locations);

		/*! Adds signature fields to the page.  This will not replace any signature fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga3f2f1980667a3a840923320736264203">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_signature_field Signature Field to add to the page.
		 *	\param in_locations Signature Field locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddSignatureFields(size_t in_count, SignatureFieldKit const in_signature_field[], IntRectangle const in_locations[]);

		/*! Adds signature fields to the page.  This will not replace any signature fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga3f2f1980667a3a840923320736264203">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_signature_field Signature Field to add to the page.
		 *	\param in_locations Signature Field locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageKit. */
		PageKit &			AddSignatureFields(SignatureFieldKitArray const & in_signature_field, IntRectangleArray const & in_locations);

		/*! Sets the PDF file which will act as a template.
		 *	\param in_filename UTF8-encoded filename for the PDF file to act as a template.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTemplateFile(char const * in_filename);

		/*! Sets an annotation on the given form field.  If there is an existing annotation on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gadf6f14b2368a856f31169dc03e0c501e">A3DPDFPageFieldSet3DAnnot</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the annotation.
		 *	\param in_annotation The annotation to be set on the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetAnnotationByField(char const * in_field, AnnotationKit const & in_annotation);

		/*! Sets annotations on the given form fields.  If there is an existing annotation on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gadf6f14b2368a856f31169dc03e0c501e">A3DPDFPageFieldSet3DAnnot</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the annotations.
		 *	\param in_annotations The annotations to be set on the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetAnnotationsByField(size_t in_count, UTF8 const in_fields[], AnnotationKit const in_annotations[]);

		/*! Sets annotations on the given form fields.  If there is an existing annotation on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gadf6f14b2368a856f31169dc03e0c501e">A3DPDFPageFieldSet3DAnnot</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_fields The form field names for the annotations.
		 *	\param in_annotations The annotations to be set on the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetAnnotationsByField(UTF8Array const & in_fields, AnnotationKitArray const & in_annotations);

		/*! Sets an image on the icon at the given form field.  If there is an existing image on an icon at the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga6051e30d012d9dd265fe8eb3a01f317f">A3DPDFPageFieldButtonSetIcon</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the icon.
		 *	\param in_image The image to be set on the icon at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtonIconByField(char const * in_field, ImageKit const & in_image);

		/*! Sets images on the icons at the given form fields.  If there is an existing image on an icon at a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga6051e30d012d9dd265fe8eb3a01f317f">A3DPDFPageFieldButtonSetIcon</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the icons.
		 *	\param in_images The images to be set on the icons at the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtonIconsByField(size_t in_count, UTF8 const in_fields[], ImageKit const in_images[]);

		/*! Sets images on the icons at the given form fields.  If there is an existing image on an icon at a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga6051e30d012d9dd265fe8eb3a01f317f">A3DPDFPageFieldButtonSetIcon</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_fields The form field names for the icons.
		 *	\param in_images The images to be set on the icons at the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtonIconsByField(UTF8Array const & in_fields, ImageKitArray const & in_images);

		/*! Sets a label on the given form field.  If there is an existing label on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldButtonSetLabel</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the label.
		 *	\param in_label UTF8-encoded label to be set on the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtonLabelByField(char const * in_field, char const * in_label);

		/*! Sets labels on the given form fields.  If there is an existing label on a given form field, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldButtonSetLabel</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the labels.
		 *	\param in_labels The labels to be set on the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtonLabelsByField(size_t in_count, UTF8 const in_fields[], UTF8 const in_labels[]);

		/*! Sets labels on the given form fields.  If there is an existing label on a given form field, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldButtonSetLabel</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_fields The form field names for the labels.
		 *	\param in_labels The labels to be set on the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetButtonLabelsByField(UTF8Array const & in_fields, UTF8Array const & in_labels);

		/*! Adds an item to the list at the given form field.  If there are any existing items in the list at the given form field, they will be overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_value UTF8-encoded value for the item to be added to the list at the given form field.
		 *	\param in_export_value UTF8-encoded export value for the item to be added to the list at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetListItemByField(char const * in_field, char const * in_value, char const * in_export_value);

		/*! Adds an item to the list at the given form field.  The item to will be appended to any existing items in the list at the given form field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_value UTF8-encoded value for the item to be added to the list at the given form field.
		 *	\param in_export_value UTF8-encoded export value for the item to be added to the list at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			AddListItemByField(char const * in_field, char const * in_value, char const * in_export_value);

		/*! Adds items to the list at the given form field.  If there are any existing items in the list at the given form field, they will be overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_count Size of the following arrays.
		 *	\param in_values The values for the items to be added to the list at the given form field.
		 *	\param in_export_values The export values for the items to be added to the list at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetListItemsByField(char const * in_field, size_t in_count, UTF8 const in_values[], UTF8 const in_export_values[]);

		/*! Adds items to the list at the given form field.  If there are any existing items in the list at the given form field, they will be overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_values The values for the items to be added to the list at the given form field.
		 *	\param in_export_values The export values for the items to be added to the list at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetListItemsByField(char const * in_field, UTF8Array const & in_values, UTF8Array const & in_export_values);

		/*! Adds items to the list at the given form field.  The items to will be appended to any existing items in the list at the given form field.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_count Size of the following arrays.
		 *	\param in_values The values for the items to be added to the list at the given form field.
		 *	\param in_export_values The export values for the items to be added to the list at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			AddListItemsByField(char const * in_field, size_t in_count, UTF8 const in_values[], UTF8 const in_export_values[]);

		/*! Adds items to the list at the given form field.  The items to will be appended to any existing items in the list at the given form field.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_values The values for the items to be added to the list at the given form field.
		 *	\param in_export_values The export values for the items to be added to the list at the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			AddListItemsByField(char const * in_field, UTF8Array const & in_values, UTF8Array const & in_export_values);

		/*! Sets a JavaScript source or file on the given form field.  If there is any existing JavaScript source or file at the given form field, it will be overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac02217c03ab6cc02581362eab77c12f6">A3DPDFPageFieldSetActionJavascriptFromString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacf5655c5eb503b4c0fa6f719699a90ee">A3DPDFPageFieldSetActionJavascriptFromFile</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the JavaScript action.
		 *	\param in_source UTF8-encoded JavaScript source or file (depending on the following argument) to be set on the given form field.
		 *	\param in_type The type of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this PageKit. */
		PageKit &			SetJavaScriptActionByField(char const * in_field, char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Sets JavaScript sources and/or files on the given form fields.  If there is any existing JavaScript source or file at a given form field, it will be overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac02217c03ab6cc02581362eab77c12f6">A3DPDFPageFieldSetActionJavascriptFromString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacf5655c5eb503b4c0fa6f719699a90ee">A3DPDFPageFieldSetActionJavascriptFromFile</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the JavaScript actions.
		 *	\param in_sources The JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be set on the corresponding form fields.
		 *	\param in_types The types for each item in the preceding argument array.
		 *	\return A reference to this PageKit. */
		PageKit &			SetJavaScriptActionsByField(size_t in_count, UTF8 const in_fields[], UTF8 const in_sources[], Source::Type const in_types[]);

		/*! Sets JavaScript sources and/or files on the given form fields.  If there is any existing JavaScript source or file at a given form field, it will be overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac02217c03ab6cc02581362eab77c12f6">A3DPDFPageFieldSetActionJavascriptFromString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacf5655c5eb503b4c0fa6f719699a90ee">A3DPDFPageFieldSetActionJavascriptFromFile</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_fields The form field names for the JavaScript actions.
		 *	\param in_sources The JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be set on the corresponding form fields.
		 *	\param in_types The types for each item in the preceding argument array.
		 *	\return A reference to this PageKit. */
		PageKit &			SetJavaScriptActionsByField(UTF8Array const & in_fields, UTF8Array const & in_sources, SourceTypeArray const & in_types);

		/*! Sets whether the object at the given form field should be visible.  If there is an existing visibility on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacdabb9f6e7fc6a887de9516148549b8d">A3DPDFPageFieldSetVisibility</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name on which to set visibility.
		 *	\param in_state Whether the object at the at the given form field should be visible.
		 *	\return A reference to this PageKit. */
		PageKit &			SetVisibilityByField(char const * in_field, bool in_state);

		/*! Sets whether the objects at the given form fields should be visible.  If there is an existing visibility on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacdabb9f6e7fc6a887de9516148549b8d">A3DPDFPageFieldSetVisibility</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names on which to set visibility.
		 *	\param in_states Whether the objects at the at the corresponding form fields should be visible.
		 *	\return A reference to this PageKit. */
		PageKit &			SetVisibilitiesByField(size_t in_count, UTF8 const in_fields[], bool const in_states[]);

		/*! Sets whether the objects at the given form fields should be visible.  If there is an existing visibility on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacdabb9f6e7fc6a887de9516148549b8d">A3DPDFPageFieldSetVisibility</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_fields The form field names on which to set visibility.
		 *	\param in_states Whether the objects at the at the corresponding form fields should be visible.
		 *	\return A reference to this PageKit. */
		PageKit &			SetVisibilitiesByField(UTF8Array const & in_fields, BoolArray const & in_states);

		/*! Sets a text value on the given form field.  If there is an existing text value on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga8f171e2004b51ae1ebcec2eceb773b00">A3DPDFPageFieldTextSetValue</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_field UTF8-encoded form field name for the text value.
		 *	\param in_value UTF8-encoded text value to be set on the given form field.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTextValueByField(char const * in_field, char const * in_value);

		/*! Sets text values on the given form fields.  If there is an existing text value on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga8f171e2004b51ae1ebcec2eceb773b00">A3DPDFPageFieldTextSetValue</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the text values.
		 *	\param in_values The text values to be set on the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTextValuesByField(size_t in_count, UTF8 const in_fields[], UTF8 const in_values[]);

		/*! Sets text values on the given form fields.  If there is an existing text value on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga8f171e2004b51ae1ebcec2eceb773b00">A3DPDFPageFieldTextSetValue</a>.
		 *	This will only have an effect if the page has a template file.
		 *	\param in_fields The form field names for the text values.
		 *	\param in_values The text values to be set on the corresponding form fields.
		 *	\return A reference to this PageKit. */
		PageKit &			SetTextValuesByField(UTF8Array const & in_fields, UTF8Array const & in_values);


		/*! Removes the format from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetFormat();

		/*! Removes the orientation from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetOrientation();

		/*! Removes the annotation at the given index from the page.
		 *	\param in_index Index of the annotation to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotation(size_t in_index);

		/*! Removes the annotations at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the annotations to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotations(size_t in_count, size_t const in_indices[]);

		/*! Removes the annotations at the given indices from the page.
		 *	\param in_indices Indices of the annotations to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotations(SizeTArray const & in_indices);

		/*! Removes all annotations from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotations();

		/*! Removes the text at the given index from the page.
		 *	\param in_index Index of the text to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetText(size_t in_index);

		/*! Removes the text at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the text to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetText(size_t in_count, size_t const in_indices[]);

		/*! Removes the text at the given indices from the page.
		 *	\param in_indices Indices of the text to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetText(SizeTArray const & in_indices);

		/*! Removes all text from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetText();

		/*! Removes the check box at the given index from the page.
		 *	\param in_index Index of the check box to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetCheckBox(size_t in_index);

		/*! Removes the check box at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the check box to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetCheckBoxes(size_t in_count, size_t const in_indices[]);

		/*! Removes the check box at the given indices from the page.
		 *	\param in_indices Indices of the check box to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetCheckBoxes(SizeTArray const & in_indices);

		/*! Removes all check boxes from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetCheckBoxes();

		/*! Removes the radio button at the given index from the page.
		 *	\param in_index Index of the radio button to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetRadioButton(size_t in_index);

		/*! Removes the radio button at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the radio button to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetRadioButtons(size_t in_count, size_t const in_indices[]);

		/*! Removes the radio button at the given indices from the page.
		 *	\param in_indices Indices of the radio button to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetRadioButtons(SizeTArray const & in_indices);

		/*! Removes all radio buttons from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetRadioButtons();

		/*! Removes the list box at the given index from the page.
		 *	\param in_index Index of the list box to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListBox(size_t in_index);

		/*! Removes the list box at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the list box to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListBoxes(size_t in_count, size_t const in_indices[]);

		/*! Removes the list box at the given indices from the page.
		 *	\param in_indices Indices of the list box to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListBoxes(SizeTArray const & in_indices);

		/*! Removes all list boxes from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListBoxes();

		/*! Removes the drop down list at the given index from the page.
		 *	\param in_index Index of the drop down list to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetDropDownList(size_t in_index);

		/*! Removes the drop down list at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the drop down list to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetDropDownLists(size_t in_count, size_t const in_indices[]);

		/*! Removes the drop down list at the given indices from the page.
		 *	\param in_indices Indices of the drop down list to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetDropDownLists(SizeTArray const & in_indices);

		/*! Removes all drop down lists from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetDropDownLists();

		/*! Removes the signature field at the given index from the page.
		 *	\param in_index Index of the signature field to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSignatureField(size_t in_index);

		/*! Removes the signature field at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the signature field to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSignatureFields(size_t in_count, size_t const in_indices[]);

		/*! Removes the signature field at the given indices from the page.
		 *	\param in_indices Indices of the signature field to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSignatureFields(SizeTArray const & in_indices);

		/*! Removes all signature fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSignatureFields();

		/*! Removes the image at the given index from the page.
		 *	\param in_index Index of the image to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetImage(size_t in_index);

		/*! Removes the images at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the images to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetImages(size_t in_count, size_t const in_indices[]);

		/*! Removes the images at the given indices from the page.
		 *	\param in_indices Indices of the images to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetImages(SizeTArray const & in_indices);

		/*! Removes all images from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetImages();

		/*! Removes the table at the given index from the page.
		 *	\param in_index Index of the table to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTable(size_t in_index);

		/*! Removes the tables at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the tables to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTables(size_t in_count, size_t const in_indices[]);

		/*! Removes the tables at the given indices from the page.
		 *	\param in_indices Indices of the tables to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTables(SizeTArray const & in_indices);

		/*! Removes all tables from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTables();

		/*! Removes the link at the given index from the page.
		 *	\param in_index Index of the link to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetLink(size_t in_index);

		/*! Removes the links at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the links to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetLinks(size_t in_count, size_t const in_indices[]);

		/*! Removes the links at the given indices from the page.
		 *	\param in_indices Indices of the links to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetLinks(SizeTArray const & in_indices);

		/*! Removes all links from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetLinks();

		/*! Removes the button at the given index from the page.
		 *	\param in_index Index of the button to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButton(size_t in_index);

		/*! Removes the buttons at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the buttons to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtons(size_t in_count, size_t const in_indices[]);

		/*! Removes the buttons at the given indices from the page.
		 *	\param in_indices Indices of the buttons to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtons(SizeTArray const & in_indices);

		/*! Removes all buttons from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtons();

		/*! Removes the text field at the given index from the page.
		 *	\param in_index Index of the text field to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextField(size_t in_index);

		/*! Removes the text fields at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the text fields to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextFields(size_t in_count, size_t const in_indices[]);

		/*! Removes the text fields at the given indices from the page.
		 *	\param in_indices Indices of the text fields to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextFields(SizeTArray const & in_indices);

		/*! Removes all text fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextFields();

		/*! Removes the slide table at the given index from the page.
		 *	\param in_index Index of the slide table to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSlideTable(size_t in_index);

		/*! Removes the slide tables at the given indices from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the slide tables to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSlideTables(size_t in_count, size_t const in_indices[]);

		/*! Removes the slide tables at the given indices from the page.
		 *	\param in_indices Indices of the slide tables to remove from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSlideTables(SizeTArray const & in_indices);

		/*! Removes all slide tables from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetSlideTables();

		/*! Removes the template file from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTemplateFile();

		/*! Removes the annotation at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name with the annotation to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotationByField(char const * in_field);

		/*! Removes the annotations at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names with the annotations to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotationsByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes the annotations at the given form fields from the page.
		 *	\param in_fields Form field names with the annotations to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotationsByField(UTF8Array const & in_fields);

		/*! Removes all annotation form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetAnnotationsByField();

		/*! Removes the image on the icon at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the icon with the image to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonIconByField(char const * in_field);

		/*! Removes the images on the icons at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names of icons with the images to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonIconsByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes the images on the icons at the given form fields from the page.
		 *	\param in_fields Form field names of icons with the images to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonIconsByField(UTF8Array const & in_fields);

		/*! Removes all button icon form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonIconsByField();

		/*! Removes the label at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the label to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonLabelByField(char const * in_field);

		/*! Removes the labels at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names of the labels to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonLabelsByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes the labels at the given form fields from the page.
		 *	\param in_fields Form field names of the labels to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonLabelsByField(UTF8Array const & in_fields);

		/*! Removes all button label form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetButtonLabelsByField();

		/*! Removes the item at the given index from the list at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the list with the item to be removed.
		 *	\param in_index Index of the item in the list at the given form field to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemByField(char const * in_field, size_t in_index);

		/*! Removes the items at the given indices from the list at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the list with the items to be removed.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the items in the list at the form field to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemsByField(char const * in_field, size_t in_count, size_t const in_indices[]);

		/*! Removes the items at the given indices from the list at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the list with the items to be removed.
		 *	\param in_indices Indices of the items in the list at the form field to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemsByField(char const * in_field, SizeTArray const & in_indices);

		/*! Removes all items from the list at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the list with the items to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemsByField(char const * in_field);

		/*! Removes all items from the lists at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names of the lists with the items to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemsByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes all items from the lists at the given form fields from the page.
		 *	\param in_fields Form field names of the lists with the items to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemsByField(UTF8Array const & in_fields);

		/*! Removes all list item form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetListItemsByField();

		/*! Removes the JavaScript action at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name of the action to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetJavaScriptActionByField(char const * in_field);

		/*! Removes the JavaScript actions at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names of the actions to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetJavaScriptActionsByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes the JavaScript actions at the given form fields from the page.
		 *	\param in_fields Form field names of the actions to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetJavaScriptActionsByField(UTF8Array const & in_fields);

		/*! Removes all JavaScript action form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetJavaScriptActionsByField();

		/*! Removes the visibility setting at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name with the visibility setting to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetVisibilityByField(char const * in_field);

		/*! Removes the visibility settings at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names with the visibility settings to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetVisibilitiesByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes the visibility settings at the given form fields from the page.
		 *	\param in_fields Form field names with the visibility settings to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetVisibilitiesByField(UTF8Array const & in_fields);

		/*! Removes all visibility settings on form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetVisibilitiesByField();

		/*! Removes the text value at the given form field from the page.
		 *	\param in_field UTF8-encoded form field name with the text value to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextValueByField(char const * in_field);

		/*! Removes the text values at the given form fields from the page.
		 *	\param in_count Size of the following array.
		 *	\param in_fields Form field names with the text values to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextValuesByField(size_t in_count, UTF8 const in_fields[]);

		/*! Removes the text values at the given form fields from the page.
		 *	\param in_fields Form field names with the text values to be removed.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextValuesByField(UTF8Array const & in_fields);

		/*! Removes all text value form fields from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetTextValuesByField();

		/*! Removes all data from the page.
		 *	\return A reference to this PageKit. */
		PageKit &			UnsetEverything();


		/*! Shows the format of the page.
		 *	\param out_format Format of the page.
		 *	\return <span class='code'>true</span> if a format was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFormat(Page::Format & out_format) const;

		/*! Shows the orientation of the page.
		 *	\param out_orientation Orientation of the page.
		 *	\return <span class='code'>true</span> if an orientation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowOrientation(Page::Orientation & out_orientation) const;

		/*! Shows the annotations on the page.
		 *	\param out_annotations Annotations on the page.
		 *	\param out_locations Annotation locations on the page.
		 *	\return <span class='code'>true</span> if annotations were specified, <span class='code'>false</span> otherwise. */
		bool				ShowAnnotations(AnnotationKitArray & out_annotations, IntRectangleArray & out_locations) const;

		/*! Shows the text on the page.
		 *	\param out_text Text on the page.
		 *	\param out_locations Text locations on the page.
		 *	\return <span class='code'>true</span> if text strings were specified, <span class='code'>false</span> otherwise. */
		bool				ShowText(TextKitArray & out_text, IntRectangleArray & out_locations) const;

		/*! Shows the images on the page.
		 *	\param out_images Images on the page.
		 *	\param out_locations Image locations on the page.
		 *	\return <span class='code'>true</span> if images were specified, <span class='code'>false</span> otherwise. */
		bool				ShowImages(ImageKitArray & out_images, IntRectangleArray & out_locations) const;

		/*! Shows the tables on the page.
		 *	\param out_tables Tables on the page.
		 *	\param out_locations Table locations on the page.
		 *	\return <span class='code'>true</span> if tables were specified, <span class='code'>false</span> otherwise. */
		bool				ShowTables(TableKitArray & out_tables, IntRectangleArray & out_locations) const;

		/*! Shows the links on the page.
		 *	\param out_links Links on the page.
		 *	\param out_locations Link locations on the page.
		 *	\return <span class='code'>true</span> if links were specified, <span class='code'>false</span> otherwise. */
		bool				ShowLinks(LinkKitArray & out_links, IntRectangleArray & out_locations) const;

		/*! Shows the buttons on the page.
		 *	\param out_buttons Buttons on the page.
		 *	\param out_locations Button locations on the page.
		 *	\return <span class='code'>true</span> if buttons were specified, <span class='code'>false</span> otherwise. */
		bool				ShowButtons(ButtonKitArray & out_buttons, IntRectangleArray & out_locations) const;

		/*! Shows the check boxes on the page.
		 *	\param out_check_boxes Check boxes on the page.
		 *	\param out_locations Check box locations on the page.
		 *	\return <span class='code'>true</span> if check boxes were specified, <span class='code'>false</span> otherwise. */
		bool				ShowCheckBoxes(CheckBoxKitArray & out_check_boxes, IntRectangleArray & out_locations) const;

		/*! Shows the radio buttons on the page.
		 *	\param out_radio_buttons Radio buttons on the page.
		 *	\param out_locations Radio buttons locations on the page.
		 *	\return <span class='code'>true</span> if radio buttons were specified, <span class='code'>false</span> otherwise. */
		bool				ShowRadioButtons(RadioButtonKitArray & out_radio_buttons, IntRectangleArray & out_locations) const;

		/*! Shows the list boxes on the page.
		 *	\param out_list_boxes List boxes on the page.
		 *	\param out_locations List boxes locations on the page.
		 *	\return <span class='code'>true</span> if list boxes were specified, <span class='code'>false</span> otherwise. */
		bool				ShowListBoxes(ListBoxKitArray & out_list_boxes, IntRectangleArray & out_locations) const;

		/*! Shows the drop down lists on the page.
		 *	\param out_drop_down_lists Drop down lists on the page.
		 *	\param out_locations Drop drown list locations on the page.
		 *	\return <span class='code'>true</span> if drop down lists were specified, <span class='code'>false</span> otherwise. */
		bool				ShowDropDownLists(DropDownListKitArray & out_drop_down_lists, IntRectangleArray & out_locations) const;

		/*! Shows the signature fields on the page.
		 *	\param out_signature_fields Signature fields on the page.
		 *	\param out_locations Signature field locations on the page.
		 *	\return <span class='code'>true</span> if signature fields were specified, <span class='code'>false</span> otherwise. */
		bool				ShowSignatureFields(SignatureFieldKitArray & out_signature_fields, IntRectangleArray & out_locations) const;

		/*! Shows the text fields on the page.
		 *	\param out_text_fields Text fields on the page.
		 *	\param out_locations Text field locations on the page.
		 *	\return <span class='code'>true</span> if text fields were specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextFields(TextFieldKitArray & out_text_fields, IntRectangleArray & out_locations) const;

		/*! Shows the slide tables on the page.
		 *	\param out_slide_tables Slide tables on the page.
		 *	\param out_locations Slide table locations on the page.
		 *	\return <span class='code'>true</span> if slide tables were specified, <span class='code'>false</span> otherwise. */
		bool				ShowSlideTables(SlideTableKitArray & out_slide_tables, IntRectangleArray & out_locations) const;

		/*! Shows the template file.
		 *	\param out_filename The filename for the PDF file to act as a template.
		 *	\return <span class='code'>true</span> if a template file was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTemplateFile(UTF8 & out_filename) const;

		/*! Shows the annotations on form fields on the page.
		 *	\param out_fields The annotation form field names on the page.
		 *	\param out_annotations The annotations on the corresponding form fields.
		 *	\return <span class='code'>true</span> if form field annotations were specified, <span class='code'>false</span> otherwise. */
		bool				ShowAnnotationsByField(UTF8Array & out_fields, AnnotationKitArray & out_annotations) const;

		/*! Shows the images on icons on form fields on the page.
		 *	\param out_fields The button icon form field names on the page.
		 *	\param out_images The images on the icons at the corresponding form fields.
		 *	\return <span class='code'>true</span> if form field icons were specified, <span class='code'>false</span> otherwise. */
		bool				ShowButtonIconsByField(UTF8Array & out_fields, ImageKitArray & out_images) const;

		/*! Shows the labels on form fields on the page.
		 *	\param out_fields The button label form field names on the page.
		 *	\param out_labels The labels on the corresponding form fields.
		 *	\return <span class='code'>true</span> if form field labels were specified, <span class='code'>false</span> otherwise. */
		bool				ShowButtonLabelsByField(UTF8Array & out_fields, UTF8Array & out_labels) const;

		/*! Shows the list item form fields on the page.
		 *	\param out_fields The list form field names on the page.
		 *	\return <span class='code'>true</span> if any form field lists were specified, <span class='code'>false</span> otherwise. */
		bool				ShowListItemFields(UTF8Array & out_fields) const;

		/*! Shows the items in the list on the given field on the page.
		 *	\param in_field UTF8-encoded form field name of the list with the items to be shown.
		 *	\param out_values The values for the items in the list at the given form field.
		 *	\param out_export_values The export values for the items in the list at the given form field.
		 *	\return <span class='code'>true</span> if list items on the given form field were specified, <span class='code'>false</span> otherwise. */
		bool				ShowListItemsByField(char const * in_field, UTF8Array & out_values, UTF8Array & out_export_values) const;

		/*! Shows the JavaScript actions on form fields on the page.
		 *	\param out_fields The form field names with JavaScript actions on the page.
		 *	\param out_sources The JavaScript sources and/or files (depending on the corresponding entry in the following argument) on the corresponding form fields.
		 *	\param out_types The types for each item in the preceding argument array.
		 *	\return <span class='code'>true</span> if form field JavaScript actions were specified, <span class='code'>false</span> otherwise. */
		bool				ShowJavaScriptActionsByField(UTF8Array & out_fields, UTF8Array & out_sources, SourceTypeArray & out_types) const;

		/*! Shows the visibility settings on form fields on the page.
		 *	\param out_fields The form field names with visibility settings on the page.
		 *	\param out_states Whether the objects at the at the corresponding form fields should be visible.
		 *	\return <span class='code'>true</span> if form field visibilities were specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibilitiesByField(UTF8Array & out_fields, BoolArray & out_states) const;

		/*! Shows the text values on form fields on the page.
		 *	\param out_fields The form field names with the text values on the template.
		 *	\param out_values The text values on the corresponding form fields.
		 *	\return <span class='code'>true</span> if form field text values were specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextValuesByField(UTF8Array & out_fields, UTF8Array & out_values) const;
	};

	/*! The AnnotationKit class is a user space object.  It acts as the container for all data that can be on an annotation in a Publish PDF. */
	class PUBLISH_API AnnotationKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty AnnotationKit object. */
		AnnotationKit();

		/*! The copy constructor creates a new AnnotationKit object that contains the same settings as the source AnnotationKit.
		 * 	\param in_kit The source AnnotationKit to copy. */
		AnnotationKit(AnnotationKit const & in_kit);

		/*! The move constructor creates an AnnotationKit by transferring the underlying object of the rvalue reference to this AnnotationKit.
		 * 	\param in_kit An rvalue reference to an AnnotationKit to take the underlying object from. */
		AnnotationKit(AnnotationKit && in_kit);

		virtual ~AnnotationKit();

	static const HPS::Type staticType = HPS::Type::PublishAnnotationKit;
		HPS::Type				ObjectType() const { return staticType; }

		/*! Creates a AnnotationKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for export unless a setting is overridden by the AnnotationKit passed to File::Export.
		 *	\return A AnnotationKit with the default settings. */
		static AnnotationKit	GetDefault();

		/*! Copies the source AnnotationKit into this AnnotationKit.
		 * 	\param in_kit The source AnnotationKit to copy. */
		void					Set(AnnotationKit const & in_kit);

		/*! Copies this AnnotationKit into the given AnnotationKit.
		 * 	\param out_kit The AnnotationKit to populate with the contents of this AnnotationKit. */
		void					Show(AnnotationKit & out_kit) const;

		/*! Copies the source AnnotationKit into this AnnotationKit.
		 * 	\param in_kit The source AnnotationKit to copy.
		 * 	\return A reference to this AnnotationKit. */
		AnnotationKit &			operator=(AnnotationKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this AnnotationKit.
		 *	\param in_kit An rvalue reference to an AnnotationKit to take the underlying object from.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			operator=(AnnotationKit && in_kit);

		/*! Indicates whether this AnnotationKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this AnnotationKit, <span class='code'>false</span> otherwise. */
		bool					Empty() const;

		/*!	Check if the source AnnotationKit is equivalent to this AnnotationKit. 
		 *	\param in_kit The source AnnotationKit to compare to this AnnotationKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(AnnotationKit const & in_kit) const;

		/*!	Check if the source AnnotationKit is equivalent to this AnnotationKit. 
		 *	\param in_kit The source AnnotationKit to compare to this AnnotationKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(AnnotationKit const & in_kit) const;

		/*!	Check if the source AnnotationKit is not equivalent to this AnnotationKit. 
		 *	\param in_kit The source AnnotationKit to compare to this AnnotationKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(AnnotationKit const & in_kit) const;


		/*! Sets the source for the annotation to Annotation::SourceType::CADModel.  
		 *  This is the CADModel which contains the 3D data to be exported into the annotation
		 *	\param in_cad_model The source CADModel for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetSource(CADModel const & in_cad_model);

		/*! Sets the source for the annotation to Annotation::SourceType::KeyPaths.  
		 *  These are the KeyPaths that will be used to locate the 3D data for the annotation.
		 *	\param in_sources The source KeyPaths for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetSource(KeyPathArray const & in_sources);

		/*! Sets the source for the annotation to Annotation::SourceType::KeyPaths.
		 *  These are the KeyPaths that will be used to locate the 3D data for the annotation.
		 *	\param in_count The number of elements in in_sources
		 *	\param in_sources The source KeyPaths for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetSource(size_t in_count, KeyPath const in_sources[]);

		/*! Sets the source for the annotation to Annotation::SourceType::KeyPaths.
		 *  The Key Path that will be used to locate the 3D data for the annotation.
		 *	\param in_source The source KeyPath for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetSource(KeyPath const & in_source);

		/*! Sets the source for the annotation to Annotation::SourceType::CADModelAndKeyPaths.
		 *  These are the CADModel which will be used to export 3D data to the annotation, and the KeyPaths
		 *  which will be added to the model
		 *	\param in_cad_model The source CADModel for the annotation.
		 *	\param in_additional_sources The additional source KeyPaths for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetSource(CADModel const & in_cad_model, KeyPathArray const & in_additional_sources);

		/*! Sets the source for the annotation to Annotation::SourceType::CADModelAndKeyPaths.
		 *  These are the CADModel which will be used to export 3D data to the annotation, and the KeyPaths
		 *  which will be added to the model
		 *	\param in_cad_model The source CADModel for the annotation.
		 *	\param in_count The number of elements in in_additional_sources
		 *	\param in_additional_sources The additional source KeyPaths for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetSource(CADModel const & in_cad_model, size_t in_count, KeyPath const in_additional_sources []);

		/*! Sets the format of the 3D data for the annotation.
		 *	\param in_format The format of the 3D data for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetFormat(Annotation::Format in_format);

		/*! Sets whether to compress tessellation for a PRC annotation.  This setting is only relevant if the format of the annotation is Annotation::Format::PRC.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#ae1cf4104a5eda56ce958e94ee1c1c42c">A3DRWParamsExportPrcData::m_bCompressTessellation</a>.
		 *	\param in_state Whether to compress tessellation for a PRC annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetPRCTessellationCompression(bool in_state);

		/*! Sets the level of compression to apply to BRep data for a PRC annotation.  This setting is only relevant if the format of the annotation is Annotation::Format::PRC.
		 *	This corresponds to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#afbbe41721d6878768a624ea8d0894531">A3DRWParamsExportPrcData::m_bCompressBrep</a> and <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#afd35a9e2bea37b388227b76e14f49daa">A3DRWParamsExportPrcData::m_eCompressBrepType</a>.
		 *	\param in_level The level of compression to apply to BRep data for a PRC annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetPRCBRepCompression(PRC::BRepCompression in_level);

		/*! Sets whether to remove the BRep information in the PRC data for this annotation.  This setting is only relevant if the format of the annotation is Annotation::Format::PRC.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#a7a61340a38925e02b30cb67f39b1fcd4">A3DRWParamsExportPrcData::m_bRemoveBRep</a>.
		 *	\param in_state Whether to remove BRep information in the PRC data for this annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetPRCBRepRemoval(bool in_state);

		/*! Sets whether to remove the attribute information in the PRC data for this annotation.  This setting is only relevant if the format of the annotation is Annotation::Format::PRC.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_prc_data.html#aa8435b8348b8d6f1c11f1bc15313ed47">A3DRWParamsExportPrcData::m_bRemoveAttributes</a>.
		 *	\param in_state Whether to remove the attribute information in the PRC data for this annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetPRCAttributeRemoval(bool in_state);

		/*! Sets the version of the data for a U3D annotation.  This setting is only relevant if the format of the annotation is Annotation::Format::U3D.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_u3_d_data.html#ae7d0446b90139bee7ccf7d0eef4a7204">A3DRWParamsExportU3DData::m_eU3DVersion</a>.
		 *	\param in_version The version of the data for a U3D annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetU3DVersion(U3D::Version in_version);

		/*! Sets the level of compression to apply to tessellations for a U3D annotation.  If no compression level is set, no compression will be applied.  This setting is only relevant if the format of the annotation is Annotation::Format::U3D.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_r_w_params_export_u3_d_data.html#ae9321a865749efe613aa1d141dff426b">A3DRWParamsExportU3DData::m_ucMeshQualityValue</a>.
		 *	\param in_level The level of compression to apply to tessellations for a U3D annotation.  This value should be in the range <span class='code'>[0, 100]</span>.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetU3DCompression(unsigned char in_level);

		/*! Sets the name for the annotation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a8d289e535b78fd163cc20db1993a4d94">A3DPDF3DAnnotData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetName(char const * in_name);

		/*! Sets the artwork for the annotation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#af18f6ede1a64e78217d550f1fd8296a0">A3DPDF3DAnnotData::m_p3DArtwork</a>.
		 *	\param in_artwork The artwork for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetArtwork(ArtworkKit const & in_artwork);

		/*! Sets when the annotation is activated.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a984cbf6fbf923367efc14693539a77fa">A3DPDF3DAnnotData::m_eActivateWhen</a>.
		 *	\param in_when When the annotation is activated.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetActivation(Activation::When in_when);

		/*! Sets when the annotation is deactivated.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a2e9c3a5c377dbec3da13497bf0c77a7c">A3DPDF3DAnnotData::m_eDesactiveWhen</a>.
		 *	\param in_when When the annotation is deactivated.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetDeactivation(Deactivation::When in_when);

		/*! Sets whether to display the toolbar when the annotation is activated.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a6713e01011a2440fb0b497174bf356f2">A3DPDF3DAnnotData::m_bShowToolbar</a>.
		 *	\param in_state Whether to display the toolbar when the annotation is activated.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetToolbarVisibility(bool in_state);

		/*! Sets whether to display the model tree when the annotation is activated.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a630c6fdbe19fd64733d9a4870f087d71">A3DPDF3DAnnotData::m_bOpenModelTree</a>.
		 *	\param in_state Whether to display the model tree when the annotation is activated.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetModelTreeVisibility(bool in_state);

		/*! Sets whether to enable interactivity on the annotation.  This corresponds to the (negated) value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#aead487a8b5b64330bfe6481a14ab23cc">A3DPDF3DAnnotData::m_bDisableInteractivity</a>.
		 *	\param in_state Whether to enable interactivity on the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetInteractivity(bool in_state);

		/*! Sets the border width in points for the annotation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a5db0afe2ae549413bf18032bdaf6f3cf">A3DPDF3DAnnotData::m_iAppearanceBorderWidth</a>.
		 *	\param in_width The border width in points for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetBorderWidth(int in_width);

		/*! Sets whether the annotation background is transparent.  This will render background color irrelevant and has negative <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a447c6ec9653b67dda1dce97f0484d69d">ramifications</a>.
		 *	This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a447c6ec9653b67dda1dce97f0484d69d">A3DPDF3DAnnotData::m_bTransparentBackground</a>.
		 *	\param in_state Whether the annotation background is transparent.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetTransparentBackground(bool in_state);

		/*! Sets the background color for the annotation.  Unused if there is a transparent background.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a7c94d67d1af04a1c8ca0a448f7359c8f">A3DPDF3DAnnotData::m_sBackgroundColor</a>.
		 *	\param in_color The background color for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetBackgroundColor(RGBColor const & in_color);

		/*! Sets the lighting mode for the annotation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#a82fa249186ff5bc4027b81252339fd7e">A3DPDF3DAnnotData::m_eLighting</a>.
		 *	\param in_mode The lighting mode for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetLighting(Lighting::Mode in_mode);

		/*! Sets the rendering mode for the annotation.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#aa2a03514e90a430f594fc7ece0dcd820">A3DPDF3DAnnotData::m_eRenderingStyle</a>.
		 *	\param in_mode The rendering mode for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetRendering(Rendering::Mode in_mode);

		/*! Sets the poster image for the annotation.  If this is not specified a poster image will be generated by Publish.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_annot_data.html#aebe3e915ab9a18a3068756a9c0677a6d">A3DPDF3DAnnotData::m_pPosterImage</a>.
		 *	\param in_image The poster image for the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetPosterImage(ImageKit const & in_image);

		/*! Sets the carousel buttons for the annotation.  This must be specified for a view carousel to be generated.  This corresponds to the button names that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeb2b809a03a348e88808edb5864542ba">A3DPDFDefineViewCarousel</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_button_names UTF8-encoded names for the view buttons in the carousel.
		 *	\param in_previous_button_name UTF8-encoded name for the previous button.
		 *	\param in_next_button_name UTF8-encoded name for the next button.
		 *	\param in_scroll_size Number of buttons to scroll when the previous or next buttons are pressed.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetCarouselButtons(size_t in_count, UTF8 const in_button_names[], char const * in_previous_button_name, char const * in_next_button_name, size_t in_scroll_size);

		/*! Sets the carousel buttons for the annotation.  This must be specified for a view carousel to be generated.  This corresponds to the button names that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeb2b809a03a348e88808edb5864542ba">A3DPDFDefineViewCarousel</a>.
		 *	\param in_button_names Names for the view buttons in the carousel.
		 *	\param in_previous_button_name UTF8-encoded name for the previous button.
		 *	\param in_next_button_name UTF8-encoded name for the next button.
		 *	\param in_scroll_size Number of buttons to scroll when the previous or next buttons are pressed.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			SetCarouselButtons(UTF8Array const & in_button_names, char const * in_previous_button_name, char const * in_next_button_name, size_t in_scroll_size);

		/*! Sets the carousel views for the annotation.  This can be optionally specified for a view carousel.  This corresponds to the views and images that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeb2b809a03a348e88808edb5864542ba">A3DPDFDefineViewCarousel</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the views on the associated ArtworkKit to use for the view carousel.
		 *	\param in_images Images to use for the view carousel buttons.  If unspecified, images will be generated from the views themselves.
		 *	\return A reference to this AnnotationKit.
		 *	\sa SetArtwork
		 *	\sa ArtworkKit::SetViews */
		AnnotationKit &			SetCarouselViews(size_t in_count, size_t const in_indices[], ImageKit const in_images[] = nullptr);

		/*! Sets the carousel views for the annotation.  This can be optionally specified for a view carousel.  This corresponds to the views and images that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeb2b809a03a348e88808edb5864542ba">A3DPDFDefineViewCarousel</a>.
		 *	\param in_indices Indices of the views on the associated ArtworkKit to use for the view carousel.
		 *	\param in_images Images to use for the view carousel buttons.  If unspecified, images will be generated from the views themselves.
		 *	\return A reference to this AnnotationKit.
		 *	\sa SetArtwork
		 *	\sa ArtworkKit::SetViews */
		AnnotationKit &			SetCarouselViews(SizeTArray const & in_indices, ImageKitArray const & in_images = ImageKitArray());

		/*! Creates a Parts List contained in a scrollable table.
		 *  The table will have three columns, listing part number, part name and quantity.
		 *  The Parts List can only be calculated if the Annotation source is a CADModel, and the annotation format is PRC.
		 *  \param in_rows The number of rows for the table housing the Parts List (including the header row)
		 *  \param in_location The location of the table housing the Parts List, specified in points relative to bottom left of the page.  All fields in the location will be used.
		 *  \param in_headers The strings displayed in the column headers. The size of this array should be 3. 
		    The first column lists the part numbers. The second column lists the part name. The third column lists the part quantity.
			Leaving it unspecified will result in a table with no headers.
		 *  \param in_columns An array of three percentages, specifying the width of each of the table's columns as a percentage of the table width.
			The total of the values contained in in_columns should total 1.0.
			For example, if the columns should take up 10%, 80% and 10% of the table width respectively, in_column will contain 0.1, 0.8, 0.1
			Leaving in_columns unspecified will use the default column widths.
		 *  \param in_text_style The text style used for the cells of the table. If not specified, default formatting is used.
		 *  \param in_header_text_style The text style used for the header of the table. If not specified, default formatting is used.
		 *  \return A reference to this AnnotationKit. 
		*   \warning The TableToPDF Publish add-on must be installed in order to use this feature. */
		AnnotationKit &			SetPartsList(size_t in_rows, IntRectangle const & in_location, UTF8Array const & in_headers = UTF8Array(), FloatArray const & in_columns = FloatArray(), TextFieldKit const & in_text_style = TextFieldKit(), TextFieldKit const & in_header_text_style = TextFieldKit());

#ifndef HPS_GULP_CLANG
		/*! Creates a Parts List contained in a scrollable table.
		*  The table will have three columns, listing part number, part name and quantity.
		*  The Parts List can only be calculated if the Annotation source is a CADModel, and the annotation format is PRC.
		*  \param in_rows The number of rows for the table housing the Parts List (including the header row)
		*  \param in_location The location of the table housing the Parts List, specified in points relative to bottom left of the page.  All fields in the location will be used.
		*  \param in_headers The strings displayed in the column headers.
			The first column lists the part numbers. The second column lists the part name. The third column lists the part quantity.
			Leaving it unspecified will result in a table with no headers.
		*  \param in_columns An array of three percentages, specifying the width of each of the table's columns as a percentage of the table width.
			The total of the values contained in in_columns should total 1.0.
			For example, if the columns should take up 10%, 80% and 10% of the table width respectively, in_column will contain 0.1, 0.8, 0.1
			Leaving in_columns unspecified will use the default column widths.
		*  \param in_text_style The text style used for the cells of the table. If not specified, default formatting is used.
		*  \param in_header_text_style The text style used for the header of the table. If not specified, default formatting is used.
		*  \return A reference to this AnnotationKit.
		*  \warning The TableToPDF Publish add-on must be installed in order to use this feature. */
		AnnotationKit &			SetPartsList(size_t in_rows, IntRectangle const & in_location, char const * in_headers[3], float const in_columns[3], TextFieldKit const & in_text_style = TextFieldKit(), TextFieldKit const & in_header_text_style = TextFieldKit());
#endif
		/*! Creates a View List Box which contains a list of the Views contained in the source file.
		 * Clicking on the entries of the list will activate the associated View.
		 * The View List can only be calculated if the Annotation source is a CADModel, and the annotation format is PRC.
		 * \param in_field UTF8-encoded form field name for the View List.
		 * \return A reference to this AnnotationKit. */
		AnnotationKit &			SetViewListByField(char const * in_field);

		/*! Creates a View List Box which contains a list of the Views contained in the source file.
		* Clicking on the entries of the list will activate the associated View.
		* The View List can only be calculated if the Annotation source is a CADModel, and the annotation format is PRC.
		*  \param in_location The location of the View List, specified in points relative to bottom left of the page.  All fields in the location will be used.
		*  \param in_list The ListBoxKit used to specify the attributes of the View List.
		* \return A reference to this AnnotationKit. */
		AnnotationKit &			SetViewList(IntRectangle const & in_location, HPS::Publish::ListBoxKit const & in_list);

		/*! Creates a PMI List Box which contains a list of the PMI contained in the source file.
		* Clicking on the entries of the list will highlight the associated PMI in the 3D annotation.
		* The PMI List can only be calculated if the Annotation source is a CADModel, and the annotation format is PRC.
		* If a View List or a View Carousel are also set on this annotation kit, the PMI List will only show the PMIs which are visible in the View you currently have selected in the PDF.
		* \param in_field UTF8-encoded form field name for the PMI List.
		* \return A reference to this AnnotationKit. */
		AnnotationKit &			SetPMIListByField(char const * in_field);

		/*! Creates a PMI List Box which contains a list of the PMI contained in the source file.
		* Clicking on the entries of the list will highlight the associated PMI in the 3D annotation.
		* The PMI List can only be calculated if the Annotation source is a CADModel, and the annotation format is PRC.
		* If a View List or a View Carousel are also set on this annotation kit, the PMI List will only show the PMIs which are visible in the View you currently have selected in the PDF.
		* \param in_location The location of the PMI List, specified in points relative to bottom left of the page.  All fields in the location will be used.
		* \param in_list The ListBoxKit used to specify the attributes of the PMI List.
		* \return A reference to this AnnotationKit. */
		AnnotationKit &			SetPMIList(IntRectangle const & in_location, HPS::Publish::ListBoxKit const & in_list);

		/*! Removes the source setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetSource();

		/*! Removes the format setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetFormat();

		/*! Removes the tessellation compression for PRC data setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetPRCTessellationCompression();

		/*! Removes the BRep compression level for PRC data setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetPRCBRepCompression();

		/*! Removes the BRep removal for PRC data setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetPRCBRepRemoval();

		/*! Removes the attribute removal for PRC data setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetPRCAttributeRemoval();

		/*! Removes the version for U3D data setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetU3DVersion();

		/*! Removes the compression level for U3D data setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetU3DCompression();

		/*! Removes the name setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetName();

		/*! Removes the artwork setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetArtwork();

		/*! Removes the activation setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetActivation();

		/*! Removes the deactivation setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetDeactivation();

		/*! Removes the toolbar visibility setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetToolbarVisibility();

		/*! Removes the model tree visibility setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetModelTreeVisibility();

		/*! Removes the interactivity setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetInteractivity();

		/*! Removes the border width setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetBorderWidth();

		/*! Removes the transparent background setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetTransparentBackground();

		/*! Removes the background color setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetBackgroundColor();

		/*! Removes the lighting mode setting.a
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetLighting();

		/*! Removes the rendering mode setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetRendering();

		/*! Removes the poster image setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetPosterImage();

		/*! Removes the carousel buttons setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetCarouselButtons();

		/*! Removes the carousel views setting.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetCarouselViews();

		/*! Removes the Parts List setting.
		*   \return A reference to this AnnotationKit */
		AnnotationKit &			UnsetPartsList();

		/*! Removes the View List setting.
		*   \return A reference to this AnnotationKit */
		AnnotationKit &			UnsetViewList();

		/*! Removes the PMI List setting.
		*   \return A reference to this AnnotationKit */
		AnnotationKit &			UnsetPMIList();

		/*! Removes all data on the annotation.
		 *	\return A reference to this AnnotationKit. */
		AnnotationKit &			UnsetEverything();


		/*! Shows the source setting.
		 *	\param out_source_type Specifies which one of the previous arguments is valid
		 *	\param out_cad_model The source CADModel for the annotation. Only valid if out_source_type is CADModel or CADModelAndKeyPath
		 *	\param out_key_paths The source KeyPath array for the annotation. Only valid if out_source_type is KeyPath or CADModelAndKeyPath
		 *	\return <span class='code'>true</span> if a source was specified, <span class='code'>false</span> otherwise. */
		bool					ShowSource(Annotation::SourceType & out_source_type, CADModel & out_cad_model, KeyPathArray & out_key_paths) const;

		/*! Shows the format setting.
		 *	\param out_format The format of the 3D data for the annotation.
		 *	\return <span class='code'>true</span> if a format was specified, <span class='code'>false</span> otherwise. */
		bool					ShowFormat(Annotation::Format & out_format) const;

		/*! Shows the tessellation compression for PRC data setting.
		 *	\param out_state Whether to compress tessellation for a PRC annotation.
		 *	\return <span class='code'>true</span> if a PRC tessellation compression setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPRCTessellationCompression(bool & out_state) const;

		/*! Shows the BRep compression level for PRC data setting.
		 *	\param out_level The level of compression to apply to BRep data for a PRC annotation.
		 *	\return <span class='code'>true</span> if a PRC BRep compression level setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPRCBRepCompression(PRC::BRepCompression & out_level) const;

		/*! Shows the BRep removal for PRC data setting.
		 *	\param out_state Whether to remove BRep information in the PRC data for this annotation.
		 *	\return <span class='code'>true</span> if a BRep removal setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPRCBRepRemoval(bool & out_state) const;

		/*! Shows the attribute removal for PRC data setting.
		 *	\param out_state Whether to remove attribute information in the PRC data for this annotation.
		 *	\return <span class='code'>true</span> if an attribute removal setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPRCAttributeRemoval(bool & out_state) const;

		/*! Shows the version for U3D data setting.
		 *	\param out_version The version of the data for a U3D annotation.
		 *	\return <span class='code'>true</span> if a U3D version setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowU3DVersion(U3D::Version & out_version) const;

		/*! Shows the compression level for U3D data for the annotation.
		 *	\param out_level The level of compression to apply to tessellations for a U3D annotation.
		 *	\return <span class='code'>true</span> if a U3D compression setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowU3DCompression(unsigned char & out_level) const;

		/*! Shows the name setting.
		 *	\param out_name The name for the annotation.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool					ShowName(UTF8 & out_name) const;

		/*! Shows the artwork setting.
		 *	\param out_artwork The artwork for the annotation.
		 *	\return <span class='code'>true</span> if an artwork was specified, <span class='code'>false</span> otherwise. */
		bool					ShowArtwork(ArtworkKit & out_artwork) const;

		/*! Shows the activation setting.
		 *	\param out_when When the annotation is activated.
		 *	\return <span class='code'>true</span> if an activation setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowActivation(Activation::When & out_when) const;

		/*! Shows the deactivation setting.
		 *	\param out_when When the annotation is deactivated.
		 *	\return <span class='code'>true</span> if a deactivation setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowDeactivation(Deactivation::When & out_when) const;

		/*! Shows the toolbar visibility setting.
		 *	\param out_state Whether to display the toolbar when the annotation is activated.
		 *	\return <span class='code'>true</span> if a toolbar visibility was specified, <span class='code'>false</span> otherwise. */
		bool					ShowToolbarVisibility(bool & out_state) const;

		/*! Shows the model tree visibility setting.
		 *	\param out_state Whether to display the model tree when the annotation is activated.
		 *	\return <span class='code'>true</span> if a model tree visibility was specified, <span class='code'>false</span> otherwise. */
		bool					ShowModelTreeVisibility(bool & out_state) const;

		/*! Shows the interactivity setting.
		 *	\param out_state Whether to enable interactivity on the annotation.
		 *	\return <span class='code'>true</span> if an interactivity setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowInteractivity(bool & out_state) const;

		/*! Shows the border width setting.
		 *	\param out_width The border width of the annotation.
		 *	\return <span class='code'>true</span> if a border width was specified, <span class='code'>false</span> otherwise. */
		bool					ShowBorderWidth(int & out_width) const;

		/*! Shows the transparent background setting.
		 *	\param out_state Whether the annotation has a transparent background.
		 *	\return <span class='code'>true</span> if a transparent background setting specified, <span class='code'>false</span> otherwise. */
		bool					ShowTransparentBackground(bool & out_state) const;

		/*! Shows the background color setting.
		 *	\param out_color The background color for the annotation.
		 *	\return <span class='code'>true</span> if a background color was specified, <span class='code'>false</span> otherwise. */
		bool					ShowBackgroundColor(RGBColor & out_color) const;

		/*! Shows the lighting mode setting.
		 *	\param out_mode The lighting mode for the annotation.
		 *	\return <span class='code'>true</span> if a lighting mode was specified, <span class='code'>false</span> otherwise. */
		bool					ShowLighting(Lighting::Mode & out_mode) const;

		/*! Shows the rendering mode setting.
		 *	\param out_mode The rendering mode for the annotation.
		 *	\return <span class='code'>true</span> if a rendering mode was specified, <span class='code'>false</span> otherwise. */
		bool					ShowRendering(Rendering::Mode & out_mode) const;

		/*! Shows the poster image setting.
		 *	\param out_image The poster image for the annotation.
		 *	\return <span class='code'>true</span> if a poster image was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPosterImage(ImageKit & out_image) const;

		/*! Shows the carousel buttons setting.
		 *	\param out_button_names Names for the view buttons in the carousel.
		 *	\param out_previous_button_name Name for the previous button.
		 *	\param out_next_button_name Name for the next button.
		 *	\param out_scroll_size Numbers of buttons scrolled when the previous or next buttons are pressed.
		 *	\return <span class='code'>true</span> if carousel buttons were specified, <span class='code'>false</span> otherwise. */
		bool					ShowCarouselButtons(UTF8Array & out_button_names, UTF8 & out_previous_button_name, UTF8 & out_next_button_name, size_t & out_scroll_size) const;

		/*! Shows the carousel views setting.
		 *	\param out_indices Indices of the views on the associated ArtworkKit to use for the view carousel.
		 *	\param out_images Images to use for the view carousel buttons.  May be empty, even if <span class='code'>out_indices</span> is not.
		 *	\return <span class='code'>true</span> if carousel views were specified, <span class='code'>false</span> otherwise. */
		bool					ShowCarouselViews(SizeTArray & out_indices, ImageKitArray & out_images) const;

		/*! Shows the Parts List setting.
		*	\param out_rows The number of rows making up the Parts List table.
		*   \param out_location The location of the Parts List table.
		*	\param out_headers The strings of the column headers. Empty if the table has no header.
		*	\param out_columns The width of the three columns as percentages of the table width.
		*   \param out_text_style The text styles used for the cells in the table.
		*   \param out_header_text_style The text styles used for the header of the table.
		*	\return <span class='code'>true</span> if a Parts List was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPartsList(size_t & out_rows, IntRectangle & out_location, UTF8Array & out_headers, FloatArray & out_columns, TextFieldKit & out_text_style, TextFieldKit & out_header_text_style) const;

		/*! Shows the View List setting.
		*	\param out_field The list box form field name on the page which will be used for the View List.
		*	\return <span class='code'>true</span> if a View List was specified by field, <span class='code'>false</span> otherwise. */
		bool					ShowViewListField(UTF8 & out_field) const;

		/*! Shows the View List setting.
		*   \param out_location The location of the View List table.
		*	\param out_list The options specified for the list box containing the View List
		*	\return <span class='code'>true</span> if a View List was specified, <span class='code'>false</span> otherwise. */
		bool					ShowViewList(IntRectangle & out_location, Publish::ListBoxKit & out_list) const;

		/*! Shows the PMI List setting.
		*	\param out_field The list box form field name on the page which will be used for the PMI List.
		*	\return <span class='code'>true</span> if a PMI List was specified by field, <span class='code'>false</span> otherwise. */
		bool					ShowPMIListField(UTF8 & out_field) const;

		/*! Shows the PMI List setting.
		*   \param out_location The location of the PMI List table.
		*	\param out_list The options specified for the list box containing the PMI List
		*	\return <span class='code'>true</span> if a PMI List was specified, <span class='code'>false</span> otherwise. */
		bool					ShowPMIList(IntRectangle & out_location, Publish::ListBoxKit & out_list) const;
	};

	/*! The ArtworkKit class is a user space object.  It acts as the container for all data that can be on an artwork in a Publish PDF. */
	class PUBLISH_API ArtworkKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ArtworkKit object. */
		ArtworkKit();

		/*! The copy constructor creates a new ArtworkKit object that contains the same settings as the source ArtworkKit.
		 * 	\param in_kit The source ArtworkKit to copy. */
		ArtworkKit(ArtworkKit const & in_kit);

		/*! The move constructor creates an ArtworkKit by transferring the underlying object of the rvalue reference to this ArtworkKit.
		 * 	\param in_kit An rvalue reference to an ArtworkKit to take the underlying object from. */
		ArtworkKit(ArtworkKit && in_kit);

		virtual ~ArtworkKit();

	static const HPS::Type staticType = HPS::Type::PublishArtworkKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a ArtworkKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for export unless a setting is overridden by the ArtworkKit passed to File::Export.
		 *	\return A ArtworkKit with the default settings. */
		static ArtworkKit	GetDefault();

		/*! Copies the source ArtworkKit into this ArtworkKit.
		 * 	\param in_kit The source ArtworkKit to copy. */
		void				Set(ArtworkKit const & in_kit);

		/*! Copies this ArtworkKit into the given ArtworkKit.
		 * 	\param out_kit The ArtworkKit to populate with the contents of this ArtworkKit. */
		void				Show(ArtworkKit & out_kit) const;

		/*! Copies the source ArtworkKit into this ArtworkKit.
		 * 	\param in_kit The source ArtworkKit to copy.
		 * 	\return A reference to this ArtworkKit. */
		ArtworkKit &		operator=(ArtworkKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ArtworkKit.
		 *	\param in_kit An rvalue reference to an ArtworkKit to take the underlying object from.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		operator=(ArtworkKit && in_kit);

		/*! Indicates whether this ArtworkKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ArtworkKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source ArtworkKit is equivalent to this ArtworkKit. 
		 *	\param in_kit The source ArtworkKit to compare to this ArtworkKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(ArtworkKit const & in_kit) const;

		/*!	Check if the source ArtworkKit is equivalent to this ArtworkKit. 
		 *	\param in_kit The source ArtworkKit to compare to this ArtworkKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(ArtworkKit const & in_kit) const;

		/*!	Check if the source ArtworkKit is not equivalent to this ArtworkKit. 
		 *	\param in_kit The source ArtworkKit to compare to this ArtworkKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(ArtworkKit const & in_kit) const;


		/*! Sets the animation style for the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#a089d0a8180bf79e10b7cdb073057af4f">A3DPDF3DArtworkData::m_eAnimationStyle</a>.
		 *	\param in_style The animation style for the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetAnimationStyle(Animation::Style in_style);

		/*! Sets the JavaScript source or file for the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#a69830b86e02e28fdb07c6d5bbda2db90">A3DPDF3DArtworkData::m_pcJavaScriptFileName</a>.
		 *	\param in_source The JavaScript source or file (depending on the next argument) for the artwork.
		 *	\param in_type The type of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetJavaScript(char const * in_source, Source::Type in_type = Source::Type::Code);

		/*!	Sets the whether to enable PMI cross-highlighting for the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#a57f639f84cc5d856af6cc99b61d219b6">A3DPDF3DArtworkData::m_bActivatePMICrossHighlight</a>.
		 *	\param in_state Whether to highlight the geometry associated with a piece of PMI when that PMI is selected.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetPMICrossHighlighting(bool in_state);

		/*!	Sets the whether to include semantic PMI information in the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#a3fd0cdd9fbd980c695013b97f16a3c4f">A3DPDF3DArtworkData::m_bAddPMISemanticInformation</a>.
		 *	\param in_state Whether to include semantic PMI information in the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetPMISemanticInformation(bool in_state);

		/*!	Sets the default view preference for the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#afe11fda891b6db864619a756e1ad2d9e">A3DPDF3DArtworkData::m_bKeepNativeDefaultView</a>.
		 *	\param in_preference The default view preference for the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetDefaultViewPreference(Artwork::View::Preference in_preference);

		/*!	Sets the display preference for the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#a16f183ec39e59f7ae82a28ac7a228c2f">A3DPDF3DArtworkData::m_bUseRuntimeDisplaySettings</a>.
		 *	\param in_preference The display preference for the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetDisplayPreference(Artwork::Display::Preference in_preference);

		/*! Adds a view to the artwork.  This will replace all views (if any) on the artwork.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dartwork__module.html#gaba259ad2983a5e64ca6dcf5fe4415d71">A3DPDF3DArtworkAddView</a>.
		 *	\param in_view The view to add to the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetView(ViewKit const & in_view);

		/*! Adds a view to the artwork.  This will append the view to any existing views on the artwork.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dartwork__module.html#gaba259ad2983a5e64ca6dcf5fe4415d71">A3DPDF3DArtworkAddView</a>.
		 *	\param in_view The view to add to the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		AddView(ViewKit const & in_view);

		/*! Adds views to the artwork.  This will replace all views (if any) on the artwork.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dartwork__module.html#gaba259ad2983a5e64ca6dcf5fe4415d71">A3DPDF3DArtworkAddView</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_views The views to add to the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetViews(size_t in_count, ViewKit const in_views[]);

		/*! Adds views to the artwork.  This will replace all views (if any) on the artwork.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dartwork__module.html#gaba259ad2983a5e64ca6dcf5fe4415d71">A3DPDF3DArtworkAddView</a>.
		 *	\param in_views The views to add to the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetViews(ViewKitArray const & in_views);

		/*! Adds views to the artwork.  This will append the views to any existing views on the artwork.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dartwork__module.html#gaba259ad2983a5e64ca6dcf5fe4415d71">A3DPDF3DArtworkAddView</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_views The views to add to the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		AddViews(size_t in_count, ViewKit const in_views[]);

		/*! Adds views to the artwork.  This will append the views to any existing views on the artwork.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__3_dartwork__module.html#gaba259ad2983a5e64ca6dcf5fe4415d71">A3DPDF3DArtworkAddView</a>.
		 *	\param in_views The views to add to the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		AddViews(ViewKitArray const & in_views);

		/*! Sets the PMI color for this artwork. This correspond to value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f3_d_artwork_data.html#a3aeb63d2227167cb586c87471844e712">A3DPDF3DArtworkData::m_sPMIColor </a>.
		 *	\param in_pmi_color The color to apply to PMIs for this artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		SetPMIColor(HPS::RGBColor const & in_pmi_color);


		/*! Removes the animation style setting.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetAnimationStyle();

		/*! Removes the JavaScript setting.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetJavaScript();

		/*! Removes the PMI cross-highlighting setting.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetPMICrossHighlighting();

		/*! Removes the PMI semantic information setting.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetPMISemanticInformation();

		/*! Removes the default view preference setting.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetDefaultViewPreference();

		/*! Removes the display preference setting.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetDisplayPreference();

		/*! Removes the view at the given index from the artwork.
		 *	\param in_index Index of the view to remove from the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetView(size_t in_index);

		/*! Removes the views at the given indices from the artwork.
		 *	\param in_count Size of the following array.
		 *	\param in_indices Indices of the views to remove from the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetViews(size_t in_count, size_t const in_indices[]);

		/*! Removes the views at the given indices from the artwork.
		 *	\param in_indices Indices of the views to remove from the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetViews(SizeTArray const & in_indices);

		/*! Removes all views from the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetViews();

		/*! Removes the PMI color setting from the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetPMIColor();

		/*! Removes all data from the artwork.
		 *	\return A reference to this ArtworkKit. */
		ArtworkKit &		UnsetEverything();


		/*! Shows the animation style setting.
		 *	\param out_style The animation style of the artwork.
		 *	\return <span class='code'>true</span> if an animation style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowAnimationStyle(Animation::Style & out_style) const;

		/*! Shows the JavaScript setting.
		 *	\param out_source The JavaScript source or file (depending on the next argument) for the artwork.
		 *	\param out_type The type of the preceding argument.
		 *	\return <span class='code'>true</span> if a JavaScript source or file was specified, <span class='code'>false</span> otherwise. */
		bool				ShowJavaScript(UTF8 & out_source, Source::Type & out_type) const;

		/*! Shows the PMI cross-highlighting setting.
		 *	\param out_state Whether to highlight the geometry associated with a piece of PMI when that PMI is selected.
		 *	\return <span class='code'>true</span> if a PMI cross-highlighting setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPMICrossHighlighting(bool & out_state) const;

		/*! Shows the semantic PMI information setting.
		 *	\param out_state Whether to include semantic PMI information in the artwork.
		 *	\return <span class='code'>true</span> if a semantic PMI information setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPMISemanticInformation(bool & out_state) const;

		/*! Shows the default view preference setting.
		 *	\param out_preference The default view preference for the artwork.
		 *	\return <span class='code'>true</span> if a default view preference was specified, <span class='code'>false</span> otherwise. */
		bool				ShowDefaultViewPreference(Artwork::View::Preference & out_preference) const;

		/*! Shows the display preference setting.
		 *	\param out_preference The display preference for the artwork.
		 *	\return <span class='code'>true</span> if a display preference was specified, <span class='code'>false</span> otherwise. */
		bool				ShowDisplayPreference(Artwork::Display::Preference & out_preference) const;

		/*! Shows the views for the artwork.
		 *	\param out_views The views for the artwork.
		 *	\return <span class='code'>true</span> if views were specified, <span class='code'>false</span> otherwise. */
		bool				ShowViews(ViewKitArray & out_views) const;

		/*! Shows the PMI color for the artwork.
		 *	\param out_pmi_color The PMI color for the artwork.
		 *	\return <span class='code'>true</span> if a PMI color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPMIColor(HPS::RGBColor & out_pmi_color) const;
	};

	/*! The ViewKit class is a user space object.  It acts as the container for all data that can be set on a view in a Publish PDF. */
	class PUBLISH_API ViewKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ViewKit object. */
		ViewKit();

		/*! The copy constructor creates a new ViewKit object that contains the same settings as the source ViewKit.
		 * 	\param in_kit The source ViewKit to copy. */
		ViewKit(ViewKit const & in_kit);

		/*! The move constructor creates a ViewKit by transferring the underlying object of the rvalue reference to this ViewKit.
		 * 	\param in_kit An rvalue reference to a ViewKit to take the underlying object from. */
		ViewKit(ViewKit && in_kit);

		virtual ~ViewKit();

	static const HPS::Type staticType = HPS::Type::PublishViewKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a ViewKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for export unless a setting is overridden by the ViewKit passed to File::Export.
		 *	\return A ViewKit with the default settings. */
		static ViewKit		GetDefault();

		/*! Copies the source ViewKit into this ViewKit.
		 * 	\param in_kit The source ViewKit to copy. */
		void				Set(ViewKit const & in_kit);

		/*! Copies this ViewKit into the given ViewKit.
		 * 	\param out_kit The ViewKit to populate with the contents of this ViewKit. */
		void				Show(ViewKit & out_kit) const;

		/*! Copies the source ViewKit into this ViewKit.
		 * 	\param in_kit The source ViewKit to copy.
		 * 	\return A reference to this ViewKit. */
		ViewKit &			operator=(ViewKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ViewKit.
		 *	\param in_kit An rvalue reference to a ViewKit to take the underlying object from.
		 *	\return A reference to this ViewKit. */
		ViewKit &			operator=(ViewKit && in_kit);

		/*! Indicates whether this ViewKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ViewKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source ViewKit is equivalent to this ViewKit. 
		 *	\param in_kit The source ViewKit to compare to this ViewKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(ViewKit const & in_kit) const;

		/*!	Check if the source ViewKit is equivalent to this ViewKit. 
		 *	\param in_kit The source ViewKit to compare to this ViewKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(ViewKit const & in_kit) const;

		/*!	Check if the source ViewKit is not equivalent to this ViewKit. 
		 *	\param in_kit The source ViewKit to compare to this ViewKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(ViewKit const & in_kit) const;


		/*! Sets the internal name for the view.
		 *	\param in_name UTF8-encoded internal name for the view.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a7c24dce66db9041eb6411204268a7517">A3DPDFViewData::m_pcInternalName</a>.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetInternalName(char const * in_name);

		/*! Sets the external name for the view.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a3accdda36cd587f78f3e7ff3d85b9893">A3DPDFViewData::m_pcExternalName</a>.
		 *	\param in_name UTF8-encoded external name for the view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetExternalName(char const * in_name);

		/*! Sets the camera for the view.  This corresponds to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a9f79bebe0e2d6ac6d29fc253d1414b4f">A3DPDFViewData::m_sPosition</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#aa29602b2ea96c0ce793040deb2ba8fc5">A3DPDFViewData::m_sTarget</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a18bf7ccfc0451bf52b1cee32dbceab9b">A3DPDFViewData::m_sUpVector</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a00eae0ff4d0f145787406c729dc83c4b">A3DPDFViewData::m_dZoomFactor</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a472edf22d468eeb5e834b5fea777d75c">A3DPDFViewData::m_eProjectionMode</a>,
		 *	and <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#aa7056b8577c01d37bc5e9af4e008011a">A3DPDFViewData::m_dFieldOfView</a>.
		 *	\param in_camera The camera for the view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetCamera(CameraKit const & in_camera);

		/*! Sets whether this view is the default view.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a4d03a8c18f745487873eb3090da55537">A3DPDFViewData::m_bIsDefault</a>.
		 *	\param in_state Whether this view is the default view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetDefault(bool in_state);

		/*! Sets the background color for the view.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a42b3a961b62e86668dd6d40462e8c603">A3DPDFViewData::m_sViewBackgroundColor</a>.
		 *	\param in_color The background color for the view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetBackgroundColor(RGBColor const & in_color);

		/*! Sets the lighting mode for the view.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a1f60c93ea54c67cb0690bcd4bd2e9c89">A3DPDFViewData::m_eViewLighting</a>.
		 *	\param in_mode The lighting mode for the view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetLighting(Lighting::Mode in_mode);

		/*! Sets the rendering mode for the view.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_view_data.html#a0003ea03bb292482032038a017eebb8a">A3DPDFViewData::m_eRenderingStyle</a>.
		 *	\param in_mode The rendering mode for the view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			SetRendering(Rendering::Mode in_mode);


		/*! Removes the internal name setting.
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetInternalName();

		/*! Removes the external name setting.
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetExternalName();

		/*! Removes the camera setting.
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetCamera();

		/*! Removes the default setting.
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetDefault();

		/*! Removes the background color setting
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetBackgroundColor();

		/*! Removes the lighting mode setting
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetLighting();

		/*! Removes the rendering mode setting
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetRendering();

		/*! Removes all data from the view.
		 *	\return A reference to this ViewKit. */
		ViewKit &			UnsetEverything();


		/*! Shows the internal name setting.
		 *	\param out_name The internal name for the view.
		 *	\return <span class='code'>true</span> if an internal name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowInternalName(UTF8 & out_name) const;

		/*! Shows the external name setting.
		 *	\param out_name The external name for the view.
		 *	\return <span class='code'>true</span> if an external name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowExternalName(UTF8 & out_name) const;

		/*! Shows the camera setting.
		 *	\param out_camera The camera for the view.
		 *	\return <span class='code'>true</span> if a camera was specified, <span class='code'>false</span> otherwise. */
		bool				ShowCamera(CameraKit & out_camera) const;

		/*! Shows the default setting.
		 *	\param out_state Whether this view is the default view.
		 *	\return <span class='code'>true</span> if a default setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowDefault(bool & out_state) const;

		/*! Shows the background color for the view.
		 *	\param out_color The background color for the view.
		 *	\return <span class='code'>true</span> if a background color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBackgroundColor(RGBColor & out_color) const;

		/*! Shows the lighting mode for the view.
		 *	\param out_mode The lighting mode for the view.
		 *	\return <span class='code'>true</span> if a lighting mode was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLighting(Lighting::Mode & out_mode) const;

		/*! Shows the rendering mode for the view.
		 *	\param out_mode The rendering mode for the view.
		 *	\return <span class='code'>true</span> if a rendering mode was specified, <span class='code'>false</span> otherwise. */
		bool				ShowRendering(Rendering::Mode & out_mode) const;
	};

	/*! The TextKit class is a user space object.  It acts as the container for all data that can be used to specify a text string for a Publish PDF. */
	class PUBLISH_API TextKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty TextKit object. */
		TextKit();

		/*! The copy constructor creates a new TextKit object that contains the same settings as the source TextKit.
		 * 	\param in_kit The source TextKit to copy. */
		TextKit(TextKit const & in_kit);

		/*! The move constructor creates a TextKit by transferring the underlying object of the rvalue reference to this TextKit.
		 * 	\param in_kit An rvalue reference to a TextKit to take the underlying object from. */
		TextKit(TextKit && in_kit);

		virtual ~TextKit();

	static const HPS::Type staticType = HPS::Type::PublishTextKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a TextKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for export unless a setting is overridden by the TextKit passed to File::Export.
		 *	\return A TextKit with the default settings. */
		static TextKit		GetDefault();

		/*! Copies the source TextKit into this TextKit.
		 * 	\param in_kit The source TextKit to copy. */
		void				Set(TextKit const & in_kit);

		/*! Copies this TextKit into the given TextKit.
		 * 	\param out_kit The TextKit to populate with the contents of this TextKit. */
		void				Show(TextKit & out_kit) const;

		/*! Copies the source TextKit into this TextKit.
		 * 	\param in_kit The source TextKit to copy.
		 * 	\return A reference to this TextKit. */
		TextKit &			operator=(TextKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this TextKit.
		 *	\param in_kit An rvalue reference to a TextKit to take the underlying object from.
		 *	\return A reference to this TextKit. */
		TextKit &			operator=(TextKit && in_kit);

		/*! Indicates whether this TextKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this TextKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source TextKit is equivalent to this TextKit. 
		 *	\param in_kit The source TextKit to compare to this TextKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(TextKit const & in_kit) const;

		/*!	Check if the source TextKit is equivalent to this TextKit. 
		 *	\param in_kit The source TextKit to compare to this TextKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(TextKit const & in_kit) const;

		/*!	Check if the source TextKit is not equivalent to this TextKit. 
		 *	\param in_kit The source TextKit to compare to this TextKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(TextKit const & in_kit) const;


		/*! Sets the text string for the text.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data.html#a46799fed1e4632775e5bd7a0821e821d">A3DPDFTextData::m_pcTextString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data_ex.html#ad1fea7e382e5d88734dbf69b04ff48c4">A3DPDFTextDataEx::m_pcTextString</a>.
		 *	\param in_text UTF8-encoded text string for the text.
		 *	\return A reference to this TextKit. */
		TextKit &			SetText(char const * in_text);

		/*! Sets a built-in font for the text.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data.html#acd2684e945fd9ce23294af5f10d58365">A3DPDFTextData::m_eFontName</a>.
		 *	\param in_font The built-in font for the text.
		 *	\return A reference to this TextKit. */
		TextKit &			SetFont(Text::Font::Name in_font);

		/*! Sets an explicit font for the text.  As of Publish 5.11, this is only known to create a valid text string in the PDF if the font name is "MS Gothic" and the language is Japanese, otherwise the creation will fail.
		 *	\param in_font_name UTF8-encoded font name for the text.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data_ex.html#a38dfb0879aafd915d6f1ea908f12749f">A3DPDFTextDataEx::m_pcFontName</a>.
		 *	\param in_language The language for the font.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data_ex.html#abe12b1fb142616bd9ce3da7f7f86cefb">A3DPDFTextDataEx::m_eLanguage</a>.
		 *	\param in_embed_font Whether to embed the font in the PDF.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data_ex.html#a0fa6e9cce5be920726da15963cc62539">A3DPDFTextDataEx::m_bEmbedFontInPDF</a>.
		 *	\return A reference to this TextKit. */
		TextKit &			SetFont(char const * in_font_name, Text::Language in_language, bool in_embed_font);

		/*! Sets the size in points for the text.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data.html#a0cf58cefed53657fb837e3441bfac53b">A3DPDFTextData::m_iFontSize</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data_ex.html#a94cb506c766286967e59c7d67929be7c">A3DPDFTextDataEx::m_iFontSize</a>.
		 *	\param in_size The size in points for the text.
		 *	\return A reference to this TextKit. */
		TextKit &			SetSize(int in_size);

		/*! Sets the color for the text.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data.html#af9b98ed87165494a3243d0d13513a596">A3DPDFTextData::m_sColor</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_data_ex.html#aa780c227759b0c75d87db4fd186d0ddf">A3DPDFTextDataEx::m_sColor</a>.
		 *	\param in_color Color for the text.
		 *	\return A reference to this TextKit. */
		TextKit &			SetColor(RGBColor const & in_color);


		/*! Removes the text string setting.
		 *	\return A reference to this TextKit. */
		TextKit &			UnsetText();

		/*! Removes the font setting.
		 *	\return A reference to this TextKit. */
		TextKit &			UnsetFont();

		/*! Removes the size setting.
		 *	\return A reference to this TextKit. */
		TextKit &			UnsetSize();

		/*! Removes the color setting.
		 *	\return A reference to this TextKit. */
		TextKit &			UnsetColor();

		/*! Removes all data from the text.
		 *	\return A reference to this TextKit. */
		TextKit &			UnsetEverything();


		/*! Shows the text string setting.
		 *	\param out_text The text string for the text.
		 *	\return <span class='code'>true</span> if a text string was specified, <span class='code'>false</span> otherwise. */
		bool				ShowText(UTF8 & out_text) const;

		/*! Shows the font setting.
		 *	\param out_type The type of font for the text.
		 *	\param out_font The built-in font for the text.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the text.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_language The language for the font.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_embed_font Whether to embed the font in the PDF.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Language & out_language, bool & out_embed_font) const;

		/*! Shows the size setting.
		 *	\param out_size The size for the text.
		 *	\return <span class='code'>true</span> if a size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowSize(int & out_size) const;

		/*! Shows the color setting
		 *	\param out_color The color for the text.
		 *	\return <span class='code'>true</span> if a color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowColor(RGBColor & out_color) const;
	};

	/*! The ImageKit class is a user space object.  It acts as the container for all data that can be used to specify an image for a Publish PDF. */
	class PUBLISH_API ImageKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ImageKit object. */
		ImageKit();

		/*! The copy constructor creates a new ImageKit object that contains the same settings as the source ImageKit.
		 * 	\param in_kit The source ImageKit to copy. */
		ImageKit(ImageKit const & in_kit);

		/*! The move constructor creates an ImageKit by transferring the underlying object of the rvalue reference to this ImageKit.
		 * 	\param in_kit An rvalue reference to an ImageKit to take the underlying object from. */
		ImageKit(ImageKit && in_kit);

		virtual ~ImageKit();

	static const HPS::Type staticType = HPS::Type::PublishImageKit;
		HPS::Type			ObjectType() const { return staticType; }

		// no GetDefault() since function doesn't make sense for this class

		/*! Copies the source ImageKit into this ImageKit.
		 * 	\param in_kit The source ImageKit to copy. */
		void				Set(ImageKit const & in_kit);

		/*! Copies this ImageKit into the given ImageKit.
		 * 	\param out_kit The ImageKit to populate with the contents of this ImageKit. */
		void				Show(ImageKit & out_kit) const;

		/*! Copies the source ImageKit into this ImageKit.
		 * 	\param in_kit The source ImageKit to copy.
		 * 	\return A reference to this ImageKit. */
		ImageKit &			operator=(ImageKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ImageKit.
		 *	\param in_kit An rvalue reference to an ImageKit to take the underlying object from.
		 *	\return A reference to this ImageKit. */
		ImageKit &			operator=(ImageKit && in_kit);

		/*! Indicates whether this ImageKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ImageKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source ImageKit is equivalent to this ImageKit. 
		 *	\param in_kit The source ImageKit to compare to this ImageKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(ImageKit const & in_kit) const;

		/*!	Check if the source ImageKit is equivalent to this ImageKit. 
		 *	\param in_kit The source ImageKit to compare to this ImageKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(ImageKit const & in_kit) const;

		/*!	Check if the source ImageKit is not equivalent to this ImageKit. 
		 *	\param in_kit The source ImageKit to compare to this ImageKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(ImageKit const & in_kit) const;


		/*! Sets the file for the image.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_image_data.html#acfdbde1f6a3d30b89649d2f8550ae4d3">A3DPDFImageData::m_pcFileName</a>.
		 *	\param in_filename UTF8-encoded filename for the image.
		 *	\return A reference to this ImageKit. */
		ImageKit &			SetFile(char const * in_filename);

		/*! Sets the size of the image in pixels.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_image_data.html#a8094c2fae308789cf6dec5a96ab54a3f">A3DPDFImageData::m_iWidth</a>
		 *	and <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_image_data.html#afd7857039238bdeaf53ab9090904402e">A3DPDFImageData::m_iHeight</a>.
		 *	\param in_width The width of the image in pixels.
		 *	\param in_height The height of the image in pixels.
		 *	\return A reference to this ImageKit. */
		ImageKit &			SetSize(int in_width, int in_height);

		/*! Sets the format of the image.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_image_data.html#afd7857039238bdeaf53ab9090904402e">A3DPDFImageData::m_eFormat</a>.
		 *	\param in_format The format of the image.
		 *	\return A reference to this ImageKit. */
		ImageKit &			SetFormat(Image::Format in_format);


		/*! Removes the file from the image.
		 *	\return A reference to this ImageKit. */
		ImageKit &			UnsetFile();

		/*! Removes the size from the image.
		 *	\return A reference to this ImageKit. */
		ImageKit &			UnsetSize();

		/*! Removes the format from the image.
		 *	\return A reference to this ImageKit. */
		ImageKit &			UnsetFormat();

		/*! Removes all data from the image.
		 *	\return A reference to this ImageKit. */
		ImageKit &			UnsetEverything();


		/*! Shows the file setting.
		 *	\param out_filename The filename for the image.
		 *	\return <span class='code'>true</span> if a color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFile(UTF8 & out_filename) const;

		/*! Shows the size setting.
		 *	\param out_width The width of the image in pixels.
		 *	\param out_height The height of the image in pixels.
		 *	\return <span class='code'>true</span> if a width and height were specified, <span class='code'>false</span> otherwise. */
		bool				ShowSize(int & out_width, int & out_height) const;

		/*! Shows the format setting.
		 *	\param out_format The format of the image.
		 *	\return <span class='code'>true</span> if a format was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFormat(Image::Format & out_format) const;
	};

	/*! The TableKit class is a user space object.  It acts as the container for all data that can be used to specify a table for a Publish PDF.
	 *  \warning The TableToPDF Publish add-on must be installed in order to use this class. */
	class PUBLISH_API TableKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty TableKit object. */
		TableKit();

		/*! The copy constructor creates a new TableKit object that contains the same settings as the source TableKit.
		 * 	\param in_kit The source TableKit to copy. */
		TableKit(TableKit const & in_kit);

		/*! The move constructor creates a TableKit by transferring the underlying object of the rvalue reference to this TableKit.
		 * 	\param in_kit An rvalue reference to a TableKit to take the underlying object from. */
		TableKit(TableKit && in_kit);

		virtual ~TableKit();

	static const HPS::Type staticType = HPS::Type::PublishTableKit;
		HPS::Type			ObjectType() const { return staticType; }

		// no GetDefault() since function doesn't make sense for this class

		/*! Copies the source TableKit into this TableKit.
		 * 	\param in_kit The source TableKit to copy. */
		void				Set(TableKit const & in_kit);

		/*! Copies this TableKit into the given TableKit.
		 * 	\param out_kit The TableKit to populate with the contents of this TableKit. */
		void				Show(TableKit & out_kit) const;

		/*! Copies the source TableKit into this TableKit.
		 * 	\param in_kit The source TableKit to copy.
		 * 	\return A reference to this TableKit. */
		TableKit &			operator=(TableKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this TableKit.
		 *	\param in_kit An rvalue reference to a TableKit to take the underlying object from.
		 *	\return A reference to this TableKit. */
		TableKit &			operator=(TableKit && in_kit);

		/*! Indicates whether this TableKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this TableKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source TableKit is equivalent to this TableKit. 
		 *	\param in_kit The source TableKit to compare to this TableKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(TableKit const & in_kit) const;

		/*!	Check if the source TableKit is equivalent to this TableKit. 
		 *	\param in_kit The source TableKit to compare to this TableKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(TableKit const & in_kit) const;

		/*!	Check if the source TableKit is not equivalent to this TableKit. 
		 *	\param in_kit The source TableKit to compare to this TableKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(TableKit const & in_kit) const;


		/*! Sets the HTML table source or file.
		 *	\param in_source UTF8-encoded HTML table source or file (depending on the following argument).
		 *	\param in_type The type of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this TableKit. */
		TableKit &			SetHTML(char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Sets the HTML table style source or file.
		 *	\param in_source UTF8-encoded HTML table style source or file (depending on the following argument).
		 *	\param in_type Type of the preceding argument.
		 *	\return A reference to this TableKit. */
		TableKit &			SetHTMLStyle(char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Adds a link to this table at the specified row and column.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga073798a1d1582e79b31e3b38f1cbf105">A3DPDFPageInsertLinkInTable</a>.
		 *	\param in_row Row in which to add the link.
		 *	\param in_column Column in which to add the link.
		 *	\param in_link Link to add at the specified row and column in the table.
		 *	\return A reference to this TableKit. */
		TableKit &			SetLink(int in_row, int in_column, LinkKit const & in_link);

		/*! Adds text to this table at the specified row and column.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gad3d5385fd9ad890e578007ee62b0100b">A3DPDFPageInsertTextInTable</a>.
		 *	\param in_row Row in which to add the text.
		 *	\param in_column Column in which to add the text.
		 *	\param in_text Text to add at the specified row and column in the table.
		 *	\return A reference to this TableKit. */
		TableKit &			SetText(int in_row, int in_column, TextKit const & in_text);

		/*! Adds a button to this table at the specified row and column.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1012ddd4a4d8e0fa1f62b0a51e4618d3">A3DPDFPageInsertButtonInTable</a>.
		 *	\param in_row Row in which to add the button.
		 *	\param in_column Column in which to add the button.
		 *	\param in_button Button to add at the specified row and column in the table.
		 *	\return A reference to this TableKit. */
		TableKit &			SetButton(int in_row, int in_column, ButtonKit const & in_button);

		/*! Adds a text field to this table at the specified row and column.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacc519dd8ddd5c28899eb093d59b8e313">A3DPDFPageInsertTextFieldInTable</a>.
		 *	\param in_row Row in which to add the text field.
		 *	\param in_column Column in which to add the text field.
		 *	\param in_text_field Text field to add at the specified row and column in the table.
		 *	\return A reference to this TableKit. */
		TableKit &			SetTextField(int in_row, int in_column, TextFieldKit const & in_text_field);


		/*! Removes the HTML table setting.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetHTML();

		/*! Removes the HTML table style setting.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetHTMLStyle();

		/*! Removes the link at the specified row and column.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetLink(int in_row, int in_column);

		/*! Removes the text at the specified row and column.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetText(int in_row, int in_column);

		/*! Removes the button at the specified row and column.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetButton(int in_row, int in_column);

		/*! Removes the text field at the specified row and column.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetTextField(int in_row, int in_column);

		/*! Removes all data from the table.
		 *	\return A reference to this TableKit. */
		TableKit &			UnsetEverything();


		/*! Shows the HTML table setting.
		 *	\param out_source The HTML table source or file (depending on the following argument).
		 *	\param out_type The type of the preceding argument.
		 *	\return <span class='code'>true</span> if an HTML table was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHTML(UTF8 & out_source, Source::Type & out_type) const;

		/*! Shows the HTML table style setting.
		 *	\param out_source The HTML table style source or file (depending on the following argument).
		 *	\param out_type The type of the preceding argument.
		 *	\return <span class='code'>true</span> if an HTML table style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHTMLStyle(UTF8 & out_source, Source::Type & out_type) const;

		/*! Shows the link at the specified row and column.
		 *	\param in_row Row in which to show the link.
		 *	\param in_column Column in which to show the link.
		 *	\param out_link The link at the specified row and column in the table.
		 *	\return <span class='code'>true</span> if a link was added at the specified row and column, <span class='code'>false</span> otherwise. */
		bool				ShowLink(int in_row, int in_column, LinkKit & out_link) const;

		/*! Shows the text at the specified row and column.
		 *	\param in_row Row in which to show the text.
		 *	\param in_column Column in which to show the text.
		 *	\param out_text The text at the specified row and column in the table.
		 *	\return <span class='code'>true</span> if text was added at the specified row and column, <span class='code'>false</span> otherwise. */
		bool				ShowText(int in_row, int in_column, TextKit & out_text) const;

		/*! Shows the button at the specified row and column.
		 *	\param in_row Row in which to show the button.
		 *	\param in_column Column in which to show the button.
		 *	\param out_button The button at the specified row and column in the table.
		 *	\return <span class='code'>true</span> if a button was added at the specified row and column, <span class='code'>false</span> otherwise. */
		bool				ShowButton(int in_row, int in_column, ButtonKit & out_button) const;

		/*! Shows the link at the specified row and column.
		 *	\param in_row Row in which to show the link.
		 *	\param in_column Column in which to show the link.
		 *	\param out_text_field The text field at the specified row and column in the table.
		 *	\return <span class='code'>true</span> if a link was added at the specified row and column, <span class='code'>false</span> otherwise. */
		bool				ShowTextField(int in_row, int in_column, TextFieldKit & out_text_field) const;
	};

	/*! The SlideTableKit class is a user space object.  It acts as the container for all data that can be used to specify a slide table for a Publish PDF.
	 *  \warning The TableToPDF Publish add-on must be installed in order to use this class. */
	class PUBLISH_API SlideTableKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty SlideTableKit object. */
		SlideTableKit();

		/*! The copy constructor creates a new SlideTableKit object that contains the same settings as the source SlideTableKit.
		 * 	\param in_kit The source SlideTableKit to copy. */
		SlideTableKit(SlideTableKit const & in_kit);

		/*! The move constructor creates a SlideTableKit by transferring the underlying object of the rvalue reference to this SlideTableKit.
		 * 	\param in_kit An rvalue reference to a SlideTableKit to take the underlying object from. */
		SlideTableKit(SlideTableKit && in_kit);

		virtual ~SlideTableKit();

	static const HPS::Type staticType = HPS::Type::PublishSlideTableKit;
		HPS::Type			ObjectType() const { return staticType; }

		// no GetDefault() since function doesn't make sense for this class

		/*! Copies the source SlideTableKit into this SlideTableKit.
		 * 	\param in_kit The source SlideTableKit to copy. */
		void				Set(SlideTableKit const & in_kit);

		/*! Copies this SlideTableKit into the given SlideTableKit.
		 * 	\param out_kit The SlideTableKit to populate with the contents of this SlideTableKit. */
		void				Show(SlideTableKit & out_kit) const;

		/*! Copies the source SlideTableKit into this SlideTableKit.
		 * 	\param in_kit The source SlideTableKit to copy.
		 * 	\return A reference to this SlideTableKit. */
		SlideTableKit &		operator=(SlideTableKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this SlideTableKit.
		 *	\param in_kit An rvalue reference to a SlideTableKit to take the underlying object from.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		operator=(SlideTableKit && in_kit);

		/*! Indicates whether this SlideTableKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this SlideTableKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source SlideTableKit is equivalent to this SlideTableKit. 
		 *	\param in_kit The source SlideTableKit to compare to this SlideTableKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(SlideTableKit const & in_kit) const;

		/*!	Check if the source SlideTableKit is equivalent to this SlideTableKit. 
		 *	\param in_kit The source SlideTableKit to compare to this SlideTableKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(SlideTableKit const & in_kit) const;

		/*!	Check if the source SlideTableKit is not equivalent to this SlideTableKit. 
		 *	\param in_kit The source SlideTableKit to compare to this SlideTableKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(SlideTableKit const & in_kit) const;


		/*! Sets the HTML table source or file.
		 *	\param in_source UTF8-encoded HTML table source or file (depending on the following argument).
		 *	\param in_type The type of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		SetHTML(char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Sets the HTML table style source or file.
		 *	\param in_source UTF8-encoded HTML table style source or file (depending on the following argument).
		 *	\param in_type Type of the preceding argument.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		SetHTMLStyle(char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Sets the button names for the slide table.
		 *	\param in_previous_button_name UTF8-encoded name for the previous button.
		 *	\param in_next_button_name UTF8-encoded name for the next button.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		SetButtons(char const * in_previous_button_name, char const * in_next_button_name);

		/*! Sets the text strings for the slide table.
		 *	\param in_rows Number of rows.
		 *	\param in_columns Number of columns.
		 *	\param in_text Text strings for the slide table.  This array should be of size <span class='code'>in_rows * in_columns</span>.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		SetText(size_t in_rows, size_t in_columns, TextFieldKit const in_text[]);

		/*! Sets the text strings for the slide table.
		 *	\param in_rows Number of rows.
		 *	\param in_columns Number of columns.
		 *	\param in_text Text strings for the slide table.  This array should be of size <span class='code'>in_rows * in_columns</span>.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		SetText(size_t in_rows, size_t in_columns, TextFieldKitArray const & in_text);

		/*! Sets whether this slide table has a header.
		 *	\param in_state Whether this slide table has a header.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		SetHeader(bool in_state);


		/*! Removes the HTML table setting.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		UnsetHTML();

		/*! Removes the HTML table style setting.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		UnsetHTMLStyle();

		/*! Removes the buttons setting.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		UnsetButtons();

		/*! Removes the text strings setting.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		UnsetText();

		/*! Removes the header setting.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		UnsetHeader();

		/*! Removes all data from the table.
		 *	\return A reference to this SlideTableKit. */
		SlideTableKit &		UnsetEverything();


		/*! Shows the HTML table setting.
		 *	\param out_source The HTML table source or file (depending on the following argument).
		 *	\param out_type The type of the preceding argument.
		 *	\return <span class='code'>true</span> if an HTML table was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHTML(UTF8 & out_source, Source::Type & out_type) const;

		/*! Shows the HTML table style setting.
		 *	\param out_source The HTML table style source or file (depending on the following argument).
		 *	\param out_type The type of the preceding argument.
		 *	\return <span class='code'>true</span> if an HTML table style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHTMLStyle(UTF8 & out_source, Source::Type & out_type) const;

		/*! Shows the buttons setting.
		 *	\param out_previous_button_name Name for the previous button.
		 *	\param out_next_button_name Name for the next button.
		 *	\return <span class='code'>true</span> if button names were specified, <span class='code'>false</span> otherwise. */
		bool				ShowButtons(UTF8 & out_previous_button_name, UTF8 & out_next_button_name) const;

		/*! Shows the text strings setting.
		 *	\param out_rows Number of rows.
		 *	\param out_columns Number of columns.
		 *	\param out_text Text strings for the slide table.
		 *	\return <span class='code'>true</span> if text strings were specified, <span class='code'>false</span> otherwise. */
		bool				ShowText(size_t & out_rows, size_t & out_columns, TextFieldKitArray & out_text) const;

		/*! Shows the header setting.
		 *	\param out_state Whether this slide table has a header.
		 *	\return <span class='code'>true</span> if a header setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHeader(bool & out_state) const;
	};

	/*! The LinkKit class is a user space object.  It acts as the container for all data that can be used to specify a link for a Publish PDF. */
	class PUBLISH_API LinkKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty LinkKit object. */
		LinkKit();

		/*! The copy constructor creates a new LinkKit object that contains the same settings as the source LinkKit.
		 * 	\param in_kit The source LinkKit to copy. */
		LinkKit(LinkKit const & in_kit);

		/*! The move constructor creates a LinkKit by transferring the underlying object of the rvalue reference to this LinkKit.
		 * 	\param in_kit An rvalue reference to a LinkKit to take the underlying object from. */
		LinkKit(LinkKit && in_kit);

		virtual ~LinkKit();

	static const HPS::Type staticType = HPS::Type::PublishLinkKit;
		HPS::Type			ObjectType() const { return staticType; }

		// no GetDefault() since function doesn't make sense for this class

		/*! Copies the source LinkKit into this LinkKit.
		 * 	\param in_kit The source LinkKit to copy. */
		void				Set(LinkKit const & in_kit);

		/*! Copies this LinkKit into the given LinkKit.
		 * 	\param out_kit The LinkKit to populate with the contents of this LinkKit. */
		void				Show(LinkKit & out_kit) const;

		/*! Copies the source LinkKit into this LinkKit.
		 * 	\param in_kit The source LinkKit to copy.
		 * 	\return A reference to this LinkKit. */
		LinkKit &			operator=(LinkKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this LinkKit.
		 *	\param in_kit An rvalue reference to a LinkKit to take the underlying object from.
		 *	\return A reference to this LinkKit. */
		LinkKit &			operator=(LinkKit && in_kit);

		/*! Indicates whether this LinkKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this LinkKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source LinkKit is equivalent to this LinkKit. 
		 *	\param in_kit The source LinkKit to compare to this LinkKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(LinkKit const & in_kit) const;

		/*!	Check if the source LinkKit is equivalent to this LinkKit. 
		 *	\param in_kit The source LinkKit to compare to this LinkKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(LinkKit const & in_kit) const;

		/*!	Check if the source LinkKit is not equivalent to this LinkKit. 
		 *	\param in_kit The source LinkKit to compare to this LinkKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(LinkKit const & in_kit) const;


		/*! Sets the JavaScript source or file for the link.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_link_data.html#a30ccc5b6802182423393fb898d422a99">A3DPDFLinkData::m_pcJavascriptString</a>.
		 *	\param in_source The JavaScript source or file (depending on the next argument) for the link.
		 *	\param in_type The type of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this LinkKit. */
		LinkKit &			SetJavaScript(char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Sets the border width in points for the link.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_link_data.html#a9379305ecf8db0edd6465a554945847f">A3DPDFLinkData::m_iBorderWidth</a>.
		 *	\param in_width The border width in points for the link.
		 *	\return A reference to this LinkKit. */
		LinkKit &			SetBorderWidth(int in_width);

		/*! Sets the highlighting mode for the link.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_link_data.html#a4d44cce7dcae5ac7780d543ee0194276">A3DPDFLinkData::m_eHighlightingMode</a>.
		 *	\param in_mode The highlighting mode for the link.
		 *	\return A reference to this LinkKit. */
		LinkKit &			SetHighlighting(Highlighting::Mode in_mode);

		/*! Sets the color of the border for the link.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_link_data.html#a2c07d8af3446367af217e9f65f0bfcc2">A3DPDFLinkData::m_sColor</a>.
		 *	\param in_color The color of the border for the link.
		 *	\return A reference to this LinkKit. */
		LinkKit &			SetBorderColor(RGBColor const & in_color);


		/*! Removes the JavaScript setting.
		 *	\return A reference to this LinkKit. */
		LinkKit &			UnsetJavaScript();

		/*! Removes the border width setting.
		 *	\return A reference to this LinkKit. */
		LinkKit &			UnsetBorderWidth();

		/*! Removes the highlighting mode setting.
		 *	\return A reference to this LinkKit. */
		LinkKit &			UnsetHighlighting();

		/*! Removes the border color setting.
		 *	\return A reference to this LinkKit. */
		LinkKit &			UnsetBorderColor();

		/*! Removes all data from the link.
		 *	\return A reference to this LinkKit. */
		LinkKit &			UnsetEverything();


		/*! Shows the JavaScript setting.
		 *	\param out_source The JavaScript source or file (depending on the next argument) for the link.
		 *	\param out_type The type of the preceding argument.
		 *	\return <span class='code'>true</span> if a JavaScript source or file was specified, <span class='code'>false</span> otherwise. */
		bool				ShowJavaScript(UTF8 & out_source, Source::Type & out_type) const;

		/*! Shows the border width setting.
		 *	\param out_width The border width of the link.
		 *	\return <span class='code'>true</span> if a border width was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderWidth(int & out_width) const;

		/*! Shows the highlighting mode setting.
		 *	\param out_mode The highlighting mode of the link.
		 *	\return <span class='code'>true</span> if a highlighting mode was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHighlighting(Highlighting::Mode & out_mode) const;

		/*! Shows the border color setting.
		 *	\param out_color The border color of the link.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;
	};

	/*! The ButtonKit class is a user space object.  It acts as the container for all data that can be used to specify a button for a Publish PDF. */
	class PUBLISH_API ButtonKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ButtonKit object. */
		ButtonKit();

		/*! The copy constructor creates a new ButtonKit object that contains the same settings as the source ButtonKit.
		 * 	\param in_kit The source ButtonKit to copy. */
		ButtonKit(ButtonKit const & in_kit);

		/*! The move constructor creates a ButtonKit by transferring the underlying object of the rvalue reference to this ButtonKit.
		 * 	\param in_kit An rvalue reference to a ButtonKit to take the underlying object from. */
		ButtonKit(ButtonKit && in_kit);

		virtual ~ButtonKit();

	static const HPS::Type staticType = HPS::Type::PublishButtonKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a ButtonKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a button unless a setting is overridden by the ButtonKit passed to a function.
		 *	\return A ButtonKit with the default settings. */
		static ButtonKit	GetDefault();

		/*! Copies the source ButtonKit into this ButtonKit.
		 * 	\param in_kit The source ButtonKit to copy. */
		void				Set(ButtonKit const & in_kit);

		/*! Copies this ButtonKit into the given ButtonKit.
		 * 	\param out_kit The ButtonKit to populate with the contents of this ButtonKit. */
		void				Show(ButtonKit & out_kit) const;

		/*! Copies the source ButtonKit into this ButtonKit.
		 * 	\param in_kit The source ButtonKit to copy.
		 * 	\return A reference to this ButtonKit. */
		ButtonKit &			operator=(ButtonKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ButtonKit.
		 *	\param in_kit An rvalue reference to a ButtonKit to take the underlying object from.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			operator=(ButtonKit && in_kit);

		/*! Indicates whether this ButtonKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ButtonKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source ButtonKit is equivalent to this ButtonKit. 
		 *	\param in_kit The source ButtonKit to compare to this ButtonKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(ButtonKit const & in_kit) const;

		/*!	Check if the source ButtonKit is equivalent to this ButtonKit. 
		 *	\param in_kit The source ButtonKit to compare to this ButtonKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(ButtonKit const & in_kit) const;

		/*!	Check if the source ButtonKit is not equivalent to this ButtonKit. 
		 *	\param in_kit The source ButtonKit to compare to this ButtonKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(ButtonKit const & in_kit) const;


		/*!	Sets the name of the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a472a84d194be5e85a890b7cfcb98a6ad">A3DPDFButtonData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetName(char const * in_name);

		/*!	Sets the label for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#af73e13a1e51846375d5c63a7c488a7ba">A3DPDFButtonData::m_pcLabel</a>.
		 *	\param in_label UTF8-encoded label for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetLabel(char const * in_label);

		/*!	Sets a builtin font for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#aae9e31522c1c3eb0a0f2ad59d3425586">A3DPDFButtonData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#aae9e31522c1c3eb0a0f2ad59d3425586">A3DPDFButtonData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the button.
		 *	\param in_style The style of font to use for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a4af94fc49eef5d2db53caac85b6d2044">A3DPDFButtonData::m_iFontSize</a>.
		 *	\param in_size The font size for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a49df6a4cddc535bb82910055c6048e79">A3DPDFButtonData::m_sTextColor</a>.
		 *	\param in_color The text color for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a022cdbd273b1ae114cd10d98a8f93d64">A3DPDFButtonData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a7ac1ac94a00ed6e7af90d2740e8c21eb">A3DPDFButtonData::m_eFormField</a>.
		 *	\param in_state The visibility for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a7ac1ac94a00ed6e7af90d2740e8c21eb">A3DPDFButtonData::m_eFormField</a>.
		 *	\param in_state The printability for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#ac054240b48718fdc938cfbc6731dd741">A3DPDFButtonData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the button is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#ae1776f49140f614ff443339008f3481c">A3DPDFButtonData::m_bIsLocked</a>.
		 *	\param in_state Whether the button is locked.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetLock(bool in_state);

		/*!	Sets whether the button has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#ad4c7a9512340cf47280a54ea224d40d2">A3DPDFButtonData::m_bHasBorder</a>.
		 *	\param in_state Whether the button has a border.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a436165abe7c60728449388fa267c6826">A3DPDFButtonData::m_sBorderColor</a>.
		 *	\param in_color The border color of the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a28d3d317552251ee79e0353339381d1d">A3DPDFButtonData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a1a8a17081af8162147029abf458f6262">A3DPDFButtonData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a29030fda42fac379b85c88976ce17524">A3DPDFButtonData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#ad98c822b7d53b1ca4200325882ab5b0b">A3DPDFButtonData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetFillColor(RGBColor const & in_color);

		/*!	Sets the label position for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a1a5e4feedae6a2af08b3525f55b39ae2">A3DPDFButtonData::m_eLayoutTextIcon</a>.
		 *	\param in_position The label position for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetLabelPosition(Label::Position in_position);

		/*!	Sets the highlighting mode for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a9cda56ca95ddb8c35a6cbe143c337409">A3DPDFButtonData::m_eHighlightingMode</a>.
		 *	\param in_mode The highlighting mode for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetHighlighting(Highlighting::Mode in_mode);

		/*!	Sets the icon image for the button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_button_data.html#a60e42630bd048b7d45b6537ce3c47227">A3DPDFButtonData::m_pImage</a>.
		 *	\param in_image The icon image for the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			SetIconImage(ImageKit const & in_image);


		/*!	Removes the name setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetName();

		/*!	Removes the label setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetLabel();

		/*!	Removes the font setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetFillColor();

		/*!	Removes the label position setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetLabelPosition();

		/*!	Removes the highlighting mode setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetHighlighting();

		/*!	Removes the icon image setting.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetIconImage();

		/*! Removes all data from the button.
		 *	\return A reference to this ButtonKit. */
		ButtonKit &			UnsetEverything();


		/*! Shows the name for the button.
		 *	\param out_name The name of the button.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the label for the button.
		 *	\param out_label The label for the button.
		 *	\return <span class='code'>true</span> if a label was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLabel(UTF8 & out_label) const;

		/*!	Shows the font for the button.
		 *	\param out_type The type of font for the button.
		 *	\param out_font The built-in font for the button.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the button.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the button.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the button.
		 *	\param out_size The font size for the button.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the button.
		 *	\param out_color The text color for the button.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the button.
		 *	\param out_tooltip The tooltip text for the button.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the button.
		 *	\param out_state The visibility for the button.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the button.
		 *	\param out_state The printability for the button.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the button.
		 *	\param out_rotation The text rotation for the button.r
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the button.
		 *	\param in_state Whether the button is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the button.
		 *	\param out_state Whether the button has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the button.
		 *	\param out_color The border color of the button.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the button.
		 *	\param out_thickness The border thickness for the button.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the button.
		 *	\param out_style The border style for the button.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the button.
		 *	\param out_color The fill color for the button.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;

		/*!	Shows the label position for the button.
		 *	\param out_position The label position for the button.
		 *	\return <span class='code'>true</span> if a label position was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLabelPosition(Label::Position & out_position) const;

		/*!	Shows the highlighting mode for the button.
		 *	\param out_mode The highlighting mode for the button.
		 *	\return <span class='code'>true</span> if a highlighting mode was specified, <span class='code'>false</span> otherwise. */
		bool				ShowHighlighting(Highlighting::Mode & out_mode) const;

		/*!	Shows the icon image for the button.
		 *	\param out_image The icon image for the button.
		 *	\return <span class='code'>true</span> if an icon image was specified, <span class='code'>false</span> otherwise. */
		bool				ShowIconImage(ImageKit & out_image) const;
	};

	/*! The CheckBoxKit class is a user space object.  It acts as the container for all data that can be used to specify a check box for a Publish PDF. */
	class PUBLISH_API CheckBoxKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty CheckBoxKit object. */
		CheckBoxKit();

		/*! The copy constructor creates a new CheckBoxKit object that contains the same settings as the source CheckBoxKit.
		 * 	\param in_kit The source CheckBoxKit to copy. */
		CheckBoxKit(CheckBoxKit const & in_kit);

		/*! The move constructor creates a CheckBoxKit by transferring the underlying object of the rvalue reference to this CheckBoxKit.
		 * 	\param in_kit An rvalue reference to a CheckBoxKit to take the underlying object from. */
		CheckBoxKit(CheckBoxKit && in_kit);

		virtual ~CheckBoxKit();

	static const HPS::Type staticType = HPS::Type::PublishCheckBoxKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a CheckBoxKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a button unless a setting is overridden by the CheckBoxKit passed to a function.
		 *	\return A CheckBoxKit with the default settings. */
		static CheckBoxKit	GetDefault();

		/*! Copies the source CheckBoxKit into this CheckBoxKit.
		 * 	\param in_kit The source CheckBoxKit to copy. */
		void				Set(CheckBoxKit const & in_kit);

		/*! Copies this CheckBoxKit into the given CheckBoxKit.
		 * 	\param out_kit The CheckBoxKit to populate with the contents of this CheckBoxKit. */
		void				Show(CheckBoxKit & out_kit) const;

		/*! Copies the source CheckBoxKit into this CheckBoxKit.
		 * 	\param in_kit The source CheckBoxKit to copy.
		 * 	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			operator=(CheckBoxKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this CheckBoxKit.
		 *	\param in_kit An rvalue reference to a CheckBoxKit to take the underlying object from.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			operator=(CheckBoxKit && in_kit);

		/*! Indicates whether this CheckBoxKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this CheckBoxKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source CheckBoxKit is equivalent to this CheckBoxKit. 
		 *	\param in_kit The source CheckBoxKit to compare to this CheckBoxKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(CheckBoxKit const & in_kit) const;

		/*!	Check if the source CheckBoxKit is equivalent to this CheckBoxKit. 
		 *	\param in_kit The source CheckBoxKit to compare to this CheckBoxKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(CheckBoxKit const & in_kit) const;

		/*!	Check if the source CheckBoxKit is not equivalent to this CheckBoxKit. 
		 *	\param in_kit The source CheckBoxKit to compare to this CheckBoxKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(CheckBoxKit const & in_kit) const;


		/*!	Sets the name of the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a70f813f3a8e5ed1b545ceb7216ea7c7f">A3DPDFCheckBoxData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetName(char const * in_name);

		/*!	Sets a builtin font for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#acf5d507f8d53e88edb732b77f55fbc7f">A3DPDFCheckBoxData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#acf5d507f8d53e88edb732b77f55fbc7f">A3DPDFCheckBoxData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the check box.
		 *	\param in_style The style of font to use for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#aa53034d802575ab762f72f5aa0451e45">A3DPDFCheckBoxData::m_iFontSize</a>.
		 *	\param in_size The font size for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a6137dcadfaec886dd2e98bed49d20169">A3DPDFCheckBoxData::m_sTextColor</a>.
		 *	\param in_color The text color for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a29554afd13838d44df6cc14a99f1fd6f">A3DPDFCheckBoxData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a8fc22870b1df024c392dd14848bf1077">A3DPDFCheckBoxData::m_eFormField</a>.
		 *	\param in_state The visibility for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a8fc22870b1df024c392dd14848bf1077">A3DPDFCheckBoxData::m_eFormField</a>.
		 *	\param in_state The printability for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a717710765f5bd52fc4a71c0fc461ea7f">A3DPDFCheckBoxData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the check box is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a97d7fa4d8f4bc1d6b40efe78bba5ba85">A3DPDFCheckBoxData::m_bIsLocked</a>.
		 *	\param in_state Whether the check box is locked.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetLock(bool in_state);

		/*!	Sets whether the check box has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a8f59c630e0f26f15d6bab9660499b36e">A3DPDFCheckBoxData::m_bHasBorder</a>.
		 *	\param in_state Whether the check box has a border.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#ab11458bd156a91bc2ac7eae2f265147b">A3DPDFCheckBoxData::m_sBorderColor</a>.
		 *	\param in_color The border color of the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#ad8dc5ab66d688a364e9c3212a15f9b89">A3DPDFCheckBoxData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#ae9d37141615cba0ab402366fa7a90f8a">A3DPDFCheckBoxData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the check box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#ae3fef1552b3a1735132d4638698030c7">A3DPDFCheckBoxData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="">A3DPDFCheckBoxData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetFillColor(RGBColor const & in_color);

		/*!	Sets the export value used when field data is exported by Acrobat functions.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a36f565103f4e88c33a1f65d3c10b268b">A3DPDFCheckBoxData::m_pcExportValue</a>.
		 *	\param in_export_value UTF8-encoded export value of the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetExportValue(char const * in_export_value);

		/*!	Sets whether the check box is checked by default.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_check_box_data.html#a2f3a43f0dcd4ad80bb3ba35b40f7201b">A3DPDFCheckBoxData::m_bIsCheckedByDefault</a>.
		 *	\param in_state Whether the check box is checked by default.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			SetDefaultState(bool in_state);


		/*!	Removes the name setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetName();

		/*!	Removes the font setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetFillColor();

		/*!	Removes the export value setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetExportValue();

		/*!	Removes the default state setting.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetDefaultState();

		/*! Removes all data from the check box.
		 *	\return A reference to this CheckBoxKit. */
		CheckBoxKit &			UnsetEverything();


		/*! Shows the name for the check box.
		 *	\param out_name The name of the check box.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the font for the check box.
		 *	\param out_type The type of font for the check box.
		 *	\param out_font The built-in font for the check box.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the check box.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the check box.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the check box.
		 *	\param out_size The font size for the check box.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the check box.
		 *	\param out_color The text color for the check box.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the check box.
		 *	\param out_tooltip The tooltip text for the check box.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the check box.
		 *	\param out_state The visibility for the check box.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the check box.
		 *	\param out_state The printability for the check box.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the check box.
		 *	\param out_rotation The text rotation for the check box.
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the check box.
		 *	\param in_state Whether the check box is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the check box.
		 *	\param out_state Whether the check box has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the check box.
		 *	\param out_color The border color of the check box.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the check box.
		 *	\param out_thickness The border thickness for the check box.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the check box.
		 *	\param out_style The border style for the check box.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the check box.
		 *	\param out_color The fill color for the check box.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;

		/*!	Shows the export value for the check box.
		 *	\param out_export_value The export value for the check box.
		 *	\return <span class='code'>true</span> if an export value was specified, <span class='code'>false</span> otherwise. */
		bool				ShowExportValue(UTF8 & out_export_value) const;

		/*!	Shows the default state for the check box.
		 *	\param out_default_state The default state for the check box.
		 *	\return <span class='code'>true</span> if a default state was specified, <span class='code'>false</span> otherwise. */
		bool				ShowDefaultState(bool & out_default_state) const;
	};

	/*! The RadioButtonKit class is a user space object.  It acts as the container for all data that can be used to specify a radio button for a Publish PDF. */
	class PUBLISH_API RadioButtonKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty RadioButtonKit object. */
		RadioButtonKit();

		/*! The copy constructor creates a new RadioButtonKit object that contains the same settings as the source RadioButtonKit.
		 * 	\param in_kit The source RadioButtonKit to copy. */
		RadioButtonKit(RadioButtonKit const & in_kit);

		/*! The move constructor creates a RadioButtonKit by transferring the underlying object of the rvalue reference to this RadioButtonKit.
		 * 	\param in_kit An rvalue reference to a RadioButtonKit to take the underlying object from. */
		RadioButtonKit(RadioButtonKit && in_kit);

		virtual ~RadioButtonKit();

	static const HPS::Type staticType = HPS::Type::PublishRadioButtonKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a RadioButtonKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a button unless a setting is overridden by the RadioButtonKit passed to a function.
		 *	\return A RadioButtonKit with the default settings. */
		static RadioButtonKit	GetDefault();

		/*! Copies the source RadioButtonKit into this RadioButtonKit.
		 * 	\param in_kit The source RadioButtonKit to copy. */
		void				Set(RadioButtonKit const & in_kit);

		/*! Copies this RadioButtonKit into the given RadioButtonKit.
		 * 	\param out_kit The RadioButtonKit to populate with the contents of this RadioButtonKit. */
		void				Show(RadioButtonKit & out_kit) const;

		/*! Copies the source RadioButtonKit into this RadioButtonKit.
		 * 	\param in_kit The source RadioButtonKit to copy.
		 * 	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			operator=(RadioButtonKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this RadioButtonKit.
		 *	\param in_kit An rvalue reference to a RadioButtonKit to take the underlying object from.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			operator=(RadioButtonKit && in_kit);

		/*! Indicates whether this RadioButtonKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this RadioButtonKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source RadioButtonKit is equivalent to this RadioButtonKit. 
		 *	\param in_kit The source RadioButtonKit to compare to this RadioButtonKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(RadioButtonKit const & in_kit) const;

		/*!	Check if the source RadioButtonKit is equivalent to this RadioButtonKit. 
		 *	\param in_kit The source RadioButtonKit to compare to this RadioButtonKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(RadioButtonKit const & in_kit) const;

		/*!	Check if the source RadioButtonKit is not equivalent to this RadioButtonKit. 
		 *	\param in_kit The source RadioButtonKit to compare to this RadioButtonKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(RadioButtonKit const & in_kit) const;


		/*!	Sets the name of the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a40ec3b6be8f1fe95cf8c714238a9410f">A3DPDFRadioButtonData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetName(char const * in_name);

		/*!	Sets a builtin font for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a0a6917b060b48e1e37ca3bc9a271d97e">A3DPDFRadioButtonData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a0a6917b060b48e1e37ca3bc9a271d97e">A3DPDFRadioButtonData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the radio button.
		 *	\param in_style The style of font to use for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#ae8bad7e264f7a9addafa38ca34332a72">A3DPDFRadioButtonData::m_iFontSize</a>.
		 *	\param in_size The font size for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a751d08997fe26528a7bc7bd3a78126fa">A3DPDFRadioButtonData::m_sTextColor</a>.
		 *	\param in_color The text color for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#ad64e22a58fa2c8ede533afff38e867a1">A3DPDFRadioButtonData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#aca90b0e3b9019ffd05cb6550cac8b4a8">A3DPDFRadioButtonData::m_eFormField</a>.
		 *	\param in_state The visibility for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#aca90b0e3b9019ffd05cb6550cac8b4a8">A3DPDFRadioButtonData::m_eFormField</a>.
		 *	\param in_state The printability for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#abc688e10f3a00649e4fbd6c7483df255">A3DPDFRadioButtonData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the radio button is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a0689a3d142cac8cef66f6757a1cd0c57">A3DPDFRadioButtonData::m_bIsLocked</a>.
		 *	\param in_state Whether the radio button is locked.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetLock(bool in_state);

		/*!	Sets whether the radio button has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a38ede965270a62e5c6de7f4f6953d1ed">A3DPDFRadioButtonData::m_bHasBorder</a>.
		 *	\param in_state Whether the radio button has a border.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#aefee8c8c7a311ae50e9944cc07c86603">A3DPDFRadioButtonData::m_sBorderColor</a>.
		 *	\param in_color The border color of the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#abf52cbae8e198d4ad6a4e8738beef85a">A3DPDFRadioButtonData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a6152162b0e121bebf1d5b61f186d5146">A3DPDFRadioButtonData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the radio button.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#ade0b81e4f2e2029f0a8e76580c34c8d7">A3DPDFRadioButtonData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="">A3DPDFRadioButtonData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetFillColor(RGBColor const & in_color);

		/*!	Sets the export value used when field data is exported by Acrobat functions.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a5953bad32c14a77500f362f43289856a">A3DPDFRadioButtonData::m_pcExportValue</a>.
		 *	\param in_export_value UTF8-encoded export value of the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetExportValue(char const * in_export_value);

		/*!	Sets whether the radio button is checked by default.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#aa40753cac423df5d167096a98b1dbc71">A3DPDFRadioButtonData::m_bIsCheckedByDefault</a>.
		 *	\param in_state Whether the radio button is checked by default.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetDefaultState(bool in_state);

		/*!	Sets whether the radio buttons with the same names are grouped, and therefore mutually exclusive.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_radio_button_data.html#a75d4fe1b092577d2ff780f3e2110cde2">A3DPDFRadioButtonData::m_bRadiosInUnison</a>.
		 *	\param in_state Whether the radio button is part of a group.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			SetGrouping(bool in_state);


		/*!	Removes the name setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetName();

		/*!	Removes the font setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetFillColor();

		/*!	Removes the export value setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetExportValue();

		/*!	Removes the default state setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetDefaultState();

		/*!	Removes the grouping setting.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetGrouping();

		/*! Removes all data from the radio button.
		 *	\return A reference to this RadioButtonKit. */
		RadioButtonKit &			UnsetEverything();


		/*! Shows the name for the radio button.
		 *	\param out_name The name of the radio button.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the font for the radio button.
		 *	\param out_type The type of font for the radio button.
		 *	\param out_font The built-in font for the radio button.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the radio button.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the radio button.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the radio button.
		 *	\param out_size The font size for the radio button.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the radio button.
		 *	\param out_color The text color for the radio button.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the radio button.
		 *	\param out_tooltip The tooltip text for the radio button.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the radio button.
		 *	\param out_state The visibility for the radio button.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the radio button.
		 *	\param out_state The printability for the radio button.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the radio button.
		 *	\param out_rotation The text rotation for the radio button.
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the radio button.
		 *	\param in_state Whether the radio button is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the radio button.
		 *	\param out_state Whether the radio button has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the radio button.
		 *	\param out_color The border color of the radio button.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the radio button.
		 *	\param out_thickness The border thickness for the radio button.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the radio button.
		 *	\param out_style The border style for the radio button.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the radio button.
		 *	\param out_color The fill color for the radio button.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;

		/*!	Shows the export value for the radio button.
		 *	\param out_export_value The export value for the radio button.
		 *	\return <span class='code'>true</span> if an export value was specified, <span class='code'>false</span> otherwise. */
		bool				ShowExportValue(UTF8 & out_export_value) const;

		/*!	Shows the default state for the radio button.
		 *	\param out_default_state The default state for the radio button.
		 *	\return <span class='code'>true</span> if a default state was specified, <span class='code'>false</span> otherwise. */
		bool				ShowDefaultState(bool & out_default_state) const;

		/*!	Shows the grouping state for the radio button.
		 *	\param out_grouping The grouping state for the radio button.
		 *	\return <span class='code'>true</span> if a groupings state was specified, <span class='code'>false</span> otherwise. */
		bool				ShowGrouping(bool & out_grouping) const;
	};

	/*! The ListBoxKit class is a user space object.  It acts as the container for all data that can be used to specify a list box for a Publish PDF. */
	class PUBLISH_API ListBoxKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ListBoxKit object. */
		ListBoxKit();

		/*! The copy constructor creates a new ListBoxKit object that contains the same settings as the source ListBoxKit.
		 * 	\param in_kit The source ListBoxKit to copy. */
		ListBoxKit(ListBoxKit const & in_kit);

		/*! The move constructor creates a ListBoxKit by transferring the underlying object of the rvalue reference to this ListBoxKit.
		 * 	\param in_kit An rvalue reference to a ListBoxKit to take the underlying object from. */
		ListBoxKit(ListBoxKit && in_kit);

		virtual ~ListBoxKit();

	static const HPS::Type staticType = HPS::Type::PublishListBoxKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a ListBoxKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a button unless a setting is overridden by the ListBoxKit passed to a function.
		 *	\return A ListBoxKit with the default settings. */
		static ListBoxKit	GetDefault();

		/*! Copies the source ListBoxKit into this ListBoxKit.
		 * 	\param in_kit The source ListBoxKit to copy. */
		void				Set(ListBoxKit const & in_kit);

		/*! Copies this ListBoxKit into the given ListBoxKit.
		 * 	\param out_kit The ListBoxKit to populate with the contents of this ListBoxKit. */
		void				Show(ListBoxKit & out_kit) const;

		/*! Copies the source ListBoxKit into this ListBoxKit.
		 * 	\param in_kit The source ListBoxKit to copy.
		 * 	\return A reference to this ListBoxKit. */
		ListBoxKit &			operator=(ListBoxKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ListBoxKit.
		 *	\param in_kit An rvalue reference to a ListBoxKit to take the underlying object from.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			operator=(ListBoxKit && in_kit);

		/*! Indicates whether this ListBoxKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ListBoxKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source ListBoxKit is equivalent to this ListBoxKit. 
		 *	\param in_kit The source ListBoxKit to compare to this ListBoxKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(ListBoxKit const & in_kit) const;

		/*!	Check if the source ListBoxKit is equivalent to this ListBoxKit. 
		 *	\param in_kit The source ListBoxKit to compare to this ListBoxKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(ListBoxKit const & in_kit) const;

		/*!	Check if the source ListBoxKit is not equivalent to this ListBoxKit. 
		 *	\param in_kit The source ListBoxKit to compare to this ListBoxKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(ListBoxKit const & in_kit) const;


		/*!	Sets the name of the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a4e87e4e13d58440e1f0b09f56d93da36">A3DPDFListBoxData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetName(char const * in_name);

		/*!	Sets a builtin font for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a6e38b53683e4fb7094eef6944d0c149b">A3DPDFListBoxData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a6e38b53683e4fb7094eef6944d0c149b">A3DPDFListBoxData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the list box.
		 *	\param in_style The style of font to use for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a37ce067341dad28f98be68e13d15f6a2">A3DPDFListBoxData::m_iFontSize</a>.
		 *	\param in_size The font size for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#abfe4860172f1e7be6d65092435fe7589">A3DPDFListBoxData::m_sTextColor</a>.
		 *	\param in_color The text color for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#aec810bf271dd6d8ee817e8913e96d95f">A3DPDFListBoxData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#abfce3c7edc84163e4ab3e26c46d3e709">A3DPDFListBoxData::m_eFormField</a>.
		 *	\param in_state The visibility for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#abfce3c7edc84163e4ab3e26c46d3e709">A3DPDFListBoxData::m_eFormField</a>.
		 *	\param in_state The printability for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a10bc7f5374f39ccb8a10a9d279fb0503">A3DPDFListBoxData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the list box is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a15f5a5f907498ef4a09c8960ca1fac30">A3DPDFListBoxData::m_bIsLocked</a>.
		 *	\param in_state Whether the list box is locked.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetLock(bool in_state);

		/*!	Sets whether the list box has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a613aa60b08374a6b5ee06b9793505fcf">A3DPDFListBoxData::m_bHasBorder</a>.
		 *	\param in_state Whether the list box has a border.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a8338d5a48d20dfd427356f5815a253f1">A3DPDFListBoxData::m_sBorderColor</a>.
		 *	\param in_color The border color of the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#aa68a2d58450a36413f7ae50213e3f2cd">A3DPDFListBoxData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#ad521f05576c362b8529b12fd3b94191b">A3DPDFListBoxData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#a1c8e058380d2c95426c7362c3104bf36">A3DPDFListBoxData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="">A3DPDFListBoxData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetFillColor(RGBColor const & in_color);

		/*!	Sets whether multiple selection is allowed.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_list_box_data.html#aa64df979f517fb4106ea040dde36cc24">A3DPDFListBoxData::m_bMultipleSelection</a>.
		 *	\param in_state Whether multiple selection is allowed
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetMultipleSelection(bool in_state);

		/*!	Sets the contents of the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gaeb695657f0a4b0864ed66c346c984a87">A3DPDFPageFieldListAddItem</a>.
		 *  The two arrays need to have the same size.
		 *	\param in_displayed_values The values displayed in the list box
		 *	\param in_export_values The export values associated with the values displayed
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetContents(UTF8Array const & in_displayed_values, UTF8Array const & in_export_values);

		/*!	Sets the contents of the list box.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gaeb695657f0a4b0864ed66c346c984a87">A3DPDFPageFieldListAddItem</a>.
		*	\param in_count The size of the arrays passed in.
		 *	\param in_displayed_values The values displayed in the list box
		 *	\param in_export_values The export values associated with the values displayed
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			SetContents(size_t in_count, UTF8 const in_displayed_values[], UTF8 const in_export_values[]);


		/*!	Removes the name setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetName();

		/*!	Removes the font setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetFillColor();

		/*!	Removes the multiple selection setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetMultipleSelection();

		/*!	Removes the content setting.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetContents();

		/*! Removes all data from the list box.
		 *	\return A reference to this ListBoxKit. */
		ListBoxKit &			UnsetEverything();


		/*! Shows the name for the list box.
		 *	\param out_name The name of the list box.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the font for the list box.
		 *	\param out_type The type of font for the list box.
		 *	\param out_font The built-in font for the list box.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the list box.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the list box.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the list box.
		 *	\param out_size The font size for the list box.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the list box.
		 *	\param out_color The text color for the list box.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the list box.
		 *	\param out_tooltip The tooltip text for the list box.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the list box.
		 *	\param out_state The visibility for the list box.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the list box.
		 *	\param out_state The printability for the list box.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the list box.
		 *	\param out_rotation The text rotation for the list box.
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the list box.
		 *	\param in_state Whether the list box is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the list box.
		 *	\param out_state Whether the list box has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the list box.
		 *	\param out_color The border color of the list box.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the list box.
		 *	\param out_thickness The border thickness for the list box.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the list box.
		 *	\param out_style The border style for the list box.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the list box.
		 *	\param out_color The fill color for the list box.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;

		/*!	Shows the multiple selection setting for the list box.
		 *	\param out_state Whether multiple selection is enabled.
		 *	\return <span class='code'>true</span> if a multiple selection setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowMultipleSelection(bool & out_state) const;

		/*!	Shows the content setting for the list box.
		 *	\param out_displayed_values The values displayed by the list box.
		 *	\param out_export_values The export values associated with the displayed values.
		 *	\return <span class='code'>true</span> if a content setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowContents(UTF8Array & out_displayed_values, UTF8Array & out_export_values) const;
	};

	/*! The DropDownListKit class is a user space object.  It acts as the container for all data that can be used to specify a drop down list for a Publish PDF. */
	class PUBLISH_API DropDownListKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty DropDownListKit object. */
		DropDownListKit();

		/*! The copy constructor creates a new DropDownListKit object that contains the same settings as the source DropDownListKit.
		 * 	\param in_kit The source DropDownListKit to copy. */
		DropDownListKit(DropDownListKit const & in_kit);

		/*! The move constructor creates a DropDownListKit by transferring the underlying object of the rvalue reference to this DropDownListKit.
		 * 	\param in_kit An rvalue reference to a DropDownListKit to take the underlying object from. */
		DropDownListKit(DropDownListKit && in_kit);

		virtual ~DropDownListKit();

	static const HPS::Type staticType = HPS::Type::PublishDropDownListKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a DropDownListKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a button unless a setting is overridden by the DropDownListKit passed to a function.
		 *	\return A DropDownListKit with the default settings. */
		static DropDownListKit	GetDefault();

		/*! Copies the source DropDownListKit into this DropDownListKit.
		 * 	\param in_kit The source DropDownListKit to copy. */
		void				Set(DropDownListKit const & in_kit);

		/*! Copies this DropDownListKit into the given DropDownListKit.
		 * 	\param out_kit The DropDownListKit to populate with the contents of this DropDownListKit. */
		void				Show(DropDownListKit & out_kit) const;

		/*! Copies the source DropDownListKit into this DropDownListKit.
		 * 	\param in_kit The source DropDownListKit to copy.
		 * 	\return A reference to this DropDownListKit. */
		DropDownListKit &			operator=(DropDownListKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this DropDownListKit.
		 *	\param in_kit An rvalue reference to a DropDownListKit to take the underlying object from.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			operator=(DropDownListKit && in_kit);

		/*! Indicates whether this DropDownListKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this DropDownListKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source DropDownListKit is equivalent to this DropDownListKit. 
		 *	\param in_kit The source DropDownListKit to compare to this DropDownListKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(DropDownListKit const & in_kit) const;

		/*!	Check if the source DropDownListKit is equivalent to this DropDownListKit. 
		 *	\param in_kit The source DropDownListKit to compare to this DropDownListKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(DropDownListKit const & in_kit) const;

		/*!	Check if the source DropDownListKit is not equivalent to this DropDownListKit. 
		 *	\param in_kit The source DropDownListKit to compare to this DropDownListKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(DropDownListKit const & in_kit) const;


		/*!	Sets the name of the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a736db78eb989039c4501ecf84e7d21ac">A3DPDFDropDownListData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetName(char const * in_name);

		/*!	Sets a builtin font for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#ac2b8f71e48e6d8b34a05b39dad7197b7">A3DPDFDropDownListData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#ac2b8f71e48e6d8b34a05b39dad7197b7">A3DPDFDropDownListData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the drop down list.
		 *	\param in_style The style of font to use for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a9e3a815ed2eb6be5fa8855b1af62917d">A3DPDFDropDownListData::m_iFontSize</a>.
		 *	\param in_size The font size for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a7f4b10a1a055edd6172003741d698785">A3DPDFDropDownListData::m_sTextColor</a>.
		 *	\param in_color The text color for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#ad96fec5ac9a5de55bca5f93dbbfab198">A3DPDFDropDownListData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a8104c8b8f8f80b76bfcf12142a51a050">A3DPDFDropDownListData::m_eFormField</a>.
		 *	\param in_state The visibility for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a8104c8b8f8f80b76bfcf12142a51a050">A3DPDFDropDownListData::m_eFormField</a>.
		 *	\param in_state The printability for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#afdf939a599e3be7354d3abd91145c25f">A3DPDFDropDownListData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the drop down list is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#ac7de3f7760af59c1b0c2c26865f594b1">A3DPDFDropDownListData::m_bIsLocked</a>.
		 *	\param in_state Whether the drop down list is locked.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetLock(bool in_state);

		/*!	Sets whether the drop down list has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a07a4601e1dfc46509cac3521af276fb4">A3DPDFDropDownListData::m_bHasBorder</a>.
		 *	\param in_state Whether the drop down list has a border.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a562350ab889cd5c71212a7ec6cd9255c">A3DPDFDropDownListData::m_sBorderColor</a>.
		 *	\param in_color The border color of the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#abc49c0df74bded60a91307c13a5b5831">A3DPDFDropDownListData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a839621262590f42bd0edcda8bf75b181">A3DPDFDropDownListData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a7062508a25fdd808f13a81d9a183bcc3">A3DPDFDropDownListData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="">A3DPDFDropDownListData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetFillColor(RGBColor const & in_color);

		/*!	Sets whether the user can enter custom text.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a4947fa1d4f199544abcffe471873ca4f">A3DPDFDropDownListData::m_bAllowUserToEnterCustomText</a>.
		 *	\param in_state Whether the user can enter custom text.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetCustomText(bool in_state);

		/*!	Sets whether spell checking is enabled.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a84a064d2607797298dcdfbc49d07a1a2">A3DPDFDropDownListData::m_bCheckSpelling</a>.
		 *	\param in_state Whether spell checking is enabled.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetSpellChecking(bool in_state);

		/*!	Sets whether selected values are committed immediately.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_drop_down_list_data.html#a2870499ba84e16ffc484546fd7cacad8">A3DPDFDropDownListData::m_bCommitImmediate</a>.
		 *	\param in_state Whether selected values are committed immediately.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetImmediateCommit(bool in_state);

		/*!	Sets the contents of the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gaeb695657f0a4b0864ed66c346c984a87">A3DPDFPageFieldListAddItem</a>.
		 *  The two arrays need to have the same size.
		 *	\param in_displayed_values The values displayed in the drop down list
		 *	\param in_export_values The export values associated with the values displayed
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetContents(UTF8Array const & in_displayed_values, UTF8Array const & in_export_values);

		/*!	Sets the contents of the drop down list.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gaeb695657f0a4b0864ed66c346c984a87">A3DPDFPageFieldListAddItem</a>.
		*	\param in_count The size of the arrays passed in.
		 *	\param in_displayed_values The values displayed in the drop down list
		 *	\param in_export_values The export values associated with the values displayed
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			SetContents(size_t in_count, UTF8 const in_displayed_values[], UTF8 const in_export_values[]);


		/*!	Removes the name setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetName();

		/*!	Removes the font setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetFillColor();

		/*!	Removes the custom text setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetCustomText();

		/*!	Removes the spell checking setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetSpellChecking();

		/*!	Removes the immediate commit setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetImmediateCommit();

		/*!	Removes the content setting.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetContents();

		/*! Removes all data from the drop down list.
		 *	\return A reference to this DropDownListKit. */
		DropDownListKit &			UnsetEverything();


		/*! Shows the name for the drop down list.
		 *	\param out_name The name of the drop down list.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the font for the drop down list.
		 *	\param out_type The type of font for the drop down list.
		 *	\param out_font The built-in font for the drop down list.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the drop down list.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the drop down list.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the drop down list.
		 *	\param out_size The font size for the drop down list.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the drop down list.
		 *	\param out_color The text color for the drop down list.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the drop down list.
		 *	\param out_tooltip The tooltip text for the drop down list.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the drop down list.
		 *	\param out_state The visibility for the drop down list.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the drop down list.
		 *	\param out_state The printability for the drop down list.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the drop down list.
		 *	\param out_rotation The text rotation for the drop down list.
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the drop down list.
		 *	\param in_state Whether the drop down list is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the drop down list.
		 *	\param out_state Whether the drop down list has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the drop down list.
		 *	\param out_color The border color of the drop down list.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the drop down list.
		 *	\param out_thickness The border thickness for the drop down list.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the drop down list.
		 *	\param out_style The border style for the drop down list.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the drop down list.
		 *	\param out_color The fill color for the drop down list.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;

		/*!	Shows the custom text setting for the drop down list.
		 *	\param out_state Whether the user can enter custom text.
		 *	\return <span class='code'>true</span> if a custom text setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowCustomText(bool & out_state) const;

		/*!	Shows the spell checking setting for the drop down list.
		 *	\param out_state Whether spell checking is enabled.
		 *	\return <span class='code'>true</span> if a spell checking setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowSpellChecking(bool & out_state) const;

		/*!	Shows the immediate commit setting for the drop down list.
		 *	\param out_state Whether selected values are committed immediately.
		 *	\return <span class='code'>true</span> if an immediate commit setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowImmediateCommit(bool & out_state) const;

		/*!	Shows the content setting for the drop down list
		 *	\param out_displayed_values The values displayed by the drop down list
		 *	\param out_export_values The export values associated with the displayed values.
		 *	\return <span class='code'>true</span> if a content setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowContents(UTF8Array & out_displayed_values, UTF8Array & out_export_values) const;
	};

	/*! The SignatureFieldKit class is a user space object.  It acts as the container for all data that can be used to specify a signature field for a Publish PDF. */
	class PUBLISH_API SignatureFieldKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty SignatureFieldKit object. */
		SignatureFieldKit();

		/*! The copy constructor creates a new SignatureFieldKit object that contains the same settings as the source SignatureFieldKit.
		 * 	\param in_kit The source SignatureFieldKit to copy. */
		SignatureFieldKit(SignatureFieldKit const & in_kit);

		/*! The move constructor creates a SignatureFieldKit by transferring the underlying object of the rvalue reference to this SignatureFieldKit.
		 * 	\param in_kit An rvalue reference to a SignatureFieldKit to take the underlying object from. */
		SignatureFieldKit(SignatureFieldKit && in_kit);

		virtual ~SignatureFieldKit();

	static const HPS::Type staticType = HPS::Type::PublishSignatureFieldKit;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Creates a SignatureFieldKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a button unless a setting is overridden by the SignatureFieldKit passed to a function.
		 *	\return A SignatureFieldKit with the default settings. */
		static SignatureFieldKit	GetDefault();

		/*! Copies the source SignatureFieldKit into this SignatureFieldKit.
		 * 	\param in_kit The source SignatureFieldKit to copy. */
		void				Set(SignatureFieldKit const & in_kit);

		/*! Copies this SignatureFieldKit into the given SignatureFieldKit.
		 * 	\param out_kit The SignatureFieldKit to populate with the contents of this SignatureFieldKit. */
		void				Show(SignatureFieldKit & out_kit) const;

		/*! Copies the source SignatureFieldKit into this SignatureFieldKit.
		 * 	\param in_kit The source SignatureFieldKit to copy.
		 * 	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			operator=(SignatureFieldKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this SignatureFieldKit.
		 *	\param in_kit An rvalue reference to a SignatureFieldKit to take the underlying object from.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			operator=(SignatureFieldKit && in_kit);

		/*! Indicates whether this SignatureFieldKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this SignatureFieldKit, <span class='code'>false</span> otherwise. */
		bool				Empty() const;

		/*!	Check if the source SignatureFieldKit is equivalent to this SignatureFieldKit. 
		 *	\param in_kit The source SignatureFieldKit to compare to this SignatureFieldKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				Equals(SignatureFieldKit const & in_kit) const;

		/*!	Check if the source SignatureFieldKit is equivalent to this SignatureFieldKit. 
		 *	\param in_kit The source SignatureFieldKit to compare to this SignatureFieldKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool				operator==(SignatureFieldKit const & in_kit) const;

		/*!	Check if the source SignatureFieldKit is not equivalent to this SignatureFieldKit. 
		 *	\param in_kit The source SignatureFieldKit to compare to this SignatureFieldKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool				operator!=(SignatureFieldKit const & in_kit) const;


		/*!	Sets the name of the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#a7a508638a8d84d5e7326afdd2eb74165">A3DPDFDigitalSignatureData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetName(char const * in_name);

		/*!	Sets a builtin font for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#afa2cd0f88515d0726e7c116a8704bc92">A3DPDFDigitalSignatureData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#afa2cd0f88515d0726e7c116a8704bc92">A3DPDFDigitalSignatureData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the signature field.
		 *	\param in_style The style of font to use for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#afd1b0da01c65ea2a096f3832859781e8">A3DPDFDigitalSignatureData::m_iFontSize</a>.
		 *	\param in_size The font size for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#ae3f0f8b50cf34d0d52e593d516d30ad1">A3DPDFDigitalSignatureData::m_sTextColor</a>.
		 *	\param in_color The text color for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#a09b9f1bdf1a93bec885753651a768f31">A3DPDFDigitalSignatureData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#ad4b94ff3bb207077a28bb86ed0a7988f">A3DPDFDigitalSignatureData::m_eFormField</a>.
		 *	\param in_state The visibility for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#ad4b94ff3bb207077a28bb86ed0a7988f">A3DPDFDigitalSignatureData::m_eFormField</a>.
		 *	\param in_state The printability for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#a999498d09b28aed0d38aae2ad82ac127">A3DPDFDigitalSignatureData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the signature field is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#af9052704838668b16253398b462b7d30">A3DPDFDigitalSignatureData::m_bIsLocked</a>.
		 *	\param in_state Whether the signature field is locked.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetLock(bool in_state);

		/*!	Sets whether the signature field has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#abadaf28f1b3cdd678be2e69ef365ae5b">A3DPDFDigitalSignatureData::m_bHasBorder</a>.
		 *	\param in_state Whether the signature field has a border.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#a7843f39c7a4d73a8dc7b613471ce9e17">A3DPDFDigitalSignatureData::m_sBorderColor</a>.
		 *	\param in_color The border color of the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#ad54bdb93311e12493a2753a4941cc231">A3DPDFDigitalSignatureData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#a3786ac114f201d2c00f24353199ac643">A3DPDFDigitalSignatureData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the signature field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_digital_signature_data.html#aef88100e02a1001b0e6d6356e62cb5e3">A3DPDFDigitalSignatureData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="">A3DPDFDigitalSignatureData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			SetFillColor(RGBColor const & in_color);


		/*!	Removes the name setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetName();

		/*!	Removes the font setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetFillColor();

		/*! Removes all data from the signature field.
		 *	\return A reference to this SignatureFieldKit. */
		SignatureFieldKit &			UnsetEverything();


		/*! Shows the name for the signature field.
		 *	\param out_name The name of the signature field.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the font for the signature field.
		 *	\param out_type The type of font for the signature field.
		 *	\param out_font The built-in font for the signature field.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the signature field.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the signature field.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the signature field.
		 *	\param out_size The font size for the signature field.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the signature field.
		 *	\param out_color The text color for the signature field.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the signature field.
		 *	\param out_tooltip The tooltip text for the signature field.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the signature field.
		 *	\param out_state The visibility for the signature field.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the signature field.
		 *	\param out_state The printability for the signature field.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the signature field.
		 *	\param out_rotation The text rotation for the signature field.
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the signature field.
		 *	\param in_state Whether the signature field is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the signature field.
		 *	\param out_state Whether the signature field has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the signature field.
		 *	\param out_color The border color of the signature field.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the signature field.
		 *	\param out_thickness The border thickness for the signature field.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the signature field.
		 *	\param out_style The border style for the signature field.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the signature field.
		 *	\param out_color The fill color for the signature field.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;
	};

	/*! The TextFieldKit class is a user space object.  It acts as the container for all data that can be used to specify a text field for a Publish PDF. */
	class PUBLISH_API TextFieldKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty TextFieldKit object. */
		TextFieldKit();

		/*! The copy constructor creates a new TextFieldKit object that contains the same settings as the source TextFieldKit.
		 * 	\param in_kit The source TextFieldKit to copy. */
		TextFieldKit(TextFieldKit const & in_kit);

		/*! The move constructor creates a TextFieldKit by transferring the underlying object of the rvalue reference to this TextFieldKit.
		 * 	\param in_kit An rvalue reference to a TextFieldKit to take the underlying object from. */
		TextFieldKit(TextFieldKit && in_kit);

		virtual ~TextFieldKit();

	static const HPS::Type staticType = HPS::Type::PublishTextFieldKit;
		HPS::Type				ObjectType() const { return staticType; }

		/*! Creates a TextFieldKit which contains the default settings.  The returned object will not necessarily have values for every setting, but it will have them
		 *	where it is reasonable to have a default.  These values will be used for a text field unless a setting is overridden by the TextFieldKit passed to a function.
		 *	\return A TextFieldKit with the default settings. */
		static TextFieldKit		GetDefault();

		/*! Copies the source TextFieldKit into this TextFieldKit.
		 * 	\param in_kit The source TextFieldKit to copy. */
		void					Set(TextFieldKit const & in_kit);

		/*! Copies this TextFieldKit into the given TextFieldKit.
		 * 	\param out_kit The TextFieldKit to populate with the contents of this TextFieldKit. */
		void					Show(TextFieldKit & out_kit) const;

		/*! Copies the source TextFieldKit into this TextFieldKit.
		 * 	\param in_kit The source TextFieldKit to copy.
		 * 	\return A reference to this TextFieldKit. */
		TextFieldKit &			operator=(TextFieldKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this TextFieldKit.
		 *	\param in_kit An rvalue reference to a TextFieldKit to take the underlying object from.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			operator=(TextFieldKit && in_kit);

		/*! Indicates whether this TextFieldKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this TextFieldKit, <span class='code'>false</span> otherwise. */
		bool					Empty() const;

		/*!	Check if the source TextFieldKit is equivalent to this TextFieldKit. 
		 *	\param in_kit The source TextFieldKit to compare to this TextFieldKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(TextFieldKit const & in_kit) const;

		/*!	Check if the source TextFieldKit is equivalent to this TextFieldKit. 
		 *	\param in_kit The source TextFieldKit to compare to this TextFieldKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(TextFieldKit const & in_kit) const;

		/*!	Check if the source TextFieldKit is not equivalent to this TextFieldKit. 
		 *	\param in_kit The source TextFieldKit to compare to this TextFieldKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(TextFieldKit const & in_kit) const;



		/*!	Sets the name of the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#aa80c111e6238b9a636b2484e59a8e72d">A3DPDFTextFieldData::m_pcName</a>.
		 *	\param in_name UTF8-encoded name of the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetName(char const * in_name);

		/*!	Sets a builtin font for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a258a98b5920aba0002c65a9f4d33ce3f">A3DPDFTextFieldData::m_pcFontName</a>.
		 *	\param in_name The builtin font for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetFont(Text::Font::Name in_name);

		/*!	Sets a specific font for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a258a98b5920aba0002c65a9f4d33ce3f">A3DPDFTextFieldData::m_pcFontName</a>.
		 *	\param in_name UTF8-encoded font name for the text field.
		 *	\param in_style The style of font to use for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetFont(char const * in_name, Text::Font::Style in_style = Text::Font::Style::Regular);

		/*!	Sets the font size for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#aa81644de695e055465ebf2bc290b3de7">A3DPDFTextFieldData::m_iFontSize</a>.
		 *	\param in_size The font size for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetFontSize(int in_size);

		/*!	Sets the text color for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a7bda027e619d995d9fb83482c2e31b1d">A3DPDFTextFieldData::m_sTextColor</a>.
		 *	\param in_color The text color for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetTextColor(RGBColor const & in_color);

		/*!	Sets the tooltip for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#aa624cd8f6e77bd66550cca969401586f">A3DPDFTextFieldData::m_pcTooltip</a>.
		 *	\param in_tooltip UTF8-encoded tooltip text for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetTooltip(char const * in_tooltip);

		/*!	Sets the visibility for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#ab3a367c3b061889913e526183879d548">A3DPDFTextFieldData::m_eFormField</a>.
		 *	\param in_state The visibility for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetVisibility(bool in_state);

		/*!	Sets the printability for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#ab3a367c3b061889913e526183879d548">A3DPDFTextFieldData::m_eFormField</a>.
		 *	\param in_state The printability for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetPrintability(bool in_state);

		/*!	Sets the text rotation for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#afd78252ea7eb673aa1b7e015dbdd3f1c">A3DPDFTextFieldData::m_eTextOrientation</a>.
		 *	\param in_rotation The text rotation for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetTextRotation(Text::Rotation in_rotation);

		/*!	Sets whether the text field is locked.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a7cddd56b6a26f4e605218c94e109dc38">A3DPDFTextFieldData::m_bIsLocked</a>.
		 *	\param in_state Whether the text field is locked.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetLock(bool in_state);

		/*!	Sets whether the text field has a border.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a9b664261134e2f2843d16c47a759c805">A3DPDFTextFieldData::m_bHasBorder</a>.
		 *	\param in_state Whether the text field has a border.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetBorder(bool in_state);

		/*!	Sets the border color for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a45a6dc6ca0befa5c2fac4842d1dda5d0">A3DPDFTextFieldData::m_sBorderColor</a>.
		 *	\param in_color The border color of the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetBorderColor(RGBColor const & in_color);

		/*!	Sets the border thickness for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#ab039b181a012258aa4108f7f52b1acc9">A3DPDFTextFieldData::m_eThicknessBorder</a>.
		 *	\param in_thickness The border thickness for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetBorderThickness(Border::Thickness in_thickness);

		/*!	Sets the border style for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#ad192deaccbb00a0e91b127f7229d87aa">A3DPDFTextFieldData::m_eLineStyleBorder</a>.
		 *	\param in_style The border style for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetBorderStyle(Border::Style in_style);

		/*!	Sets the fill color for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a1b7ed09daf0bbdc1300a5c6a6ac297c3">A3DPDFTextFieldData::m_sFillColor</a>
		 *	and implicitly sets <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a8f3b7f3f36404f295ecf62942cdaa680">A3DPDFTextFieldData::m_bHasFillColor</a> to <span class='code'>true</span>.
		 *	\param in_color The fill color for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetFillColor(RGBColor const & in_color);

		/*!	Sets the default value for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a238cb8fc22eb59a5c5f3e62d21208644">A3DPDFTextFieldData::m_pcDefaultValue</a>.
		 *	\param in_value UT8-encoded default value for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetDefaultValue(char const * in_value);

		/*!	Sets the text justification for the text field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#ad528b189c2a0b627df1d72c773bbbde6">A3DPDFTextFieldData::m_eTextAlignment</a>.
		 *	\param in_justification The text justification for the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetTextJustification(HPS::Text::Justification in_justification);

		/*!	Sets whether the text field is multiline.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#adc977b31dd4d224a62ad1e37107b266b">A3DPDFTextFieldData::m_bMultiline</a>.
		 *	\param in_state Whether the text field is multiline.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetMultiline(bool in_state);

		/*!	Sets whether the text field can scroll.  This corresponds to the (inverted) value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#abc187316edc79d3918f6c5d39c113815">A3DPDFTextFieldData::m_bDoNotScroll</a>.
		 *	\param in_state Whether the text field can scroll.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetScrolling(bool in_state);

		/*!	Sets whether the text field is read only.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_text_field_data.html#a62c1657be16955023f55337ee47f6d5e">A3DPDFTextFieldData::m_bReadOnly</a>.
		 *	\param in_state Whether the text field can scroll.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			SetReadOnly(bool in_state);


		/*!	Removes the name setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetName();

		/*!	Removes the font setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetFont();

		/*!	Removes the font size setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetFontSize();

		/*!	Removes the text color setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetTextColor();

		/*!	Removes the tooltip setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetTooltip();

		/*!	Removes the visibility setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetVisibility();

		/*!	Removes the printability setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetPrintability();

		/*!	Removes the text rotation setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetTextRotation();

		/*!	Removes the lock setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetLock();

		/*!	Removes the border setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetBorder();

		/*!	Removes the border color setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetBorderColor();

		/*!	Removes the border thickness setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetBorderThickness();

		/*!	Removes the border style setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetBorderStyle();

		/*!	Removes the fill color setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetFillColor();

		/*!	Removes the default value setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetDefaultValue();

		/*!	Removes the text justification setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetTextJustification();

		/*!	Removes the multiline setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetMultiline();

		/*!	Removes the scrolling setting.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetScrolling();

		/*!	Removes the read-only setting.
		*	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetReadOnly();

		/*! Removes all data from the text field.
		 *	\return A reference to this TextFieldKit. */
		TextFieldKit &			UnsetEverything();


		/*! Shows the name for the text field.
		 *	\param out_name The name of the text field.
		 *	\return <span class='code'>true</span> if a name was specified, <span class='code'>false</span> otherwise. */
		bool				ShowName(UTF8 & out_name) const;

		/*!	Shows the font for the text field.
		 *	\param out_type The type of font for the text field.
		 *	\param out_font The built-in font for the text field.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::BuiltIn.
		 *	\param out_font_name The explicit font name for the text field.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\param out_style The font style for the text field.  This is only valid if <span class='code'>out_type</span> is Text::Font::Type::Explicit.
		 *	\return <span class='code'>true</span> if a font was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFont(Text::Font::Type & out_type, Text::Font::Name & out_font, UTF8 & out_font_name, Text::Font::Style & out_style) const;

		/*!	Shows the font size for the text field.
		 *	\param out_size The font size for the text field.
		 *	\return <span class='code'>true</span> if a font size was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFontSize(int & out_size) const;

		/*!	Shows the text color for the text field.
		 *	\param out_color The text color for the text field.
		 *	\return <span class='code'>true</span> if a text color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextColor(RGBColor & out_color) const;

		/*!	Shows the tooltip for the text field.
		 *	\param out_tooltip The tooltip text for the text field.
		 *	\return <span class='code'>true</span> if a tooltip was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTooltip(UTF8 & out_tooltip) const;

		/*!	Shows the visibility for the text field.
		 *	\param out_state The visibility for the text field.
		 *	\return <span class='code'>true</span> if a visibility was specified, <span class='code'>false</span> otherwise. */
		bool				ShowVisibility(bool & out_state) const;

		/*!	Shows the printability for the text field.
		 *	\param out_state The printability for the text field.
		 *	\return <span class='code'>true</span> if a printability was specified, <span class='code'>false</span> otherwise. */
		bool				ShowPrintability(bool & out_state) const;

		/*!	Shows the text rotation for the text field.
		 *	\param out_rotation The text rotation for the text field.
		 *	\return <span class='code'>true</span> if a rotation was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextRotation(Text::Rotation & out_rotation) const;

		/*!	Shows the lock setting for the text field.
		 *	\param in_state Whether the text field is locked.
		 *	\return <span class='code'>true</span> if a lock setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowLock(bool & out_state) const;

		/*!	Shows the border setting for the text field.
		 *	\param out_state Whether the text field has a border.
		 *	\return <span class='code'>true</span> if a border setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorder(bool & out_state) const;

		/*!	Shows the border color for the text field.
		 *	\param out_color The border color of the text field.
		 *	\return <span class='code'>true</span> if a border color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderColor(RGBColor & out_color) const;

		/*!	Shows the border thickness for the text field.
		 *	\param out_thickness The border thickness for the text field.
		 *	\return <span class='code'>true</span> if a border thickness was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderThickness(Border::Thickness & out_thickness) const;

		/*!	Shows the border style for the text field.
		 *	\param out_style The border style for the text field.
		 *	\return <span class='code'>true</span> if a border style was specified, <span class='code'>false</span> otherwise. */
		bool				ShowBorderStyle(Border::Style & out_style) const;

		/*!	Shows the fill color for the text field.
		 *	\param out_color The fill color for the text field.
		 *	\return <span class='code'>true</span> if a fill color was specified, <span class='code'>false</span> otherwise. */
		bool				ShowFillColor(RGBColor & out_color) const;

		/*!	Shows the default value for the text field.
		 *	\param out_value The default value for the text field.
		 *	\return <span class='code'>true</span> if a default value was specified, <span class='code'>false</span> otherwise. */
		bool				ShowDefaultValue(UTF8 & out_value) const;

		/*!	Shows the text justification for the text field.
		 *	\param out_justification The text justification for the text field.
		 *	\return <span class='code'>true</span> if a text justification was specified, <span class='code'>false</span> otherwise. */
		bool				ShowTextJustification(HPS::Text::Justification & out_justification) const;

		/*!	Shows the multiline setting for the text field.
		 *	\param out_state Whether the text field is multiline.
		 *	\return <span class='code'>true</span> if a multiline setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowMultiline(bool & out_state) const;

		/*!	Shows the scrolling setting for the text field.
		 *	\param out_state Whether the text field can scroll.
		 *	\return <span class='code'>true</span> if a scrolling setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowScrolling(bool & out_state) const;

		/*!	Shows the read-only setting for the text field.
		 *	\param out_state Whether the text field is read only.
		 *	\return <span class='code'>true</span> if a scrolling setting was specified, <span class='code'>false</span> otherwise. */
		bool				ShowReadOnly(bool & out_state) const;
	};

	/*! The DocumentKey class is a smart pointer to a Publish PDF document.  It allows direct interaction with a PDF document. */
	class PUBLISH_API DocumentKey : public Sprocket
	{
	public:
		/*! The default constructor creates an uninitialized DocumentKey object.  The Type() function will return Type::None. */
		DocumentKey();

		/*! The copy constructor creates a DocumentKey object that shares the underlying smart-pointer of the source DocumentKey.
		 *	\param in_that The source DocumentKey to copy. */
		DocumentKey(DocumentKey const & in_that);

		/*! The move constructor creates a DocumentKey by transferring the underlying object of the rvalue reference to this DocumentKey.
		 * 	\param in_kit An rvalue reference to a DocumentKey to take the underlying object from. */
		DocumentKey(DocumentKey && in_that);

		/*! Releases a reference to this DocumentKey.  When the last reference goes out of scope, the underlying PDF document will be closed
		 *	and any associated memory will be released by Publish if necessary.  This will not write the underlying PDF document out to disk. */
		virtual ~DocumentKey();

	static const HPS::Type staticType = HPS::Type::PublishDocumentKey;
		HPS::Type				ObjectType() const { return staticType; }

		/*! Share the underlying smart-pointer of the DocumentKey source.
		 *	\param in_that The DocumentKey source of the assignment.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			operator=(DocumentKey const & in_that);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this DocumentKey.
		 *	\param in_kit An rvalue reference to a DocumentKey to take the underlying object from.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			operator=(DocumentKey && in_that);

		/*! Share the underlying smart-pointer of the DocumentKey source.
		 *	\param in_that The DocumentKey source of the assignment.
		 *	\return A reference to this DocumentKey. */
		virtual void			Assign(DocumentKey const & in_that);

		/*!	Check if the source DocumentKey points to the same underlying impl as this DocumentKey.
		 *	\param in_that The source DocumentKey to compare to this DocumentKey.
		 *	\return <span class='code'>true</span> if the objects reference the same impl, <span class='code'>false</span> otherwise. */
		bool					Equals(DocumentKey const & in_that) const;

		/*!	Check if the source DocumentKey points to a different impl than this DocumentKey.
		 *	\param in_that The source DocumentKey to compare to this DocumentKey.
		 *	\return <span class='code'>true</span> if the objects reference different impls, <span class='code'>false</span> otherwise. */
		bool					operator!=(DocumentKey const & in_that) const;

		/*!	Check if the source DocumentKey points to the same underlying impl as this DocumentKey.
		 *	\param in_that The source DocumentKey to compare to this DocumentKey.
		 *	\return <span class='code'>true</span> if the objects reference the same impl, <span class='code'>false</span> otherwise. */
		bool					operator==(DocumentKey const & in_that) const;

		/*! Closes the PDF document underlying this DocumentKey and frees the associated memory in Publish.  This will effectively invalidate all references to
		 *	the underlying PDF document (including other DocumentKey or PageControl objects which refer to the same document).  This will not write the underlying
		 *	PDF document out to disk. */
		void					Delete();


		/*! Adds the given page to the document.  This will append the page after any existing pages on the document.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_page Page to add to the document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddPage(PageKit const & in_page);

		/*! Adds the given pages to the document.  This will append the pages after any existing pages on the document.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_count Size of the Following array.
		 *	\param in_pages Pages to add to the document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddPages(size_t in_count, PageKit const in_pages[]);

		/*! Adds the given pages to the document.  This will append the pages after any existing pages on the document.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaeae4478957db2ff1e61c850ced4aad89">A3DPDFDocumentAppendNewPage</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga1b57273ec442dbe92ca20d858f5d5e12">A3DPDFDocumentAppendPageFromPDFFileEx</a>.
		 *	\param in_pages Pages to add to the document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddPages(PageKitArray const & in_pages);

		/*! Sets the title, author, subject and creator for this document.  These corresponds to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a23a95fae20138d1c2b41e72d1e023f65">A3DPDFDocumentInformationData::m_pcTitle</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a80251b9b9e2a5943f44841cb9c996c36">A3DPDFDocumentInformationData::m_pcAuthor</a>,
		 *	<a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a7a2428427055c10dd5b5b625e1a3ddf1">A3DPDFDocumentInformationData::m_pcSubject</a>,
		 *	and <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/struct_a3_d_p_d_f_document_information_data.html#a233e35e235385a13f5f26153223aed1a">A3DPDFDocumentInformationData::m_pcCreator</a> (respectively).
		 *	\param in_title UTF8-encoded title string to set on the document.
		 *	\param in_author UTF8-encoded author string to set on the document.
		 *	\param in_subject UTF8-encoded subject string to set on the document.
		 *	\param in_creator UTF8-encoded creator string to set on the document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			SetInformation(char const * in_title, char const * in_author, char const * in_subject, char const * in_creator);

		/*! Sets the passwords for this document.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3caa56b7a947ac2b42f2e9c66ede5aa4">A3DPDFDocumentSetPassword</a>.
		 *	\param in_user_password UTF8-encoded password string required when opening, modifying or printing the document.  If an empty string is specified, no password will be required.
		 *	\param in_owner_password UTF8-encoded password string required when changing security features of document.  If an empty string is specified, no password will be required.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			SetPasswords(char const * in_user_password, char const * in_owner_password);

		/*! Adds a named JavaScript source or file to the document.  If there is an existing script on the document with the given name, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga4e6020563c56827aa43faf9566f043df">A3DPDFDocumentAddJavascriptFromString</a>.
		 *	\param in_script_name UTF8-encoded script name to be added to the document.
		 *	\param in_source UTF8-encoded JavaScript source or file (depending on the following argument) to be added to the document.
		 *	\param in_type The types of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddJavaScript(char const * in_script_name, char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Adds a list of named JavaScript sources or files to the document.  If there is an existing script on the document with a name which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga4e6020563c56827aa43faf9566f043df">A3DPDFDocumentAddJavascriptFromString</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_script_names Script names to be added to the document.
		 *	\param in_sources JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be added to the document.
		 *	\param in_types Types for each item in the preceding argument array.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddJavaScript(size_t in_count, UTF8 const in_script_names[], UTF8 const in_sources[], Source::Type const in_types[]);

		/*! Adds a list of named JavaScript sources or files to the document.  If there is an existing script on the document with a name which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga4e6020563c56827aa43faf9566f043df">A3DPDFDocumentAddJavascriptFromString</a>.
		 *	\param in_script_names Script names to be added to the document.
		 *	\param in_sources JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be added to the document.
		 *	\param in_types Types for each item in the preceding argument array.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddJavaScript(UTF8Array const & in_script_names, UTF8Array const & in_sources, SourceTypeArray const & in_types);

		/*! Adds an attachment to the document.  If there is an existing attachment on the document with the given filename, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3f4bcf1bf99759ba44543b5ab7462b99">A3DPDFDocumentAddFileAttachment</a>.
		 *	\param in_filename UTF8-encoded filename of the file to be attached.
		 *	\param in_description UTF8-encoded description of the file to be attached.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddAttachment(char const * in_filename, char const * in_description);

		/*! Adds a list of attachments to the document.  If there is an existing attachment on the document with a filename which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3f4bcf1bf99759ba44543b5ab7462b99">A3DPDFDocumentAddFileAttachment</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_filenames Filenames of the files to be attached.
		 *	\param in_descriptions Descriptions of the files to be attached.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddAttachments(size_t in_count, UTF8 const in_filenames[], UTF8 const in_descriptions[]);

		/*! Adds a list of attachments to the document.  If there is an existing attachment on the document with a filename which is in the given list, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga3f4bcf1bf99759ba44543b5ab7462b99">A3DPDFDocumentAddFileAttachment</a>.
		 *	\param in_filenames Filenames of the files to be attached.
		 *	\param in_descriptions Descriptions of the files to be attached.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddAttachments(UTF8Array const & in_filenames, UTF8Array const & in_descriptions);

		/*! Adds an icon image to the document.  If there is an existing icon image on the document with the given name, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga7acb308303e1a5f8092e2fa368999048">A3DPDFDocumentAddImageAsIcon</a>.
		 *	\param in_name UTF8-encoded name of the icon image to be attached.
		 *	\param in_image Image to be attached as an icon.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddIconImage(char const * in_name, ImageKit const & in_image);

		/*! Adds icon images to the document.  If there is an existing icon image on the document with a name which is in the given list, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga7acb308303e1a5f8092e2fa368999048">A3DPDFDocumentAddImageAsIcon</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_names Names of the icon images to be attached.
		 *	\param in_images Images to be attached as icons.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddIconImages(size_t in_count, UTF8 const in_names[], ImageKit const in_images[]);

		/*! Adds icon images to the document.  If there is an existing icon image on the document with a name which is in the given list, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#ga7acb308303e1a5f8092e2fa368999048">A3DPDFDocumentAddImageAsIcon</a>.
		 *	\param in_names Names of the icon images to be attached.
		 *	\param in_images Images to be attached as icons.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			AddIconImages(UTF8Array const & in_names, ImageKitArray const & in_images);

		/*! Sets document permissions. This correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaa3cd3a52f56a9f817801f8b021afa64a">A3DPDFDocumentSetDocumentPermissions</a>.
		 *	\param in_count Size of the following array.
		 *	\param in_permissions The permissions to be set on the document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			SetPermissions(size_t in_count, Permission::Type const in_permissions[]);

		/*! Sets document permissions. This correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gaa3cd3a52f56a9f817801f8b021afa64a">A3DPDFDocumentSetDocumentPermissions</a>.
		 *	\param in_permissions The permissions to be set on the document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			SetPermissions(PermissionTypeArray const & in_permissions);


		/*! Gets the number of pages in the underlying PDF document. */
		size_t					GetPageCount() const;

		/*! Removes the page at the given index from the underlying PDF document.
		 *	\param in_index The index of the page to remove from the underlying PDF document.  The index of the first page is 0.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			RemovePage(size_t in_index);

		/*! Removes the specified number of pages starting at the given index from the underlying PDF document.
		 *	\param in_start The first index at which to start to removing pages from the underlying PDF document.  The index of the first page is 0.
		 *	\param in_count The number of pages to remove from the underlying PDF document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			RemovePages(size_t in_start, size_t in_count);

		/*! Removes the pages at the given indices from the underlying PDF document.
		 *	\param in_count Size of the following array.
		 *	\param in_indices The indices of pages to remove from the underlying PDF document.  The index of the first page is 0.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			RemovePages(size_t in_count, size_t const in_indices[]);

		/*! Removes the pages at the given indices from the underlying PDF document.
		 *	\param in_indices The indices of pages to remove from the underlying PDF document.  The index of the first page is 0.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			RemovePages(SizeTArray const & in_indices);

		/*! Removes all pages from the underlying PDF document.
		 *	\return A reference to this DocumentKey. */
		DocumentKey &			RemoveAllPages();

		/*! Returns a control that allows the user to manipulate and query details of a page at the given index in the underlying PDF document.
		 *	\param in_index The index of the page for which to create a PageControl.  The index of the first page is 0.
		 *	\return A PageControl object for the specified page. */
		PageControl				GetPageControl(size_t in_index);

		/*! Returns a control that allows the user to manipulate and query details of a page at the given index in the underlying PDF document.
		 *	\param in_index The index of the page for which to create a PageControl.  The index of the first page is 0.
		 *	\return A PageControl object for the specified page. */
		PageControl const		GetPageControl(size_t in_index) const;
	};

	/*! The PageControl class is a smart pointer to a page within a Publish PDF document.  It allows direct interaction with a PDF page. */
	class PUBLISH_API PageControl : public Sprocket
	{
	public:
		/*! The default constructor creates an uninitialized PageControl object.  The Type() function will return Type::None. */
		PageControl();

		/*! The copy constructor creates a PageControl object that shares the underlying smart-pointer of the source PageControl.
		 *	\param in_that The source PageControl to copy. */
		PageControl(PageControl const & in_that);

		/*! The move constructor creates a PageControl by transferring the underlying object of the rvalue reference to this PageControl.
		 * 	\param in_kit An rvalue reference to a PageControl to take the underlying object from. */
		PageControl(PageControl && in_that);

		virtual ~PageControl();

	static const HPS::Type staticType = HPS::Type::PublishPageControl;
		HPS::Type			ObjectType() const { return staticType; }

		/*! Share the underlying smart-pointer of the PageControl source.
		 *	\param in_that The PageControl source of the assignment.
		 *	\return A reference to this PageControl. */
		PageControl &		operator=(PageControl const & in_that);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this PageControl.
		 *	\param in_kit An rvalue reference to a PageControl to take the underlying object from.
		 *	\return A reference to this PageControl. */
		PageControl &		operator=(PageControl && in_kit);


		/*! Adds an annotation to the page.  This will not replace any existing annotations on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_annotation Annotation to add to the page.
		 *	\param in_location Annotation location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddAnnotation(AnnotationKit const & in_annotation, IntRectangle const & in_location);

		/*! Adds a list of annotations to the page.  This will not replace any existing annotations on the page.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_annotations Annotations to add to the page.
		 *	\param in_locations Annotation locations in points relative to bottom left of the page.
		 *	\return A reference to this PageControl. */
		PageControl &		AddAnnotations(size_t in_count, AnnotationKit const in_annotations[], IntRectangle const in_locations[]);

		/*! Adds a list of annotations to the page.  This will not replace any existing annotations on the page.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga7dfdb3dab2c487594a96cdba21f8295f">A3DPDFPageInsert3DAnnot</a>.
		 *	\param in_annotations Annotations to add to the page.
		 *	\param in_locations Annotation locations in points relative to bottom left of the page.
		 *	\return A reference to this PageControl. */
		PageControl &		AddAnnotations(AnnotationKitArray const & in_annotations, IntRectangleArray const & in_locations);

		/*! Adds a text string to the page.  This will not replace any text on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_text Text to add to the page.
		 *	\param in_location Text location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddText(TextKit const & in_text, IntRectangle const & in_location);

		/*! Adds text strings to the page.  This will not replace any text on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_text Text to add to the page.
		 *	\param in_locations Text locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddText(size_t in_count, TextKit const in_text[], IntRectangle const in_locations[]);

		/*! Adds text strings to the page.  This will not replace any text on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gafd4ece4c88ce91b60228a4e72988b1ff">A3DPDFPageInsertText</a>.
		 *	\param in_text Text to add to the page.
		 *	\param in_locations Text locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddText(TextKitArray const & in_text, IntRectangleArray const & in_locations);

		/*! Adds an image to the page.  This will not replace any images on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_image Image to add to the page.
		 *	\param in_location Image location in points relative to bottom left of the page.  Only the left and bottom fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddImage(ImageKit const & in_image, IntRectangle const & in_location);

		/*! Adds a list of images to the page.  This will not replace any images on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_images Images to add to the page.
		 *	\param in_locations Image locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddImages(size_t in_count, ImageKit const in_images[], IntRectangle const in_locations[]);

		/*! Adds a list of images to the page.  This will not replace any images on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga4a0580850d8442f86b2af8856c02654a">A3DPDFPageInsertImage</a>.
		 *	\param in_images Images to add to the page.
		 *	\param in_locations Image locations in points relative to bottom left of the page.  Only the left and bottom fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddImages(ImageKitArray const & in_images, IntRectangleArray const & in_locations);

		/*! Adds a table to the page.  This will not replace any tables on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_table Table to add to the page.
		 *	\param in_location Table location in points relative to upper left of the page.  Only the left and top fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddTable(TableKit const & in_table, IntRectangle const & in_location);

		/*! Adds a list of tables to the page.  This will not replace any tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_tables Tables to add to the page.
		 *	\param in_locations Table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddTables(size_t in_count, TableKit const in_tables[], IntRectangle const in_locations[]);

		/*! Adds a list of tables to the page.  This will not replace any tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae924b1a89f5cfa5cdbe2913480597b21">A3DPDFPageInsertTable</a>.
		 *	\param in_tables Tables to add to the page.
		 *	\param in_locations Table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddTables(TableKitArray const & in_tables, IntRectangleArray const & in_locations);

		/*! Adds a link to the page.  This will not replace any links on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_link Link to add to the page.
		 *	\param in_location Link location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddLink(LinkKit const & in_link, IntRectangle const & in_location);

		/*! Adds a list of links to the page.  This will not replace any links on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_links Links to add to the page.
		 *	\param in_locations Link locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddLinks(size_t in_count, LinkKit const in_links[], IntRectangle const in_locations[]);

		/*! Adds a list of links to the page.  This will not replace any links on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1822ea723295e01b53e222864b8efd31">A3DPDFPageInsertLink</a>.
		 *	\param in_links Links to add to the page.
		 *	\param in_locations Link locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddLinks(LinkKitArray const & in_links, IntRectangleArray const & in_locations);

		/*! Adds a button to the page.  This will not replace any buttons on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_button Button to add to the page.
		 *	\param in_location Button location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddButton(ButtonKit const & in_button, IntRectangle const & in_location);

		/*! Adds a list of buttons to the page.  This will not replace any buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_buttons Buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddButtons(size_t in_count, ButtonKit const in_buttons[], IntRectangle const in_locations[]);

		/*! Adds a list of buttons to the page.  This will not replace any buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertButton</a>.
		 *	\param in_buttons Buttons to add to the page.
		 *	\param in_locations Button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddButtons(ButtonKitArray const & in_buttons, IntRectangleArray const & in_locations);

		/*! Adds a text field to the page.  This will not replace any text fields on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_text_field Text field to add to the page.
		 *	\param in_location Text field location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddTextField(TextFieldKit const & in_text_field, IntRectangle const & in_location);

		/*! Adds a list of text fields to the page.  This will not replace any text fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_text_fields Text fields to add to the page.
		 *	\param in_locations Text field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddTextFields(size_t in_count, TextFieldKit const in_text_fields[], IntRectangle const in_locations[]);

		/*! Adds a list of text fields to the page.  This will not replace any text fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga04003d7a34557c95e84c462525ef1728">A3DPDFPageInsertTextField</a>.
		 *	\param in_text_fields Text fields to add to the page.
		 *	\param in_locations Text field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddTextFields(TextFieldKitArray const & in_text_fields, IntRectangleArray const & in_locations);

		/*! Adds a slide table to the page.  This will not replace any slide tables on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_slide_table Slide table to add to the page.
		 *	\param in_location Slide table location in points relative to upper left of the page.  Only the left and top fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddSlideTable(SlideTableKit const & in_slide_table, IntRectangle const & in_location);

		/*! Adds a list of slide tables to the page.  This will not replace any slide tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_slide_tables Slide tables to add to the page.
		 *	\param in_locations Slide table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddSlideTables(size_t in_count, SlideTableKit const in_slide_tables[], IntRectangle const in_locations[]);

		/*! Adds a list of slide tables to the page.  This will not replace any slide tables on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__document__module.html#gad3131986068fd4ef196d26f841093b6c">A3DPDFDefineSlideTable</a>.
		 *	\param in_slide_tables Slide tables to add to the page.
		 *	\param in_locations Slide table locations in points relative to upper left of the page.  Only the left and top fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddSlideTables(SlideTableKitArray const & in_slide_tables, IntRectangleArray const & in_locations);

		/*! Adds a check box to the page.  This will not replace any check boxes on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_check_box Check box to add to the page.
		 *	\param in_location Check box location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddCheckBox(CheckBoxKit const & in_check_box, IntRectangle const & in_location);

		/*! Adds a list of check boxes to the page.  This will not replace any check boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_check_boxes Check boxes to add to the page.
		 *	\param in_locations Check box locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddCheckBoxes(size_t in_count, CheckBoxKit const in_check_boxes[], IntRectangle const in_locations[]);

		/*! Adds a list of check boxes to the page.  This will not replace any check boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertCheckBox</a>.
		 *	\param in_check_boxes Check boxes to add to the page.
		 *	\param in_locations Check box locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddCheckBoxes(CheckBoxKitArray const & in_check_boxes, IntRectangleArray const & in_locations);

		/*! Adds a radio button to the page.  This will not replace any radio buttons on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_radio_button Radio button to add to the page.
		 *	\param in_location Radio button location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddRadioButton(RadioButtonKit const & in_radio_button, IntRectangle const & in_location);

		/*! Adds a list of radio buttons to the page.  This will not replace any radio buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_radio_buttons Radio buttons to add to the page.
		 *	\param in_locations Radio button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddRadioButtons(size_t in_count, RadioButtonKit const in_radio_buttons[], IntRectangle const in_locations[]);

		/*! Adds a list of radio buttons to the page.  This will not replace any radio buttons on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertRadioButton</a>.
		 *	\param in_radio_buttons Radio buttons to add to the page.
		 *	\param in_locations Radio button locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddRadioButtons(RadioButtonKitArray const & in_radio_buttons, IntRectangleArray const & in_locations);

		/*! Adds a list box to the page.  This will not replace any list boxes on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertListBox</a>.
		 *	\param in_list_box List box to add to the page.
		 *	\param in_location List box location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddListBox(ListBoxKit const & in_list_box, IntRectangle const & in_location);

		/*! Adds a list of list boxes to the page.  This will not replace any list boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertListBox</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_list_boxes List boxes to add to the page.
		 *	\param in_locations List box locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddListBoxes(size_t in_count, ListBoxKit const in_list_boxes[], IntRectangle const in_locations[]);

		/*! Adds a list of list boxes to the page.  This will not replace any list boxes on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertListBox</a>.
		 *	\param in_list_boxes List boxes to add to the page.
		 *	\param in_locations List box locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddListBoxes(ListBoxKitArray const & in_list_boxes, IntRectangleArray const & in_locations);

		/*! Adds a drop down list to the page.  This will not replace any drop down lists on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_drop_down_list Drop down list to add to the page.
		 *	\param in_location Drop down list location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddDropDownList(DropDownListKit const & in_drop_down_list, IntRectangle const & in_location);

		/*! Adds a list of drop down lists to the page.  This will not replace any drop down lists on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_drop_down_lists Drop down lists to add to the page.
		 *	\param in_locations Drop down list locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddDropDownLists(size_t in_count, DropDownListKit const in_drop_down_lists[], IntRectangle const in_locations[]);

		/*! Adds a list of drop down lists to the page.  This will not replace any drop down lists on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertDropDownList</a>.
		 *	\param in_drop_down_lists Drop down lists to add to the page.
		 *	\param in_locations Drop down list locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddDropDownLists(DropDownListKitArray const & in_drop_down_lists, IntRectangleArray const & in_locations);

		/*! Adds a signature field to the page.  This will not replace any signature fields on the page.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_signature_field Signature field to add to the page.
		 *	\param in_location Signature field location in points relative to bottom left of the page.  All fields in the location will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddSignatureField(SignatureFieldKit const & in_signature_field, IntRectangle const & in_location);

		/*! Adds a list of signature fields to the page.  This will not replace any signature fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_count Size of the following arrays.
		 *	\param in_signature_fields Signature fields to add to the page.
		 *	\param in_locations Signature field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddSignatureFields(size_t in_count, SignatureFieldKit const in_signature_fields[], IntRectangle const in_locations[]);

		/*! Adds a list of signature fields to the page.  This will not replace any signature fields on the page.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga1727c2d83b817942b2a9a9759c7aa7c1">A3DPDFPageInsertSignatureField</a>.
		 *	\param in_signature_fields Signature fields to add to the page.
		 *	\param in_locations Signature field locations in points relative to bottom left of the page.  All fields in the locations will be used.
		 *	\return A reference to this PageControl. */
		PageControl &		AddSignatureFields(SignatureFieldKitArray const & in_signature_fields, IntRectangleArray const & in_locations);


		/*! Sets an annotation on the given form field.  If there is an existing annotation on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gadf6f14b2368a856f31169dc03e0c501e">A3DPDFPageFieldSet3DAnnot</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the annotation.
		 *	\param in_annotation The annotation to be set on the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		SetAnnotationByField(char const * in_field, AnnotationKit const & in_annotation);

		/*! Sets annotations on the given form fields.  If there is an existing annotation on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gadf6f14b2368a856f31169dc03e0c501e">A3DPDFPageFieldSet3DAnnot</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the annotations.
		 *	\param in_annotations The annotations to be set on the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetAnnotationsByField(size_t in_count, UTF8 const in_fields[], AnnotationKit const in_annotations[]);

		/*! Sets annotations on the given form fields.  If there is an existing annotation on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gadf6f14b2368a856f31169dc03e0c501e">A3DPDFPageFieldSet3DAnnot</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_fields The form field names for the annotations.
		 *	\param in_annotations The annotations to be set on the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetAnnotationsByField(UTF8Array const & in_fields, AnnotationKitArray const & in_annotations);

		/*! Sets an image on the icon at the given form field.  If there is an existing image on an icon at the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga6051e30d012d9dd265fe8eb3a01f317f">A3DPDFPageFieldButtonSetIcon</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the icon.
		 *	\param in_image The image to be set on the icon at the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		SetButtonIconByField(char const * in_field, ImageKit const & in_image);

		/*! Sets images on the icons at the given form fields.  If there is an existing image on an icon at a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga6051e30d012d9dd265fe8eb3a01f317f">A3DPDFPageFieldButtonSetIcon</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the icons.
		 *	\param in_images The images to be set on the icons at the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetButtonIconsByField(size_t in_count, UTF8 const in_fields[], ImageKit const in_images[]);

		/*! Sets images on the icons at the given form fields.  If there is an existing image on an icon at a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga6051e30d012d9dd265fe8eb3a01f317f">A3DPDFPageFieldButtonSetIcon</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_fields The form field names for the icons.
		 *	\param in_images The images to be set on the icons at the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetButtonIconsByField(UTF8Array const & in_fields, ImageKitArray const & in_images);

		/*! Sets a label on the given form field.  If there is an existing label on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldButtonSetLabel</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the label.
		 *	\param in_label UTF8-encoded label to be set on the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		SetButtonLabelByField(char const * in_field, char const * in_label);

		/*! Sets labels on the given form fields.  If there is an existing label on a given form field, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldButtonSetLabel</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the labels.
		 *	\param in_labels The labels to be set on the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetButtonLabelsByField(size_t in_count, UTF8 const in_fields[], UTF8 const in_labels[]);

		/*! Sets labels on the given form fields.  If there is an existing label on a given form field, it will get overwritten.  These correspond to the values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldButtonSetLabel</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_fields The form field names for the labels.
		 *	\param in_labels The labels to be set on the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetButtonLabelsByField(UTF8Array const & in_fields, UTF8Array const & in_labels);

		/*! Adds an item to the list at the given form field.  The item to will be appended to any existing items in the list at the given form field.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_value UTF8-encoded value for the item to be added to the list at the given form field.
		 *	\param in_export_value UTF8-encoded export value for the item to be added to the list at the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		AddListItemByField(char const * in_field, char const * in_value, char const * in_export_value);

		/*! Adds items to the list at the given form field.  The items to will be appended to any existing items in the list at the given form field.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_count Size of the following arrays.
		 *	\param in_values The values for the items to be added to the list at the given form field.
		 *	\param in_export_values The export values for the items to be added to the list at the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		AddListItemsByField(char const * in_field, size_t in_count, UTF8 const in_values[], UTF8 const in_export_values[]);

		/*! Adds items to the list at the given form field.  The items to will be appended to any existing items in the list at the given form field.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gae268fe9055181033df8eb554bf6d9ae6">A3DPDFPageFieldListAddItem</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the list.
		 *	\param in_values The values for the items to be added to the list at the given form field.
		 *	\param in_export_values The export values for the items to be added to the list at the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		AddListItemsByField(char const * in_field, UTF8Array const & in_values, UTF8Array const & in_export_values);

		/*! Sets a JavaScript source or file on the given form field.  If there is any existing JavaScript source or file at the given form field, it will be overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac02217c03ab6cc02581362eab77c12f6">A3DPDFPageFieldSetActionJavascriptFromString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacf5655c5eb503b4c0fa6f719699a90ee">A3DPDFPageFieldSetActionJavascriptFromFile</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the JavaScript action.
		 *	\param in_source UTF8-encoded JavaScript source or file (depending on the following argument) to be set on the given form field.
		 *	\param in_type The type of the preceding argument.  Defaults to Source::Type::Code.
		 *	\return A reference to this PageControl. */
		PageControl &		SetJavaScriptActionByField(char const * in_field, char const * in_source, Source::Type in_type = Source::Type::Code);

		/*! Sets JavaScript sources and/or files on the given form fields.  If there is any existing JavaScript source or file at a given form field, it will be overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac02217c03ab6cc02581362eab77c12f6">A3DPDFPageFieldSetActionJavascriptFromString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacf5655c5eb503b4c0fa6f719699a90ee">A3DPDFPageFieldSetActionJavascriptFromFile</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the JavaScript actions.
		 *	\param in_sources The JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be set on the corresponding form fields.
		 *	\param in_types The types for each item in the preceding argument array.
		 *	\return A reference to this PageControl. */
		PageControl &		SetJavaScriptActionsByField(size_t in_count, UTF8 const in_fields[], UTF8 const in_sources[], Source::Type const in_types[]);

		/*! Sets JavaScript sources and/or files on the given form fields.  If there is any existing JavaScript source or file at a given form field, it will be overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gac02217c03ab6cc02581362eab77c12f6">A3DPDFPageFieldSetActionJavascriptFromString</a>
		 *	or <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacf5655c5eb503b4c0fa6f719699a90ee">A3DPDFPageFieldSetActionJavascriptFromFile</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_fields The form field names for the JavaScript actions.
		 *	\param in_sources The JavaScript sources and/or files (depending on the corresponding entry in the following argument) to be set on the corresponding form fields.
		 *	\param in_types The types for each item in the preceding argument array.
		 *	\return A reference to this PageControl. */
		PageControl &		SetJavaScriptActionsByField(UTF8Array const & in_fields, UTF8Array const & in_sources, SourceTypeArray const & in_types);

		/*! Sets whether the object at the given form field should be visible.  If there is an existing visibility on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacdabb9f6e7fc6a887de9516148549b8d">A3DPDFPageFieldSetVisibility</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name on which to set visibility.
		 *	\param in_state Whether the object at the at the given form field should be visible.
		 *	\return A reference to this PageControl. */
		PageControl &		SetVisibilityByField(char const * in_field, bool in_state);

		/*! Sets whether the objects at the given form fields should be visible.  If there is an existing visibility on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacdabb9f6e7fc6a887de9516148549b8d">A3DPDFPageFieldSetVisibility</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names on which to set visibility.
		 *	\param in_states Whether the objects at the at the corresponding form fields should be visible.
		 *	\return A reference to this PageControl. */
		PageControl &		SetVisibilitiesByField(size_t in_count, UTF8 const in_fields[], bool const in_states[]);

		/*! Sets whether the objects at the given form fields should be visible.  If there is an existing visibility on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#gacdabb9f6e7fc6a887de9516148549b8d">A3DPDFPageFieldSetVisibility</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_fields The form field names on which to set visibility.
		 *	\param in_states Whether the objects at the at the corresponding form fields should be visible.
		 *	\return A reference to this PageControl. */
		PageControl &		SetVisibilitiesByField(UTF8Array const & in_fields, BoolArray const & in_states);

		/*! Sets a text value on the given form field.  If there is an existing text value on the given form field, it will get overwritten.  This corresponds to the value that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga8f171e2004b51ae1ebcec2eceb773b00">A3DPDFPageFieldTextSetValue</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_field UTF8-encoded form field name for the text value.
		 *	\param in_value UTF8-encoded text value to be set on the given form field.
		 *	\return A reference to this PageControl. */
		PageControl &		SetTextValueByField(char const * in_field, char const * in_value);

		/*! Sets text values on the given form fields.  If there is an existing text value on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga8f171e2004b51ae1ebcec2eceb773b00">A3DPDFPageFieldTextSetValue</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_count Size of the following arrays.
		 *	\param in_fields The form field names for the text values.
		 *	\param in_values The text values to be set on the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetTextValuesByField(size_t in_count, UTF8 const in_fields[], UTF8 const in_values[]);

		/*! Sets text values on the given form fields.  If there is an existing text value on a given form field, it will get overwritten.  These correspond to values that will be passed to <a target="_blank" href="http://docs.techsoft3d.com/publish/latest/build/api_ref/group__a3d__publish__page__module.html#ga8f171e2004b51ae1ebcec2eceb773b00">A3DPDFPageFieldTextSetValue</a>.
		 *	This will only have an effect if the underlying page has a form field with the given name and is of the appropriate type.
		 *	\param in_fields The form field names for the text values.
		 *	\param in_values The text values to be set on the corresponding form fields.
		 *	\return A reference to this PageControl. */
		PageControl &		SetTextValuesByField(UTF8Array const & in_fields, UTF8Array const & in_values);


		/*! Shows the PDF form fields on the page associated with this PageControl.
		 *	\param out_names The names of the PDF form fields on the associated page.
		 *	\param out_locations The locations of the PDF form fields on the associated page.
		 *	\param out_types The types of the PDF form fields on the associated page.
		 *	\return <span class='code'>true</span> if there are any PDF form fields on the associated page, <span class='code'>false</span> otherwise. */
		bool				ShowFields(UTF8Array & out_names, IntRectangleArray & out_locations, FieldTypeArray & out_types) const;
	};


	class ExportOptionsKit;

	/*!	The File class provides functions to export Model objects to 3D PDFs via Publish. */
	class PUBLISH_API File
	{
	public:
		/*! Creates a DocumentKey based on the given PDF file.  The returned object will allow direct interaction with the underlying in-memory copy of that PDF document and any pages in it.
		 *	If no filename is specified, an empty PDF document will be created.  The in-memory PDF document can be written to disk via the File::Export function.
		 * 	\param in_file_name The name of the PDF file from which to create an in-memory copy.  Defaults to <span class='code'>nullptr</span>, which results in an empty in-memory PDF document.
		 *	\return A DocumentKey handle for the in-memory PDF document. */
		static DocumentKey CreateDocument(char const * in_file_name = nullptr);

		/*! Performs a synchronous Publish export of the given CAD Model with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(CADModel const & in_cad_model, char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given KeyPathArray with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_key_paths The KeyPaths containing the data to export.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(KeyPathArray const & in_key_paths, char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given KeyPathArray with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_count The number of key paths in in_key_paths.
		* 	\param in_key_paths The KeyPaths containing the data to export.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(size_t in_count, KeyPath const in_key_paths[], char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given KeyPath with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_key_path The KeyPath containing the data to export.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(KeyPath const & in_key_path, char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given CAD Model with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.
		* 	\param in_additional_key_paths An array of KeyPaths for which PRC data will be created and which will be added to in_cad_model during export. Use this
		*	parameter to specify non-Exchange data you have added to the scene which you want to export.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(CADModel const & in_cad_model, KeyPathArray const & in_additional_key_paths, char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given CAD Model with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.
		* 	\param in_count The size of in_additional_key_paths.
		* 	\param in_additional_key_paths An array of KeyPaths for which PRC data will be created and which will be added to in_cad_model during export. Use this
		*	parameter to specify non-Exchange data you have added to the scene which you want to export.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(CADModel const & in_cad_model, size_t in_count, KeyPath const in_additional_key_paths[], char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given AnnotationKit with the provided options to a given filename.  This will create a single page PDF document with a single 3D annotation.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_annotation The AnnotationKit containing the data to export.  If the Model in the source SprocketPath for the AnnotationKit corresponds to a CADModel which was imported via Exchange,
		*		the underlying Exchange data for the CADModel will be embedded in the PDF, otherwise, the 3D data will be generated from the tessellation in the Visualize scene graph for the Model.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(AnnotationKit const & in_annotation, char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given DocumentKit with the provided options to a given filename.  This will create a PDF document based on the DocumentKit.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_document The DocumentKit containing the data to export.  If any Model in the source SprocketPath for any AnnotationKit in the DocumentKit corresponds to a CADModel which was imported
		*		via Exchange, the underlying Exchange data for the CADModel will be embedded in the PDF, otherwise, the 3D data will be generated from the tessellation in the Visualize scene graph for the Model.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_options The options controlling the export of the 3D PDF data. */
		static void ExportPDF(DocumentKit const & in_document, char const * in_file_name, ExportOptionsKit const & in_options);

		/*! Performs a synchronous Publish export of the given DocumentKey to a given filename.  This will not close the PDF document underlying the DocumentKey, rather it will just create
		*	a copy of it on disk.  The DocumentKey can still be used after this call to interact with the in-memory PDF document.
		*	\param in_document The DocumentKey from which to create a PDF file on disk.
		* 	\param in_file_name The name of the file to write the 3D PDF data to.  This filename should be a fully-qualified path, otherwise the export may fail. */
		static void ExportPDF(DocumentKey const & in_document, char const * in_file_name);


		/*! Performs a synchronous Publish export of the given CAD Model with the provided options to a given filename.
		*   \warning This feature is only supported on 64bit builds of HOOPS Visualize. An InvalidOperationException will be thrown if this function is used on 32bit builds.
		*   This will create an HTML page using the provided template.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.
		* 	\param in_file_name The name of the file to write the HTML data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_template_name The name of HTML template to use during export.
		* 	\param in_include_measurement_information Whether to retain information which will allow accurate measurements to be performed on the HTML data if the input CADModel was imported with BRep data.*/
		static void ExportHTML(CADModel const & in_cad_model, char const * in_file_name, char const * in_template_name, bool in_include_measurement_information);

		/*! Performs a synchronous Publish export of the given KeyPathArray with the provided options to a given filename.  
		*   \warning This feature is only supported on 64bit builds of HOOPS Visualize. An InvalidOperationException will be thrown if this function is used on 32bit builds.
		*   This will create an HTML page using the provided template.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_key_paths The KeyPaths containing the data to export.
		* 	\param in_file_name The name of the file to write the HTML data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_template_name The name of HTML template to use during export. */
		static void ExportHTML(KeyPathArray const & in_key_paths, char const * in_file_name, char const * in_template_name);

		/*! Performs a synchronous Publish export of the given KeyPathArray with the provided options to a given filename.
		*   \warning This feature is only supported on 64bit builds of HOOPS Visualize. An InvalidOperationException will be thrown if this function is used on 32bit builds.
		*   This will create an HTML page using the provided template.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_count The number of key paths in in_key_paths.
		* 	\param in_key_paths The KeyPaths containing the data to export.
		* 	\param in_file_name The name of the file to write the HTML data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_template_name The name of HTML template to use during export. */
		static void ExportHTML(size_t in_count, KeyPath const in_key_paths[], char const * in_file_name, char const * in_template_name);

		/*! Performs a synchronous Publish export of the given KeyPath with the provided options to a given filename.
		*   \warning This feature is only supported on 64bit builds of HOOPS Visualize. An InvalidOperationException will be thrown if this function is used on 32bit builds.
		*   This will create an HTML page using the provided template.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_key_path The KeyPath containing the data to export.
		* 	\param in_file_name The name of the file to write the HTML data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_template_name The name of HTML template to use during export. */
		static void ExportHTML(KeyPath const & in_key_path, char const * in_file_name, char const * in_template_name);

		/*! Performs a synchronous Publish export of the given CAD Model with the provided options to a given filename
		*   \warning This feature is only supported on 64bit builds of HOOPS Visualize. An InvalidOperationException will be thrown if this function is used on 32bit builds.
		*   This will create an HTML page using the provided template.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.
		* 	\param in_additional_key_paths An array of KeyPaths for which PRC data will be created and which will be added to in_cad_model during export. Use this
		*	parameter to specify non-Exchange data you have added to the scene which you want to export.
		* 	\param in_file_name The name of the file to write the HTML data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_template_name The name of HTML template to use during export.
		* 	\param in_include_measurement_information Whether to retain information which will allow accurate measurements to be performed on the HTML data if the input CADModel was imported with BRep data.*/
		static void ExportHTML(CADModel const & in_cad_model, KeyPathArray const & in_additional_key_paths, char const * in_file_name, char const * in_template_name, bool in_include_measurement_information);

		/*! Performs a synchronous Publish export of the given CAD Model with the provided options to a given filename.
		*   \warning This feature is only supported on 64bit builds of HOOPS Visualize. An InvalidOperationException will be thrown if this function is used on 32bit builds.
		*   This will create an HTML page using the provided template.
		*	An exception will be thrown if a problem is encountered during export.
		* 	\param in_cad_model The CADModel containing the data to export.
		* 	\param in_count The size of in_additional_key_paths.
		* 	\param in_additional_key_paths An array of KeyPaths for which PRC data will be created and which will be added to in_cad_model during export. Use this
		*	parameter to specify non-Exchange data you have added to the scene which you want to export.
		* 	\param in_file_name The name of the file to write the HTML data to.  This filename should be a fully-qualified path, otherwise the export may fail.
		* 	\param in_template_name The name of HTML template to use during export.
		* 	\param in_include_measurement_information Whether to retain information which will allow accurate measurements to be performed on the HTML data if the input CADModel was imported with BRep data.*/
		static void ExportHTML(CADModel const & in_cad_model, size_t in_count, KeyPath const in_additional_key_paths[], char const * in_file_name, char const * in_template_name, bool in_include_measurement_information);

	private:
		//! Private default constructor to prevent instantiation.
		File();
	};

	/*! The ExportOptionsKit class is a user space object.  It contains settings controlling how 3D PDF data is exported via Publish. */
	class PUBLISH_API ExportOptionsKit : public SprocketKit
	{
	public:
		/*! The default constructor creates an empty ExportOptionsKit object. */
		ExportOptionsKit();

		/*! The copy constructor creates a new ExportOptionsKit object that contains the same settings as the source ExportOptionsKit.
		 * 	\param in_kit The source ExportOptionsKit to copy. */
		ExportOptionsKit(ExportOptionsKit const & in_kit);

		/*! The move constructor creates an ExportOptionsKit by transferring the underlying object of the rvalue reference to this ExportOptionsKit.
		 * 	\param in_kit An rvalue reference to an ExportOptionsKit to take the underlying object from. */
		ExportOptionsKit(ExportOptionsKit && in_kit);

		virtual ~ExportOptionsKit();

	static const HPS::Type staticType = HPS::Type::PublishExportOptionsKit;
		HPS::Type				ObjectType() const { return staticType; }

		/*! Copies the source ExportOptionsKit into this ExportOptionsKit.
		 * 	\param in_kit The source ExportOptionsKit to copy. */
		void					Set(ExportOptionsKit const & in_kit);

		/*! Copies this ExportOptionsKit into the given ExportOptionsKit.
		 * 	\param out_kit The ExportOptionsKit to populate with the contents of this ExportOptionsKit. */
		void					Show(ExportOptionsKit & out_kit) const;

		/*! Copies the source ExportOptionsKit into this ExportOptionsKit.
		 * 	\param in_kit The source ExportOptionsKit to copy.
		 * 	\return A reference to this ExportOptionsKit. */
		ExportOptionsKit &		operator=(ExportOptionsKit const & in_kit);

		/*! The move assignment operator transfers the underlying object of the rvalue reference to this ExportOptionsKit.
		 *	\param in_kit An rvalue reference to an ExportOptionsKit to take the underlying object from.
		 *	\return A reference to this ExportOptionsKit. */
		ExportOptionsKit &		operator=(ExportOptionsKit && in_kit);

		/*! Indicates whether this ExportOptionsKit has any values set on it.
		 * 	\return <span class='code'>true</span> if no values are set on this ExportOptionsKit, <span class='code'>false</span> otherwise. */
		bool					Empty() const;

		/*!	Check if the source ExportOptionsKit is equivalent to this ExportOptionsKit. 
		 *	\param in_kit The source ExportOptionsKit to compare to this ExportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					Equals(ExportOptionsKit const & in_kit) const;

		/*!	Check if the source ExportOptionsKit is equivalent to this ExportOptionsKit. 
		 *	\param in_kit The source ExportOptionsKit to compare to this ExportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are equivalent, <span class='code'>false</span> otherwise. */
		bool					operator==(ExportOptionsKit const & in_kit) const;

		/*!	Check if the source ExportOptionsKit is not equivalent to this ExportOptionsKit. 
		 *	\param in_kit The source ExportOptionsKit to compare to this ExportOptionsKit.
		 *	\return <span class='code'>true</span> if the objects are not equivalent, <span class='code'>false</span> otherwise. */
		bool					operator!=(ExportOptionsKit const & in_kit) const;


		/*! Sets the PDF document to append to.
		 *	\param in_filename UTF8-encoded filename of the PDF document to append to.
		 *	\return A reference to this ExportOptionsKit. */
		ExportOptionsKit &		SetDocumentToAppendTo(char const * in_filename);


		/*! Removes the PDF document to append to setting.
		 *	\return A reference to this ExportOptionsKit. */
		ExportOptionsKit &		UnsetDocumentToAppendTo();

		/*! Removes all data from the options kit.
		 *	\return A reference to this ExportOptionsKit. */
		ExportOptionsKit &		UnsetEverything();


		/*! Shows the PDF document to append to setting.
		 *	\param out_filename Filename of the PDF document to append to.
		 *	\return <span class='code'>true</span> if a PDF document to append to setting was specified, <span class='code'>false</span> otherwise. */
		bool					ShowDocumentToAppendTo(UTF8 & out_filename) const;
	};

private:
	//! Private default constructor to prevent instantiation.
	Publish();
};

}

#endif
