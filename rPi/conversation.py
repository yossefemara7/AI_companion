from langchain_ollama import OllamaLLM
from langchain_core.prompts import ChatPromptTemplate
from conversation_utils.mic import WhisperMic
import ollama
from time import sleep
import tts

from conversation_utils.utils import \
    load_conversation, contains_greetings, print_and_say, update_conversation, check_empty, parser, \
    extract_encoding_from_movement_array, compile_instructions

from conversation_utils.constants import \
    CONTEXT_FILE_NAME, STARTING_PROMPT, TEMPLATE_PROMPT, TTS_MODEL_NAME, EMOTIONS_DICT, GREETINGS, RED, RESET

prompt = ChatPromptTemplate.from_template(TEMPLATE_PROMPT)
model = OllamaLLM(model="llama3.1", gpu=1)
chain = prompt | model

_context = []

def handle_exit(text: str, context: list[dict[str, str]]) -> bool:
    """
    A simple function to handle the behavior for exiting the conversation.
    
    Args:
        text (str): The text that the user has said.
    
    Returns:
        True if the user wants to exit, False otherwise
    """
    
    if text.strip().lower() == "exit.":
        update_conversation(CONTEXT_FILE_NAME, context)
        print_and_say("Exitting")
        tts.close_text_to_speech()
        return True
    
    return False

def handle_continuous_conversation(text: str, context: list[dict[str, str]]) -> str:
    """
    A function that handles the behavior for the continuous conversation between the user and the LLM.
    
    Args:
        text (str): The text that the user has said.
        context (list[dict[str, str]]): The conversation
    
    Returns:
        The updated context after the conversation.
    """
    context.append({"role": "user", "content": str(text)})
    
    result = ollama.chat("llama3.1", context)
            
    print(f"{RED}Bot: {result}{RESET}")
    
    current_emotion, movement_array, sentence = compile_instructions(result["message"]["content"])
    
    print(f'Emotion Encoding: {int(current_emotion)}')
    print(f'Movement Array Encoding: {movement_array}')
    print(f'Sentence : {sentence}')
    
    tts.say(sentence, TTS_MODEL_NAME)

    # Additional code related to communicating with the Arduino Nano
    # for sending emotion and the movemeent array should go around here
    # or in a separate function.
    
    context.append(result["message"])
    
    return f"\n{str(text)} \nAI: {str(sentence)}"

def handle_conversation_speech_mode():
    """
    A new implementation for handling conversational interactions with the user and the LLM using speech.
    """
    global _context
    
    tts.initiate_text_to_speech()
    
    if check_empty(CONTEXT_FILE_NAME):
        _context.append({"role": "system", "content": str(STARTING_PROMPT)})
    else:
        _context = load_conversation(CONTEXT_FILE_NAME)
    
    # Initialize the WhisperMic object for listening to the user's speech.
    mic = WhisperMic(model="small", pause=1.23)
    
    # We want to make sure that the user has greeted the companion before starting the conversation.
    conversation_initiated = False
    
    print_and_say("Welcome! Start the conversation by greeting your companion! Or if you want to exit early, simply say 'exit.'")
    
    while True:
        text = mic.listen()
        
        if text is None:
            continue
        
        if handle_exit(text, _context):
            return
        
        print("User said:", text)
        
        if not conversation_initiated:
            speaking = contains_greetings(text.strip().lower())
            if speaking:
                conversation_initiated = True
                print_and_say("Hey there! How can I help you today?")
            else:
                print_and_say("Please initiate the conversation with a greeting.")
        else:
            handle_continuous_conversation(text, _context)

if __name__ == "__main__":
    handle_conversation_speech_mode()
