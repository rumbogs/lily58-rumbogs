#include QMK_KEYBOARD_H
#include <stdio.h>

enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
};

#define M_CTESC LCTL_T(KC_ESC)
#define M_GSPC LGUI(KC_SPC)
#define M_CF LCTL(KC_F)
#define M_ATAB LALT(KC_TAB)
#define M_CSPC LCTL(KC_SPC)
#define LY_LWR MO(_LOWER)
#define LY_RSE MO(_RAISE)

#define HY_Q HYPR(KC_Q)
#define HY_W HYPR(KC_W)
#define HY_E HYPR(KC_E)
#define HY_R HYPR(KC_R)
#define HY_T HYPR(KC_T)
#define HY_Y HYPR(KC_Y)
#define HY_U HYPR(KC_U)
#define HY_I HYPR(KC_I)
#define HY_O HYPR(KC_O)
#define HY_P HYPR(KC_P)
#define HY_A HYPR(KC_A)
#define HY_S HYPR(KC_S)
#define HY_D HYPR(KC_D)
#define HY_F HYPR(KC_F)
#define HY_G HYPR(KC_G)
#define HY_H HYPR(KC_H)
#define HY_J HYPR(KC_J)
#define HY_K HYPR(KC_K)
#define HY_L HYPR(KC_L)
#define HY_SCLN HYPR(KC_SCLN)
#define HY_Z HYPR(KC_Z)
#define HY_X HYPR(KC_X)
#define HY_C HYPR(KC_C)
#define HY_V HYPR(KC_V)
#define HY_B HYPR(KC_B)
#define HY_N HYPR(KC_N)
#define HY_M HYPR(KC_M)
#define HY_COMM HYPR(KC_COMM)
#define HY_DOT HYPR(KC_DOT)
#define HY_SLSH HYPR(KC_SLSH)

// OLED setup
#define IDLE_FRAMES 5
#define IDLE_SPEED 30
#define TAP_FRAMES 2
#define TAP_SPEED 40
#define ANIM_FRAME_DURATION 200
#define ANIM_SIZE 512

char     wpm_str[10];
uint32_t anim_timer         = 0;
uint32_t anim_sleep         = 0;
uint8_t  current_idle_frame = 0;
uint8_t  current_tap_frame  = 0;

