# sheriff

*QMK config for the [Sheriff keyboard](https://github.com/AJGamma/sheriff-keyboard)*

<!-- See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs). -->


## Usage
1. Install QMK on your local machine. See [QMK documentation](https://docs.qmk.fm/newbs_getting_started)
2. Clone this repo into the `keyboards` directory:
```bash
# for example, in MacOS or Linux:
cd ~/qmk_firmware/keyboards
git clone https://github.com/AJGamma/sheriff-qmk-config
```
3. Compile and flash:
```bash
qmk compile -kb sheriff -km custom_scroll_layers
qmk flash -kb sheriff -km custom_scroll_layers
# or:
qmk flash -kb sheriff -km default
```
use `qmk config user.keyboard=sheriff user.keymap=custom_scroll_layers` to set default parameters, and then you can use `qmk compile` and `qmk flash` without specifying the keyboard and keymap.


## Branches
Currently there are 2 versions of keymaps, each version uses a different way to implement scroll wheel functions.

- the `default` version uses layers to implement scroll wheel functions. The limitation of this method is that you can only define 16 layer-tap layers, and there's currently no implementations of macros, so repeat-8-times behavior is not possible. 
- the `custom_scroll_layers` version uses custom functions to implement scroll wheel functions. This version is more flexible, with macros implemented, but the tap behaviors are not as advanced as that of the `default` version, with tap-hold repeating unavailable.

## Scroll wheel behaviors

### Default version

| Key Held  | Left Counter-clockwise | Left Clockwise | Right Counter-clockwise       | Right Clockwise | Description |
|-----------|------------------------|----------------|-------------------------------|-----------------|-------------|
| None      | Left                   | Right          | Down                          | Up              |             |
| Backspace | Backspace              | Delete         | Backspace                     | Delete          | Sym layer   |
| Esc       | Volume-down            | Volume-up      | Mouse wheel down              | Mouse wheel up  | Nav layer   |
| Space     | Left                   | Right          | Repeat last key's alternative | Repeat last key | Num layer   |
| V         | Mouse left             | Mouse right    | Mouse down                    | Mouse up        | Mouse layer |
| W         | b                      | w              | }                             | {               |             |
| E         | b                      | e              | Down                          | Up              |             |
| U         | u                      | C-r            | C-r                           | u               |             |
| Y         | C-z                    | C-y            | C-y                           | C-z             |             |
| Z         | C-z                    | S-C-z          | S-C-z                         | C-z             |             |

### Custom_scroll_layers version

| Key Held  | Left Counter-clockwise      | Left Clockwise              | Right Counter-clockwise     | Right Clockwise               | Description |
|-----------|-----------------------------|-----------------------------|-----------------------------|-------------------------------|-------------|
| None      | Left                        | Right                       | Down                        | Up                            |             |
| Backspace | Backspace                   | Delete                      | Backspace                   | Delete                        |             |
| Esc       | Volume-down                 | Volume-up                   | Mouse wheel down            | Mouse wheel up                | Sym layer   |
| Space     | Left                        | Right                       | Repeat last key             | Repeat last key's alternative | Nav layer   |
| V         | Mouse left                  | Mouse right                 | Mouse down                  | Mouse up                      | Mouse layer |
| W         | b                           | w                           | }                           | {                             |             |
| E         | ge                          | e                           | Down                        | Up                            |             |
| 8 or I    | Next activated behavior * 8 | Next activated behavior * 8 | Next activated behavior * 8 | Next activated behavior * 8   |             |
| U         | u                           | C-r                         | C-r                         | u                             |             |
| Y         | C-z                         | C-y                         | C-y                         | C-z                           |             |
| Z         | C-z                         | S-C-z                       | S-C-z                       | C-z                           |             |

### Mouse Emulation
Hold `a` `s` `d` for speed control see [keymap file](keymaps/custom_scroll_layers/keymap.c) for details.


## TODO
- [X] Mouse emulation
- [X] Macro
- [X] Repeat 8 times layer
- [ ] Key codes depending on different OS's
- [X] fix the problem that repeat does not include modifiers
- [X] disable scroll layers and enable holds on wasd layer
- [X] implement mod-tap for custom_scroll_layers keymap
- [ ] Single handed use
- [ ] Macropad layer
- [ ] Dynamic macros
- Scroll wheel behaviors:
    - [ ] Vim jump list
    - [ ] Emacs undo/redo
    - [O] Vim C-a C-x
    - [O] Vim semicolon comma
    - [O] Vim search
    - [O] Vim C-o C-i
    - [X] Vim gj gk
