# 🪐🤺 Lightsaber

## Description

Introducing... a homemade **lightsaber**, powered by an ESP32 microcontroller! 

This project is still a **work in progress**, so please hang tight while I get things going.


## How It Works

Start by clicking the joystick. This will activate the lightsaber with a cool sound effect (lightspeed)! 

Maneuver the joystick to move between colors. Once you've found a color you like, hold that spot / angle, and click the joystick as you would a button. Keep your ears open for a themed sound corresponding to that specific color! 

To change the blade's hue, angle the joystick button, give it another click, and you'll have returned to "color selection mode". 

When you're ready to deactivate the lightsaber, simply click the joystick again and watch as the lights recede. Listen closely for a message as you go!


## Electronics / Pieces

1. **ESP32-WROOM-32**: microcontroller (logic)
2. **Addressable LED strip (WS2812B)**: lightsaber blade
    - individual LED manipulation 
    - actuator — output
3. **Joystick module**: lightsaber (de)activation
    - range of x, y values to map (+ button)
    - sensor — input
4. **9V battery + clip**: power source
5. **Power supply module**: 5V / 3.3V output converter
6. **Others ad hoc**: cables, wires, resistors, etc


## Tools / Tech

1. **C++**: code running on device
2. **Python**: code to play themed sounds
    - connects via serial monitor
    - using `playsound` library
    - with `PyObjC` to run efficiently
3. **PlatformIO**: embedded development toolset
    - VS Code extension
    - cross-platform, cross-architecture
    - debugger, static analysis, unit testing, etc
    - project configs (build env, dependencies)


## Features

- **⭐️ Lightsaber (de)activation**: Exciting visual effects with sequential LEDs.
- **🎨 Customizable saber blade**: Highly flexible color wheel selection via joystick.

- **🔒 Color locking ability**: Option to hold selected color in place with joystick button.
- **🎵 Themed sound effects**: Music and phrases to match colors and saber (de)activation.


## Video Demos

![Video demo of lightsaber activation and color selection](./example-media/lightsaber-activation-demo.gif)


## Example Images

🎮 Wiring View   |   🌈 Lightsaber LED Strip   |   🔌 Wiring Above
:-------------------------:|:-------------------------:|:-------------------------:
![Example image of front wiring view](./example-media/wiring-view.png)  |  ![Example image of saber LED strip](./example-media/LED-strip.png)  |  ![Example image of above wiring view](./example-media/wiring-above.png)



