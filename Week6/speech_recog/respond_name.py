import speech_recognition
import re

# Obtain audio from microphone
recognizer = speech_recognition.Recognizer()
with speech_recognition.Microphone() as source:
    print("Say something!!")
    audio = recognizer.listen(source)

# Recog using Google's speech Recognition
words = recognizer.recognize_google(audio)

# Respond to speech
matches = re.search("my name is (.*)", words)
if matches:
    print(f"Hey, {matches[1]}")
else:
    print("Hey, you.")
