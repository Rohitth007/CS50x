# Recognize a voice using Google Speech Recognition
# https://pypi.org/project/SpeechRecognition/
# Error? : https://stackoverflow.com/questions/7088672/pyaudio-working-but-spits-out-error-messages-each-time
import speech_recognition

# Obtain audio from microphone
recognizer = speech_recognition.Recognizer()
with speech_recognition.Microphone() as source:
    print("Say something!!")
    audio = recognizer.listen(source)

# Recog using Google's speech Recognition
print("Google Speech Recognition thinks you said:")
print(recognizer.recognize_google(audio))
