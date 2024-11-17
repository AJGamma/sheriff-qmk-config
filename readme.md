# sheriff

*A short description of the keyboard/project*

* Keyboard Maintainer: [Adrian Lym](https://github.com/AJGamma)
* Hardware Supported: *The PCBs, controllers supported*
* Hardware Availability: *Links to where you can find this hardware*

Make example for this keyboard (after setting up your build environment):

    make sheriff:default

Flashing example for this keyboard:

    make sheriff:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).


## Branches
Currently there are 2 versions of keymaps, each version uses a different way to implement scroll wheel functions.

- the `default` version uses layers to implement scroll wheel functions. The limitation of this method is that you can only define 16 layer-tap layers, and there's currently no implementations of macros, so repeat-8-times behavior is not possible. 
- the `custom_scroll_layers` version uses custom functions to implement scroll wheel functions. This version is more flexible, with macros implemented, but the tap behaviors are not as advanced as that of the `default` version, with tap-hold repeating unavailable.

## Scroll wheel behaviors

### Default version

| Key Held | Left Counter-clockwise | Left Clockwise | Right Counter-clockwise | Right Clockwise |  Description |
| --------------- | --------------- | --------------- | --------------- | --------------- |  --------------- |
| None | Left | Right | Down | Up |  |
| Backspace | Backspace | Delete | Backspace | Delete | Sym layer |
| Esc | Volume-down | Volume-up | Mouse wheel down | Mouse wheel up | Nav layer |
| Space | Left | Right | Repeat last key | Repeat last key's alternative | Num layer |
| V | Mouse left | Mouse right | Mouse down | Mouse up | Mouse layer |
| W | b | w | } | { |  |
| E | b | e | Down | Up |  |
| U | u | C-r | C-r | u |  |
| Y | C-z | C-y | C-y | C-z |  |
| Z | C-z | S-C-z | S-C-z | C-z |  |

### Custom_scroll_layers version

| Key Held | Left Counter-clockwise | Left Clockwise | Right Counter-clockwise | Right Clockwise |  Description |
| --------------- | --------------- | --------------- | --------------- | --------------- |  --------------- |
| None | Left | Right | Down | Up |  |
| Backspace | Backspace | Delete | Backspace | Delete |  |
| Esc | Volume-down | Volume-up | Mouse wheel down | Mouse wheel up | Sym layer |
| Space | Left | Right | Repeat last key | Repeat last key's alternative | Nav layer |
| V | Mouse left | Mouse right | Mouse down | Mouse up | Mouse layer |
| W | b | w | } | { |  |
| E | ge | e | Down | Up |  |
| U | u | C-r | C-r | u |  |
| Y | C-z | C-y | C-y | C-z |  |
| Z | C-z | S-C-z | S-C-z | C-z |  |
| 8 | Next activated behavior 8 times | Next activated behavior 8 times | Next activated behavior 8 times | Next activated behavior 8 times |  |



## TODO
- [X] Mouse emulation
- [X] Macro
- [X] Repeat 8 times layer
- [ ] Key codes depending on different OS's
