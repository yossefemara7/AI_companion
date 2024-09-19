# To start the ollama server
#ollama serve

# Make sure to check the README to download requirements, use a venv (optional),
# and import models

from ollama import chat

messages = [
  {
    'role': 'user',
    'content': 'When was microsoft founded?',
  },
]

# Ensure you change "Phi3" to the name of the model you picked on creation
# This code simply keeps showing the words the AI prints continuously
# instead of once at the end
for part in chat('Phi3', messages=messages, stream=True):
  print(part['message']['content'], end='', flush=True)

# end with a newline
print()
