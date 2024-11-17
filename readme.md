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

## TODO
- [X] Mouse emulation
- [X] Macro
- [X] Repeat 8 times layer
