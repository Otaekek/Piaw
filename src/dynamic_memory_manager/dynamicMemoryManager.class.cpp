#include "dynamicMemoryManager.class.hpp"
#include <stdio.h>
#include <stdlib.h>
poolAllocator 		dynamicMemoryManager::allocator[NUM_CLUSTERS];
uint32_t			dynamicMemoryManager::cluster_id = 0;
void 				*dynamicMemoryManager::ref_to_ptr[MAX_REF] = {nullptr};
void 				*dynamicMemoryManager::ref_to_bloc[MAX_REF] = {nullptr};
std::mutex			dynamicMemoryManager::mu;
uint32_t			dynamicMemoryManager::ref_pool[MAX_REF];
uint32_t			dynamicMemoryManager::ref_pool_size = MAX_REF;

uint32_t			dynamicMemoryManager::init()
{
	for (uint32_t i = 0; i < MAX_REF; i++)
		ref_pool[i] = i;
}

uint32_t			dynamicMemoryManager::cluster_init(uint32_t sizeBloc, uint32_t numBloc)
{
	mu.lock();
	cluster_id++;
	allocator[cluster_id].modifySize(sizeBloc, numBloc);
	mu.unlock();
	return (cluster_id);
}

uint32_t			dynamicMemoryManager::create_slot(uint32_t type_id)
{
	void		*bloc;
	uint32_t	referencer;

	mu.lock();
	referencer = ref_pool[--ref_pool_size];
	bloc = allocator[type_id].mem_alloc();
	ref_to_ptr[referencer] = bloc;
	ref_to_bloc[referencer] = bloc;
	mu.unlock();
	return (referencer);
}

void				dynamicMemoryManager::clear_data(uint32_t ref, uint32_t type_id)
{
	mu.lock();
	ref_pool[ref_pool_size++] = ref;
	allocator[type_id].mem_free(ref_to_bloc[ref]);
	mu.unlock();
}

void 				*dynamicMemoryManager::get_ptr(uint32_t ref)
{
	return (ref_to_ptr[ref]);
}

void				dynamicMemoryManager::zerofy(uint32_t handler, uint32_t size)
{
	mu.lock();
	bzero(get_ptr(handler), size);
	mu.unlock();
}
