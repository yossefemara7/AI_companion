import os
import re
from tts import say
from .constants import EMOTIONS_DICT, TTS_MODEL_NAME, MOVEMENT_DICT, GREETINGS
import json

def load_conversation(txt_name: str) -> list[dict[str, str]]:
    """
    A simple function for extracting the conversation history from the provided text file.
    
    Args:
        txt_name (str): The name of the text file to extract the conversation history from.
    
    Returns:
        The conversation history.
    """
    
    with open(txt_name, 'r') as file:
        content = file.read()
    
    # parse the content as JSON
    content = json.loads(content)
    
    return content

def has_common_element(arr1: list, arr2: list) -> bool:
    """
    A simple function for checking if two arrays have a common element.
    
    Args:
        arr1 (list): The first array to check.
        arr2 (list): The second array to check.
    
    Returns:
        True if the arrays have a common element, False otherwise
    """
    
    set1 = set(arr1)    
    for elem in arr2:
        if elem in set1:
            return True
    
    return False

def contains_greetings(text: str) -> bool:
    """
    A simple function to check if the provided text contains any greetings.
    
    Args:
        text (str): The text to check for greetings.
    
    Returns:
        True if the text contains any greetings, False otherwise.
    """
    
    for greeting in GREETINGS:
        if greeting in text:
            return True
    
    return False

def print_and_say(*args, sep=" ", tts_model_name=TTS_MODEL_NAME):
    """
    A wrapper function that allows the user to print and say the provided text.
    
    Args:
        *args: The text to print and say.
        sep (str): The separator to use when joining the text.
        tts_model_name (str): The name of the TTS model to use.
    """
    
    print(*args)
    say(sep.join(args), tts_model_name)

def update_conversation(txt_name: str, context: list[dict[str, str]]):
    """
    A simple function to update the conversation history.
    
    Args:
        txt_name (str): The name of the text file to update.
        context (list[dict[str, str]]): The context to update the conversation history with.
    """
    
    with open(txt_name, 'w') as file:
        # Write the context as a json string
        file.write(json.dumps(context))

def delete_conversation(txt_name: str):
    """
    A simple function to delete the conversation history.
    
    Args:
        txt_name (str): The name of the text file to delete
    """
    
    with open(txt_name, 'w') as file:
        pass

def check_empty(filename: str) -> bool:
    """
    A simple function to quickly check whether or not the file 
    we're looking for is empty.
    
    Args:
        filename (str): The name of the file to check.
    
    Returns:
        True if the file is empty, False otherwise.
    """
    
    # If the path doesn't exist, then it must be empty.
    if os.path.exists(filename) is False:
        return True
    
    with open(filename, 'r') as file:
        content = file.read()
    
    return not content

def parser(text: str) -> tuple[dict[str, any], bool]:
    """
    A helper function to parse the emotion, movement, and sentences from the provided text.
    
    Args:
        text (str): The text to parse.
    
    Returns:
        A dictionary containing the parsed information.
        A boolean indicating whether or not the parsing was successful.
    """
    
    # Regular expressions to extract each part
    emotion_match = re.search(r'Emotion:\s*([^|]+)', text)
    movement_array_match = re.search(r'Movement Array:\s*(\[[^\]]+\])', text)
    sentence_match = re.search(r'Sentence:\s*(.+)$', text)

    # Extracted values
    emotion = emotion_match.group(1).strip() if emotion_match else None
    movement_array = movement_array_match.group(1).strip() if movement_array_match else None
    sentence = sentence_match.group(1).strip() if sentence_match else None
    
    # Extra processing on movement_array
    if movement_array is not None:
        movement_array = movement_array     \
            .replace("\"", "")              \
            .replace("[", "")               \
            .replace("]", "")               \
            .split(",")                     
        
        movement_array = list(map(lambda x: x.strip(), movement_array))
    
    info = {
        'Emotion' : emotion, 
        "Movement" : movement_array, 
        "Sentence" : sentence,
    }
    
    # If all are None, then the parsing was unsuccessful
    success = emotion        is not None or        \
              movement_array is not None or        \
              sentence       is not None
    
    return info, success

def compile_instructions(text: str) -> tuple[int, list[int], str]:
    """
    A helper function to compile the instructions from the provided text.
    
    Args:
        text (str): The text to compile the instructions from.
    
    Returns:
        A tuple containing the emotion encoding, the movement array encoding, and the sentence.
    """
    
    instructions, success = parser(text)
    
    if success is False:
        return 0, [3], text
    
    if not instructions["Emotion"] in EMOTIONS_DICT:
        current_emotion = 0
    else:
        current_emotion = EMOTIONS_DICT[instructions["Emotion"]]
        
    movement_array = extract_encoding_from_movement_array(instructions["Movement"])
    
    sentence = instructions["Sentence"] or ""
    
    return current_emotion, movement_array, sentence

def extract_context_from_conversation_history(conversation_history: str) -> list[str]:
    """
    A helper function to extract the context from the provided conversation history.
    
    Args:
        conversation_history (str): The conversation history to extract the context from.
    
    Returns:
        A list of strings representing the context.
    """
    
    return conversation_history.split("\n")

def extract_encoding_from_movement_array(movement: (list[str] | None)) -> list[int]:
    """
    A helper function to extract the encodings from the provided movement array.
    
    Args:
        movement (list[str] | None): The movement array to extract the encodings from.
        
    Returns:
        A list of integers representing the movement encodings.
    """
    movement_array = []
    
    for move in movement or []:
        if move in MOVEMENT_DICT and MOVEMENT_DICT[move] is not None:
            movement_array.append(MOVEMENT_DICT[move])
    
    if len(movement_array) == 0:
        movement_array.append(3)
    
    return movement_array