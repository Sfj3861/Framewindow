// Frame Window - hooks de partida: registro de inputs, conteo de updates y overlay.
#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include "FrameWindowOverlay.hpp"
#include "InputRecorder.hpp"

using namespace geode::prelude;

// Solo registramos en PlayLayer (no en el editor ni en otras capas).
static bool isPlayLayer(GJBaseGameLayer* layer) {
	auto pl = PlayLayer::get();
	return pl && static_cast<GJBaseGameLayer*>(pl) == layer;
}

class $modify(FWGameLayer, GJBaseGameLayer) {
	void update(float dt) {
		GJBaseGameLayer::update(dt);
		if (isPlayLayer(this)) fw::InputRecorder::get().tick(dt);
	}

	void handleButton(bool down, int button, bool isPlayer1) {
		GJBaseGameLayer::handleButton(down, button, isPlayer1);
		if (isPlayLayer(this)) fw::InputRecorder::get().record(down, button, isPlayer1);
	}
};

class $modify(FWPlayLayer, PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

		fw::InputRecorder::get().beginSession();
		if (auto overlay = fw::FrameWindowOverlay::create()) {
			overlay->setID("overlay"_spr);
			this->addChild(overlay, 1000);
		}
		return true;
	}

	void onQuit() {
		fw::InputRecorder::get().endSession();
		PlayLayer::onQuit();
	}
};
