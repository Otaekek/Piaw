#include "entity.class.hpp"

entity::entity()
{
	for (uint32_t i = 0; i < E_SIZEQ; i++)
		_counts[i] = 0;
}

entity::~entity()
{

}

void		entity::update()
{

}

void entity::push(uint32_t queue, uint32_t handler)
{
	if (_counts[queue] < MAX_COMPONENT)
		_queues[queue][_counts[queue]++] = handler;
}

void entity::del(uint32_t queue, uint32_t handler)
{
	for (uint32_t i = 0; i < _counts[queue]; i++)
	{
		if (handler == _queues[queue][i])
			_queues[queue][i] =_queues[queue][--_counts[queue]];
	}
}
