import tkinter as tk
import tkinter.font as tkfont
import subprocess
import threading

EXE_PATH = "amethyst.exe"

try:
    backend_proc = subprocess.Popen(
        [EXE_PATH],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1
    )
except Exception as e:
    print(f"Failed to start backend: {e}")
    exit(1)

def draw_bubble(canvas, text, y, color, align="left"):
    padding_x = 12
    padding_y = 8
    max_width = 220
    lines = []

    for word in text.split():
        if not lines:
            lines.append(word)
        else:
            if canvas.font.measure(lines[-1] + " " + word) < max_width:
                lines[-1] += " " + word
            else:
                lines.append(word)

    text_width = min(max_width, max(canvas.font.measure(line) for line in lines))
    text_height = len(lines) * 18

    tail_size = 8
    bubble_w = text_width + padding_x * 2
    bubble_h = text_height + padding_y * 2

    canvas_width = canvas.winfo_width() if canvas.winfo_width() > 1 else 360

    if align == "right":
        x0 = canvas_width - bubble_w - 20
        tail_x = x0 + bubble_w
        tail_dir = -1
    else:
        x0 = 20
        tail_x = x0
        tail_dir = 1

    y0 = y
    x1 = x0 + bubble_w
    y1 = y0 + bubble_h

    r = 15

    # Rounded rectangle for bubble
    canvas.create_arc(x0, y0, x0+2*r, y0+2*r, start=90, extent=90, fill=color, outline=color)
    canvas.create_arc(x1-2*r, y0, x1, y0+2*r, start=0, extent=90, fill=color, outline=color)
    canvas.create_arc(x1-2*r, y1-2*r, x1, y1, start=270, extent=90, fill=color, outline=color)
    canvas.create_arc(x0, y1-2*r, x0+2*r, y1, start=180, extent=90, fill=color, outline=color)
    canvas.create_rectangle(x0+r, y0, x1-r, y1, fill=color, outline=color)
    canvas.create_rectangle(x0, y0+r, x1, y1-r, fill=color, outline=color)

    # Tail
    canvas.create_polygon(
        tail_x, y1 - tail_size*0.5,
        tail_x + tail_dir*tail_size, y1 + tail_size,
        tail_x, y1 + tail_size*0.5,
        fill=color, outline=color
    )

    # Draw text inside bubble
    text_y = y0 + padding_y
    for line in lines:
        canvas.create_text((x0 + padding_x, text_y), text=line, anchor="nw", fill="white", font=canvas.font)
        text_y += 18

    return bubble_h + 20

# -----------------------------
# Add message safely
# -----------------------------
last_y = 0
def add_message_safe(text, sender="user"):
    root.after(0, lambda: add_message(text, sender))

def add_message(text, sender="user"):
    global last_y
    color = "#1abc9c" if sender == "user" else "#2c2c2c"
    align = "right" if sender == "user" else "left"
    delta = draw_bubble(chat_canvas, text, last_y, color, align)
    last_y += delta
    chat_canvas.configure(scrollregion=chat_canvas.bbox("all"))
    chat_canvas.yview_moveto(1.0)

# -----------------------------
# Backend response
# -----------------------------
def get_full_response():
    resp = ""
    while True:
        line = backend_proc.stdout.readline()
        if not line or "<END_OF_RESPONSE>" in line:
            break
        resp += line
    return resp.strip()

def send_message(text=None):
    msg = text if text else entry_text.get().strip()
    if not msg:
        return
    entry_text.set("")
    add_message_safe(msg, "user")

    def fetch_response():
        backend_proc.stdin.write(msg + "\n")
        backend_proc.stdin.flush()
        bot_resp = get_full_response()
        add_message_safe(bot_resp, "bot")

    threading.Thread(target=fetch_response, daemon=True).start()

root = tk.Tk()
root.title("Amethyst Chatbot")
root.geometry("360x600")
BG_COLOR = "#121212"
root.configure(bg=BG_COLOR)

header = tk.Label(root, text="Amethyst Chat Assistant",
                  font=("Arial", 16, "bold"), bg="#1abc9c", fg="white", pady=10)
header.pack(fill=tk.X)

# Chat canvas
chat_frame = tk.Frame(root, bg=BG_COLOR)
chat_frame.pack(expand=True, fill=tk.BOTH, padx=10, pady=10)

chat_canvas = tk.Canvas(chat_frame, bg=BG_COLOR, highlightthickness=0)
chat_canvas.font = tkfont.Font(family="Arial", size=12)
chat_canvas.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

# Scroll support
scrollbar = tk.Scrollbar(chat_frame, orient="vertical", command=chat_canvas.yview)
scrollbar.pack_forget()  # hidden
scroll_frame = tk.Frame(chat_canvas, bg=BG_COLOR)
canvas_window = chat_canvas.create_window((0,0), window=scroll_frame, anchor="nw")

def on_frame_configure(event):
    chat_canvas.configure(scrollregion=chat_canvas.bbox("all"))
scroll_frame.bind("<Configure>", on_frame_configure)

def on_mousewheel(event):
    chat_canvas.yview_scroll(int(-1*(event.delta/120)), "units")
chat_canvas.bind_all("<MouseWheel>", on_mousewheel)

input_frame = tk.Frame(root, bg=BG_COLOR)
input_frame.pack(fill=tk.X, padx=10, pady=10)

entry_text = tk.StringVar()
entry = tk.Entry(input_frame, textvariable=entry_text, font=("Arial",12), bg="#2e2e2e",
                 fg="white", insertbackground="white", bd=0, relief="flat")
entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0,5), pady=5, ipady=8)
entry.configure(highlightthickness=1, highlightbackground="#2e2e2e", highlightcolor="#2e2e2e")

send_btn = tk.Button(input_frame, text="➤", bg="#3a3a3a", fg="white", font=("Arial",12),
                     bd=0, relief="flat", width=3, command=send_message)
send_btn.pack(side=tk.LEFT, padx=5, pady=5)
send_btn.bind("<Enter>", lambda e: send_btn.configure(bg="#1abc9c"))
send_btn.bind("<Leave>", lambda e: send_btn.configure(bg="#3a3a3a"))

voice_btn = tk.Button(input_frame, text="🎤", bg="#3a3a3a", fg="white", font=("Arial",12),
                      bd=0, relief="flat", width=3, command=lambda: send_message("voice"))
voice_btn.pack(side=tk.LEFT, padx=5, pady=5)
voice_btn.bind("<Enter>", lambda e: voice_btn.configure(bg="#1abc9c"))
voice_btn.bind("<Leave>", lambda e: voice_btn.configure(bg="#3a3a3a"))

root.bind("<Return>", lambda e: send_message())

try:
    root.mainloop()
finally:
    if backend_proc.poll() is None:
        backend_proc.terminate()
