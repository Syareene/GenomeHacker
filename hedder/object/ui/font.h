#pragma once

#include "object/2d_object.h"
#include "lib/write_font.h"
#include <memory>

class DirectWriteSample : public Object2D
{
public:
	void Init(Transform trans = Transform()) override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetDisplayText(const std::string& text);
	void GetDisplayText(std::string& outText) const;	
private:
	FontData m_FontData;
	std::unique_ptr<DirectWriteCustomFont> m_Write = nullptr;
	std::string m_DisplayText = "";
};