static long int oled_timeout = OLED_TIMEOUT;  // 10 minutes


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* QWERTY
	 * ,---------------------------------------------.                     ,-------------------------------------------.
	 * | +        |   1  |   2  |   3  |   4  |   5  |                     |   6  |   7  |   8  |   9  |   0  |   -    |
	 * |----------+------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * | Tab      |   Q  |   W  |   E  |   R  |   T  |                     |   Y  |   U  |   I  |   O  |   P  |BackSP  |
	 * |----------+------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |LCTRLT/ESC|   A  |   S  |   D  |   F  |   G  |--------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '     |
	 * |----------+------+------+------+------+------|LGUI+SPC|    |   ~   |------+------+------+------+------+--------|
	 * |LShift/(  |   Z  |   X  |   C  |   V  |   B  |--------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift/)|
	 * `--------------------------------------------/       /      \       \-------------------------------------------'
	 *                   | LAlt | LGUI |LOWER |   / Space /         \ Enter \  |RAISE |LCTRL+F| RGUI |
	 *                   |      |      |      | /       /            \       \ |      |       |      |
	 *                   `----------------------------'              '-------''----------------------'
	 */
	[_QWERTY] = LAYOUT(
		KC_EQL,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, 
		KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
		M_CTESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
		KC_LSPO,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    M_GSPC,  KC_GRV,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,
		                            KC_LALT, KC_LGUI, LY_LWR,  KC_SPC,   KC_ENT, LY_RSE,  M_CF,   KC_RGUI
	),
	/* LOWER
	 * ,----------------------------------------------.                     ,-------------------------------------------.
	 * | +        |MUTE   |VOL-  | VOL+ |WWWBac|WWWFor|                     |   6  |   7  |   8  |   9  |   0  |   -    |
	 * |----------+-------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |LALT+TAB  |   !   |   @  |   #  |   $  |   %  |                     |   ^  |   &  |   *  |   (  |   )  |   F12  |
	 * |----------+-------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |LCTRL+Spc |   \   |   -  |   =  |   [  |   ]  |--------.    ,-------|   |  |   _  |   +  |   {  |   }  |    ~   |
	 * |----------+-------+------+------+------+------|LGUI+SPC|    |   ~   |------+------+------+------+------+--------|
	 * |Bright-   |Bright+|PRTSCR|SCRLOC|PAUSE |RGUI  |--------|    |-------|   \  |   -  |   =  |   [  |   ]  |   `    |
	 * `--------------------------------------------/       /      \       \-------------------------------------------'
	 *                   | LAlt | LGUI |LOWER |   / Space /         \ Enter \  |RAISE |LCTRL+F| RGUI |
	 *                   |      |      |      | /       /            \       \ |      |       |      |
	 *                   `----------------------------'              '-------''----------------------'
	 */
	[_LOWER] = LAYOUT(
		KC_TRNS,  KC_MUTE, KC_VOLD, KC_VOLU, KC_WBAK, KC_WFWD,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		M_ATAB,   KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                   KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_F12, 
		M_CSPC,   KC_BSLS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC,                   KC_PIPE, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_TILD, 
		KC_BRID,  KC_BRIU, KC_PSCR, KC_SLCK, KC_PAUS, KC_RGUI, KC_TRNS, KC_TRNS, KC_BSLS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_GRV, 
		                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS
	
	),
	/* RAISE
	 * ,---------------------------------------------.                     ,-------------------------------------------.
	 * | Prev     | Next | Stop | Play |PrevTr|NextTr|                     |   6  |   7  |   8  |   9  |   0  |   -    |
	 * |----------+------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |CapsLock  |   1  |   2  |   3  |   4  |   5  |                     |   6  |   7  |   8  |   9  |   0  |BackSP  |
	 * |----------+------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |LCTRLT/ESC|  F1  |  F2  |  F3  |  F4  |  F5  |--------.    ,-------| LEFT | DOWN |  UP  |RIGHT | F11  |Delete  |
	 * |----------+------+------+------+------+------|LGUI+SPC|    |   ~   |------+------+------+------+------+--------|
	 * |LShift/(  |  F6  |  F7  |  F8  |  F9  | F10  |--------|    |-------| HOME | PGDN | PGUP |  END |  F12 |Insert  |
	 * `--------------------------------------------/       /      \       \-------------------------------------------'
	 *                   | LAlt | LGUI |LOWER |   / Space /         \ Enter \  |RAISE |LCTRL+F| RGUI |
	 *                   |      |      |      | /       /            \       \ |      |       |      |
	 *                   `----------------------------'              '-------''----------------------'
	 */
	[_RAISE] = LAYOUT(
		KC_MPRV,  KC_MNXT, KC_MSTP, KC_MPLY, KC_MRWD, KC_MFFD,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, 
		KC_CAPS,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                      KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_TRNS, 
		KC_TRNS,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_F11,  KC_DEL, 
		KC_TRNS,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_F12,  KC_INS, 
		                            KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),
	/* RAISE
	 * ,---------------------------------------------.                     ,-------------------------------------------.
	 * |          |      |      |      |      |      |                     |      |      |      |      |      |        |
	 * |----------+------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |          |HYP+Q |HYP+W |HYP+E |HYP+R |HYP+T |                     |HYP+Y |HYP+U |HYP+I |HYP+O |HYP+P |        |
	 * |----------+------+------+------+------+------|                     |------+------+------+------+------+--------|
	 * |          |HYP+A |HYP+S |HYP+D |HYP+F |HYP+G |--------.    ,-------|HYP+H |HYP+J |HYP+K |HYP+L |HYP+; |        |
	 * |----------+------+------+------+------+------|        |    |       |------+------+------+------+------+--------|
	 * |          |HYP+Z |HYP+X |HYP+C |HYP+V |HYP+B |--------|    |-------|HYP+N |HYP+M |HYP+, |HYP+. |HYP+/ |        |
	 * `--------------------------------------------/       /      \       \-------------------------------------------'
	 *                   | LAlt | LGUI |      |   / Space /         \ Enter \  |      |LCTRL+F| RGUI |
	 *                   |      |      |      | /       /            \       \ |      |       |      |
	 *                   `----------------------------'              '-------''----------------------'
	 */
	[_ADJUST] = LAYOUT(
		KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,                     KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,    HY_Q,    HY_W,    HY_E,    HY_R,    HY_T,                      HY_Y,    HY_U,    HY_I,    HY_O,    HY_P,    KC_NO, 
		KC_NO,    HY_A,    HY_S,    HY_D,    HY_F,    HY_G,                      HY_H,    HY_J,    HY_K,    HY_L,    HY_SCLN, KC_NO, 
		KC_NO,    HY_Z,    HY_X,    HY_C,    HY_V,    HY_B,    KC_NO,   KC_NO,   HY_N,    HY_M,    HY_COMM, HY_DOT,  HY_SLSH, KC_NO, 
		                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	)
};

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return OLED_ROTATION_270;
}

