import serial
from playsound import playsound


# Map color names & event flags to sound file paths 
color_sounds = {
    "red": "media/darth-vader.mp3",
    "orange": "media/obi-wan.mp3",
    "yellow": "media/jar-jar.mp3",
    "green": "media/yoda.mp3",
    "cyan": "media/x-wing.mp3",
    "blue": "media/luke-skywalker.mp3",
    "purple": "media/WEEEEEOW.mp3",
    "magenta": "media/tie-fighter.mp3",
    "black": "media/tie-fighter.mp3",
    "white": "media/x-wing.mp3",
    "gray": "media/r2d2.mp3",
}

event_sounds = {
    "activated": "media/lightspeed.mp3",
    "deactivated": "media/c3po.mp3",
}


def main():
    # ESP32 port plugged in
    ser = serial.Serial('/dev/cu.usbserial-0001', 9600)

    while True:
        line = ser.readline().decode('utf-8').strip()
        print("Received from serial:", line)
        
        sound_file = None
        source = None

        if line.startswith("Play COLOR:"):
            color = line.split("Play COLOR:")[1].strip().lower()
            sound_file = color_sounds.get(color)
            source = f"color '{color}'"

        elif line.startswith("Play EVENT:"):
            event = line.split("Play EVENT:")[1].strip().lower()
            sound_file = event_sounds.get(event)
            source = f"event '{event}'"

        if sound_file:
            print(f"Playing sound for {source}: {sound_file}")
            playsound(sound_file)
        else:
            print(f"No sound mapped for {source}")


if __name__ == "__main__":
    main()


