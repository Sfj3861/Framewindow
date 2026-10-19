# Frame Window (v0.1.0)

Mod de Geode para Geometry Dash **2.2081** (Android 32 y 64 bits). Registra pulsaciones y liberaciones
con su frame y tiempo, y las muestra en un overlay durante la partida.

> **Estado real: código fuente SIN COMPILAR y SIN PROBAR.** No se ha generado ningun `.geode`.
> Se escribio en un entorno sin red, sin Geode SDK y sin Android NDK.

## Que hace
- Hook de `GJBaseGameLayer::handleButton` -> registra down/up, boton, jugador (P1/P2), frame y tiempo.
  Descarta eventos duplicados (mismo estado para el mismo boton y jugador).
- Hook de `GJBaseGameLayer::update` -> cuenta **actualizaciones de la capa de juego** y suma `dt`.
  Ese contador NO es necesariamente un frame de render.
- Overlay (esquina superior izquierda) con estado, updates, tiempo, pulsaciones y liberaciones.
- Botones **Frame Window** (estadisticas) y **FW Reset** (reinicia contadores) en el menu de pausa.

## Ajustes (`mod.json`)
| Id | Tipo | Defecto | Efecto |
|---|---|---|---|
| `window-frames` | int 0-30 | 10 | Ventana (en updates) para contar inputs recientes y estimar su duracion en ms. **Solo informativa**: no toca la fisica. |
| `show-overlay` | bool | true | Muestra/oculta el overlay. |
| `show-details` | bool | true | Anade ventana, ultimo evento y dt medio. |

## Compilar en local (sin GitHub)
Requisitos: Geode CLI, Geode SDK (v5.x, que apunta a GD 2.2081), CMake, Ninja y Android NDK
(version exacta: la que indique la documentacion oficial de Geode para tu version del SDK).

```
geode sdk update
geode sdk install-binaries -p android64
geode sdk install-binaries -p android32
geode build -p android64
geode build -p android32
```
Cada comando deja un `.geode` en `build-android64/` o `build-android32/`. Instalacion manual:
`/storage/emulated/0/Android/media/com.geode.launcher/game/geode/mods/`
(un `.geode` por arquitectura; para uno combinado revisa `geode package --help`).

## Sin verificar (revisar primero si falla la compilacion)
1. `GJBaseGameLayer::handleButton(bool down, int button, bool isPlayer1)`: firma tomada de memoria,
   no confirmada en la documentacion; tampoco que tenga direccion en Android 2.2081.
   Alternativa: hookear `PlayerObject::pushButton/releaseButton(PlayerButton)` (existen en la doc).
2. Firmas de `GJBaseGameLayer::update`, `PlayLayer::init/onQuit`, `PauseLayer::customSetup`.
3. `ButtonSprite::create(const char*)`, `FLAlertLayer::create(...)` con `std::string`, `Notification`.
4. `"geode": "5.9.0"` en `mod.json` debe coincidir con tu SDK instalado (`geode sdk version`).
5. Estandar C++ (`20`): compara con el `CMakeLists.txt` que genera `geode new`.
6. Tags `interface` y `enhancement` y `min`/`max` en el ajuste int: vistos en la doc, no validados con `geode project check`.
7. Posicion de los botones en pausa y legibilidad del overlay en pantallas reales.

## Licencia
MIT.
