# vim: shiftwidth=4 softtabstop=4 tabstop=4 expandtab

import sys
import os, os.path
import re
import argparse
import string

from os.path import join

if sys.version_info < (2, 7, 0):
    sys.stderr.write('You need python 2.7 or later to run this script\n')
    exit(1)

# ------------------------------------

SURFACE_ACTION_TOKEN = 'SURFACE_ACTION'
APP_ACTION_TOKEN = 'APP_ACTION'

this_dir = os.path.dirname(__file__)
sandbox_dir = os.path.abspath(os.path.join(this_dir, '..'))
samples_shared_dir = os.path.abspath(os.path.join(sandbox_dir, 'app', 'src', 'main', 'cpp'))
hps_dir = os.getenv('HPS_VISUALIZE_INSTALL_DIR')

print(hps_dir)

DEFAULT_OUT_JNI_PATH = os.path.join(sandbox_dir, 'app', 'src', 'main', 'cpp', 'JNI')
DEFAULT_IN_USER_MOBILE_SURFACE_H = os.path.join(samples_shared_dir, 'UserMobileSurface.h')

print(samples_shared_dir, sandbox_dir, hps_dir);

DEFAULT_IN_MOBILE_APP_H = os.path.join(samples_shared_dir, 'MobileApp.h')
DEFAULT_OUT_JAVA_SRC_PATH = os.path.join(sandbox_dir, 'app', 'src', 'main', 'java')
DEFAULT_PACKAGE = 'com.techsoft3d.hps.virtual_training'

# ------------------------------------

JTYPES = {
    #type: (rep, jnitype, javatype, overloadSymbol, JNIHelpers::ArrayName)
    'void': ('V', 'void', 'void', 'V', 'NA'),
    'bool': ('Z', 'jboolean', 'boolean', 'Z', 'BoolArray'),
    'int': ('I', 'jint', 'int', 'I', 'IntArray'),
    'long long': ('J', 'jlong', 'long', 'J', 'LongArray'),
    'float': ('F', 'jfloat', 'float', 'F', 'FloatArray'),
    'double': ('D', 'jdouble', 'double', 'D', 'DoubleArray'),
    'const char *': ('Ljava/lang/String;', 'jstring', 'String', 'S', 'NA'),
    'char *': ('Ljava/lang/StringBuffer;', 'jobject', 'StringBuffer', 'SB', 'NA'),
    'char': ('B', 'jbyte', 'byte', 'B', 'ByteArray')
    }

# ------------------------------------

def getTemplate(fn):
    with open(fn) as f:
        lines = f.read()
    
    return string.Template(lines)

# ------------------------------------

class Param:
    def __init__(self, rawParam):
        reConstArray = re.compile(r'const\s*(.*?)\s*(\w+)\[\]\s*$')
        reArray      = re.compile(r'\s*(.*?)\s*(\w+)\[\]\s*$')
        reRest       = re.compile(r'\s*(.*?)\s*(\w+?)\s*$')

        reInString   = re.compile(r'\s*const\s+char\s*\*')
        reOutString  = re.compile(r'\s*char\s*\*')

        # try const array
        m = reConstArray.match(rawParam)
        if m:
            self.ctype, self.name = m.groups()
            self.isConst = True
            self.isArray = True

        # try array
        if not m:
            m = reArray.match(rawParam)
            if m:
                self.ctype, self.name = m.groups()
                self.isConst = False
                self.isArray = True

        # rest
        # handles 'const char *name'
        if not m:
            m = reRest.match(rawParam)
            self.ctype, self.name = m.groups()
            self.isConst = False
            self.isArray = False

            # Fixup strings
            if reInString.match(self.ctype):
                self.ctype = 'const char *'
            elif reOutString.match(self.ctype):
                self.ctype = 'char *'

        self.stype = JTYPES[self.ctype][0]   # string representation type
        self.jnitype = JTYPES[self.ctype][1]
        self.jtype = JTYPES[self.ctype][2]
        self.symbol = JTYPES[self.ctype][3]

        if self.isArray:
            self.jnitype = self.jnitype + 'Array'
            self.stype = '[' + self.stype
            self.arrayName = JTYPES[self.ctype][4]

class Method:
    def __init__(self, rawMethod, prefix):
        p = prefix + r'\s+(.+?)\s*(\w+)\s*\(\s*(.*?)\s*\)\s*;'
        m = re.match(p, rawMethod)
        if not m:
            raise Exception('Error parsing method: ' + rawMethod)

        ret, name, params = m.groups()

        self.cRet = ret
        self.jniRet = JTYPES[ret][1]
        self.jniSymbolRet = JTYPES[ret][0]
        self.javaRet = JTYPES[ret][2]

        self.name = name

        if params:
            self.params = [Param(param) for param in params.split(',')]
        else:
            self.params = None

        self.overloadName = self.name

        if self.params:
            symbols = []
            for p in self.params:
                arr = ''
                if p.isArray:
                    arr = 'A'
                symbols.append(p.symbol + arr)
            self.overloadName = self.name + ''.join(symbols)
        else:
            self.overloadName = self.name + 'V'

