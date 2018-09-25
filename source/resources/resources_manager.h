#pragma once

#include "resource.h"

extern std::recursive_mutex mtxResources;

class CResourceManager {

	// I'm the owner of all resources, and are stored in the map using the resource.name
	std::map< std::string, IResource* > all_resources;

	// These are the 'resource classes' stored by extension
	std::map< std::string, const CResourceClass* > resource_classes;

	// To make the reload in proper order
	std::vector< const CResourceClass* > resource_classes_by_file_change_priority;

public:

	void registerResourceClass(const CResourceClass* new_class);
	const IResource* get(const std::string& res_name);

	void debugInMenu();
	void destroyAll();
	void destroyResource(const std::string& name);

	// 
	void registerResource(IResource* res);
	void onFileChanged(const std::string& filename);

	void loadResources(const std::string& file);
	void loadResourcesBackground(const std::string& file);

	std::thread loaderThread;
	volatile bool resourcesLoaded = false;
};

extern CResourceManager Resources;

