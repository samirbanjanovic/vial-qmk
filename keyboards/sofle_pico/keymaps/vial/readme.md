![SofleKeyboard default keymap](https://i.imgur.com/MZxVvm9.png)
![SofleKeyboard adjust layer](https://i.imgur.com/f5sKy0I.png)


# Default keymap for Sofle Pico Keyboard

Adapted from the classic Sofle Keyboard by Josef Adamcik.
Simplified to serve as a starting point for further customizations.
Layout in [Keyboard Layout Editor](http://www.keyboard-layout-editor.com/#/gists/76efb423a46cbbea75465cb468eef7ff)
Left encoder controls volume up/down/mute. Right encoder PGUP/PGDOWN.

The master OLED keeps the session word count on row 8 and displays the current
RGB Matrix effect name across rows 9-11. Effect names use spaces in place of
underscores; `RGB: OFF` is shown when RGB is disabled. The display updates when
the effect is changed, including changes made through Vial.

## Rainbow Rain

Rainbow Rain adds falling drops with fading trails. Each physical row has a
different rainbow hue, with matching row colors on both halves. RGB brightness
and animation speed keycodes apply; hue and saturation are fixed by the effect.
The original Digital Rain effect remains available.

After building and flashing this keymap to both halves, reconnect in Vial, select
a key on your preferred layer, and assign **Rainbow Rain** from the **User** tab.
Pressing it enables RGB and selects Rainbow Rain, saving the selection to EEPROM.
No existing key binding is replaced. The OLED displays `RAINBOW RAIN`.

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

Assign **Rainbow Pools** from Vial's **User** tab after flashing both halves and
reconnecting. Pressing the key enables RGB and saves the selected effect, just
like Rainbow Rain. Existing bindings and effects are unchanged, and the OLED
shows `RAINBOW POOLS`.

Use the RGB brightness keycodes to set glow intensity and animation speed
keycodes to adjust how quickly pools appear and fade. Colors are drawn from the
full rainbow at full saturation. The same Vial lighting-dropdown limitations
described above apply.
