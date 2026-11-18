# RotaryEncoderMousewheel_ch552

Custom firmware for a cheap CH552G-based USB rotary encoder ("SmartKeyBoard_EC11", sold as a volume knob, see Amazon link below).

<p align="center"><img width="400" src="https://m.media-amazon.com/images/W/BW_MEDIAX_AVIF_MEASUREMENT_1306696-T4/images/I/618LQ0KQ7FL._AC_SL1500_.jpg" alt="USB Volume Knob" /></p>

It turns the knob into a **mouse wheel** and the encoder push button into **Ctrl**, so holding the button while turning gives Ctrl + wheel (zoom, fine control in Reaper, etc.). No driver or configuration is needed.

The idea behind this is to use it with the Reaper DAW for universal parameter control. Reaper already allows controlling it´s parameters and plugin parameters per mousewheel when hovering the mousecursor over it (needs to be enabled in preferences). Pushing the encoder button equals holding "left ctrl" for fine-grain control in Reaper. No controller needs to be configured in Reaper, it´s plug and play.
> **Warning:** flashing overwrites the original firmware. I did not find a way to back it up or restore it. You do this at your own risk; no warranty (see LICENSE).

## Behavior

| Action | Result |
|---|---|
| Turn knob | Mouse wheel up / down (one step per detent) |
| Press knob (hold) | Left Ctrl held while pressed |
| LED | Disabled in the firmware, prepared for custom use |

The device enumerates as a USB keyboard + mouse composite (taken from the ch55xduino `HidKeyboardMouse` example).

## Hardware

- Device: SmartKeyBoard_EC11 (303SMARKET2), original USB ID `1189:8890` ("LCKEY")
- MCU: WCH **CH552G** (SOP-16)
- EEPROM: 24C08C, **not used** by this firmware
- Encoder: EC11, detented. An undetented encoder (e.g. Bourns PEC11R-4015F-S0024) works too if you want free-spinning scrolling.
The Bourns has a 6mm axis with a flat side and takes standard potentiometer knob caps.

### Pin mapping

"Package pin" is the physical pin number of the CH552G SOP-16. The Arduino pin number (ch55xduino) is the port number times 10 plus the bit number within the port: P3.1 is port 3, bit 1, so `3 * 10 + 1 = 31`; P3.0 is `3 * 10 + 0 = 30`.

| Function | Package pin | Port | Arduino pin |
|---|---|---|---|
| Encoder A | 7 | P3.1 | 31 |
| Encoder B | 8 | P3.0 | 30 |
| Encoder common (W) | 14 | GND | - |
| Encoder button (to GND) | 10 | P3.3 | 33 |
| LED (VCC = pin 15, active low) | 11 | P3.4 | 34 |
| Boot pad P6 | 16 / 12 | V33 / UDP (D+) | - |

The other CH552G pins are not used by this project. They likely connect to the 24C08C EEPROM.

## Repository layout

```
RotaryEncoderMousewheel_ch552/
  RotaryEncoderMousewheel_ch552.ino
```

The USB HID code (`src/userUsbHidKeyboardMouse`) is **not** included in this repository. You copy it from your own ch55xduino installation, see below.

The `.ino` must live in a folder with the same name.

## Build (Arduino IDE)

You can skip this step by using RotaryEncoderMousewheel_ch552.ino.hex from the repo, which is a compilation of RotaryEncoderMousewheel_ch552.ino.
You only need to recompile if you want to modify the behaviour.

1. Install the **CH55xDuino** core (tested with 0.0.26) via the Boards Manager. Add this URL under *Preferences -> Additional boards manager URLs*:
   `https://raw.githubusercontent.com/DeqingSun/ch55xduino/ch55xduino/package_ch55xduino_mcs51_index.json`
2. Put the sketch in a folder named `RotaryEncoderMousewheel_ch552`.
3. **Copy the USB code from the core's example.** Copy the `src` folder of the `HidKeyboardMouse` example next to the `.ino`, so that this file exists:
   `RotaryEncoderMousewheel_ch552/src/userUsbHidKeyboardMouse/USBHIDKeyboardMouse.h`

   On Windows the example is here (adjust the core version):
   `%LOCALAPPDATA%\Arduino15\packages\CH55xDuino\hardware\mcs51\<version>\libraries\Generic_Examples\examples\05.USB\HidKeyboardMouse\src`

   You can also open it in the IDE via *File -> Examples -> Generic_Examples -> 05.USB -> HidKeyboardMouse*. Leave the copied files unchanged.
