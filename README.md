# Mobile UI Example

A fully custom, Eclipse-inspired mod menu for Geometry Dash, built from scratch with Geode. Built against:

- Geode: `5.3.0`
- GD (Android): `2.2081`

## What it does

A small floating button appears on screen persistently — main menu, level list, in a level, everywhere — inspired by the "Eclipse" mod's floating menu button. It's draggable to reposition, and tapping it (without dragging) opens a custom panel:

- **Draggable window** — drag by the header to move it anywhere on screen
- **Tabs** — General / Display / About, with a sliding accent-color indicator and a pop-in animation when switching
- **Custom animated toggles** — no checkmarks; the track cross-fades from grey to the accent color while the knob slides across, with a little squash-and-settle on release
- **Premium buttons** — press/release scale animation plus a brief color flash on tap
- **Fully custom art** — every panel, button, toggle, and the floating button itself use hand-generated textures (baked-in soft shadows, subtle gradients, rounded corners) instead of any stock GD popup sprite

## Project layout

```
mobile-ui-example/
├── mod.json                        # Metadata + custom spritesheet registration
├── CMakeLists.txt                   # CMake build config
├── about.md                         # In-game "About" page
├── resources/                       # Custom-generated art (panel, knob, track, FAB, icons)
├── src/
│   ├── Theme.hpp                    # Shared design tokens: colors, spacing, animation timing
│   ├── PremiumButton.hpp            # Button + panel-sprite factory, tap-flash helper
│   ├── PremiumToggle.hpp/.cpp       # Custom animated toggle switch
│   ├── DraggablePanel.hpp/.cpp      # The main window: header, tabs, content, animations
│   ├── FloatingButton.hpp/.cpp      # The persistent, draggable Eclipse-style FAB
│   └── main.cpp                     # Hooks MenuLayer once to spawn the persistent FAB
└── .github/workflows/build.yml      # CI: builds Android32 + Android64, packages a .geode file
```

## How persistence works

The floating button is created once (guarded by a static flag) the first time `MenuLayer::init` runs, then registered with Geode's `SceneManager::keepAcrossScenes`, which keeps it alive and attached across every subsequent scene change — so it follows you from the main menu into levels, the editor, etc.

## A couple of deliberate simplifications

Geometry Dash's rendering stack (an older cocos2d-x 2.x fork) doesn't support live shader-based blur or runtime gradients, so:
- Shadows and gradients are **baked into the generated PNGs** (`resources/*.png`) rather than computed at runtime — this is what actually gives the "soft shadow / subtle gradient" look.
- The toggle's fill animation is a **color cross-fade + sliding knob**, not a literal left-to-right wipe mask (true clip-mask wipes are possible but meaningfully riskier in this engine for the payoff).

## Building locally

Requires the [Geode CLI](https://docs.geode-sdk.org/getting-started/) and SDK installed, with `GEODE_SDK` set in your environment.

```bash
geode build
```

## Building with GitHub Actions

Push this repo to GitHub and the included workflow builds Android32 + Android64 on every push, combining them into a single `.geode` file available as a workflow artifact under **Actions → (latest run) → Build Output**.

Yes
