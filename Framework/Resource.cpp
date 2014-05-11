
#include "Resource.h"
#include <map>
#include <algorithm>

#include "WinAPI.h"

#include "Debug.h"

unsigned int Resource::nextid = 0;

Resource::Resource(void) {
	this->children = nullptr;
	this->ResourceId = nextid++;

	RSC_DBG_OUT("Resource #%d created (auto increment)\n", this->ResourceId);
}

Resource::Resource(unsigned int id) {
	this->children = nullptr;
	this->ResourceId = id;

	RSC_DBG_OUT("Resource #%d created (manual id)\n", this->ResourceId);
}


Resource::~Resource () {
	if( !isDisposed() ) DisposeResources();
	RSC_DBG_OUT("Resource #%d deleted\n", this->ResourceId);
}

std::shared_ptr<Resource> Resource::AddResource(std::shared_ptr<Resource> r) {
	if (!children) children = new children_type();
	children->push_back(r);
	RSC_DBG_OUT("Resource #%d added #%d\n", this->ResourceId, r->GetResourceID());
	return r;
}

bool Resource::RemoveResource(std::shared_ptr<Resource> r,  bool all, bool recursive) {
	if (!children) return false;
	bool removed = false;
	auto it = std::find(children->begin(), children->end(), r);
	while (it != children->end()) {
		auto rid = (*it)->GetResourceID();
		children->erase(it);

		RSC_DBG_OUT("Resource #%d removed #%d\n", this->ResourceId, rid);
		removed = true;

		if (!all) return true;
		it = std::find(children->begin(), children->end(), r);
	}

	if (recursive) {
		for(auto i : *children) {
			bool found = i->RemoveResource(r, all, recursive);
			removed = found;
			if(!all && found) return true;
		}
	}

	return removed;
}

std::weak_ptr<Resource> Resource::FindResource(unsigned int id, bool recursive) {
	if (!children) return std::weak_ptr<Resource>(); // return null weak_ptr
	auto it = std::find_if(
		children->begin(),
		children->end(),
		[id](const children_type::value_type a) { return (a)->ResourceId == id; }
	);

	if (it != children->end()) {
		return std::weak_ptr<Resource>(*it);
	}

	if (recursive) {
		for(auto i : *children) {
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
	return !children;
}

void Resource::Dispose(void) {

	RSC_DBG_OUT("Resource #%d disposing...\n", this->ResourceId);

	DisposeResources();

	RSC_DBG_OUT("Resource #%d disposed\n", this->ResourceId);

}

void Resource::DisposeResources(){
	// ’Ç‰Á‚³‚ê‚½‡˜‚Æ‚Í‹t‡‚É‰ð•ú
	if (!children) return;
	for (auto i = children->rbegin(); i != children->rend(); i++){
		auto r = *i;
		if (!r->isDisposed()) r->Dispose();
	}
	delete children;
	children = nullptr;
}

