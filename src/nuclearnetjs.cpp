#include <nan.h>

#include "NUClear/src/extension/network/NUClearNetwork.cpp"
#include "NUClear/src/include/nuclear_bits/extension/network/wire_protocol.hpp"
#include "NUClear/src/include/nuclear_bits/extension/network/NUClearNetwork.hpp"

class nuclearnetjs : public Nan::ObjectWrap
{
public:

	nuclearnetjs(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	}

	void send(const Nan::FunctionCallbackInfo<v8::Value>& info) {
		if (info.Length() < 4) {
			Nan::ThrowTypeError("Wrong number of arguments supplied for the function: send");
			return;
		}

		uint64_t hash;
		if(info[0]->IsNumber()) {
			hash = (uint64_t) info[0]->NumberValue();
		}else {
			Nan::ThrowTypeError("First argument is not a number for the function: send");
			return;
		}

		std::vector<char> *payload;
		if(info[1]->IsArray()) {

			payload = new std::vector<char>();

			//http://www.puritys.me/docs-blog/article-286-How-to-pass-the-paramater-of-Node.js-or-io.js-into-native-C/C++-function..html
			v8::Handle<Value> val;
			v8::Handle<Array> jsArray = v8::Handle<Array>::Cast(info[1]);

			for (unsigned int i = 0; i < jsArray->Length(); i++) {
        		val = jsArray->Get(i);
        		payload->push_back(string(*String::Utf8Value(val)));
        	}

		} else {
			Nan::ThrowTypeError("Second argument is not an array for the function: send");
			return;
		}

		std::string target;
		if(info[2]->IsString()) {
			target = info[2]->ToString();
		} else {
			Nan::ThrowTypeError("Third argument is not a string for the function: send");
			return;
		}
		
		bool reliable;
		if(info[3]->IsBoolean()) {
			reliable = info[3]->BooleanValue();
		} else {
			Nan::ThrowTypeError("Fourth argument is not a boolean for the function: send");
			return;
		}

		nuclearNetwork.send(hash, payload, target, reliable);
	}

	void set_packet_callback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
		v8::Local<v8::Function> callback = info[0].As<v8::Function>();
		v8::Local<v8::Value> args[0] = {};

		nuclearNetwork.set_packet_callback([](const NetworkTarget& network_target , const uint64_t& hash, std::vector<char>&& data) {
			
			v8::Local<v8::Value> responseInfo[1];
			responseInfo[0] = Nan::New<v8::String>(network_target.name);

			//send back the 

			Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, 0, responseInfo);
		});
	}

	void set_join_callback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
		v8::Local<v8::Function> callback = info[0].As<v8::Function>();
		v8::Local<v8::Value> args[0] = {};

		nuclearNetwork.set_join_callback([](const NetworkTarget& network_target) {

			v8::Local<v8::Value> responseInfo[1];
			responseInfo[0] = Nan::New<v8::String>(network_target.name);

			Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, 0, responseInfo);
		});
	}

	void set_leave_callback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
		v8::Local<v8::Function> callback = info[0].As<v8::Function>();
		v8::Local<v8::Value> args[0] = {};

		nuclearNetwork.set_leave_callback([](const NetworkTarget& network_target) {

			v8::Local<v8::Value> responseInfo[1];
			responseInfo[0] = Nan::New<v8::String>(network_target.name);

			Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, 0, responseInfo);
		});
	}

	void set_next_event_callback(const Nan::FunctionCallbackInfo<v8::Value>& info) {
		v8::Local<v8::Function> callback = info[0].As<v8::Function>();
		v8::Local<v8::Value> args[0] = {};

		nuclearNetwork.set_next_event_callback([](std::chrono::steady_clock::time_point time_point) {

			v8::Local<v8::Value> responseInfo[1];
			responseInfo[0] = Nan::New<v8::Integer>(time_point.count());

			Nan::MakeCallback(Nan::GetCurrentContext()->Global(), callback, 0, responseInfo);
		});
	}

	void shutdown() {
		nuclearNetwork.shutdown();
	}


private:
	NUClear::extension::network::NUClearNetwork nuclearNetwork;
}

void Init(v8::Local<v8::Object> exports) {  

}

NODE_MODULE(pow, Init) 