#pragma once

// Entities
static const std::string PLAYER_NAME = "The Player";
static const std::string PLAYER_CAMERA = "player_camera";
static const std::string GAME_CAMERA = "game_camera";
static const std::string DEBUG_CAMERA = "debug_camera";
static const std::string MUSIC_PLAYER = "music_player";

static const std::string CURVE_INTERPOLATION_CAMERA = "curve_interpolation_camera";
static const std::string TELEPORT_CAMERA = "teleport_camera";
static const int NUMBER_OF_POWER_LEVELS = 3;
static const float PAD_DEAD_ZONE = 0.1f;
static const float PAD_RUN_THRESHOLD = 0.8f;

static const std::string PREFAB_GOLEM = "data/prefabs/enemy_melee.prefab";
static const std::string PREFAB_BALL = "data/prefabs/enemy_ball.prefab";
static const std::string PREFAB_MEDUSA = "data/prefabs/enemy_flying.prefab";
static const std::string PREFAB_CHRYSALIS = "data/prefabs/crisalida.prefab";
static const std::string PREFAB_COIN = "data/prefabs/coin.prefab";
static const std::string PREFAB_DREIDEL = "data/prefabs/dreidel.prefab";
static const std::string PREFAB_KIPPAH = "data/prefabs/kippah.prefab";
static const std::string PREFAB_KIPPAH_DUMMY = "data/prefabs/kippah_dummy.prefab";
static const std::string PREFAB_MURO = "data/prefabs/muro.prefab";
static const std::string PREFAB_PLATAFORMA_HEX = "data/prefabs/plataforma_hex.prefab";
static const std::string PREFAB_PLATAFORMA_RECT = "data/prefabs/plataforma_rect.prefab";
static const std::string PREFAB_PLATAFORMA_ROCA = "data/prefabs/plataforma_roca.prefab";
static const std::string PREFAB_PRISMA = "data/prefabs/prisma_mario_sunshine.prefab";

static const std::string INTERPOLATOR_LINEAR = "Linear";
static const std::string INTERPOLATOR_QUAD_IN = "Quad in";
static const std::string INTERPOLATOR_QUAD_OUT = "Quad out";
static const std::string INTERPOLATOR_QUAD_IN_OUT = "Quad in out";
static const std::string INTERPOLATOR_CUBIC_IN = "Cubic in";
static const std::string INTERPOLATOR_CUBIC_OUT = "Cubic out";
static const std::string INTERPOLATOR_CUBIC_IN_OUT = "Cubic in out";
static const std::string INTERPOLATOR_QUART_IN = "Quart in";
static const std::string INTERPOLATOR_QUART_OUT = "Quart out";
static const std::string INTERPOLATOR_QUART_IN_OUT = "Quart in out";
static const std::string INTERPOLATOR_QUINT_IN = "Quint in";
static const std::string INTERPOLATOR_QUINT_OUT = "Quint out";
static const std::string INTERPOLATOR_QUINT_IN_OUT = "Quint in out";
static const std::string INTERPOLATOR_BACK_IN = "Back in";
static const std::string INTERPOLATOR_BACK_OUT = "Back out";
static const std::string INTERPOLATOR_BACK_IN_OUT = "Back in out";
static const std::string INTERPOLATOR_ELASTIC_IN = "Elastic in";
static const std::string INTERPOLATOR_ELASTIC_OUT = "Elastic out";
static const std::string INTERPOLATOR_ELASTIC_IN_OUT = "Elastic in out";
static const std::string INTERPOLATOR_BOUNCE_IN = "Bounce in";
static const std::string INTERPOLATOR_BOUNCE_OUT = "Bounce out";
static const std::string INTERPOLATOR_BOUNCE_IN_OUT = "Bounce in out";
static const std::string INTERPOLATOR_CIRCULAR_IN = "Circular in";
static const std::string INTERPOLATOR_CIRCULAR_OUT = "Circular out";
static const std::string INTERPOLATOR_CIRCULAR_IN_OUT = "Circular in out";
static const std::string INTERPOLATOR_EXPO_IN = "Expo in";
static const std::string INTERPOLATOR_EXPO_OUT = "Expo out";
static const std::string INTERPOLATOR_EXPO_IN_OUT = "Expo in out";
static const std::string INTERPOLATOR_SINE_IN = "Sine in";
static const std::string INTERPOLATOR_SINE_OUT = "Sine out";
static const std::string INTERPOLATOR_SINE_IN_OUT = "Sine in out";

