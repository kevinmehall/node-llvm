#include <vector>

class ProtoBuilder{
	public:
		typedef void (*InitFn)(Handle<Object> target);
		typedef std::vector<InitFn> InitFnList;
		static InitFnList initFns;

		static void initAll(Handle<Object> target){
			for (InitFnList::iterator it=initFns.begin() ; it < initFns.end(); it++ ){
				(**it)(target);
			}
		}

};

template <class T>
class Proto{
public:
	Proto(const char *_name, ProtoBuilder::InitFn initfn=NULL, const char* _contextArg=""):
		name(_name), contextArg(_contextArg){
		tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New());
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		tpl->SetClassName(String::NewSymbol(name));

		if (initfn) ProtoBuilder::initFns.push_back(initfn);
	}

	void init(Handle<Object> target){
		target->Set(String::NewSymbol(name), tpl->GetFunction());	
	}

	void addMethod(const char* name, v8::InvocationCallback fn){
		NODE_SET_PROTOTYPE_METHOD(tpl, name, fn);
	}

	void addStaticMethod(const char* name, v8::InvocationCallback fn){
		NODE_SET_METHOD(tpl, name, fn);
	}

	Handle<Value> create(T*, Handle<Value> context){
		return Undefined(); // TODO
	}

	T* unwrap(Handle<Value> value){
		return NULL; // TODO
	}

	Persistent<FunctionTemplate> tpl;
	const char* const name;
	const char* const contextArg;
};

#define THROW_BAD_ARGS(FAIL_MSG) return ThrowException(Exception::TypeError(String::New(FAIL_MSG)));
#define THROW_ERROR(FAIL_MSG) return ThrowException(Exception::Error(String::New(FAIL_MSG)));
#define CHECK_N_ARGS(MIN_ARGS) if (args.Length() < MIN_ARGS) THROW_BAD_ARGS("Expected " #MIN_ARGS " arguments")

#define ENTER_METHOD(PROTO, MIN_ARGS) \
	HandleScope scope;                \
	CHECK_N_ARGS(MIN_ARGS);           \
	auto self = PROTO.unwrap(args.This());

#define UNWRAP_ARG(PROTO, NAME, ARGNO)     \
	auto NAME = PROTO.unwrap(args[ARGNO]); \
	if (!NAME) THROW_BAD_ARGS("Parameter " #NAME " (" #ARGNO ") is of incorrec type")
