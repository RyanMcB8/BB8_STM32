import cv2
import asyncio
from bleak import BleakClient
import time
import pygame
import os
import urllib.request
import mediapipe as mp
from mediapipe.tasks import python
from mediapipe.tasks.python import vision

os.environ['SDL_AUDIODRIVER'] = 'alsa'
os.environ['AUDIODEV'] = 'hw:2,0'

# ─── BLE CONFIG ───────────────────────────────────────────────
BLE_ADDRESS = "XX:XX:XX:XX:XX:XX"
BLE_CHAR_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"

# ─── SOUND FILES ──────────────────────────────────────────────
SOUNDS = {
    "face_detected":  "sounds/face_detected.wav",
    "idle_loop":      "sounds/idle_loop.wav",
    "thumbs_up":      "sounds/thumbs_up.wav",
    "thumbs_down":    "sounds/thumbs_down.wav",
}

# ─── FRAME CONFIG ─────────────────────────────────────────────
FRAME_WIDTH = 320
FRAME_HEIGHT = 240
FRAME_CENTRE_X = FRAME_WIDTH // 2
FRAME_CENTRE_Y = FRAME_HEIGHT // 2

# ─── DOWNLOAD MODELS ──────────────────────────────────────────
def download_model(url, path):
    if not os.path.exists(path):
        print(f"Downloading {path}...")
        urllib.request.urlretrieve(url, path)
        print("Downloaded!")

download_model(
    'https://storage.googleapis.com/mediapipe-models/face_landmarker/face_landmarker/float16/1/face_landmarker.task',
    'face_landmarker.task'
)
download_model(
    'https://storage.googleapis.com/mediapipe-models/gesture_recognizer/gesture_recognizer/float16/1/gesture_recognizer.task',
    'gesture_recognizer.task'
)

# ─── SETUP MEDIAPIPE ──────────────────────────────────────────
face_landmarker = vision.FaceLandmarker.create_from_options(
    vision.FaceLandmarkerOptions(
        base_options=python.BaseOptions(model_asset_path='face_landmarker.task'),
        output_face_blendshapes=False,
        num_faces=1
    )
)

gesture_recognizer = vision.GestureRecognizer.create_from_options(
    vision.GestureRecognizerOptions(
        base_options=python.BaseOptions(model_asset_path='gesture_recognizer.task')
    )
)

# ─── STATE ────────────────────────────────────────────────────
last_gesture = None
gesture_cooldown = 0
COOLDOWN_FRAMES = 40
was_face_detected = False
face_absent_frames = 0
FACE_ABSENT_THRESHOLD = 30
ble_client = None

# ─── PYGAME AUDIO ─────────────────────────────────────────────
pygame.mixer.init(frequency=44100, size=-16, channels=2, buffer=4096)

def play_sound_once(key):
    path = SOUNDS.get(key)
    if path and os.path.exists(path):
        if not pygame.mixer.get_busy():
            sound = pygame.mixer.Sound(path)
            sound.play()

def start_idle_loop():
    path = SOUNDS.get("idle_loop")
    if path and os.path.exists(path):
        pygame.mixer.music.load(path)
        pygame.mixer.music.play(-1)

def stop_loop():
    pygame.mixer.music.stop()

async def ble_connect():
    global ble_client
    try:
        ble_client = BleakClient(BLE_ADDRESS)
        await ble_client.connect()
        print("BLE connected to STM32")
    except Exception as e:
        print(f"BLE connection failed: {e}")
        ble_client = None

async def send_coordinates(offset_x, offset_y):
    if ble_client and ble_client.is_connected:
        try:
            data = f"{offset_x},{offset_y}\n".encode()
            await ble_client.write_gatt_char(BLE_CHAR_UUID, data)
        except Exception as e:
            print(f"BLE send failed: {e}")

async def main():
    global gesture_cooldown, last_gesture, was_face_detected
    global face_absent_frames

    await ble_connect()
    start_idle_loop()

    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, FRAME_WIDTH)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT)

    prev_time = time.time()
    print("BB8 Vision started...")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        if gesture_cooldown > 0:
            gesture_cooldown -= 1

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        mp_image = mp.Image(image_format=mp.ImageFormat.SRGB, data=rgb)

        # ── FACE DETECTION ────────────────────────────────────
        face_results = face_landmarker.detect(mp_image)
        face_detected = False
        offset_x, offset_y = 0, 0

        if face_results.face_landmarks:
            face_detected = True
            landmarks = face_results.face_landmarks[0]
            face_x = int(landmarks[1].x * FRAME_WIDTH)
            face_y = int(landmarks[1].y * FRAME_HEIGHT)
            offset_x = face_x - FRAME_CENTRE_X
            offset_y = face_y - FRAME_CENTRE_Y
            await send_coordinates(offset_x, offset_y)
            print(f"Face | Offset X:{offset_x:+d} Y:{offset_y:+d}")

        # ── FACE SOUND LOGIC WITH COOLDOWN ────────────────────
        if face_detected:
            face_absent_frames = 0
            if not was_face_detected:
                stop_loop()
                play_sound_once("face_detected")
                print("Face detected")
        else:
            face_absent_frames += 1
            if face_absent_frames >= FACE_ABSENT_THRESHOLD and was_face_detected:
                start_idle_loop()
                print("No face — idle")

        was_face_detected = face_detected if face_detected else (face_absent_frames < FACE_ABSENT_THRESHOLD)

        # ── GESTURE DETECTION (only if face detected) ─────────
        if face_detected:
            gesture_results = gesture_recognizer.recognize(mp_image)

            if gesture_results.gestures and gesture_results.hand_landmarks:
                current_gesture = gesture_results.gestures[0][0].category_name
                confidence = gesture_results.gestures[0][0].score

                if confidence > 0.7 and gesture_cooldown == 0:
                    if current_gesture == 'Thumb_Up' and last_gesture != 'Thumb_Up':
                        last_gesture = 'Thumb_Up'
                        gesture_cooldown = COOLDOWN_FRAMES
                        play_sound_once("thumbs_up")
                        print("Gesture: THUMBS UP")
                    elif current_gesture == 'Thumb_Down' and last_gesture != 'Thumb_Down':
                        last_gesture = 'Thumb_Down'
                        gesture_cooldown = COOLDOWN_FRAMES
                        play_sound_once("thumbs_down")
                        print("Gesture: THUMBS DOWN")
            else:
                last_gesture = None

        # ── FPS ───────────────────────────────────────────────
        fps = 1 / (time.time() - prev_time)
        prev_time = time.time()

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    pygame.mixer.quit()
    if ble_client:
        await ble_client.disconnect()

asyncio.run(main())