
# 🩸 O⁻ (O-Negative)
**The universal donor of game mod loaders.**

O⁻ is a cross-game, cross-DLL proxy mod injection system that works by masquerading as system DLLs like `dinput8.dll`, `xinput1_4.dll`, or `dxgi.dll`. Compatible with hundreds of Windows games.

## 💡 Why O⁻?
Just like the universal blood donor, O⁻ is accepted everywhere — by any game that loads a system DLL. It injects your mods silently, reliably, and without modifying the game's files.

## 🚀 Features
- Supports multiple entrypoint DLLs
- Shared logging and mod loading core
- Transparent DLL proxying (calls forwarded to system DLL)
- Designed for injectionless modding

## 🔧 Usage
1. Build or download the generated DLL (e.g., `dinput8.dll` or another variant).
2. Place it in the game's executable directory.
3. Launch the game — your mod code runs automatically.
4. Check `o-neg.log` for debug info.

## 🧪 Supported Entry Points
- `dinput8.dll`
- `xinput1_4.dll`
- `dxgi.dll` (coming soon)

## 📁 License
MIT

---
Made with blood, sweat, and breakpoints. 🩸
