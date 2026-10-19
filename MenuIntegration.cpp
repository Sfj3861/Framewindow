#include "MenuIntegration.hpp"

#include "InputRecorder.hpp"

#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/ui/Notification.hpp>

using namespace geode::prelude;

namespace fw::menu {

std::string buildStatsText() {
	auto mod = Mod::get();
	auto& rec = InputRecorder::get();
	int window = static_cast<int>(mod->getSettingValue<int64_t>("window-frames"));

	std::string text = fmt::format(
		"Registro: {}\nUpdates de PlayLayer: {}\nTiempo (suma de dt): {:.3f}s\n"
		"Pulsaciones: {}\nLiberaciones: {}\nInputs en ventana de {} upd: {}\n",
		rec.active() ? "activo" : "inactivo",
		rec.frames(), rec.elapsed(), rec.presses(), rec.releases(),
		window, rec.eventsInWindow(window)
	);
	text += fmt::format(
		"Overlay: {}  Detalles: {}\n",
		mod->getSettingValue<bool>("show-overlay") ? "si" : "no",
		mod->getSettingValue<bool>("show-details") ? "si" : "no"
	);
	text += "Cambia estos ajustes en la config del mod.";
	return text;
}

}  // namespace fw::menu

class $modify(FWPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto win = CCDirector::get()->getWinSize();
		auto menu = CCMenu::create();
		menu->setID("menu"_spr);
		menu->setPosition({win.width - 72.f, 50.f});

		auto statsSpr = ButtonSprite::create("Frame Window");
		statsSpr->setScale(0.6f);
		auto statsBtn = CCMenuItemSpriteExtra::create(
			statsSpr, this, menu_selector(FWPauseLayer::onFrameWindow)
		);
		statsBtn->setPosition({0.f, 14.f});
		menu->addChild(statsBtn);

		auto resetSpr = ButtonSprite::create("FW Reset");
		resetSpr->setScale(0.6f);
		auto resetBtn = CCMenuItemSpriteExtra::create(
			resetSpr, this, menu_selector(FWPauseLayer::onFrameWindowReset)
		);
		resetBtn->setPosition({0.f, -14.f});
		menu->addChild(resetBtn);

		this->addChild(menu);
	}

	void onFrameWindow(CCObject*) {
		FLAlertLayer::create("Frame Window", fw::menu::buildStatsText(), "OK")->show();
	}

	void onFrameWindowReset(CCObject*) {
		fw::InputRecorder::get().reset();
		Notification::create("Frame Window: contadores reiniciados", NotificationIcon::Success)->show();
	}
};
