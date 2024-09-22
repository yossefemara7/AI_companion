import speech_recognition as sr  # Import the Speech Recognition library
import pyttsx3  # Import the text-to-speech library
import subprocess  # Import subprocess to call the local model
import json  # Import json to handle model output
import re  # Import the regular expression library

def initialize_tts_engine():
    """Initializes the text-to-speech engine."""
    engine = pyttsx3.init()  # Create a TTS engine instance
    engine.setProperty('rate', 150)  # Set the speech rate
    engine.setProperty('volume', 1)  # Set the volume level (0.0 to 1.0)
    return engine  # Return the initialized engine

def remove_emojis(text):
    """Removes emojis from the given text."""
    emoji_pattern = re.compile(
        "["
        u"\U0001F600-\U0001F64F"  # emoticons
        u"\U0001F300-\U0001F5FF"  # symbols & pictographs
        u"\U0001F680-\U0001F6FF"  # transport & map symbols
        u"\U0001F1E0-\U0001F1FF"  # flags (iOS)
        u"\U00002702-\U000027B0"  # other symbols
        u"\U000024C2-\U0001F251"
        "]+", flags=re.UNICODE
    )
    return emoji_pattern.sub(r'', text)  # Substitute emojis with an empty string

def speak(engine, text):
    """Converts text to speech, ignoring emojis."""
    cleaned_text = remove_emojis(text)  # Remove emojis from the text
    engine.say(cleaned_text)  # Queue the cleaned text to be spoken
    engine.runAndWait()  # Wait until the speech is finished

def recognize_speech_from_mic(recognizer, mic):
    """Listens to microphone input and recognizes speech, with noise filtering."""
    with mic as source:
        recognizer.adjust_for_ambient_noise(source, duration=1)  # Adjust for ambient noise
        print("Listening...")  # Prompt user to speak
        audio = recognizer.listen(source)  # Capture audio input

    try:
        # Use Google's speech recognition to convert audio to text
        speech_text = recognizer.recognize_google(audio)
        print(f"You said: {speech_text}")  # Output the recognized text
        return speech_text  # Return the recognized text
    except sr.UnknownValueError:
        print("Sorry, I could not understand the audio.")  # Handle unrecognized speech
        return None  # Return None if speech is not understood
    except sr.RequestError:
        print("Could not request results from Google Speech Recognition service.")  # Handle request errors
        return None  # Return None in case of a request error


def generate_response(user_input):
    """Generates a response from the Gemma model based on user input."""
    try:
        # Use TinyLLaMA instead of the previous model
        result = subprocess.run(
            ["ollama", "run", "gemma:2b", user_input],  # Adjust command for Gemma
            capture_output=True,
            text=True
        )
        print("Raw output:", result.stdout)  # Print raw output to see the model's response
        print("Error output:", result.stderr)  # Print any errors
        return result.stdout.strip()  # Return the response text
    except Exception as e:
        print(f"Error generating response: {e}")
        return "I'm having trouble responding right now."


def main():
    """Main function to run the AI desk pet."""
    recognizer = sr.Recognizer()  # Create a speech recognizer instance
    mic = sr.Microphone()  # Create a microphone instance
    tts_engine = initialize_tts_engine()  # Initialize the TTS engine

    while True:
        spoken_text = recognize_speech_from_mic(recognizer, mic)  # Recognize speech from the mic
        if spoken_text is None:  # Check if recognition failed
            continue  # Skip to the next iteration if there's an issue

        if spoken_text.lower() == "exit":  # Check for exit command
            speak(tts_engine, "Goodbye!")  # Speak goodbye
            break  # Exit the loop

        response = generate_response(spoken_text)  # Generate a response based on user input
        print(f"AI: {response}")  # Print the AI's response
        speak(tts_engine, response)  # Speak the AI's response

if __name__ == "__main__":
    main()  # Run the main function
