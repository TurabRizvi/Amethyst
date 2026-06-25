import sys
import requests
import json

API_KEY = "YOUR_GROQ_API_KEY"  # Replace
API_URL = "https://api.groq.ai/v1/chat/completions"
MODEL = "llama-3.1-8b-instant"

def ask_groq(prompt):
    headers = {
        "Authorization": f"Bearer {API_KEY}",
        "Content-Type": "application/json"
    }

    data = {
        "model": MODEL,
        "messages": [
            {"role": "user", "content": prompt}
        ],
        "temperature": 0.7,
        "max_tokens": 512
    }

    r = requests.post(API_URL, headers=headers, json=data, timeout=30)
    r.raise_for_status()
    return r.json()["choices"][0]["message"]["content"]

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("No input provided")
        sys.exit(1)

    user_input = sys.argv[1]
    reply = ask_groq(user_input)
    print(reply)
