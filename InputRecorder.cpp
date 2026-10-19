#include "InputRecorder.hpp"

#include <algorithm>

namespace fw {

InputRecorder& InputRecorder::get() {
	static InputRecorder instance;
	return instance;
}

void InputRecorder::beginSession() {
	reset();
	m_held.clear();
	m_active = true;
}

void InputRecorder::endSession() {
	m_active = false;
}

void InputRecorder::reset() {
	m_frames = 0;
	m_elapsed = 0.0;
	m_presses = 0;
	m_releases = 0;
	m_last.reset();
	m_events.clear();
}

void InputRecorder::tick(float dt) {
	if (!m_active) return;
	++m_frames;
	m_elapsed += dt;
}

bool InputRecorder::record(bool down, int button, bool player1) {
	if (!m_active) return false;

	int key = (button << 1) | (player1 ? 0 : 1);
	bool& held = m_held[key];
	if (held == down) return false;  // mismo estado: evento duplicado
	held = down;

	InputEvent ev{m_frames, m_elapsed, down, button, player1};
	m_last = ev;
	m_events.push_back(ev);
	if (m_events.size() > kMaxEvents) m_events.pop_front();

	if (down) ++m_presses;
	else ++m_releases;
	return true;
}

uint64_t InputRecorder::eventsInWindow(int windowFrames) const {
	uint64_t w = static_cast<uint64_t>(std::max(0, windowFrames));
	return static_cast<uint64_t>(std::count_if(
		m_events.begin(), m_events.end(),
		[&](InputEvent const& e) { return e.frame + w >= m_frames; }
	));
}

double InputRecorder::averageDt() const {
	return m_frames ? m_elapsed / static_cast<double>(m_frames) : 0.0;
}

}  // namespace fw
