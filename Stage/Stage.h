#pragma once

class Stage final
{
public: //ƒVƒ“ƒOƒ‹ƒgƒ“‰»
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;
};
