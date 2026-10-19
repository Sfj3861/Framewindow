#include "FrameWindowOverlay.hpp"

#include "InputRecorder.hpp"

using namespace geode::prelude;

namespace fw {

static char const* buttonName(int button) {
	switch (button) {
		case 1: return "Jump";
		case 2: return "Left";
		case 3: return "Right";
		default: return "?";
	}
}

FrameWindowOverlay* FrameWindowOverlay::create() {
	auto ret = new FrameWindowOverlay();
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	return nullptr;
}

bool FrameWindowOverlay::init() {
	if (!CCNode::init()) return false;

	m_label = CCLabelBMFont::create("", "chatFont.fnt");
	m_label->setAnchorPoint({0.f, 1.f});
	m_label->setAlignment(kCCTextAlignmentLeft);
	m_label->setOpacity(190);
	this->addChild(m_label);

	this->scheduleUpdate();
	this->refresh();
	return true;
}

void FrameWindowOverlay::update(float) {
	this->refresh();
}

void FrameWindowOverlay::refresh() {
	auto mod = Mod::get();
	if (!mod->getSettingValue<bool>("show-overlay")) {
		this->setVisible(false);
		return;
	}
	this->setVisible(true);

	bool details = mod->getSettingValue<bool>("show-details");
	int window = static_cast<int>(mod->getSettingValue<int64_t>("window-frames"));
	auto& rec = InputRecorder::get();

	std::string text = fmt::format("Frame Window [{}]\n", rec.active() ? "REC" : "OFF");
	text += fmt::format("Updates: {}  t: {:.3f}s\n", rec.frames(), rec.elapsed());
	text += fmt::format("Press: {}  Release: {}", rec.presses(), rec.releases());

	if (details) {
		double winMs = window * rec.averageDt() * 1000.0;
		text += fmt::format(
			"\nVentana: {} upd (~{:.1f}ms)  inputs: {}",
			window, winMs, rec.eventsInWindow(window)
		);
		if (auto last = rec.last()) {
			text += fmt::format(
				"\nUlt: P{} {} {} @{} t={:.3f}s (hace {})",
				last->player1 ? 1 : 2, buttonName(last->button),
				last->down ? "DOWN" : "UP", last->frame, last->time,
				rec.frames() - last->frame
			);
		} else {
			text += "\nUlt: -";
		}
		double dt = rec.averageDt();
		if (dt > 0.0) {
			text += fmt::format("\ndt medio: {:.2f}ms (~{:.0f} upd/s)", dt * 1000.0, 1.0 / dt);
		}
	}

	m_label->setString(text.c_str());
	this->applyLayout();
}

void FrameWindowOverlay::applyLayout() {
	// Mantiene el texto dentro de la pantalla visible, con margen.
	auto win = CCDirector::get()->getWinSize();
	constexpr float margin = 14.f;
	constexpr float baseScale = 0.45f;

	auto size = m_label->getContentSize();
	float scale = baseScale;
	float maxW = win.width * 0.6f - margin * 2.f;
	float maxH = win.height - margin * 2.f;
	if (size.width * scale > maxW && size.width > 0.f) scale = maxW / size.width;
	if (size.height * scale > maxH && size.height > 0.f) scale = std::min(scale, maxH / size.height);

	m_label->setScale(scale);
	m_label->setPosition({margin, win.height - margin});
}

}  // namespace fw
