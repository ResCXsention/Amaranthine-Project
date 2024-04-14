#ifndef RES_RESOURCE
#define RES_RESOURCE

#include <unordered_map>
#include <memory>
#include <string>
#include <type_traits>

namespace res
{
	class Resource;
	template<class T>
	concept ResourceType = std::is_base_of<Resource, T>::value;

	class Resource
	{
	public:
		virtual void load(const std::string path) = 0;
		virtual void unload() = 0;
	};

	template<ResourceType T> 
	class ResourceController final
	{
	public:
		~ResourceController();

		void index(const std::string key, const std::string path);
		const std::weak_ptr<T> retrieve(const std::string key) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<T>> resources;
	};
}

#include "resource.txx"

#endif
