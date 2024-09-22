from langchain_ollama import OllamaLLM
from langchain_core.prompts import ChatPromptTemplate
from whisper_mic import WhisperMic
import tts

from conversation_utils.utils import \
    load_conversation, has_common_element, print_and_say, update_conversation, check_empty, parser, \
    extract_encoding_from_movement_array

from conversation_utils.constants import \
    CONTEXT_FILE_NAME, STARTING_PROMPT, TEMPLATE_PROMPT, TTS_MODEL_NAME, EMOTIONS_DICT, GREETINGS, RED, RESET

prompt = ChatPromptTemplate.from_template(TEMPLATE_PROMPT)
model = OllamaLLM(model="llama3.1", gpu=1)
chain = prompt | model

def handle_exit(text: str, context: str) -> bool:
    """
    A simple function to handle the behavior for exiting the conversation.
    
    Args:
        text (str): The text that the user has said.
    
    Returns:
        True if the user wants to exit, False otherwise
    """
    
    if text.strip().lower() == "exit.":
        update_conversation(CONTEXT_FILE_NAME, str(context))
        print_and_say("Exitting")
        tts.close_text_to_speech()
        return True
    
    return False

def handle_continuous_conversation(text: str, context: str) -> str:
    """
    A function that handles the behavior for the continuous conversation between the user and the LLM.
    
    Args:
        text (str): The text that the user has said.
        context (str): The current context of the conversation.
    
    Returns:
        The updated context after the conversation.
    """
    
    result = chain.invoke({"context": context, "question": text})
            
    print(f"{RED}Bot: {result}{RESET}")
    
    instructions = parser(result)
    print(instructions)
    
    current_emotion = EMOTIONS_DICT[instructions["Emotion"]]
    movement_array = extract_encoding_from_movement_array(instructions["Movement"])
    sentence = instructions["Sentence"]
    
    print(f'Emotion Encoding: {int(current_emotion)}')
    print(f'Movement Array Encoding: {movement_array}')
    print(f'Sentence : {sentence}')
    
    tts.say(sentence, TTS_MODEL_NAME)

    # Additional code related to communicating with the Arduino Nano
    # for sending emotion and the movemeent array should go around here
    # or in a separate function.
    
    return context + f"\n{str(text)} \nAI: {str(sentence)}"

def handle_conversation_speech_mode():
    """
    A new implementation for handling conversational interactions with the user and the LLM using speech.
    """
    
    tts.initiate_text_to_speech()
    
    if check_empty(CONTEXT_FILE_NAME):
        context = str(STARTING_PROMPT)
    else:
        context = str(load_conversation(CONTEXT_FILE_NAME))
    
    # Initialize the WhisperMic object for listening to the user's speech.
    mic = WhisperMic(model="small", pause=0.88)
    
    # We want to make sure that the user has greeted the companion before starting the conversation.
    conversation_initiated = False
    
    print_and_say("Welcome! Start the conversation by greeting your companion! Or if you want to exit early, simply say 'exit.'")
    
    while True:
        text = mic.listen()
        
        for text in mic.listen_continuously():
            if text is None:
                continue
            
            if handle_exit(text, context):
                return
            
            print("User said:", text)
            
            if not conversation_initiated:
                speaking = has_common_element(text.strip().lower().split(), GREETINGS)
                if speaking:
                    conversation_initiated = True
                    print_and_say("Hey there! How can I help you today?")
                else:
                    print_and_say("Please initiate the conversation with a greeting.")
            else:
                context = handle_continuous_conversation(text, context)

if __name__ == "__main__":
    handle_conversation_speech_mode()
