#include "graphics.h"

t_xy	iso_map(t_xy pos)
{
	return ((t_xy){(pos.x - pos.y) * 1.4, (pos.x + pos.y) / 1.4});
}


t_xy	map_iso(t_xy pos)
{
	return ((t_xy){
		(pos.x * 0.7 + pos.y * 0.7) / 2.0, (pos.y * 0.7 - pos.x * 0.7) / 2.0});
}
