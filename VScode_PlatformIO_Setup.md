# Setting up VS Code + PlatformIO for ESP32 projects

General setup guide for building and flashing any of the ESP32 device folders in this repo (Study Light, and any future room/device added under `Lights/`, etc.). Do this once per computer.

## 1. Install VS Code

Download and install [Visual Studio Code](https://code.visualstudio.com/).

## 2. Install the PlatformIO IDE extension

1. Open VS Code.
2. Go to the Extensions panel (icon on the left sidebar, or `Ctrl+Shift+X`).
3. Search for **PlatformIO IDE** and install it (published by PlatformIO).
4. Wait for it to finish installing — VS Code will prompt a reload. Let it reload.
5. A new alien-head icon appears in the left sidebar — that's the PlatformIO Home button.

This first install also downloads PlatformIO's own core tools in the background; give it a few minutes on first run.

## 3. Open a project

- **File → Open Folder…** and select the specific device folder (e.g. `Lights/Study_light`), not the whole repo — each device folder is its own PlatformIO project with its own `platformio.ini`.
- On opening, PlatformIO reads `platformio.ini` and will silently start fetching the board's platform (e.g. `espressif32`) and any libraries listed under `lib_deps` the first time. This is why includes like `WiFi.h` or a library's header can show up red/unresolved right after opening — it hasn't downloaded them yet.

## 4. The PlatformIO toolbar

Bottom status bar (blue), left side, in order:

| Icon | Action |
|---|---|
| House | PlatformIO Home |
| ✓ (checkmark) | **Build** — compiles the project |
| → (right arrow) | **Upload** — flashes the compiled firmware to the board |
| 🗑 (trash) | Clean |
| 🔌 (plug) | **Serial Monitor** — opens a terminal streaming the board's Serial output |

Typical workflow: **Build → Upload → Serial Monitor**, in that order, each time you change code.

## 5. First build

Click the checkmark (✓). Watch the terminal panel that opens at the bottom. The first build for a new board/platform can take several minutes since it's downloading the toolchain. A successful build ends with `[SUCCESS]`.

If the build fails on a specific library's internals (not your own code), check that library's `README` for compiler requirements — for example, the SinricPro library needs C++17, which the ESP32 Arduino core doesn't default to (see the Study Light README for the exact `platformio.ini` flags that fixes this).

## 6. Uploading — drivers and ports

The ESP32 connects over USB as a virtual serial (COM) port, via a USB-to-serial chip on the board (commonly **CP2102** or **CH340** — check the small chip near the USB connector).

1. Plug the ESP32 in with a **data-capable** USB cable (many cheap cables are charge-only).
2. Open Windows **Device Manager** → **Ports (COM & LPT)**. Unplug/replug and see if a COM port appears.
   - If nothing appears, or it shows as an unknown device: install the matching CP2102 or CH340 USB driver, then replug.
3. Click the right-arrow (→) to upload. PlatformIO usually auto-detects the port.
4. If it fails with `Please specify upload_port` or `Looking for upload port... Error`, add the port explicitly to that project's `platformio.ini`:
   ```ini
   upload_port = COM5
   ```
   (use whatever port Device Manager showed).
5. If the port is correct but upload still fails at the same point, some boards need the **BOOT** button held the moment the terminal says `Connecting.....`, released once it starts uploading.

## 7. Per-project setup checklist

Each device folder that needs credentials will have an `include/secrets.example.h` — copy it to `include/secrets.h` in that same folder and fill in real values. `secrets.h` is gitignored at the repo root, so it's never committed; see the main repo README (or ask in chat) for what else is and isn't pushed to GitHub.

## Common issues

| Symptom | Likely cause |
|---|---|
| Red squiggles under `#include <...>` right after opening | Platform/libraries haven't finished downloading yet — build once, then re-check. |
| Build fails inside a library's own files, not yours | Missing compiler flag the library needs (check its docs) — add via `build_flags`/`build_unflags`. |
| `Please specify upload_port` | No COM port detected — check Device Manager, drivers, and cable. |
| `Brownout detector was triggered` in Serial Monitor | Power problem, not a code problem — check the power supply and any wiring feeding extra current draw (e.g. a relay coil) back through the ESP32's rails. |