<div align="center">
 <img alt="ollama" height="200px" src="https://github.com/yossefemara7/AI_companion/Assets/CHANGE_TO_ICON_LINK_WHEN_UPLOADED">
</div>
[IEEE at the University of Minnesota](https://ieee.umn.edu/)

# Introduction

IEEE at the University of Minnesota has been the official student branch of IEEE on campus since 1916, with focuses on research standards, professional networking, technical innovation, and sustainable development.

## AI Companion project

This AI companion project harnesses the power of local LLMs and computer vision to navigate and explore the world through this workshop based event. This project was designed for software and hardware enthusiasts alike, offering an exciting opportunity to dive into innovative technologies. If you're passionate about AI, computer vision, or robotics, this is your chance to get involved!

# Ollama

Steps and Commands

### Install Ollama
https://github.com/ollama/ollama

### To start the ollama server
```
ollama serve
```

### To import a model (change "example" to any name you want)
```
ollama create example -f Python/models/Modelfile
ollama run example
```

### Ollama Python Resource
https://github.com/ollama/ollama-python

# Python

Python Information

### This installs requirements on your device (or into the venv if made and activated)
```
pip install -r requirements.txt
```

## Venv

This is like a virtual machine for installing for python

### To use a venv, use the following (search it up beforehand)
```
cd into the main project folder
#python -m venv venv/
#source venv/bin/activate
```

### The following deactivates the venv
```
deactivate
```