class Actions:
    def __init__(self, filename, prefix):
        lines = None
        with open(filename) as f:
            lines = map(str.strip, f.readlines())

        if lines is None:
            raise Exception('Error: Could not read anything from ' + filename)

        lines = [line for line in lines if line.startswith(prefix)]

        self.methods = [Method(line, prefix) for line in lines]

    def emit(self, className, package, jni, java_src, jpath, header, needsPtrArg):
        emitJNI(self, className, jni, 'tpl-actionsJNI.cpp.txt', jpath, header, needsPtrArg)
        emitJava(self, className, java_src, package, 'tpl-'+className+'.java.txt', needsPtrArg)

# ------------------------------------

def buildJNIFunc(method, needsPtrArg):
    sparams = ''
    header = ''
    args = ''

    if method.params:
        params = []
        header = []
        args = []
        for param in method.params:
            params.append('{} {}'.format(param.jnitype, param.name))

            if param.isArray:
                f = 'JNIHelpers::{0} {1}_arr(env, {1});'
                header.append(f.format(param.arrayName, param.name))
                args.append('{}_arr.arr()'.format(param.name))

            elif param.jtype == 'String':
                f = 'JNIHelpers::String c{0}(env, {0});'
                header.append(f.format(param.name))
                args.append('c{}.str()'.format(param.name))

            elif param.jtype == 'StringBuffer':
                f = 'JNIHelpers::StringBuffer sb{0}(env, {0});'
                header.append(f.format(param.name))
                args.append('sb{}.str()'.format(param.name))

            else:
                args.append(param.name)

        header = '\n'.join(header)
        args = ', '.join(args)
        sparams = ', ' + ', '.join(params)

    jret = method.jniRet
    rtemp = ''
    sret = ''
    if method.cRet != 'void':
        sret = 'return ret;'
        rtemp = '{} ret ='.format(jret)

    d = {'jret': jret, 'name': method.name, 'params': sparams,
         'overloadName': method.overloadName,
         'header': header, 'return': sret, 'rtemp': rtemp,
         'args': args, 'className': 'alkdj'
        }
    tplName = 'tpl-jnifunction-static.cpp.txt'
    if needsPtrArg:
        tplName = 'tpl-jnifunction.cpp.txt'
    tpl = getTemplate(tplName)
    return tpl.substitute(d)

def buildJNIMethodSig(method, needsPtrArg):
    sret = method.jniSymbolRet
    if needsPtrArg:
        sig = '(J)' + sret
    else:
        sig = '()' + sret

    if method.params:
        stypes = ''
        for param in method.params:
            stypes = stypes + param.stype

        if needsPtrArg:
            sig = '(J{}){}'.format(stypes, sret)
        else:
            sig = '({}){}'.format(stypes, sret)

    d = dict(overloadName=method.overloadName, sig=sig)
    tplstr = '\t\t{"$overloadName", "$sig", (void*)$overloadName},'
    tpl = string.Template(tplstr)
    return tpl.substitute(d)
    
def buildNativeJavaMethodSeg(method, needsPtrArg):

    sparams = ''

    if method.params:
        fmt = lambda p: '{}[] {}' if p.isArray else '{} {}'
        vparams = [fmt(p).format(p.jtype, p.name) for p in method.params]
        sparams = ', '.join(vparams)
        if needsPtrArg:
            sparams = ', ' + sparams

    sret = method.javaRet
    d = {'ret': sret, 'name': method.name, 'overloadName': method.overloadName, 'params': sparams}
    if needsPtrArg:
        tplstr = '\tprivate static native $ret $overloadName(long ptr$params);'
    else:
        tplstr = '\tprivate static native $ret $overloadName($params);'
    tpl = string.Template(tplstr)
    return tpl.substitute(d)

def buildJavaMethod(method, needsPtrArg):
    sparams = ''
    args = ''
    static = 'static'
    if needsPtrArg:
        args = 'mSurfacePointer'
        static = ''

    if method.params:
        fmt = lambda p: '{}[] {}' if p.isArray else '{} {}'
        vparams = [fmt(p).format(p.jtype, p.name) for p in method.params]
        sparams = ', '.join(vparams)

        vargs = [p.name for p in method.params]
        if needsPtrArg:
            args = args + ', ' + ', '.join(vargs)
        else:
            args = args + ', '.join(vargs)

    jret = method.javaRet
    sret = ''
    if method.cRet != 'void':
        sret = 'return '

    d = {'jret': jret, 'name': method.name, 'params': sparams,
         'overloadName': method.overloadName, 'static': static,
         'return': sret, 'args': args
        }
    tpl = getTemplate('tpl-javaMethod.java.txt')
    return tpl.substitute(d)


# ------------------------------------

