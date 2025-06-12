import serial
from playsound import playsound


# Map color names to sound file paths 
color_sounds = {
    "Red": "media/red.mp3",
    "Orange": "media/orange.mp3",
    "Yellow": "media/yellow.mp3",
    "Green": "media/green.mp3",
    "Cyan": "media/cyan.mp3",
    "Blue": "media/blue.mp3",
    "Purple": "media/purple.mp3",
    "Magenta": "media/magenta.mp3",
    "Black": "media/black.mp3",
    "White": "media/white.mp3",
    "Gray": "media/gray.mp3",
}


def main():
    # ESP32 port plugged in
    ser = serial.Serial('/dev/cu.usbserial-0001', 9600)

    while True:
        line = ser.readline().decode('utf-8').strip()
        print("Received from serial:", line)

        if line.startswith("Play COLOR:"):
            color = line.split("Play COLOR:")[1]

            sound_file = "media/WEEEEEOW.mp3"

            # sound_file = color_sounds.get(color)

            if sound_file:
                print(f"Playing sound for {color}")
                playsound(sound_file)
            else:
                print(f"No sound mapped for color: {color}")


if __name__ == "__main__":
    main()


