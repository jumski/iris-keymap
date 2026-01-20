# Agent Guidelines for iris-keymap

## Prerequisites

- QMK firmware environment installed and configured (https://docs.qmk.fm/#/new_build)
- `gh` CLI tool installed for releases
- Symlinked files in QMK directory - see README.md for setup instructions

## Build Commands

- `make compile` - Compile keymap using QMK
- `make flash` - Flash firmware to keyboard
- `make main` - Compile and flash in one command
- `make flash_latest` - Download latest GitHub release and flash
- `make download_latest` - Download latest .hex firmware file
- `make release` - Create GitHub release (interactive, increments version)

## Project Structure

This is a QMK firmware keymap for the Keebio Iris rev6 split keyboard.

**Key files:**
- `keymap.c` - Main keymap definition and custom logic
- `config.h` - QMK configuration options
- `rules.mk` - QMK features to enable
- `keymap.json` - JSON representation of keymap
- `Makefile` - Build targets for compile/flash/release

## Code Style

**General:**
- Language: C (QMK firmware)
- Indentation: 3 spaces (strictly no tabs)
- Comments: Use `//` for single-line notes, `/* */` for multi-line blocks
- Header guards: Use `#pragma once` in config.h (not traditional `#ifndef` guards)

**Imports:**
- Always start with `#include QMK_KEYBOARD_H` at the top of keymap.c
- Wrap optional features with `#ifdef` guards (e.g., `#ifdef ENCODER_ENABLE`)

**Layer Definitions:**
- Define layer constants as `#define _LAYER_NAME N`
- Start from 0 and increment sequentially
- Use descriptive names with underscore prefix (e.g., `_QWERTY`, `_LOWER`, `_RAISE`, `_ADJUST`, `_APEX`)

**Custom Keycodes:**
- Define in enum starting with `SAFE_RANGE`
- Process in `process_record_user()` function
- Check `record->event.pressed` for press vs release
- Return `false` to stop further processing, `true` to continue
- Always include `break` after each case in switch statement

**Tap Dance:**
- Define actions in enum starting from 0
- Use `ACTION_TAP_DANCE_DOUBLE(key1, key2)` for double-tap behavior
- Define tap dance actions in `tap_dance_actions[]` array
- Reference with `TD(TD_NAME)` in keymap

**Keymap Arrays:**
- Use `LAYOUT()` macro with ASCII art alignment for readability
- Transpose transparent keys: use `_`, `__`, `___`, `____` for different lengths (visual alignment)
- Align comments vertically to create clear keyboard layout visualization
- Use `KC_TRNS` or defined macros for transparent keys

**Naming Conventions:**
- Layer constants: `_UPPER_CASE` with leading underscore
- Custom keycodes: `UPPER_CASE` without underscore prefix
- Functions: `snake_case` for QMK callbacks (e.g., `process_record_user`, `layer_state_set_user`)
- QMK macros: `UPPER_CASE` (e.g., `tap_code`, `layer_on`, `register_code`)
- Global state variables: `snake_case` with descriptive names (e.g., `is_alt_tab_active`, `is_apex_lalt_active`)
- Enums: `UPPER_CASE` for both tap dances and custom keycodes

**State Management:**
- Use global boolean variables for tracking state
- Declare at file scope, initialize on definition
- Clean up state on layer changes (e.g., unregister held keys in `layer_state_set_user`)
- Track key hold states to prevent unintended key presses

**Common QMK Functions:**
- `matrix_init_user()` - Initialize on startup (currently unused, consider adding if needed)
- `process_record_user()` - Handle custom keycodes, returns bool
- `encoder_update_user(index, clockwise)` - Handle rotary encoder events, returns bool
- `layer_state_set_user(state)` - Handle layer changes, returns layer_state_t

**Layer Management:**
- `layer_on(layer)` - Activate layer temporarily
- `layer_off(layer)` - Deactivate layer
- `set_single_persistent_default_layer(layer)` - Set persistent base layer
- `update_tri_layer(lower, raise, upper)` - Enable third layer when two others active
- Call `update_tri_layer()` on both press and release of layer keys

**Keycodes:**
- `tap_code(key)` - Send single keypress (tap and release)
- `register_code(key)` / `unregister_code(key)` - Hold/release for key combinations
- Use `KC_*` constants for standard keys from QMK keycodes.h
- Use `QK_GESC` for Escape/Backtick behavior
- Use mod-tap functions like `RCTL_T(KC_QUOT)` for modified tap keys

**Configuration (config.h):**
- Tapping timeout: `#define TAPPING_TERM 175`
- Left side master: `#define MASTER_LEFT`
- Serial communication: `#define USE_SERIAL` (disable `USE_I2C`)
- Caps Word: `#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD`
- See https://docs.qmk.fm/#/config_options for full config reference

**Features (rules.mk):**
- `CAPS_WORD_ENABLE` - Caps Word functionality
- `TAP_DANCE_ENABLE` - Tap dance keycodes
- `MOUSEKEY_ENABLE` - Mouse key support (currently not enabled, add if needed)
- See https://docs.qmk.fm/#/feature_list for all available features

## Testing

No unit tests - validation is performed by flashing to the physical keyboard and testing functionality. Always test:
- Basic key presses on QWERTY layer
- Layer switching (LOWER, RAISE, ADJUST, APEX)
- Custom keycodes (ALT_TAB, APEX_* keys)
- Tap dance functionality (e.g., TD_APEX_ESC)
- Rotary encoders (if connected)
- State cleanup on layer changes

## Common Pitfalls

- **Missing tri-layer updates**: Always call `update_tri_layer()` on both key press and release in `process_record_user()`
- **Stuck modifiers**: Always unregister held keys in `layer_state_set_user()` to prevent modifier key state leaks
- **Incorrect return values**: Custom keycodes should return `false` to stop QMK processing; return `true` for pass-through
- **Missing break statements**: Always include `break` after each case in switch statement to avoid fall-through
- **Incorrect indentation**: Use exactly 3 spaces, never tabs (QMK linters will complain)
- **Hard-coded paths**: Makefile has user-specific paths (`/home/jumski/...`) - verify these match local setup
- **Symlink issues**: Ensure keymap files are properly symlinked in QMK directory before compiling
- **Missing preprocessor guards**: Wrap optional features (encoders, etc.) with `#ifdef` checks

## Debugging

Since there are no unit tests, debugging requires:
1. **Compilation verification**: Run `make compile` to check for syntax errors
2. **Flash testing**: Use `make flash` to load firmware and test on hardware
3. **Debug logging**: Add `dprintf()` or `uprintf()` statements for runtime debugging (requires CONSOLE_ENABLE)
4. **QMK Configurator**: Use https://config.qmk.fm to visualize and test keymaps without flashing
5. **Keycode reference**: See https://docs.qmk.fm/#/keycodes for available keycodes
6. **Layer debugging**: Add visual indicators (e.g., RGB LED changes) on layer activation

## Version Management

Version follows semantic versioning (MAJOR.MINOR.PATCH). Release process increments third version number automatically via `release.sh`. Ensure all changes are:
1. Committed to local repository
2. Pushed to remote origin
3. Tested on hardware before release

Release script prompts for confirmation before creating GitHub release with compiled `.hex`, `.elf`, and `.map` files.