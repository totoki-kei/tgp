
#include "Resource.h"
#include <map>
#include <algorithm>

#include "WinAPI.h"

#include "Debug.h"


typedef std::vector< std::shared_ptr<Resource> > children_t;

//typedef std::weak_ptr<Resource> wp_t;
//typedef std::map<unsigned int, wp_t > map_t;
//typedef std::pair<unsigned int, wp_t > mapEntry_t;
//map_t resMap;

unsigned int Resource::nextid = 0;

Resource::Resource(void) {
	this->ResourceId = nextid++;

	RSC_DBG_OUT("Resource #%d created (auto increment)\n", this->ResourceId);
}

Resource::Resource(unsigned int id) {
	this->ResourceId = id;

	RSC_DBG_OUT("Resource #%d created (manual id)\n", this->ResourceId);
}


Resource::~Resource () {
	if( !isDisposed() ) Dispose(); 
	RSC_DBG_OUT("Resource #%d deleted\n", this->ResourceId);
}

void Resource::AddResource(std::shared_ptr<Resource> r) {
	children.push_back(r);
	RSC_DBG_OUT("Resource #%d added #%d\n", this->ResourceId, r->GetResourceID());
}

bool Resource::RemoveResource(std::shared_ptr<Resource> r,  bool all, bool recursive) {
	auto it = std::find(children.begin(), children.end(), r);
	while (it != children.end()) {
		auto rid = (*it)->GetResourceID();
		children.erase(it);

		RSC_DBG_OUT("Resource #%d removed #%d\n", this->ResourceId, rid);


		if (!all) return true;
		it = std::find(children.begin(), children.end(), r);
	}

	if (recursive) {
		for(auto i : children) {
			bool found = i->RemoveResource(r, all, recursive);
			if(!all && found) return true;
		}
	}

	return false;
}

std::weak_ptr<Resource> Resource::FindResource(unsigned int id, bool recursive) {
	auto it = std::find_if(
		children.begin(),
		children.end(),
		[id](const children_t::value_type a) { return (a)->ResourceId == id; }
	);

	if (it != children.end()) {
		return std::weak_ptr<Resource>(*it);
	}

	if (recursive) {
		for(auto i : children) {
			auto found = i->FindResource(id, recursive);
			if(!found.expired()){
				return found;
			}
		}
	}

	return std::weak_ptr<Resource>(); // return null weak_ptr
}


unsigned int Resource::GetResourceID() const {
	return this->ResourceId;
}

bool Resource::isDisposed(void) {
	return false;
}

void Resource::Dispose(void) {

	RSC_DBG_OUT("Resource #%d disposing...\n", this->ResourceId);

	// ’Ç‰Á‚³‚ê‚½‡˜‚Æ‚Í‹t‡‚É‰ð•ú
	for(auto i = children.rbegin(); i != children.rend(); i++){
		auto r = *i;
		if(!r->isDisposed()) r->Dispose();
	}

	RSC_DBG_OUT("Resource #%d disposed\n", this->ResourceId);

	children.clear();
}