static const std::string AUTOLOAD_SCRIPTS_PATH = "data/scripts/autoload/";

// Sound
auto constexpr SOUND_LANDING = "event:/PROTA/caida";
auto constexpr SOUND_STEP_LEFT = "event:/PROTA/correr terreno/walk_ground_l";
auto constexpr SOUND_STEP_RIGHT = "event:/PROTA/correr terreno/walk_ground_l";
auto constexpr SOUND_JUMP = "event:/PROTA/saltar/salto corto/salto_corto_saltar";
auto constexpr SOUND_JUMP_FLY = "event:/PROTA/saltar/salto corto/salto_corto_volar";
auto constexpr SOUND_JUMP_LONG = "event:/PROTA/saltar/salto largo/salto_largo_saltar";
auto constexpr SOUND_JUMP_LONG_FLY = "event:/PROTA/saltar/salto largo/salto_largo_volar";
auto constexpr SOUND_DODGE = "event:/PROTA/dodge";
auto constexpr SOUND_HUGGING_WALL = "event:/PROTA/deslizar_pared";
auto constexpr SOUND_RELEASE_POWER = "event:/PROTA/liberar_energia";
auto constexpr SOUND_DIE = "event:/PROTA/muerte";
auto constexpr SOUND_ATTACK_HIT = "event:/PROTA/golpes/golpe_hit";
auto constexpr SOUND_ATTACK_MOVEMENT = "event:/PROTA/golpes/golpe_movimiento";
auto constexpr SOUND_LAUNCHER_HORIZONTAL = "event:/PROTA/launcher/launcher_horizontal_lanzar";
auto constexpr SOUND_LAUNCHER_VERTICAL = "event:/PROTA/launcher/launcher_vertical_lanzar";
auto constexpr SOUND_LAUNCHER_LOOP = "event:/PROTA/launcher/launcher_estatica";
auto constexpr SOUND_LAUNCHER_END = "event:/PROTA/launcher/launcher_final";
auto constexpr SOUND_COIN = "event:/ELEMENTOS GAMEPLAY/coger_crisalida";
auto constexpr SOUND_COLLECT_CHRYSALIS = "event:/ELEMENTOS GAMEPLAY/coger_crisalida";
auto constexpr SOUND_MECHANISM_ON = "event:/ELEMENTOS GAMEPLAY/mecanismo/mecanismo_on";
auto constexpr SOUND_MECHANISM_OFF = "event:/ELEMENTOS GAMEPLAY/mecanismo/mecanismo_off";
auto constexpr SOUND_AMBIENT_WIND = "event:/AMBIENTACION/viento";
auto constexpr SOUND_AMBIENT = "event:/AMBIENTACION/ambientacion";
auto constexpr SOUND_LVL_UP_1 = "event:/PROTA/cambio de estado/cambio_1";
auto constexpr SOUND_LVL_UP_2 = "event:/PROTA/cambio de estado/cambio_2";
auto constexpr SOUND_LVL_UP_3 = "event:/PROTA/cambio de estado/cambio_3";

// Particles
auto constexpr SLASH_CLOCKWISE = "data/particles/player/slash_clockwise.particles";
auto constexpr SLASH_COUNTER_CLOCKWISE = "data/particles/player/slash_counter_clockwise.particles";
auto constexpr SLASH_STRONG_3 = "data/particles/player/slash_strong_3.particles";
auto constexpr PARTICLES_RELEASE_POWER = "data/particles/player/release_power.particles";
auto constexpr PARTICLES_PLAYER_ATTACK = "data/particles/player/attack.particles";
auto constexpr PARTICLES_LANDING = "data/particles/player/landing.particles";
auto constexpr PARTICLES_COIN = "data/particles/coin.particles";
