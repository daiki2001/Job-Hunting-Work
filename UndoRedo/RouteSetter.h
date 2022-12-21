#pragma once
#include "BaseUndoRedo.h"
#include "./Header/EngineGeneral.h"

class RouteSetter final : public BaseUndoRedo
{
public: //メンバ変数
	Math::Vector3 roomPos;
	size_t routeIndex;
	int route;
	int oldRoute;

public: //メンバ関数
	RouteSetter(const Math::Vector3& roomPos, const size_t& routeIndex);
	RouteSetter(const Math::Vector3& roomPos, const size_t& routeIndex,
				int route, int oldRoute);
	~RouteSetter() override = default;

	void Undo() override;
	void Redo() override;
private:
	RouteSetter() = delete;
};
