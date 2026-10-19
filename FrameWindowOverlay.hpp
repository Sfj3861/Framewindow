#pragma once

#include <Geode/Geode.hpp>

namespace fw {

// Overlay de texto fijo en pantalla (esquina superior izquierda).
class FrameWindowOverlay : public cocos2d::CCNode {
public:
	static FrameWindowOverlay* create();

protected:
	bool init() override;
	void update(float dt) override;

private:
	void refresh();
	void applyLayout();

	cocos2d::CCLabelBMFont* m_label = nullptr;
};

}  // namespace fw
