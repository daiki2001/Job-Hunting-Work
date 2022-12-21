#include "RouteSetter.h"
#include "./Stage/Stage.h"

RouteSetter::RouteSetter(const Math::Vector3& roomPos, const size_t& routeIndex) :
	BaseUndoRedo(),
	roomPos(roomPos),
	routeIndex(routeIndex)
{
	auto& r = Stage::Get()->GetArea().GetRoute();
	if (this->routeIndex < r.size())
	{
		route = (r[this->routeIndex] + 1) % (Area::NONE_LOST_FOREST + 1);
		oldRoute = r[this->routeIndex];
	}
	else
	{
		route = Area::NONE_LOST_FOREST;
		oldRoute = Area::NONE_LOST_FOREST;
	}
}

RouteSetter::RouteSetter(const Math::Vector3& roomPos, const size_t& routeIndex, int route, int oldRoute) :
	BaseUndoRedo(),
	roomPos(roomPos),
	routeIndex(routeIndex),
	route(route),
	oldRoute(oldRoute)
{
}

void RouteSetter::Undo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::Get()->GetArea().SetRoute(routeIndex, oldRoute);
}

void RouteSetter::Redo()
{
	if (Stage::SetRoom(roomPos) == FUNCTION_ERROR) return;
	Stage::Get()->GetArea().SetRoute(routeIndex, route);
}
