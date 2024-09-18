import asyncio
import edge_tts
import pygame
import os
import re


def extract_short_names_from_file(file_path, locale='en'):
    with open(file_path, 'r') as file:
        file_content = file.read()
    
    # Find all entries in the file
    entries = re.findall(r'Name: .*?\nShortName: (\S+)\nGender: .*?\nLocale: (en-\S+)', file_content, re.DOTALL)
    
    # Filter the short names where the locale matches
    short_names = [short_name for short_name, file_locale in entries if file_locale.startswith(locale)]
    
    return short_names

def say(TEXT, VOICE):
    OUTPUT_FILE = "test.mp3"
    SPEECH_RATE = "+25%"  # Increase speech speed by 50%

    async def amain():
        communicate = edge_tts.Communicate(TEXT, VOICE, rate=SPEECH_RATE)
        await communicate.save(OUTPUT_FILE)

    try:
        asyncio.run(amain())

        pygame.mixer.init()
        pygame.mixer.music.load(OUTPUT_FILE)
        pygame.mixer.music.play()
        while pygame.mixer.music.get_busy():
            pygame.time.Clock().tick(3)
    finally:
        pygame.mixer.music.stop()
        pygame.mixer.quit()
        if os.path.exists(OUTPUT_FILE):
            os.remove(OUTPUT_FILE)

if __name__ == "__main__":
    say("Today's weather is sunny", 'en-AU-WilliamNeural')
    voices = extract_short_names_from_file('voices.txt')
    for voice in voices:
        print(voice)
        say("I love dogs and cats because they are fluffy", voice)
