from langchain_ollama import OllamaLLM
from langchain_core.prompts import ChatPromptTemplate
import os
import re
from tts import say
import tts

template = """
Here is the conversation history: {context}
Question: {question}
Answer: 
"""

# Color codes for terminal printing.
RED = "\033[31m"
RESET = "\033[0m"

prompt = ChatPromptTemplate.from_template(template)
model = OllamaLLM(model="llama3.1")
chain = prompt | model

def load_conversation(txt_name):
    with open(txt_name, 'r') as file:
        content = file.read()
    return content

def has_common_element(arr1, arr2):
    set1 = set(arr1)    
    for elem in arr2:
        if elem in set1:
            return True
    
    return False

def update_conversation(txt_name, context):
    with open(txt_name, 'a') as file:
        file.write(context)

def delete_conversation(txt_name):
    with open(txt_name, 'w') as file:
        pass

def check_empty(filename):
    # If the path doesn't exist, then it must be empty.
    if os.path.exists(filename) is False:
        return True
    
    with open(filename, 'r') as file:
        content = file.read()
    
    return not content

def parser(text):
    # Regular expressions to extract each part
    emotion_match = re.search(r'Emotion:\s*([^|]+)', text)
    movement_array_match = re.search(r'Movement Array:\s*(\[[^\]]+\])', text)
    sentence_match = re.search(r'Sentence:\s*(.+)$', text)

    # Extracted values
    emotion = emotion_match.group(1).strip() if emotion_match else None
    movement_array = movement_array_match.group(1).strip() if movement_array_match else None
    sentence = sentence_match.group(1).strip() if sentence_match else None
    info = {'Emotion' : emotion, "Movement" : movement_array, "Sentence" : sentence}
    return info

def handle_conversation():
    starting_prompt = "Instructions (VERY IMPORTANT): You will be part of my project to make a desk pet, you have to limit your words to one to three sentence(s) only and the format of your output should strictly follow this where emotion is one of the following emotions [Happy, Angry, Sad] and the movement array is an array of movement of Turn Right Turn Left Move Forward None and the array has to look something like this [\"Turn Right\" , \"Move Forward\"] : The final output should look something like this 'Emotion: Happy | Movement Array: [\"Turn Right" , "Move Forward\"] | Sentence: Hello! Im here to be your desk pet companion.', you dont necessarily have to move but only do so when appropriate: YOU MUST UNDER ALL CIRCUMSTANCES FOLLOW THE FORMAT DESCRIBED"
    context_file_name = "context.txt"

    if check_empty(context_file_name):
        context = str(starting_prompt)
    else:
        context = str(load_conversation(context_file_name))

    print("Welcome to the AI chatbot, Type exit to quit or delete to reset.")
    while True:
        user_input = input("Enter Input .... :")
        greetings = ['hello', 'hey', 'companion']
        speaking = has_common_element(user_input.lower().split(), greetings)
        while speaking:
            user_input = input(f"Me: ")
            if user_input.lower() == "exit":
                update_conversation(context_file_name, str(context))
                print("Exitting")
                break
            text = user_input
            print(text)
            print("Model Loading....")
            result = chain.invoke({"context": context, "question": user_input})
            RED = "\033[31m"
            RESET = "\033[0m"

            print(f"{RED}Bot: {result}{RESET}")
            # print()
            # print("Instructions: ")
            instructions = parser(result)
            print(instructions)
            input()
            emotion = instructions["Emotion"]
            movement = instructions["Movement"]
            sentence = instructions["Sentence"]
            emotions_dict = {"Happy" : 0, "Sad" : 1, "Angry" : 2}
            movement_dict = {"Turn Right" : 0, "Turn Left" : 1, "Move Forward" : 2}
            cur_emotion = emotions_dict[emotion]
            movement_array = [movement_dict[i] for i in movement or [] if i in movement_dict and movement_dict[i] is not None]
            if len(movement_array) == 0:
                movement_array.append(3)
            print(f'Emotion Encoding: {int(cur_emotion)}')
            print(f'Movement Array Encoding: {movement_array}')
            print(f'Sentence : {sentence}')
            say(sentence,'en-AU-WilliamNeural')
            context += f"\n{str(user_input)} \nAI: {str(sentence)}"

            #Serial Communication with arduino nano send emotion and movement array

        update_conversation(context_file_name, str(context))
        break

if __name__ == "__main__":
    handle_conversation()
