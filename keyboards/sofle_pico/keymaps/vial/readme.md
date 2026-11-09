![SofleKeyboard default keymap](https://i.imgur.com/MZxVvm9.png)
![SofleKeyboard adjust layer](https://i.imgur.com/f5sKy0I.png)


# Vial keymap for Sofle Pico Keyboard

Adapted from the classic Sofle Keyboard by Josef Adamcik.
Layout in [Keyboard Layout Editor](http://www.keyboard-layout-editor.com/#/gists/76efb423a46cbbea75465cb468eef7ff)
Encoder mapping is not enabled for this keymap.

## Compiled layout defaults

The four layers in [keymap.c](keymap.c) and the typing/mouse settings in
[config.h](config.h) are imported from `default-sofle-layout.vil`. New builds
restore these defaults when Vial invalidates its saved layout after a build-ID
change; you do not need to reload that saved file after each firmware update.
Vial edits remain available and are retained across normal restarts.

The imported defaults include both custom RGB-effect bindings on Lower, NKRO
enabled, and zero tap-code delay. All saved macros, tap dances, combos, key
overrides, and alternate-repeat entries were empty. They retain their standard
empty/disabled defaults. The placeholder encoder entries were deliberately not
imported, and `layout_options: -1` was not treated as an actual layout option.
The saved file has no RGB effect or brightness settings to import.

Build and flash the Vial firmware to both halves. If an existing EEPROM layout
is still being used, resetting EEPROM restores these compiled defaults but also
clears subsequent Vial edits. Later changes made in Vial do not automatically
update the compiled defaults; export them again and update the source when you
want a new baseline. The original `.vil` file is not needed to compile.

The master OLED keeps the session word count on row 8 and displays the current
RGB Matrix effect name across rows 9-11. Effect names use spaces in place of
underscores; `RGB: OFF` is shown when RGB is disabled. The display updates when
the effect is changed, including changes made through Vial.

## Rainbow Rain

Rainbow Rain adds falling drops with fading trails. Each physical row has a
different rainbow hue, with matching row colors on both halves. RGB brightness
and animation speed keycodes apply; hue and saturation are fixed by the effect.
The original Digital Rain effect remains available.

The imported Lower layer already binds **Rainbow Rain**. You can also select
a key on your preferred layer and assign **Rainbow Rain** from Vial's **User** tab.
Pressing it enables RGB and selects Rainbow Rain, saving the selection to EEPROM.
The OLED displays `RAINBOW RAIN`.

Vial's lighting dropdown does not support custom effect names and may show Off
while Rainbow Rain runs. Use the custom key to select it, and select any built-in
effect in the lighting dropdown to switch back. Changing lighting settings in
Vial may also switch effects; use the RGB brightness and speed keycodes to adjust
Rainbow Rain without leaving it.

## Rainbow Pools

Rainbow Pools is a non-reactive underglow effect for the acrylic case: random
rainbow-colored pools softly bloom and fade beneath each half, blending where
they overlap. It uses LED positions rather than key rows or key presses, with
three independent pools per half. The appearance follows the LED coordinates
configured for the keyboard.

The imported Lower layer already binds **Rainbow Pools**. It is also available
from Vial's **User** tab. Pressing the key enables RGB and saves the selected
effect, just like Rainbow Rain. The OLED shows `RAINBOW POOLS`.

Use the RGB brightness keycodes to set glow intensity and animation speed
keycodes to adjust how quickly pools appear and fade. Colors are drawn from the
full rainbow at full saturation. The same Vial lighting-dropdown limitations
described above apply.
