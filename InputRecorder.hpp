#pragma once

#include <cstdint>
#include <deque>
#include <map>
#include <optional>

namespace fw {

struct InputEvent {
	uint64_t frame;  // actualizaciones de GJBaseGameLayer contadas hasta el evento
	double time;     // suma de dt desde el inicio de la sesion (segundos)
	bool down;       // true = pulsacion, false = liberacion
	int button;      // 1 = salto, 2 = izquierda, 3 = derecha (segun el juego)
	bool player1;
};

// Logica pura (sin dependencias de Geode): registra eventos y contadores.
// "frame" = numero de actualizaciones de la capa de juego, NO frames de render.
class InputRecorder {
public:
	static InputRecorder& get();

	void beginSession();  // nueva partida: reinicia todo y activa el registro
	void endSession();    // salida de la partida: desactiva el registro
	void reset();         // reinicia contadores, frames y tiempo

	void tick(float dt);
	// Devuelve false si el evento se descarto por ser un duplicado.
	bool record(bool down, int button, bool player1);

	bool active() const { return m_active; }
	uint64_t frames() const { return m_frames; }
	double elapsed() const { return m_elapsed; }
	uint64_t presses() const { return m_presses; }
	uint64_t releases() const { return m_releases; }
	std::optional<InputEvent> last() const { return m_last; }
	uint64_t eventsInWindow(int windowFrames) const;
	double averageDt() const;

private:
	static constexpr size_t kMaxEvents = 256;

	bool m_active = false;
	uint64_t m_frames = 0;
	double m_elapsed = 0.0;
	uint64_t m_presses = 0;
	uint64_t m_releases = 0;
	std::optional<InputEvent> m_last;
	std::deque<InputEvent> m_events;
	std::map<int, bool> m_held;  // estado por (boton, jugador) para evitar duplicados
};

}  // namespace fw