// Domantas Petrauskas - https://github.com/qmk/qmk_firmware/blob/master/keyboards/lily58/keymaps/domnantas/keymap.c
static void render_bongo_cat(void) {

    // Idle animation
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 128, 64, 64, 32, 32, 32, 32, 16, 16,  16, 16, 16, 8,  8,  4,  4, 4, 8, 48, 64, 128, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 100, 130, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 192, 193, 193, 194, 4, 8, 16, 32, 64, 128, 0, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 56, 4, 3, 0, 0, 0, 0, 0, 0, 0,   12, 12, 12, 13, 1,  0,  64, 160, 33, 34, 18, 17, 17, 17, 9, 8, 8, 8,  8,  4,   4, 8, 8, 16, 16, 16, 16, 16, 17, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1,   1, 1, 1, 1, 1, 2, 2, 4, 4, 8, 8,   8,   8, 8, 7, 0, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0,   0,   0,   0,   0, 0, 0,  0,  0,  0,   0, 0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 128, 64, 64, 32, 32, 32, 32, 16, 16,  16, 16, 16, 8,  8,  4,  4, 4, 8, 48, 64, 128, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 100, 130, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 192, 193, 193, 194, 4, 8, 16, 32, 64, 128, 0, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 192, 56, 4, 3, 0, 0, 0, 0, 0, 0, 0,   12, 12, 12, 13, 1,  0,  64, 160, 33, 34, 18, 17, 17, 17, 9, 8, 8, 8,  8,  4,   4, 8, 8, 16, 16, 16, 16, 16, 17, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1,   1, 1, 1, 1, 1, 2, 2, 4, 4, 8, 8,   8,   8, 8, 7, 0, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0,   0,   0,   0,   0, 0, 0,  0,  0,  0,   0, 0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 64, 64, 64, 64, 32, 32,  32, 32, 16, 8,  4,  2,  2,  4, 24, 96, 128, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 194, 1, 1, 2, 2, 4, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96, 96, 0, 129, 130, 130, 132, 8, 16, 32, 64, 128, 0, 0, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 112, 25, 6, 0, 0, 0, 0, 0, 0, 0, 24,  24,  24, 27, 3,  0,  64, 160, 34, 36, 20, 18, 18, 18, 11, 8, 8,  8,  8,   5, 5, 9, 9, 16, 16, 16, 16, 16, 17, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1, 1, 1, 1, 1, 1, 2, 2, 4, 4, 8, 8, 8, 8, 8, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0, 0,   0,   0,   0,   0, 0,  0,  0,  0,   0, 0, 0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,  128, 128, 0, 0, 0, 0, 0, 128, 64, 64, 32, 32, 32, 32, 16, 16,  16, 16, 8,  4,  2,  1,  1, 2, 12, 48, 64, 128, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 225, 0, 0, 1, 1, 2, 2, 1, 0, 0, 0, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 192, 193, 193, 194, 4, 8, 16, 32, 64, 128, 0, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 112, 12, 3,   0,   0, 0, 0, 0, 0, 0,   12, 12, 12, 13, 1,  0,  64, 160, 33, 34, 18, 17, 17, 17, 9, 8, 8,  8,  8,  4,   4, 8, 8, 16, 16, 16, 16, 16, 17, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1, 1, 1, 1, 1, 1, 2, 2, 4, 4, 8, 8,   8,   8, 8, 7, 0, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0,   0,   0,   0,   0, 0, 0,  0,  0,  0,   0, 0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,  0, 0, 0, 0, 0, 0, 0, 128, 64, 64, 32, 32, 32, 32, 16, 16,  16, 16, 8,  8,  4,  2,  2, 2, 4, 56, 64, 128, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 226, 1, 1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 192, 193, 193, 194, 4, 8, 16, 32, 64, 128, 0, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 112, 12, 3, 0, 0, 0, 0, 0, 0, 0,   12, 12, 12, 13, 1,  0,  64, 160, 33, 34, 18, 17, 17, 17, 9, 8, 8, 8,  8,  4,   4, 8, 8, 16, 16, 16, 16, 16, 17, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1, 1, 1, 1, 1, 1, 2, 2, 4, 4, 8, 8,   8,   8, 8, 7, 0, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0,   0,   0,   0,   0, 0, 0,  0,  0,  0,   0, 0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    };

    // Prep animation
    static const char PROGMEM prep[][ANIM_SIZE] = {

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,  128, 128, 0, 0,  0, 0, 0,   128, 64,  64,  32,  32, 32, 32, 16, 16, 16, 16, 8,  4,  2,  1,  1, 2, 12, 48, 64, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 225, 0, 0, 1, 1, 2, 2, 129, 128, 128, 0, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 0, 1, 225, 26, 6, 9, 49, 53, 1, 138, 124, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 112, 12, 3,   0,   0, 24, 6, 5, 152, 153, 132, 195, 124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17, 9, 8, 8,  8,  8,  4,   4, 4, 4, 4, 4, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1, 1, 1, 1, 1, 1, 0,   0,   0,   0, 0, 0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0, 0, 0,   0,  0, 0, 0,  0,  0, 0,   0,   0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    };

    // Typing animation
    static const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE] = {

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,  128, 128, 0, 0,  0, 0, 0,   128, 64,  64, 32,  32, 32, 32, 16, 16, 16, 16, 8,  4,  2,  1,  1, 2, 12, 48, 64, 128, 0, 0, 0, 0,  0,  0,  0,  248, 248, 248, 248, 0,  0,   0,   0,   0,   128, 128, 0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 225, 0, 0, 1, 1, 2, 2, 129, 128, 128, 0, 0, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 0, 1, 1, 2, 4, 8, 16, 32, 67, 135, 7, 1, 0, 184, 188, 190, 159, 95, 95, 79, 76, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 112, 12, 3,   0,   0, 24, 6, 5, 152, 153, 132, 67, 124, 65, 65, 64, 64, 32, 33, 34, 18, 17, 17, 17, 9, 8, 8,  8,  8,  4,   4, 8, 8, 16, 16, 16, 16, 16,  17,  15,  1,   61, 124, 252, 252, 252, 252, 252, 60, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 2,  2,   1, 1, 1, 1, 1, 1, 0,   0,   0,   0, 0, 0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,   0, 0, 0, 0,   0,   1,   3,   3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,  128, 128, 0, 0, 0, 0, 0, 128, 64, 64, 32, 32, 32, 32, 16, 16,  16, 16, 8,  4,  2,  1,  1, 2, 12, 48, 64, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30,  225, 0,   0,   1,   1,   2,  2,  1, 0, 0, 0, 0, 128, 128, 0,   0,   0,   0,   0, 0, 0, 0, 0, 128, 0, 48, 48, 0, 0, 1, 225, 26, 6, 9, 49, 53, 1, 138, 124, 0, 0, 128, 128, 128, 128, 64, 64, 64, 64, 32, 32, 32, 32, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 112, 12, 3,   0,   0, 0, 0, 0, 0, 0,   0,  0,  0,  1,  1,  0,  64, 160, 33, 34, 18, 17, 17, 17, 9, 8, 8,  8,  8,  4,   4, 4, 4, 4, 4, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 64, 64, 64, 64, 64, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 8, 8, 8, 8, 8, 4, 4, 4, 4, 4, 2, 3, 122, 122, 121, 121, 121, 121, 57, 49, 2, 2, 4, 4, 8, 8,   8,   136, 136, 135, 128, 0, 0, 0, 0, 0, 0,   0, 0,  0,  0, 0, 0, 0,   0,  0, 0, 0,  0,  0, 0,   0,   0, 0, 0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

    };

    void animation_phase(void) {
        if (get_current_wpm() <= IDLE_SPEED) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES - 1) - current_idle_frame)], ANIM_SIZE);
        }

        if (get_current_wpm() > IDLE_SPEED && get_current_wpm() < TAP_SPEED) {
            oled_write_raw_P(prep[0], ANIM_SIZE);
        }

        if (get_current_wpm() >= TAP_SPEED) {
            current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
            oled_write_raw_P(tap[abs((TAP_FRAMES - 1) - current_tap_frame)], ANIM_SIZE);
        }
    }

    if (get_current_wpm() != 000) {
        oled_on();

        if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }

        anim_sleep = timer_read32();
    } else {
        if (timer_elapsed32(anim_sleep) > oled_timeout) {
            oled_off();
        } else {
            if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}

static void render_status(void) {
    // WPM
    oled_write_ln_P(PSTR("wpm"), false);
    sprintf(wpm_str, "%03d", get_current_wpm());
    oled_write_ln(wpm_str, false);
    oled_write_ln_P(PSTR(""), false);

    // Layer display
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("deflt"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("raise"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("lower"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("adjst"), false);
            break;
        default:
            oled_write_ln_P(PSTR("error"), false);
    }

    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR(""), false);
    oled_write_ln_P(PSTR(""), false);
    oled_write_ln_P(PSTR("caps"), led_state.caps_lock);
}

void oled_task_user(void) {
  if (is_keyboard_master()) {
    render_status();
  } else {
    render_bongo_cat();
  }
}
#endif // OLED_ENABLE
