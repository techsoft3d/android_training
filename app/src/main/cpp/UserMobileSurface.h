#pragma once

#include "MobileSurface.h"

#define SURFACE_ACTION

// UserMobileSurface is a plaform-independent class which contains user-defined
// action methods called by Android/iOS gui code.  This class (along with MobileApp)
// allow Android/iOS gui code to communicate with C++ mobile independent code.
//
// Methods acting on a surface should be declared here.
//
//
// For Android users:
//
//  JNI bindings will be created for methods prefixed with SURFACE_ACTION.  To do so, declare
// a method starting with SURFACE_ACTION.  Add parameters according to the rules stated below.
// When you build your project, sip.py will run, generate JNI conversion code, and update
// AndroidUserMobileSurfaceView.java with the newly created methods.
//
// Valid input parameters:
//   - C++ type           -> Java type
//   - bool               -> boolean
//   - char               -> byte
//   - int (32-bit)       -> int
//   - long long (64-bit) -> long
//   - float              -> float
//   - double             -> double
//   - const char *       -> String
//
// Valid output parameters:
//   - char *             -> StringBuffer
//
// Valid input/output arrays:
//   (Note that you have to use the C++ [] syntax)
//   - char []            -> byte[]
//   - int []             -> int[]
//   - long long []       -> long[]
//   - float []           -> float[]
//   - double []          -> double[]
//
// Valid return values:
//   - void               -> void
//   - bool               -> boolean
//   - char               -> byte
//   - int (32-bit)       -> int
//   - long long (64-bit) -> long
//   - float              -> float
//   - double             -> double
//
// Notes:
//   - SURFACE_ACTION methods should be declared on a single line
//
// Examples:
//
//   SURFACE_ACTION void test2(int a, int b[], const char *c, char *d);
//     will create in AndroidUserMobileSurfaceView.java
//   public void test2(int a, int[] b, String c, StringBuffer d)
//

class UserMobileSurface : public MobileSurface
{
public:
    UserMobileSurface();
    virtual ~UserMobileSurface();

    virtual bool			bind(void *window);
    virtual void			release(int flags);
    virtual void			singleTap(int x, int y);
    virtual void			doubleTap(int x, int y, HPS::TouchID id);

    void					SetMainDistantLight(HPS::Vector const & lightDirection = HPS::Vector(1, 0, -1.5f));
    void                    SetMainDistantLight(HPS::DistantLightKit const & light);
    void					SetupSceneDefaults();

    SURFACE_ACTION bool		loadFile(const char *fileName);

    SURFACE_ACTION void		setOperatorOrbit();

    SURFACE_ACTION void		onModeSimpleShadow(bool enable);
    SURFACE_ACTION void		onModeSmooth();
    SURFACE_ACTION void		onModeHiddenLine();

    SURFACE_ACTION void		onUserCode1();
    SURFACE_ACTION void		onUserCode2();
    SURFACE_ACTION void		onUserCode3();
    SURFACE_ACTION void		onUserCode4();

private:

#ifdef USING_EXCHANGE
    HPS::CADModel           activeCADModel;
#endif

    // User code 1 test
    bool					displayResourceMonitor;

    HPS::DistantLightKey	mainDistantLight;
    HPS::Rendering::Mode	currentRenderingMode;
    bool                    frameRateEnabled;

    void 					loadCamera(HPS::View & view, HPS::Stream::ImportResultsKit const & results);
    bool importHSFFile(const char * filename, HPS::Model const & model, HPS::Stream::ImportResultsKit &);
    bool importSTLFile(const char * filename, HPS::Model const & model);
    bool importOBJFile(const char * filename, HPS::Model const & model);
    bool importPointCloudFile(const char * filename, HPS::Model const & model);
#ifdef USING_EXCHANGE
    bool importExchangeFile(const char * filename, HPS::Exchange::ImportOptionsKit ioOpts = HPS::Exchange::ImportOptionsKit());
#endif

};

