# C0PP3R — Educational Visual Demo (Safe / Non‑Destructive)

WARNING: This repository contains code originally associated with a destructive proof‑of‑concept. This README, and the repository's Safety/ folder, are intended to provide a safe, non‑destructive educational demonstration only. Do NOT run any code from the Destructive/ folder. The project owner and contributors do not endorse or support using any code here for harming systems, disabling security, or making systems unbootable.

Purpose
- Provide a harmless visual demonstration inspired by a historical "GDI" visual payload for education and research.
- Help learners study Windows GDI programming, program structure, and secure handling of system resources — without destructive or privileged actions.

High‑level goals
- Keep all examples safe to run on a developer workstation or Windows virtual machine.
- Explicitly separate any destructive code from harmless demos and mark destructive code as off‑limits.
- Provide guidance on safe analysis practices and legal/ethical considerations.

What is safe to run
- C0PP3R/Safety/Source/C0PP3R.harmless.cpp — a non‑destructive program that repeatedly reads the screen via GDI and applies a simple pixel transformation for a visual effect. It performs only user‑mode GDI operations and does not modify disk, boot records, or system policies.

What you must NOT run or re‑enable
- C0PP3R/Destructive/Source/C0PP3R.cpp
- C0PP3R/Destructive/Source/bootrec.h
These files contain destructive behavior (for example, writing to raw physical disks and modifying system policies). They are intentionally separated and must never be executed. The repository is provided for study only; any destructive code is preserved for archival/analysis visibility and MUST remain inert.

Quick start — build and run the harmless demo
1. Recommended: run inside an isolated Windows VM (snapshot beforehand).
2. Build with Visual Studio (Developer Command Prompt):
   - Open a Developer Command Prompt for VS.
   - Compile:
     cl /EHsc "C0PP3R\Safety\Source\C0PP3R.harmless.cpp" /link user32.lib gdi32.lib
   - Run the produced exe from the VM.
3. Or build with MinGW-w64 (MSYS2 / mingw-w64):
   - Example:
     g++ -std=c++17 -O2 -mwindows "C0PP3R/Safety/Source/C0PP3R.harmless.cpp" -lgdi32 -luser32 -o C0PP3R_harmless.exe
   - Run in a sandboxed VM.

Notes about the harmless demo
- The program demonstrates:
  - Acquiring a screen DC and creating a DIB section.
  - Copying the screen to an in‑memory bitmap (BitBlt).
  - Applying a deterministic per‑pixel transformation to produce a visual glitch effect.
- It intentionally avoids any privileged operations, registry writes, raw device access, or any actions that could alter disk/firmware/boot state.

Repository layout (relevant)
- C0PP3R/
  - Safety/Source/
    - C0PP3R.harmless.cpp   — Safe demo (recommended to build/run)
  - Destructive/Source/
    - C0PP3R.cpp            — Destructive sample (DO NOT RUN)
    - bootrec.h             — Contains raw MBR bytes (DO NOT USE)
  - README.md               — This file

Security, legal & ethical guidance
- Always follow applicable laws, institutional policies, and ethical guidelines.
- Do not use code from this repository to damage, impair, or gain unauthorized access to systems.
- If you are studying malware or destructive code for research, do so only in properly authorized, isolated lab environments with explicit permission and appropriate safeguards.
- The maintainers disclaim liability for misuse of any code in this repository.

Safe research recommendations
- Use an air‑gapped VM or an isolated virtual network.
- Take and keep snapshots before running experimental code.
- Use least privilege — do not run potentially dangerous samples as Administrator unless absolutely necessary for a controlled experiment you are authorized to perform.
- Prefer static analysis and disassembly in place of execution if your goal is to understand behavior.

Contributing
- Contributions are welcome only if they improve safety, documentation, or the harmless demo.
- Do not submit pull requests that add or re‑enable destructive functionality.
- If you discover any unsafe or questionable content, open an issue and tag it "safety" so maintainers can review and remediate.

License & disclaimer
- This repository is provided for educational and research purposes only. By using it you agree not to use it for harmful or unlawful activities.
- No warranty is provided. The authors and maintainers are not responsible for any damage resulting from misuse.

If you want help
- I can:
  - Review and improve the harmless demo (code quality, resource handling).
  - Help convert destructive examples into safe simulations (visuals, UI flow).
  - Provide high‑level resources about malware analysis, defensive techniques, and safe lab setup.

If you'd like a README variant focused on packaging the harmless demo into a friendly example (with build scripts or an annotated walkthrough), say which target build system (Visual Studio, MSYS2/mingw, or CMake) you prefer and I’ll prepare that next.