import edge_tts
import pygame
import re
import asyncio

from io import BytesIO

def extract_short_names_from_file(file_path, locale='en'):
    with open(file_path, 'r') as file:
        file_content = file.read()
    
    # Find all entries in the file
    entries = re.findall(r'Name: .*?\nShortName: (\S+)\nGender: .*?\nLocale: (en-\S+)', file_content, re.DOTALL)
    
    # Filter the short names where the locale matches
    short_names = [short_name for short_name, file_locale in entries if file_locale.startswith(locale)]
    
    return short_names

def initiate_text_to_speech():
    pygame.mixer.init()

def close_text_to_speech():
    pygame.mixer.quit()

def say(TEXT: (str | None), VOICE: str):
    """
    A simple wrapper that allows us to easily use the edge_tts library to say the given text.
    
    Args:
        TEXT (str): The text that we want to say.
        VOICE (str): The voice that we want to use to say the text.
    """
    
    SPEECH_RATE = "+25%"  # Increase speech speed by 50%
    
    if TEXT is None:
        return

    try:
        data = BytesIO()
        
        communicate = edge_tts.Communicate(TEXT, VOICE, rate=SPEECH_RATE)
        for chunk in communicate.stream_sync():
            if chunk["type"] == "audio":
                data.write(chunk["data"])
                
        data.seek(0)
        
        sound = pygame.mixer.Sound(data)
        sound.play()
        while pygame.mixer.get_busy():
            pygame.time.Clock().tick(3)
            
        sound.stop()
        data.close()
    except:
        print("An error occurred while trying to say the text.")
        # pygame.mixer.stop()
    

if __name__ == "__main__":
    say("Today's weather is sunny", 'en-AU-WilliamNeural')
    voices = extract_short_names_from_file('voices.txt')
    for voice in voices:
        print(voice)
        say("I love dogs and cats because they are fluffy", voice)
