#ifndef HEADER_RESOURCEMANAGER
#define HEADER_RESOURCEMANAGER

#include <common.hpp>
#include <GLmanager.hpp>

template<typename T>
struct TresourceManager{
	map<string, shared_ptr<T>> resources;
	shared_ptr<T> get(const string &name){
		if(not resources.count(name)) resources[name] = shared_ptr<T>{new T(name)};
		return resources[name];
	}
	void clear(){
		if(resources.empty()) return;
		if(not GLmanager::isGlInitialized) throw runtime_error("Clearing resourceManager with gl not initialized");
		resources.clear();
	}
	void clearUnused(){
		if(resources.empty()) return;
		if(not GLmanager::isGlInitialized) throw runtime_error("Clearing resourceManager with gl not initialized");
		for(auto it = resources.begin(); it != resources.end(); ){
			if(it->ss.use_count() == 1) it = resources.erase(it);
			else it++;
		}
	}
};



#endif//HEADER_RESOURCEMANAGER
