import sys
import io
sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')

from vosk import Model, KaldiRecognizer
import sounddevice as sd
import json

model_path = "D:/vosk-model-small-en-us-0.15" 
model = Model(model_path)

samplerate = 16000
duration = 5  # seconds

def listen():
    print("Listening...")
    rec = KaldiRecognizer(model, samplerate)
    recording = sd.rec(int(duration * samplerate), samplerate=samplerate, channels=1, dtype='int16')
    sd.wait()
    if rec.AcceptWaveform(recording.tobytes()):
        result = json.loads(rec.Result())
        text = result.get("text", "")
        return text
    else:
        partial = json.loads(rec.PartialResult())
        return partial.get("partial", "")

if __name__ == "__main__":
    text = listen()
    print(text)
