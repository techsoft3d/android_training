#pragma once

namespace JNIHelpers
{

class String {
public:
	String(JNIEnv *env, jstring s) : _env(env), _s(s) {
		_cs = _env->GetStringUTFChars(_s, 0);
	}
	
	~String() {
		_env->ReleaseStringUTFChars(_s, _cs);
	}

	const char *str() const {
		return _cs;
	}

private:
	JNIEnv *			_env;
	jstring				_s;
	const char *		_cs;
};

class StringBuffer {
public:
	StringBuffer(JNIEnv *env, jobject sb) : _env(env), _jsb(sb), _str(0) {
		jclass sbClass = env->GetObjectClass(_jsb);

		jmethodID capacityId = env->GetMethodID(sbClass, "capacity", "()I");
		jint capacity = env->CallIntMethod(_jsb, capacityId);
		_str = new char[capacity + 1];

		jmethodID toStringId = env->GetMethodID(sbClass, "toString", "()Ljava/lang/String;");
		jstring js = (jstring) env->CallObjectMethod(_jsb, toStringId);
		const char *charStr = env->GetStringUTFChars(js, 0);
		strcpy(_str, charStr);
		env->ReleaseStringUTFChars(js, charStr);
	}

	~StringBuffer() {
		jclass sbClass = _env->GetObjectClass(_jsb);
		jmethodID setLengthId = _env->GetMethodID(sbClass, "setLength", "(I)V");
		_env->CallVoidMethod(_jsb, setLengthId, (jint) 0);

		jmethodID appendId = _env->GetMethodID(sbClass, "append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
		jstring newString = _env->NewStringUTF(_str);
		_env->CallObjectMethod(_jsb, appendId, newString);
		delete [] _str;
	}

	char *str() {
		return _str;
	}

private:
	JNIEnv *			_env;
	jobject				_jsb;
	char *				_str;
};

class IntArray {
public:
	IntArray(JNIEnv *env, jintArray arr) : _env(env), _arr(arr) {
		_carr = _env->GetIntArrayElements(_arr, 0);
	}

	~IntArray() {
		_env->ReleaseIntArrayElements(_arr, _carr, 0);
	}

	int *arr() {
		return _carr;
	}

private:
	JNIEnv *			_env;
	jintArray			_arr;
	int *				_carr;
};

class LongArray {
public:
	LongArray(JNIEnv *env, jlongArray arr) : _env(env), _arr(arr) {
		_carr = _env->GetLongArrayElements(_arr, 0);
	}

	~LongArray() {
		_env->ReleaseLongArrayElements(_arr, _carr, 0);
	}

	jlong *arr() {
		return _carr;
	}

private:
	JNIEnv *			_env;
	jlongArray			_arr;
	jlong *				_carr;
};

class FloatArray {
public:
	FloatArray(JNIEnv *env, jfloatArray arr) : _env(env), _arr(arr) {
		_carr = _env->GetFloatArrayElements(_arr, 0);
	}

	~FloatArray() {
		_env->ReleaseFloatArrayElements(_arr, _carr, 0);
	}

	float *arr() {
		return _carr;
	}

private:
	JNIEnv *			_env;
	jfloatArray			_arr;
	float *				_carr;
};

class DoubleArray {
public:
	DoubleArray(JNIEnv *env, jdoubleArray arr) : _env(env), _arr(arr) {
		_carr = _env->GetDoubleArrayElements(_arr, 0);
	}

	~DoubleArray() {
		_env->ReleaseDoubleArrayElements(_arr, _carr, 0);
	}

	double *arr() {
		return _carr;
	}

private:
	JNIEnv *			_env;
	jdoubleArray		_arr;
	double *			_carr;
};

class ByteArray {
public:
	ByteArray(JNIEnv *env, jbyteArray arr) : _env(env), _arr(arr) {
		_carr = _env->GetByteArrayElements(_arr, 0);
	}

	~ByteArray() {
		_env->ReleaseByteArrayElements(_arr, _carr, 0);
	}

	// Note that jbyte is a 'signed char'
	// Users should be compiling with -fsigned-char, but doing explicit cast anyway.
	char *arr() {
		return (char*)_carr;
	}

private:
	JNIEnv *			_env;
	jbyteArray			_arr;
	jbyte *				_carr;
};

class ShowKeyboardHandler : public HPS::EventHandler
{
public:
	ShowKeyboardHandler();
	virtual ~ShowKeyboardHandler();

	virtual HPS::EventHandler::HandleResult Handle(HPS::Event const * in_event);
};


} // namespace JNIHelpers


