#include "resource.hxx"

namespace res
{
	template<ResourceType T>
	ResourceController<T>::~ResourceController()
	{
		for (auto r{resources.begin()}; r != resources.end(); ++r) {
			r->second->unload();
		}
	}

	template<ResourceType T>
	void ResourceController<T>::index(const std::string key, const std::string path)
	{
		resources.insert({key, std::make_shared<T>()});
		resources[key]->load(path);
	}


	template<ResourceType T>
	const std::weak_ptr<T> ResourceController<T>::retrieve(const std::string key) const
	{
		return std::weak_ptr<T>(resources.at(key));
	}
}
