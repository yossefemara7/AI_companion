# To start the ollama server
#ollama serve

# To use a venv, use the following (search it up beforehand)
# cd into the main project folder
#python -m venv venv/
#source venv/bin/activate

# The following installing requirements into the venv
#pip install -r requirements.txt

# The following deactivates the venv
#deactivate

from ollama import chat

messages = [
  {
    'role': 'user',
    'content': 'When was microsoft founded?',
  },
]

# Ensure you change "Phi3" to the name of the model you picked on creation
for part in chat('Phi3', messages=messages, stream=True):
  print(part['message']['content'], end='', flush=True)

# end with a newline
print()