4. Open the sketch and set under *Tools*:
   - Board: **CH552**
   - USB Settings: **USER CODE w/ 148B USB ram**
   - Clock Source: **16 MHz (internal) 3.3V or 5V**
5. Compile, then *Sketch -> Export Compiled Binary*. The `.hex` ends up in the sketch's `build` folder.

If you skip the USB setting, the sketch stops with a `#error`. If you skip step 3, the compiler reports `USBHIDKeyboardMouse.h: No such file or directory`.

## Flash

1. Install **WCHISPStudio** (WCH's official tool, also called WCHISPTool) from https://www.wch.cn/products/CH552.html. Select chip series **CH55x**, chip **CH552**, load your `.hex`. Leave the configuration options at their defaults.
2. Open the case. Unplug the knob.
3. Enter the CH552 bootloader by connecting the P6 pad with a wire while plugging in the USB cable.
   - The P6 pad is wired to V33 (pin 16) and, through 10 kOhm, to D+ (pin 12). Bridging it pulls D+ high at power-up.
4. The chip shows up in WCHISPStudio's device list (if not, try reconnecting a few times, it sometimes fails). It may **not** appear in Windows Device Manager or Zadig, and it can disappear again after a short time. Load the `.hex` first and click **Download** immediately.
5. Unplug, plug in again without the bridge. The knob should now scroll the mousewheel and rotary push is "left ctrl".

### Bootloader troubleshooting

- Hold the bridge at P6 from before plugging in until the chip is detected. This should only take a few seconds if working, otherwise try reconnecting a few times.
- Use a USB 2.0 port directly on the PC, no hub, short cable.
- Detection in Device Manager as `4348:55E0` ("USB Module") is not required when you use WCHISPStudio.

## Customize

In the `.ino`:

| Setting | Effect |
|---|---|
| `STEPS_PER_DETENT` | Edges counted per detent. 4 is typical for EC11; use 2 for more output per click, 8 for one wheel step every second detent |
| `REVERSE_DIRECTION` | `1` swaps up/down |
| LED lines | The `digitalWrite(PIN_LED, ...)` lines are commented out; re-enable them for the LED |

To reduce scrolling per click on Windows without reflashing, set *Settings -> Bluetooth & devices -> Mouse -> Lines to scroll at a time* to 1 (applies to all mice).

## Known issue: Windows compile error in the core

With CH55xDuino 2026.07.10 and a recent Arduino CLI, compiling can fail with
`sdcc.sh: syntax error: unexpected "("`.
The wrapper script uses a Bash array in the `re12` branch, but Windows runs it with plain `sh`. Replacing the array with a POSIX loop fixes it:

```sh
N=$#
skip_next=0
while [ $N -gt 0 ]; do
    arg=$1; shift; N=$((N-1))
    if [ $skip_next -eq 1 ]; then skip_next=0; continue; fi
    case "$arg" in -c|-MMD|-MF) skip_next=1; continue ;; esac
    set -- "$@" "$arg"
done
```

then use `"$@"` instead of `"${FILTERED_ARGS[@]}"`. The file is `...\Arduino15\packages\CH55xDuino\tools\MCS51Tools\<version>\wrapper\sdcc.sh`. A core update can overwrite the change.

## Credits

- [ch55xduino](https://github.com/DeqingSun/ch55xduino) by Deqing Sun (toolchain and the `HidKeyboardMouse` example this project builds on)
- https://rabid-inventor.blogspot.com/2020/05/getting-started-with-ch55x.html

- https://hackaday.io/project/172143/instructions

- https://www.amazon.de/USB-Lautst%C3%A4rkeregler-Multimedia-Typ-C-Controller-Medienanpassung-7-Farben-RGB-Licht-Tastenkombinationen/dp/B0DQBYZVZH/ref=dp_prsubs_d_sccl_3/258-9941439-7408910?pd_rd_w=1n1LF&content-id=amzn1.sym.f16eb515-347d-4f51-8281-c041e0040816&pf_rd_p=f16eb515-347d-4f51-8281-c041e0040816&pf_rd_r=DSEAGCEM69QQG6XYZ351&pd_rd_wg=t5JXW&pd_rd_r=54f8d289-5d0e-4a47-b37d-db57e750ca91&pd_rd_i=B0DQBYZVZH&psc=1

## License

The code in this repository is MIT licensed, see [LICENSE](LICENSE).
The ch55xduino core and its examples (including the copied `src` folder) are not part of this repository and remain under their own license terms.