def emitJNI(actions, className, jni, jniTemplate, jpathDefine, includeFile, needsPtrArg):

    jniFuncLines = []
    jniMethodLines = []

    for method in actions.methods:
        jniFuncLines.append(buildJNIFunc(method, needsPtrArg))
        jniMethodLines.append(buildJNIMethodSig(method, needsPtrArg))

    jniFuncLines = '\n'.join(jniFuncLines)
    jniMethodLines = '\n'.join(jniMethodLines)

    tpl = getTemplate(jniTemplate)
    x = tpl.substitute({'className': className, 'functions': jniFuncLines, 'methods': jniMethodLines, 'jpath_define': jpathDefine, 'include_file': includeFile})

    jniFile = join(jni, className + 'JNI.cpp')
    with open(jniFile, 'w') as f:
        f.write(x)

def emitJava(actions, className, java_src, packageName, javaTemplate, needsPtrArg):

    javaNativeMethodLines = []
    javaMethodLines = []

    for method in actions.methods:
        javaNativeMethodLines.append(buildNativeJavaMethodSeg(method, needsPtrArg))
        javaMethodLines.append(buildJavaMethod(method, needsPtrArg))

    javaNativeMethodLines = '\n'.join(javaNativeMethodLines)
    javaMethodLines = '\n'.join(javaMethodLines)

    tpl = getTemplate(javaTemplate)
    x = tpl.substitute({'className': className, 'package': packageName, 'native_methods': javaNativeMethodLines, 'methods': javaMethodLines})
    javaFile = join(java_src, className + '.java')
    with open(javaFile, 'w') as f:
        f.write(x)

def emitPathheader(pathHeader, packageName):

    lines = []
    slashPath = packageName.replace('.', '/')
    lines.append('#define JPATH_ANDROID_MOBILE_SURFACE_VIEW "{}/AndroidMobileSurfaceView"'.format(slashPath))
    lines.append('#define JPATH_ANDROID_USER_MOBILE_SURFACE_VIEW "{}/AndroidUserMobileSurfaceView"'.format(slashPath))
    lines.append('#define JPATH_MOBILE_APP "{}/MobileApp"'.format(slashPath))
    with open(pathHeader, 'w') as f:
        f.write('\n'.join(lines))

# ------------------------------------

def handleArgs():

    desc = 'Parses UIActions file and generates JNI/Pinvoke bindings'
    ap = argparse.ArgumentParser(description=desc)
    ap.add_argument('--surfaceHeader', help='path to UserMobileSurface.h')
    ap.add_argument('--appHeader', help='path to MobileApp.h')
    ap.add_argument('--jni', help='path to jni folder')
    ap.add_argument('--java_src', help='path to java src folder')
    ap.add_argument('--package', help='package name')

    return ap.parse_args()

def main(args):

    if args is None:
        print("LOL!")
        return -1

    surfaceHeader = DEFAULT_IN_USER_MOBILE_SURFACE_H
    appHeader = DEFAULT_IN_MOBILE_APP_H
    jni = DEFAULT_OUT_JNI_PATH
    java_src = DEFAULT_OUT_JAVA_SRC_PATH
    package = DEFAULT_PACKAGE

    if args.surfaceHeader:
        surfaceHeader = args.surfaceHeader
    if args.appHeader:
        appHeader = args.appHeader
    if args.jni:
        jni = args.jni
    if args.java_src:
        java_src = args.java_src
    if args.package:
        package = args.package

    # Set working dir to this scripts dir
    abspath = os.path.abspath(__file__)
    dname = os.path.dirname(abspath)
    os.chdir(dname)

    if not os.path.isfile(surfaceHeader):
        raise Exception('Cannot open ' + surfaceHeader)

    if not os.path.isfile(appHeader):
        raise Exception('Cannot open ' + appHeader)

    if not os.path.isdir(jni):
        raise Exception('Invalid jni folder')

    if not os.path.isdir(java_src):
        raise Exception('Invalid Java src folder')

    java_src = join(java_src, package.replace('.', '/'))
    if not os.path.isdir(java_src):
        raise Exception('Invalid Java src folder: ' + java_src)

    surfaceActions = Actions(surfaceHeader, SURFACE_ACTION_TOKEN)
    surfaceActions.emit('AndroidUserMobileSurfaceView', package, jni, java_src, 'JPATH_ANDROID_USER_MOBILE_SURFACE_VIEW', 'UserMobileSurface.h', True)

    appActions = Actions(appHeader, APP_ACTION_TOKEN)
    appActions.emit('MobileApp', package, jni, java_src, 'JPATH_MOBILE_APP', 'MobileApp.h', False)

    emitPathheader(join(jni, 'jpaths.h'), package)

    print("I have completed.")


def tryMain(args):
    import traceback
    try:
        main(args)

    except Exception as e:
        print('Error:', e)
        traceback.print_exc()

    except:
        traceback.print_exc()

if __name__ == '__main__':
    tryMain(handleArgs())
